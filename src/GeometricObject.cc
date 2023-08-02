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

#include "GeometricObject.hh"

GeometricObject::GeometricObject() {}

GeometricObject::GeometricObject( G4VSolid* t_solid, G4Material* t_material, G4VSensitiveDetector* t_sensitiveDetector ) :
{
    set_solid            ( t_solid             );
    set_material         ( t_material          );
    set_sensitiveDetector( t_sensitiveDetector );

    make_logicalVolume();
}

GeometricObject::~GeometricObject() {}

void GeometricObject::set_material( G4Material* t_material ) { 
    m_material = t_material; 
}

void GeometricObject::set_material( G4String t_materialName ) { 
    m_material = G4Material::GetMaterial( t_materialName ); 
}

void GeometricObject::set_sensitiveDetector( G4VSensitiveDetector* t_sensitiveDetector ) { 
    m_sensitiveDetector = t_sensitiveDetector; 
    if( m_logicalVolume ) m_logicalVolume->SetSensitiveDetector( m_sensitiveDetector );
}

void GeometricObject::set_solid( G4VSolid* t_solid ) { 
    m_solid = t_solid; 
    m_name  = m_solid->GetName();
}

void GeometricObject::set_logicalVolume( G4LogicalVolume* t_logicalVolume ) { 
    m_logicalVolume = t_logicalVolume; 
}

void GeometricObject::make_logicalVolume() {
    m_logicalVolume = new G4LogicalVolume( m_solid, m_material, m_name, nullptr, m_sensitiveDetector, nullptr, true );
}

G4Material* GeometricObject::get_material() const { 
    return m_material; 
}

G4String GeometricObject::get_material_name() const {
    return m_material->GetName();
}

G4VSensitiveDetector* GeometricObject::get_sensitiveDetector() const { 
    return m_sensitiveDetector; 
}

G4VSolid* GeometricObject::get_solid() const { 
    return m_solid; 
}

G4LogicalVolume* GeometricObject::get_logicalVolume() const { 
    return m_logicalVolume; 
}

G4PVPlacement* GeometricObject::place( G4RotationMatrix* t_rotationMatrix   , 
                                       G4ThreeVector   * t_translationVector, 
                                       G4LogicalVolume * t_motherLogicalVolume, 
                                       G4bool            t_isMany = false ) {
    G4int copyNumber = 0;
    if( t_isMany ) {
        copyNumber = m_copyNumber;
        m_copyNumber++;
    }
    return new G4PVPlacement( t_rotationMatrix     , 
                              t_translationVector  , 
                              m_logicalVolume      , 
                              m_name               , 
                              t_motherLogicalVolume, 
                              t_isMany             , 
                              copyNumber            );
}