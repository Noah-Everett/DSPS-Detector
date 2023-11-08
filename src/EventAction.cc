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
    m_photoSensor_hits_position_absolute_ID = m_outputManager->get_tuple_column_ID( "photoSensor_hits_position_absolute" );
    m_photoSensor_hits_position_relative_ID = m_outputManager->get_tuple_column_ID( "photoSensor_hits_position_relative" );
    m_photoSensor_hits_time_ID              = m_outputManager->get_tuple_column_ID( "photoSensor_hits_time"              );
    m_photoSensor_hits_process_ID           = m_outputManager->get_tuple_column_ID( "photoSensor_hits_process"           );
    m_photoSensor_hits_photoSensorID_ID     = m_outputManager->get_tuple_column_ID( "photoSensor_hits_photoSensorID"     );
    m_photoSensor_hits_energy_ID            = m_outputManager->get_tuple_column_ID( "photoSensor_hits_energy"            );

    G4cout << "m_photoSensor_hits_position_absolute_ID = " << m_photoSensor_hits_position_absolute_ID.first << ", " << m_photoSensor_hits_position_absolute_ID.second << G4endl;
    G4cout << "m_photoSensor_hits_position_relative_ID = " << m_photoSensor_hits_position_relative_ID.first << ", " << m_photoSensor_hits_position_relative_ID.second << G4endl;
    G4cout << "m_photoSensor_hits_time_ID              = " << m_photoSensor_hits_time_ID             .first << ", " << m_photoSensor_hits_time_ID             .second << G4endl;
    G4cout << "m_photoSensor_hits_process_ID           = " << m_photoSensor_hits_process_ID          .first << ", " << m_photoSensor_hits_process_ID          .second << G4endl;
    G4cout << "m_photoSensor_hits_photoSensorID_ID     = " << m_photoSensor_hits_photoSensorID_ID    .first << ", " << m_photoSensor_hits_photoSensorID_ID    .second << G4endl;
    G4cout << "m_photoSensor_hits_energy_ID            = " << m_photoSensor_hits_energy_ID           .first << ", " << m_photoSensor_hits_energy_ID           .second << G4endl;

    G4RunManager::GetRunManager()->SetPrintProgress( 1 );
}

EventAction::~EventAction() {
}

void EventAction::BeginOfEventAction( const G4Event* t_event ) {
    m_analysisManager = G4AnalysisManager::Instance();

    G4int ID = m_outputManager->get_histogram_2D_ID( "photoSensor_0" );
    if( ID != kInvalidId && m_analysisManager->GetH2Title( ID ) != "photoSensor_0" )
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
        G4int photoSensorHitHistogramID = m_outputManager->get_histogram_2D_ID( photoSensorHitHistogramName );

        for( G4int i = 0; i < photoSensorHitCollection->GetSize(); i++ ) {
            PhotoSensorHit* photoSensorHit = static_cast< PhotoSensorHit* >( photoSensorHitCollection->GetHit( i ) );

            G4ThreeVector hit_position_absolute = photoSensorHit->get_hit_position_absolute();
            G4ThreeVector hit_position_relative = photoSensorHit->get_hit_position_relative();
            G4double      hit_time              = photoSensorHit->get_hit_time             ();
            G4String      hit_process           = photoSensorHit->get_hit_process          ();
            G4double      hit_energy            = photoSensorHit->get_hit_energy           ();
            G4String      hit_photoSensor_name  = photoSensorHit->get_photoSensor_name     ();

            if( photoSensorHitHistogramID >= 0 ) m_analysisManager->FillH2( photoSensorHitHistogramID, hit_position_relative.x(), hit_position_relative.y(), 1 );
            if( m_photoSensor_hits_position_absolute_ID.first >= 0 ) m_analysisManager->FillNtupleDColumn( m_photoSensor_hits_position_absolute_ID.first, m_photoSensor_hits_position_absolute_ID.second  , hit_position_absolute.x() );
            if( m_photoSensor_hits_position_absolute_ID.first >= 0 ) m_analysisManager->FillNtupleDColumn( m_photoSensor_hits_position_absolute_ID.first, m_photoSensor_hits_position_absolute_ID.second+1, hit_position_absolute.y() );
            if( m_photoSensor_hits_position_absolute_ID.first >= 0 ) m_analysisManager->FillNtupleDColumn( m_photoSensor_hits_position_absolute_ID.first, m_photoSensor_hits_position_absolute_ID.second+2, hit_position_absolute.z() );
            if( m_photoSensor_hits_position_relative_ID.first >= 0 ) m_analysisManager->FillNtupleDColumn( m_photoSensor_hits_position_relative_ID.first, m_photoSensor_hits_position_relative_ID.second  , hit_position_relative.x() );
            if( m_photoSensor_hits_position_relative_ID.first >= 0 ) m_analysisManager->FillNtupleDColumn( m_photoSensor_hits_position_relative_ID.first, m_photoSensor_hits_position_relative_ID.second+1, hit_position_relative.y() );
            if( m_photoSensor_hits_position_relative_ID.first >= 0 ) m_analysisManager->FillNtupleDColumn( m_photoSensor_hits_position_relative_ID.first, m_photoSensor_hits_position_relative_ID.second+2, hit_position_relative.z() );
            if( m_photoSensor_hits_time_ID             .first >= 0 ) m_analysisManager->FillNtupleDColumn( m_photoSensor_hits_time_ID             .first, m_photoSensor_hits_time_ID             .second  , hit_time                  );
            if( m_photoSensor_hits_process_ID          .first >= 0 ) m_analysisManager->FillNtupleSColumn( m_photoSensor_hits_process_ID          .first, m_photoSensor_hits_process_ID          .second  , hit_process               );
            if( m_photoSensor_hits_photoSensorID_ID    .first >= 0 ) m_analysisManager->FillNtupleSColumn( m_photoSensor_hits_photoSensorID_ID    .first, m_photoSensor_hits_photoSensorID_ID    .second  , hit_photoSensor_name      );
            if( m_photoSensor_hits_energy_ID           .first >= 0 ) m_analysisManager->FillNtupleDColumn( m_photoSensor_hits_energy_ID           .first, m_photoSensor_hits_energy_ID           .second  , hit_energy                );
            m_analysisManager->AddNtupleRow( m_photoSensor_hits_position_absolute_ID.first );
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