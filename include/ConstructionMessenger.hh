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
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UImessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4Exception.hh"
#include "G4VisAttributes.hh"

#include <vector> 

using std::vector;

class  ConstructionMessenger : public G4UImessenger
{
    public:
        static ConstructionMessenger* get_instance   ();
        static void                   delete_instance();
    
        void SetNewValue( G4UIcommand* command, G4String newValue );

        G4ThreeVector    get_world_size                              ();
        G4double         get_world_size_x                            ();
        G4double         get_world_size_y                            ();
        G4double         get_world_size_z                            ();
        G4String         get_world_material                          ();
        G4VisAttributes* get_world_visAttributes                     ();
        G4bool           get_world_visibility                        ();
        G4String         get_world_color                             ();
        G4double         get_world_alpha                             ();
        G4bool           get_world_forceSolid                        ();
        
        G4double         get_detector_wall_thickness                 ();
        G4String         get_detector_wall_material                  ();
        G4VisAttributes* get_detector_wall_visAttributes             ();
        G4bool           get_detector_wall_visibility                ();
        G4String         get_detector_wall_color                     ();
        G4double         get_detector_wall_alpha                     ();
        G4bool           get_detector_wall_forceSolid                ();

        G4String         get_detector_medium_material                ();
        G4VisAttributes* get_detector_medium_visAttributes           ();
        G4bool           get_detector_medium_visibility              ();
        G4String         get_detector_medium_color                   ();
        G4double         get_detector_medium_alpha                   ();
        G4bool           get_detector_medium_forceSolid              ();

        G4ThreeVector    get_calorimeter_size                        ();
        G4double         get_calorimeter_size_width                  ();
        G4double         get_calorimeter_size_height                 ();
        G4double         get_calorimeter_size_depth                  ();
        G4String         get_calorimeter_material                    ();
        G4VisAttributes* get_calorimeter_visAttributes               ();
        G4bool           get_calorimeter_visibility                  ();
        G4String         get_calorimeter_color                       ();
        G4double         get_calorimeter_alpha                       ();
        G4bool           get_calorimeter_forceSolid                  ();

        G4ThreeVector    get_photoSensor_surface_size                ();
        G4double         get_photoSensor_surface_size_width          ();
        G4double         get_photoSensor_surface_size_height         ();
        G4double         get_photoSensor_surface_size_depth          ();
        G4double         get_photoSensor_surface_size_widthAndHeight ();
        G4String         get_photoSensor_surface_material            ();
        G4VisAttributes* get_photoSensor_surface_visAttributes       ();
        G4bool           get_photoSensor_surface_visibility          ();
        G4String         get_photoSensor_surface_color               ();
        G4double         get_photoSensor_surface_alpha               ();
        G4bool           get_photoSensor_surface_forceSolid          ();

        G4ThreeVector    get_photoSensor_body_size                   ();
        G4double         get_photoSensor_body_size_width             ();
        G4double         get_photoSensor_body_size_height            ();
        G4double         get_photoSensor_body_size_depth             ();
        G4double         get_photoSensor_body_size_widthAndHeight    ();
        G4String         get_photoSensor_body_material               ();
        G4VisAttributes* get_photoSensor_body_visAttributes          ();
        G4bool           get_photoSensor_body_visibility             ();
        G4String         get_photoSensor_body_color                  ();
        G4double         get_photoSensor_body_alpha                  ();
        G4bool           get_photoSensor_body_forceSolid             ();

