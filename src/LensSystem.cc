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
    m_rotationMatrix    = t_rotationMatrix;
    m_translationVector = t_translationVector;

    for( G4int nLens{ 0 }; nLens < m_lenses.size(); nLens++ ) {
        G4ThreeVector position( 0, 0, m_constructionMessenger->get_lens_position( nLens ) );
        if( position.z() > m_relativePosition_back.z() )
            m_relativePosition_back = position;
        if( position.z() < m_relativePosition_front.z() )
            m_relativePosition_front = position;
        G4cout << "LensSystem::place: " << m_name << " lens " << nLens << " position: " << position << G4endl;
        position = *t_rotationMatrix * position;
        m_lenses[ nLens ]->place( t_rotationMatrix, t_translationVector + position, t_motherLogicalVolume, t_isMany );
    }
    m_relativePosition_center = ( m_relativePosition_front + m_relativePosition_back ) / 2;
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

vector< G4int > LensSystem::calculate_lensOrder() {
    ConstructionMessenger* constructionMessenger{ ConstructionMessenger::get_instance() };
    if( constructionMessenger->get_lens_amount() == 0 )
        G4Exception( "LensSystem::calculate_lensOrder", "InvalidArgument", FatalException, "Lens amount is 0" );

    vector< G4int > lensOrder; // lensOrder[ 0 ] is the front lens, lensOrder[ lensOrder.size() - 1 ] is the back lens
    for( G4int nLens = 0; nLens < constructionMessenger->get_lens_amount(); nLens++ ) {
        G4double position = constructionMessenger->get_lens_position( nLens );
        if( lensOrder.size() == 0 ) {
            lensOrder.push_back( nLens );
            continue;
        }
        for( G4int nLensOrder = 0; nLensOrder < lensOrder.size(); nLensOrder++ ) {
            G4double positionOrder = constructionMessenger->get_lens_position( lensOrder[ nLensOrder ] );
            if( position < positionOrder ) {
                lensOrder.insert( lensOrder.begin() + nLensOrder, nLens );
                break;
            }
            if( nLensOrder == lensOrder.size() - 1 ) {
                lensOrder.push_back( nLens );
                break;
            }
        }
    }

    return lensOrder;
}

vector< G4ThreeVector > LensSystem::calculate_relativePositions() {
    return calculate_relativePositions( calculate_lensOrder() );
}

vector< G4ThreeVector > LensSystem::calculate_relativePositions( const vector< G4int >& t_lensOrder ) {
    ConstructionMessenger* constructionMessenger{ ConstructionMessenger::get_instance() };

    G4ThreeVector relativePosition_front = G4ThreeVector( 0, 0, constructionMessenger->get_lens_position( t_lensOrder[ 0 ] ) );
    G4ThreeVector relativePosition_back  = G4ThreeVector( 0, 0, constructionMessenger->get_lens_position( t_lensOrder[ t_lensOrder.size() - 1 ] ) );
    G4ThreeVector relativePosition_center = ( relativePosition_front + relativePosition_back ) / 2;

    return { relativePosition_front, relativePosition_center, relativePosition_back };
}

G4ThreeVector LensSystem::get_position_front( G4RotationMatrix* t_rotationMatrix   ,
                                              G4ThreeVector     t_translationVector,
                                              const char      * t_relativePosition  ) {
    G4String relativePosition( t_relativePosition );
    to_lower( relativePosition );

    vector< G4int > lensOrder = calculate_lensOrder();

    if( relativePosition == "front" || relativePosition == "f" ) {
        return t_translationVector;
    } else if( relativePosition == "center" || relativePosition == "c" ) {
        G4Exception( "LensSystem::get_position_front", "InvalidArgument", FatalException, "Not implemented" );
        return t_translationVector;
    } else if( relativePosition == "back" || relativePosition == "b" ) {
        return Lens::get_position_front( t_rotationMatrix, t_translationVector, "back", lensOrder[ 0 ] );
    } else
        G4Exception( "LensSystem::get_position_front", "InvalidArgument", FatalException, "Invalid relative position" );

    return G4ThreeVector();
}

// G4ThreeVector LensSystem::get_position_center( G4RotationMatrix* t_rotationMatrix   ,
//                                                G4ThreeVector     t_translationVector,
//                                                const char      * t_relativePosition  ) {
//     G4String relativePosition( t_relativePosition );
//     to_lower( relativePosition );

//     if( relativePosition == "front" || relativePosition == "f" )
//         return t_translationVector + *t_rotationMatrix * G4ThreeVector( 0, 0, get_size().z() / 2 );
//     else if( relativePosition == "center" || relativePosition == "c" )
//         return t_translationVector;
//     else if( relativePosition == "back" || relativePosition == "b" )
//         return t_translationVector + *t_rotationMatrix * G4ThreeVector( 0, 0, -get_size().z() / 2 );
//     else
//         G4Exception( "LensSystem::get_position_center", "InvalidArgument", FatalException, "Invalid relative position" );

//     return G4ThreeVector();
// }

G4ThreeVector LensSystem::get_position_front() {
    return m_translationVector + *m_rotationMatrix * m_relativePosition_front;
}

G4ThreeVector LensSystem::get_position_center() {
    return m_translationVector + *m_rotationMatrix * m_relativePosition_center;
}

G4ThreeVector LensSystem::get_position_back() {
    return m_translationVector + *m_rotationMatrix * m_relativePosition_back;
}

G4ThreeVector LensSystem::get_position( const char* t_relativePosition ) {
    G4String relativePosition( t_relativePosition );
    to_lower( relativePosition );

    if( relativePosition == "front" || relativePosition == "f" )
        return get_position_front();
    else if( relativePosition == "center" || relativePosition == "c" )
        return get_position_center();
    else if( relativePosition == "back" || relativePosition == "b" )
        return get_position_back();
    else
        G4Exception( "LensSystem::get_position", "InvalidArgument", FatalException, "Invalid relative position" );

    return G4ThreeVector();
}