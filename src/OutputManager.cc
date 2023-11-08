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

pair< G4int, G4int > OutputManager::add_tuple_column_Integer( const G4String& t_name, G4int t_index_tuple ) {
    G4cout << "OutputManager::add_tuple_column_Integer: " << t_name << G4endl;
    if( m_tuple_column_IDs.find( t_name ) == m_tuple_column_IDs.end() ) {
        m_analysisManager = G4AnalysisManager::Instance();
        G4int ID = m_analysisManager->CreateNtupleIColumn( t_name );
        if( ID == kInvalidId )
            G4Exception( "OutputManager::add_tuple_column_Integer", "Error", FatalException, "Tuple column already exists but is not in map" );
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