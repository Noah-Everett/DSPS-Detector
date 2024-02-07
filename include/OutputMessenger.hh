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

#ifndef OutputMessenger_hh
#define OutputMessenger_hh

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UImessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tokenizer.hh"
#include "G4String.hh"

#include "ConstructionMessenger.hh"

#include <vector>

using std::vector;
using std::any_of;
using std::stoi  ;
using G4StrUtil::to_lower_copy;

class OutputMessenger : public G4UImessenger
{
    public:
        static OutputMessenger* get_instance   ();
        static void             delete_instance();
    
        void SetNewValue( G4UIcommand* command, G4String newValue );

        G4bool          get_GDML_save                                         (       ) const;
        G4String        get_GDML_fileName                                     (       ) const;
        G4bool          get_photoSensor_hits_position_binned_save             (       ) const;
        G4int           get_photoSensor_hits_position_binned_nBinsPerSide     (       ) const;
        G4bool          get_photoSensor_hits_position_absolute_save           (       ) const;
        G4bool          get_photoSensor_hits_position_relative_save           (       ) const;
        G4bool          get_photoSensor_hits_position_relative_lens_save      ( G4int ) const;
        vector< G4int > get_photoSensor_hits_position_relative_lens_save      (       ) const;
        G4bool          get_photoSensor_hits_position_initial_save            (       ) const;
        G4bool          get_photoSensor_hits_momentum_save                    (       ) const;
        G4bool          get_photoSensor_hits_direction_save                   (       ) const;
        G4bool          get_photoSensor_hits_direction_relative_save          (       ) const;
        G4bool          get_photoSensor_hits_direction_relative_lens_save     ( G4int ) const;
        vector< G4int > get_photoSensor_hits_direction_relative_lens_save     (       ) const;
        G4bool          get_photoSensor_hits_time_save                        (       ) const;
        G4bool          get_photoSensor_hits_process_save                     (       ) const;
        G4bool          get_photoSensor_hits_photoSensorID_save               (       ) const;
        G4bool          get_photoSensor_hits_energy_save                      (       ) const;
        G4bool          get_calorimeter_hits_position_absolute_save           (       ) const;
        G4bool          get_calorimeter_hits_position_relative_save           (       ) const;
        G4bool          get_calorimeter_hits_position_initial_save            (       ) const;
        G4bool          get_calorimeter_hits_momentum_save                    (       ) const;
        G4bool          get_calorimeter_hits_direction_save                   (       ) const;
        G4bool          get_calorimeter_hits_direction_relative_save          (       ) const;
        G4bool          get_calorimeter_hits_time_save                        (       ) const;
        G4bool          get_calorimeter_hits_process_save                     (       ) const;
        G4bool          get_calorimeter_hits_calorimeterID_save               (       ) const;
        G4bool          get_calorimeter_hits_energy_save                      (       ) const;
        G4bool          get_lens_hits_position_absolute_save                  (       ) const;
        G4bool          get_lens_hits_position_relative_save                  (       ) const;
        G4bool          get_lens_hits_position_initial_save                   (       ) const;
        G4bool          get_lens_hits_momentum_save                           (       ) const;
        G4bool          get_lens_hits_direction_save                          (       ) const;
        G4bool          get_lens_hits_direction_relative_save                 (       ) const;
        G4bool          get_lens_hits_time_save                               (       ) const;
        G4bool          get_lens_hits_process_save                            (       ) const;
        G4bool          get_lens_hits_lensID_save                             (       ) const;
        G4bool          get_lens_hits_energy_save                             (       ) const;
        G4bool          get_lens_hits_transmittance_save                      (       ) const;
        G4bool          get_medium_hits_position_absolute_save                (       ) const;
        G4bool          get_medium_hits_position_initial_save                 (       ) const;
        G4bool          get_medium_hits_momentum_save                         (       ) const;
        G4bool          get_medium_hits_energy_save                           (       ) const;
        G4bool          get_medium_hits_process_save                          (       ) const;
        G4bool          get_medium_hits_time_save                             (       ) const;
        G4bool          get_medium_hits_mediumID_save                         (       ) const;
        G4bool          get_medium_hits_transmittance_save                    (       ) const;
        G4bool          get_primary_position_save                             (       ) const;
        G4bool          get_primary_momentum_save                             (       ) const;
        G4bool          get_primary_emission_photon_save                      (       ) const;
        G4bool          get_primary_emission_electron_save                    (       ) const;
        G4bool          get_primary_process_save                              (       ) const;
        G4bool          get_primary_time_save                                 (       ) const;
        G4bool          get_primary_energy_save                               (       ) const;
        G4bool          get_primary_volume_save                               (       ) const;
        G4bool          get_primary_pdg_save                                  (       ) const;
        G4bool          get_photon_length_save                                (       ) const;
        G4bool          get_photon_process_save                               (       ) const;
        G4bool          get_photon_time_save                                  (       ) const;
        G4bool          get_photon_position_save                              (       ) const;
        G4bool          get_photon_momentum_save                              (       ) const;
        G4bool          get_photon_energy_save                                (       ) const;
        G4bool          get_photon_volume_save                                (       ) const;
        G4bool          get_photon_stepNumber_save                            (       ) const;
        G4bool          get_photoSensor_hits_save                             (       ) const;
        G4bool          get_calorimeter_hits_save                             (       ) const;
        G4bool          get_lens_hits_save                                    (       ) const;
        G4bool          get_medium_hits_save                                  (       ) const;
        G4bool          get_primary_save                                      (       ) const;
        G4bool          get_photon_save                                       (       ) const;

