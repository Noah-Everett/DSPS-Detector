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

void OutputManager::make_histogram_photoSensor_hits( G4String t_photoSensorID ) {
    G4cout << "make_histogram_photoSensor_hits(): " << t_photoSensorID << G4endl;
    G4double width = m_constructionMessenger->get_photoSensor_body_size_width();
    G4int nBins = m_outputMessenger->get_photoSensor_hits_position_binned_nBinsPerSide();
    add_histogram_2D( t_photoSensorID,
                      t_photoSensorID,
                      nBins, -width/2, width/2, 
                      nBins, -width/2, width/2 );
}

void OutputManager::make_tuple_photoSensor_hits() {
    add_tuple_initialize( "photoSensor_hits", "photoSensor_hits" );
    if( m_outputMessenger->get_photoSensor_hits_position_absolute_save() ) {
        add_tuple_column_3vector( "photoSensor_hits_position_absolute" );
    }
    if( m_outputMessenger->get_photoSensor_hits_position_relative_save() ) {
        add_tuple_column_3vector( "photoSensor_hits_position_relative" );
    }
    if( m_outputMessenger->get_photoSensor_hits_time_save() ) {
        add_tuple_column_double( "photoSensor_hits_time" );
    }
    if( m_outputMessenger->get_photoSensor_hits_process_save() ) {
        add_tuple_column_string( "photoSensor_hits_process" );
    }
    if( m_outputMessenger->get_photoSensor_hits_photoSensorID_save() ) {
        add_tuple_column_string( "photoSensor_hits_photoSensorID" );
    }
    if( m_outputMessenger->get_photoSensor_hits_energy_save() ) {
        add_tuple_column_double( "photoSensor_hits_energy" );
    }
    add_tuple_finalize();
}

void OutputManager::make_tuple_primary() {
    add_tuple_initialize( "primary", "primary" );
    if( m_outputMessenger->get_primary_position_save() ) {
        add_tuple_column_3vector( "primary_position" );
    }
    if( m_outputMessenger->get_primary_direction_save() ) {
        add_tuple_column_3vector( "primary_direction" );
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

void OutputManager::make_tuple_photon() {
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
    if( m_outputMessenger->get_photon_position_save() ) {
        add_tuple_column_3vector( "photon_position" );
    }
    if( m_outputMessenger->get_photon_direction_save() ) {
        add_tuple_column_3vector( "photon_direction" );
    }
    if( m_outputMessenger->get_photon_energy_save() ) {
        add_tuple_column_double( "photon_energy" );
    }
    if( m_outputMessenger->get_photon_volume_save() ) {
        add_tuple_column_string( "photon_volume" );
    }
    add_tuple_finalize();
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
    m_tuple_id.insert( std::pair<G4String,G4int>( t_name, m_tuple_id.size() ) );
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

G4int OutputManager::get_histogram_1D_id( G4String t_name ) {
    m_analysisManager = G4AnalysisManager::Instance();
    return m_analysisManager->GetH1Id( t_name );
}

G4int OutputManager::get_histogram_2D_id( G4String t_name ) {
    m_analysisManager = G4AnalysisManager::Instance();
    return m_analysisManager->GetH2Id( t_name );
}

G4int OutputManager::get_tuple_id( G4String t_name ) {
    m_analysisManager = G4AnalysisManager::Instance();
    return m_tuple_id[ t_name ];
}

void OutputManager::save_step_photoSensor_hits( const G4Step* t_step, G4String t_photoSensorID, G4ThreeVector t_position, G4RotationMatrix t_rotation, G4bool t_verbose = false ) {
    G4cout << "save_step_photoSensor_hits(): " << t_photoSensorID << G4endl;
    m_index_histogram = get_histogram_2D_id( t_photoSensorID );
    m_index_histogram = m_analysisManager->GetH2Id( t_photoSensorID );
    if( t_verbose )
        G4cout << "m_outputMessenger->get_photoSensor_hits_position_binned_save(): " << m_outputMessenger->get_photoSensor_hits_position_binned_save() << G4endl;
    G4ThreeVector relativePosition = t_step->GetPostStepPoint()->GetPosition() - t_position;
    if( t_verbose ) {
        G4cout << "Before rotation:" << G4endl;
        G4cout << "relativePosition.x(): " << relativePosition.x() << G4endl;
        G4cout << "relativePosition.y(): " << relativePosition.y() << G4endl;
        G4cout << "relativePosition.z(): " << relativePosition.z() << G4endl;
    }
    relativePosition = t_rotation * relativePosition;
    if( t_verbose ) {
        G4cout << "After rotation:" << G4endl;
        G4cout << "relativePosition.x(): " << relativePosition.x() << G4endl;
        G4cout << "relativePosition.y(): " << relativePosition.y() << G4endl;
        G4cout << "relativePosition.z(): " << relativePosition.z() << G4endl;
    }
    if( m_outputMessenger->get_photoSensor_hits_position_binned_save() ) {
        if( t_verbose ) {
            G4cout << "save_step_photoSensor_hits(): " << t_photoSensorID << " binned" << G4endl;
            G4cout << "m_index_histogram: " << m_index_histogram << G4endl;
        }
        m_analysisManager->FillH2( m_index_histogram, 
                                   relativePosition.x(), 
                                   relativePosition.y() );
    }

    m_index_tuple  = get_tuple_id( "photoSensor_hits" );
    m_index_column = 0;
    if( m_outputMessenger->get_photoSensor_hits_position_absolute_save() ) {
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetPosition().x() );
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetPosition().y() );
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetPosition().z() );
    }
    if( m_outputMessenger->get_photoSensor_hits_position_relative_save() ) {
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, relativePosition.x() );
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, relativePosition.y() );
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, relativePosition.z() );
    }
    if( m_outputMessenger->get_photoSensor_hits_time_save() ) {
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetGlobalTime() );
    }
    if( m_outputMessenger->get_photoSensor_hits_process_save() ) {
        m_analysisManager->FillNtupleSColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() );
    }
    if( m_outputMessenger->get_photoSensor_hits_photoSensorID_save() ) {
        m_analysisManager->FillNtupleSColumn( m_index_tuple, m_index_column++, t_photoSensorID );
    }
    if( m_outputMessenger->get_photoSensor_hits_energy_save() ) {
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetTotalEnergy() );
    }
    m_analysisManager->AddNtupleRow( m_index_tuple );
}

