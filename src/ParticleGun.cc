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

G4double ParticleGun::get_position_random( G4double t_min, G4double t_max , G4int nSteps ) {
    if( nSteps <= 0 ) {
        return G4UniformRand() * ( t_max - t_min ) + t_min;
    } else {
        
        // G4cout << "int( G4UniformRand() * nSteps ) = " << int( G4UniformRand() * nSteps ) << G4endl;
        // G4cout << "Initial Position: " << t_min + ( t_max - t_min ) * int( G4UniformRand() * nSteps ) / double( nSteps ) << G4endl;
        // G4cout << G4endl;
        return t_min + ( t_max - t_min ) * int( G4UniformRand() * nSteps ) / double( nSteps - 1 );
    }
}

G4ThreeVector ParticleGun::get_momentum_random( G4double t_x_min, G4double t_x_max, 
                                                G4double t_y_min, G4double t_y_max, 
                                                G4double t_z_min, G4double t_z_max ) {
    G4ThreeVector momentum = G4RandomDirection();
    while ( momentum.x() < t_x_min || momentum.x() > t_x_max ||
            momentum.y() < t_y_min || momentum.y() > t_y_max ||
            momentum.z() < t_z_min || momentum.z() > t_z_max   ) {
        momentum = G4RandomDirection();
    }

    return momentum;
}

void ParticleGun::GeneratePrimaries( G4Event* t_event ) {
    if( particle_definition == 0 )
        return;

    // create a new vertex
    G4ThreeVector position = particle_position;
    if( m_particleGunMessenger->get_position_x_random() )
        position.setX( get_position_random( m_particleGunMessenger->get_position_x_random_min(),
                                            m_particleGunMessenger->get_position_x_random_max(),
                                            m_particleGunMessenger->get_position_x_nSteps    () ) );
    if( m_particleGunMessenger->get_position_y_random() )
        position.setY( get_position_random( m_particleGunMessenger->get_position_y_random_min(),
                                            m_particleGunMessenger->get_position_y_random_max(),
                                            m_particleGunMessenger->get_position_y_nSteps    () ) );
    if( m_particleGunMessenger->get_position_z_random() )
        position.setZ( get_position_random( m_particleGunMessenger->get_position_z_random_min(),
                                            m_particleGunMessenger->get_position_z_random_max(),
                                            m_particleGunMessenger->get_position_z_nSteps    () ) );
    G4PrimaryVertex* vertex = new G4PrimaryVertex( position, particle_time );

    // create new primaries and set them to the vertex
    G4double mass =  particle_definition->GetPDGMass();
    for( G4int i{ 0 }; i < NumberOfParticlesToBeGenerated; i++ ){
        G4PrimaryParticle* particle{ nullptr };
        if( particle_definition->GetParticleName() == "PhotonCreator" ) {
            particle = new G4PrimaryParticle( G4OpticalPhoton::Definition() );
            particle->SetKineticEnergy( 6.974754362888755 * eV ); // fit from NEST data
            // G4double energy = G4RandGauss::shoot( 24.12 * eV, 1979. * eV );
            // if( energy > 24.12 * eV / 2. )
            //     particle->SetKineticEnergy( energy );
            // else
            //     particle->SetKineticEnergy( 24.12 * eV / 2. );
            particle->SetMass( G4OpticalPhoton::Definition()->GetPDGMass() );
            particle->SetCharge( G4OpticalPhoton::Definition()->GetPDGCharge() );
        } else {
            particle = new G4PrimaryParticle( particle_definition );
            particle->SetKineticEnergy( particle_energy );
            particle->SetMass( mass );
            particle->SetCharge( particle_charge );
        }

        if( m_particleGunMessenger->get_momentum_random() )
            particle->SetMomentumDirection( get_momentum_random( m_particleGunMessenger->get_momentum_x_random_min(),
                                                                 m_particleGunMessenger->get_momentum_x_random_max(),
                                                                 m_particleGunMessenger->get_momentum_y_random_min(),
                                                                 m_particleGunMessenger->get_momentum_y_random_max(),
                                                                 m_particleGunMessenger->get_momentum_z_random_min(),
                                                                 m_particleGunMessenger->get_momentum_z_random_max() ) );
        else
            particle->SetMomentumDirection( particle_momentum_direction );

        particle->SetPolarization( particle_polarization.x(),
                                   particle_polarization.y(),
                                   particle_polarization.z() );

        vertex->SetPrimary( particle );
    }
    t_event->AddPrimaryVertex( vertex );
}