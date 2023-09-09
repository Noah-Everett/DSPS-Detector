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

#include "PhotoSensor.hh"

PhotoSensor::PhotoSensor( G4String t_name ) {
    m_surface->set_material( m_constructionMessenger->get_photoSensor_surface_material() );
    m_body   ->set_material( m_constructionMessenger->get_photoSensor_body_material   () );

    G4ThreeVector surface_size = m_constructionMessenger->get_photoSensor_surface_size();
    G4ThreeVector body_size    = m_constructionMessenger->get_photoSensor_body_size   ();
    m_surface->set_solid( new G4Box( "surface", surface_size.x()/2, surface_size.y()/2, surface_size.z()/2 ) );
    m_body   ->set_solid( new G4Box( "body"   , body_size   .x()/2, body_size   .y()/2, body_size   .z()/2 ) );

    m_surface->set_visAttributes( m_constructionMessenger->get_photoSensor_surface_visAttributes() );
    m_body   ->set_visAttributes( m_constructionMessenger->get_photoSensor_body_visAttributes   () );

    m_surface->make_logicalVolume();
    m_body   ->make_logicalVolume();

    m_sensitiveDetector = new PhotoSensorSensitiveDetector( t_name );
    // G4SDManager::GetSDMpointer()->AddNewDetector( m_sensitiveDetector );
    m_surface->set_sensitiveDetector( m_sensitiveDetector );
}

PhotoSensor::~PhotoSensor() {
    if( m_surface           ) delete m_surface;
    if( m_body              ) delete m_body;
    if( m_sensitiveDetector ) delete m_sensitiveDetector;
}
void PhotoSensor::place( G4RotationMatrix* t_rotationMatrix     , 
                         G4ThreeVector     t_translation        , 
                         G4LogicalVolume * t_motherLogicalVolume, 
                         G4bool            t_isMany              ) {
    G4double surface_z = m_surface->get_solid()->GetZHalfLength();
    G4double body_z    = m_body   ->get_solid()->GetZHalfLength();
    G4double total_z   = surface_z * 2 + body_z * 2;
    G4ThreeVector translation_surface( 0, 0, -total_z/2 + surface_z );
    G4ThreeVector translation_body   ( 0, 0,  total_z/2 - body_z    );
    translation_surface = *t_rotationMatrix * translation_surface;
    translation_body    = *t_rotationMatrix * translation_body;
    translation_surface += t_translation;
    translation_body    += t_translation;
    m_surface->place( t_rotationMatrix, translation_surface, t_motherLogicalVolume, t_isMany );
    m_body   ->place( t_rotationMatrix, translation_body   , t_motherLogicalVolume, t_isMany );
}