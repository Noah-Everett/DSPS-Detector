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

#include "UIMessenger.hh"

UIMessenger* UIMessenger::m_instance{ nullptr };

UIMessenger::UIMessenger() {
    m_parameter_showGUI = new G4UIcmdWithABool( "/USSD/showGUI", this );

    m_parameter_showGUI->SetDefaultValue( false );
}

UIMessenger::~UIMessenger() {
    if( m_parameter_showGUI ) delete m_parameter_showGUI;
}

UIMessenger* UIMessenger::get_instance() {
    if( !m_instance ) 
        m_instance = new UIMessenger();
    return m_instance;
}

void UIMessenger::delete_instance() {
    if( m_instance ) {
        delete m_instance;
        m_instance = nullptr;
    }
}

void UIMessenger::SetNewValue( G4UIcommand* t_command, G4String t_newValue ) {
    if( t_command == m_parameter_showGUI ) {
        set_showGUI( m_parameter_showGUI->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `showGUI' to " << t_newValue << G4endl;
    }
}

G4bool UIMessenger::get_showGUI(){ 
    return m_variable_showGUI; 
}

void UIMessenger::set_showGUI( G4bool t_variable_showGUI ){ 
    m_variable_showGUI = t_variable_showGUI; 
}