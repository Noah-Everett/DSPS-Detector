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

#include "OutputManager.hh"

// OutputManager* OutputManager::m_instance{ nullptr };

// OutputManager* OutputManager::get_instance( G4bool t_new ) {
//     if ( t_new || !m_instance )
//         m_instance = new OutputManager();
//     return m_instance;
// }

// void OutputManager::delete_instance() {
//     if ( m_instance ) {
//         delete m_instance;
//         m_instance = nullptr;
//     }
// }

OutputManager::OutputManager() {
}

OutputManager::~OutputManager() {
}

G4int OutputManager::add_histogram_1D( const G4String& t_name , const G4String& t_title,
                                             G4int     t_nBins,       G4double  t_x_min, G4double  t_x_max ) {
    G4cout << "OutputManager::add_histogram_1D: " << t_name << G4endl;
    if( m_histogram_1D_IDs.find( t_name ) == m_histogram_1D_IDs.end() ) {
        m_analysisManager = G4AnalysisManager::Instance();
        G4int ID = m_analysisManager->CreateH1( t_name, t_title, t_nBins, t_x_min, t_x_max );
        if( ID == kInvalidId )
            G4Exception( "OutputManager::add_histogram_1D", "Error", FatalException, "Histogram already exists but is not in map" );
        m_histogram_1D_IDs.insert( { t_name, ID } ); 
        return ID;
    }
    return kInvalidId;
}

G4int OutputManager::add_histogram_2D( const G4String& t_name   , const G4String& t_title, 
                                             G4int     t_nBins_x,       G4double  t_x_min, G4double t_x_max, 
                                             G4int     t_nBins_y,       G4double  t_y_min, G4double t_y_max ) {
    G4cout << "OutputManager::add_histogram_2D: " << t_name << G4endl;
    if( m_histogram_2D_IDs.find( t_name ) == m_histogram_2D_IDs.end() ) {
        m_analysisManager = G4AnalysisManager::Instance();
        G4int ID = m_analysisManager->CreateH2( t_name, t_title, t_nBins_x, t_x_min, t_x_max, t_nBins_y, t_y_min, t_y_max );
        if( ID == kInvalidId )
            G4Exception( "OutputManager::add_histogram_2D", "Error", FatalException, "Histogram already exists but is not in map" );
        m_histogram_2D_IDs.insert( { t_name, ID } );
        return ID;
    }
    return kInvalidId;
}

G4int OutputManager::add_tuple_initialize( const G4String& t_name, const G4String& t_title ) {
    G4cout << "OutputManager::add_tuple_initialize: " << t_name << G4endl;
    if( m_tuple_IDs.find( t_name ) == m_tuple_IDs.end() ) {
        m_analysisManager = G4AnalysisManager::Instance();
        G4int ID = m_analysisManager->CreateNtuple( t_name, t_title );
        if( ID == kInvalidId )
            G4Exception( "OutputManager::add_tuple_initialize", "Error", FatalException, "Tuple already exists but is not in map" );
        m_tuple_IDs.insert( { t_name, ID } );
        return ID;
    }
    return kInvalidId;
}

void OutputManager::add_tuple_finalize() {
    G4cout << "OutputManager::add_tuple_finalize" << G4endl;
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->FinishNtuple();
}

pair< G4int, G4int > OutputManager::add_tuple_column_integer( const G4String& t_name, G4int t_index_tuple ) {
    G4cout << "OutputManager::add_tuple_column_integer: " << t_name << G4endl;
    if( m_tuple_column_IDs.find( t_name ) == m_tuple_column_IDs.end() ) {
        m_analysisManager = G4AnalysisManager::Instance();
        G4int ID = m_analysisManager->CreateNtupleIColumn( t_name );
        if( ID == kInvalidId )
            G4Exception( "OutputManager::add_tuple_column_integer", "Error", FatalException, "Tuple column already exists but is not in map" );
        m_tuple_column_IDs.insert( { t_name, { t_index_tuple, ID } } );
        return { t_index_tuple, ID };
    }
    return { kInvalidId, kInvalidId };
}

pair< G4int, G4int > OutputManager::add_tuple_column_double( const G4String& t_name, G4int t_index_tuple ) {
    G4cout << "OutputManager::add_tuple_column_double: " << t_name << G4endl;
    if( m_tuple_column_IDs.find( t_name ) == m_tuple_column_IDs.end() ) {
        m_analysisManager = G4AnalysisManager::Instance();
        G4int ID = m_analysisManager->CreateNtupleDColumn( t_name );
        if( ID == kInvalidId )
            G4Exception( "OutputManager::add_tuple_column_double", "Error", FatalException, "Tuple column already exists but is not in map" );
        m_tuple_column_IDs.insert( { t_name, { t_index_tuple, ID } } );
        return { t_index_tuple, ID };
    }
    return { kInvalidId, kInvalidId };
}

