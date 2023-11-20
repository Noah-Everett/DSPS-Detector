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

#ifndef PrimaryGeneratorAction_hh
#define PrimaryGeneratorAction_hh

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Event.hh"

#include "ParticleGun.hh"

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        PrimaryGeneratorAction();
       ~PrimaryGeneratorAction();

    void GeneratePrimaries( G4Event* ) override;

    private:
        ParticleGun* m_particleGun;
};

#endif
