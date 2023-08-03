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

DetectorConstruction::DetectorConstruction( ConstructionMessenger* t_constructionMessenger, Materials* t_materials ) :
    m_constructionMessenger( t_constructionMessenger ),
    m_materials            ( t_materials             )
{
}
    
DetectorConstruction::~DetectorConstruction() {
    if( m_lensParameterFileReader ) delete m_lensParameterFileReader;

    if( m_world               ) delete m_world              ;
    if( m_detector_wall       ) delete m_detector_wall      ;
    if( m_detector_medium     ) delete m_detector_medium    ;
    if( m_calorimeter         ) delete m_calorimeter        ;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    m_materials            ->print_materials ();
    m_constructionMessenger->print_parameters();

    set_materials    (                                              );
    set_checkOverlaps( m_constructionMessenger->get_checkOverlaps() );

    make_world                          ();
    make_detector                       ();
    make_calorimeter                    ();
    make_directionSensitivePhotoDetector();
    
    G4VPhysicalVolume* world_pv =
    m_world          ->place( nullptr, G4ThreeVector(0,0,0), nullptr                              );
    m_detector_wall  ->place( nullptr, G4ThreeVector(0,0,0), m_world        ->get_logicalVolume() );
    m_detector_medium->place( nullptr, G4ThreeVector(0,0,0), m_detector_wall->get_logicalVolume() );

    place_surface(  m_axis_x );
    place_surface( -m_axis_x );
    place_surface(  m_axis_y );
    place_surface( -m_axis_y );
    place_surface(  m_axis_z );
    place_surface( -m_axis_z );

    return world_pv;
}

void DetectorConstruction::set_materials() {
    m_world           ->set_material( m_constructionMessenger->get_world_material              () );
    m_detector_wall   ->set_material( m_constructionMessenger->get_detector_wall_material      () );
    m_detector_medium ->set_material( m_constructionMessenger->get_detector_medium_material    () );
    m_calorimeter     ->set_material( m_constructionMessenger->get_calorimeter_material        () );
}

void DetectorConstruction::set_checkOverlaps( G4bool t_checkOverlaps ) {
    m_checkOverlaps = t_checkOverlaps;
}

void DetectorConstruction::make_world() {
    G4double world_size_x = m_constructionMessenger->get_world_size_x() / 2;
    G4double world_size_y = m_constructionMessenger->get_world_size_y() / 2;
    G4double world_size_z = m_constructionMessenger->get_world_size_z() / 2;

    m_world->set_solid( new G4Box( "world", world_size_x, world_size_y, world_size_z ) );
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
    m_detector_wall->set_solid( new G4Box( "detector_wall", detector_wall_size_x / 2, detector_wall_size_y / 2, detector_wall_size_z / 2 ) );
    m_detector_wall->make_logicalVolume();

    G4double detector_medium_size_x = detector_wall_size_x - 2 * detector_wall_thickness;
    G4double detector_medium_size_y = detector_wall_size_y - 2 * detector_wall_thickness;
    G4double detector_medium_size_z = detector_wall_size_z - 2 * detector_wall_thickness;
    m_detector_medium->set_solid( new G4Box( "detector_medium", detector_medium_size_x / 2, detector_medium_size_y / 2, detector_medium_size_z / 2 ) );
    m_detector_medium->make_logicalVolume();
}

void DetectorConstruction::make_calorimeter() {
    G4double calorimeter_size_width  = m_constructionMessenger->get_calorimeter_size_width () / 2;
    G4double calorimeter_size_height = m_constructionMessenger->get_calorimeter_size_height() / 2;
    G4double calorimeter_size_depth  = m_constructionMessenger->get_calorimeter_size_depth () / 2;

    m_calorimeter->set_solid( new G4Box( "calorimeter", calorimeter_size_width, calorimeter_size_height, calorimeter_size_depth ) );
    m_calorimeter->make_logicalVolume();
}

void DetectorConstruction::make_directionSensitivePhotoDetector() {

    m_directionSensitivePhotoDetector = 
    new DirectionSensitivePhotoDetector( m_lensParameterFileReader,
                                         G4Material::GetMaterial( m_constructionMessenger->get_photoDetector_surface_material() ),
                                         G4Material::GetMaterial( m_constructionMessenger->get_photoDetector_body_material   () ),
                                         m_constructionMessenger->get_photoSensor_surface_size                               ()  ,
                                         m_constructionMessenger->get_photoSensor_body_size                                  ()   );
}

