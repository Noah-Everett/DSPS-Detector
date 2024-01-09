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

#include "Medium.hh"

#include <cmath>

using std::max;
using std::to_string;

Medium::Medium( G4String t_name, G4int t_nMedium, G4ThreeVector t_size ) {
    m_name = t_name + "_medium_" + to_string( t_nMedium );
    m_size = t_size;

    m_medium = new GeometricObjectBox();
    m_medium->set_material( m_constructionMessenger->get_detector_medium_material() );
    m_medium->set_solid( new G4Box( m_name, m_size.x() / 2, m_size.y() / 2, m_size.z() / 2 ) );
    m_medium->set_visAttributes( m_constructionMessenger->get_detector_medium_visAttributes() );
    m_medium->make_logicalVolume();
}

Medium::~Medium() {
    if( m_medium ) delete m_medium;
}

ostream& operator<<( ostream& t_os, Medium* t_medium )
{
    t_os << *t_medium;
    return t_os;
}

ostream& operator<<( ostream& t_os, const Medium& t_medium ) {
    // todo
    return t_os;
}

void Medium::place( G4RotationMatrix * t_rotationMatrix     , 
                    G4ThreeVector      t_translation        ,
                    G4LogicalVolume  * t_motherLogicalVolume,
                    G4bool             t_isMany              ) {
    m_rotationMatrix = t_rotationMatrix;
    m_translation    = t_translation;
    m_medium->place( t_rotationMatrix, t_translation, t_motherLogicalVolume, t_isMany );
}

GeometricObjectBox* Medium::get_geometricObject() {
    return m_medium;
}

G4String Medium::get_name() {
    return m_name;
}

G4LogicalVolume* Medium::get_logicalVolume() {
    return m_medium->get_logicalVolume();
}

MediumSensitiveDetector* Medium::get_sensitiveDetector() {
    return m_mediumSensitiveDetector;
}

void Medium::set_name( const G4String& t_name ) {
    m_name = t_name;
}


G4ThreeVector Medium::get_position() {
    return m_translation;
}

G4RotationMatrix* Medium::get_rotationMatrix() {
    return m_rotationMatrix;
}

void Medium::set_sensitiveDetector( MediumSensitiveDetector* t_mediumSensitiveDetector ) {
    m_mediumSensitiveDetector = t_mediumSensitiveDetector;
    m_medium->set_sensitiveDetector( t_mediumSensitiveDetector );
}

G4ThreeVector Medium::get_size() {
    return m_size;
}

G4double Medium::get_size_x() {
    return m_size.x();
}

G4double Medium::get_size_y() {
    return m_size.y();
}

G4double Medium::get_size_z() {
    return m_size.z();
}