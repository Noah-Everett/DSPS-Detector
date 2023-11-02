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

#include "RunAction.hh"

RunAction::RunAction() {
    G4cout << "RunAction::RunAction()" << G4endl;
    m_analysisManager = G4AnalysisManager::Instance();

    m_analysisManager->SetDefaultFileType( "root" );
    m_analysisManager->SetFileName( "output" );
    m_analysisManager->SetVerboseLevel( 1 );
    // m_analysisManager->SetActivation( true );
    m_analysisManager->SetNtupleMerging( true );
    
    G4cout << "make_histogram_photoSensor_hits(): " << t_photoSensorID << G4endl;
    G4double width = m_constructionMessenger->get_photoSensor_body_size_width();
    G4int nBins = m_outputMessenger->get_photoSensor_hits_position_binned_nBinsPerSide();
    add_histogram_2D( t_photoSensorID,
                      t_photoSensorID,
                      nBins, -width/2, width/2, 
                      nBins, -width/2, width/2 );
    
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->CreateNtuple( "photoSensor_hits", "photoSensor_hits" );
    if( m_outputMessenger->get_photoSensor_hits_position_absolute_save() ) {
        m_analysisManager->CreateNtupleDColumn( "photoSensor_hits_position_absolute_x" );
        m_analysisManager->CreateNtupleDColumn( "photoSensor_hits_position_absolute_y" );
        m_analysisManager->CreateNtupleDColumn( "photoSensor_hits_position_absolute_z" );
    }
    if( m_outputMessenger->get_photoSensor_hits_position_relative_save() ) {
        m_analysisManager->CreateNtupleDColumn( "photoSensor_hits_position_relative_x" );
        m_analysisManager->CreateNtupleDColumn( "photoSensor_hits_position_relative_y" );
        m_analysisManager->CreateNtupleDColumn( "photoSensor_hits_position_relative_z" );
    }
    if( m_outputMessenger->get_photoSensor_hits_time_save() ) {
        m_analysisManager->CreateNtupleDColumn( "photoSensor_hits_time" );
    }
    if( m_outputMessenger->get_photoSensor_hits_process_save() ) {
        m_analysisManager->CreateNtupleSColumn( "photoSensor_hits_process" );
    }
    if( m_outputMessenger->get_photoSensor_hits_photoSensorID_save() ) {
        m_analysisManager->CreateNtupleSColumn( "photoSensor_hits_photoSensorID" );
    }
    if( m_outputMessenger->get_photoSensor_hits_energy_save() ) {
        m_analysisManager->CreateNtupleDColumn( "photoSensor_hits_energy" );
    }
    m_analysisManager->FinishNtuple();

    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->CreateNtuple( "primary", "primary" );
    if( m_outputMessenger->get_primary_position_save() ) {
        m_analysisManager->CreateNtupleDColumn( "primary_position_x" );
        m_analysisManager->CreateNtupleDColumn( "primary_position_y" );
        m_analysisManager->CreateNtupleDColumn( "primary_position_z" );
    }
    if( m_outputMessenger->get_primary_direction_save() ) {
        m_analysisManager->CreateNtupleDColumn( "primary_direction_x" );
        m_analysisManager->CreateNtupleDColumn( "primary_direction_y" );
        m_analysisManager->CreateNtupleDColumn( "primary_direction_z" );
    }
    if( m_outputMessenger->get_primary_emission_photon_save() ) {
        m_analysisManager->CreateNtupleIColumn( "primary_emission_photon" );
    }
    if( m_outputMessenger->get_primary_emission_electron_save() ) {
        m_analysisManager->CreateNtupleIColumn( "primary_emission_electron" );
    }
    if( m_outputMessenger->get_primary_process_save() ) {
        m_analysisManager->CreateNtupleSColumn( "primary_process" );
    }
    if( m_outputMessenger->get_primary_time_save() ) {
        m_analysisManager->CreateNtupleDColumn( "primary_time" );
    }
    m_analysisManager->FinishNtuple();

    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->CreateNtuple( "photon", "photon" );
    if( m_outputMessenger->get_photon_length_save() ) {
        m_analysisManager->CreateNtupleDColumn( "photon_length" );
    }
    if( m_outputMessenger->get_photon_process_save() ) {
        m_analysisManager->CreateNtupleSColumn( "photon_process" );
    }
    if( m_outputMessenger->get_photon_time_save() ) {
        m_analysisManager->CreateNtupleDColumn( "photon_time" );
    }
    if( m_outputMessenger->get_photon_position_save() ) {
        m_analysisManager->CreateNtupleDColumn( "photon_position_x" );
        m_analysisManager->CreateNtupleDColumn( "photon_position_y" );
        m_analysisManager->CreateNtupleDColumn( "photon_position_z" );
    }
    if( m_outputMessenger->get_photon_direction_save() ) {
        m_analysisManager->CreateNtupleDColumn( "photon_direction_x" );
        m_analysisManager->CreateNtupleDColumn( "photon_direction_y" );
        m_analysisManager->CreateNtupleDColumn( "photon_direction_z" );
    }
    if( m_outputMessenger->get_photon_energy_save() ) {
        m_analysisManager->CreateNtupleDColumn( "photon_energy" );
    }
    if( m_outputMessenger->get_photon_volume_save() ) {
        m_analysisManager->CreateNtupleSColumn( "photon_volume" );
    }
    m_analysisManager->FinishNtuple();
}

RunAction::~RunAction() {
    m_outputManager->delete_instance();
}

void RunAction::BeginOfRunAction(const G4Run*) {
    G4cout << "RunAction::BeginOfRunAction()" << G4endl;
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->Reset();
    m_analysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run* run) {
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->Write();
    m_analysisManager->CloseFile( false );
}