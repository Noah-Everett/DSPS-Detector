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

#include "PhotoSensorSensitiveDetector.hh"

PhotoSensorSensitiveDetector::PhotoSensorSensitiveDetector( G4String t_name, G4int t_ID )
    : G4VSensitiveDetector( t_name ) {
    m_name = t_name;
    m_ID = t_ID;
    collectionName.insert( "PhotoSensorSensitiveDetector" );
}

void PhotoSensorSensitiveDetector::Initialize( G4HCofThisEvent* t_hitCollectionOfThisEvent ) {
    m_photoSensorHitsCollection = new PhotoSensorHitsCollection( SensitiveDetectorName, collectionName[ 0 ] );
    if( m_photoSensorHitsCollection_ID < 0 )
        m_photoSensorHitsCollection_ID = G4SDManager::GetSDMpointer()->GetCollectionID( m_photoSensorHitsCollection );
    t_hitCollectionOfThisEvent->AddHitsCollection( m_photoSensorHitsCollection_ID, m_photoSensorHitsCollection );
}

G4bool PhotoSensorSensitiveDetector::ProcessHits( G4Step* t_step, G4TouchableHistory* t_hist ) {
    // m_outputManager->save_step_photoSensor_hits( t_step, m_name, m_position, m_rotationMatrix, false );
    PhotoSensorHit* hit = new PhotoSensorHit();

    hit->set_photoSensor_position      ( m_position                                                            );
    hit->set_photoSensor_rotationMatrix( m_rotationMatrix                                                      );
    hit->set_photoSensor_name          ( m_name                                                                );
    hit->set_photoSensor_ID            ( m_ID                                                                  );
    hit->set_hit_position_absolute     ( t_step->GetPostStepPoint()->GetPosition      ()                       );
    hit->set_hit_time                  ( t_step->GetPostStepPoint()->GetGlobalTime    ()                       );
    hit->set_hit_energy                ( t_step->GetPostStepPoint()->GetKineticEnergy ()                       );
    hit->set_hit_momentum              ( t_step->GetPostStepPoint()->GetMomentum      ()                       );
    hit->set_hit_process               ( t_step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() );
    hit->set_particle_energy           ( t_step->GetTrack        ()->GetKineticEnergy ()                       );
    hit->set_particle_momentum         ( t_step->GetTrack        ()->GetMomentum      ()                       );
    hit->set_particle_position_initial ( t_step->GetTrack        ()->GetVertexPosition()                                           );

    m_photoSensorHitsCollection->insert( hit );

    t_step->GetTrack()->SetTrackStatus( fKillTrackAndSecondaries );

    return true;
}

G4String PhotoSensorSensitiveDetector::get_name() {
    return m_name;
}


void PhotoSensorSensitiveDetector::set_position( G4ThreeVector t_position ) {
    m_position = t_position;
}

void PhotoSensorSensitiveDetector::set_rotationMatrix( G4RotationMatrix* t_rotationMatrix ) {
    m_rotationMatrix = t_rotationMatrix;
}

G4ThreeVector PhotoSensorSensitiveDetector::get_position() {
    return m_position;
}

G4RotationMatrix* PhotoSensorSensitiveDetector::get_rotationMatrix() {
    return m_rotationMatrix;
}

PhotoSensorHitsCollection* PhotoSensorSensitiveDetector::get_hitsCollection( const G4Event* t_event ) {
    G4HCofThisEvent* hitCollectionOfThisEvent = t_event->GetHCofThisEvent();
    m_photoSensorHitsCollection_ID = G4SDManager::GetSDMpointer()->GetCollectionID( SensitiveDetectorName + "/" + collectionName[ 0 ] );
    return static_cast< PhotoSensorHitsCollection* >( hitCollectionOfThisEvent->GetHC( m_photoSensorHitsCollection_ID ) );
}

G4String PhotoSensorSensitiveDetector::get_hitsCollection_name() {
    return collectionName[ 0 ];
}

G4int PhotoSensorSensitiveDetector::get_hitsCollection_ID() {
    return m_photoSensorHitsCollection_ID;
}

void PhotoSensorSensitiveDetector::set_hitsCollection_ID( G4int t_photoSensorHitsCollection_ID ) {
    m_photoSensorHitsCollection_ID = t_photoSensorHitsCollection_ID;
}

G4int PhotoSensorSensitiveDetector::get_ID() {
    return m_ID;
}