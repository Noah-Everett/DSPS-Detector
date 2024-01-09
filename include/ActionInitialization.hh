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

#ifndef ActionInitialization_hh
#define ActionInitialization_hh

#include "G4VUserActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"
#include "OutputMessenger.hh"
#include "DetectorConstruction.hh"
#include "ParticleGunMessenger.hh"

class ActionInitialization : public G4VUserActionInitialization
{
    public:
        ActionInitialization( DetectorConstruction* );
       ~ActionInitialization(                       );

        void BuildForMaster() const override;
        void Build() const override;
    
    protected:
        DetectorConstruction* m_detectorConstruction{ nullptr };
};

#endif