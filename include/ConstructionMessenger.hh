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
#include "G4UIcmdWith3Vector.hh"
#include "G4UImessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4Exception.hh"

class  ConstructionMessenger : public G4UImessenger
{
    public:
         ConstructionMessenger();
       ~ ConstructionMessenger();
    
        void SetNewValue( G4UIcommand* command, G4String newValue );

        G4ThreeVector get_world_size                              ();
        G4double      get_world_size_x                            ();
        G4double      get_world_size_y                            ();
        G4double      get_world_size_z                            ();
        G4String      get_world_material                          ();
        G4double      get_detector_wall_thickness                 ();
        G4String      get_detector_wall_material                  ();
        G4String      get_detector_medium_material                ();
        G4ThreeVector get_calorimeter_size                        ();
        G4double      get_calorimeter_size_width                  ();
        G4double      get_calorimeter_size_height                 ();
        G4double      get_calorimeter_size_depth                  ();
        G4String      get_calorimeter_material                    ();
        G4ThreeVector get_photoSensor_surface_size                ();
        G4double      get_photoSensor_surface_size_width          ();
        G4double      get_photoSensor_surface_size_height         ();
        G4double      get_photoSensor_surface_size_depth          ();
        G4double      get_photoSensor_surface_size_widthAndHeight ();
        G4String      get_photoSensor_surface_material            ();
        G4ThreeVector get_photoSensor_body_size                   ();
        G4double      get_photoSensor_body_size_width             ();
        G4double      get_photoSensor_body_size_height            ();
        G4double      get_photoSensor_body_size_depth             ();
        G4double      get_photoSensor_body_size_widthAndHeight    ();
        G4String      get_photoSensor_body_material               ();
        G4String      get_lens_parameterFilePath                  ();
        G4ThreeVector get_directionSensitivePhotoDetector_amount  ();
        G4int         get_directionSensitivePhotoDetector_amount_x();
        G4int         get_directionSensitivePhotoDetector_amount_y();
        G4int         get_directionSensitivePhotoDetector_amount_z();
        G4bool        get_checkOverlaps                           ();

        void set_world_size                              ( G4ThreeVector );
        void set_world_size_x                            ( G4double      );
        void set_world_size_y                            ( G4double      );
        void set_world_size_z                            ( G4double      );
        void set_world_material                          ( G4String      );
        void set_detector_wall_thickness                 ( G4double      );
        void set_detector_wall_material                  ( G4String      );
        void set_detector_medium_material                ( G4String      );
        void set_calorimeter_size                        ( G4ThreeVector );
        void set_calorimeter_size_width                  ( G4double      );
        void set_calorimeter_size_height                 ( G4double      );
        void set_calorimeter_size_depth                  ( G4double      );
        void set_calorimeter_material                    ( G4String      );
        void set_photoSensor_surface_size                ( G4ThreeVector );
        void set_photoSensor_surface_size_width          ( G4double      );
        void set_photoSensor_surface_size_height         ( G4double      );
        void set_photoSensor_surface_size_depth          ( G4double      );
        void set_photoSensor_surface_size_widthAndHeight ( G4double      );
        void set_photoSensor_surface_material            ( G4String      );
        void set_photoSensor_body_size                   ( G4ThreeVector );
        void set_photoSensor_body_size_width             ( G4double      );
        void set_photoSensor_body_size_height            ( G4double      );
        void set_photoSensor_body_size_depth             ( G4double      );
        void set_photoSensor_body_size_widthAndHeight    ( G4double      );
        void set_photoSensor_body_material               ( G4String      );
        void set_lens_parameterFilePath                  ( G4String      );
        void set_directionSensitivePhotoDetector_amount  ( G4ThreeVector );
        void set_directionSensitivePhotoDetector_amount_x( G4double      );
        void set_directionSensitivePhotoDetector_amount_y( G4double      );
        void set_directionSensitivePhotoDetector_amount_z( G4double      );
        void set_checkOverlaps                           ( G4bool        );

        void print_parameters();

    protected:
        G4UIcmdWith3VectorAndUnit * m_command_world_size                            { nullptr }; G4ThreeVector m_variable_world_size                            { 1000.0 * mm, 1000.0 * mm, 1000.0 * mm };
        G4UIcmdWithAString        * m_command_world_material                        { nullptr }; G4String      m_variable_world_material                        { "G4Air" };
        G4UIcmdWithADoubleAndUnit * m_command_detector_wall_thickness               { nullptr }; G4double      m_variable_detector_wall_thickness               { 1.0 * mm };
        G4UIcmdWithAString        * m_command_detector_wall_material                { nullptr }; G4String      m_variable_detector_wall_material                { "G4Air" };
        G4UIcmdWithAString        * m_command_detector_medium_material              { nullptr }; G4String      m_variable_detector_medium_material              { "G4Air" };
        G4UIcmdWith3VectorAndUnit * m_command_calorimeter_size                      { nullptr }; G4ThreeVector m_variable_calorimeter_size                      { 100.0 * mm, 100.0 * mm, 100.0 * mm };
        G4UIcmdWithAString        * m_command_calorimeter_material                  { nullptr }; G4String      m_variable_calorimeter_material                  { "G4Air" };
        G4UIcmdWith3VectorAndUnit * m_command_photoSensor_surface_size              { nullptr }; G4ThreeVector m_variable_photoSensor_surface_size              { 100.0 * mm, 100.0 * mm, 1.0 * mm };
        G4UIcmdWithAString        * m_command_photoSensor_surface_material          { nullptr }; G4String      m_variable_photoSensor_surface_material          { "G4Air" };
        G4UIcmdWith3VectorAndUnit * m_command_photoSensor_body_size                 { nullptr }; G4ThreeVector m_variable_photoSensor_body_size                 { 100.0 * mm, 100.0 * mm, 1.0 * mm };
        G4UIcmdWithAString        * m_command_photoSensor_body_material             { nullptr }; G4String      m_variable_photoSensor_body_material             { "G4Air" };
        G4UIcmdWithAString        * m_command_lens_parameterFilePath                { nullptr }; G4String      m_variable_lens_parameterFilePath                { "" };
        G4UIcmdWith3Vector        * m_command_directionSensitivePhotoDetector_amount{ nullptr }; G4ThreeVector m_variable_directionSensitivePhotoDetector_amount{ 1, 1, 1 };
        G4UIcmdWithABool          * m_command_checkOverlaps                         { nullptr }; G4bool        m_variable_checkOverlaps                         { true };
};

#endif