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

ConstructionMessenger::ConstructionMessenger() {
    m_command_world_size                             = new G4UIcmdWith3VectorAndUnit( "/geometry/world/size"                            , this );
    m_command_world_material                         = new G4UIcmdWithAString       ( "/geometry/world/material"                        , this );
    m_command_detector_wall_thickness                = new G4UIcmdWithADoubleAndUnit( "/geometry/detector/wall/thickness"               , this );
    m_command_detector_wall_material                 = new G4UIcmdWithAString       ( "/geometry/detector/wall/material"                , this );
    m_command_detector_medium_material               = new G4UIcmdWithAString       ( "/geometry/detector/medium/material"              , this );
    m_command_calorimeter_size                       = new G4UIcmdWith3VectorAndUnit( "/geometry/calorimeter/size"                      , this );
    m_command_calorimeter_material                   = new G4UIcmdWithAString       ( "/geometry/calorimeter/material"                  , this );
    m_command_photoSensor_surface_size               = new G4UIcmdWith3VectorAndUnit( "/geometry/photoSensor/surface/size"              , this );
    m_command_photoSensor_surface_material           = new G4UIcmdWithAString       ( "/geometry/photoSensor/surface/material"          , this );
    m_command_photoSensor_body_size                  = new G4UIcmdWith3VectorAndUnit( "/geometry/photoSensor/body/size"                 , this );
    m_command_photoSensor_body_material              = new G4UIcmdWithAString       ( "/geometry/photoSensor/body/material"             , this );
    m_command_lens_parameterFilePath                 = new G4UIcmdWithAString       ( "/geometry/lens/parameterFilePath"                , this );
    m_command_directionSensitivePhotoDetector_amount = new G4UIcmdWith3Vector       ( "/geometry/directionSensitivePhotoDetector/amount", this );
    m_command_checkOverlaps                          = new G4UIcmdWithABool         ( "/geometry/checkOverlaps"                         , this );
}

