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

#include "ConstructionMessenger.hh"

ConstructionMessenger* ConstructionMessenger::m_instance{ nullptr };

ConstructionMessenger::ConstructionMessenger() {
    m_command_world_size                             = new G4UIcmdWith3VectorAndUnit( "/geometry/world/size"                            , this );
    m_command_world_material                         = new G4UIcmdWithAString       ( "/geometry/world/material"                        , this );
    m_command_world_visibility                       = new G4UIcmdWithABool         ( "/geometry/world/visibility"                      , this );
    m_command_world_color                            = new G4UIcmdWithAString       ( "/geometry/world/color"                           , this );
    m_command_world_alpha                            = new G4UIcmdWithADouble       ( "/geometry/world/alpha"                           , this );
    m_command_world_forceSolid                       = new G4UIcmdWithABool         ( "/geometry/world/forceSolid"                      , this );

    m_command_detector_wall_thickness                = new G4UIcmdWithADoubleAndUnit( "/geometry/detector/wall/thickness"               , this );
    m_command_detector_wall_material                 = new G4UIcmdWithAString       ( "/geometry/detector/wall/material"                , this );
    m_command_detector_wall_visibility               = new G4UIcmdWithABool         ( "/geometry/detector/wall/visibility"              , this );
    m_command_detector_wall_color                    = new G4UIcmdWithAString       ( "/geometry/detector/wall/color"                   , this );
    m_command_detector_wall_alpha                    = new G4UIcmdWithADouble       ( "/geometry/detector/wall/alpha"                   , this );
    m_command_detector_wall_forceSolid               = new G4UIcmdWithABool         ( "/geometry/detector/wall/forceSolid"              , this );

    m_command_detector_medium_material               = new G4UIcmdWithAString       ( "/geometry/detector/medium/material"              , this );
    m_command_detector_medium_visibility             = new G4UIcmdWithABool         ( "/geometry/detector/medium/visibility"            , this );
    m_command_detector_medium_color                  = new G4UIcmdWithAString       ( "/geometry/detector/medium/color"                 , this );
    m_command_detector_medium_alpha                  = new G4UIcmdWithADouble       ( "/geometry/detector/medium/alpha"                 , this );
    m_command_detector_medium_forceSolid             = new G4UIcmdWithABool         ( "/geometry/detector/medium/forceSolid"            , this );

    m_command_calorimeter_size                       = new G4UIcmdWith3VectorAndUnit( "/geometry/calorimeter/size"                      , this );
    m_command_calorimeter_material                   = new G4UIcmdWithAString       ( "/geometry/calorimeter/material"                  , this );
    m_command_calorimeter_visibility                 = new G4UIcmdWithABool         ( "/geometry/calorimeter/visibility"                , this );
    m_command_calorimeter_color                      = new G4UIcmdWithAString       ( "/geometry/calorimeter/color"                     , this );
    m_command_calorimeter_alpha                      = new G4UIcmdWithADouble       ( "/geometry/calorimeter/alpha"                     , this );
    m_command_calorimeter_forceSolid                 = new G4UIcmdWithABool         ( "/geometry/calorimeter/forceSolid"                , this );

    m_command_photoSensor_surface_size               = new G4UIcmdWith3VectorAndUnit( "/geometry/photoSensor/surface/size"              , this );
    m_command_photoSensor_surface_material           = new G4UIcmdWithAString       ( "/geometry/photoSensor/surface/material"          , this );
    m_command_photoSensor_surface_visibility         = new G4UIcmdWithABool         ( "/geometry/photoSensor/surface/visibility"        , this );
    m_command_photoSensor_surface_color              = new G4UIcmdWithAString       ( "/geometry/photoSensor/surface/color"             , this );
    m_command_photoSensor_surface_alpha              = new G4UIcmdWithADouble       ( "/geometry/photoSensor/surface/alpha"             , this );
    m_command_photoSensor_surface_forceSolid         = new G4UIcmdWithABool         ( "/geometry/photoSensor/surface/forceSolid"        , this );

    m_command_photoSensor_body_size                  = new G4UIcmdWith3VectorAndUnit( "/geometry/photoSensor/body/size"                 , this );
    m_command_photoSensor_body_material              = new G4UIcmdWithAString       ( "/geometry/photoSensor/body/material"             , this );
    m_command_photoSensor_body_visibility            = new G4UIcmdWithABool         ( "/geometry/photoSensor/body/visibility"           , this );
    m_command_photoSensor_body_color                 = new G4UIcmdWithAString       ( "/geometry/photoSensor/body/color"                , this );
    m_command_photoSensor_body_alpha                 = new G4UIcmdWithADouble       ( "/geometry/photoSensor/body/alpha"                , this );
    m_command_photoSensor_body_forceSolid            = new G4UIcmdWithABool         ( "/geometry/photoSensor/body/forceSolid"           , this );

    m_command_lens_incramentCurrentLens              = new G4UIcmdWithoutParameter  ( "/geometry/lens/incramentCurrentLens"             , this );
    m_command_lens_surface_1_radius_x                = new G4UIcmdWithADoubleAndUnit( "/geometry/lens/surfaces/1/radius/x"              , this );
    m_command_lens_surface_1_radius_y                = new G4UIcmdWithADoubleAndUnit( "/geometry/lens/surfaces/1/radius/y"              , this );
    m_command_lens_surface_1_yLimits                 = new G4UIcmdWithADoubleAndUnit( "/geometry/lens/surfaces/1/yLimits"               , this );
    m_command_lens_surface_2_radius_x                = new G4UIcmdWithADoubleAndUnit( "/geometry/lens/surfaces/2/radius/x"              , this );
    m_command_lens_surface_2_radius_y                = new G4UIcmdWithADoubleAndUnit( "/geometry/lens/surfaces/2/radius/y"              , this );
    m_command_lens_surface_2_yLimits                 = new G4UIcmdWithADoubleAndUnit( "/geometry/lens/surfaces/2/yLimits"               , this );
    m_command_lens_distance                          = new G4UIcmdWithADoubleAndUnit( "/geometry/lens/distance"                         , this );
    m_command_lens_position                          = new G4UIcmdWithADoubleAndUnit( "/geometry/lens/position"                         , this );
    m_command_lens_material                          = new G4UIcmdWithAString       ( "/geometry/lens/material"                         , this );
    m_command_lens_visibility                        = new G4UIcmdWithABool         ( "/geometry/lens/visibility"                       , this );
    m_command_lens_color                             = new G4UIcmdWithAString       ( "/geometry/lens/color"                            , this );
    m_command_lens_alpha                             = new G4UIcmdWithADouble       ( "/geometry/lens/alpha"                            , this );
    m_command_lens_forceSolid                        = new G4UIcmdWithABool         ( "/geometry/lens/forceSolid"                       , this );

    m_command_directionSensitivePhotoDetector_amount = new G4UIcmdWith3Vector       ( "/geometry/directionSensitivePhotoDetector/amount", this );
    m_command_checkOverlaps                          = new G4UIcmdWithABool         ( "/geometry/checkOverlaps"                         , this );
}

