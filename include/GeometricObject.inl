//*/////////////////////////////////////////////////////////////////////////*//
//*//                    G4-DSPS-Detector-Simulation                      //*//
//*/////////////////////////////////////////////////////////////////////////*//
//*//                                                                     //*//
//*// Author:                                                             //*//
//*//   Noah Everett (noah.everett@mines.sdsmt.edu)                       //*//
//*//                                                                     //*//
//*/////////////////////////////////////////////////////////////////////////*//
//*//                                                                     //*//
//*/////////////////////////////////////////////////////////////////////////*//

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
GeometricObject< SolidType >::~GeometricObject() {
    // if( m_material          ) delete m_material;
    // if( m_sensitiveDetector ) delete m_sensitiveDetector;
    if( m_solid             ) delete m_solid;
    // if( m_logicalVolume     ) delete m_logicalVolume;
    // if( m_visAttributes     ) delete m_visAttributes;
}

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
    if( m_logicalVolume ) {
        m_logicalVolume->SetSensitiveDetector( m_sensitiveDetector );
        G4cout << "Sensitive detector set to " << m_sensitiveDetector->GetName() << G4endl;
    }
    // G4cout << "size = " << m_logicalVolume->GetSolid()->GetDimensions() << G4endl;
    G4cout << "translation = " << m_translationVector << G4endl;
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
    if( t_isMany )
        copyNumber = m_copyNumber++;

    m_rotationMatrix    = t_rotationMatrix;
    m_translationVector = t_translationVector;

    return new G4PVPlacement( t_rotationMatrix     , 
                              t_translationVector  , 
                              m_logicalVolume      , 
                              m_name               , 
                              t_motherLogicalVolume, 
                              t_isMany             , 
                              copyNumber           ,
                              m_constructionMessenger->get_checkOverlaps() );
}

template< class SolidType >
G4String GeometricObject< SolidType >::get_name() const { 
    return m_name; 
}