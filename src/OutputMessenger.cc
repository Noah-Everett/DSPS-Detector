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
    m_command_photoSensor_hits_save     = new G4UIcmdWithABool  ( "/output/photoSensor/hits/save"    , this );
    m_command_photoSensor_hits_fileName = new G4UIcmdWithAString( "/output/photoSensor/hits/fileName", this );
    m_command_GDML_save                 = new G4UIcmdWithABool  ( "/output/GDML/save"                , this );
    m_command_GDML_fileName             = new G4UIcmdWithAString( "/output/GDML/fileName"            , this );
}

OutputMessenger::~OutputMessenger() {
    if( m_command_photoSensor_hits_save     ) delete m_command_photoSensor_hits_save    ;
    if( m_command_photoSensor_hits_fileName ) delete m_command_photoSensor_hits_fileName;
    if( m_command_GDML_save                 ) delete m_command_GDML_save                ;
    if( m_command_GDML_fileName             ) delete m_command_GDML_fileName            ;
}

void OutputMessenger::SetNewValue( G4UIcommand* t_command, G4String t_newValue ) {
    if( t_command == m_command_photoSensor_hits_save ) {
        set_photoSensor_hits_save( m_command_photoSensor_hits_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `photoSensor_hits_save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_hits_fileName ) {
        set_photoSensor_hits_fileName( t_newValue );
        G4cout << "Setting `photoSensor_hits_fileName' to " << t_newValue << G4endl;
    } else if( t_command == m_command_GDML_save ) {
        set_GDML_save( m_command_GDML_save->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `GDML_save' to " << t_newValue << G4endl;
    } else if( t_command == m_command_GDML_fileName ) {
        set_GDML_fileName(  t_newValue );
        G4cout << "Setting `GDML_fileName' to " << t_newValue << G4endl;
    }
}

G4bool OutputMessenger::get_photoSensor_hits_save() const {
    return m_variable_photoSensor_hits_save;
}
G4String OutputMessenger::get_photoSensor_hits_fileName() const {
    return m_variable_photoSensor_hits_fileName;
}
G4bool OutputMessenger::get_GDML_save() const {
    return m_variable_GDML_save;
}
G4String OutputMessenger::get_GDML_fileName() const {
    return m_variable_GDML_fileName;
}

void OutputMessenger::set_photoSensor_hits_save( G4bool t_variable_photoSensor_hits_save ) {
    m_variable_photoSensor_hits_save = t_variable_photoSensor_hits_save;
}
void OutputMessenger::set_photoSensor_hits_fileName( G4String t_variable_photoSensor_hits_fileName ) {
    m_variable_photoSensor_hits_fileName = t_variable_photoSensor_hits_fileName;
}
void OutputMessenger::set_GDML_save( G4bool t_variable_GDML_save ) {
    m_variable_GDML_save = t_variable_GDML_save;
}
void OutputMessenger::set_GDML_fileName( G4String t_variable_GDML_fileName ) {
    m_variable_GDML_fileName = t_variable_GDML_fileName;
}