pair< G4int, G4int > OutputManager::add_tuple_column_3vector( const G4String& t_name, G4int t_index_tuple ) { 
    G4cout << "OutputManager::add_tuple_column_3vector: " << t_name << G4endl;
    char axis[ 3 ] = { 'x', 'y', 'z' };
    pair< G4int, G4int > ID[ 3 ];
    for( G4int i = 0; i < 3; i++ ) {
        G4String name = t_name + "_" + axis[ i ];
        ID[ i ] = add_tuple_column_double( name, t_index_tuple );
        if( ID[ i ].first == kInvalidId || ID[ i ].second == kInvalidId )
            G4Exception( "OutputManager::add_tuple_column_3vector", "Error", FatalException, "Tuple column already exists but is not in map" );
    }
    return ID[0];
}

pair< G4int, G4int > OutputManager::add_tuple_column_string( const G4String& t_name, G4int t_index_tuple ) {
    G4cout << "OutputManager::add_tuple_column_string: " << t_name << G4endl;
    if( m_tuple_column_IDs.find( t_name ) == m_tuple_column_IDs.end() ) {
        m_analysisManager = G4AnalysisManager::Instance();
        G4int ID = m_analysisManager->CreateNtupleSColumn( t_name );
        if( ID == kInvalidId )
            G4Exception( "OutputManager::add_tuple_column_string", "Error", FatalException, "Tuple column already exists but is not in map" );
        m_tuple_column_IDs.insert( { t_name, { t_index_tuple, ID } } );
        return { t_index_tuple, ID };
    }
    return { kInvalidId, kInvalidId };
}

pair< G4int, G4int > OutputManager::add_tuple_column_boolean( const G4String& t_name, G4int t_index_tuple ) {
    G4cout << "OutputManager::add_tuple_column_boolean: " << t_name << G4endl;
    if( m_tuple_column_IDs.find( t_name ) == m_tuple_column_IDs.end() ) {
        m_analysisManager = G4AnalysisManager::Instance();
        G4int ID = m_analysisManager->CreateNtupleSColumn( t_name );
        if( ID == kInvalidId )
            G4Exception( "OutputManager::add_tuple_column_boolean", "Error", FatalException, "Tuple column already exists but is not in map" );
        m_tuple_column_IDs.insert( { t_name, { t_index_tuple, ID } } );
        return { t_index_tuple, ID };
    }
    return { kInvalidId, kInvalidId };
}

G4int OutputManager::get_histogram_1D_ID( const G4String& t_name ) {
    if( m_histogram_1D_IDs.find( t_name ) != m_histogram_1D_IDs.end() )
        return m_histogram_1D_IDs.at( t_name );
    else 
        return kInvalidId;
}

G4int OutputManager::get_histogram_2D_ID( const G4String& t_name ) {
    if( m_histogram_2D_IDs.find( t_name ) != m_histogram_2D_IDs.end() )
        return m_histogram_2D_IDs.at( t_name );
    else 
        return kInvalidId;
}

G4int OutputManager::get_tuple_ID( const G4String& t_name ) {
    if( m_tuple_IDs.find( t_name ) != m_tuple_IDs.end() )
        return m_tuple_IDs.at( t_name );
    else
        return kInvalidId;
}

pair< G4int, G4int > OutputManager::get_tuple_column_ID( const G4String& t_name ) {
    if( m_tuple_column_IDs.find( t_name ) != m_tuple_column_IDs.end() )
        return m_tuple_column_IDs.at( t_name );
    else if( m_tuple_column_IDs.find( t_name + "_x" ) != m_tuple_column_IDs.end() )
        return m_tuple_column_IDs.at( t_name + "_x" );
    else
        return { kInvalidId, kInvalidId };
}

void OutputManager::reset() {
    m_histogram_1D_IDs.clear();
    m_histogram_2D_IDs.clear();
    m_tuple_IDs       .clear();
    m_tuple_column_IDs.clear();
}

G4int OutputManager::get_tuple_ID( const vector< G4String >& t_names ) {
    vector< G4int > IDs;
    for( G4String name : t_names )
        IDs.push_back( get_tuple_column_ID( name ).first );
    
    return get_tuple_ID( IDs );
}

G4int OutputManager::get_tuple_ID( const vector< pair< G4int, G4int > >& t_IDs ) {
    vector< G4int > IDs;
    for( pair< G4int, G4int > ID : t_IDs )
        IDs.push_back( ID.first );
    
    return get_tuple_ID( IDs );
}

G4int OutputManager::get_tuple_ID( const vector< G4int >& t_IDs ) {
    vector< G4int > temp = t_IDs;
    for( vector< G4int >::iterator it = temp.begin(); it != temp.end(); it++ )
        if( *it == kInvalidId )
            temp.erase( it );

    if( temp.size() == 0 )
        return kInvalidId;

    G4int firstID = temp[ 0 ];
    for( G4int ID : temp )
        if( ID != firstID )
            return kInvalidId;
    
    return firstID;
}

