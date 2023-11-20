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

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4DecayTable.hh"
#include "G4VDecayChannel.hh"
#include "G4Gamma.hh"
#include "G4PhysicsVector.hh"
#include "G4Decay.hh"
#include "G4GeneralPhaseSpaceDecay.hh"

void make_photonCreator() {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

    G4String particleName = "PhotonCreator";
    G4ParticleDefinition* existingParticle = particleTable->FindParticle( particleName );
    if( existingParticle ) {
        G4cout << "Particle " << particleName << " already exists!" << G4endl;
        return;
    }

    //    Arguments for constructor are as follows
    //               name             mass          width             charge
    //             2*spin           parity  C-conjugation
    //          2*Isospin       2*Isospin3       G-parity
    //               type    lepton number  baryon number       PDG encoding
    //             stable         lifetime    decay table
    //             shortlived      subType    anti_encoding  magnetic moment
    G4ParticleDefinition* photonCreator = new G4ParticleDefinition(
             particleName,          4*2*eV,          0.0,                0.0,
                        0,               0,            0,
                        0,               0,            0,
                  "boson",               0,            0,                  0,
                    false,            1*ps,      nullptr,
                     true,        "photon",            0,                0.0
    );

    G4DecayTable* decayTable = new G4DecayTable();
    photonCreator->SetDecayTable( decayTable );
    decayTable->Insert( new G4GeneralPhaseSpaceDecay( "PhotonCreator", 1.0, 3,
                                                      "opticalphoton",
                                                      "opticalphoton",
                                                      "opticalphoton" ) );

    if( !particleTable->contains( photonCreator) )
        particleTable->Insert( photonCreator );
}