        G4int            get_lens_amount                             ();
        G4double         get_lens_surface_1_radius_x                 ( G4int );
        G4double         get_lens_surface_1_radius_y                 ( G4int );
        G4double         get_lens_surface_1_yLimits                  ( G4int );
        G4double         get_lens_surface_2_radius_x                 ( G4int );
        G4double         get_lens_surface_2_radius_y                 ( G4int );
        G4double         get_lens_surface_2_yLimits                  ( G4int );
        G4double         get_lens_distance                           ( G4int );
        G4double         get_lens_position                           ( G4int );
        G4String         get_lens_material                           ( G4int );
        G4VisAttributes* get_lens_visAttributes                      ( G4int );
        G4bool           get_lens_visibility                         ( G4int );
        G4String         get_lens_color                              ( G4int );
        G4double         get_lens_alpha                              ( G4int );
        G4bool           get_lens_forceSolid                         ( G4int );
        G4bool           get_lens_circular                           ( G4int );

        G4ThreeVector    get_directionSensitivePhotoDetector_amount      ();
        G4int            get_directionSensitivePhotoDetector_amount_x    ();
        G4int            get_directionSensitivePhotoDetector_amount_y    ();
        G4int            get_directionSensitivePhotoDetector_amount_z    ();
        G4int            get_directionSensitivePhotoDetector_amount_total();

        G4bool           get_checkOverlaps                           ();

        void set_world_size                              ( G4ThreeVector );
        void set_world_size_x                            ( G4double      );
        void set_world_size_y                            ( G4double      );
        void set_world_size_z                            ( G4double      );
        void set_world_material                          ( G4String      );
        void set_world_visibility                        ( G4bool        );
        void set_world_color                             ( G4String      );
        void set_world_alpha                             ( G4double      );
        void set_world_forceSolid                        ( G4bool        );

        void set_detector_wall_thickness                 ( G4double      );
        void set_detector_wall_material                  ( G4String      );
        void set_detector_wall_visibility                ( G4bool        );
        void set_detector_wall_color                     ( G4String      );
        void set_detector_wall_alpha                     ( G4double      );
        void set_detector_wall_forceSolid                ( G4bool        );

        void set_detector_medium_material                ( G4String      );
        void set_detector_medium_visibility              ( G4bool        );
        void set_detector_medium_color                   ( G4String      );
        void set_detector_medium_alpha                   ( G4double      );
        void set_detector_medium_forceSolid              ( G4bool        );

        void set_calorimeter_size                        ( G4ThreeVector );
        void set_calorimeter_size_width                  ( G4double      );
        void set_calorimeter_size_height                 ( G4double      );
        void set_calorimeter_size_depth                  ( G4double      );
        void set_calorimeter_material                    ( G4String      );
        void set_calorimeter_visibility                  ( G4bool        );
        void set_calorimeter_color                       ( G4String      );
        void set_calorimeter_alpha                       ( G4double      );
        void set_calorimeter_forceSolid                  ( G4bool        );

        void set_photoSensor_surface_size                ( G4ThreeVector );
        void set_photoSensor_surface_size_width          ( G4double      );
        void set_photoSensor_surface_size_height         ( G4double      );
        void set_photoSensor_surface_size_depth          ( G4double      );
        void set_photoSensor_surface_size_widthAndHeight ( G4double      );
        void set_photoSensor_surface_material            ( G4String      );
        void set_photoSensor_surface_visibility          ( G4bool        );
        void set_photoSensor_surface_color               ( G4String      );
        void set_photoSensor_surface_alpha               ( G4double      );
        void set_photoSensor_surface_forceSolid          ( G4bool        );

        void set_photoSensor_body_size                   ( G4ThreeVector );
        void set_photoSensor_body_size_width             ( G4double      );
        void set_photoSensor_body_size_height            ( G4double      );
        void set_photoSensor_body_size_depth             ( G4double      );
        void set_photoSensor_body_size_widthAndHeight    ( G4double      );
        void set_photoSensor_body_material               ( G4String      );
        void set_photoSensor_body_visibility             ( G4bool        );
        void set_photoSensor_body_color                  ( G4String      );
        void set_photoSensor_body_alpha                  ( G4double      );
        void set_photoSensor_body_forceSolid             ( G4bool        );

