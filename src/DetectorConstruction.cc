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

#include "DetectorConstruction.hh"

#include<string>
using std::to_string;
using std::string;

DetectorConstruction::DetectorConstruction( G4bool t_make_SDandField ) :
    m_make_SDandField( t_make_SDandField ) {
}
    
DetectorConstruction::~DetectorConstruction() {
    if( m_world           ) delete m_world              ;
    if( m_detector_wall   ) delete m_detector_wall      ;
    if( m_detector_medium ) delete m_detector_medium    ;
    if( m_GDMLParser      ) delete m_GDMLParser         ;

    for( auto& calorimeter : m_calorimeters_full )
        if( calorimeter ) 
            delete calorimeter;
    for( auto& calorimeter : m_calorimeters_middle )
        if( calorimeter ) 
            delete calorimeter;
    for( auto& directionSensitivePhotoDetector : m_directionSensitivePhotoDetectors )
        if( directionSensitivePhotoDetector ) 
            delete directionSensitivePhotoDetector;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    m_materials            ->print_materials ();
    m_constructionMessenger->print_parameters();

    m_checkOverlaps = m_constructionMessenger->get_checkOverlaps();

    make_world                          ();
    make_detector                       ();
    
    m_world_physicalVolume = 
    m_world          ->place( nullptr, G4ThreeVector(0,0,0), nullptr                              );
    m_detector_wall  ->place( nullptr, G4ThreeVector(0,0,0), m_world        ->get_logicalVolume() );
    m_detector_medium->place( nullptr, G4ThreeVector(0,0,0), m_detector_wall->get_logicalVolume() );

    G4int countIndex { 0 };
    place_surface(  m_axis_x, countIndex++ );
    place_surface( -m_axis_x, countIndex++ );
    place_surface(  m_axis_y, countIndex++ );
    place_surface( -m_axis_y, countIndex++ );
    place_surface(  m_axis_z, countIndex++ );
    place_surface( -m_axis_z, countIndex++ );

    return m_world_physicalVolume;
}

void DetectorConstruction::make_world() {
    G4double world_size_x = m_constructionMessenger->get_world_size_x() / 2;
    G4double world_size_y = m_constructionMessenger->get_world_size_y() / 2;
    G4double world_size_z = m_constructionMessenger->get_world_size_z() / 2;

    m_world->set_material( m_constructionMessenger->get_world_material() );
    m_world->set_solid( new G4Box( "world", world_size_x, world_size_y, world_size_z ) );
    m_world->set_visAttributes( m_constructionMessenger->get_world_visAttributes() );
    m_world->make_logicalVolume();
}

void DetectorConstruction::make_detector() {
    G4double detector_wall_thickness                  = m_constructionMessenger->get_detector_wall_thickness                 ();
    G4double calorimeter_size_width                   = m_constructionMessenger->get_calorimeter_size_width                  ();
    G4double calorimeter_size_height                  = m_constructionMessenger->get_calorimeter_size_height                 ();
    G4double calorimeter_size_depth                   = m_constructionMessenger->get_calorimeter_size_depth                  ();
    G4double photoSensor_surface_size_width           = m_constructionMessenger->get_photoSensor_surface_size_width          ();
    G4double photoSensor_surface_size_height          = m_constructionMessenger->get_photoSensor_surface_size_height         ();
    G4double photoSensor_surface_size_depth           = m_constructionMessenger->get_photoSensor_surface_size_depth          ();
    G4double photoSensor_body_size_width              = m_constructionMessenger->get_photoSensor_body_size_width             ();
    G4double photoSensor_body_size_height             = m_constructionMessenger->get_photoSensor_body_size_height            ();
    G4double photoSensor_body_size_depth              = m_constructionMessenger->get_photoSensor_body_size_depth             ();
    G4int    directionSensitivePhotoDetector_amount_x = m_constructionMessenger->get_directionSensitivePhotoDetector_amount_x();
    G4int    directionSensitivePhotoDetector_amount_y = m_constructionMessenger->get_directionSensitivePhotoDetector_amount_y();
    G4int    directionSensitivePhotoDetector_amount_z = m_constructionMessenger->get_directionSensitivePhotoDetector_amount_z();

    if( calorimeter_size_width != photoSensor_surface_size_width ) {
        G4Exception( "DetectorConstruction::make_detector()", "Error", FatalException, 
                     "calorimeter_size_width != photoSensor_surface_size_width" );
    } else if ( photoSensor_surface_size_width != photoSensor_body_size_width ) {
        G4Exception( "DetectorConstruction::make_detector()", "Error", FatalException, 
                     "photoSensor_surface_size_width != photoSensor_body_size_width" );
    }

    G4double detector_wall_size_x = directionSensitivePhotoDetector_amount_x * photoSensor_surface_size_width
                                  + ( directionSensitivePhotoDetector_amount_x + 1 ) * calorimeter_size_height
                                  + 2 * detector_wall_thickness
                                  + 2 * calorimeter_size_depth;
    G4double detector_wall_size_y = directionSensitivePhotoDetector_amount_y * photoSensor_surface_size_width
                                  + ( directionSensitivePhotoDetector_amount_y + 1 ) * calorimeter_size_height
                                  + 2 * detector_wall_thickness
                                  + 2 * calorimeter_size_depth;
    G4double detector_wall_size_z = directionSensitivePhotoDetector_amount_z * photoSensor_surface_size_width
                                  + ( directionSensitivePhotoDetector_amount_z + 1 ) * calorimeter_size_height
                                  + 2 * detector_wall_thickness
                                  + 2 * calorimeter_size_depth;
    m_detector_wall->set_material( m_constructionMessenger->get_detector_wall_material() );
    m_detector_wall->set_solid( new G4Box( "detector_wall", detector_wall_size_x / 2, detector_wall_size_y / 2, detector_wall_size_z / 2 ) );
    m_detector_wall->set_visAttributes( m_constructionMessenger->get_detector_wall_visAttributes() );
    m_detector_wall->make_logicalVolume();

    G4double detector_medium_size_x = detector_wall_size_x - 2 * detector_wall_thickness ;
    G4double detector_medium_size_y = detector_wall_size_y - 2 * detector_wall_thickness ;
    G4double detector_medium_size_z = detector_wall_size_z - 2 * detector_wall_thickness ;
    m_detector_medium->set_material( m_constructionMessenger->get_detector_medium_material() );
    m_detector_medium->set_solid( new G4Box( "detector_medium", detector_medium_size_x / 2, detector_medium_size_y / 2, detector_medium_size_z / 2 ) );
    m_detector_medium->set_visAttributes( m_constructionMessenger->get_detector_medium_visAttributes() );
    m_detector_medium->make_logicalVolume();
}

