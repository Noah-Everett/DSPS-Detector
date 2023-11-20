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

#ifndef SteppingAction_hh
#define SteppingAction_hh

#include "G4Step.hh"
#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4AnalysisManager.hh"

#include "OutputManager.hh"
#include "OutputMessenger.hh"

class G4LogicalVolume;

class EventAction;

class SteppingAction : public G4UserSteppingAction
{
    public:
        SteppingAction( RunAction* )         ;
       ~SteppingAction(            ) override;

        void UserSteppingAction( const G4Step* ) override;
        
    private:
        RunAction        * m_runAction      { nullptr                           };
        OutputManager    * m_outputManager  { nullptr                           };
        OutputMessenger  * m_outputMessenger{ OutputMessenger  ::get_instance() };
        G4AnalysisManager* m_analysisManager{ nullptr                           };

        G4int m_index_photon { -1 };
        G4int m_index_primary{ -1 };
};

#endif