        void set_GDML_save                                         ( G4bool   value );
        void set_GDML_fileName                                     ( G4String value );
        void set_photoSensor_hits_position_binned_save             ( G4bool   value );
        void set_photoSensor_hits_position_binned_nBinsPerSide     ( G4int    value );
        void set_photoSensor_hits_position_absolute_save           ( G4bool   value );
        void set_photoSensor_hits_position_relative_save           ( G4bool   value );
        void set_photoSensor_hits_position_relative_lens_save      ( G4String value );
        void set_photoSensor_hits_position_relative_lens_noSave    ( G4String value );
        void set_photoSensor_hits_position_initial_save            ( G4bool   value );
        void set_photoSensor_hits_momentum_save                    ( G4bool   value );
        void set_photoSensor_hits_direction_save                   ( G4bool   value );
        void set_photoSensor_hits_direction_relative_save          ( G4bool   value );
        void set_photoSensor_hits_direction_relative_lens_save     ( G4String value );
        void set_photoSensor_hits_direction_relative_lens_noSave   ( G4String value );
        void set_photoSensor_hits_time_save                        ( G4bool   value );
        void set_photoSensor_hits_process_save                     ( G4bool   value );
        void set_photoSensor_hits_photoSensorID_save               ( G4bool   value );
        void set_photoSensor_hits_energy_save                      ( G4bool   value );
        void set_calorimeter_hits_position_absolute_save           ( G4bool   value );
        void set_calorimeter_hits_position_relative_save           ( G4bool   value );
        void set_calorimeter_hits_position_initial_save            ( G4bool   value );
        void set_calorimeter_hits_momentum_save                    ( G4bool   value );
        void set_calorimeter_hits_direction_save                   ( G4bool   value );
        void set_calorimeter_hits_direction_relative_save          ( G4bool   value );
        void set_calorimeter_hits_time_save                        ( G4bool   value );
        void set_calorimeter_hits_process_save                     ( G4bool   value );
        void set_calorimeter_hits_calorimeterID_save               ( G4bool   value );
        void set_calorimeter_hits_energy_save                      ( G4bool   value );
        void set_lens_hits_position_absolute_save                  ( G4bool   value );
        void set_lens_hits_position_relative_save                  ( G4bool   value );
        void set_lens_hits_position_initial_save                   ( G4bool   value );
        void set_lens_hits_momentum_save                           ( G4bool   value );
        void set_lens_hits_direction_save                          ( G4bool   value );
        void set_lens_hits_direction_relative_save                 ( G4bool   value );
        void set_lens_hits_time_save                               ( G4bool   value );
        void set_lens_hits_process_save                            ( G4bool   value );
        void set_lens_hits_lensID_save                             ( G4bool   value );
        void set_lens_hits_energy_save                             ( G4bool   value );
        void set_lens_hits_transmittance_save                      ( G4bool   value );
        void set_medium_hits_position_absolute_save                ( G4bool   value );
        void set_medium_hits_position_initial_save                 ( G4bool   value );
        void set_medium_hits_momentum_save                         ( G4bool   value );
        void set_medium_hits_energy_save                           ( G4bool   value );
        void set_medium_hits_process_save                          ( G4bool   value );
        void set_medium_hits_time_save                             ( G4bool   value );
        void set_medium_hits_mediumID_save                         ( G4bool   value );
        void set_medium_hits_transmittance_save                    ( G4bool   value );
        void set_primary_position_save                             ( G4bool   value );
        void set_primary_momentum_save                             ( G4bool   value );
        void set_primary_emission_photon_save                      ( G4bool   value );
        void set_primary_emission_electron_save                    ( G4bool   value );
        void set_primary_process_save                              ( G4bool   value );
        void set_primary_time_save                                 ( G4bool   value );
        void set_primary_energy_save                               ( G4bool   value );
        void set_primary_volume_save                               ( G4bool   value );
        void set_primary_pdg_save                                  ( G4bool   value );
        void set_photon_length_save                                ( G4bool   value );
        void set_photon_process_save                               ( G4bool   value );
        void set_photon_time_save                                  ( G4bool   value );
        void set_photon_position_save                              ( G4bool   value );
        void set_photon_momentum_save                              ( G4bool   value );
        void set_photon_energy_save                                ( G4bool   value );
        void set_photon_volume_save                                ( G4bool   value );
        void set_photon_stepNumber_save                            ( G4bool   value );

