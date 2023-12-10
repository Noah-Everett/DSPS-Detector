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

#include "ParticleGunMessenger.hh"

ParticleGunMessenger* ParticleGunMessenger::m_instance{ nullptr };

ParticleGunMessenger::ParticleGunMessenger() {
    m_parameter_momentum_random = new G4UIcmdWithABool( "/particleGun/momentum/random", this );

    m_parameter_momentum_random->SetDefaultValue( false );
}

ParticleGunMessenger::~ParticleGunMessenger() {
    if( m_parameter_momentum_random ) {
        delete m_parameter_momentum_random;
        m_parameter_momentum_random = nullptr;
    }
}

ParticleGunMessenger* ParticleGunMessenger::get_instance() {
    if( !m_instance ) 
        m_instance = new ParticleGunMessenger();
    return m_instance;
}

void ParticleGunMessenger::delete_instance() {
    if( m_instance ) {
        delete m_instance;
        m_instance = nullptr;
    }
}

void ParticleGunMessenger::SetNewValue( G4UIcommand* t_command, G4String t_newValue ) {
    if( t_command == m_parameter_momentum_random ) {
        set_momentum_random( m_parameter_momentum_random->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `momentum_random' to " << t_newValue << G4endl;
    }
}