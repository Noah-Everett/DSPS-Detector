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
    m_materials        ( t_materials         )
{
}
    
DetectorConstruction::~DetectorConstruction() {
    delete m_constructionMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    m_materials->print_materials();
    print_sizes();

    make_world();
    make_detector();

    return m_world_pv;
}

void DetectorConstruction::set_material( G4Material*& t_material, G4String t_name ) {
    t_material = ( G4Material::GetMaterial( t_name ) );
}

void DetectorConstruction::print_sizes() {
    G4cout << "Attempting to use dimensions:" << G4endl
            << "  world_x-------------------------: " << m_constructionMessenger->get_world_size_x                   () << G4endl
            << "  world_y-------------------------: " << m_constructionMessenger->get_world_size_y                   () << G4endl
            << "  world_z-------------------------: " << m_constructionMessenger->get_world_size_z                   () << G4endl
            << "  detector_wall_x-----------------: " << m_constructionMessenger->get_detector_wall_size_x           () << G4endl
            << "  detector_wall_y-----------------: " << m_constructionMessenger->get_detector_wall_size_y           () << G4endl
            << "  detector_wall_z-----------------: " << m_constructionMessenger->get_detector_wall_size_z           () << G4endl
            << "  detector_wall_thickness---------: " << m_constructionMessenger->get_detector_wall_thickness        () << G4endl
            << "  calorimeter_size_widthAndHeight-: " << m_constructionMessenger->get_calorimeter_size_widthAndHeight() << G4endl
            << "  calorimeter_size_depth----------: " << m_constructionMessenger->get_calorimeter_size_depth         () << G4endl;
}

void DetectorConstruction::make_world() {
    set_material( m_world_medium_material, m_constructionMessenger->get_world_medium_material() );

    G4double world_size_x = m_constructionMessenger->get_world_size_x();
    G4double world_size_y = m_constructionMessenger->get_world_size_y();
    G4double world_size_z = m_constructionMessenger->get_world_size_z();

    m_world_s  = new G4Box( "world", world_size_x, world_size_y, world_size_z );
    m_world_lv = new G4LogicalVolume( m_world_s, m_world_medium_material, "world" );
    m_world_pv = new G4PVPlacement( 0, G4ThreeVector(0,0,0), m_world_lv, "world", 0, false, 0, m_checkOverlaps );
}

void DetectorConstruction::make_detector() {
    set_material( m_detector_medium_material, m_constructionMessenger->get_detector_medium_material() );
    set_material( m_detector_wall_material  , m_constructionMessenger->get_detector_wall_material  () );

    G4double detector_wall_size_x    = m_constructionMessenger->get_detector_wall_size_x   ();
    G4double detector_wall_size_y    = m_constructionMessenger->get_detector_wall_size_y   ();
    G4double detector_wall_size_z    = m_constructionMessenger->get_detector_wall_size_z   ();
    G4double detector_wall_thickness = m_constructionMessenger->get_detector_wall_thickness();

    G4double detector_medium_size_x  = detector_wall_size_x - detector_wall_thickness;
    G4double detector_medium_size_y  = detector_wall_size_y - detector_wall_thickness;
    G4double detector_medium_size_z  = detector_wall_size_z - detector_wall_thickness;

    m_detector_wall_s  = new G4Box( "detector_wall", detector_wall_size_x, detector_wall_size_y, detector_wall_size_z );
    m_detector_wall_lv = new G4LogicalVolume( m_detector_wall_s, m_detector_wall_material, "detector_wall" );
    m_detector_wall_pv = new G4PVPlacement( 0, G4ThreeVector(0,0,0), m_detector_wall_lv, "detector_wall", m_world_lv, false, 0, m_checkOverlaps );
    m_detector_medium_s  = new G4Box( "detector_medium", detector_medium_size_x, detector_medium_size_y, detector_medium_size_z );
    m_detector_medium_lv = new G4LogicalVolume( m_detector_medium_s, m_detector_medium_material, "detector_medium" );
    m_detector_medium_pv = new G4PVPlacement( 0, G4ThreeVector(0,0,0), m_detector_medium_lv, "detector_medium", m_detector_wall_lv, false, 0, m_checkOverlaps );
}

void DetectorConstruction::make_calorimeter() {
    set_material( m_calorimeter_material, m_constructionMessenger->get_calorimeter_material() );

    G4double calorimeter_size_widthAndHeight = m_constructionMessenger->get_calorimeter_size_widthAndHeight();
    G4double calorimeter_size_depth          = m_constructionMessenger->get_calorimeter_size_depth         ();
}

void DetectorConstruction::make_DSPD() {
    set_material( m_lens_material,                m_constructionMessenger->get_lens_material               () );
    set_material( m_photoSensor_surface_material, m_constructionMessenger->get_photoSensor_surface_material() );
}