    protected:
                 OutputMessenger();
        virtual ~OutputMessenger();
        
        G4UIcmdWithABool    * m_command_GDML_save                                      { nullptr };
        G4UIcmdWithAString  * m_command_GDML_fileName                                  { nullptr };
        G4UIcmdWithABool    * m_command_photoSensor_hits_position_binned_save          { nullptr };
        G4UIcmdWithAnInteger* m_command_photoSensor_hits_position_binned_nBinsPerSide  { nullptr };
        G4UIcmdWithABool    * m_command_photoSensor_hits_position_absolute_save        { nullptr };
        G4UIcmdWithABool    * m_command_photoSensor_hits_position_relative_save        { nullptr };
        G4UIcmdWithAString  * m_command_photoSensor_hits_position_relative_lens_save   { nullptr };
        G4UIcmdWithAString  * m_command_photoSensor_hits_position_relative_lens_noSave { nullptr };
        G4UIcmdWithABool    * m_command_photoSensor_hits_position_initial_save         { nullptr };
        G4UIcmdWithABool    * m_command_photoSensor_hits_momentum_save                 { nullptr };
        G4UIcmdWithABool    * m_command_photoSensor_hits_direction_save                { nullptr };
        G4UIcmdWithABool    * m_command_photoSensor_hits_direction_relative_save       { nullptr };
        G4UIcmdWithAString  * m_command_photoSensor_hits_direction_relative_lens_save  { nullptr };
        G4UIcmdWithAString  * m_command_photoSensor_hits_direction_relative_lens_noSave{ nullptr };
        G4UIcmdWithABool    * m_command_photoSensor_hits_time_save                     { nullptr };
        G4UIcmdWithABool    * m_command_photoSensor_hits_process_save                  { nullptr };
        G4UIcmdWithABool    * m_command_photoSensor_hits_photoSensorID_save            { nullptr };
        G4UIcmdWithABool    * m_command_photoSensor_hits_energy_save                   { nullptr };
        G4UIcmdWithABool    * m_command_calorimeter_hits_position_absolute_save        { nullptr };
        G4UIcmdWithABool    * m_command_calorimeter_hits_position_relative_save        { nullptr };
        G4UIcmdWithABool    * m_command_calorimeter_hits_position_initial_save         { nullptr };
        G4UIcmdWithABool    * m_command_calorimeter_hits_momentum_save                 { nullptr };
        G4UIcmdWithABool    * m_command_calorimeter_hits_direction_save                { nullptr };
        G4UIcmdWithABool    * m_command_calorimeter_hits_direction_relative_save       { nullptr };
        G4UIcmdWithABool    * m_command_calorimeter_hits_time_save                     { nullptr };
        G4UIcmdWithABool    * m_command_calorimeter_hits_process_save                  { nullptr };
        G4UIcmdWithABool    * m_command_calorimeter_hits_calorimeterID_save            { nullptr };
        G4UIcmdWithABool    * m_command_calorimeter_hits_energy_save                   { nullptr };
        G4UIcmdWithABool    * m_command_lens_hits_position_absolute_save               { nullptr };
        G4UIcmdWithABool    * m_command_lens_hits_position_relative_save               { nullptr };
        G4UIcmdWithABool    * m_command_lens_hits_position_initial_save                { nullptr };
        G4UIcmdWithABool    * m_command_lens_hits_momentum_save                        { nullptr };
        G4UIcmdWithABool    * m_command_lens_hits_direction_save                       { nullptr };
        G4UIcmdWithABool    * m_command_lens_hits_direction_relative_save              { nullptr };
        G4UIcmdWithABool    * m_command_lens_hits_time_save                            { nullptr };
        G4UIcmdWithABool    * m_command_lens_hits_process_save                         { nullptr };
        G4UIcmdWithABool    * m_command_lens_hits_lensID_save                          { nullptr };
        G4UIcmdWithABool    * m_command_lens_hits_energy_save                          { nullptr };
        G4UIcmdWithABool    * m_command_lens_hits_transmittance_save                   { nullptr };
        G4UIcmdWithABool    * m_command_medium_hits_position_absolute_save             { nullptr };
        G4UIcmdWithABool    * m_command_medium_hits_position_initial_save              { nullptr };
        G4UIcmdWithABool    * m_command_medium_hits_momentum_save                      { nullptr };
        G4UIcmdWithABool    * m_command_medium_hits_energy_save                        { nullptr };
        G4UIcmdWithABool    * m_command_medium_hits_process_save                       { nullptr };
        G4UIcmdWithABool    * m_command_medium_hits_time_save                          { nullptr };
        G4UIcmdWithABool    * m_command_medium_hits_mediumID_save                      { nullptr };
        G4UIcmdWithABool    * m_command_medium_hits_transmittance_save                 { nullptr };
        G4UIcmdWithABool    * m_command_primary_position_save                          { nullptr };
        G4UIcmdWithABool    * m_command_primary_momentum_save                          { nullptr };
        G4UIcmdWithABool    * m_command_primary_emission_photon_save                   { nullptr };
        G4UIcmdWithABool    * m_command_primary_emission_electron_save                 { nullptr };
        G4UIcmdWithABool    * m_command_primary_process_save                           { nullptr };
        G4UIcmdWithABool    * m_command_primary_time_save                              { nullptr };
        G4UIcmdWithABool    * m_command_primary_energy_save                            { nullptr };
        G4UIcmdWithABool    * m_command_primary_volume_save                            { nullptr };
        G4UIcmdWithABool    * m_command_primary_pdg_save                               { nullptr };
        G4UIcmdWithABool    * m_command_photon_length_save                             { nullptr };
        G4UIcmdWithABool    * m_command_photon_process_save                            { nullptr };
        G4UIcmdWithABool    * m_command_photon_time_save                               { nullptr };
        G4UIcmdWithABool    * m_command_photon_position_save                           { nullptr };
        G4UIcmdWithABool    * m_command_photon_momentum_save                           { nullptr };
        G4UIcmdWithABool    * m_command_photon_energy_save                             { nullptr };
        G4UIcmdWithABool    * m_command_photon_volume_save                             { nullptr };
        G4UIcmdWithABool    * m_command_photon_stepNumber_save                         { nullptr };

