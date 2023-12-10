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

#include <cmath>
#include <string>

#include "G4Step.hh"
#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProcessTable.hh"

#include "OutputManager.hh"
#include "OutputMessenger.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "ConstructionMessenger.hh"

using std::string;
using G4StrUtil::to_lower;

class SteppingAction : public G4UserSteppingAction
{
    public:
        SteppingAction( RunAction* )         ;
       ~SteppingAction(            ) override;

        void UserSteppingAction( const G4Step* ) override;
        
    private:
        RunAction            * m_runAction            { nullptr                               };
        OutputManager        * m_outputManager        { nullptr                               };
        OutputMessenger      * m_outputMessenger      { OutputMessenger      ::get_instance() };
        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };
        G4AnalysisManager    * m_analysisManager      { nullptr                               };

        G4ThreeVector m_world_size{ m_constructionMessenger->get_world_size() / 2 };

        G4int m_index_photon { -1 };
        G4int m_index_primary{ -1 };
};

#endif