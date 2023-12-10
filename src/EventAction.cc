//*/////////////////////////////////////////////////////////////////////////*//
//*//                    G4-DSPS-Detector-Simulation                      //*//
//*/////////////////////////////////////////////////////////////////////////*//
//*//                                                                     //*//
//*// Author:                                                             //*//
//*//   Noah Everett (noah.everett@mines.sdsmt.edu)                       //*//
//*//                                                                     //*//
//*/////////////////////////////////////////////////////////////////////////*//
//*//                                                                     //*//
//*/////////////////////////////////////////////////////////////////////////*//

#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

EventAction::EventAction( RunAction* t_runAction, DetectorConstruction* t_detectorConstruction )
    : m_runAction           ( t_runAction                      ), 
      m_detectorConstruction( t_detectorConstruction           ), 
      m_outputManager       ( t_runAction->get_outputManager() ) {
    G4cout << "EventAction::EventAction()" << G4endl;

    G4RunManager::GetRunManager()->SetPrintProgress( 1 );
}

EventAction::~EventAction() {
}

void EventAction::BeginOfEventAction( const G4Event* t_event ) {
}

void EventAction::EndOfEventAction( const G4Event* t_event ) {
    m_analysisManager = G4AnalysisManager::Instance();

    for( DirectionSensitivePhotoDetector* DSPD : m_detectorConstruction->get_directionSensitivePhotoDetectors() ) {
        PhotoSensorSensitiveDetector* photoSensorSensitiveDetector = DSPD->get_photoSensor()->get_sensitiveDetector();
        PhotoSensorHitsCollection* photoSensorHitCollection = photoSensorSensitiveDetector->get_hitsCollection( t_event );
        G4String photoSensorHitHistogramName = "photoSensor_" + to_string( DSPD->get_photoSensor()->get_sensitiveDetector()->get_ID() );

        for( G4int i = 0; i < photoSensorHitCollection->GetSize(); i++ ) {
            PhotoSensorHit* photoSensorHit = static_cast< PhotoSensorHit* >( photoSensorHitCollection->GetHit( i ) );

            m_outputManager->fill_histogram_2D( photoSensorHitHistogramName                    , 
                                                photoSensorHit->get_hit_position_relative().x(), 
                                                photoSensorHit->get_hit_position_relative().y(), 1 );
            m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_position_absolute", photoSensorHit->get_hit_position_absolute    () );
            m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_position_relative", photoSensorHit->get_hit_position_relative    () );
            m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_position_initial" , photoSensorHit->get_particle_position_initial() );
            m_outputManager->fill_tuple_column_double ( "photoSensor_hits_time"             , photoSensorHit->get_hit_time                 () );
            m_outputManager->fill_tuple_column_string ( "photoSensor_hits_process"          , photoSensorHit->get_hit_process              () );
            m_outputManager->fill_tuple_column_double ( "photoSensor_hits_energy"           , photoSensorHit->get_hit_energy               () );
            m_outputManager->fill_tuple_column_string ( "photoSensor_hits_photoSensorID"    , photoSensorHit->get_photoSensor_name         () );
            m_outputManager->fill_tuple_column        ( "photoSensor_hits" );
        }
    }

    for( Calorimeter* cal : m_detectorConstruction->get_calorimeters() ) {
        G4cout << "cal->get_name(): " << cal->get_name() << G4endl;
        CalorimeterSensitiveDetector* calorimeterSensitiveDetector = cal->get_sensitiveDetector();
        G4cout << "calorimeterSensitiveDetector->get_name(): " << calorimeterSensitiveDetector->get_name() << G4endl;
        CalorimeterHitsCollection* calorimeterHitCollection = calorimeterSensitiveDetector->get_hitsCollection( t_event );

        G4cout << "calorimeterHitCollection->GetSize(): " << calorimeterHitCollection->GetSize() << G4endl;
        for( G4int i = 0; i < calorimeterHitCollection->GetSize(); i++ ) {
            CalorimeterHit* calorimeterHit = static_cast< CalorimeterHit* >( calorimeterHitCollection->GetHit( i ) );

            m_outputManager->fill_tuple_column_3vector( "calorimeter_hits_position_absolute", calorimeterHit->get_hit_position_absolute    () );
            m_outputManager->fill_tuple_column_3vector( "calorimeter_hits_position_relative", calorimeterHit->get_hit_position_relative    () );
            m_outputManager->fill_tuple_column_3vector( "calorimeter_hits_position_initial" , calorimeterHit->get_particle_position_initial() );
            m_outputManager->fill_tuple_column_double ( "calorimeter_hits_time"             , calorimeterHit->get_hit_time                 () );
            m_outputManager->fill_tuple_column_string ( "calorimeter_hits_process"          , calorimeterHit->get_hit_process              () );
            m_outputManager->fill_tuple_column_double ( "calorimeter_hits_energy"           , calorimeterHit->get_hit_energy               () );
            m_outputManager->fill_tuple_column_string ( "calorimeter_hits_calorimeterID"    , calorimeterHit->get_calorimeter_name         () );
            m_outputManager->fill_tuple_column        ( "calorimeter_hits" );
        }   
    }

    G4cout << "EndOfEventAction" << G4endl;
}