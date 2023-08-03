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

PhotoSensor::PhotoSensor( G4Material   * t_surface_material, G4Material   * t_body_material, 
                          G4ThreeVector  t_surface_size    , G4ThreeVector  t_body_size     ) 
    : m_surface( new GeometricObjectBox() ),
      m_body   ( new GeometricObjectBox() ) {
    m_surface->set_material( m_surface_material );
    m_body   ->set_material( m_body_material    );

    m_surface->set_sold( new G4Box( "surface", t_surface_size.x()/2, t_surface_size.y()/2, t_surface_size.z()/2 ) );
    m_body   ->set_sold( new G4Box( "body"   , t_body_size   .x()/2, t_body_size   .y()/2, t_body_size   .z()/2 ) );

    m_surface->make_logicalVolume();
    m_body   ->make_logicalVolume();
}

PhotoSensor::~PhotoSensor() {
    delete m_surface;
    delete m_body;
}
void PhotoSensor::place( G4RotationMatrix* t_rotationMatrix     , 
                         G4ThreeVector     t_translation        , 
                         G4LogicalVolume * t_motherLogicalVolume, 
                         G4bool            t_isMany              ); 
    m_surface->place( t_rotationMatrix, t_translation, t_motherLogicalVolume, t_isMany );
}