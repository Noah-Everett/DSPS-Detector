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

#ifndef UIMessenger_hh
#define UIMessenger_hh

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UImessenger.hh"
#include "G4SystemOfUnits.hh"

class UIMessenger : public G4UImessenger
{
    public:
        static UIMessenger* get_instance   ();
        static void         delete_instance();
    
        void SetNewValue( G4UIcommand* command, G4String newValue );

        G4bool get_showGUI(){ return m_variable_showGUI; }

        void set_showGUI( G4bool t_variable_showGUI ){ m_variable_showGUI = t_variable_showGUI; }

    private:
        static UIMessenger* m_instance;

    protected:
        UIMessenger();
       ~UIMessenger();

        G4UIcmdWithABool* m_parameter_showGUI{ nullptr };

        G4bool m_variable_showGUI{ false };
};

#endif