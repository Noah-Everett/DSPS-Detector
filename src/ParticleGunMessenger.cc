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
#include "ParticleGun.hh"

ParticleGunMessenger* ParticleGunMessenger::m_instance{ nullptr };

ParticleGunMessenger::ParticleGunMessenger() {
    m_parameter_momentum_random       = new G4UIcmdWithABool         ( "/particleGun/momentum/random"  , this );
    m_parameter_position_x_random     = new G4UIcmdWithABool         ( "/particleGun/position/x/random", this );
    m_parameter_position_y_random     = new G4UIcmdWithABool         ( "/particleGun/position/y/random", this );
    m_parameter_position_z_random     = new G4UIcmdWithABool         ( "/particleGun/position/z/random", this );
    m_parameter_position_x_nSteps     = new G4UIcmdWithAnInteger     ( "/particleGun/position/x/nSteps", this );
    m_parameter_position_y_nSteps     = new G4UIcmdWithAnInteger     ( "/particleGun/position/y/nSteps", this );
    m_parameter_position_z_nSteps     = new G4UIcmdWithAnInteger     ( "/particleGun/position/z/nSteps", this );
    m_parameter_position_x_random_min = new G4UIcmdWithADoubleAndUnit( "/particleGun/position/x/min"   , this );
    m_parameter_position_y_random_min = new G4UIcmdWithADoubleAndUnit( "/particleGun/position/y/min"   , this );
    m_parameter_position_z_random_min = new G4UIcmdWithADoubleAndUnit( "/particleGun/position/z/min"   , this );
    m_parameter_position_x_random_max = new G4UIcmdWithADoubleAndUnit( "/particleGun/position/x/max"   , this );
    m_parameter_position_y_random_max = new G4UIcmdWithADoubleAndUnit( "/particleGun/position/y/max"   , this );
    m_parameter_position_z_random_max = new G4UIcmdWithADoubleAndUnit( "/particleGun/position/z/max"   , this );
    m_parameter_nParticles            = new G4UIcmdWithAnInteger     ( "/particleGun/nParticles"       , this );

    m_parameter_momentum_random->SetDefaultValue( false );
}

