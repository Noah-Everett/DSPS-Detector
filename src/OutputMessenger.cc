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
    m_parameter_fileName = new G4UIcmdWithAString( "/output/fileName", this );
}

OutputMessenger::~OutputMessenger() {
    if( m_parameter_fileName ) delete m_parameter_fileName;
}

void OutputMessenger::SetNewValue( G4UIcommand* t_command, G4String t_newValue ) {
    if( t_command == m_parameter_fileName ) {
        set_fileName( t_newValue );
        G4cout << "Setting `fileName' to " << t_newValue << G4endl;
    }
}