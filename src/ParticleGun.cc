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

#include "ParticleGun.hh"

ParticleGun::ParticleGun() :
    G4ParticleGun() {
}

ParticleGun::ParticleGun( G4int t_nParticles ) :
    G4ParticleGun( t_nParticles ) {
}

ParticleGun::~ParticleGun() {
}

void ParticleGun::GeneratePrimaries( G4Event* t_event ) {
    if( particle_definition == 0 )
        return;

    // create a new vertex
    G4PrimaryVertex* vertex = new G4PrimaryVertex( particle_position, particle_time );

    // create new primaries and set them to the vertex
    G4double mass =  particle_definition->GetPDGMass();
    for( G4int i{ 0 }; i < NumberOfParticlesToBeGenerated; i++ ){
        G4PrimaryParticle* particle{ nullptr };
        if( particle_definition->GetParticleName() == "PhotonCreator" ) {
            particle = new G4PrimaryParticle( G4OpticalPhoton::Definition() );
            particle->SetMomentumDirection( G4RandomDirection() );
            particle->SetKineticEnergy( 24.12 * eV );
            // G4double energy = G4RandGauss::shoot( 24.12 * eV, 1979. * eV );
            // if( energy > 24.12 * eV / 2. )
            //     particle->SetKineticEnergy( energy );
            // else
            //     particle->SetKineticEnergy( 24.12 * eV / 2. );
            particle->SetMass( G4OpticalPhoton::Definition()->GetPDGMass() );
            particle->SetCharge( G4OpticalPhoton::Definition()->GetPDGCharge() );
        } else {
            particle = new G4PrimaryParticle( particle_definition );
            if( m_particleGunMessenger->get_momentum_random() )
                particle->SetMomentumDirection( G4RandomDirection() );
            else
                particle->SetMomentumDirection( particle_momentum_direction );
            particle->SetKineticEnergy( particle_energy );
            particle->SetMass( mass );
            particle->SetCharge( particle_charge );
        }
        particle->SetPolarization( particle_polarization.x(),
                                   particle_polarization.y(),
                                   particle_polarization.z() );
        vertex->SetPrimary( particle );
    }
    t_event->AddPrimaryVertex( vertex );
}