        G4bool           m_variable_GDML_save                                    { false         };
        G4String         m_variable_GDML_fileName                                { "output.gdml" };
        G4bool           m_variable_photoSensor_hits_position_binned_save        { false         };
        G4int            m_variable_photoSensor_hits_position_binned_nBinsPerSide{ 1             };
        G4bool           m_variable_photoSensor_hits_position_absolute_save      { false         };
        G4bool           m_variable_photoSensor_hits_position_relative_save      { false         };
        vector< G4bool > m_variable_photoSensor_hits_position_relative_lens_save { {}            };
        G4bool           m_variable_photoSensor_hits_position_initial_save       { false         };
        G4bool           m_variable_photoSensor_hits_momentum_save               { false         };
        G4bool           m_variable_photoSensor_hits_direction_save              { false         };
        G4bool           m_variable_photoSensor_hits_direction_relative_save     { false         };
        vector< G4bool > m_variable_photoSensor_hits_direction_relative_lens_save{ {}            };
        G4bool           m_variable_photoSensor_hits_time_save                   { false         };
        G4bool           m_variable_photoSensor_hits_process_save                { false         };
        G4bool           m_variable_photoSensor_hits_photoSensorID_save          { false         };
        G4bool           m_variable_photoSensor_hits_energy_save                 { false         };
        G4bool           m_variable_calorimeter_hits_position_absolute_save      { false         };
        G4bool           m_variable_calorimeter_hits_position_relative_save      { false         };
        G4bool           m_variable_calorimeter_hits_position_initial_save       { false         };
        G4bool           m_variable_calorimeter_hits_momentum_save               { false         };
        G4bool           m_variable_calorimeter_hits_direction_save              { false         };
        G4bool           m_variable_calorimeter_hits_direction_relative_save     { false         };
        G4bool           m_variable_calorimeter_hits_time_save                   { false         };
        G4bool           m_variable_calorimeter_hits_process_save                { false         };
        G4bool           m_variable_calorimeter_hits_calorimeterID_save          { false         };
        G4bool           m_variable_calorimeter_hits_energy_save                 { false         };
        G4bool           m_variable_lens_hits_position_absolute_save             { false         };
        G4bool           m_variable_lens_hits_position_relative_save             { false         };
        G4bool           m_variable_lens_hits_position_initial_save              { false         };
        G4bool           m_variable_lens_hits_momentum_save                      { false         };
        G4bool           m_variable_lens_hits_direction_save                     { false         };
        G4bool           m_variable_lens_hits_direction_relative_save            { false         };
        G4bool           m_variable_lens_hits_time_save                          { false         };
        G4bool           m_variable_lens_hits_process_save                       { false         };
        G4bool           m_variable_lens_hits_lensID_save                        { false         };
        G4bool           m_variable_lens_hits_energy_save                        { false         };
        G4bool           m_variable_lens_hits_transmittance_save                 { false         };
        G4bool           m_variable_medium_hits_position_absolute_save           { false         };
        G4bool           m_variable_medium_hits_position_initial_save            { false         };
        G4bool           m_variable_medium_hits_momentum_save                    { false         };
        G4bool           m_variable_medium_hits_energy_save                      { false         };
        G4bool           m_variable_medium_hits_process_save                     { false         };
        G4bool           m_variable_medium_hits_time_save                        { false         };
        G4bool           m_variable_medium_hits_mediumID_save                    { false         };
        G4bool           m_variable_medium_hits_transmittance_save               { false         };
        G4bool           m_variable_primary_position_save                        { false         };
        G4bool           m_variable_primary_momentum_save                        { false         };
        G4bool           m_variable_primary_emission_photon_save                 { false         };
        G4bool           m_variable_primary_emission_electron_save               { false         };
        G4bool           m_variable_primary_process_save                         { false         };
        G4bool           m_variable_primary_time_save                            { false         };
        G4bool           m_variable_primary_energy_save                          { false         };
        G4bool           m_variable_primary_volume_save                          { false         };
        G4bool           m_variable_primary_pdg_save                             { false         };
        G4bool           m_variable_photon_length_save                           { false         };
        G4bool           m_variable_photon_process_save                          { false         };
        G4bool           m_variable_photon_time_save                             { false         };
        G4bool           m_variable_photon_position_save                         { false         };
        G4bool           m_variable_photon_momentum_save                         { false         };
        G4bool           m_variable_photon_energy_save                           { false         };
        G4bool           m_variable_photon_volume_save                           { false         };
        G4bool           m_variable_photon_stepNumber_save                       { false         };
        
        void            initialize_nLenses( G4int, vector< G4bool >& );
        vector< G4int > parse_nLenses     ( G4String                 );

        G4bool any( const vector< G4bool >& ) const;

        ConstructionMessenger* m_constructionMessenger = ConstructionMessenger::get_instance();    

    private:
        static OutputMessenger* m_instance;
};

#endif