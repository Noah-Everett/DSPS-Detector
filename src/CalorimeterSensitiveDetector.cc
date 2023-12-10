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

#include "CalorimeterSensitiveDetector.hh"

CalorimeterSensitiveDetector::CalorimeterSensitiveDetector( G4String t_name, G4int t_ID )
    : G4VSensitiveDetector( t_name ) {
    m_name = t_name;
    m_ID = t_ID;
    collectionName.insert( "CalorimeterSensitiveDetector" );
}

void CalorimeterSensitiveDetector::Initialize( G4HCofThisEvent* t_hitCollectionOfThisEvent ) {
    m_calorimeterHitsCollection = new CalorimeterHitsCollection( SensitiveDetectorName, collectionName[ 0 ] );
    if( m_calorimeterHitsCollection_ID < 0 )
        m_calorimeterHitsCollection_ID = G4SDManager::GetSDMpointer()->GetCollectionID( m_calorimeterHitsCollection );
    t_hitCollectionOfThisEvent->AddHitsCollection( m_calorimeterHitsCollection_ID, m_calorimeterHitsCollection );
}

G4bool CalorimeterSensitiveDetector::ProcessHits( G4Step* t_step, G4TouchableHistory* t_hist ) {
    CalorimeterHit* hit = new CalorimeterHit();

    hit->set_calorimeter_position      ( m_position                                                            );
    hit->set_calorimeter_rotationMatrix( m_rotationMatrix                                                      );
    hit->set_calorimeter_name          ( m_name                                                                );
    hit->set_calorimeter_ID            ( m_ID                                                                  );
    hit->set_hit_position_absolute     ( t_step->GetPostStepPoint()->GetPosition      ()                       );
    hit->set_hit_time                  ( t_step->GetPostStepPoint()->GetGlobalTime    ()                       );
    hit->set_hit_energy                ( t_step->GetPostStepPoint()->GetKineticEnergy ()                       );
    hit->set_hit_momentum              ( t_step->GetPostStepPoint()->GetMomentum      ()                       );
    hit->set_hit_process               ( t_step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() );
    hit->set_particle_energy           ( t_step->GetTrack        ()->GetKineticEnergy ()                       );
    hit->set_particle_momentum         ( t_step->GetTrack        ()->GetMomentum      ()                       );
    hit->set_particle_position_initial ( t_step->GetTrack        ()->GetVertexPosition()                                           );

    m_calorimeterHitsCollection->insert( hit );

    t_step->GetTrack()->SetTrackStatus( fStopAndKill );

    return true;
}

G4String CalorimeterSensitiveDetector::get_name() {
    return m_name;
}


void CalorimeterSensitiveDetector::set_position( G4ThreeVector t_position ) {
    m_position = t_position;
}

void CalorimeterSensitiveDetector::set_rotationMatrix( G4RotationMatrix* t_rotationMatrix ) {
    m_rotationMatrix = t_rotationMatrix;
}

G4ThreeVector CalorimeterSensitiveDetector::get_position() {
    return m_position;
}

G4RotationMatrix* CalorimeterSensitiveDetector::get_rotationMatrix() {
    return m_rotationMatrix;
}

CalorimeterHitsCollection* CalorimeterSensitiveDetector::get_hitsCollection( const G4Event* t_event ) {
    G4HCofThisEvent* hitCollectionOfThisEvent = t_event->GetHCofThisEvent();
    m_calorimeterHitsCollection_ID = G4SDManager::GetSDMpointer()->GetCollectionID( SensitiveDetectorName + "/" + collectionName[ 0 ] );
    return static_cast< CalorimeterHitsCollection* >( hitCollectionOfThisEvent->GetHC( m_calorimeterHitsCollection_ID ) );
}

G4String CalorimeterSensitiveDetector::get_hitsCollection_name() {
    return collectionName[ 0 ];
}

G4int CalorimeterSensitiveDetector::get_hitsCollection_ID() {
    return m_calorimeterHitsCollection_ID;
}

void CalorimeterSensitiveDetector::set_hitsCollection_ID( G4int t_calorimeterHitsCollection_ID ) {
    m_calorimeterHitsCollection_ID = t_calorimeterHitsCollection_ID;
}

G4int CalorimeterSensitiveDetector::get_ID() {
    return m_ID;
}