void OutputManager::save_step_primary( const G4Step* t_step ) {
    m_index_tuple  = get_tuple_id( "primary" );
    m_index_column = 0;
    if( m_outputMessenger->get_primary_position_save() ) {
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetTrack()->GetPosition().x() );
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetTrack()->GetPosition().y() );
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetTrack()->GetPosition().z() );
    }
    if( m_outputMessenger->get_primary_direction_save() ) {
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetTrack()->GetMomentumDirection().x() );
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetTrack()->GetMomentumDirection().y() );
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetTrack()->GetMomentumDirection().z() );
    }
    if( m_outputMessenger->get_primary_emission_photon_save() ) {
        m_analysisManager->FillNtupleIColumn( m_index_tuple, m_index_column++, t_step->GetTrack()->GetTrackID() ); // clearly wrong
    }
    if( m_outputMessenger->get_primary_emission_electron_save() ) {
        m_analysisManager->FillNtupleIColumn( m_index_tuple, m_index_column++, t_step->GetTrack()->GetTrackID() ); // clearly wrong
    }
    if( m_outputMessenger->get_primary_process_save() ) {
        m_analysisManager->FillNtupleSColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() );
    }
    if( m_outputMessenger->get_primary_time_save() ) {
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetTrack()->GetGlobalTime() );
    }
    m_analysisManager->AddNtupleRow( m_index_tuple );
}

void OutputManager::save_step_photon( const G4Step* t_step ) {
    m_index_tuple  = get_tuple_id( "photon" );
    m_index_column = 0;
    if( m_outputMessenger->get_photon_length_save() ) {
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetStepLength() );
    }
    if( m_outputMessenger->get_photon_process_save() ) {
        m_analysisManager->FillNtupleSColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() );
    }
    if( m_outputMessenger->get_photon_time_save() ) {
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetGlobalTime() );
    }
    if( m_outputMessenger->get_photon_position_save() ) {
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetPosition().x() );
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetPosition().y() );
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetPosition().z() );
    }
    if( m_outputMessenger->get_photon_direction_save() ) {
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetMomentumDirection().x() );
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetMomentumDirection().y() );
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetMomentumDirection().z() );
    }
    if( m_outputMessenger->get_photon_energy_save() ) {
        m_analysisManager->FillNtupleDColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetTotalEnergy() );
    }
    if( m_outputMessenger->get_photon_volume_save() ) {
        m_analysisManager->FillNtupleSColumn( m_index_tuple, m_index_column++, t_step->GetPostStepPoint()->GetPhysicalVolume()->GetName() );
    }
    m_analysisManager->AddNtupleRow( m_index_tuple );
}

void OutputManager::reset() {
    m_histogram_1D_id.clear();
    m_histogram_2D_id.clear();
    m_tuple_id       .clear();
}