//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************

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
    m_analysisManager = G4AnalysisManager::Instance();

    G4int ID = m_outputManager->get_histogram_2D_ID( "photoSensor_0" );
    if( ID != kInvalidId && m_analysisManager->GetH2Title( ID ) == "photoSensor_0" )
        for( DirectionSensitivePhotoDetector* DSPD : m_detectorConstruction->get_directionSensitivePhotoDetectors() ) {
            G4cout << "Resetting histogram name : photoSensor_" << DSPD->get_photoSensor()->get_sensitiveDetector()->get_ID() 
                   << " --> " << DSPD->get_photoSensor()->get_sensitiveDetector()->get_name() << G4endl;
            m_analysisManager->SetH2Title( m_outputManager->get_histogram_2D_ID( 
                                           "photoSensor_" + to_string( DSPD->get_photoSensor()->get_sensitiveDetector()->get_ID() ) ),
                                           DSPD->get_photoSensor()->get_sensitiveDetector()->get_name() );
        }
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
            m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_position_absolute", photoSensorHit->get_hit_position_absolute() );
            m_outputManager->fill_tuple_column_3vector( "photoSensor_hits_position_relative", photoSensorHit->get_hit_position_relative() );
            m_outputManager->fill_tuple_column_double ( "photoSensor_hits_time"             , photoSensorHit->get_hit_time             () );
            m_outputManager->fill_tuple_column_string ( "photoSensor_hits_process"          , photoSensorHit->get_hit_process          () );
            m_outputManager->fill_tuple_column_double ( "photoSensor_hits_energy"           , photoSensorHit->get_hit_energy           () );
            m_outputManager->fill_tuple_column_string ( "photoSensor_hits_photoSensorID"    , photoSensorHit->get_photoSensor_name     () );
            m_outputManager->fill_tuple_column        ( "photoSensor_hits" );
        }

        // vector< LensSensitiveDetector* > lensSensitiveDetectors;
        // for( Lens* lens : DSPD->get_lensSystem()->get_lenses() )
        //     lensSensitiveDetectors.push_back( lens->get_sensitiveDetector() );

        // vector< LensHitCollection* > lensHitCollections;
        // for( LensSensitiveDetector* lensSensitiveDetector : lensSensitiveDetectors )
        //     lensHitCollections.push_back( lensSensitiveDetector->get_hitCollection() );

        // vector< G4String > lensHitHistogramNames;
    }
    G4cout << "EndOfEventAction" << G4endl;
}