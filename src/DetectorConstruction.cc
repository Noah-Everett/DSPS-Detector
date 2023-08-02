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
    if( m_detector            ) delete m_detector           ;
    if( m_calorimeter         ) delete m_calorimeter        ;
    if( m_lens                ) delete m_lens               ;
    if( m_photoSensor_surface ) delete m_photoSensor_surface;
    if( m_photoSensor_body    ) delete m_photoSensor_body   ;
    if( m_photoSensor         ) delete m_photoSensor        ;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    m_materials->print_materials();
    print_parameters();
    get_lensParameters();
    set_materials();

    make_world();
    make_detector();

    return m_world_pv;
}

void DetectorConstruction::print_parameters() {
    G4cout << "Attempting to use the following parameters:" << G4endl
            << "  world_x-------------------------: " << m_constructionMessenger->get_world_size_x                   () << G4endl
            << "  world_y-------------------------: " << m_constructionMessenger->get_world_size_y                   () << G4endl
            << "  world_z-------------------------: " << m_constructionMessenger->get_world_size_z                   () << G4endl
            << "  world_material------------------: " << m_constructionMessenger->get_world_material                 () << G4endl
            << "  detector_wall_thickness---------: " << m_constructionMessenger->get_detector_wall_thickness        () << G4endl
            << "  detector_wall_material----------: " << m_constructionMessenger->get_detector_wall_material         () << G4endl
            << "  detector_medium_material--------: " << m_constructionMessenger->get_detector_medium_material       () << G4endl
            << "  calorimeter_size_width----------: " << m_constructionMessenger->get_calorimeter_size_width         () << G4endl
            << "  calorimeter_size_height---------: " << m_constructionMessenger->get_calorimeter_size_height        () << G4endl
            << "  calorimeter_size_depth----------: " << m_constructionMessenger->get_calorimeter_size_depth         () << G4endl
            << "  calorimeter_material------------: " << m_constructionMessenger->get_calorimeter_material           () << G4endl
            << "  photoSensor_surface_size_width--: " << m_constructionMessenger->get_photoSensor_surface_size_width () << G4endl
            << "  photoSensor_surface_size_height-: " << m_constructionMessenger->get_photoSensor_surface_size_height() << G4endl
            << "  photoSensor_surface_size_depth--: " << m_constructionMessenger->get_photoSensor_surface_size_depth () << G4endl
            << "  photoSensor_surface_material----: " << m_constructionMessenger->get_photoSensor_surface_material   () << G4endl
            << "  photoSensor_body_size_width-----: " << m_constructionMessenger->get_photoSensor_body_size_width    () << G4endl
            << "  photoSensor_body_size_height----: " << m_constructionMessenger->get_photoSensor_body_size_height   () << G4endl
            << "  photoSensor_body_size_depth-----: " << m_constructionMessenger->get_photoSensor_body_size_depth    () << G4endl
            << "  photoSensor_body_material-------: " << m_constructionMessenger->get_photoSensor_body_material      () << G4endl
            << "  lens_parameterFilePath----------: " << m_constructionMessenger->get_lens_parameterFilePath         () << G4endl;
}

void DetectorConstruction::set_materials() {
    m_world           ->set_material( m_constructionMessenger->get_world_material              () );
    m_detector_wall   ->set_material( m_constructionMessenger->get_detector_wall_material      () );
    m_detector_medium ->set_material( m_constructionMessenger->get_detector_medium_material    () );
    m_calorimeter     ->set_material( m_constructionMessenger->get_calorimeter_material        () );
    m_photoSensor     ->set_material( m_constructionMessenger->get_photoSensor_surface_material() );
    m_photoSensor_body->set_material( m_constructionMessenger->get_photoSensor_body_material   () );
}