ConstructionMessenger::~ConstructionMessenger() {
    if( m_command_world_size                             ) delete m_command_world_size                            ;
    if( m_command_world_material                         ) delete m_command_world_material                        ;
    if( m_command_detector_wall_thickness                ) delete m_command_detector_wall_thickness               ;
    if( m_command_detector_wall_material                 ) delete m_command_detector_wall_material                ;
    if( m_command_detector_medium_material               ) delete m_command_detector_medium_material              ;
    if( m_command_calorimeter_size                       ) delete m_command_calorimeter_size                      ;
    if( m_command_calorimeter_material                   ) delete m_command_calorimeter_material                  ;
    if( m_command_photoSensor_surface_size               ) delete m_command_photoSensor_surface_size              ;
    if( m_command_photoSensor_surface_material           ) delete m_command_photoSensor_surface_material          ;
    if( m_command_photoSensor_body_size                  ) delete m_command_photoSensor_body_size                 ;
    if( m_command_photoSensor_body_material              ) delete m_command_photoSensor_body_material             ;
    if( m_command_lens_parameterFilePath                 ) delete m_command_lens_parameterFilePath                ;
    if( m_command_directionSensitivePhotoDetector_amount ) delete m_command_directionSensitivePhotoDetector_amount;
    if( m_command_checkOverlaps                          ) delete m_command_checkOverlaps                         ;
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
    } else if( t_command == m_command_detector_wall_thickness ) { 
        set_detector_wall_thickness( m_command_detector_wall_thickness->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `detector_wall_thickness' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_wall_material ) { 
        set_detector_wall_material( t_newValue );
        G4cout << "Setting `detector_wall_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_medium_material ) { 
        set_detector_medium_material( t_newValue );
        G4cout << "Setting `detector_medium_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_size ) { 
        set_calorimeter_size( m_command_calorimeter_size->GetNew3VectorValue( t_newValue ) );
        G4cout << "Setting `calorimeter_size' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_material ) { 
        set_calorimeter_material( t_newValue );
        G4cout << "Setting `calorimeter_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_surface_size ) {
        set_photoSensor_surface_size( m_command_photoSensor_surface_size->GetNew3VectorValue( t_newValue ) );
        G4cout << "Setting `photoSensor_surface_size' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_surface_material ) { 
        set_photoSensor_surface_material( t_newValue );
        G4cout << "Setting `photoSensor_surface_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_body_size ) {
        set_photoSensor_body_size( m_command_photoSensor_body_size->GetNew3VectorValue( t_newValue ) );
        G4cout << "Setting `photoSensor_body_size' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_body_material ) { 
        set_photoSensor_body_material( t_newValue );
        G4cout << "Setting `photoSensor_body_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_parameterFilePath ) { 
        set_lens_parameterFilePath( t_newValue );
        G4cout << "Setting `lens_parameterFilePath' to " 
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
    G4cout << "Attempting to use the following parameters:" << G4endl
            << "  world_x----------------------------------: " << get_world_size_x                            () << G4endl
            << "  world_y----------------------------------: " << get_world_size_y                            () << G4endl
            << "  world_z----------------------------------: " << get_world_size_z                            () << G4endl
            << "  world_material---------------------------: " << get_world_material                          () << G4endl
            << "  detector_wall_thickness------------------: " << get_detector_wall_thickness                 () << G4endl
            << "  detector_wall_material-------------------: " << get_detector_wall_material                  () << G4endl
            << "  detector_medium_material-----------------: " << get_detector_medium_material                () << G4endl
            << "  calorimeter_size_width-------------------: " << get_calorimeter_size_width                  () << G4endl
            << "  calorimeter_size_height------------------: " << get_calorimeter_size_height                 () << G4endl
            << "  calorimeter_size_depth-------------------: " << get_calorimeter_size_depth                  () << G4endl
            << "  calorimeter_material---------------------: " << get_calorimeter_material                    () << G4endl
            << "  photoSensor_surface_size_width-----------: " << get_photoSensor_surface_size_width          () << G4endl
            << "  photoSensor_surface_size_height----------: " << get_photoSensor_surface_size_height         () << G4endl
            << "  photoSensor_surface_size_depth-----------: " << get_photoSensor_surface_size_depth          () << G4endl
            << "  photoSensor_surface_material-------------: " << get_photoSensor_surface_material            () << G4endl
            << "  photoSensor_body_size_width--------------: " << get_photoSensor_body_size_width             () << G4endl
            << "  photoSensor_body_size_height-------------: " << get_photoSensor_body_size_height            () << G4endl
            << "  photoSensor_body_size_depth--------------: " << get_photoSensor_body_size_depth             () << G4endl
            << "  photoSensor_body_material----------------: " << get_photoSensor_body_material               () << G4endl
            << "  lens_parameterFilePath-------------------: " << get_lens_parameterFilePath                  () << G4endl
            << "  directionSensitivePhotoDetector_amount_x-: " << get_directionSensitivePhotoDetector_amount_x() << G4endl
            << "  directionSensitivePhotoDetector_amount_y-: " << get_directionSensitivePhotoDetector_amount_y() << G4endl
            << "  directionSensitivePhotoDetector_amount_z-: " << get_directionSensitivePhotoDetector_amount_z() << G4endl
            << "  checkOverlaps----------------------------: " << get_checkOverlaps                           () << G4endl;
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

G4double ConstructionMessenger::get_detector_wall_thickness() { 
    return m_variable_detector_wall_thickness;
}

G4String ConstructionMessenger::get_detector_wall_material() { 
    return m_variable_detector_wall_material;
}

G4String ConstructionMessenger::get_detector_medium_material() { 
    return m_variable_detector_medium_material;
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

G4String ConstructionMessenger::get_lens_parameterFilePath() { 
    return m_variable_lens_parameterFilePath;
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

void ConstructionMessenger::set_world_size                              ( G4ThreeVector t_variable_world_size                               ){ m_variable_world_size                                 = t_variable_world_size                                ; }
void ConstructionMessenger::set_world_size_x                            ( G4double      t_variable_world_size_x                             ){ m_variable_world_size.setX                            ( t_variable_world_size_x                             ); }
void ConstructionMessenger::set_world_size_y                            ( G4double      t_variable_world_size_y                             ){ m_variable_world_size.setY                            ( t_variable_world_size_y                             ); }
void ConstructionMessenger::set_world_size_z                            ( G4double      t_variable_world_size_z                             ){ m_variable_world_size.setZ                            ( t_variable_world_size_z                             ); }
void ConstructionMessenger::set_world_material                          ( G4String      t_variable_world_material                           ){ m_variable_world_material                             = t_variable_world_material                            ; }
void ConstructionMessenger::set_detector_wall_thickness                 ( G4double      t_variable_detector_wall_thickness                  ){ m_variable_detector_wall_thickness                    = t_variable_detector_wall_thickness                   ; }
void ConstructionMessenger::set_detector_wall_material                  ( G4String      t_variable_detector_wall_material                   ){ m_variable_detector_wall_material                     = t_variable_detector_wall_material                    ; }
void ConstructionMessenger::set_detector_medium_material                ( G4String      t_variable_detector_medium_material                 ){ m_variable_detector_medium_material                   = t_variable_detector_medium_material                  ; }
void ConstructionMessenger::set_calorimeter_size                        ( G4ThreeVector t_variable_calorimeter_size                         ){ m_variable_calorimeter_size                           = t_variable_calorimeter_size                          ; }
void ConstructionMessenger::set_calorimeter_size_width                  ( G4double      t_variable_calorimeter_size_width                   ){ m_variable_calorimeter_size.setX                      ( t_variable_calorimeter_size_width                   ); }
void ConstructionMessenger::set_calorimeter_size_height                 ( G4double      t_variable_calorimeter_size_height                  ){ m_variable_calorimeter_size.setY                      ( t_variable_calorimeter_size_height                  ); }
void ConstructionMessenger::set_calorimeter_size_depth                  ( G4double      t_variable_calorimeter_size_depth                   ){ m_variable_calorimeter_size.setZ                      ( t_variable_calorimeter_size_depth                   ); }
void ConstructionMessenger::set_calorimeter_material                    ( G4String      t_variable_calorimeter_material                     ){ m_variable_calorimeter_material                       = t_variable_calorimeter_material                      ; }
void ConstructionMessenger::set_photoSensor_surface_size                ( G4ThreeVector t_variable_photoSensor_surface_size                 ){ m_variable_photoSensor_surface_size                   = t_variable_photoSensor_surface_size                  ; }
void ConstructionMessenger::set_photoSensor_surface_size_width          ( G4double      t_variable_photoSensor_surface_size_width           ){ m_variable_photoSensor_surface_size.setX              ( t_variable_photoSensor_surface_size_width           ); }
void ConstructionMessenger::set_photoSensor_surface_size_height         ( G4double      t_variable_photoSensor_surface_size_height          ){ m_variable_photoSensor_surface_size.setY              ( t_variable_photoSensor_surface_size_height          ); }
void ConstructionMessenger::set_photoSensor_surface_size_depth          ( G4double      t_variable_photoSensor_surface_size_depth           ){ m_variable_photoSensor_surface_size.setZ              ( t_variable_photoSensor_surface_size_depth           ); }
void ConstructionMessenger::set_photoSensor_surface_size_widthAndHeight ( G4double      t_variable_photoSensor_surface_size_widthAndHeight  ){ m_variable_photoSensor_surface_size.setX              ( t_variable_photoSensor_surface_size_widthAndHeight  );
                                                                                                                                               m_variable_photoSensor_surface_size.setY              ( t_variable_photoSensor_surface_size_widthAndHeight  ); }
void ConstructionMessenger::set_photoSensor_surface_material            ( G4String      t_variable_photoSensor_surface_material             ){ m_variable_photoSensor_surface_material               = t_variable_photoSensor_surface_material              ; }
void ConstructionMessenger::set_photoSensor_body_size                   ( G4ThreeVector t_variable_photoSensor_body_size                    ){ m_variable_photoSensor_body_size                      = t_variable_photoSensor_body_size                     ; }
void ConstructionMessenger::set_photoSensor_body_size_width             ( G4double      t_variable_photoSensor_body_size_width              ){ m_variable_photoSensor_body_size.setX                 ( t_variable_photoSensor_body_size_width              ); }
void ConstructionMessenger::set_photoSensor_body_size_height            ( G4double      t_variable_photoSensor_body_size_height             ){ m_variable_photoSensor_body_size.setY                 ( t_variable_photoSensor_body_size_height             ); }
void ConstructionMessenger::set_photoSensor_body_size_depth             ( G4double      t_variable_photoSensor_body_size_depth              ){ m_variable_photoSensor_body_size.setZ                 ( t_variable_photoSensor_body_size_depth              ); }
void ConstructionMessenger::set_photoSensor_body_size_widthAndHeight    ( G4double      t_variable_photoSensor_body_size_widthAndHeight     ){ m_variable_photoSensor_body_size.setX                 ( t_variable_photoSensor_body_size_widthAndHeight     );
                                                                                                                                               m_variable_photoSensor_body_size.setY                 ( t_variable_photoSensor_body_size_widthAndHeight     ); }
void ConstructionMessenger::set_photoSensor_body_material               ( G4String      t_variable_photoSensor_body_material                ){ m_variable_photoSensor_body_material                  = t_variable_photoSensor_body_material                 ; }
void ConstructionMessenger::set_lens_parameterFilePath                  ( G4String      t_variable_lens_parameterFilePath                   ){ m_variable_lens_parameterFilePath                     = t_variable_lens_parameterFilePath                    ; }
void ConstructionMessenger::set_directionSensitivePhotoDetector_amount  ( G4ThreeVector t_variable_directionSensitivePhotoDetector_amount   ){ m_variable_directionSensitivePhotoDetector_amount     = t_variable_directionSensitivePhotoDetector_amount    ; }
void ConstructionMessenger::set_directionSensitivePhotoDetector_amount_x( G4double      t_variable_directionSensitivePhotoDetector_amount_x ){ m_variable_directionSensitivePhotoDetector_amount.setX( t_variable_directionSensitivePhotoDetector_amount_x ); }
void ConstructionMessenger::set_directionSensitivePhotoDetector_amount_y( G4double      t_variable_directionSensitivePhotoDetector_amount_y ){ m_variable_directionSensitivePhotoDetector_amount.setY( t_variable_directionSensitivePhotoDetector_amount_y ); }
void ConstructionMessenger::set_directionSensitivePhotoDetector_amount_z( G4double      t_variable_directionSensitivePhotoDetector_amount_z ){ m_variable_directionSensitivePhotoDetector_amount.setZ( t_variable_directionSensitivePhotoDetector_amount_z ); }
void ConstructionMessenger::set_checkOverlaps                           ( G4bool        t_variable_checkOverlaps                            ){ m_variable_checkOverlaps                              = t_variable_checkOverlaps                             ; }