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

#ifndef EventAction_hh
#define EventAction_hh

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "OutputMessenger.hh"
#include "ConstructionMessenger.hh"
#include "RunAction.hh"

using std::to_string;

class EventAction : public G4UserEventAction
{
    public:
        EventAction( RunAction*, DetectorConstruction* )         ;
       ~EventAction(                                   ) override;

        void BeginOfEventAction( const G4Event* ) override;
        void EndOfEventAction  ( const G4Event* ) override;

    private:
        RunAction            * m_runAction            { nullptr                               };
        OutputMessenger      * m_outputMessenger      { OutputMessenger      ::get_instance() };
        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };
        DetectorConstruction * m_detectorConstruction { nullptr                               };
        OutputManager        * m_outputManager        { nullptr                               };
        G4AnalysisManager    * m_analysisManager      { nullptr                               };
        G4SDManager          * m_SDManager            { nullptr                               };
};

#endif