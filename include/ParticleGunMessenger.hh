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

        G4bool get_momentum_random(){ return m_parameter_momentum_random; }

        void set_momentum_random( G4bool t_variable_momentum_random ){ m_variable_momentum_random = t_variable_momentum_random; }

    private:
        static ParticleGunMessenger* m_instance;

    protected:
        ParticleGunMessenger();
       ~ParticleGunMessenger();

        G4UIcmdWithABool* m_parameter_momentum_random{ nullptr };

        G4bool m_variable_momentum_random{ false };
};

#endif