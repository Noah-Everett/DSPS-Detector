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
    m_command_world_medium_material        = new G4UIcmdWithAString       ( "/geometry/world/medium/material"       , this );
    m_command_detector_wall_material       = new G4UIcmdWithAString       ( "/geometry/detector/wall/material"      , this );
    m_command_detector_medium_material     = new G4UIcmdWithAString       ( "/geometry/detector/medium/material"    , this );
    m_command_calorimeter_material         = new G4UIcmdWithAString       ( "/geometry/calorimeter/material"        , this );
    m_command_lens_material                = new G4UIcmdWithAString       ( "/geometry/lens/material"               , this );
    m_command_photoSensor_surface_material = new G4UIcmdWithAString       ( "/geometry/photoSensor/surface/material", this );
    m_command_world_size                   = new G4UIcmdWith3VectorAndUnit( "/geometry/world/size"                  , this );
    m_command_detector_wall_size           = new G4UIcmdWith3VectorAndUnit( "/geometry/detector/wall/size"          , this );
    m_command_detector_wall_thickness      = new G4UIcmdWithADoubleAndUnit( "/geometry/detector/wall/thickness"     , this );
    m_command_detector_calorimeter_size    = new G4UIcmdWith3VectorAndUnit( "/geometry/calorimeter/size"            , this );
    m_command_lens_parameterFilePath       = new G4UIcmdWithAString       ( "/geometry/lens/parameterFilePath"      , this );

    m_command_world_medium_material       ->SetDefaultValue( "G4Water"  );
    m_command_detector_wall_material      ->SetDefaultValue( "G4Water"  );
    m_command_detector_medium_material    ->SetDefaultValue( "G4Water"  );
    m_command_calorimeter_material        ->SetDefaultValue( "G4Water"  );
    m_command_lens_material               ->SetDefaultValue( "G4Water"  );
    m_command_photoSensor_surface_material->SetDefaultValue( "G4Water"  );
    m_command_world_size                  ->SetDefaultValue( G4ThreeVector( 100.0 * cm, 100.0 * cm, 100.0 * cm ) );
    m_command_detector_wall_size          ->SetDefaultValue( G4ThreeVector( 100.0 * cm, 100.0 * cm, 100.0 * cm ) );
    m_command_detector_wall_thickness     ->SetDefaultValue( 1.0   * cm );
    m_command_detector_calorimeter_size   ->SetDefaultValue( G4ThreeVector( 100.0 * cm, 100.0 * cm, 100.0 * cm ) );
    m_command_lens_parameterFilePath      ->SetDefaultValue( ""         );
}

ConstructionMessenger::~ConstructionMessenger() {
    if( m_command_world_medium_material        ) delete m_command_world_medium_material       ;
    if( m_command_detector_wall_material       ) delete m_command_detector_wall_material      ;
    if( m_command_detector_medium_material     ) delete m_command_detector_medium_material    ;
    if( m_command_calorimeter_material         ) delete m_command_calorimeter_material        ;
    if( m_command_lens_material                ) delete m_command_lens_material               ;
    if( m_command_photoSensor_surface_material ) delete m_command_photoSensor_surface_material;
    if( m_command_world_size                   ) delete m_command_world_size                  ;
    if( m_command_detector_wall_size           ) delete m_command_detector_wall_size          ;
    if( m_command_detector_wall_thickness      ) delete m_command_detector_wall_thickness     ;
    if( m_command_detector_calorimeter_size    ) delete m_command_detector_calorimeter_size   ;
    if( m_command_lens_parameterFilePath       ) delete m_command_lens_parameterFilePath      ;
}

void ConstructionMessenger::SetNewValue( G4UIcommand* t_command, G4String t_newValue ) {
    if( t_command == m_command_world_medium_material ) { 
        set_world_medium_material( t_newValue );
        G4cout << "Setting `world_medium_material' to "
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_wall_material ) { 
        set_detector_wall_material( t_newValue );
        G4cout << "Setting `detector_wall_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_medium_material ) { 
        set_detector_medium_material( t_newValue );
        G4cout << "Setting `detector_medium_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_calorimeter_material ) { 
        set_calorimeter_material( t_newValue );
        G4cout << "Setting `calorimeter_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_material ) { 
        set_lens_material( t_newValue );
        G4cout << "Setting `lens_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_photoSensor_surface_material ) { 
        set_photoSensor_surface_material( t_newValue );
        G4cout << "Setting `photoSensor_surface_material' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_world_size ) { 
        set_world_size( m_command_world_size->GetNew3VectorValue( t_newValue ) );
        G4cout << "Setting `world_size' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_wall_size ) { 
        set_detector_wall_size( m_command_detector_wall_size->GetNew3VectorValue( t_newValue ) );
        G4cout << "Setting `detector_wall_size' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_wall_thickness ) { 
        set_detector_wall_thickness( m_command_detector_wall_thickness->GetNewDoubleValue( t_newValue ) );
        G4cout << "Setting `detector_wall_thickness' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_detector_calorimeter_size ) { 
        set_calorimeter_size( m_command_detector_calorimeter_size->GetNew3VectorValue( t_newValue ) );
        G4cout << "Setting `detector_calorimeter_size' to " 
               << t_newValue << G4endl;
    } else if( t_command == m_command_lens_parameterFilePath ) { 
        set_lens_parameterFilePath( t_newValue );
        G4cout << "Setting `lens_parameterFilePath' to " 
               << t_newValue << G4endl;
    } else {
        G4cerr << "ERROR: ConstructionMessenger::SetNewValue: Unknown command" << G4endl;
    }
}