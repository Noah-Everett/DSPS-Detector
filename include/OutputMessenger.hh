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

#ifndef OutputMessenger_h
#define OutputMessenger_h

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UImessenger.hh"
#include "G4SystemOfUnits.hh"

class OutputMessenger : public G4UImessenger
{
    public:
        static OutputMessenger* get_instance   ();
        static void             delete_instance();
    
        void SetNewValue( G4UIcommand* command, G4String newValue );

        G4bool   get_GDML_save                                    () const;
        G4String get_GDML_fileName                                () const;
        G4String get_fileName                                     () const;
        G4bool   get_photoSensor_hits_position_binned_save        () const;
        G4int    get_photoSensor_hits_position_binned_nBinsPerSide() const;
        G4bool   get_photoSensor_hits_position_save               () const;
        G4bool   get_photoSensor_hits_time_save                   () const;
        G4bool   get_photoSensor_hits_process_save                () const;
        G4bool   get_photoSensor_hits_photoSensorID_save          () const;
        G4bool   get_photoSensor_hits_energy_save                 () const;
        G4bool   get_primary_position_save                        () const;
        G4bool   get_primary_emission_photon_save                 () const;
        G4bool   get_primary_emission_electron_save               () const;
        G4bool   get_primary_process_save                         () const;
        G4bool   get_primary_time_save                            () const;
        G4bool   get_photon_length_save                           () const;
        G4bool   get_photon_process_save                          () const;
        G4bool   get_photon_time_save                             () const;
        G4bool   get_photon_position_save                         () const;
        G4bool   get_photon_direction_save                        () const;
        G4bool   get_photon_energy_save                           () const;
        G4bool   get_photon_volume_save                           () const;

        void set_GDML_save                                    ( G4bool   value );
        void set_GDML_fileName                                ( G4String value );
        void set_fileName                                     ( G4String value );
        void set_photoSensor_hits_position_binned_save        ( G4bool   value );
        void set_photoSensor_hits_position_binned_nBinsPerSide( G4int    value );
        void set_photoSensor_hits_position_save               ( G4bool   value );
        void set_photoSensor_hits_time_save                   ( G4bool   value );
        void set_photoSensor_hits_process_save                ( G4bool   value );
        void set_photoSensor_hits_photoSensorID_save          ( G4bool   value );
        void set_photoSensor_hits_energy_save                 ( G4bool   value );
        void set_primary_position_save                        ( G4bool   value );
        void set_primary_emission_photon_save                 ( G4bool   value );
        void set_primary_emission_electron_save               ( G4bool   value );
        void set_primary_process_save                         ( G4bool   value );
        void set_primary_time_save                            ( G4bool   value );
        void set_photon_length_save                           ( G4bool   value );
        void set_photon_process_save                          ( G4bool   value );
        void set_photon_time_save                             ( G4bool   value );
        void set_photon_position_save                         ( G4bool   value );
        void set_photon_direction_save                        ( G4bool   value );
        void set_photon_energy_save                           ( G4bool   value );
        void set_photon_volume_save                           ( G4bool   value );

    protected:
                 OutputMessenger();
        virtual ~OutputMessenger();
        
        G4UIcmdWithABool    * m_command_GDML_save;
        G4UIcmdWithAString  * m_command_GDML_fileName;
        G4UIcmdWithAString  * m_command_fileName;
        G4UIcmdWithABool    * m_command_photoSensor_hits_position_binned_save;
        G4UIcmdWithAnInteger* m_command_photoSensor_hits_position_binned_nBinsPerSide;
        G4UIcmdWithABool    * m_command_photoSensor_hits_position_save;
        G4UIcmdWithABool    * m_command_photoSensor_hits_time_save;
        G4UIcmdWithABool    * m_command_photoSensor_hits_process_save;
        G4UIcmdWithABool    * m_command_photoSensor_hits_photoSensorID_save;
        G4UIcmdWithABool    * m_command_photoSensor_hits_energy_save;
        G4UIcmdWithABool    * m_command_primary_position_save;
        G4UIcmdWithABool    * m_command_primary_emission_photon_save;
        G4UIcmdWithABool    * m_command_primary_emission_electron_save;
        G4UIcmdWithABool    * m_command_primary_process_save;
        G4UIcmdWithABool    * m_command_primary_time_save;
        G4UIcmdWithABool    * m_command_photon_length_save;
        G4UIcmdWithABool    * m_command_photon_process_save;
        G4UIcmdWithABool    * m_command_photon_time_save;
        G4UIcmdWithABool    * m_command_photon_position_save;
        G4UIcmdWithABool    * m_command_photon_direction_save;
        G4UIcmdWithABool    * m_command_photon_energy_save;
        G4UIcmdWithABool    * m_command_photon_volume_save;

        G4bool   m_variable_GDML_save;
        G4String m_variable_GDML_fileName;
        G4String m_variable_fileName;
        G4bool   m_variable_photoSensor_hits_position_binned_save;
        G4int    m_variable_photoSensor_hits_position_binned_nBinsPerSide;
        G4bool   m_variable_photoSensor_hits_position_save;
        G4bool   m_variable_photoSensor_hits_time_save;
        G4bool   m_variable_photoSensor_hits_process_save;
        G4bool   m_variable_photoSensor_hits_photoSensorID_save;
        G4bool   m_variable_photoSensor_hits_energy_save;
        G4bool   m_variable_primary_position_save;
        G4bool   m_variable_primary_emission_photon_save;
        G4bool   m_variable_primary_emission_electron_save;
        G4bool   m_variable_primary_process_save;
        G4bool   m_variable_primary_time_save;
        G4bool   m_variable_photon_length_save;
        G4bool   m_variable_photon_process_save;
        G4bool   m_variable_photon_time_save;
        G4bool   m_variable_photon_position_save;
        G4bool   m_variable_photon_direction_save;
        G4bool   m_variable_photon_energy_save;
        G4bool   m_variable_photon_volume_save;
    
    private:
        static OutputMessenger* m_instance;
};

#endif