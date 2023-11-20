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

#ifndef ParticleGun_hh
#define ParticleGun_hh

#include "G4ParticleGun.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"
#include "G4OpticalPhoton.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

class ParticleGun : public G4ParticleGun 
{
    public:
        ParticleGun(       );
        ParticleGun( G4int );
       ~ParticleGun(       );

        void GeneratePrimaries( G4Event* );
};

#endif