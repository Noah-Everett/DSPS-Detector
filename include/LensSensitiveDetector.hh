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

#ifndef LensSensitiveDetector_hh
#define LensSensitiveDetector_hh

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"

#include "OutputMessenger.hh"
#include "OutputManager.hh"

class OutputManager;

class LensSensitiveDetector : public G4VSensitiveDetector 
{
    public:
        LensSensitiveDetector( G4String );
       ~LensSensitiveDetector() override = default;

        void Initialize( G4HCofThisEvent* ) override;
        G4bool ProcessHits( G4Step*, G4TouchableHistory* ) override;

        G4String get_name();
    
    protected:
        G4String m_name;
};

#endif