ConstructionMessenger::~ConstructionMessenger() {
    if( m_command_world_size                             ) delete m_command_world_size                            ;
    if( m_command_world_material                         ) delete m_command_world_material                        ;
    if( m_command_world_visibility                       ) delete m_command_world_visibility                      ;
    if( m_command_world_color                            ) delete m_command_world_color                           ;
    if( m_command_world_alpha                            ) delete m_command_world_alpha                           ;
    if( m_command_world_forceSolid                       ) delete m_command_world_forceSolid                      ;

    if( m_command_detector_wall_thickness                ) delete m_command_detector_wall_thickness               ;
    if( m_command_detector_wall_material                 ) delete m_command_detector_wall_material                ;
    if( m_command_detector_wall_visibility               ) delete m_command_detector_wall_visibility              ;
    if( m_command_detector_wall_color                    ) delete m_command_detector_wall_color                   ;
    if( m_command_detector_wall_alpha                    ) delete m_command_detector_wall_alpha                   ;
    if( m_command_detector_wall_forceSolid               ) delete m_command_detector_wall_forceSolid              ;

    if( m_command_detector_medium_material               ) delete m_command_detector_medium_material              ;
    if( m_command_detector_medium_visibility             ) delete m_command_detector_medium_visibility            ;
    if( m_command_detector_medium_color                  ) delete m_command_detector_medium_color                 ;
    if( m_command_detector_medium_alpha                  ) delete m_command_detector_medium_alpha                 ;
    if( m_command_detector_medium_forceSolid             ) delete m_command_detector_medium_forceSolid            ;

    if( m_command_calorimeter_size                       ) delete m_command_calorimeter_size                      ;
    if( m_command_calorimeter_material                   ) delete m_command_calorimeter_material                  ;
    if( m_command_calorimeter_visibility                 ) delete m_command_calorimeter_visibility                ;
    if( m_command_calorimeter_color                      ) delete m_command_calorimeter_color                     ;
    if( m_command_calorimeter_alpha                      ) delete m_command_calorimeter_alpha                     ;
    if( m_command_calorimeter_forceSolid                 ) delete m_command_calorimeter_forceSolid                ;

    if( m_command_photoSensor_surface_size               ) delete m_command_photoSensor_surface_size              ;
    if( m_command_photoSensor_surface_material           ) delete m_command_photoSensor_surface_material          ;
    if( m_command_photoSensor_surface_visibility         ) delete m_command_photoSensor_surface_visibility        ;
    if( m_command_photoSensor_surface_color              ) delete m_command_photoSensor_surface_color             ;
    if( m_command_photoSensor_surface_alpha              ) delete m_command_photoSensor_surface_alpha             ;
    if( m_command_photoSensor_surface_forceSolid         ) delete m_command_photoSensor_surface_forceSolid        ;

    if( m_command_photoSensor_body_size                  ) delete m_command_photoSensor_body_size                 ;
    if( m_command_photoSensor_body_material              ) delete m_command_photoSensor_body_material             ;
    if( m_command_photoSensor_body_visibility            ) delete m_command_photoSensor_body_visibility           ;
    if( m_command_photoSensor_body_color                 ) delete m_command_photoSensor_body_color                ;
    if( m_command_photoSensor_body_alpha                 ) delete m_command_photoSensor_body_alpha                ;
    if( m_command_photoSensor_body_forceSolid            ) delete m_command_photoSensor_body_forceSolid           ;

    if( m_command_lens_incramentCurrentLens              ) delete m_command_lens_incramentCurrentLens             ;
    if( m_command_lens_surface_1_radius_x                ) delete m_command_lens_surface_1_radius_x               ;
    if( m_command_lens_surface_1_radius_y                ) delete m_command_lens_surface_1_radius_y               ;
    if( m_command_lens_surface_1_yLimits                 ) delete m_command_lens_surface_1_yLimits                ;
    if( m_command_lens_surface_2_radius_x                ) delete m_command_lens_surface_2_radius_x               ;
    if( m_command_lens_surface_2_radius_y                ) delete m_command_lens_surface_2_radius_y               ;
    if( m_command_lens_surface_2_yLimits                 ) delete m_command_lens_surface_2_yLimits                ;
    if( m_command_lens_distance                          ) delete m_command_lens_distance                         ;
    if( m_command_lens_position                          ) delete m_command_lens_position                         ;
    if( m_command_lens_material                          ) delete m_command_lens_material                         ;
    if( m_command_lens_visibility                        ) delete m_command_lens_visibility                       ;
    if( m_command_lens_color                             ) delete m_command_lens_color                            ;
    if( m_command_lens_alpha                             ) delete m_command_lens_alpha                            ;
    if( m_command_lens_forceSolid                        ) delete m_command_lens_forceSolid                       ;

    if( m_command_directionSensitivePhotoDetector_amount ) delete m_command_directionSensitivePhotoDetector_amount;
    if( m_command_checkOverlaps                          ) delete m_command_checkOverlaps                         ;

    if( m_variable_world_visAttributes                   ) delete m_variable_world_visAttributes                  ;
    if( m_variable_detector_wall_visAttributes           ) delete m_variable_detector_wall_visAttributes          ;
    if( m_variable_detector_medium_visAttributes         ) delete m_variable_detector_medium_visAttributes        ;
    if( m_variable_calorimeter_visAttributes             ) delete m_variable_calorimeter_visAttributes            ;
    if( m_variable_photoSensor_surface_visAttributes     ) delete m_variable_photoSensor_surface_visAttributes    ;
    if( m_variable_photoSensor_body_visAttributes        ) delete m_variable_photoSensor_body_visAttributes       ;
    for( G4VisAttributes* visAttributes : m_variable_lens_visAttributess ) if( visAttributes ) delete visAttributes;
}

ConstructionMessenger* ConstructionMessenger::get_instance() {
    if( !m_instance ) 
        m_instance = new ConstructionMessenger();
    return m_instance;
}

void ConstructionMessenger::delete_instance() {
    if( m_instance ) {
        delete m_instance;
        m_instance = nullptr;
    }
}