        void set_lens_incramentCurrentLens               ();
        void set_lens_surface_1_radius_x                 ( G4int, G4double );
        void set_lens_surface_1_radius_y                 ( G4int, G4double );
        void set_lens_surface_1_yLimits                  ( G4int, G4double );
        void set_lens_surface_2_radius_x                 ( G4int, G4double );
        void set_lens_surface_2_radius_y                 ( G4int, G4double );
        void set_lens_surface_2_yLimits                  ( G4int, G4double );
        void set_lens_distance                           ( G4int, G4double );
        void set_lens_position                           ( G4int, G4double );
        void set_lens_material                           ( G4int, G4String );
        void set_lens_visibility                         ( G4int, G4bool   );
        void set_lens_color                              ( G4int, G4String );
        void set_lens_alpha                              ( G4int, G4double );
        void set_lens_forceSolid                         ( G4int, G4bool   );
        void set_lens_circular                           ( G4int, G4bool   );

        void set_directionSensitivePhotoDetector_amount  ( G4ThreeVector );
        void set_directionSensitivePhotoDetector_amount_x( G4double      );
        void set_directionSensitivePhotoDetector_amount_y( G4double      );
        void set_directionSensitivePhotoDetector_amount_z( G4double      );
        void set_checkOverlaps                           ( G4bool        );

        void set_visAttributes_visibility                ( G4bool  , G4VisAttributes*& );
        void set_visAttributes_color                     ( G4String, G4VisAttributes*& );
        void set_visAttributes_alpha                     ( G4double, G4VisAttributes*& );
        void set_visAttributes_forceSolid                ( G4bool  , G4VisAttributes*& );

        void print_parameters();

    protected:
        ConstructionMessenger();
       ~ConstructionMessenger();
        
        static ConstructionMessenger* m_instance;

        G4UIcmdWith3VectorAndUnit* m_command_world_size                            { nullptr }; G4ThreeVector m_variable_world_size                            { 1000.0 * mm, 1000.0 * mm, 1000.0 * mm };
        G4UIcmdWithAString       * m_command_world_material                        { nullptr }; G4String      m_variable_world_material                        { "G4Air" };
        G4UIcmdWithABool         * m_command_world_visibility                      { nullptr }; G4bool        m_variable_world_visibility                      { true };
        G4UIcmdWithAString       * m_command_world_color                           { nullptr }; G4String      m_variable_world_color                           { "" };
        G4UIcmdWithADouble       * m_command_world_alpha                           { nullptr }; G4double      m_variable_world_alpha                           { 0.0 };
        G4UIcmdWithABool         * m_command_world_forceSolid                      { nullptr }; G4bool        m_variable_world_forceSolid                      { true };

        G4UIcmdWithADoubleAndUnit* m_command_detector_wall_thickness               { nullptr }; G4double      m_variable_detector_wall_thickness               { 0.0 };
        G4UIcmdWithAString       * m_command_detector_wall_material                { nullptr }; G4String      m_variable_detector_wall_material                { "G4Air" };
        G4UIcmdWithABool         * m_command_detector_wall_visibility              { nullptr }; G4bool        m_variable_detector_wall_visibility              { true };
        G4UIcmdWithAString       * m_command_detector_wall_color                   { nullptr }; G4String      m_variable_detector_wall_color                   { "" };
        G4UIcmdWithADouble       * m_command_detector_wall_alpha                   { nullptr }; G4double      m_variable_detector_wall_alpha                   { 0.0 };
        G4UIcmdWithABool         * m_command_detector_wall_forceSolid              { nullptr }; G4bool        m_variable_detector_wall_forceSolid              { true };

