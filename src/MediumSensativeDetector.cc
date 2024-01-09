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

#include "MediumSensitiveDetector.hh"

MediumSensitiveDetector::MediumSensitiveDetector( G4String t_name, G4int t_ID )
    : G4VSensitiveDetector( t_name ) {
    m_name = t_name;
    m_ID = t_ID;
    collectionName.insert( "MediumSensitiveDetector" );
}

void MediumSensitiveDetector::Initialize( G4HCofThisEvent* t_hitCollectionOfThisEvent ) {
    m_mediumHitsCollection = new MediumHitsCollection( SensitiveDetectorName, collectionName[ 0 ] );
    if( m_mediumHitsCollection_ID < 0 )
        m_mediumHitsCollection_ID = G4SDManager::GetSDMpointer()->GetCollectionID( m_mediumHitsCollection );
    t_hitCollectionOfThisEvent->AddHitsCollection( m_mediumHitsCollection_ID, m_mediumHitsCollection );
}

G4bool MediumSensitiveDetector::ProcessHits( G4Step* t_step, G4TouchableHistory* t_hist ) {
    MediumHit* hit = new MediumHit();

    hit->set_medium_name              ( m_name                                                                  );
    hit->set_medium_ID                ( m_ID                                                                    );
    hit->set_medium_position          ( m_position                                                              );
    hit->set_medium_rotationMatrix    ( m_rotationMatrix                                                        );
    hit->set_hit_position_absolute    ( t_step->GetPostStepPoint()->GetPosition      ()                         );
    hit->set_hit_time                 ( t_step->GetPostStepPoint()->GetGlobalTime    ()                         );
    hit->set_hit_process              ( t_step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()   );
    hit->set_hit_energy               ( t_step->GetPostStepPoint()->GetKineticEnergy ()                         );
    hit->set_hit_momentum             ( t_step->GetPostStepPoint()->GetMomentum      ()                         );
    hit->set_particle_position_initial( t_step->GetTrack        ()->GetVertexPosition()                         );
    hit->set_particle_transmittance   ( ( t_step->GetTrack()->GetTrackStatus() == fStopAndKill ) ? false : true );
    
    m_mediumHitsCollection->insert( hit );

    return true;
}

void MediumSensitiveDetector::set_position( G4ThreeVector t_position ) {
    m_position = t_position;
}

void MediumSensitiveDetector::set_rotationMatrix( G4RotationMatrix* t_rotationMatrix ) {
    m_rotationMatrix = t_rotationMatrix;
}

G4ThreeVector MediumSensitiveDetector::get_position() {
    return m_position;
}

G4RotationMatrix* MediumSensitiveDetector::get_rotationMatrix() {
    return m_rotationMatrix;
}

MediumHitsCollection* MediumSensitiveDetector::get_hitsCollection( const G4Event* t_event ) {
    G4HCofThisEvent* hitCollectionOfThisEvent = t_event->GetHCofThisEvent();
    m_mediumHitsCollection_ID = G4SDManager::GetSDMpointer()->GetCollectionID( SensitiveDetectorName + "/" + collectionName[ 0 ] );
    return static_cast< MediumHitsCollection* >( hitCollectionOfThisEvent->GetHC( m_mediumHitsCollection_ID ) );
}

G4String MediumSensitiveDetector::get_hitsCollection_name() {
    return collectionName[ 0 ];
}

G4int MediumSensitiveDetector::get_hitsCollection_ID() {
    return m_mediumHitsCollection_ID;
}

void MediumSensitiveDetector::set_hitsCollection_ID( G4int t_mediumHitsCollection_ID ) {
    m_mediumHitsCollection_ID = t_mediumHitsCollection_ID;
}

G4int MediumSensitiveDetector::get_ID() {
    return m_ID;
}

G4String MediumSensitiveDetector::get_name() {
    return m_name;
}