void ConstructionMessenger::SetNewValue( G4UIcommand* t_command, G4String t_newValue ) {
    if( t_command == m_command_world_size ) { 
        set_world_size( m_command_world_size->GetNew3VectorValue( t_newValue ) );
        G4cout << "Setting `world_size' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_world_material ) { 
        set_world_material( t_newValue );
        G4cout << "Setting `world_medium_material' to "
               << t_newValue << G4endl;
    } else if( t_command == m_command_world_visibility ) {
        set_world_visibility( m_command_world_visibility->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `world_visibility' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_world_color ) { 
        set_world_color( t_newValue );
        G4cout << "Setting `world_color' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_world_alpha ) { 
        set_world_alpha( m_command_world_alpha->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `world_alpha' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_world_forceSolid ) {
        set_world_forceSolid( m_command_world_forceSolid->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `world_forceSolid' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_wall_thickness ) { 
        set_detector_wall_thickness( m_command_detector_wall_thickness->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `detector_wall_thickness' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_wall_material ) { 
        set_detector_wall_material( t_newValue );
        G4cout << "Setting `detector_wall_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_wall_visibility ) {
        set_detector_wall_visibility( m_command_detector_wall_visibility->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `detector_wall_visibility' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_wall_color ) { 
        set_detector_wall_color( t_newValue );
        G4cout << "Setting `detector_wall_color' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_wall_alpha ) { 
        set_detector_wall_alpha( m_command_detector_wall_alpha->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `detector_wall_alpha' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_wall_forceSolid ) {
        set_detector_wall_forceSolid( m_command_detector_wall_forceSolid->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `detector_wall_forceSolid' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_medium_material ) { 
        set_detector_medium_material( t_newValue );
        G4cout << "Setting `detector_medium_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_medium_visibility ) {
        set_detector_medium_visibility( m_command_detector_medium_visibility->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `detector_medium_visibility' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_medium_color ) { 
        set_detector_medium_color( t_newValue );
        G4cout << "Setting `detector_medium_color' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_medium_alpha ) { 
        set_detector_medium_alpha( m_command_detector_medium_alpha->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `detector_medium_alpha' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_medium_forceSolid ) {
        set_detector_medium_forceSolid( m_command_detector_medium_forceSolid->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `detector_medium_forceSolid' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_size ) { 
        set_calorimeter_size( m_command_calorimeter_size->GetNew3VectorValue( t_newValue ) );
        G4cout << "Setting `calorimeter_size' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_material ) { 
        set_calorimeter_material( t_newValue );
        G4cout << "Setting `calorimeter_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_visibility ) {
        set_calorimeter_visibility( m_command_calorimeter_visibility->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `calorimeter_visibility' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_color ) { 
        set_calorimeter_color( t_newValue );
        G4cout << "Setting `calorimeter_color' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_alpha ) { 
        set_calorimeter_alpha( m_command_calorimeter_alpha->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `calorimeter_alpha' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_forceSolid ) {
        set_calorimeter_forceSolid( m_command_calorimeter_forceSolid->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `calorimeter_forceSolid' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_surface_size ) {
        set_photoSensor_surface_size( m_command_photoSensor_surface_size->GetNew3VectorValue( t_newValue ) );
        G4cout << "Setting `photoSensor_surface_size' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_surface_material ) { 
        set_photoSensor_surface_material( t_newValue );
        G4cout << "Setting `photoSensor_surface_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_surface_visibility ) {
        set_photoSensor_surface_visibility( m_command_photoSensor_surface_visibility->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `photoSensor_surface_visibility' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_surface_color ) { 
        set_photoSensor_surface_color( t_newValue );
        G4cout << "Setting `photoSensor_surface_color' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_surface_alpha ) { 
        set_photoSensor_surface_alpha( m_command_photoSensor_surface_alpha->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `photoSensor_surface_alpha' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_surface_forceSolid ) {
        set_photoSensor_surface_forceSolid( m_command_photoSensor_surface_forceSolid->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `photoSensor_surface_forceSolid' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_body_size ) {
        set_photoSensor_body_size( m_command_photoSensor_body_size->GetNew3VectorValue( t_newValue ) );
        G4cout << "Setting `photoSensor_body_size' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_body_material ) { 
        set_photoSensor_body_material( t_newValue );
        G4cout << "Setting `photoSensor_body_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_body_visibility ) {
        set_photoSensor_body_visibility( m_command_photoSensor_body_visibility->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `photoSensor_body_visibility' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_body_color ) { 
        set_photoSensor_body_color( t_newValue );
        G4cout << "Setting `photoSensor_body_color' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_body_alpha ) { 
        set_photoSensor_body_alpha( m_command_photoSensor_body_alpha->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `photoSensor_body_alpha' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_body_forceSolid ) {
        set_photoSensor_body_forceSolid( m_command_photoSensor_body_forceSolid->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `photoSensor_body_forceSolid' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_incramentCurrentLens ) {
        set_lens_incramentCurrentLens();
        G4cout << "Setting `lens_incramentCurrentLens' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_surface_1_radius_x ) {
        set_lens_surface_1_radius_x( m_variable_lens_currentLens, m_command_lens_surface_1_radius_x->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `lens_surface_1_radius_x' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_surface_1_radius_y ) {
        set_lens_surface_1_radius_y( m_variable_lens_currentLens, m_command_lens_surface_1_radius_y->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `lens_surface_1_radius_y' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_surface_1_yLimits ) {
        set_lens_surface_1_yLimits( m_variable_lens_currentLens, m_command_lens_surface_1_yLimits->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `lens_surface_1_yLimits' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_surface_2_radius_x ) {
        set_lens_surface_2_radius_x( m_variable_lens_currentLens, m_command_lens_surface_2_radius_x->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `lens_surface_2_radius_x' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_surface_2_radius_y ) {
        set_lens_surface_2_radius_y( m_variable_lens_currentLens, m_command_lens_surface_2_radius_y->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `lens_surface_2_radius_y' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_surface_2_yLimits ) {
        set_lens_surface_2_yLimits( m_variable_lens_currentLens, m_command_lens_surface_2_yLimits->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `lens_surface_2_yLimits' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_distance ) {
        set_lens_distance( m_variable_lens_currentLens, m_command_lens_distance->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `lens_distance' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_position ) {
        set_lens_position( m_variable_lens_currentLens, m_command_lens_position->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `lens_position' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_material ) { 
        set_lens_material( m_variable_lens_currentLens, t_newValue );
        G4cout << "Setting `lens_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_visibility ) {
        set_lens_visibility( m_variable_lens_currentLens, m_command_lens_visibility->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `lens_visibility' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_color ) { 
        set_lens_color( m_variable_lens_currentLens, t_newValue );
        G4cout << "Setting `lens_color' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_alpha ) { 
        set_lens_alpha( m_variable_lens_currentLens, m_command_lens_alpha->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `lens_alpha' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_forceSolid ) {
        set_lens_forceSolid( m_variable_lens_currentLens, m_command_lens_forceSolid->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `lens_forceSolid' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_directionSensitivePhotoDetector_amount ) {
        set_directionSensitivePhotoDetector_amount( m_command_directionSensitivePhotoDetector_amount->GetNew3VectorValue( t_newValue ) );
        G4cout << "Setting `directionSensitivePhotoDetector_amount' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_checkOverlaps ) {
        set_checkOverlaps( m_command_checkOverlaps->GetNewBoolValue( t_newValue ) );
        G4cout << "Setting `checkOverlaps' to " 
               << t_newValue << G4endl;
    } else {
        G4cerr << "ERROR: ConstructionMessenger::SetNewValue: Unknown command" << G4endl;
    }
}

void ConstructionMessenger::print_parameters() {
    G4cout << "[-]==: Attempting to use the following parameters" << G4endl
           << " |=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << G4endl
           << " |--< world_x >----------------------------------: " << get_world_size_x                            () << G4endl
           << " |--< world_y >----------------------------------: " << get_world_size_y                            () << G4endl
           << " |--< world_z >----------------------------------: " << get_world_size_z                            () << G4endl
           << " |--< world_material >---------------------------: " << get_world_material                          () << G4endl
           << " |--< world_visibility >-------------------------: " << get_world_visibility                        () << G4endl
           << " |--< world_color >------------------------------: " << get_world_color                             () << G4endl
           << " |--< world_alpha >------------------------------: " << get_world_alpha                             () << G4endl
           << " |--< world_forceSolid >-------------------------: " << get_world_forceSolid                        () << G4endl
           << " |                                                 "                                                   << G4endl
           << " |--< detector_wall_thickness >------------------: " << get_detector_wall_thickness                 () << G4endl
           << " |--< detector_wall_material >-------------------: " << get_detector_wall_material                  () << G4endl
           << " |--< detector_wall_visibility >-----------------: " << get_detector_wall_visibility                () << G4endl
           << " |--< detector_wall_color >----------------------: " << get_detector_wall_color                     () << G4endl
           << " |--< detector_wall_alpha >----------------------: " << get_detector_wall_alpha                     () << G4endl
           << " |--< detector_wall_forceSolid >-----------------: " << get_detector_wall_forceSolid                () << G4endl
           << " |                                                 "                                                   << G4endl
           << " |--< detector_medium_material >-----------------: " << get_detector_medium_material                () << G4endl
           << " |--< detector_medium_visibility >---------------: " << get_detector_medium_visibility              () << G4endl
           << " |--< detector_medium_color >--------------------: " << get_detector_medium_color                   () << G4endl
           << " |--< detector_medium_alpha >--------------------: " << get_detector_medium_alpha                   () << G4endl
           << " |--< detector_medium_forceSolid >---------------: " << get_detector_medium_forceSolid              () << G4endl
           << " |                                                 "                                                   << G4endl
           << " |--< calorimeter_size_width >-------------------: " << get_calorimeter_size_width                  () << G4endl
           << " |--< calorimeter_size_height >------------------: " << get_calorimeter_size_height                 () << G4endl
           << " |--< calorimeter_size_depth >-------------------: " << get_calorimeter_size_depth                  () << G4endl
           << " |--< calorimeter_material >---------------------: " << get_calorimeter_material                    () << G4endl
           << " |--< calorimeter_visibility >-------------------: " << get_calorimeter_visibility                  () << G4endl
           << " |--< calorimeter_color >------------------------: " << get_calorimeter_color                       () << G4endl
           << " |--< calorimeter_alpha >------------------------: " << get_calorimeter_alpha                       () << G4endl
           << " |--< calorimeter_forceSolid >-------------------: " << get_calorimeter_forceSolid                  () << G4endl
           << " |                                                 "                                                   << G4endl
           << " |--< photoSensor_surface_size_width >-----------: " << get_photoSensor_surface_size_width          () << G4endl
           << " |--< photoSensor_surface_size_height >----------: " << get_photoSensor_surface_size_height         () << G4endl
           << " |--< photoSensor_surface_size_depth >-----------: " << get_photoSensor_surface_size_depth          () << G4endl
           << " |--< photoSensor_surface_material >-------------: " << get_photoSensor_surface_material            () << G4endl
           << " |--< photoSensor_surface_visibility >-----------: " << get_photoSensor_surface_visibility          () << G4endl
           << " |--< photoSensor_surface_color >----------------: " << get_photoSensor_surface_color               () << G4endl
           << " |--< photoSensor_surface_alpha >----------------: " << get_photoSensor_surface_alpha               () << G4endl
           << " |--< photoSensor_surface_forceSolid >-----------: " << get_photoSensor_surface_forceSolid          () << G4endl
           << " |                                                 "                                                   << G4endl
           << " |--< photoSensor_body_size_width >--------------: " << get_photoSensor_body_size_width             () << G4endl
           << " |--< photoSensor_body_size_height >-------------: " << get_photoSensor_body_size_height            () << G4endl
           << " |--< photoSensor_body_size_depth >--------------: " << get_photoSensor_body_size_depth             () << G4endl
           << " |--< photoSensor_body_material >----------------: " << get_photoSensor_body_material               () << G4endl
           << " |--< photoSensor_body_visibility >--------------: " << get_photoSensor_body_visibility             () << G4endl
           << " |--< photoSensor_body_color >-------------------: " << get_photoSensor_body_color                  () << G4endl
           << " |--< photoSensor_body_alpha >-------------------: " << get_photoSensor_body_alpha                  () << G4endl
           << " |--< photoSensor_body_forceSolid >--------------: " << get_photoSensor_body_forceSolid             () << G4endl
           << " |                                                 "                                                   << G4endl;
    for( G4int nLens{ 0 }; nLens <= m_variable_lens_currentLens; nLens++ ) {
    G4cout << " |--< lens_currentLens >-------------------------: " << nLens                                                 << G4endl
           << " |--< lens_surface_1_radius_x >------------------: " << get_lens_surface_1_radius_x                 ( nLens ) << G4endl
           << " |--< lens_surface_1_radius_y >------------------: " << get_lens_surface_1_radius_y                 ( nLens ) << G4endl
           << " |--< lens_surface_1_yLimits >-------------------: " << get_lens_surface_1_yLimits                  ( nLens ) << G4endl
           << " |--< lens_surface_2_radius_x >------------------: " << get_lens_surface_2_radius_x                 ( nLens ) << G4endl
           << " |--< lens_surface_2_radius_y >------------------: " << get_lens_surface_2_radius_y                 ( nLens ) << G4endl
           << " |--< lens_surface_2_yLimits >-------------------: " << get_lens_surface_2_yLimits                  ( nLens ) << G4endl
           << " |--< lens_distance >----------------------------: " << get_lens_distance                           ( nLens ) << G4endl
           << " |--< lens_position >----------------------------: " << get_lens_position                           ( nLens ) << G4endl
           << " |--< lens_material >----------------------------: " << get_lens_material                           ( nLens ) << G4endl
           << " |--< lens_visibility >--------------------------: " << get_lens_visibility                         ( nLens ) << G4endl
           << " |--< lens_color >-------------------------------: " << get_lens_color                              ( nLens ) << G4endl
           << " |--< lens_alpha >-------------------------------: " << get_lens_alpha                              ( nLens ) << G4endl
           << " |--< lens_forceSolid >--------------------------: " << get_lens_forceSolid                         ( nLens ) << G4endl
           << " |                                                 "                                                          << G4endl;
    }
    G4cout << " |--< directionSensitivePhotoDetector_amount_x >-: " << get_directionSensitivePhotoDetector_amount_x() << G4endl
           << " |--< directionSensitivePhotoDetector_amount_y >-: " << get_directionSensitivePhotoDetector_amount_y() << G4endl
           << " |--< directionSensitivePhotoDetector_amount_z >-: " << get_directionSensitivePhotoDetector_amount_z() << G4endl
           << " |--< checkOverlaps >----------------------------: " << get_checkOverlaps                           () << G4endl;
}

G4ThreeVector ConstructionMessenger::get_world_size() { 
    return m_variable_world_size;
}
G4double ConstructionMessenger::get_world_size_x() { 
    return m_variable_world_size.x();
}
G4double ConstructionMessenger::get_world_size_y() { 
    return m_variable_world_size.y();
}
G4double ConstructionMessenger::get_world_size_z() { 
    return m_variable_world_size.z();
}
G4String ConstructionMessenger::get_world_material() { 
    return m_variable_world_material;
}
G4bool ConstructionMessenger::get_world_visibility() { 
    return m_variable_world_visibility;
}
G4String ConstructionMessenger::get_world_color() { 
    return m_variable_world_color;
}
G4double ConstructionMessenger::get_world_alpha() { 
    return m_variable_world_alpha;
}
G4bool ConstructionMessenger::get_world_forceSolid() { 
    return m_variable_world_forceSolid;
}
G4VisAttributes* ConstructionMessenger::get_world_visAttributes() { 
    return m_variable_world_visAttributes;
}
G4double ConstructionMessenger::get_detector_wall_thickness() { 
    return m_variable_detector_wall_thickness;
}
G4String ConstructionMessenger::get_detector_wall_material() { 
    return m_variable_detector_wall_material;
}
G4bool ConstructionMessenger::get_detector_wall_visibility() { 
    return m_variable_detector_wall_visibility;
}
G4String ConstructionMessenger::get_detector_wall_color() { 
    return m_variable_detector_wall_color;
}
G4double ConstructionMessenger::get_detector_wall_alpha() { 
    return m_variable_detector_wall_alpha;
}
G4bool ConstructionMessenger::get_detector_wall_forceSolid() { 
    return m_variable_detector_wall_forceSolid;
}
G4VisAttributes* ConstructionMessenger::get_detector_wall_visAttributes() {
    return m_variable_detector_wall_visAttributes;
}
G4String ConstructionMessenger::get_detector_medium_material() { 
    return m_variable_detector_medium_material;
}
G4bool ConstructionMessenger::get_detector_medium_visibility() { 
    return m_variable_detector_medium_visibility;
}
G4String ConstructionMessenger::get_detector_medium_color() { 
    return m_variable_detector_medium_color;
}
G4double ConstructionMessenger::get_detector_medium_alpha() { 
    return m_variable_detector_medium_alpha;
}
G4bool ConstructionMessenger::get_detector_medium_forceSolid() { 
    return m_variable_detector_medium_forceSolid;
}
G4VisAttributes* ConstructionMessenger::get_detector_medium_visAttributes() {
    return m_variable_detector_medium_visAttributes;
}
G4ThreeVector ConstructionMessenger::get_calorimeter_size() { 
    return m_variable_calorimeter_size;
}
G4double ConstructionMessenger::get_calorimeter_size_width() { 
    return m_variable_calorimeter_size.x();
}
G4double ConstructionMessenger::get_calorimeter_size_height() { 
    return m_variable_calorimeter_size.y();
}
G4double ConstructionMessenger::get_calorimeter_size_depth() { 
    return m_variable_calorimeter_size.z();
}
G4String ConstructionMessenger::get_calorimeter_material() { 
    return m_variable_calorimeter_material;
}
G4bool ConstructionMessenger::get_calorimeter_visibility() { 
    return m_variable_calorimeter_visibility;
}
G4String ConstructionMessenger::get_calorimeter_color() { 
    return m_variable_calorimeter_color;
}
G4double ConstructionMessenger::get_calorimeter_alpha() { 
    return m_variable_calorimeter_alpha;
}
G4bool ConstructionMessenger::get_calorimeter_forceSolid() { 
    return m_variable_calorimeter_forceSolid;
}
G4VisAttributes* ConstructionMessenger::get_calorimeter_visAttributes() {
    return m_variable_calorimeter_visAttributes;
}
G4ThreeVector ConstructionMessenger::get_photoSensor_surface_size() { 
    return m_variable_photoSensor_surface_size;
}
G4double ConstructionMessenger::get_photoSensor_surface_size_width() { 
    return m_variable_photoSensor_surface_size.x();
}
G4double ConstructionMessenger::get_photoSensor_surface_size_height() { 
    return m_variable_photoSensor_surface_size.y();
}
G4double ConstructionMessenger::get_photoSensor_surface_size_depth() { 
    return m_variable_photoSensor_surface_size.z();
}
G4double ConstructionMessenger::get_photoSensor_surface_size_widthAndHeight() { 
    if( m_variable_photoSensor_surface_size.x() == m_variable_photoSensor_surface_size.y() )
        return m_variable_photoSensor_surface_size.x();
    else
        G4Exception( "ConstructionMessenger::get_photoSensor_surface_size_widthAndHeight()",
                     "m_variable_photoSensor_size.x() != m_variable_photoSensor_size.y()",
                     FatalException,
                     "PhotoSensor surface width and height are not equal." );
        return 0;
}
G4String ConstructionMessenger::get_photoSensor_surface_material() { 
    return m_variable_photoSensor_surface_material;
}
G4bool ConstructionMessenger::get_photoSensor_surface_visibility() { 
    return m_variable_photoSensor_surface_visibility;
}
G4String ConstructionMessenger::get_photoSensor_surface_color() { 
    return m_variable_photoSensor_surface_color;
}
G4double ConstructionMessenger::get_photoSensor_surface_alpha() { 
    return m_variable_photoSensor_surface_alpha;
}
G4bool ConstructionMessenger::get_photoSensor_surface_forceSolid() { 
    return m_variable_photoSensor_surface_forceSolid;
}
G4VisAttributes* ConstructionMessenger::get_photoSensor_surface_visAttributes() {
    return m_variable_photoSensor_surface_visAttributes;
}
G4ThreeVector ConstructionMessenger::get_photoSensor_body_size() { 
    return m_variable_photoSensor_body_size;
}
G4double ConstructionMessenger::get_photoSensor_body_size_width() { 
    return m_variable_photoSensor_body_size.x();
}
G4double ConstructionMessenger::get_photoSensor_body_size_height() { 
    return m_variable_photoSensor_body_size.y();
}
G4double ConstructionMessenger::get_photoSensor_body_size_depth() { 
    return m_variable_photoSensor_body_size.z();
}
G4double ConstructionMessenger::get_photoSensor_body_size_widthAndHeight() { 
    if( m_variable_photoSensor_body_size.x() == m_variable_photoSensor_body_size.y() )
        return m_variable_photoSensor_body_size.x();
    else
        G4Exception( "ConstructionMessenger::get_photoSensor_body_size_widthAndHeight()",
                     "m_variable_photoSensor_size.x() != m_variable_photoSensor_size.y()",
                     FatalException,
                     "PhotoSensor body width and height are not equal." );
        return 0;
}
G4String ConstructionMessenger::get_photoSensor_body_material() { 
    return m_variable_photoSensor_body_material;
}
G4bool ConstructionMessenger::get_photoSensor_body_visibility() { 
    return m_variable_photoSensor_body_visibility;
}
G4String ConstructionMessenger::get_photoSensor_body_color() { 
    return m_variable_photoSensor_body_color;
}
G4double ConstructionMessenger::get_photoSensor_body_alpha() { 
    return m_variable_photoSensor_body_alpha;
}
G4bool ConstructionMessenger::get_photoSensor_body_forceSolid() { 
    return m_variable_photoSensor_body_forceSolid;
}
G4VisAttributes* ConstructionMessenger::get_photoSensor_body_visAttributes() {
    return m_variable_photoSensor_body_visAttributes;
}
G4int ConstructionMessenger::get_lens_amount() {
    if( m_variable_lens_currentLens + 1 != m_variable_lens_surface_1_radii_x .size() ||
        m_variable_lens_currentLens + 1 != m_variable_lens_surface_1_radii_y .size() ||
        m_variable_lens_currentLens + 1 != m_variable_lens_surface_1_yLimitss.size() ||
        m_variable_lens_currentLens + 1 != m_variable_lens_surface_2_radii_x .size() ||
        m_variable_lens_currentLens + 1 != m_variable_lens_surface_2_radii_y .size() ||
        m_variable_lens_currentLens + 1 != m_variable_lens_surface_2_yLimitss.size() ||
        m_variable_lens_currentLens + 1 != m_variable_lens_distances         .size() ||
        m_variable_lens_currentLens + 1 != m_variable_lens_positions         .size() ||
        m_variable_lens_currentLens + 1 != m_variable_lens_materials         .size() ||
        m_variable_lens_currentLens + 1 != m_variable_lens_visibilities      .size() ||
        m_variable_lens_currentLens + 1 != m_variable_lens_colors            .size() ||
        m_variable_lens_currentLens + 1 != m_variable_lens_alphas            .size() ||
        m_variable_lens_currentLens + 1 != m_variable_lens_forceSolids       .size() ||
        m_variable_lens_currentLens + 1 != m_variable_lens_visAttributess    .size() )
        G4Exception( "ConstructionMessenger::get_lens_amount()",
                     "m_variable_lens_currentLens != m_variable_lens_surface_1_radii_x.size()",
                     FatalException,
                     "Lens amount is not equal to the amount of lens properties." );
    return m_variable_lens_currentLens + 1;
}
G4double ConstructionMessenger::get_lens_surface_1_radius_x( G4int t_lens_index ) {
    return m_variable_lens_surface_1_radii_x[ t_lens_index ];
}
G4double ConstructionMessenger::get_lens_surface_1_radius_y( G4int t_lens_index ) {
    return m_variable_lens_surface_1_radii_y[ t_lens_index ];
}
G4double ConstructionMessenger::get_lens_surface_1_yLimits( G4int t_lens_index ) {
    return m_variable_lens_surface_1_yLimitss[ t_lens_index ];
}
G4double ConstructionMessenger::get_lens_surface_2_radius_x( G4int t_lens_index ) {
    return m_variable_lens_surface_2_radii_x[ t_lens_index ];
}
G4double ConstructionMessenger::get_lens_surface_2_radius_y( G4int t_lens_index ) {
    return m_variable_lens_surface_2_radii_y[ t_lens_index ];
}
G4double ConstructionMessenger::get_lens_surface_2_yLimits( G4int t_lens_index ) {
    return m_variable_lens_surface_2_yLimitss[ t_lens_index ];
}
G4double ConstructionMessenger::get_lens_distance( G4int t_lens_index ) {
    return m_variable_lens_distances[ t_lens_index ];
}
G4double ConstructionMessenger::get_lens_position( G4int t_lens_index ) {
    return m_variable_lens_positions[ t_lens_index ];
}
G4String ConstructionMessenger::get_lens_material( G4int t_lens_index ) {
    return m_variable_lens_materials[ t_lens_index ];
}
G4bool ConstructionMessenger::get_lens_visibility( G4int t_lens_index ) {
    return m_variable_lens_visibilities[ t_lens_index ];
}
G4String ConstructionMessenger::get_lens_color( G4int t_lens_index ) {
    return m_variable_lens_colors[ t_lens_index ];
}
G4double ConstructionMessenger::get_lens_alpha( G4int t_lens_index ) {
    return m_variable_lens_alphas[ t_lens_index ];
}
G4bool ConstructionMessenger::get_lens_forceSolid( G4int t_lens_index ) {
    return m_variable_lens_forceSolids[ t_lens_index ];
}
G4VisAttributes* ConstructionMessenger::get_lens_visAttributes( G4int t_lens_index ) {
    return m_variable_lens_visAttributess[ t_lens_index ];
}
G4ThreeVector ConstructionMessenger::get_directionSensitivePhotoDetector_amount() { 
    return m_variable_directionSensitivePhotoDetector_amount;
}
G4int ConstructionMessenger::get_directionSensitivePhotoDetector_amount_x() { 
    G4int temp = m_variable_directionSensitivePhotoDetector_amount.x();
    if( temp == m_variable_directionSensitivePhotoDetector_amount.x() )
        return temp;
    else
        G4Exception( "ConstructionMessenger::get_directionSensitivePhotoDetector_amount_x()",
                     "ConstructionMessenger::get_directionSensitivePhotoDetector_amount_x()",
                     FatalException,
                     "DirectionSensitivePhotoDetector amount x is not an integer." );
        return 0;
}
G4int ConstructionMessenger::get_directionSensitivePhotoDetector_amount_y() { 
    G4int temp = m_variable_directionSensitivePhotoDetector_amount.y();
    if( temp == m_variable_directionSensitivePhotoDetector_amount.y() )
        return temp;
    else
        G4Exception( "ConstructionMessenger::get_directionSensitivePhotoDetector_amount_y()",
                     "ConstructionMessenger::get_directionSensitivePhotoDetector_amount_y()",
                     FatalException,
                     "DirectionSensitivePhotoDetector amount y is not an integer." );
        return 0;
}
G4int ConstructionMessenger::get_directionSensitivePhotoDetector_amount_z() { 
    G4int temp = m_variable_directionSensitivePhotoDetector_amount.z();
    if( temp == m_variable_directionSensitivePhotoDetector_amount.z() )
        return temp;
    else
        G4Exception( "ConstructionMessenger::get_directionSensitivePhotoDetector_amount_z()",
                     "ConstructionMessenger::get_directionSensitivePhotoDetector_amount_z()",
                     FatalException,
                     "DirectionSensitivePhotoDetector amount z is not an integer." );
        return 0;
}
G4bool ConstructionMessenger::get_checkOverlaps() {
    return m_variable_checkOverlaps;
}

void ConstructionMessenger::set_world_size( G4ThreeVector t_variable_world_size ) { 
    m_variable_world_size=t_variable_world_size; 
}
void ConstructionMessenger::set_world_size_x( G4double t_variable_world_size_x ) { 
    m_variable_world_size.setX( t_variable_world_size_x );
}
void ConstructionMessenger::set_world_size_y( G4double t_variable_world_size_y ) { 
    m_variable_world_size.setY( t_variable_world_size_y );
}
void ConstructionMessenger::set_world_size_z( G4double t_variable_world_size_z ) { 
    m_variable_world_size.setZ( t_variable_world_size_z );
}
void ConstructionMessenger::set_world_material( G4String t_variable_world_material ) { 
    m_variable_world_material = t_variable_world_material;
}
void ConstructionMessenger::set_world_visibility( G4bool t_variable_world_visibility ) { 
    m_variable_world_visibility = t_variable_world_visibility;
    set_visAttributes_alpha( t_variable_world_visibility, m_variable_world_visAttributes );
}
void ConstructionMessenger::set_world_color( G4String t_variable_world_color ) { 
    m_variable_world_color = t_variable_world_color;
    set_visAttributes_color( t_variable_world_color, m_variable_world_visAttributes );
}
void ConstructionMessenger::set_world_alpha( G4double t_variable_world_alpha ) { 
    m_variable_world_alpha = t_variable_world_alpha;
    set_visAttributes_alpha( t_variable_world_alpha, m_variable_world_visAttributes );
}
void ConstructionMessenger::set_world_forceSolid( G4bool t_variable_world_forceSolid ) { 
    m_variable_world_forceSolid = t_variable_world_forceSolid;
    set_visAttributes_forceSolid( t_variable_world_forceSolid, m_variable_world_visAttributes );
}
void ConstructionMessenger::set_detector_wall_thickness( G4double t_variable_detector_wall_thickness ) { 
    m_variable_detector_wall_thickness = t_variable_detector_wall_thickness;
}
void ConstructionMessenger::set_detector_wall_material( G4String t_variable_detector_wall_material ) { 
    m_variable_detector_wall_material = t_variable_detector_wall_material;
}
void ConstructionMessenger::set_detector_wall_visibility( G4bool t_variable_detector_wall_visibility ) { 
    m_variable_detector_wall_visibility = t_variable_detector_wall_visibility;
    set_visAttributes_visibility( t_variable_detector_wall_visibility, m_variable_detector_wall_visAttributes );
}
void ConstructionMessenger::set_detector_wall_color( G4String t_variable_detector_wall_color ) { 
    m_variable_detector_wall_color = t_variable_detector_wall_color;
    set_visAttributes_color( t_variable_detector_wall_color, m_variable_detector_wall_visAttributes );
}
void ConstructionMessenger::set_detector_wall_alpha( G4double t_variable_detector_wall_alpha ) { 
    m_variable_detector_wall_alpha = t_variable_detector_wall_alpha;
    set_visAttributes_alpha( t_variable_detector_wall_alpha, m_variable_detector_wall_visAttributes );
}
void ConstructionMessenger::set_detector_wall_forceSolid( G4bool t_variable_detector_wall_forceSolid ) { 
    m_variable_detector_wall_forceSolid = t_variable_detector_wall_forceSolid;
    set_visAttributes_forceSolid( t_variable_detector_wall_forceSolid, m_variable_detector_wall_visAttributes );
}
void ConstructionMessenger::set_detector_medium_material( G4String t_variable_detector_medium_material ) { 
    m_variable_detector_medium_material = t_variable_detector_medium_material;
}
void ConstructionMessenger::set_detector_medium_visibility( G4bool t_variable_detector_medium_visibility ) { 
    m_variable_detector_medium_visibility = t_variable_detector_medium_visibility;
    set_visAttributes_visibility( t_variable_detector_medium_visibility, m_variable_detector_medium_visAttributes );
}
void ConstructionMessenger::set_detector_medium_color( G4String t_variable_detector_medium_color ) { 
    m_variable_detector_medium_color = t_variable_detector_medium_color;
    set_visAttributes_color( t_variable_detector_medium_color, m_variable_detector_medium_visAttributes );
}
void ConstructionMessenger::set_detector_medium_alpha( G4double t_variable_detector_medium_alpha ) { 
    m_variable_detector_medium_alpha = t_variable_detector_medium_alpha;
    set_visAttributes_alpha( t_variable_detector_medium_alpha, m_variable_detector_medium_visAttributes );
}
void ConstructionMessenger::set_detector_medium_forceSolid( G4bool t_variable_detector_medium_forceSolid ) { 
    m_variable_detector_medium_forceSolid = t_variable_detector_medium_forceSolid;
    set_visAttributes_forceSolid( t_variable_detector_medium_forceSolid, m_variable_detector_medium_visAttributes );
}
void ConstructionMessenger::set_calorimeter_size( G4ThreeVector t_variable_calorimeter_size ) { 
    m_variable_calorimeter_size = t_variable_calorimeter_size;
}
void ConstructionMessenger::set_calorimeter_size_width( G4double t_variable_calorimeter_size_width ) { 
    m_variable_calorimeter_size.setX( t_variable_calorimeter_size_width );
}
void ConstructionMessenger::set_calorimeter_size_height( G4double t_variable_calorimeter_size_height ) { 
    m_variable_calorimeter_size.setY( t_variable_calorimeter_size_height );
}
void ConstructionMessenger::set_calorimeter_size_depth( G4double t_variable_calorimeter_size_depth ) { 
    m_variable_calorimeter_size.setZ( t_variable_calorimeter_size_depth );
}
void ConstructionMessenger::set_calorimeter_material( G4String t_variable_calorimeter_material ) { 
    m_variable_calorimeter_material = t_variable_calorimeter_material;
}
void ConstructionMessenger::set_calorimeter_visibility( G4bool t_variable_calorimeter_visibility ) { 
    m_variable_calorimeter_visibility = t_variable_calorimeter_visibility;
    set_visAttributes_visibility( t_variable_calorimeter_visibility, m_variable_calorimeter_visAttributes );
}
void ConstructionMessenger::set_calorimeter_color( G4String t_variable_calorimeter_color ) { 
    m_variable_calorimeter_color = t_variable_calorimeter_color;
    set_visAttributes_color( t_variable_calorimeter_color, m_variable_calorimeter_visAttributes );
}
void ConstructionMessenger::set_calorimeter_alpha( G4double t_variable_calorimeter_alpha ) { 
    m_variable_calorimeter_alpha = t_variable_calorimeter_alpha;
    set_visAttributes_alpha( t_variable_calorimeter_alpha, m_variable_calorimeter_visAttributes );
}
void ConstructionMessenger::set_calorimeter_forceSolid( G4bool t_variable_calorimeter_forceSolid ) { 
    m_variable_calorimeter_forceSolid = t_variable_calorimeter_forceSolid;
    set_visAttributes_forceSolid( t_variable_calorimeter_forceSolid, m_variable_calorimeter_visAttributes );
}
void ConstructionMessenger::set_photoSensor_surface_size( G4ThreeVector t_variable_photoSensor_surface_size ) { 
    m_variable_photoSensor_surface_size = t_variable_photoSensor_surface_size;
}
void ConstructionMessenger::set_photoSensor_surface_size_width( G4double t_variable_photoSensor_surface_size_width ) { 
    m_variable_photoSensor_surface_size.setX( t_variable_photoSensor_surface_size_width );
}
void ConstructionMessenger::set_photoSensor_surface_size_height( G4double t_variable_photoSensor_surface_size_height ) { 
    m_variable_photoSensor_surface_size.setY( t_variable_photoSensor_surface_size_height );
}
void ConstructionMessenger::set_photoSensor_surface_size_depth( G4double t_variable_photoSensor_surface_size_depth ) { 
    m_variable_photoSensor_surface_size.setZ( t_variable_photoSensor_surface_size_depth );
}
void ConstructionMessenger::set_photoSensor_surface_size_widthAndHeight( G4double t_variable_photoSensor_surface_size_widthAndHeight ) { 
    m_variable_photoSensor_surface_size.setX( t_variable_photoSensor_surface_size_widthAndHeight );
    m_variable_photoSensor_surface_size.setY( t_variable_photoSensor_surface_size_widthAndHeight );
}
void ConstructionMessenger::set_photoSensor_surface_material( G4String t_variable_photoSensor_surface_material ) {
    m_variable_photoSensor_surface_material = t_variable_photoSensor_surface_material;
}
void ConstructionMessenger::set_photoSensor_surface_visibility( G4bool t_variable_photoSensor_surface_visibility ) { 
    m_variable_photoSensor_surface_visibility = t_variable_photoSensor_surface_visibility;
    set_visAttributes_visibility( t_variable_photoSensor_surface_visibility, m_variable_photoSensor_surface_visAttributes );
}
void ConstructionMessenger::set_photoSensor_surface_color( G4String t_variable_photoSensor_surface_color ) { 
    m_variable_photoSensor_surface_color = t_variable_photoSensor_surface_color;
    set_visAttributes_color( t_variable_photoSensor_surface_color, m_variable_photoSensor_surface_visAttributes );
}
void ConstructionMessenger::set_photoSensor_surface_alpha( G4double t_variable_photoSensor_surface_alpha ) { 
    m_variable_photoSensor_surface_alpha = t_variable_photoSensor_surface_alpha;
    set_visAttributes_alpha( t_variable_photoSensor_surface_alpha, m_variable_photoSensor_surface_visAttributes );
}
void ConstructionMessenger::set_photoSensor_surface_forceSolid( G4bool t_variable_photoSensor_surface_forceSolid ) { 
    m_variable_photoSensor_surface_forceSolid = t_variable_photoSensor_surface_forceSolid;
    set_visAttributes_forceSolid( t_variable_photoSensor_surface_forceSolid, m_variable_photoSensor_surface_visAttributes );
}
void ConstructionMessenger::set_photoSensor_body_size( G4ThreeVector t_variable_photoSensor_body_size ) {
    m_variable_photoSensor_body_size = t_variable_photoSensor_body_size;
}
void ConstructionMessenger::set_photoSensor_body_size_width( G4double t_variable_photoSensor_body_size_width ) { 
    m_variable_photoSensor_body_size.setX( t_variable_photoSensor_body_size_width );
}
void ConstructionMessenger::set_photoSensor_body_size_height( G4double t_variable_photoSensor_body_size_height ) { 
    m_variable_photoSensor_body_size.setY( t_variable_photoSensor_body_size_height );
}
void ConstructionMessenger::set_photoSensor_body_size_depth( G4double t_variable_photoSensor_body_size_depth ) { 
    m_variable_photoSensor_body_size.setZ( t_variable_photoSensor_body_size_depth );
}
void ConstructionMessenger::set_photoSensor_body_size_widthAndHeight( G4double t_variable_photoSensor_body_size_widthAndHeight ) { 
    m_variable_photoSensor_body_size.setX( t_variable_photoSensor_body_size_widthAndHeight );
    m_variable_photoSensor_body_size.setY( t_variable_photoSensor_body_size_widthAndHeight );
}
void ConstructionMessenger::set_photoSensor_body_material( G4String t_variable_photoSensor_body_material ) {
    m_variable_photoSensor_body_material = t_variable_photoSensor_body_material;
}
void ConstructionMessenger::set_photoSensor_body_visibility( G4bool t_variable_photoSensor_body_visibility ) { 
    m_variable_photoSensor_body_visibility = t_variable_photoSensor_body_visibility;
    set_visAttributes_visibility( t_variable_photoSensor_body_visibility, m_variable_photoSensor_body_visAttributes );
}
void ConstructionMessenger::set_photoSensor_body_color( G4String t_variable_photoSensor_body_color ) { 
    m_variable_photoSensor_body_color = t_variable_photoSensor_body_color;
    set_visAttributes_color( t_variable_photoSensor_body_color, m_variable_photoSensor_body_visAttributes );
}
void ConstructionMessenger::set_photoSensor_body_alpha( G4double t_variable_photoSensor_body_alpha ) { 
    m_variable_photoSensor_body_alpha = t_variable_photoSensor_body_alpha;
    set_visAttributes_alpha( t_variable_photoSensor_body_alpha, m_variable_photoSensor_body_visAttributes );
}
void ConstructionMessenger::set_photoSensor_body_forceSolid( G4bool t_variable_photoSensor_body_forceSolid ) { 
    m_variable_photoSensor_body_forceSolid = t_variable_photoSensor_body_forceSolid;
    set_visAttributes_forceSolid( t_variable_photoSensor_body_forceSolid, m_variable_photoSensor_body_visAttributes );
}
void ConstructionMessenger::set_lens_incramentCurrentLens() {
    if( m_variable_lens_surface_1_radii_x.size() != 0 )
        m_variable_lens_currentLens++;
    m_variable_lens_surface_1_radii_x .push_back( 0 );
    m_variable_lens_surface_1_radii_y .push_back( 0 );
    m_variable_lens_surface_1_yLimitss.push_back( 0 );
    m_variable_lens_surface_2_radii_x .push_back( 0 );
    m_variable_lens_surface_2_radii_y .push_back( 0 );
    m_variable_lens_surface_2_yLimitss.push_back( 0 );
    m_variable_lens_distances         .push_back( 0 );
    m_variable_lens_positions         .push_back( 0 );
    m_variable_lens_materials         .push_back( "" );
    m_variable_lens_visibilities      .push_back( true );
    m_variable_lens_colors            .push_back( "" );
    m_variable_lens_alphas            .push_back( 0 );
    m_variable_lens_forceSolids       .push_back( false );
    m_variable_lens_visAttributess    .push_back( new G4VisAttributes() );
}
void ConstructionMessenger::set_lens_surface_1_radius_x( G4int t_variable_lens_currentLens, G4double t_variable_lens_surface_1_radius_x ) {
    m_variable_lens_surface_1_radii_x[ t_variable_lens_currentLens ] = t_variable_lens_surface_1_radius_x;
}
void ConstructionMessenger::set_lens_surface_1_radius_y( G4int t_variable_lens_currentLens, G4double t_variable_lens_surface_1_radius_y ) {
    m_variable_lens_surface_1_radii_y[ t_variable_lens_currentLens ] = t_variable_lens_surface_1_radius_y;
}
void ConstructionMessenger::set_lens_surface_1_yLimits( G4int t_variable_lens_currentLens, G4double t_variable_lens_surface_1_yLimits ) {
    m_variable_lens_surface_1_yLimitss[ t_variable_lens_currentLens ] = t_variable_lens_surface_1_yLimits;
}
void ConstructionMessenger::set_lens_surface_2_radius_x( G4int t_variable_lens_currentLens, G4double t_variable_lens_surface_2_radius_x ) {
    m_variable_lens_surface_2_radii_x[ t_variable_lens_currentLens ] = t_variable_lens_surface_2_radius_x;
}
void ConstructionMessenger::set_lens_surface_2_radius_y( G4int t_variable_lens_currentLens, G4double t_variable_lens_surface_2_radius_y ) {
    m_variable_lens_surface_2_radii_y[ t_variable_lens_currentLens ] = t_variable_lens_surface_2_radius_y;
}
void ConstructionMessenger::set_lens_surface_2_yLimits( G4int t_variable_lens_currentLens, G4double t_variable_lens_surface_2_yLimits ) {
    m_variable_lens_surface_2_yLimitss[ t_variable_lens_currentLens ] = t_variable_lens_surface_2_yLimits;
}
void ConstructionMessenger::set_lens_distance( G4int t_variable_lens_currentLens, G4double t_variable_lens_distance ) {
    m_variable_lens_distances[ t_variable_lens_currentLens ] = t_variable_lens_distance;
}
void ConstructionMessenger::set_lens_position( G4int t_variable_lens_currentLens, G4double t_variable_lens_position ) {
    m_variable_lens_positions[ t_variable_lens_currentLens ] = t_variable_lens_position;
}
void ConstructionMessenger::set_lens_material( G4int t_variable_lens_currentLens, G4String t_variable_lens_material ) {
    m_variable_lens_materials[ t_variable_lens_currentLens ] = t_variable_lens_material;
}
void ConstructionMessenger::set_lens_visibility( G4int t_variable_lens_currentLens, G4bool t_variable_lens_visibility ) {
    m_variable_lens_visibilities[ t_variable_lens_currentLens ] = t_variable_lens_visibility;
    set_visAttributes_visibility( t_variable_lens_visibility, m_variable_lens_visAttributess[ t_variable_lens_currentLens ] );
}
void ConstructionMessenger::set_lens_color( G4int t_variable_lens_currentLens, G4String t_variable_lens_color ) {
    m_variable_lens_colors[ t_variable_lens_currentLens ] = t_variable_lens_color;
    set_visAttributes_color( t_variable_lens_color, m_variable_lens_visAttributess[ t_variable_lens_currentLens ] );
}
void ConstructionMessenger::set_lens_alpha( G4int t_variable_lens_currentLens, G4double t_variable_lens_alpha ) {
    m_variable_lens_alphas[ t_variable_lens_currentLens ] = t_variable_lens_alpha;
    set_visAttributes_alpha( t_variable_lens_alpha, m_variable_lens_visAttributess[ t_variable_lens_currentLens ] );
}
void ConstructionMessenger::set_lens_forceSolid( G4int t_variable_lens_currentLens, G4bool t_variable_lens_forceSolid ) {
    m_variable_lens_forceSolids[ t_variable_lens_currentLens ] = t_variable_lens_forceSolid;
    set_visAttributes_forceSolid( t_variable_lens_forceSolid, m_variable_lens_visAttributess[ t_variable_lens_currentLens ] );
}
void ConstructionMessenger::set_directionSensitivePhotoDetector_amount( G4ThreeVector t_variable_directionSensitivePhotoDetector_amount ) { 
    m_variable_directionSensitivePhotoDetector_amount = t_variable_directionSensitivePhotoDetector_amount;
}
void ConstructionMessenger::set_directionSensitivePhotoDetector_amount_x( G4double t_variable_directionSensitivePhotoDetector_amount_x ) {
    m_variable_directionSensitivePhotoDetector_amount.setX( t_variable_directionSensitivePhotoDetector_amount_x );
}
void ConstructionMessenger::set_directionSensitivePhotoDetector_amount_y( G4double t_variable_directionSensitivePhotoDetector_amount_y ) {
    m_variable_directionSensitivePhotoDetector_amount.setY( t_variable_directionSensitivePhotoDetector_amount_y );
}
void ConstructionMessenger::set_directionSensitivePhotoDetector_amount_z( G4double t_variable_directionSensitivePhotoDetector_amount_z ) {
    m_variable_directionSensitivePhotoDetector_amount.setZ( t_variable_directionSensitivePhotoDetector_amount_z );
}
void ConstructionMessenger::set_checkOverlaps( G4bool t_variable_checkOverlaps ) {
    m_variable_checkOverlaps = t_variable_checkOverlaps;
}
void ConstructionMessenger::set_visAttributes_visibility( G4bool t_variable_visibility, G4VisAttributes*& t_variable_visAttributes ) {
    if( !t_variable_visAttributes )
        t_variable_visAttributes = new G4VisAttributes( t_variable_visibility );
    else
        t_variable_visAttributes->SetVisibility( t_variable_visibility );
}
void ConstructionMessenger::set_visAttributes_color( G4String t_variable_color, G4VisAttributes*& t_variable_visAttributes ) {
    G4Colour temp;
    if( !G4Colour::GetColour( t_variable_color, temp ) )
        G4Exception( "ConstructionMessenger::set_visAttributes_color", "InvalidSetup", FatalException, "Invalid color name." );
    else if( !t_variable_visAttributes )
        t_variable_visAttributes = new G4VisAttributes( temp );
    else
        t_variable_visAttributes->SetColor( temp );
}
void ConstructionMessenger::set_visAttributes_alpha( G4double t_variable_alpha, G4VisAttributes*& t_variable_visAttributes ) {
    if( !t_variable_visAttributes )
        t_variable_visAttributes = new G4VisAttributes();
    
    t_variable_visAttributes->SetColour( t_variable_visAttributes->GetColour().GetRed  (), 
                                         t_variable_visAttributes->GetColour().GetGreen(), 
                                         t_variable_visAttributes->GetColour().GetBlue (), 
                                         t_variable_alpha                                 );
}
void ConstructionMessenger::set_visAttributes_forceSolid( G4bool t_variable_forceSolid, G4VisAttributes*& t_variable_visAttributes ) {
    if( !t_variable_visAttributes )
        t_variable_visAttributes = new G4VisAttributes();
    
    t_variable_visAttributes->SetForceSolid( t_variable_forceSolid );
}