void DetectorConstruction::place_surface( G4ThreeVector t_axis_normal ) {
    t_axis_normal = t_axis_normal.unit();

    G4double detector_medium_x  = m_detector_medium->get_solid()->GetXHalfLength();
    G4double detector_medium_y  = m_detector_medium->get_solid()->GetYHalfLength();
    G4double detector_medium_z  = m_detector_medium->get_solid()->GetZHalfLength();
    G4double calorimeter_width  = m_calorimeter    ->get_solid()->GetXHalfLength();
    G4double calorimeter_height = m_calorimeter    ->get_solid()->GetYHalfLength();
    G4double calorimeter_depth  = m_calorimeter    ->get_solid()->GetZHalfLength();
    G4int calorimeter_amount_x = m_constructionMessenger->get_directionSensitivePhotoDetector_amount_x();
    G4int calorimeter_amount_y = m_constructionMessenger->get_directionSensitivePhotoDetector_amount_y();
    G4int calorimeter_amount_z = m_constructionMessenger->get_directionSensitivePhotoDetector_amount_z();
    G4ThreeVector detector_medium   ( detector_medium_x   , detector_medium_y   , detector_medium_z    );
    G4ThreeVector calorimeter       ( calorimeter_width   , calorimeter_height  , calorimeter_depth    );
    G4ThreeVector calorimeter_amount( calorimeter_amount_x, calorimeter_amount_y, calorimeter_amount_z );

    G4double angle = m_axis_z.angle( t_axis_normal );
    G4ThreeVector axis = m_axis_z.cross( t_axis_normal );
    axis = axis.unit();
    G4RotationMatrix* rotationMatrix = new G4RotationMatrix( axis, angle );
    rotationMatrix->print( G4cout );
    G4RotationMatrix* rotationMatrixTurn = new G4RotationMatrix( m_axis_z, m_pi_2 );
    *rotationMatrixTurn *= *rotationMatrix;

    // place "horizontal" calorimeters
    G4ThreeVector translation_initial_horizontal( detector_medium_x - calorimeter_width - 2*calorimeter_height - 2*calorimeter_depth,
                                                  detector_medium_y - calorimeter_height - 2*calorimeter_depth,
                                                  detector_medium_z - calorimeter_depth );
    for( G4int index_x{ 0 }; index_x < calorimeter_amount.getX(); index_x++ ) {
        G4ThreeVector translation_delta( -2*(calorimeter_width + calorimeter_height) * index_x, 0, 0 );
        for( G4int index_y{ 0 }; index_y < calorimeter_amount.getY() + 1; index_y++ ) {
            translation_delta.setY( -2*(calorimeter_width + calorimeter_height) * index_y );
            m_calorimeter->place( rotationMatrix, *rotationMatrix * (translation_initial_horizontal + translation_delta), m_detector_medium->get_logicalVolume(), true );
        }
    }

    // place "vertical" calorimeters
    G4ThreeVector translation_initial_vertical( detector_medium_x - calorimeter_height - 2*calorimeter_depth,
                                                detector_medium_y - calorimeter_width - 2*calorimeter_height - 2*calorimeter_depth,
                                                detector_medium_z - calorimeter_depth );
    for( G4int index_x{ 0 }; index_x < calorimeter_amount.getX() + 1; index_x++ ) {
        G4ThreeVector translation_delta( -2*(calorimeter_width + calorimeter_height) * index_x, 0, 0 );
        for( G4int index_y{ 0 }; index_y < calorimeter_amount.getY(); index_y++ ) {
            translation_delta.setY( -2*(calorimeter_width + calorimeter_height) * index_y );
            m_calorimeter->place( rotationMatrixTurn, *rotationMatrix * (translation_initial_vertical + translation_delta), m_detector_medium->get_logicalVolume(), true );
        }
    }

    // place direction sensitive photodetectors
    G4ThreeVector translation_initial_directionSensitivePhotoDetector( detector_medium_x - calorimeter_width - calorimeter_height - calorimeter_depth,
                                                                       detector_medium_y - calorimeter_height - calorimeter_depth,
                                                                       detector_medium_z - calorimeter_depth );
}