Calorimeter* DetectorConstruction::make_calorimeter_full( const G4String& t_name, const G4String& t_index ) {
    Calorimeter* calorimeter = new Calorimeter( t_name, t_index, m_constructionMessenger->get_calorimeter_size() );

    m_calorimeters_full.push_back( calorimeter );
    return calorimeter;
}

Calorimeter* DetectorConstruction::make_calorimeter_middle( const G4String& t_name, const G4String& t_index ) {
    Calorimeter* calorimeter = new Calorimeter( t_name, t_index, G4ThreeVector( Calorimeter::get_height(), Calorimeter::get_height(), Calorimeter::get_depth() ) );

    m_calorimeters_middle.push_back( calorimeter );
    return calorimeter;
}

DirectionSensitivePhotoDetector* DetectorConstruction::make_directionSensitivePhotoDetector( const G4String& t_name, const G4String& t_index ) {
    DirectionSensitivePhotoDetector* directionSensitivePhotoDetector = new DirectionSensitivePhotoDetector( t_name, t_index );

    m_directionSensitivePhotoDetectors.push_back( directionSensitivePhotoDetector );
    return directionSensitivePhotoDetector;
}

void DetectorConstruction::place_surface( G4ThreeVector t_axis_normal, G4int t_countIndex ) {
    G4int    count{ 0  };
    G4String name { "" };
    G4String index{ "" };
         if( t_axis_normal ==  m_axis_x ) index = "+x";
    else if( t_axis_normal == -m_axis_x ) index = "-x";
    else if( t_axis_normal ==  m_axis_y ) index = "+y";
    else if( t_axis_normal == -m_axis_y ) index = "-y";
    else if( t_axis_normal ==  m_axis_z ) index = "+z";
    else if( t_axis_normal == -m_axis_z ) index = "-z";

    t_axis_normal = t_axis_normal.unit();

    G4double detector_medium_x  = m_detector_medium->get_solid()->GetXHalfLength();
    G4double detector_medium_y  = m_detector_medium->get_solid()->GetYHalfLength();
    G4double detector_medium_z  = m_detector_medium->get_solid()->GetZHalfLength();
    G4double calorimeter_width  = Calorimeter::get_width () / 2;
    G4double calorimeter_height = Calorimeter::get_height() / 2;
    G4double calorimeter_depth  = Calorimeter::get_depth () / 2;
    G4double DSPD_depth         = DirectionSensitivePhotoDetector::get_depth() / 2;
    G4int calorimeter_amount_x  = m_constructionMessenger->get_directionSensitivePhotoDetector_amount_x();
    G4int calorimeter_amount_y  = m_constructionMessenger->get_directionSensitivePhotoDetector_amount_y();
    G4int calorimeter_amount_z  = m_constructionMessenger->get_directionSensitivePhotoDetector_amount_z();
    G4ThreeVector detector_medium   ( detector_medium_x   , detector_medium_y   , detector_medium_z    );
    G4ThreeVector calorimeter       ( calorimeter_width   , calorimeter_height  , calorimeter_depth    );
    G4ThreeVector calorimeter_amount( calorimeter_amount_x, calorimeter_amount_y, calorimeter_amount_z );

    G4double angle = m_axis_z.angle( t_axis_normal );
    G4ThreeVector axis = m_axis_z.cross( t_axis_normal ).unit();
    G4RotationMatrix* rotationMatrix = new G4RotationMatrix( axis, angle );
    // if( index == "+y" )
    //     *rotationMatrix *= G4RotationMatrix( m_axis_y, m_pi );
    // G4RotationMatrix* rotationMatrix = new G4RotationMatrix( G4ThreeVector( 1, 0, 0 ),
    //                                                          G4ThreeVector( 0, 1, 0 ),
    //                                                          G4ThreeVector( 0, 0, 1 ) );
    G4RotationMatrix* rotationMatrix_zPi2 = new G4RotationMatrix( m_axis_z, m_pi_2 );
    G4RotationMatrix* rotationMatrix_calorimeterFull = new G4RotationMatrix();
    *rotationMatrix_calorimeterFull = *rotationMatrix_zPi2 * *rotationMatrix;

    // place "horizontal" calorimeters
    G4cout << "place horizontal calorimeters" << G4endl;
    count = 0;
    name = "/calorimeter_horizontal";
    G4ThreeVector translation_initial_horizontal( detector_medium_x - calorimeter_width - 2*calorimeter_height - 2*calorimeter_depth,
                                                  detector_medium_y - calorimeter_height - 2*calorimeter_depth,
                                                  detector_medium_z - calorimeter_depth );
    for( G4int index_x{ 0 }; index_x < calorimeter_amount.getX(); index_x++ ) {
        G4ThreeVector translation( -2*(calorimeter_width + calorimeter_height) * index_x, 0, 0 );
        for( G4int index_y{ 0 }; index_y < calorimeter_amount.getY() + 1; index_y++ ) {
            translation.setY( -2*(calorimeter_width + calorimeter_height) * index_y );
            G4ThreeVector position = translation + translation_initial_horizontal;
            position = *rotationMatrix * position;
            make_calorimeter_full( name, index                     + "_"
                                       + to_string( position.x() ) + "_" 
                                       + to_string( position.y() ) + "_" 
                                       + to_string( position.z() ) + "_" 
                                       + to_string( count++ ) 
                                 )->place( rotationMatrix, position, m_detector_medium->get_logicalVolume(), true );
        }
    }

    // place "vertical" calorimeters
    G4cout << "place vertical calorimeters" << G4endl;
    count = 0;
    name = "/calorimeter_vertical";
    G4ThreeVector translation_initial_vertical( detector_medium_x - calorimeter_height - 2*calorimeter_depth,
                                                detector_medium_y - calorimeter_width - 2*calorimeter_height - 2*calorimeter_depth,
                                                detector_medium_z - calorimeter_depth );
    for( G4int index_x{ 0 }; index_x < calorimeter_amount.getX() + 1; index_x++ ) {
        G4ThreeVector translation( -2*(calorimeter_width + calorimeter_height) * index_x, 0, 0 );
        for( G4int index_y{ 0 }; index_y < calorimeter_amount.getY(); index_y++ ) {
            translation.setY( -2*(calorimeter_width + calorimeter_height) * index_y );
            G4ThreeVector position = translation + translation_initial_vertical;
            position = *rotationMatrix * position;
            make_calorimeter_full( name, index                     + "_"
                                       + to_string( position.x() ) + "_" 
                                       + to_string( position.y() ) + "_" 
                                       + to_string( position.z() ) + "_" 
                                       + to_string( count++ ) 
                                 )->place( rotationMatrix_calorimeterFull, position, m_detector_medium->get_logicalVolume(), true );
        }
    }

    // place "middle" calorimeters
    G4cout << "place middle calorimeters" << G4endl;
    count = 0;
    name = "/calorimeter_middle";
    G4ThreeVector translation_initial_middle( detector_medium_x - calorimeter_height - 2*calorimeter_depth,
                                              detector_medium_y - calorimeter_height - 2*calorimeter_depth,
                                              detector_medium_z - calorimeter_depth );
    for( G4int index_x{ 0 }; index_x < calorimeter_amount.getX() + 1; index_x++ ) {
        G4ThreeVector translation( -2*(calorimeter_width + calorimeter_height) * index_x, 0, 0 );
        for( G4int index_y{ 0 }; index_y < calorimeter_amount.getY() + 1; index_y++ ) {
            translation.setY( -2*(calorimeter_width + calorimeter_height) * index_y );
            G4ThreeVector position = translation + translation_initial_middle;
            position = *rotationMatrix * position;
            make_calorimeter_middle( name, index                     + "_"
                                         + to_string( position.x() ) + "_" 
                                         + to_string( position.y() ) + "_" 
                                         + to_string( position.z() ) + "_" 
                                         + to_string( count++ ) 
                                    )->place( rotationMatrix, position, m_detector_medium->get_logicalVolume(), true );
        }
    }

    // place direction sensitive photodetectors
    G4cout << "place direction sensitive photodetectors" << G4endl;
    count = 0;
    name = "/DSPD";
    G4ThreeVector translation_initial_DSPD( detector_medium_x - calorimeter_width - 2*calorimeter_height - 2*calorimeter_depth,
                                            detector_medium_y - calorimeter_width - 2*calorimeter_height - 2*calorimeter_depth,
                                            detector_medium_z );
    for( G4int index_x{ 0 }; index_x < calorimeter_amount.getX(); index_x++ ) {
        G4ThreeVector translation( -2*(calorimeter_width + calorimeter_height) * index_x, 0, 0 );
        for( G4int index_y{ 0 }; index_y < calorimeter_amount.getY(); index_y++ ) {
            translation.setY( -2*(calorimeter_width + calorimeter_height) * index_y );
            G4ThreeVector position = translation + translation_initial_DSPD;
            position = *rotationMatrix * position;
            G4ThreeVector position_front = DirectionSensitivePhotoDetector::get_position_front( rotationMatrix, position, "back" );
            make_directionSensitivePhotoDetector( name, index                     + "_"
                                                      + to_string( position_front.x() ) + "_" 
                                                      + to_string( position_front.y() ) + "_" 
                                                      + to_string( position_front.z() ) + "_" 
                                                      + to_string( count++ ) 
                                                )->place( rotationMatrix, position, m_detector_medium->get_logicalVolume(), true, "back" );
        }
    }
}

