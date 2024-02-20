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
    m_outputMessenger = OutputMessenger::get_instance();

    if( m_outputMessenger->get_photoSensor_hits_save() ) {
        for( DirectionSensitivePhotoDetector* DSPD : m_detectorConstruction->get_directionSensitivePhotoDetectors() ) {
            PhotoSensorSensitiveDetector* photoSensorSensitiveDetector = DSPD->get_photoSensor()->get_sensitiveDetector();
            PhotoSensorHitsCollection* photoSensorHitCollection = photoSensorSensitiveDetector->get_hitsCollection( t_event );
            G4String photoSensorHitHistogramName = "photoSensor_" + to_string( DSPD->get_photoSensor()->get_sensitiveDetector()->get_ID() );

            if( photoSensorHitCollection ) {
                for( G4int i = 0; i < photoSensorHitCollection->GetSize(); i++ ) {
                    PhotoSensorHit* photoSensorHit = static_cast< PhotoSensorHit* >( photoSensorHitCollection->GetHit( i ) );

                    m_outputManager->fill_histogram_2D( photoSensorHitHistogramName                    , 
                                                        photoSensorHit->get_hit_position_relative().x(), 
                                                        photoSensorHit->get_hit_position_relative().y(), 1 );
                    m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_position_absolute"           , photoSensorHit->get_hit_position_absolute                          () );
                    m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_position_relative"           , photoSensorHit->get_hit_position_relative                          () );
                    for( G4int i : m_outputMessenger->get_photoSensor_hits_position_relative_lens_save() ) {
                        if( photoSensorHit->get_lensHit( i ) ) {
                            m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_position_relative_lens_" + to_string( i ), photoSensorHit->get_lensHit( i )->get_hit_position_relative() );
                        } else {
                            m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_position_relative_lens_" + to_string( i ), G4ThreeVector( 0, 0, 0 ) );
                            // m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_position_relative_lens_" + to_string( i ), G4ThreeVector( nan(""), nan(""), nan("") ) );
                        }
                    }
                    m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_position_initial"            , photoSensorHit->get_particle_position_initial                      () );
                    m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_momentum"                    , photoSensorHit->get_particle_momentum                              () );
                    m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_direction"                   , photoSensorHit->get_particle_direction                             () );
                    m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_direction_relative"          , photoSensorHit->get_particle_direction_relative                    () );
                    for( G4int i : m_outputMessenger->get_photoSensor_hits_direction_relative_lens_save() ) {
                        if( photoSensorHit->get_lensHit( i ) ) {
                            m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_direction_relative_lens_" + to_string( i ), photoSensorHit->get_lensHit( i )->get_particle_direction_relative() );
                        } else {
                            m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_direction_relative_lens_" + to_string( i ), G4ThreeVector( 0, 0, 0 ) );
                            // m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_direction_relative_lens_" + to_string( i ), G4ThreeVector( nan(""), nan(""), nan("") ) );
                        }
                    }
                    m_outputManager->fill_tuple_column_double ( "photoSensor_hits_time"                        , photoSensorHit->get_hit_time                                       () );
                    m_outputManager->fill_tuple_column_string ( "photoSensor_hits_process"                     , photoSensorHit->get_hit_process                                    () );
                    m_outputManager->fill_tuple_column_double ( "photoSensor_hits_energy"                      , photoSensorHit->get_particle_energy                                () );
                    m_outputManager->fill_tuple_column_string ( "photoSensor_hits_photoSensorID"               , photoSensorHit->get_photoSensor_name                               () );
                    m_outputManager->fill_tuple_column        ( "photoSensor_hits" );
                }
            } else {
                // G4ExceptionDescription description;
                // description << "photoSensorHitCollection is NULL. Not saving photoSensor hits for photoSensor=" << DSPD->get_photoSensor()->get_name();
                // G4Exception( "EventAction::EndOfEventAction", "EventAction001", JustWarning, description );
            }
        }
    } 

    if( m_outputMessenger->get_calorimeter_hits_save() ) {
        for( Calorimeter* cal : m_detectorConstruction->get_calorimeters() ) {
            CalorimeterSensitiveDetector* calorimeterSensitiveDetector = cal->get_sensitiveDetector();
            CalorimeterHitsCollection* calorimeterHitCollection = calorimeterSensitiveDetector->get_hitsCollection( t_event );

            for( G4int i = 0; i < calorimeterHitCollection->GetSize(); i++ ) {
                CalorimeterHit* calorimeterHit = static_cast< CalorimeterHit* >( calorimeterHitCollection->GetHit( i ) );

                m_outputManager->fill_tuple_column_3vector( "calorimeter_hits_position_absolute" , calorimeterHit->get_hit_position_absolute      () );
                m_outputManager->fill_tuple_column_3vector( "calorimeter_hits_position_relative" , calorimeterHit->get_hit_position_relative      () );
                m_outputManager->fill_tuple_column_3vector( "calorimeter_hits_position_initial"  , calorimeterHit->get_particle_position_initial  () );
                m_outputManager->fill_tuple_column_3vector( "calorimeter_hits_momentum"          , calorimeterHit->get_particle_momentum          () );
                m_outputManager->fill_tuple_column_3vector( "calorimeter_hits_direction"         , calorimeterHit->get_particle_direction         () );
                m_outputManager->fill_tuple_column_3vector( "calorimeter_hits_direction_relative", calorimeterHit->get_particle_direction_relative() );
                m_outputManager->fill_tuple_column_double ( "calorimeter_hits_time"              , calorimeterHit->get_hit_time                   () );
                m_outputManager->fill_tuple_column_string ( "calorimeter_hits_process"           , calorimeterHit->get_hit_process                () );
                m_outputManager->fill_tuple_column_double ( "calorimeter_hits_energy"            , calorimeterHit->get_particle_energy            () );
                m_outputManager->fill_tuple_column_string ( "calorimeter_hits_calorimeterID"     , calorimeterHit->get_calorimeter_name           () );
                m_outputManager->fill_tuple_column        ( "calorimeter_hits" );
            }   
        }
    }

    if( m_outputMessenger->get_lens_hits_save() ) {
        for( DirectionSensitivePhotoDetector* DSPD : m_detectorConstruction->get_directionSensitivePhotoDetectors() ) {
            LensSystem* lensSystem = DSPD->get_lensSystem();
            for( Lens* lens : lensSystem->get_lenses() ) {
                LensSensitiveDetector* lensSensitiveDetector = lens->get_sensitiveDetector();
                LensHitsCollection* lensHitCollection = lensSensitiveDetector->get_hitsCollection( t_event );

                if( lensHitCollection ) {
                    for( G4int i = 0; i < lensHitCollection->GetSize(); i++ ) {
                        LensHit* lensHit = static_cast< LensHit* >( lensHitCollection->GetHit( i ) );

                        m_outputManager->fill_tuple_column_3vector( "lens_hits_position_absolute" , lensHit->get_hit_position_absolute      () );
                        m_outputManager->fill_tuple_column_3vector( "lens_hits_position_relative" , lensHit->get_hit_position_relative      () );
                        m_outputManager->fill_tuple_column_3vector( "lens_hits_position_initial"  , lensHit->get_particle_position_initial  () );
                        m_outputManager->fill_tuple_column_3vector( "lens_hits_momentum"          , lensHit->get_particle_momentum          () );
                        m_outputManager->fill_tuple_column_3vector( "lens_hits_direction"         , lensHit->get_particle_direction         () );
                        m_outputManager->fill_tuple_column_3vector( "lens_hits_direction_relative", lensHit->get_particle_direction_relative() );
                        m_outputManager->fill_tuple_column_double ( "lens_hits_time"              , lensHit->get_hit_time                   () );
                        m_outputManager->fill_tuple_column_string ( "lens_hits_process"           , lensHit->get_hit_process                () );
                        m_outputManager->fill_tuple_column_double ( "lens_hits_energy"            , lensHit->get_particle_energy            () );
                        m_outputManager->fill_tuple_column_string ( "lens_hits_lensID"            , lensHit->get_lens_name                  () );
                        m_outputManager->fill_tuple_column_boolean( "lens_hits_transmittance"     , lensHit->get_particle_transmittance     () );
                        m_outputManager->fill_tuple_column        ( "lens_hits" );
                    }
                } else {
                    // G4ExceptionDescription description;
                    // description << "lensHitCollection is NULL. Not saving lens hits for lens=" << lens->get_name();
                    // G4Exception( "EventAction::EndOfEventAction", "EventAction002", JustWarning, description );
                }
            }
        }
    }

    if( m_outputMessenger->get_medium_hits_save() ) {
        for( Medium* medium : m_detectorConstruction->get_mediums() ) {
            MediumSensitiveDetector* mediumSensitiveDetector = medium->get_sensitiveDetector();
            MediumHitsCollection* mediumHitCollection = mediumSensitiveDetector->get_hitsCollection( t_event );

            for( G4int i = 0; i < mediumHitCollection->GetSize(); i++ ) {
                MediumHit* mediumHit = static_cast< MediumHit* >( mediumHitCollection->GetHit( i ) );

                m_outputManager->fill_tuple_column_3vector( "medium_hits_position_absolute", mediumHit->get_hit_position_absolute    () );
                m_outputManager->fill_tuple_column_3vector( "medium_hits_position_initial" , mediumHit->get_particle_position_initial() );
                m_outputManager->fill_tuple_column_3vector( "medium_hits_momentum"         , mediumHit->get_particle_momentum        () );
                m_outputManager->fill_tuple_column_double ( "medium_hits_time"             , mediumHit->get_hit_time                 () );
                m_outputManager->fill_tuple_column_string ( "medium_hits_process"          , mediumHit->get_hit_process              () );
                m_outputManager->fill_tuple_column_double ( "medium_hits_energy"           , mediumHit->get_particle_energy          () );
                m_outputManager->fill_tuple_column_string ( "medium_hits_mediumID"         , mediumHit->get_medium_name              () );
                m_outputManager->fill_tuple_column_boolean( "medium_hits_transmittance"    , mediumHit->get_particle_transmittance   () );
                m_outputManager->fill_tuple_column        ( "medium_hits" );
            }
        }
    }

    G4cout << "EndOfEventAction" << G4endl;
}