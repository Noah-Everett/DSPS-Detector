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

// #ifndef Track_hh
// #define Track_hh

// #include "G4Track.hh"
// #include "G4TrackingManager.hh"
// #include "G4EventManager.hh"

// G4Track* GetParentTrack( G4Track* t_track ) {
//     G4TrackingManager* trackingManager = G4EventManager::GetEventManager()->GetTrackingManager();
//     return trackingManager->GimmeSecondaries()->at(0);
// }

// G4ThreeVector GetStartingPosition( G4Track* t_track ) {
//     while (t_track->GetParentID() > 0) {
//         t_track = GetParentTrack(t_track);
//     }

//     return t_track->GetVertexPosition();
// }

// #endif