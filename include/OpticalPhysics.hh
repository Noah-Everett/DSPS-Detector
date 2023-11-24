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
        NESTDetector* m_NESTDetector{ nullptr };
};

#endif