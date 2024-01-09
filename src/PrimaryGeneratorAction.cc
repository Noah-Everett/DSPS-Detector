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

#include "PrimaryGeneratorAction.hh"

#include "globals.hh"
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    m_particleGun = new ParticleGun( 1 );

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

    G4String particleName;
    m_particleGun->SetParticleDefinition( particleTable->FindParticle( particleName = "gamma" ) );
    // Default energy,position,momentum
    m_particleGun->SetParticleEnergy( 511. * keV );
    m_particleGun->SetParticlePosition( G4ThreeVector( 0., 0., -20. * cm ) );
    m_particleGun->SetParticleMomentumDirection( G4ThreeVector( 0., 0., 1. ) );
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() { 
    if( m_particleGun )
        delete m_particleGun; 
}

void PrimaryGeneratorAction::GeneratePrimaries( G4Event* event ) {
    m_particleGun->GeneratePrimaries( event );
}

ParticleGun* PrimaryGeneratorAction::get_particleGun() {
    return m_particleGun;
}