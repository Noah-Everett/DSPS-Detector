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

#include "LensSensitiveDetector.hh"

LensSensitiveDetector::LensSensitiveDetector( G4String t_name, G4int t_ID )
    : G4VSensitiveDetector( t_name ) {
    m_name = t_name;
    m_ID = t_ID;
    collectionName.insert( "LensSensitiveDetector" );
}

void LensSensitiveDetector::Initialize( G4HCofThisEvent* t_hitCollectionOfThisEvent ) {
    m_lensHitsCollection = new LensHitsCollection( SensitiveDetectorName, collectionName[ 0 ] );
    if( m_lensHitsCollection_ID < 0 )
        m_lensHitsCollection_ID = G4SDManager::GetSDMpointer()->GetCollectionID( m_lensHitsCollection );
    t_hitCollectionOfThisEvent->AddHitsCollection( m_lensHitsCollection_ID, m_lensHitsCollection );
}

G4bool LensSensitiveDetector::ProcessHits( G4Step* t_step, G4TouchableHistory* t_hist ) {
    LensHit* hit = new LensHit();

    hit->set_lens_position            ( m_position                                                              );
    hit->set_lens_rotationMatrix      ( m_rotationMatrix                                                        );
    hit->set_lens_name                ( m_name                                                                  );
    hit->set_lens_ID                  ( m_ID                                                                    );
    hit->set_hit_position_absolute    ( t_step->GetPostStepPoint()->GetPosition      ()                         );
    hit->set_hit_time                 ( t_step->GetPostStepPoint()->GetGlobalTime    ()                         );
    hit->set_hit_process              ( t_step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()   );
    hit->set_particle_energy          ( t_step->GetPostStepPoint()->GetKineticEnergy ()                         );
    hit->set_particle_momentum        ( t_step->GetPostStepPoint()->GetMomentum      ()                         );
    hit->set_particle_position_initial( t_step->GetTrack        ()->GetVertexPosition()                         );
    hit->set_particle_transmittance   ( ( t_step->GetTrack()->GetTrackStatus() == fStopAndKill ) ? false : true );

    m_lensHitsCollection->insert( hit );

    if( t_step->IsFirstStepInVolume() )
        m_firstHit = hit;

    return true;
}

G4String LensSensitiveDetector::get_name() {
    return m_name;
}


void LensSensitiveDetector::set_position( G4ThreeVector t_position ) {
    m_position = t_position;
}

void LensSensitiveDetector::set_rotationMatrix( G4RotationMatrix* t_rotationMatrix ) {
    m_rotationMatrix = t_rotationMatrix;
}

G4ThreeVector LensSensitiveDetector::get_position() {
    return m_position;
}

G4RotationMatrix* LensSensitiveDetector::get_rotationMatrix() {
    return m_rotationMatrix;
}

LensHitsCollection* LensSensitiveDetector::get_hitsCollection( const G4Event* t_event ) {
    G4HCofThisEvent* hitCollectionOfThisEvent = t_event->GetHCofThisEvent();
    m_lensHitsCollection_ID = G4SDManager::GetSDMpointer()->GetCollectionID( SensitiveDetectorName + "/" + collectionName[ 0 ] );
    return static_cast< LensHitsCollection* >( hitCollectionOfThisEvent->GetHC( m_lensHitsCollection_ID ) );
}

G4String LensSensitiveDetector::get_hitsCollection_name() {
    return collectionName[ 0 ];
}

G4int LensSensitiveDetector::get_hitsCollection_ID() {
    return m_lensHitsCollection_ID;
}

void LensSensitiveDetector::set_hitsCollection_ID( G4int t_lensHitsCollection_ID ) {
    m_lensHitsCollection_ID = t_lensHitsCollection_ID;
}

G4int LensSensitiveDetector::get_ID() {
    return m_ID;
}

LensHit* LensSensitiveDetector::get_firstHit() {
    return m_firstHit;
}