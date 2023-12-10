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

#include "OutputMessenger.hh"

OutputMessenger* OutputMessenger::m_instance{ nullptr };

OutputMessenger* OutputMessenger::get_instance() {
    if( !m_instance )
        m_instance = new OutputMessenger();
    
    return m_instance;
}

void OutputMessenger::delete_instance() {
    if( m_instance ) {
        delete m_instance;
        m_instance = nullptr;
    }
}

OutputMessenger::OutputMessenger() {
    m_command_GDML_save                                     = new G4UIcmdWithABool    ( "/GDML/save"                                            , this );
    m_command_GDML_fileName                                 = new G4UIcmdWithAString  ( "/GDML/fileName"                                        , this );
    m_command_photoSensor_hits_position_binned_save         = new G4UIcmdWithABool    ( "/output/photoSensor/hits/position/binned/save"         , this );
    m_command_photoSensor_hits_position_binned_nBinsPerSide = new G4UIcmdWithAnInteger( "/output/photoSensor/hits/position/binned/nBinsPerSide" , this );
    m_command_photoSensor_hits_position_absolute_save       = new G4UIcmdWithABool    ( "/output/photoSensor/hits/position/absolute/save"       , this );
    m_command_photoSensor_hits_position_relative_save       = new G4UIcmdWithABool    ( "/output/photoSensor/hits/position/relative/save"       , this );
    m_command_photoSensor_hits_position_initial_save        = new G4UIcmdWithABool    ( "/output/photoSensor/hits/position/initial/save"        , this );
    m_command_photoSensor_hits_time_save                    = new G4UIcmdWithABool    ( "/output/photoSensor/hits/time/save"                    , this );
    m_command_photoSensor_hits_process_save                 = new G4UIcmdWithABool    ( "/output/photoSensor/hits/process/save"                 , this );
    m_command_photoSensor_hits_photoSensorID_save           = new G4UIcmdWithABool    ( "/output/photoSensor/hits/photoSensorID/save"           , this );
    m_command_photoSensor_hits_energy_save                  = new G4UIcmdWithABool    ( "/output/photoSensor/hits/energy/save"                  , this );
    m_command_calorimeter_hits_position_absolute_save       = new G4UIcmdWithABool    ( "/output/calorimeter/hits/position/absolute/save"       , this );
    m_command_calorimeter_hits_position_relative_save       = new G4UIcmdWithABool    ( "/output/calorimeter/hits/position/relative/save"       , this );
    m_command_calorimeter_hits_position_initial_save        = new G4UIcmdWithABool    ( "/output/calorimeter/hits/position/initial/save"        , this );
    m_command_calorimeter_hits_time_save                    = new G4UIcmdWithABool    ( "/output/calorimeter/hits/time/save"                    , this );
    m_command_calorimeter_hits_process_save                 = new G4UIcmdWithABool    ( "/output/calorimeter/hits/process/save"                 , this );
    m_command_calorimeter_hits_calorimeterID_save           = new G4UIcmdWithABool    ( "/output/calorimeter/hits/calorimeterID/save"           , this );
    m_command_calorimeter_hits_energy_save                  = new G4UIcmdWithABool    ( "/output/calorimeter/hits/energy/save"                  , this );
    m_command_primary_position_save                         = new G4UIcmdWithABool    ( "/output/primary/position/save"                         , this );
    m_command_primary_direction_save                        = new G4UIcmdWithABool    ( "/output/primary/direction/save"                        , this );
    m_command_primary_emission_photon_save                  = new G4UIcmdWithABool    ( "/output/primary/emission/photon/save"                  , this );
    m_command_primary_emission_electron_save                = new G4UIcmdWithABool    ( "/output/primary/emission/electron/save"                , this );
    m_command_primary_process_save                          = new G4UIcmdWithABool    ( "/output/primary/process/save"                          , this );
    m_command_primary_time_save                             = new G4UIcmdWithABool    ( "/output/primary/time/save"                             , this );
    m_command_primary_energy_save                           = new G4UIcmdWithABool    ( "/output/primary/energy/save"                           , this );
    m_command_primary_volume_save                           = new G4UIcmdWithABool    ( "/output/primary/volume/save"                           , this );
    m_command_primary_pdg_save                              = new G4UIcmdWithABool    ( "/output/primary/pdg/save"                              , this );
    m_command_photon_length_save                            = new G4UIcmdWithABool    ( "/output/photon/length/save"                            , this );
    m_command_photon_process_save                           = new G4UIcmdWithABool    ( "/output/photon/process/save"                           , this );
    m_command_photon_time_save                              = new G4UIcmdWithABool    ( "/output/photon/time/save"                              , this );
    m_command_photon_position_save                          = new G4UIcmdWithABool    ( "/output/photon/position/save"                          , this );
    m_command_photon_direction_save                         = new G4UIcmdWithABool    ( "/output/photon/direction/save"                         , this );
    m_command_photon_energy_save                            = new G4UIcmdWithABool    ( "/output/photon/energy/save"                            , this );
    m_command_photon_volume_save                            = new G4UIcmdWithABool    ( "/output/photon/volume/save"                            , this );
    m_command_photon_stepNumber_save                        = new G4UIcmdWithABool    ( "/output/photon/stepNumber/save"                        , this );
}

