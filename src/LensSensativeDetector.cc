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

LensSensitiveDetector::LensSensitiveDetector( G4String t_name ) 
    : G4VSensitiveDetector( t_name ) {
    G4cout << G4endl;
    collectionName.insert( "LensSensitiveDetector" );
}

void LensSensitiveDetector::Initialize( G4HCofThisEvent* hce ) {
}

G4bool LensSensitiveDetector::ProcessHits( G4Step* t_step, G4TouchableHistory* t_hist ) {
    return true;
}

G4String LensSensitiveDetector::get_name() {
    return m_name;
}