        G4UIcmdWithAString       * m_command_detector_medium_material              { nullptr }; G4String      m_variable_detector_medium_material              { "G4Air" };
        G4UIcmdWithABool         * m_command_detector_medium_visibility            { nullptr }; G4bool        m_variable_detector_medium_visibility            { true };
        G4UIcmdWithAString       * m_command_detector_medium_color                 { nullptr }; G4String      m_variable_detector_medium_color                 { "" };
        G4UIcmdWithADouble       * m_command_detector_medium_alpha                 { nullptr }; G4double      m_variable_detector_medium_alpha                 { 0.0 };
        G4UIcmdWithABool         * m_command_detector_medium_forceSolid            { nullptr }; G4bool        m_variable_detector_medium_forceSolid            { true };

        G4UIcmdWith3VectorAndUnit* m_command_calorimeter_size                      { nullptr }; G4ThreeVector m_variable_calorimeter_size                      { 100.0 * mm, 100.0 * mm, 100.0 * mm };
        G4UIcmdWithAString       * m_command_calorimeter_material                  { nullptr }; G4String      m_variable_calorimeter_material                  { "G4Air" };
        G4UIcmdWithABool         * m_command_calorimeter_visibility                { nullptr }; G4bool        m_variable_calorimeter_visibility                { true };
        G4UIcmdWithAString       * m_command_calorimeter_color                     { nullptr }; G4String      m_variable_calorimeter_color                     { "" };
        G4UIcmdWithADouble       * m_command_calorimeter_alpha                     { nullptr }; G4double      m_variable_calorimeter_alpha                     { 0.0 };
        G4UIcmdWithABool         * m_command_calorimeter_forceSolid                { nullptr }; G4bool        m_variable_calorimeter_forceSolid                { true };

        G4UIcmdWith3VectorAndUnit* m_command_photoSensor_surface_size              { nullptr }; G4ThreeVector m_variable_photoSensor_surface_size              { 100.0 * mm, 100.0 * mm, 1.0 * mm };
        G4UIcmdWithAString       * m_command_photoSensor_surface_material          { nullptr }; G4String      m_variable_photoSensor_surface_material          { "G4Air" };
        G4UIcmdWithABool         * m_command_photoSensor_surface_visibility        { nullptr }; G4bool        m_variable_photoSensor_surface_visibility        { true };
        G4UIcmdWithAString       * m_command_photoSensor_surface_color             { nullptr }; G4String      m_variable_photoSensor_surface_color             { "" };
        G4UIcmdWithADouble       * m_command_photoSensor_surface_alpha             { nullptr }; G4double      m_variable_photoSensor_surface_alpha             { 0.0 };
        G4UIcmdWithABool         * m_command_photoSensor_surface_forceSolid        { nullptr }; G4bool        m_variable_photoSensor_surface_forceSolid        { true };

        G4UIcmdWith3VectorAndUnit* m_command_photoSensor_body_size                 { nullptr }; G4ThreeVector m_variable_photoSensor_body_size                 { 100.0 * mm, 100.0 * mm, 1.0 * mm };
        G4UIcmdWithAString       * m_command_photoSensor_body_material             { nullptr }; G4String      m_variable_photoSensor_body_material             { "G4Air" };
        G4UIcmdWithABool         * m_command_photoSensor_body_visibility           { nullptr }; G4bool        m_variable_photoSensor_body_visibility           { true };
        G4UIcmdWithAString       * m_command_photoSensor_body_color                { nullptr }; G4String      m_variable_photoSensor_body_color                { "" };
        G4UIcmdWithADouble       * m_command_photoSensor_body_alpha                { nullptr }; G4double      m_variable_photoSensor_body_alpha                { 0.0 };
        G4UIcmdWithABool         * m_command_photoSensor_body_forceSolid           { nullptr }; G4bool        m_variable_photoSensor_body_forceSolid           { true };

