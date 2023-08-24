//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************

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
    m_command_fileName                                      = new G4UIcmdWithAString  ( "/output/fileName"                                      , this );
    m_command_photoSensor_hits_position_binned_save         = new G4UIcmdWithABool    ( "/output/photoSensor/hits/position/binned/save"         , this );
    m_command_photoSensor_hits_position_binned_nBinsPerSide = new G4UIcmdWithAnInteger( "/output/photoSensor/hits/position/binned/nBinsPerSide" , this );
    m_command_photoSensor_hits_position_save                = new G4UIcmdWithABool    ( "/output/photoSensor/hits/position/save"                , this );
    m_command_photoSensor_hits_time_save                    = new G4UIcmdWithABool    ( "/output/photoSensor/hits/time/save"                    , this );
    m_command_photoSensor_hits_process_save                 = new G4UIcmdWithABool    ( "/output/photoSensor/hits/process/save"                 , this );
    m_command_primary_position_save                         = new G4UIcmdWithABool    ( "/output/primary/position/save"                         , this );
    m_command_primary_emission_photon_save                  = new G4UIcmdWithABool    ( "/output/primary/emission/photon/save"                  , this );
    m_command_primary_emission_electron_save                = new G4UIcmdWithABool    ( "/output/primary/emission/electron/save"                , this );
    m_command_primary_process_save                          = new G4UIcmdWithABool    ( "/output/primary/process/save"                          , this );
    m_command_primary_time_save                             = new G4UIcmdWithABool    ( "/output/primary/time/save"                             , this );
    m_command_photon_length_save                            = new G4UIcmdWithABool    ( "/output/photon/length/save"                            , this );
    m_command_photon_process_save                           = new G4UIcmdWithABool    ( "/output/photon/process/save"                           , this );
    m_command_photon_time_save                              = new G4UIcmdWithABool    ( "/output/photon/time/save"                              , this );
}

OutputMessenger::~OutputMessenger() {
    if( m_command_GDML_save                                     ) delete m_command_GDML_save;
    if( m_command_GDML_fileName                                 ) delete m_command_GDML_fileName;
    if( m_command_fileName                                      ) delete m_command_fileName;
    if( m_command_photoSensor_hits_position_binned_save         ) delete m_command_photoSensor_hits_position_binned_save;
    if( m_command_photoSensor_hits_position_binned_nBinsPerSide ) delete m_command_photoSensor_hits_position_binned_nBinsPerSide;
    if( m_command_photoSensor_hits_position_save                ) delete m_command_photoSensor_hits_position_save;
    if( m_command_photoSensor_hits_time_save                    ) delete m_command_photoSensor_hits_time_save;
    if( m_command_photoSensor_hits_process_save                 ) delete m_command_photoSensor_hits_process_save;
    if( m_command_primary_position_save                         ) delete m_command_primary_position_save;
    if( m_command_primary_emission_photon_save                  ) delete m_command_primary_emission_photon_save;
    if( m_command_primary_emission_electron_save                ) delete m_command_primary_emission_electron_save;
    if( m_command_primary_process_save                          ) delete m_command_primary_process_save;
    if( m_command_primary_time_save                             ) delete m_command_primary_time_save;
    if( m_command_photon_length_save                            ) delete m_command_photon_length_save;
    if( m_command_photon_process_save                           ) delete m_command_photon_process_save;
    if( m_command_photon_time_save                              ) delete m_command_photon_time_save;
}

