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

#ifndef ParticleGunMessenger_hh
#define ParticleGunMessenger_hh

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UImessenger.hh"
#include "G4SystemOfUnits.hh"

class ParticleGunMessenger : public G4UImessenger
{
    public:
        static ParticleGunMessenger* get_instance   ();
        static void                  delete_instance();
    
        void SetNewValue( G4UIcommand* command, G4String newValue );

        G4bool   get_momentum_random      ();
        G4bool   get_position_x_random    ();
        G4bool   get_position_y_random    ();
        G4bool   get_position_z_random    ();
        G4double get_position_x_random_min();
        G4double get_position_y_random_min();
        G4double get_position_z_random_min();
        G4double get_position_x_random_max();
        G4double get_position_y_random_max();
        G4double get_position_z_random_max();

        void set_momentum_random      ( G4bool   );
        void set_position_x_random    ( G4bool   );
        void set_position_y_random    ( G4bool   );
        void set_position_z_random    ( G4bool   );
        void set_position_x_random_min( G4double );
        void set_position_y_random_min( G4double );
        void set_position_z_random_min( G4double );
        void set_position_x_random_max( G4double );
        void set_position_y_random_max( G4double );
        void set_position_z_random_max( G4double );

    private:
        static ParticleGunMessenger* m_instance;

    protected:
        ParticleGunMessenger();
       ~ParticleGunMessenger();

        G4UIcmdWithABool         * m_parameter_momentum_random      { nullptr };
        G4UIcmdWithABool         * m_parameter_position_x_random    { nullptr };
        G4UIcmdWithABool         * m_parameter_position_y_random    { nullptr };
        G4UIcmdWithABool         * m_parameter_position_z_random    { nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_position_x_random_min{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_position_y_random_min{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_position_z_random_min{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_position_x_random_max{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_position_y_random_max{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_position_z_random_max{ nullptr };

        G4bool   m_variable_momentum_random      { false };
        G4bool   m_variable_position_x_random    { false };
        G4bool   m_variable_position_y_random    { false };
        G4bool   m_variable_position_z_random    { false };
        G4double m_variable_position_x_random_min{ 0     };
        G4double m_variable_position_y_random_min{ 0     };
        G4double m_variable_position_z_random_min{ 0     };
        G4double m_variable_position_x_random_max{ 0     };
        G4double m_variable_position_y_random_max{ 0     };
        G4double m_variable_position_z_random_max{ 0     };
};

#endif