        G4UIcmdWithoutParameter  * m_command_lens_incramentCurrentLens             { nullptr }; G4int         m_variable_lens_currentLens                      { 0 };
        G4UIcmdWithADoubleAndUnit* m_command_lens_surface_1_radius_x               { nullptr }; G4double      m_variable_lens_surface_1_radius_x               { 0.0 * mm };
        G4UIcmdWithADoubleAndUnit* m_command_lens_surface_1_radius_y               { nullptr }; G4double      m_variable_lens_surface_1_radius_y               { 0.0 * mm };
        G4UIcmdWithADoubleAndUnit* m_command_lens_surface_1_yLimits                { nullptr }; G4double      m_variable_lens_surface_1_yLimits                { 0.0 * mm };
        G4UIcmdWithADoubleAndUnit* m_command_lens_surface_2_radius_x               { nullptr }; G4double      m_variable_lens_surface_2_radius_x               { 0.0 * mm };
        G4UIcmdWithADoubleAndUnit* m_command_lens_surface_2_radius_y               { nullptr }; G4double      m_variable_lens_surface_2_radius_y               { 0.0 * mm };
        G4UIcmdWithADoubleAndUnit* m_command_lens_surface_2_yLimits                { nullptr }; G4double      m_variable_lens_surface_2_yLimits                { 0.0 * mm };
        G4UIcmdWithADoubleAndUnit* m_command_lens_distance                         { nullptr }; G4double      m_variable_lens_distance                         { 0.0 * mm };
        G4UIcmdWithADoubleAndUnit* m_command_lens_position                         { nullptr }; G4double      m_variable_lens_position                         { 0.0 * mm };
        G4UIcmdWithAString       * m_command_lens_material                         { nullptr }; G4String      m_variable_lens_material                         { "G4Air" };
        G4UIcmdWithABool         * m_command_lens_visibility                       { nullptr }; G4bool        m_variable_lens_visibility                       { true };
        G4UIcmdWithAString       * m_command_lens_color                            { nullptr }; G4String      m_variable_lens_color                            { "" };
        G4UIcmdWithADouble       * m_command_lens_alpha                            { nullptr }; G4double      m_variable_lens_alpha                            { 0.0 };
        G4UIcmdWithABool         * m_command_lens_forceSolid                       { nullptr }; G4bool        m_variable_lens_forceSolid                       { true };
        G4UIcmdWithABool         * m_command_lens_circular                         { nullptr }; G4bool        m_variable_lens_circular                         { true };

        G4UIcmdWith3Vector       * m_command_directionSensitivePhotoDetector_amount{ nullptr }; G4ThreeVector m_variable_directionSensitivePhotoDetector_amount{ 1, 1, 1 };
        G4UIcmdWithABool         * m_command_checkOverlaps                         { nullptr }; G4bool        m_variable_checkOverlaps                         { true };

        G4VisAttributes* m_variable_world_visAttributes              { nullptr };
        G4VisAttributes* m_variable_detector_wall_visAttributes      { nullptr };
        G4VisAttributes* m_variable_detector_medium_visAttributes    { nullptr };
        G4VisAttributes* m_variable_calorimeter_visAttributes        { nullptr };
        G4VisAttributes* m_variable_photoSensor_surface_visAttributes{ nullptr };
        G4VisAttributes* m_variable_photoSensor_body_visAttributes   { nullptr };

        vector< G4double > m_variable_lens_surface_1_radii_x;
        vector< G4double > m_variable_lens_surface_1_radii_y;
        vector< G4double > m_variable_lens_surface_1_yLimitss;
        vector< G4double > m_variable_lens_surface_2_radii_x;
        vector< G4double > m_variable_lens_surface_2_radii_y;
        vector< G4double > m_variable_lens_surface_2_yLimitss;
        vector< G4double > m_variable_lens_distances;
        vector< G4double > m_variable_lens_positions;
        vector< G4String > m_variable_lens_materials;
        vector< G4bool >   m_variable_lens_visibilities;
        vector< G4String > m_variable_lens_colors;
        vector< G4double > m_variable_lens_alphas;
        vector< G4bool >   m_variable_lens_forceSolids;
        vector< G4bool >   m_variable_lens_circulars;
        vector< G4VisAttributes* > m_variable_lens_visAttributess;
};

#endif