void DetectorConstruction::get_lensParameters() {
    G4String path = m_constructionMessenger->get_lens_parameterFilePath();
    m_lensParameterFileReader = new LensParameterFileReader( path );
    m_lensParameters = m_lensParameterFileReader->get_lenses();

    G4cout << "Lens parameters:" << G4endl;
    for( auto lens_parameters : m_lensParameters ) {
        G4cout << lens_parameters << G4endl;

        Surface  surface_1 = lens_parameters.surface_1;
        Surface  surface_2 = lens_parameters.surface_2;
        G4double d         = lens_parameters.d        ;
        G4double n         = lens_parameters.n        ;
        G4double x_l       = lens_parameters.x_l      ;

        GeometricObject* lens_geometricObject = new GeometricObject();


        m_lenses.push_back( lens );
    }
}

void DetectorConstruction::make_world() {
    G4double world_size_x = m_constructionMessenger->get_world_size_x() / 2;
    G4double world_size_y = m_constructionMessenger->get_world_size_y() / 2;
    G4double world_size_z = m_constructionMessenger->get_world_size_z() / 2;

    m_world.set_solid( new G4Box( "world", world_size_x, world_size_y, world_size_z ) );
    m_world.make_logicalVolume();
    m_world.set_logicalVolume( new G4LogicalVolume( m_world.get_solid(), m_world_material, "world"    ) );
    m_world_pv = new G4PVPlacement( 0, G4ThreeVector(0,0,0), m_world_lv, "world", 0, false, 0, m_checkOverlaps );
}

void DetectorConstruction::make_detector() {
    G4double calorimeter_size_width  = m_constructionMessenger->get_calorimeter_size_width () / 2;
    G4double calorimeter_size_height = m_constructionMessenger->get_calorimeter_size_height() / 2;
    G4double calorimeter_size_depth  = m_constructionMessenger->get_calorimeter_size_depth () / 2;

    G4double photoSensor_surface_size_width  = m_constructionMessenger->get_photoSensor_surface_size_width () / 2;
    G4double photoSensor_surface_size_height = m_constructionMessenger->get_photoSensor_surface_size_height() / 2;
    G4double photoSensor_surface_size_depth  = m_constructionMessenger->get_photoSensor_surface_size_depth () / 2;

    if( calorimeter_size_height != m_photoSensor_surface_size_height ) {
        G4cout << "Error: calorimeter_size_height != photoSensor_surface_size_height" << G4endl;
        exit(1);
    }

    // G4double detector_wall_size_x    = m_constructionMessenger->get_detector_wall_size_x   ();
    // G4double detector_wall_size_y    = m_constructionMessenger->get_detector_wall_size_y   ();
    // G4double detector_wall_size_z    = m_constructionMessenger->get_detector_wall_size_z   ();
    G4double detector_wall_thickness = m_constructionMessenger->get_detector_wall_thickness();

    // G4double detector_medium_size_x  = detector_wall_size_x - detector_wall_thickness;
    // G4double detector_medium_size_y  = detector_wall_size_y - detector_wall_thickness;
    // G4double detector_medium_size_z  = detector_wall_size_z - detector_wall_thickness;

    // m_detector_wall_s  = new G4Box( "detector_wall", detector_wall_size_x, detector_wall_size_y, detector_wall_size_z );
    // m_detector_wall_lv = new G4LogicalVolume( m_detector_wall_s, m_detector_wall_material, "detector_wall" );
    // m_detector_wall_pv = new G4PVPlacement( 0, G4ThreeVector(0,0,0), m_detector_wall_lv, "detector_wall", m_world_lv, false, 0, m_checkOverlaps );
    // m_detector_medium_s  = new G4Box( "detector_medium", detector_medium_size_x, detector_medium_size_y, detector_medium_size_z );
    // m_detector_medium_lv = new G4LogicalVolume( m_detector_medium_s, m_detector_medium_material, "detector_medium" );
    // m_detector_medium_pv = new G4PVPlacement( 0, G4ThreeVector(0,0,0), m_detector_medium_lv, "detector_medium", m_detector_wall_lv, false, 0, m_checkOverlaps );
}

void DetectorConstruction::make_calorimeter() {
    G4double calorimeter_size_widthAndHeight = m_constructionMessenger->get_calorimeter_size_widthAndHeight() / 2;
    G4double calorimeter_size_depth          = m_constructionMessenger->get_calorimeter_size_depth         () / 2;
}

