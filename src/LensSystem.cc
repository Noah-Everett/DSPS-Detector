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

#include "LensSystem.hh"

LensSystem::LensSystem( const G4String& t_name, G4bool t_makeLenses ) {
    m_name = t_name;

    if( !t_makeLenses ) 
        return;

    if( m_constructionMessenger->get_lens_amount() == 0 )
        G4Exception( "LensSystem::LensSystem", "InvalidArgument", FatalException, "Lens amount is 0" );

    for( G4int nLens{ 0 }; nLens < m_constructionMessenger->get_lens_amount(); nLens++ )
        add_lens( new Lens( t_name, nLens ) );
}

LensSystem::~LensSystem() {
    for( Lens* lens : m_lenses )
        if( lens ) 
            delete lens;
}

void LensSystem::add_lens( Lens* t_lens ) {
    m_lenses.push_back( t_lens );
}

void  LensSystem::place( G4RotationMatrix* t_rotationMatrix     , 
                         G4ThreeVector     t_translationVector  , 
                         G4LogicalVolume * t_motherLogicalVolume, 
                         G4bool            t_isMany              ) {
    for( G4int nLens{ 0 }; nLens < m_lenses.size(); nLens++ ) {
        G4ThreeVector position( 0, 0, -m_constructionMessenger->get_lens_position( nLens ) );
        if( position.z() > m_position_back.z() )
            m_position_back = position;
        if( position.z() < m_position_front.z() )
            m_position_front = position;
        G4cout << "LensSystem::place: " << m_name << " lens " << nLens << " position: " << position << G4endl;
        position = *t_rotationMatrix * position;
        m_lenses[ nLens ]->place( t_rotationMatrix, t_translationVector - position, t_motherLogicalVolume, t_isMany );
    }
    m_position_center = ( m_position_front + m_position_back ) / 2;
}

vector< Lens* > LensSystem::get_lenses() const { 
    return m_lenses; 
}

Lens* LensSystem::get_lens( G4int t_index ) const { 
    return m_lenses.at( t_index ); 
}

G4String LensSystem::get_name() const { 
    return m_name; 
}

void LensSystem::set_name( const G4String& t_name ) { 
    m_name = t_name; 
    for( G4int nLens{ 0 }; nLens < m_lenses.size(); nLens++ )
        m_lenses[ nLens ]->set_name( t_name + "_lens_" + to_string( nLens ) );
}

vector< G4ThreeVector > calculate_relativePositions() {
    ConstructionMessenger* constructionMessenger{ ConstructionMessenger::get_instance() };

    G4ThreeVector relativePosition_front ;
    G4ThreeVector relativePosition_center;
    G4ThreeVector relativePosition_back  ;

    for( G4int nLens = 0; nLens < constructionMessenger->get_lens_amount(); nLens++ ) {
        G4double position = -constructionMessenger->get_lens_position( nLens );
        if( position > relativePosition_back.z() )
            m_position_back = G4ThreeVector( 0, 0, position );
        if( position < relativePosition_front.z() )
            m_position_front = G4ThreeVector( 0, 0, position );
    }
    position_center = ( m_position_front + m_position_back ) / 2;

    return { relativePosition_front, relativePosition_center, relativePosition_back };
}

G4ThreeVector LensSystem::get_position_front( G4RotationMatrix* t_rotationMatrix   ,
                                              G4ThreeVector     t_translationVector,
                                              const char      * t_relativePosition  ) {
    G4String relativePosition( t_relativePosition );
    to_lower( relativePosition );

    vector< G4ThreeVector > relativePositions = calculate_relativePositions();
    relativePosition_front  = relativePositions[ 0 ];
    relativePosition_center = relativePositions[ 1 ];
    relativePosition_back   = relativePositions[ 2 ];

    if( relativePosition == "front" || relativePosition == "f" )
        return t_translationVector;
    else if( relativePosition == "center" || relativePosition == "c" )
        return t_translationVector + *t_rotationMatrix * calculate
    else if( relativePosition == "back" || relativePosition == "b" )
        return t_translationVector + *t_rotationMatrix * G4ThreeVector( 0, 0, -get_size().z() );
    else
        G4Exception( "LensSystem::get_position_front", "InvalidArgument", FatalException, "Invalid relative position" );

    return G4ThreeVector();
}

G4ThreeVector LensSystem::get_position_center( G4RotationMatrix* t_rotationMatrix   ,
                                               G4ThreeVector     t_translationVector,
                                               const char      * t_relativePosition  ) {
    G4String relativePosition( t_relativePosition );
    to_lower( relativePosition );

    if( relativePosition == "front" || relativePosition == "f" )
        return t_translationVector + *t_rotationMatrix * G4ThreeVector( 0, 0, get_size().z() / 2 );
    else if( relativePosition == "center" || relativePosition == "c" )
        return t_translationVector;
    else if( relativePosition == "back" || relativePosition == "b" )
        return t_translationVector + *t_rotationMatrix * G4ThreeVector( 0, 0, -get_size().z() / 2 );
    else
        G4Exception( "LensSystem::get_position_center", "InvalidArgument", FatalException, "Invalid relative position" );

    return G4ThreeVector();
}