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

#ifndef RunAction_hh
#define RunAction_hh

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include "G4AnalysisManager.hh"

#include "OutputMessenger.hh"
#include "OutputManager.hh"
#include "DetectorConstruction.hh"
#include "ConstructionMessenger.hh"

using std::to_string;

class RunAction : public G4UserRunAction
{
    public:
        RunAction( DetectorConstruction* )         ;
       ~RunAction(                       ) override;

        void BeginOfRunAction( const G4Run* ) override;
        void   EndOfRunAction( const G4Run* ) override;

        OutputManager* get_outputManager();

    private:
        G4AnalysisManager    * m_analysisManager      { G4AnalysisManager    ::Instance    () };
        OutputMessenger      * m_outputMessenger      { OutputMessenger      ::get_instance() };
        // OutputManager        * m_outputManager        { OutputManager        ::get_instance() };
        OutputManager        * m_outputManager        { new OutputManager()                   };
        DetectorConstruction * m_detectorConstruction { nullptr                               };
        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };
};

#endif