void DetectorConstruction::make_photoSensor() {
    G4double photoSensor_surface_size_width  = m_constructionMessenger->get_photoSensor_surface_size_width () / 2;
    G4double photoSensor_surface_size_height = m_constructionMessenger->get_photoSensor_surface_size_height() / 2;
    G4double photoSensor_surface_size_depth  = m_constructionMessenger->get_photoSensor_surface_size_depth () / 2;

    G4double photoSensor_body_size_width  = m_constructionMessenger->get_photoSensor_body_size_width () / 2;
    G4double photoSensor_body_size_height = m_constructionMessenger->get_photoSensor_body_size_height() / 2;
    G4double photoSensor_body_size_depth  = m_constructionMessenger->get_photoSensor_body_size_depth () / 2;

    if( photoSensor_surface_size_width != photoSensor_body_size_width ) {
        G4cout << "Error: photoSensor_surface_size_width != photoSensor_body_size_width" << G4endl;
        exit(1);
    } else if( photoSensor_surface_size_height != photoSensor_body_size_height ) {
        G4cout << "Error: photoSensor_surface_size_height != photoSensor_body_size_height" << G4endl;
        exit(1);
    }

    G4Box*           photoSensor_s  = new G4Box("photoSensor_s", photoSensor_surface_size_width, photoSensor_surface_size_height, photoSensor_surface_size_depth + photoSensor_body_size_depth);
    G4LogicalVolume* photoSensor_lv = new G4LogicalVolume(photoSensor_box, detector_material, "photoSensor_lv");

    G4Box*           photoSensor_surface_s  = new G4Box("photoSensor_surface_box", photoSensor_surface_size_width, photoSensor_surface_size_height, photoSensor_surface_size_depth);
    G4LogicalVolume* photoSensor_surface_lv = new G4LogicalVolume(photoSensor_surface_box, detector_material, "photoSensor_surface_logical");

    // Set the sensitive detector for the surface logical volume (assuming you have defined a sensitive detector named "PhotoSensorSD")
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String surfaceSDname = "PhotoSensorSD";
    PhotoSensorSD* photoSensorSD = new PhotoSensorSD(surfaceSDname);
    SDman->AddNewDetector(photoSensorSD);
    photoSensor_surface_logical->SetSensitiveDetector(photoSensorSD);

    G4Box*           photoSensor_body_s  = new G4Box("photoSensor_body_box", photoSensor_body_size_width, photoSensor_body_size_height, photoSensor_body_size_depth);
    G4LogicalVolume* photoSensor_body_lv = new G4LogicalVolume(photoSensor_body_box, detector_material, "photoSensor_body_logical");

    // Set the sensitive detector for the body logical volume (assuming you have defined a sensitive detector named "PhotoSensorSD")
    photoSensor_body_logical->SetSensitiveDetector(photoSensorSD);

    // Place the surface and body logical volumes inside the main photoSensor logical volume
    G4ThreeVector bodyPosition(0, 0, photoSensor_surface_size_depth + photoSensor_body_size_depth);
    G4Transform3D bodyTransform = G4Transform3D(G4RotationMatrix(), bodyPosition);
    new G4PVPlacement(bodyTransform, photoSensor_body_logical, "photoSensor_body_placement", photoSensor_logical, false, 0);

    G4ThreeVector surfacePosition(0, 0, 0);
    G4Transform3D surfaceTransform = G4Transform3D(G4RotationMatrix(), surfacePosition);
    new G4PVPlacement(surfaceTransform, photoSensor_surface_logical, "photoSensor_surface_placement", photoSensor_logical, false, 0);

    // Save the photoSensor logical volume or do any other processing as needed.
    // You can store it in a data structure or a class member for later use in the construction of the geometry.

    // Clean up memory if needed
    delete photoSensor_box;
    delete photoSensor_surface_box;
    delete photoSensor_body_box;
}

void DetectorConstruction::make_DSPD() {
}