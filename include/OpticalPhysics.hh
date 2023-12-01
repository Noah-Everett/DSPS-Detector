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

#ifndef OpticalPhysics_hh
#define OpticalPhysics_hh

#include "G4ParticleTable.hh"
#include "G4OpticalPhysics.hh"
#include "G4ProcessManager.hh"
#include "G4Cerenkov.hh"
#include "G4EmSaturation.hh"
#include "G4LossTableManager.hh"
#include "G4OpAbsorption.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpticalParameters.hh"
#include "G4OpWLS.hh"
#include "G4OpWLS2.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4Scintillation.hh"

#include "NESTProc.hh"

#include "NESTDetector.hh"
#include "DetectorConstruction.hh"

using NEST::NESTThermalElectron;
using NEST::NESTProc;

class OpticalPhysics : public G4OpticalPhysics 
{
    public:
        OpticalPhysics( DetectorConstruction*             , 
                        const G4String& = "OpticalPhysics" );
       ~OpticalPhysics();

        void ConstructProcess();
        void ConstructParticle();

    private:
        DetectorConstruction* m_detectorConstruction{ nullptr };
};

#endif