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

#include "CommandLineArgumentManager.hh"

CommandLineArgumentManager::CommandLineArgumentManager( int t_argc, char** t_argv, 
                                                        const vector< G4String >& t_arguments )
    : m_argc( t_argc ), m_argv( t_argv ), m_arguments( t_arguments ) {
    m_argLocations.resize( m_arguments.size() );
    for( int i{ 0 }; i < m_arguments.size(); i++ )
        m_argLocations[ i ] = findArgumentLocation( m_arguments[ i ] );
}

CommandLineArgumentManager::~CommandLineArgumentManager() {
    m_arguments.clear();
}

G4bool CommandLineArgumentManager::findArgument( const G4String& t_argument ) const {
    return ( findArgumentLocation( t_argument ) ) ? true : false;
}

G4bool CommandLineArgumentManager::findArgument_string( const G4String& t_argument ) const {
    G4int location = findArgumentLocation( t_argument );

    if( location != -1 )
        if( location + 1 < m_argc )
            if( m_argv[ location + 1 ][ 0 ] != '-' )
                return true;
    
    return false;
}

G4bool CommandLineArgumentManager::findArgument_int( const G4String& t_argument ) const {
    G4int location = findArgumentLocation( t_argument );

    if( location != -1 )
        if( location + 1 < m_argc )
            if( m_argv[ location + 1 ][ 0 ] != '-' )
                return true;
    
    return false;
}

G4String CommandLineArgumentManager::getArgument_string( const G4String& t_argument ) const {
    G4int location = findArgumentLocation( t_argument );

    if( location != -1 )
        if( location + 1 < m_argc )
            if( m_argv[ location + 1 ][ 0 ] != '-' )
                return m_argv[ location + 1 ];
    
    return "";
}

G4int CommandLineArgumentManager::getArgument_int( const G4String& t_argument ) const {
    G4int location = findArgumentLocation( t_argument );

    if( location != -1 )
        if( location + 1 < m_argc )
            if( m_argv[ location + 1 ][ 0 ] != '-' )
                return stoi( m_argv[ location + 1 ] );
    
    return -1;
}

G4int CommandLineArgumentManager::findArgumentLocation_argv( const G4String& t_argument ) const {
    for( int i{ 0 }; i < m_argc; i++ )
        if( m_argv[ i ] == t_argument )
            return i;
    
    return -1;
}

G4int CommandLineArgumentManager::findArgumentLocation( const G4String& t_argument ) const {
    for( int i{ 0 }; i < m_arguments.size(); i++ )
        if( m_arguments[ i ] == t_argument )
            return m_argLocations[ i ];
    
    return -1;
}

G4bool CommandLineArgumentManager::saveArgumentLocation( const G4String& t_argument ) {
    G4int location_argv = findArgumentLocation_argv( t_argument );
    G4int location = findArgumentLocation( t_argument );

    if( location != -1 && location_argv != -1 ) {
        m_argLocations[ location ] = location_argv;
        return true;
    }

    return false;
}