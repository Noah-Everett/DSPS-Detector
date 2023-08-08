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

OutputManager* OutputManager::m_instance{ nullptr };

OutputManager* OutputManager::get_instance() {
    if ( !m_instance ) {
        m_instance = new OutputManager();
    }
    return m_instance;
}

void OutputManager::delete_instance() {
    if ( m_instance ) {
        delete m_instance;
        m_instance = nullptr;
    }
}

OutputManager::OutputManager() {
}

OutputManager::~OutputManager() {
}

void OutputManager::make_histograms() {
    if( m_outputMessenger->get_photoSensor_hits_position_binned_save() ) {
        G4double width = m_constructionMessenger->get_photoSensor_size_width();
        G4int nBins = width / m_outputMessenger->get_photoSensor_hits_position_binned_nBinsPerSide();
        add_histogram_2D( "photoSensor_hits_position_binned", 
                          "photoSensor_hits_position_binned", 
                          nBins, -width/2, width/2, 
                          nBins, -width/2, width/2 );
    }
}

void OutputManager::make_tuples() {
    if( m_outputMessenger->get_photoSensor_hits_position_save() ||
        m_outputMessenger->get_photoSensor_hits_time_save    () ||
        m_outputMessenger->get_photoSensor_hits_process_save () ) {
        add_tuple_initialize( "photoSensor_hits", "photoSensor_hits" );
        if( m_outputMessenger->get_photoSensor_hits_position_save() ) {
            add_tuple_column_double( "photoSensor_hits_position_x" );
            add_tuple_column_double( "photoSensor_hits_position_y" );
        }
        if( m_outputMessenger->get_photoSensor_hits_time_save() ) {
            add_tuple_column_double( "photoSensor_hits_time" );
        }
        if( m_outputMessenger->get_photoSensor_hits_process_save() ) {
            add_tuple_column_string( "photoSensor_hits_process" );
        }
        add_tuple_finalize();
    }
    if( m_outputMessenger->get_primary_position_save         () ||
        m_outputMessenger->get_primary_emission_photon_save  () ||
        m_outputMessenger->get_primary_emission_electron_save() ||
        m_outputMessenger->get_primary_process_save          () ||
        m_outputMessenger->get_primary_time_save             () ) {
        add_tuple_initialize( "primary", "primary" );
        if( m_outputMessenger->get_primary_position_save() ) {
            add_tuple_column_3vector( "primary_position" );
        }
        if( m_outputMessenger->get_primary_emission_photon_save() ) {
            add_tuple_column_boolean( "primary_emission_photon" );
        }
        if( m_outputMessenger->get_primary_emission_electron_save() ) {
            add_tuple_column_boolean( "primary_emission_electron" );
        }
        if( m_outputMessenger->get_primary_process_save() ) {
            add_tuple_column_string( "primary_process" );
        }
        if( m_outputMessenger->get_primary_time_save() ) {
            add_tuple_column_double( "primary_time" );
        }
        add_tuple_finalize();
    }
    if( m_outputMessenger->get_photon_length_save () ||
        m_outputMessenger->get_photon_process_save() ||
        m_outputMessenger->get_photon_time_save   () ) {
        add_tuple_initialize( "photon", "photon" );
        if( m_outputMessenger->get_photon_length_save() ) {
            add_tuple_column_double( "photon_length" );
        }
        if( m_outputMessenger->get_photon_process_save() ) {
            add_tuple_column_string( "photon_process" );
        }
        if( m_outputMessenger->get_photon_time_save() ) {
            add_tuple_column_double( "photon_time" );
        }
        add_tuple_finalize();
    }
}

void OutputManager::add_histogram_1D( G4String t_name, G4String t_title, G4int t_nbins, G4double t_xmin, G4double t_xmax ) {
    G4cout << "OutputManager::add_histogram_1D: " << t_name << G4endl;
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->CreateH1( t_name, t_title, t_nbins, t_xmin, t_xmax );
}

void OutputManager::add_histogram_2D( G4String t_name, G4String t_title, G4int t_nbinsx, G4double t_xmin, G4double t_xmax, G4int t_nbinsy, G4double t_ymin, G4double t_ymax ) {
    G4cout << "OutputManager::add_histogram_2D: " << t_name << G4endl;
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->CreateH2( t_name, t_title, t_nbinsx, t_xmin, t_xmax, t_nbinsy, t_ymin, t_ymax );
}

void OutputManager::add_tuple_initialize( G4String t_name, G4String t_title ) {
    G4cout << "OutputManager::add_tuple_initialize: " << t_name << G4endl;
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->CreateNtuple( t_name, t_title );
}

void OutputManager::add_tuple_finalize() {
    G4cout << "OutputManager::add_tuple_finalize" << G4endl;
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->FinishNtuple();
}

void OutputManager::add_tuple_column_intiger( G4String t_name ) {
    G4cout << "OutputManager::add_tuple_column_intiger: " << t_name << G4endl;
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->CreateNtupleIColumn( t_name );
}

void OutputManager::add_tuple_column_double( G4String t_name ) {
    G4cout << "OutputManager::add_tuple_column_double: " << t_name << G4endl;
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->CreateNtupleDColumn( t_name );
}

void OutputManager::add_tuple_column_3vector( G4String t_name ) {
    G4cout << "OutputManager::add_tuple_column_3vector: " << t_name << G4endl;
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->CreateNtupleDColumn( t_name + "_x" );
    m_analysisManager->CreateNtupleDColumn( t_name + "_y" );
    m_analysisManager->CreateNtupleDColumn( t_name + "_z" );
}

void OutputManager::add_tuple_column_string( G4String t_name ) {
    G4cout << "OutputManager::add_tuple_column_string: " << t_name << G4endl;
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->CreateNtupleSColumn( t_name );
}

void OutputManager::add_tuple_column_boolean( G4String t_name ) {
    G4cout << "OutputManager::add_tuple_column_boolean: " << t_name << G4endl;
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->CreateNtupleIColumn( t_name );
}

G4int OutputManager::get_histogram_id( G4String t_name ) {
    return m_histogramIdMap[ t_name ];
}

void OutputManager::save_primary_track_step( const G4Step* t_step ) {
    m_analysisManager = G4AnalysisManager::Instance();

    G4int id_xy = get_histogram_id( "primary_position_xy" );
    G4int id_xz = get_histogram_id( "primary_position_xz" );
    G4int id_yz = get_histogram_id( "primary_position_yz" );

    G4double x = t_step->GetPreStepPoint()->GetPosition().x();
    G4double y = t_step->GetPreStepPoint()->GetPosition().y();
    G4double z = t_step->GetPreStepPoint()->GetPosition().z();

    m_analysisManager->FillH2( id_xy, x, y );
    m_analysisManager->FillH2( id_xz, x, z );
    m_analysisManager->FillH2( id_yz, y, z );
}