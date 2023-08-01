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

#ifndef  ConstructionMessenger_h
#define  ConstructionMessenger_h

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UImessenger.hh"
#include "G4SystemOfUnits.hh"

class  ConstructionMessenger : public G4UImessenger
{
    public:
         ConstructionMessenger();
       ~ ConstructionMessenger();
    
        void SetNewValue( G4UIcommand* command, G4String newValue );

        G4String      get_world_medium_material          () { return m_variable_world_medium_material       ; }
        G4String      get_detector_wall_material         () { return m_variable_detector_wall_material      ; }
        G4String      get_detector_medium_material       () { return m_variable_detector_medium_material    ; }
        G4String      get_calorimeter_material           () { return m_variable_calorimeter_material        ; }
        G4String      get_lens_material                  () { return m_variable_lens_material               ; }
        G4String      get_photoSensor_surface_material   () { return m_variable_photoSensor_surface_material; }
        G4ThreeVector get_world_size                     () { return m_variable_world_size                  ; }
        G4double      get_world_size_x                   () { return m_variable_world_size.x()              ; }
        G4double      get_world_size_y                   () { return m_variable_world_size.y()              ; }
        G4double      get_world_size_z                   () { return m_variable_world_size.z()              ; }
        G4ThreeVector get_detector_wall_size             () { return m_variable_detector_wall_size          ; }
        G4double      get_detector_wall_size_x           () { return m_variable_detector_wall_size.x()      ; }
        G4double      get_detector_wall_size_y           () { return m_variable_detector_wall_size.y()      ; }
        G4double      get_detector_wall_size_z           () { return m_variable_detector_wall_size.z()      ; }
        G4double      get_detector_wall_thickness        () { return m_variable_detector_wall_thickness     ; }
        G4ThreeVector get_calorimeter_size               () { return m_variable_calorimeter_size            ; }
        G4double      get_calorimeter_size_widthAndHeight() { return m_variable_calorimeter_size.x()         
                                                                  == m_variable_calorimeter_size.y()         
                                                                   ? m_variable_calorimeter_size.x() : 1/0  ; }
        G4double      get_calorimeter_size_depth         () { return m_variable_calorimeter_size.y()        ; }
        G4String      get_lens_parameterFilePath         () { return m_variable_lens_parameterFilePath      ; }

        void set_world_medium_material       ( G4String      t_variable_world_medium_material        ){ m_variable_world_medium_material        = t_variable_world_medium_material       ; }
        void set_detector_wall_material      ( G4String      t_variable_detector_wall_material       ){ m_variable_detector_wall_material       = t_variable_detector_wall_material      ; }
        void set_detector_medium_material    ( G4String      t_variable_detector_medium_material     ){ m_variable_detector_medium_material     = t_variable_detector_medium_material    ; }
        void set_calorimeter_material        ( G4String      t_variable_calorimeter_material         ){ m_variable_calorimeter_material         = t_variable_calorimeter_material        ; }
        void set_lens_material               ( G4String      t_variable_lens_material                ){ m_variable_lens_material                = t_variable_lens_material               ; }
        void set_photoSensor_surface_material( G4String      t_variable_photoSensor_surface_material ){ m_variable_photoSensor_surface_material = t_variable_photoSensor_surface_material; }
        void set_world_size                  ( G4ThreeVector t_variable_world_size                   ){ m_variable_world_size                   = t_variable_world_size                  ; }
        void set_detector_wall_size          ( G4ThreeVector t_variable_detector_wall_size           ){ m_variable_detector_wall_size           = t_variable_detector_wall_size          ; }
        void set_detector_wall_thickness     ( G4double      t_variable_detector_wall_thickness      ){ m_variable_detector_wall_thickness      = t_variable_detector_wall_thickness     ; }
        void set_calorimeter_size            ( G4ThreeVector t_variable_calorimeter_size             ){ m_variable_calorimeter_size             = t_variable_calorimeter_size            ; }
        void set_lens_parameterFilePath      ( G4String      t_variable_lens_parameterFilePath       ){ m_variable_lens_parameterFilePath       = t_variable_lens_parameterFilePath      ; }

    protected:
        G4UIcmdWithAString       * m_command_world_medium_material       { nullptr }; G4String      m_variable_world_medium_material       { "G4Water" };
        G4UIcmdWithAString       * m_command_detector_wall_material      { nullptr }; G4String      m_variable_detector_wall_material      { "G4Water" };
        G4UIcmdWithAString       * m_command_detector_medium_material    { nullptr }; G4String      m_variable_detector_medium_material    { "LXe"     };
        G4UIcmdWithAString       * m_command_calorimeter_material        { nullptr }; G4String      m_variable_calorimeter_material        { "G4Water" };
        G4UIcmdWithAString       * m_command_lens_material               { nullptr }; G4String      m_variable_lens_material               { "G4Water" };
        G4UIcmdWithAString       * m_command_photoSensor_surface_material{ nullptr }; G4String      m_variable_photoSensor_surface_material{ "G4Water" };
        G4UIcmdWith3VectorAndUnit* m_command_world_size                  { nullptr }; G4ThreeVector m_variable_world_size                  { 100.0 * cm, 100.0 * cm, 100.0 * cm };
        G4UIcmdWith3VectorAndUnit* m_command_detector_wall_size          { nullptr }; G4ThreeVector m_variable_detector_wall_size          { 100.0 * cm, 100.0 * cm, 100.0 * cm };
        G4UIcmdWithADoubleAndUnit* m_command_detector_wall_thickness     { nullptr }; G4double      m_variable_detector_wall_thickness     { 1.0   * cm };
        G4UIcmdWith3VectorAndUnit* m_command_detector_calorimeter_size   { nullptr }; G4ThreeVector m_variable_calorimeter_size            { 100.0 * cm, 100.0 * cm, 100.0 * cm };
        G4UIcmdWithAString       * m_command_lens_parameterFilePath      { nullptr }; G4String      m_variable_lens_parameterFilePath      { "lens.dat" };




























};

#endif