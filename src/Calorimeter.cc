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

#include "Calorimeter.hh"

Calorimeter::Calorimeter( G4String t_name, G4String t_index, G4ThreeVector t_size ) {
    m_name = t_name + "_" + t_index;

    m_calorimeter = new GeometricObjectBox();

    if( t_size == G4ThreeVector() )
        t_size = G4ThreeVector( get_width(), get_height(), get_depth() );

    m_calorimeter->set_solid            ( new G4Box( t_name + "_G4Box", t_size.x()/2, t_size.y()/2, t_size.z()/2 ) );
    m_calorimeter->set_material         ( m_constructionMessenger->get_calorimeter_material                         () );
    m_calorimeter->set_visAttributes    ( m_constructionMessenger->get_calorimeter_visAttributes                    () );
    m_calorimeter->set_visibility       ( m_constructionMessenger->get_calorimeter_visibility                       () );
    m_calorimeter->make_logicalVolume   (                                                                              );
}

Calorimeter::~Calorimeter() {
    if( m_calorimeter ) delete m_calorimeter;
}

void Calorimeter::place( G4RotationMatrix* t_rotationMatrix, G4ThreeVector t_translationVector, G4LogicalVolume* t_parentLogicalVolume, G4bool t_isMany ) {
    m_calorimeter->place( t_rotationMatrix, t_translationVector, t_parentLogicalVolume, t_isMany );
}

G4ThreeVector Calorimeter::get_size() {
    return G4ThreeVector( get_width(), get_height(), get_depth() );
}

G4double Calorimeter::get_width() {
    return ConstructionMessenger::get_instance()->get_calorimeter_size_width();
}

G4double Calorimeter::get_height() {
    return ConstructionMessenger::get_instance()->get_calorimeter_size_height();
}

G4double Calorimeter::get_depth() {
    return ConstructionMessenger::get_instance()->get_calorimeter_size_depth();
}

G4String Calorimeter::get_name() {
    return m_name;
}

G4LogicalVolume* Calorimeter::get_logicalVolume() {
    return m_calorimeter->get_logicalVolume();
}