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

template< class SolidType >
GeometricObject< SolidType >::GeometricObject() {}

template< class SolidType >
GeometricObject< SolidType >::GeometricObject( SolidType           * t_solid            , 
                                               G4Material          * t_material         , 
                                               G4VSensitiveDetector* t_sensitiveDetector,
                                               G4bool                t_visibility       ,
                                               G4VisAttributes     * t_visAttributes     ) {
    set_solid            ( t_solid             );
    set_material         ( t_material          );
    set_sensitiveDetector( t_sensitiveDetector );
    set_visibility       ( t_visibility        );
    set_visAttributes    ( t_visAttributes     );

    make_logicalVolume();
}

template< class SolidType >
GeometricObject< SolidType >::~GeometricObject() {}

template< class SolidType >
void GeometricObject< SolidType >::set_material( G4Material* t_material ) { 
    m_material = t_material; 
}

template< class SolidType >
void GeometricObject< SolidType >::set_material( G4String t_materialName ) { 
    m_material = G4Material::GetMaterial( t_materialName ); 
}

template< class SolidType >
void GeometricObject< SolidType >::set_sensitiveDetector( G4VSensitiveDetector* t_sensitiveDetector ) { 
    m_sensitiveDetector = t_sensitiveDetector; 
    if( m_logicalVolume ) 
        m_logicalVolume->SetSensitiveDetector( m_sensitiveDetector );
}

template< class SolidType >
void GeometricObject< SolidType >::set_solid( SolidType* t_solid ) { 
    m_solid = t_solid; 
    m_name  = m_solid->GetName();
}

template< class SolidType >
void GeometricObject< SolidType >::set_logicalVolume( G4LogicalVolume* t_logicalVolume ) { 
    m_logicalVolume = t_logicalVolume; 
    if( m_visAttributes )
        m_logicalVolume->SetVisAttributes( m_visAttributes );
}

template< class SolidType >
void GeometricObject< SolidType >::set_visibility( G4bool t_visibility ) { 
    m_visibility = t_visibility; 
    if( m_visAttributes ) 
        m_visAttributes->SetVisibility( m_visibility );
    else
        G4Exception( "GeometricObject::set_visibility", "GeometricObject", FatalException, "VisAttributes not set" );
}

template< class SolidType >
void GeometricObject< SolidType >::set_visAttributes( G4VisAttributes* t_visAttributes ) { 
    m_visAttributes = t_visAttributes; 
    if( m_visAttributes ) {
        m_visAttributes->SetVisibility( m_visibility );
        if( m_logicalVolume ) 
            m_logicalVolume->SetVisAttributes( m_visAttributes );
    }
}

template< class SolidType >
void GeometricObject< SolidType >::set_colour( G4String t_colourName ) { 
    G4Colour temp;
    if( !G4Colour::GetColour( t_colourName, temp ) )
        G4Exception( "GeometricObject::set_colour", "GeometricObject", FatalException, "Colour not found" );
    else if( m_visAttributes ) 
        m_visAttributes->SetColour( temp );
    else
        G4Exception( "GeometricObject::set_colour", "GeometricObject", FatalException, "VisAttributes not set" );
}

template< class SolidType >
void GeometricObject< SolidType >::set_colour( G4Colour t_colour ) { 
    if( m_visAttributes ) 
        m_visAttributes->SetColour( t_colour );
    else
        G4Exception( "GeometricObject::set_colour", "GeometricObject", FatalException, "VisAttributes not set" );
}

template< class SolidType >
void GeometricObject< SolidType >::set_alpha( G4double t_alpha ) { 
    if( m_visAttributes ) {
        G4Colour temp = m_visAttributes->GetColour();
        m_visAttributes->SetColour( G4Colour( temp.GetRed(), temp.GetGreen(), temp.GetBlue(), t_alpha ) );
    } else
        G4Exception( "GeometricObject::set_alpha", "GeometricObject", FatalException, "VisAttributes not set" );
}

template< class SolidType >
void GeometricObject< SolidType >::set_forceSolid( G4bool t_forceSolid ) { 
    if( m_visAttributes ) 
        m_visAttributes->SetForceSolid( t_forceSolid );
    else
        G4Exception( "GeometricObject::set_forceSolid", "GeometricObject", FatalException, "VisAttributes not set" );
}

template< class SolidType >
void GeometricObject< SolidType >::make_logicalVolume() {
    m_logicalVolume = new G4LogicalVolume( m_solid, m_material, m_name, nullptr, m_sensitiveDetector, nullptr, true );
    if( m_visAttributes ) 
        m_logicalVolume->SetVisAttributes( m_visAttributes );
    if( m_sensitiveDetector ) 
        m_logicalVolume->SetSensitiveDetector( m_sensitiveDetector );
}

template< class SolidType >
G4Material* GeometricObject< SolidType >::get_material() const { 
    return m_material; 
}

template< class SolidType >
G4String GeometricObject< SolidType >::get_material_name() const {
    return m_material->GetName();
}

template< class SolidType >
G4VSensitiveDetector* GeometricObject< SolidType >::get_sensitiveDetector() const { 
    return m_sensitiveDetector; 
}

template< class SolidType >
SolidType* GeometricObject< SolidType >::get_solid() const { 
    return m_solid; 
}

template< class SolidType >
G4LogicalVolume* GeometricObject< SolidType >::get_logicalVolume() const { 
    return m_logicalVolume; 
}

template< class SolidType >
G4bool GeometricObject< SolidType >::get_visibility() const { 
    return m_visibility; 
}

template< class SolidType >
G4VisAttributes* GeometricObject< SolidType >::get_visAttributes() const { 
    return m_visAttributes; 
}

template< class SolidType >
G4PVPlacement* GeometricObject< SolidType >::place( G4RotationMatrix* t_rotationMatrix     , 
                                                    G4ThreeVector     t_translationVector  , 
                                                    G4LogicalVolume * t_motherLogicalVolume, 
                                                    G4bool            t_isMany              ) {
    if( !m_logicalVolume ) 
        make_logicalVolume();

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