void DetectorConstruction::ConstructSDandField() {
    if( !m_make_SDandField ) return;

    G4SDManager* SDManager = G4SDManager::GetSDMpointer();
    G4VSensitiveDetector* sensitiveDetector = nullptr;

    for( auto& calorimeter : m_calorimeters_full ) {
        sensitiveDetector = new CalorimeterSensitiveDetector( calorimeter->get_name() + "_sensitiveDetector" );
        SDManager->AddNewDetector( sensitiveDetector );
        calorimeter->get_logicalVolume()->SetSensitiveDetector( sensitiveDetector );
    }
    for( auto& calorimeter : m_calorimeters_middle ) {
        sensitiveDetector = new CalorimeterSensitiveDetector( calorimeter->get_name() + "_sensitiveDetector" );
        SDManager->AddNewDetector( sensitiveDetector );
        calorimeter->get_logicalVolume()->SetSensitiveDetector( sensitiveDetector );
    }
    for( int i = 0; i < m_directionSensitivePhotoDetectors.size(); i++ ) {
        auto& directionSensitivePhotoDetector = m_directionSensitivePhotoDetectors[i];
        auto lensSystem  = directionSensitivePhotoDetector->get_lensSystem();
        for( auto& lens : lensSystem->get_lenses() ) {
            sensitiveDetector = new LensSensitiveDetector( lens->get_name() + "_sensitiveDetector" );
            SDManager->AddNewDetector( sensitiveDetector );
            lens->get_logicalVolume()->SetSensitiveDetector( sensitiveDetector );
        }

        auto photoSensorSurface = directionSensitivePhotoDetector->get_photoSensor()->get_surface();
        PhotoSensorSensitiveDetector* psSD = new PhotoSensorSensitiveDetector( photoSensorSurface->get_name() + "_sensitiveDetector", i );
        psSD->set_position( m_directionSensitivePhotoDetectors[i]->get_position_front() );
        psSD->set_rotationMatrix( m_directionSensitivePhotoDetectors[i]->get_rotationMatrix() );
        SDManager->AddNewDetector( psSD );
        directionSensitivePhotoDetector->get_photoSensor()->set_sensitiveDetector( psSD );
        photoSensorSurface->get_logicalVolume()->SetSensitiveDetector( psSD );
    }
}

void DetectorConstruction::make_GDMLFile( const G4String& t_fileName ) {
    m_GDMLParser->Write( t_fileName, m_world_physicalVolume, true );
}

vector< Calorimeter* > DetectorConstruction::get_calorimeters_full() const {
    return m_calorimeters_full;
}

vector< Calorimeter* > DetectorConstruction::get_calorimeters_middle() const {
    return m_calorimeters_middle;
}

vector< DirectionSensitivePhotoDetector* > DetectorConstruction::get_directionSensitivePhotoDetectors() const {
    return m_directionSensitivePhotoDetectors;
}