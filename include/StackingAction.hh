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

#ifndef StackingAction_hh
#define StackingAction_hh

#include "G4UserStackingAction.hh"
#include "globals.hh"

#include "NESTStackingAction.hh"

class StackingAction : public NESTStackingAction
{
    public:
        StackingAction();
       ~StackingAction() override;

        G4ClassificationOfNewTrack ClassifyNewTrack( const G4Track* ) override;
};

#endif