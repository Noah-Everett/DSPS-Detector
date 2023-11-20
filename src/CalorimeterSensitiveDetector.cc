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

#include "G4SDManager.hh"

CalorimeterSensitiveDetector::CalorimeterSensitiveDetector( G4String t_name ) 
    : G4VSensitiveDetector( t_name ) {
    collectionName.insert( "CalorimeterSensitiveDetector" );
}

void CalorimeterSensitiveDetector::Initialize( G4HCofThisEvent* hce ) {
}

G4bool CalorimeterSensitiveDetector::ProcessHits( G4Step* t_step, G4TouchableHistory* t_hist ) {
    // G4int sensorHistogramID = m_outputManager->get_histogram_id( m_name );
    // G4cout << "HERE" << G4endl;
    // m_analysisManager->FillH2( sensorHistogramID, t_step->GetPreStepPoint()->GetPosition().x(), t_step->GetPreStepPoint()->GetPosition().y() );
    return true;
}

G4String CalorimeterSensitiveDetector::get_name() {
    return m_name;
}