void OutputMessenger::SetNewValue( G4UIcommand* t_command, G4String t_newValue ) {
    if( t_command == m_command_GDML_save ) {
        set_GDML_save( m_command_GDML_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/GDML/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_GDML_fileName ) {
        set_GDML_fileName( t_newValue );
        G4cout << "Setting `/GDML/fileName' to " << t_newValue << G4endl;
    } else if( t_command == m_command_fileName ) {
        set_fileName( t_newValue );
        G4cout << "Setting `/output/fileName' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_position_binned_save ) {
        set_photoSensor_hits_position_binned_save( m_command_photoSensor_hits_position_binned_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photoSensor/hits/position/binned/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_position_binned_nBinsPerSide ) {
        set_photoSensor_hits_position_binned_nBinsPerSide( m_command_photoSensor_hits_position_binned_nBinsPerSide->GetNewIntValue( t_newValue ) );
        G4cout << "Setting `/output/photoSensor/hits/position/binned/nBinsPerSide' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_position_save ) {
        set_photoSensor_hits_position_save( m_command_photoSensor_hits_position_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photoSensor/hits/position/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_time_save ) {
        set_photoSensor_hits_time_save( m_command_photoSensor_hits_time_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photoSensor/hits/time/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_process_save ) {
        set_photoSensor_hits_process_save( m_command_photoSensor_hits_process_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photoSensor/hits/process/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_primary_position_save ) {
        set_primary_position_save( m_command_primary_position_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/primary/position/save' to " << t_newValue << G4endl;
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
    } else if( t_command == m_command_photon_length_save ) {
        set_photon_length_save( m_command_photon_length_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photon/length/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photon_process_save ) {
        set_photon_process_save( m_command_photon_process_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photon/process/save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photon_time_save ) {
        set_photon_time_save( m_command_photon_time_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `/output/photon/time/save' to " << t_newValue << G4endl;
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
G4String OutputMessenger::get_fileName() const {
    return m_variable_fileName;
}
G4bool OutputMessenger::get_photoSensor_hits_position_binned_save() const {
    return m_variable_photoSensor_hits_position_binned_save;
}
G4int OutputMessenger::get_photoSensor_hits_position_binned_nBinsPerSide() const {
    return m_variable_photoSensor_hits_position_binned_nBinsPerSide;
}
G4bool OutputMessenger::get_photoSensor_hits_position_save() const {
    return m_variable_photoSensor_hits_position_save;
}
G4bool OutputMessenger::get_photoSensor_hits_time_save() const {
    return m_variable_photoSensor_hits_time_save;
}
G4bool OutputMessenger::get_photoSensor_hits_process_save() const {
    return m_variable_photoSensor_hits_process_save;
}
G4bool OutputMessenger::get_primary_position_save() const {
    return m_variable_primary_position_save;
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
G4bool OutputMessenger::get_photon_length_save() const {
    return m_variable_photon_length_save;
}
G4bool OutputMessenger::get_photon_process_save() const {
    return m_variable_photon_process_save;
}
G4bool OutputMessenger::get_photon_time_save() const {
    return m_variable_photon_time_save;
}

void OutputMessenger::set_GDML_save( G4bool t_newValue ) {
    m_variable_GDML_save = t_newValue;
}
void OutputMessenger::set_GDML_fileName( G4String t_newValue ) {
    m_variable_GDML_fileName = t_newValue;
}
void OutputMessenger::set_fileName( G4String t_newValue ) {
    m_variable_fileName = t_newValue;
}
void OutputMessenger::set_photoSensor_hits_position_binned_save( G4bool t_newValue ) {
    m_variable_photoSensor_hits_position_binned_save = t_newValue;
}
void OutputMessenger::set_photoSensor_hits_position_binned_nBinsPerSide( G4int t_newValue ) {
    m_variable_photoSensor_hits_position_binned_nBinsPerSide = t_newValue;
}
void OutputMessenger::set_photoSensor_hits_position_save( G4bool t_newValue ) {
    m_variable_photoSensor_hits_position_save = t_newValue;
}
void OutputMessenger::set_photoSensor_hits_time_save( G4bool t_newValue ) {
    m_variable_photoSensor_hits_time_save = t_newValue;
}
void OutputMessenger::set_photoSensor_hits_process_save( G4bool t_newValue ) {
    m_variable_photoSensor_hits_process_save = t_newValue;
}
void OutputMessenger::set_primary_position_save( G4bool t_newValue ) {
    m_variable_primary_position_save = t_newValue;
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
void OutputMessenger::set_photon_length_save( G4bool t_newValue ) {
    m_variable_photon_length_save = t_newValue;
}
void OutputMessenger::set_photon_process_save( G4bool t_newValue ) {
    m_variable_photon_process_save = t_newValue;
}
void OutputMessenger::set_photon_time_save( G4bool t_newValue ) {
    m_variable_photon_time_save = t_newValue;
}