G4bool OutputManager::fill_histogram_1D( G4int t_ID, G4double t_value, G4double t_weight = 1.0 ) {
    if( t_ID == kInvalidId )
        return false;

    m_analysisManager = G4AnalysisManager::Instance();
    return m_analysisManager->FillH1( t_ID, t_value, t_weight );
}

G4bool OutputManager::fill_histogram_1D( const G4String& t_name, G4double t_value, G4double t_weight = 1.0 ) {
    return fill_histogram_1D( get_histogram_1D_ID( t_name ), t_value, t_weight );
}

G4bool OutputManager::fill_histogram_2D( G4int t_ID, G4double t_value_x, G4double t_value_y, G4double t_weight = 1.0 ) {
    if( t_ID == kInvalidId )
        return false;

    m_analysisManager = G4AnalysisManager::Instance();
    return m_analysisManager->FillH2( t_ID, t_value_x, t_value_y, t_weight );
}

G4bool OutputManager::fill_histogram_2D( const G4String& t_name, G4double t_value_x, G4double t_value_y, G4double t_weight = 1.0 ) {
    return fill_histogram_2D( get_histogram_2D_ID( t_name ), t_value_x, t_value_y, t_weight );
}

G4bool OutputManager::fill_tuple_column_integer( pair< G4int, G4int > t_ID, G4int t_value ) {
    if( t_ID.first == kInvalidId || t_ID.second == kInvalidId )
        return false;

    m_analysisManager = G4AnalysisManager::Instance();
    return m_analysisManager->FillNtupleIColumn( t_ID.first, t_ID.second, t_value );
}

G4bool OutputManager::fill_tuple_column_integer( const G4String& t_name, G4int t_value ) {
    return fill_tuple_column_integer( get_tuple_column_ID( t_name ), t_value );
}

G4bool OutputManager::fill_tuple_column_double( pair< G4int, G4int > t_ID, G4double t_value ) {
    if( t_ID.first == kInvalidId || t_ID.second == kInvalidId )
        return false;

    m_analysisManager = G4AnalysisManager::Instance();
    return m_analysisManager->FillNtupleDColumn( t_ID.first, t_ID.second, t_value );
}

G4bool OutputManager::fill_tuple_column_double( const G4String& t_name, G4double t_value ) {
    return fill_tuple_column_double( get_tuple_column_ID( t_name ), t_value );
}

G4bool OutputManager::fill_tuple_column_3vector( pair< G4int, G4int > t_ID, const G4ThreeVector& t_value ) {
    if( t_ID.first == kInvalidId || t_ID.second == kInvalidId )
        return false;

    m_analysisManager = G4AnalysisManager::Instance();
    return fill_tuple_column_double( t_ID, t_value.x()                                     ) &&
           fill_tuple_column_double( make_pair( t_ID.first, t_ID.second + 1 ), t_value.y() ) &&
           fill_tuple_column_double( make_pair( t_ID.first, t_ID.second + 2 ), t_value.z() );
}

G4bool OutputManager::fill_tuple_column_3vector( const G4String& t_name, const G4ThreeVector& t_value ) {
    return fill_tuple_column_3vector( get_tuple_column_ID( t_name ), t_value );
}

G4bool OutputManager::fill_tuple_column_string( pair< G4int, G4int > t_ID, const G4String& t_value ) {
    if( t_ID.first == kInvalidId || t_ID.second == kInvalidId )
        return false;

    m_analysisManager = G4AnalysisManager::Instance();
    return m_analysisManager->FillNtupleSColumn( t_ID.first, t_ID.second, t_value );
}

G4bool OutputManager::fill_tuple_column_string( const G4String& t_name, const G4String& t_value ) {
    return fill_tuple_column_string( get_tuple_column_ID( t_name ), t_value );
}

G4bool OutputManager::fill_tuple_column_boolean( pair< G4int, G4int > t_ID, G4bool t_value ) {
    if( t_ID.first == kInvalidId || t_ID.second == kInvalidId )
        return false;

    m_analysisManager = G4AnalysisManager::Instance();
    return m_analysisManager->FillNtupleIColumn( t_ID.first, t_ID.second, t_value );
}

G4bool OutputManager::fill_tuple_column_boolean( const G4String& t_name, G4bool t_value ) {
    return fill_tuple_column_boolean( get_tuple_column_ID( t_name ), t_value );
}

G4bool OutputManager::fill_tuple_column( G4int t_ID ) {
    if( t_ID == kInvalidId )
        return false;

    m_analysisManager = G4AnalysisManager::Instance();
    return m_analysisManager->AddNtupleRow( t_ID );
}

G4bool OutputManager::fill_tuple_column( const G4String& t_name ) {
    return fill_tuple_column( get_tuple_ID( t_name ) );
}