OutputMessenger::~OutputMessenger() {
    if( m_command_GDML_save                                     ) delete m_command_GDML_save;
    if( m_command_GDML_fileName                                 ) delete m_command_GDML_fileName;
    if( m_command_photoSensor_hits_position_binned_save         ) delete m_command_photoSensor_hits_position_binned_save;
    if( m_command_photoSensor_hits_position_binned_nBinsPerSide ) delete m_command_photoSensor_hits_position_binned_nBinsPerSide;
    if( m_command_photoSensor_hits_position_absolute_save       ) delete m_command_photoSensor_hits_position_absolute_save;
    if( m_command_photoSensor_hits_position_relative_save       ) delete m_command_photoSensor_hits_position_relative_save;
    if( m_command_photoSensor_hits_position_initial_save        ) delete m_command_photoSensor_hits_position_initial_save; 
    if( m_command_photoSensor_hits_time_save                    ) delete m_command_photoSensor_hits_time_save;
    if( m_command_photoSensor_hits_process_save                 ) delete m_command_photoSensor_hits_process_save;
    if( m_command_photoSensor_hits_photoSensorID_save           ) delete m_command_photoSensor_hits_photoSensorID_save;
    if( m_command_photoSensor_hits_energy_save                  ) delete m_command_photoSensor_hits_energy_save;
    if( m_command_calorimeter_hits_position_absolute_save       ) delete m_command_calorimeter_hits_position_absolute_save;
    if( m_command_calorimeter_hits_position_relative_save       ) delete m_command_calorimeter_hits_position_relative_save;
    if( m_command_calorimeter_hits_position_initial_save        ) delete m_command_calorimeter_hits_position_initial_save;
    if( m_command_calorimeter_hits_time_save                    ) delete m_command_calorimeter_hits_time_save;
    if( m_command_calorimeter_hits_process_save                 ) delete m_command_calorimeter_hits_process_save;
    if( m_command_calorimeter_hits_calorimeterID_save           ) delete m_command_calorimeter_hits_calorimeterID_save;
    if( m_command_calorimeter_hits_energy_save                  ) delete m_command_calorimeter_hits_energy_save;
    if( m_command_primary_position_save                         ) delete m_command_primary_position_save;
    if( m_command_primary_direction_save                        ) delete m_command_primary_direction_save;
    if( m_command_primary_emission_photon_save                  ) delete m_command_primary_emission_photon_save;
    if( m_command_primary_emission_electron_save                ) delete m_command_primary_emission_electron_save;
    if( m_command_primary_process_save                          ) delete m_command_primary_process_save;
    if( m_command_primary_time_save                             ) delete m_command_primary_time_save;
    if( m_command_primary_energy_save                           ) delete m_command_primary_energy_save;
    if( m_command_primary_volume_save                           ) delete m_command_primary_volume_save;
    if( m_command_primary_pdg_save                              ) delete m_command_primary_pdg_save;
    if( m_command_photon_length_save                            ) delete m_command_photon_length_save;
    if( m_command_photon_process_save                           ) delete m_command_photon_process_save;
    if( m_command_photon_time_save                              ) delete m_command_photon_time_save;
    if( m_command_photon_position_save                          ) delete m_command_photon_position_save;
    if( m_command_photon_direction_save                         ) delete m_command_photon_direction_save;
    if( m_command_photon_energy_save                            ) delete m_command_photon_energy_save;
    if( m_command_photon_volume_save                            ) delete m_command_photon_volume_save;
    if( m_command_photon_stepNumber_save                        ) delete m_command_photon_stepNumber_save;
}

