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

RunAction::RunAction( DetectorConstruction* t_detectorConstruction ) 
    : m_detectorConstruction( t_detectorConstruction ) {
    G4cout << "RunAction::RunAction()" << G4endl;

    if( m_detectorConstruction && !m_detectorConstruction->get_make_SDandField() ) 
        return;

    m_analysisManager = G4AnalysisManager::Instance();

    m_analysisManager->SetDefaultFileType( "root" );
    m_analysisManager->SetFileName( "output" );
    m_analysisManager->SetVerboseLevel( 1 ); //( 10 );
    m_analysisManager->SetActivation( true );
    m_analysisManager->SetNtupleMerging( true );
    m_analysisManager->SetHistoDirectoryName( "photoSensor_hits" );
    
    // Make DSPD histograms
    G4int index_histogram_1D{ 0 };
    // for( DirectionSensitivePhotoDetector* DSPD : m_detectorConstruction->get_directionSensitivePhotoDetectors() ) {
    for( G4int i = 0; i < m_constructionMessenger->get_directionSensitivePhotoDetector_amount_total(); i++ ) {
        G4String t_photoSensorID = "photoSensor_" + to_string( i );
        G4double width = m_constructionMessenger->get_photoSensor_body_size_width();
        G4int nBins = m_outputMessenger->get_photoSensor_hits_position_binned_nBinsPerSide();
        // G4cout << "ID = " << t_photoSensorID << ", nBins = " << nBins << ", width = " << width << G4endl;
        m_outputManager->add_histogram_2D( t_photoSensorID, t_photoSensorID,
                                           nBins, -width/2, width/2,
                                           nBins, -width/2, width/2 );
    }

    // Make tuples
    G4int index_tuple { 0 };

    // Make photoSensor_hits tuple
    index_tuple = m_outputManager->add_tuple_initialize( "photoSensor_hits", "photoSensor_hits" );
    if( m_outputMessenger->get_photoSensor_hits_position_absolute_save() )
        m_outputManager->add_tuple_column_3vector( "photoSensor_hits_position_absolute", index_tuple );
    if( m_outputMessenger->get_photoSensor_hits_position_relative_save() )
        m_outputManager->add_tuple_column_3vector( "photoSensor_hits_position_relative", index_tuple );
    if( m_outputMessenger->get_photoSensor_hits_time_save() )
        m_outputManager->add_tuple_column_double ( "photoSensor_hits_time", index_tuple );
    if( m_outputMessenger->get_photoSensor_hits_process_save() )
        m_outputManager->add_tuple_column_string ( "photoSensor_hits_process", index_tuple );
    if( m_outputMessenger->get_photoSensor_hits_photoSensorID_save() )
        m_outputManager->add_tuple_column_string ( "photoSensor_hits_photoSensorID", index_tuple );
    if( m_outputMessenger->get_photoSensor_hits_energy_save() )
        m_outputManager->add_tuple_column_double ( "photoSensor_hits_energy", index_tuple );
    m_outputManager->add_tuple_finalize();

    // Make primary tuple
    index_tuple = m_outputManager->add_tuple_initialize( "primary", "primary" );
    if( m_outputMessenger->get_primary_position_save() )
        m_outputManager->add_tuple_column_3vector( "primary_position", index_tuple );
    if( m_outputMessenger->get_primary_direction_save() )
        m_outputManager->add_tuple_column_3vector( "primary_direction", index_tuple );
    if( m_outputMessenger->get_primary_process_save() )
        m_outputManager->add_tuple_column_string( "primary_process", index_tuple );
    if( m_outputMessenger->get_primary_time_save() )
        m_outputManager->add_tuple_column_double( "primary_time", index_tuple );
    m_outputManager->add_tuple_finalize();

    // Make photon tuple
    index_tuple = m_outputManager->add_tuple_initialize( "photon", "photon" );
    if( m_outputMessenger->get_photon_length_save() )
        m_outputManager->add_tuple_column_double( "photon_length", index_tuple );
    if( m_outputMessenger->get_photon_process_save() )
        m_outputManager->add_tuple_column_string( "photon_process", index_tuple );
    if( m_outputMessenger->get_photon_time_save() )
        m_outputManager->add_tuple_column_double( "photon_time", index_tuple );
    if( m_outputMessenger->get_photon_position_save() )
        m_outputManager->add_tuple_column_3vector( "photon_position", index_tuple );
    if( m_outputMessenger->get_photon_direction_save() )
        m_outputManager->add_tuple_column_3vector( "photon_direction", index_tuple );
    if( m_outputMessenger->get_photon_energy_save() )
        m_outputManager->add_tuple_column_double( "photon_energy", index_tuple );
    if( m_outputMessenger->get_photon_volume_save() )
        m_outputManager->add_tuple_column_string( "photon_volume", index_tuple );
    m_outputManager->add_tuple_finalize();
}

RunAction::~RunAction() {
    G4cout << "RunAction::~RunAction()" << G4endl;
    delete m_outputManager;
}

void RunAction::BeginOfRunAction( const G4Run* t_run ) {
    G4cout << "RunAction::BeginOfRunAction()" << G4endl;
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->Reset();
    m_analysisManager->OpenFile();
}

void RunAction::EndOfRunAction( const G4Run* run ) {
    G4cout << "RunAction::EndOfRunAction()" << G4endl;
    m_analysisManager = G4AnalysisManager::Instance();
    m_analysisManager->Write();
    m_analysisManager->CloseFile( false );
}

OutputManager* RunAction::get_outputManager() {
    return m_outputManager;
}