ParticleGunMessenger::~ParticleGunMessenger() {
    if( m_parameter_momentum_random       ) delete m_parameter_momentum_random      ;
    if( m_parameter_position_x_random     ) delete m_parameter_position_x_random    ;
    if( m_parameter_position_y_random     ) delete m_parameter_position_y_random    ;
    if( m_parameter_position_z_random     ) delete m_parameter_position_z_random    ;
    if( m_parameter_position_x_nSteps     ) delete m_parameter_position_x_nSteps    ;
    if( m_parameter_position_y_nSteps     ) delete m_parameter_position_y_nSteps    ;
    if( m_parameter_position_z_nSteps     ) delete m_parameter_position_z_nSteps    ;
    if( m_parameter_position_x_random_min ) delete m_parameter_position_x_random_min;
    if( m_parameter_position_y_random_min ) delete m_parameter_position_y_random_min;
    if( m_parameter_position_z_random_min ) delete m_parameter_position_z_random_min;
    if( m_parameter_position_x_random_max ) delete m_parameter_position_x_random_max;
    if( m_parameter_position_y_random_max ) delete m_parameter_position_y_random_max;
    if( m_parameter_position_z_random_max ) delete m_parameter_position_z_random_max;
    if( m_parameter_nParticles            ) delete m_parameter_nParticles           ;
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
    } else if( t_command == m_parameter_position_x_random ) {
        set_position_x_random( m_parameter_position_x_random->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `position_x_random' to " << t_newValue << G4endl;
    } else if( t_command == m_parameter_position_y_random ) {
        set_position_y_random( m_parameter_position_y_random->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `position_y_random' to " << t_newValue << G4endl;
    } else if( t_command == m_parameter_position_z_random ) {
        set_position_z_random( m_parameter_position_z_random->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `position_z_random' to " << t_newValue << G4endl;
    } else if( t_command == m_parameter_position_x_nSteps ) {
        set_position_x_nSteps( m_parameter_position_x_nSteps->GetNewIntValue( t_newValue ) );
        G4cout << "Setting `position_x_nSteps' to " << t_newValue << G4endl;
    } else if( t_command == m_parameter_position_y_nSteps ) {
        set_position_y_nSteps( m_parameter_position_y_nSteps->GetNewIntValue( t_newValue ) );
        G4cout << "Setting `position_y_nSteps' to " << t_newValue << G4endl;
    } else if( t_command == m_parameter_position_z_nSteps ) {
        set_position_z_nSteps( m_parameter_position_z_nSteps->GetNewIntValue( t_newValue ) );
        G4cout << "Setting `position_z_nSteps' to " << t_newValue << G4endl;
    } else if( t_command == m_parameter_position_x_random_min ) {
        set_position_x_random_min( m_parameter_position_x_random_min->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `position_x_min' to " << t_newValue << G4endl;
    } else if( t_command == m_parameter_position_y_random_min ) {
        set_position_y_random_min( m_parameter_position_y_random_min->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `position_y_min' to " << t_newValue << G4endl;
    } else if( t_command == m_parameter_position_z_random_min ) {
        set_position_z_random_min( m_parameter_position_z_random_min->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `position_z_min' to " << t_newValue << G4endl;
    } else if( t_command == m_parameter_position_x_random_max ) {
        set_position_x_random_max( m_parameter_position_x_random_max->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `position_x_max' to " << t_newValue << G4endl;
    } else if( t_command == m_parameter_position_y_random_max ) {
        set_position_y_random_max( m_parameter_position_y_random_max->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `position_y_max' to " << t_newValue << G4endl;
    } else if( t_command == m_parameter_position_z_random_max ) {
        set_position_z_random_max( m_parameter_position_z_random_max->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `position_z_max' to " << t_newValue << G4endl;
    } else if( t_command == m_parameter_nParticles ) {
        set_nParticles( m_parameter_nParticles->GetNewIntValue( t_newValue ) );
        G4cout << "Setting `nParticles' to " << t_newValue << G4endl;
    } else
        G4Exception( "ParticleGunMessenger::SetNewValue", "Unknown command", FatalErrorInArgument, t_newValue );
}

G4bool ParticleGunMessenger::get_momentum_random() { 
    return m_variable_momentum_random; 
}
G4bool ParticleGunMessenger::get_position_x_random() { 
    return m_variable_position_x_random; 
}
G4bool ParticleGunMessenger::get_position_y_random() { 
    return m_variable_position_y_random; 
}
G4bool ParticleGunMessenger::get_position_z_random() { 
    return m_variable_position_z_random; 
}
G4int ParticleGunMessenger::get_position_x_nSteps() { 
    return m_variable_position_x_nSteps; 
}
G4int ParticleGunMessenger::get_position_y_nSteps() { 
    return m_variable_position_y_nSteps; 
}
G4int ParticleGunMessenger::get_position_z_nSteps() { 
    return m_variable_position_z_nSteps; 
}
G4double ParticleGunMessenger::get_position_x_random_min() { 
    return m_variable_position_x_random_min; 
}
G4double ParticleGunMessenger::get_position_y_random_min() { 
    return m_variable_position_y_random_min; 
}
G4double ParticleGunMessenger::get_position_z_random_min() { 
    return m_variable_position_z_random_min; 
}
G4double ParticleGunMessenger::get_position_x_random_max() { 
    return m_variable_position_x_random_max; 
}
G4double ParticleGunMessenger::get_position_y_random_max() { 
    return m_variable_position_y_random_max; 
}
G4double ParticleGunMessenger::get_position_z_random_max() { 
    return m_variable_position_z_random_max; 
}
G4int ParticleGunMessenger::get_nParticles() { 
    return m_variable_nParticles; 
}

void ParticleGunMessenger::set_momentum_random( G4bool t_variable_momentum_random ) { 
    m_variable_momentum_random = t_variable_momentum_random; 
}
void ParticleGunMessenger::set_position_x_random( G4bool t_variable_position_x_random ) { 
    m_variable_position_x_random = t_variable_position_x_random; 
}
void ParticleGunMessenger::set_position_y_random( G4bool t_variable_position_y_random ) { 
    m_variable_position_y_random = t_variable_position_y_random; 
}
void ParticleGunMessenger::set_position_z_random( G4bool t_variable_position_z_random ) { 
    m_variable_position_z_random = t_variable_position_z_random; 
}
void ParticleGunMessenger::set_position_x_nSteps( G4int t_variable_position_x_nSteps ) { 
    m_variable_position_x_nSteps = t_variable_position_x_nSteps; 
}
void ParticleGunMessenger::set_position_y_nSteps( G4int t_variable_position_y_nSteps ) { 
    m_variable_position_y_nSteps = t_variable_position_y_nSteps; 
}
void ParticleGunMessenger::set_position_z_nSteps( G4int t_variable_position_z_nSteps ) { 
    m_variable_position_z_nSteps = t_variable_position_z_nSteps; 
}
void ParticleGunMessenger::set_position_x_random_min( G4double t_variable_position_x_random_min ) { 
    m_variable_position_x_random_min = t_variable_position_x_random_min; 
}
void ParticleGunMessenger::set_position_y_random_min( G4double t_variable_position_y_random_min ) { 
    m_variable_position_y_random_min = t_variable_position_y_random_min; 
}
void ParticleGunMessenger::set_position_z_random_min( G4double t_variable_position_z_random_min ) { 
    m_variable_position_z_random_min = t_variable_position_z_random_min; 
}
void ParticleGunMessenger::set_position_x_random_max( G4double t_variable_position_x_random_max ) { 
    m_variable_position_x_random_max = t_variable_position_x_random_max; 
}
void ParticleGunMessenger::set_position_y_random_max( G4double t_variable_position_y_random_max ) { 
    m_variable_position_y_random_max = t_variable_position_y_random_max; 
}
void ParticleGunMessenger::set_position_z_random_max( G4double t_variable_position_z_random_max ) { 
    m_variable_position_z_random_max = t_variable_position_z_random_max; 
}
void ParticleGunMessenger::set_nParticles( G4int t_variable_nParticles ) { 
    m_variable_nParticles = t_variable_nParticles; 

    if( m_primaryGeneratorActions.size() > 0 )
        for( ParticleGun* particleGun : m_particleGuns )
            particleGun->SetNumberOfParticles( m_variable_nParticles );
}

void ParticleGunMessenger::add_primaryGeneratorAction( PrimaryGeneratorAction* t_primaryGeneratorAction ) {
    m_primaryGeneratorActions.push_back( t_primaryGeneratorAction );
}

void ParticleGunMessenger::add_particleGun( ParticleGun* t_particleGun ) {
    m_particleGuns.push_back( t_particleGun );
}