void OutputMessenger::SetNewValue( G4UIcommand* t_command, G4String t_newValue ) {
    if( t_command == m_command_GDML_save ) {
        set_GDML_save( m_command_GDML_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/GDML/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_GDML_fileName ) {
        set_GDML_fileName( t_newValue );
        G4cout << "Setting `/GDML/fileName' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_position_binned_save ) {
        set_photoSensor_hits_position_binned_save( m_command_photoSensor_hits_position_binned_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photoSensor/hits/position/binned/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_position_binned_nBinsPerSide ) {
        set_photoSensor_hits_position_binned_nBinsPerSide( m_command_photoSensor_hits_position_binned_nBinsPerSide->GetNewIntValue( t_newValue ) );
        G4cout << "Setting `/output/photoSensor/hits/position/binned/nBinsPerSide' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_position_absolute_save ) {
        set_photoSensor_hits_position_absolute_save( m_command_photoSensor_hits_position_absolute_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photoSensor/hits/position/absolute/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_position_relative_save ) {
        set_photoSensor_hits_position_relative_save( m_command_photoSensor_hits_position_relative_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photoSensor/hits/position/relative/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_position_initial_save ) {
        set_photoSensor_hits_position_initial_save( m_command_photoSensor_hits_position_initial_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photoSensor/hits/position/initial/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_time_save ) {
        set_photoSensor_hits_time_save( m_command_photoSensor_hits_time_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photoSensor/hits/time/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_process_save ) {
        set_photoSensor_hits_process_save( m_command_photoSensor_hits_process_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photoSensor/hits/process/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_photoSensorID_save ) {
        set_photoSensor_hits_photoSensorID_save( m_command_photoSensor_hits_photoSensorID_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photoSensor/hits/photoSensorID/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_energy_save ) {
        set_photoSensor_hits_energy_save( m_command_photoSensor_hits_energy_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photoSensor/hits/energy/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_hits_position_absolute_save ) {
        set_calorimeter_hits_position_absolute_save( m_command_calorimeter_hits_position_absolute_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/calorimeter/hits/position/absolute/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_hits_position_relative_save ) {
        set_calorimeter_hits_position_relative_save( m_command_calorimeter_hits_position_relative_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/calorimeter/hits/position/relative/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_hits_position_initial_save ) {
        set_calorimeter_hits_position_initial_save( m_command_calorimeter_hits_position_initial_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/calorimeter/hits/position/initial/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_hits_time_save ) {
        set_calorimeter_hits_time_save( m_command_calorimeter_hits_time_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/calorimeter/hits/time/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_hits_process_save ) {
        set_calorimeter_hits_process_save( m_command_calorimeter_hits_process_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/calorimeter/hits/process/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_hits_calorimeterID_save ) {
        set_calorimeter_hits_calorimeterID_save( m_command_calorimeter_hits_calorimeterID_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/calorimeter/hits/calorimeterID/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_hits_energy_save ) {
        set_calorimeter_hits_energy_save( m_command_calorimeter_hits_energy_save->GetNewBoolValue( t_newValue ) );
    } else if( t_command == m_command_primary_position_save ) {
        set_primary_position_save( m_command_primary_position_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/primary/position/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_primary_direction_save ) {
        set_primary_direction_save( m_command_primary_direction_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/primary/direction/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_primary_emission_photon_save ) {
        set_primary_emission_photon_save( m_command_primary_emission_photon_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/primary/emission/photon/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_primary_emission_electron_save ) {
        set_primary_emission_electron_save( m_command_primary_emission_electron_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/primary/emission/photon/electron/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_primary_process_save ) {
        set_primary_process_save( m_command_primary_process_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/primary/process/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_primary_time_save ) {
        set_primary_time_save( m_command_primary_time_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/primary/time/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_primary_energy_save ) {
        set_primary_energy_save( m_command_primary_energy_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/primary/energy/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_primary_volume_save ) {
        set_primary_volume_save( m_command_primary_volume_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/primary/volume/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_primary_pdg_save ) {
        set_primary_pdg_save( m_command_primary_pdg_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/primary/pdg/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photon_length_save ) {
        set_photon_length_save( m_command_photon_length_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photon/length/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photon_process_save ) {
        set_photon_process_save( m_command_photon_process_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photon/process/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photon_time_save ) {
        set_photon_time_save( m_command_photon_time_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photon/time/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photon_position_save ) {
        set_photon_position_save( m_command_photon_position_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photon/position/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photon_direction_save ) {
        set_photon_direction_save( m_command_photon_direction_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photon/direction/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photon_energy_save ) {
        set_photon_energy_save( m_command_photon_energy_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photon/energy/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photon_volume_save ) {
        set_photon_volume_save( m_command_photon_volume_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photon/volume/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photon_stepNumber_save ) {
        set_photon_stepNumber_save( m_command_photon_stepNumber_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photon/stepNumber/save' to " << t_newValue << G4endl;
    } else {
        G4Exception( "OutputMessenger::SetNewValue()", "Invalid command", FatalErrorInArgument, "Command not found" );
    }
}

G4bool OutputMessenger::get_GDML_save() const {
    return m_variable_GDML_save;
}
G4String OutputMessenger::get_GDML_fileName() const {
    return m_variable_GDML_fileName;
}
G4bool OutputMessenger::get_photoSensor_hits_position_binned_save() const {
    return m_variable_photoSensor_hits_position_binned_save;
}
G4int OutputMessenger::get_photoSensor_hits_position_binned_nBinsPerSide() const {
    return m_variable_photoSensor_hits_position_binned_nBinsPerSide;
}
G4bool OutputMessenger::get_photoSensor_hits_position_absolute_save() const {
    return m_variable_photoSensor_hits_position_absolute_save;
}
G4bool OutputMessenger::get_photoSensor_hits_position_relative_save() const {
    return m_variable_photoSensor_hits_position_relative_save;
}
G4bool OutputMessenger::get_photoSensor_hits_position_initial_save() const {
    return m_variable_photoSensor_hits_position_initial_save;
}
G4bool OutputMessenger::get_photoSensor_hits_time_save() const {
    return m_variable_photoSensor_hits_time_save;
}
G4bool OutputMessenger::get_photoSensor_hits_process_save() const {
    return m_variable_photoSensor_hits_process_save;
}
G4bool OutputMessenger::get_photoSensor_hits_photoSensorID_save() const {
    return m_variable_photoSensor_hits_photoSensorID_save;
}
G4bool OutputMessenger::get_photoSensor_hits_energy_save() const {
    return m_variable_photoSensor_hits_energy_save;
}
G4bool OutputMessenger::get_calorimeter_hits_position_absolute_save() const {
    return m_variable_calorimeter_hits_position_absolute_save;
}
G4bool OutputMessenger::get_calorimeter_hits_position_relative_save() const {
    return m_variable_calorimeter_hits_position_relative_save;
}
G4bool OutputMessenger::get_calorimeter_hits_position_initial_save() const {
    return m_variable_calorimeter_hits_position_initial_save;
}
G4bool OutputMessenger::get_calorimeter_hits_time_save() const {
    return m_variable_calorimeter_hits_time_save;
}
G4bool OutputMessenger::get_calorimeter_hits_process_save() const {
    return m_variable_calorimeter_hits_process_save;
}
G4bool OutputMessenger::get_calorimeter_hits_calorimeterID_save() const {
    return m_variable_calorimeter_hits_calorimeterID_save;
}
G4bool OutputMessenger::get_calorimeter_hits_energy_save() const {
    return m_variable_calorimeter_hits_energy_save;
}
G4bool OutputMessenger::get_primary_position_save() const {
    return m_variable_primary_position_save;
}
G4bool OutputMessenger::get_primary_direction_save() const {
    return m_variable_primary_direction_save;
}
G4bool OutputMessenger::get_primary_emission_photon_save() const {
    return m_variable_primary_emission_photon_save;
}
G4bool OutputMessenger::get_primary_emission_electron_save() const {
    return m_variable_primary_emission_electron_save;
}
G4bool OutputMessenger::get_primary_process_save() const {
    return m_variable_primary_process_save;
}
G4bool OutputMessenger::get_primary_time_save() const {
    return m_variable_primary_time_save;
}
G4bool OutputMessenger::get_primary_energy_save() const {
    return m_variable_primary_energy_save;
}
G4bool OutputMessenger::get_primary_volume_save() const {
    return m_variable_primary_volume_save;
}
G4bool OutputMessenger::get_primary_pdg_save() const {
    return m_variable_primary_pdg_save;
}
G4bool OutputMessenger::get_photon_length_save() const {
    return m_variable_photon_length_save;
}
G4bool OutputMessenger::get_photon_process_save() const {
    return m_variable_photon_process_save;
}
G4bool OutputMessenger::get_photon_time_save() const {
    return m_variable_photon_time_save;
}
G4bool OutputMessenger::get_photon_position_save() const {
    return m_variable_photon_position_save;
}
G4bool OutputMessenger::get_photon_direction_save() const {
    return m_variable_photon_direction_save;
}
G4bool OutputMessenger::get_photon_energy_save() const {
    return m_variable_photon_energy_save;
}
G4bool OutputMessenger::get_photon_volume_save() const {
    return m_variable_photon_volume_save;
}
G4bool OutputMessenger::get_photon_stepNumber_save() const {
    return m_variable_photon_stepNumber_save;
}

void OutputMessenger::set_GDML_save( G4bool t_newValue ) {
    m_variable_GDML_save = t_newValue;
}
void OutputMessenger::set_GDML_fileName( G4String t_newValue ) {
    m_variable_GDML_fileName = t_newValue;
}
void OutputMessenger::set_photoSensor_hits_position_binned_save( G4bool t_newValue ) {
    m_variable_photoSensor_hits_position_binned_save = t_newValue;
}
void OutputMessenger::set_photoSensor_hits_position_binned_nBinsPerSide( G4int t_newValue ) {
    m_variable_photoSensor_hits_position_binned_nBinsPerSide = t_newValue;
}
void OutputMessenger::set_photoSensor_hits_position_absolute_save( G4bool t_newValue ) {
    m_variable_photoSensor_hits_position_absolute_save = t_newValue;
}
void OutputMessenger::set_photoSensor_hits_position_relative_save( G4bool t_newValue ) {
    m_variable_photoSensor_hits_position_relative_save = t_newValue;
}
void OutputMessenger::set_photoSensor_hits_position_initial_save( G4bool t_newValue ) {
    m_variable_photoSensor_hits_position_initial_save = t_newValue;
}
void OutputMessenger::set_photoSensor_hits_time_save( G4bool t_newValue ) {
    m_variable_photoSensor_hits_time_save = t_newValue;
}
void OutputMessenger::set_photoSensor_hits_process_save( G4bool t_newValue ) {
    m_variable_photoSensor_hits_process_save = t_newValue;
}
void OutputMessenger::set_photoSensor_hits_photoSensorID_save( G4bool t_newValue ) {
    m_variable_photoSensor_hits_photoSensorID_save = t_newValue;
}
void OutputMessenger::set_photoSensor_hits_energy_save( G4bool t_newValue ) {
    m_variable_photoSensor_hits_energy_save = t_newValue;
}
void OutputMessenger::set_calorimeter_hits_position_absolute_save( G4bool t_newValue ) {
    m_variable_calorimeter_hits_position_absolute_save = t_newValue;
}
void OutputMessenger::set_calorimeter_hits_position_relative_save( G4bool t_newValue ) {
    m_variable_calorimeter_hits_position_relative_save = t_newValue;
}
void OutputMessenger::set_calorimeter_hits_position_initial_save( G4bool t_newValue ) {
    m_variable_calorimeter_hits_position_initial_save = t_newValue;
}
void OutputMessenger::set_calorimeter_hits_time_save( G4bool t_newValue ) {
    m_variable_calorimeter_hits_time_save = t_newValue;
}
void OutputMessenger::set_calorimeter_hits_process_save( G4bool t_newValue ) {
    m_variable_calorimeter_hits_process_save = t_newValue;
}
void OutputMessenger::set_calorimeter_hits_calorimeterID_save( G4bool t_newValue ) {
    m_variable_calorimeter_hits_calorimeterID_save = t_newValue;
}
void OutputMessenger::set_calorimeter_hits_energy_save( G4bool t_newValue ) {
    m_variable_calorimeter_hits_energy_save = t_newValue;
}
void OutputMessenger::set_primary_position_save( G4bool t_newValue ) {
    m_variable_primary_position_save = t_newValue;
}
void OutputMessenger::set_primary_direction_save( G4bool t_newValue ) {
    m_variable_primary_direction_save = t_newValue;
}
void OutputMessenger::set_primary_emission_photon_save( G4bool t_newValue ) {
    m_variable_primary_emission_photon_save = t_newValue;
}
void OutputMessenger::set_primary_emission_electron_save( G4bool t_newValue ) {
    m_variable_primary_emission_electron_save = t_newValue;
}
void OutputMessenger::set_primary_process_save( G4bool t_newValue ) {
    m_variable_primary_process_save = t_newValue;
}
void OutputMessenger::set_primary_time_save( G4bool t_newValue ) {
    m_variable_primary_time_save = t_newValue;
}
void OutputMessenger::set_primary_energy_save( G4bool t_newValue ) {
    m_variable_primary_energy_save = t_newValue;
}
void OutputMessenger::set_primary_volume_save( G4bool t_newValue ) {
    m_variable_primary_volume_save = t_newValue;
}
void OutputMessenger::set_primary_pdg_save( G4bool t_newValue ) {
    m_variable_primary_pdg_save = t_newValue;
}
void OutputMessenger::set_photon_length_save( G4bool t_newValue ) {
    m_variable_photon_length_save = t_newValue;
}
void OutputMessenger::set_photon_process_save( G4bool t_newValue ) {
    m_variable_photon_process_save = t_newValue;
}
void OutputMessenger::set_photon_time_save( G4bool t_newValue ) {
    m_variable_photon_time_save = t_newValue;
}
void OutputMessenger::set_photon_position_save( G4bool t_newValue ) {
    m_variable_photon_position_save = t_newValue;
}
void OutputMessenger::set_photon_direction_save( G4bool t_newValue ) {
    m_variable_photon_direction_save = t_newValue;
}
void OutputMessenger::set_photon_energy_save( G4bool t_newValue ) {
    m_variable_photon_energy_save = t_newValue;
}
void OutputMessenger::set_photon_volume_save( G4bool t_newValue ) {
    m_variable_photon_volume_save = t_newValue;
}
void OutputMessenger::set_photon_stepNumber_save( G4bool t_newValue ) {
    m_variable_photon_stepNumber_save = t_newValue;
}