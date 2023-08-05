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
    m_analysisManager->SetDefaultFileType( "root" );
    m_analysisManager->SetFileName( m_outputMessenger->get_fileName() );
    m_analysisManager->SetVerboseLevel( 1 );
    m_analysisManager->SetActivation( true );
    m_analysisManager->OpenFile();
}

OutputManager::~OutputManager() {
    m_analysisManager->Write();
    m_analysisManager->CloseFile();
}

void OutputManager::add_histogram_1D( G4String t_name, G4String t_title, G4int t_nbins, G4double t_xmin, G4double t_xmax ) {
    G4cout << "OutputManager::add_histogram_1D: " << t_name << G4endl;
    m_analysisManager->CreateH1( t_name, t_title, t_nbins, t_xmin, t_xmax );
    m_histogramIdMap.insert( std::make_pair( t_name, m_histogramIdMap.size() ) );
}

void OutputManager::add_histogram_2D( G4String t_name, G4String t_title, G4int t_nbinsx, G4double t_xmin, G4double t_xmax, G4int t_nbinsy, G4double t_ymin, G4double t_ymax ) {
    G4cout << "OutputManager::add_histogram_2D: " << t_name << G4endl;
    m_analysisManager->CreateH2( t_name, t_title, t_nbinsx, t_xmin, t_xmax, t_nbinsy, t_ymin, t_ymax );
    m_histogramIdMap.insert( std::make_pair( t_name, m_histogramIdMap.size() ) );
}

G4int OutputManager::get_histogram_id( G4String t_name ) {
    return m_histogramIdMap.at( t_name );
}