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
#include "G4UIcmdWithAnInteger.hh"
#include "G4UImessenger.hh"
#include "G4SystemOfUnits.hh"

class ParticleGun;
class PrimaryGeneratorAction;

using std::vector;

class ParticleGunMessenger : public G4UImessenger
{
    public:
        static ParticleGunMessenger* get_instance   ();
        static void                  delete_instance();
    
        void SetNewValue( G4UIcommand* command, G4String newValue );

        G4bool   get_momentum_random      ();
        G4double get_momentum_x_random_min();
        G4double get_momentum_y_random_min();
        G4double get_momentum_z_random_min();
        G4double get_momentum_x_random_max();
        G4double get_momentum_y_random_max();
        G4double get_momentum_z_random_max();
        G4bool   get_position_x_random    ();
        G4bool   get_position_y_random    ();
        G4bool   get_position_z_random    ();
        G4int    get_position_x_nSteps    ();
        G4int    get_position_y_nSteps    ();
        G4int    get_position_z_nSteps    ();
        G4double get_position_x_random_min();
        G4double get_position_y_random_min();
        G4double get_position_z_random_min();
        G4double get_position_x_random_max();
        G4double get_position_y_random_max();
        G4double get_position_z_random_max();
        G4int    get_nParticles           ();

        void set_momentum_random      ( G4bool   );
        void set_momentum_x_random_min( G4double );
        void set_momentum_y_random_min( G4double );
        void set_momentum_z_random_min( G4double );
        void set_momentum_x_random_max( G4double );
        void set_momentum_y_random_max( G4double );
        void set_momentum_z_random_max( G4double );
        void set_position_x_random    ( G4bool   );
        void set_position_y_random    ( G4bool   );
        void set_position_z_random    ( G4bool   );
        void set_position_x_nSteps    ( G4int    );
        void set_position_y_nSteps    ( G4int    );
        void set_position_z_nSteps    ( G4int    );
        void set_position_x_random_min( G4double );
        void set_position_y_random_min( G4double );
        void set_position_z_random_min( G4double );
        void set_position_x_random_max( G4double );
        void set_position_y_random_max( G4double );
        void set_position_z_random_max( G4double );
        void set_nParticles           ( G4int    );

        void add_primaryGeneratorAction( PrimaryGeneratorAction* );
        void add_particleGun           ( ParticleGun           * );

    private:
        static ParticleGunMessenger* m_instance;

    protected:
        ParticleGunMessenger();
       ~ParticleGunMessenger();

        G4UIcmdWithABool         * m_parameter_momentum_random      { nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_momentum_x_random_min{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_momentum_y_random_min{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_momentum_z_random_min{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_momentum_x_random_max{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_momentum_y_random_max{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_momentum_z_random_max{ nullptr };
        G4UIcmdWithABool         * m_parameter_position_x_random    { nullptr };
        G4UIcmdWithABool         * m_parameter_position_y_random    { nullptr };
        G4UIcmdWithABool         * m_parameter_position_z_random    { nullptr };
        G4UIcmdWithAnInteger     * m_parameter_position_x_nSteps    { nullptr };
        G4UIcmdWithAnInteger     * m_parameter_position_y_nSteps    { nullptr };
        G4UIcmdWithAnInteger     * m_parameter_position_z_nSteps    { nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_position_x_random_min{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_position_y_random_min{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_position_z_random_min{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_position_x_random_max{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_position_y_random_max{ nullptr };
        G4UIcmdWithADoubleAndUnit* m_parameter_position_z_random_max{ nullptr };
        G4UIcmdWithAnInteger     * m_parameter_nParticles           { nullptr };

        G4bool   m_variable_momentum_random      { false };
        G4double m_variable_momentum_x_random_min{ -10   };
        G4double m_variable_momentum_y_random_min{ -10   };
        G4double m_variable_momentum_z_random_min{ -10   };
        G4double m_variable_momentum_x_random_max{  10   };
        G4double m_variable_momentum_y_random_max{  10   };
        G4double m_variable_momentum_z_random_max{  10   };
        G4bool   m_variable_position_x_random    { false };
        G4bool   m_variable_position_y_random    { false };
        G4bool   m_variable_position_z_random    { false };
        G4int    m_variable_position_x_nSteps    { -1    };
        G4int    m_variable_position_y_nSteps    { -1    };
        G4int    m_variable_position_z_nSteps    { -1    };
        G4double m_variable_position_x_random_min{ 0     };
        G4double m_variable_position_y_random_min{ 0     };
        G4double m_variable_position_z_random_min{ 0     };
        G4double m_variable_position_x_random_max{ 0     };
        G4double m_variable_position_y_random_max{ 0     };
        G4double m_variable_position_z_random_max{ 0     };
        G4int    m_variable_nParticles           { 1     };

        vector< PrimaryGeneratorAction* > m_primaryGeneratorActions;
        vector< ParticleGun           * > m_particleGuns           ;
};

#endif