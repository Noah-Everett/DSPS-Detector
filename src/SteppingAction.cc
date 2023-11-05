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

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "Maps.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProcessTable.hh"

#include <cmath>

SteppingAction::SteppingAction( RunAction* t_runAction ) {
    m_runAction     = t_runAction;
    m_outputManager = m_runAction->get_outputManager();

    photon_length_ID       = m_outputManager->get_tuple_column_ID( "photon_length"       );
    photon_process_ID      = m_outputManager->get_tuple_column_ID( "photon_process"      );
    photon_time_ID         = m_outputManager->get_tuple_column_ID( "photon_time"         );
    photon_position_x_ID   = m_outputManager->get_tuple_column_ID( "photon_position_x"   );
    photon_position_y_ID   = m_outputManager->get_tuple_column_ID( "photon_position_y"   );
    photon_position_z_ID   = m_outputManager->get_tuple_column_ID( "photon_position_z"   );
    photon_direction_x_ID  = m_outputManager->get_tuple_column_ID( "photon_direction_x"  );
    photon_direction_y_ID  = m_outputManager->get_tuple_column_ID( "photon_direction_y"  );
    photon_direction_z_ID  = m_outputManager->get_tuple_column_ID( "photon_direction_z"  );
    photon_energy_ID       = m_outputManager->get_tuple_column_ID( "photon_energy"       );
    photon_volume_ID       = m_outputManager->get_tuple_column_ID( "photon_volume"       );

    primary_position_x_ID  = m_outputManager->get_tuple_column_ID( "primary_position_x"  );
    primary_position_y_ID  = m_outputManager->get_tuple_column_ID( "primary_position_y"  );
    primary_position_z_ID  = m_outputManager->get_tuple_column_ID( "primary_position_z"  );
    primary_direction_x_ID = m_outputManager->get_tuple_column_ID( "primary_direction_x" );
    primary_direction_y_ID = m_outputManager->get_tuple_column_ID( "primary_direction_y" );
    primary_direction_z_ID = m_outputManager->get_tuple_column_ID( "primary_direction_z" );
    primary_process_ID     = m_outputManager->get_tuple_column_ID( "primary_process"     );
    primary_time_ID        = m_outputManager->get_tuple_column_ID( "primary_time"        );
}

SteppingAction::~SteppingAction() {
}

void SteppingAction::UserSteppingAction( const G4Step* t_step ) {
    if( !t_step->GetPostStepPoint()->GetPhysicalVolume() ) {
        t_step->GetTrack()->SetTrackStatus( fKillTrackAndSecondaries );
        return;
    }

    m_analysisManager = G4AnalysisManager::Instance();
    if( abs( t_step->GetTrack()->GetDefinition()->GetPDGEncoding() ) == 0  || 
        abs( t_step->GetTrack()->GetDefinition()->GetPDGEncoding() ) == 22    ) {
        if( photon_length_ID     .second >= 0 ) m_analysisManager->FillNtupleDColumn( photon_length_ID     .first, photon_length_ID     .second, t_step->GetStepLength()                                               );
        if( photon_process_ID    .second >= 0 ) m_analysisManager->FillNtupleSColumn( photon_process_ID    .first, photon_process_ID    .second, t_step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() );
        if( photon_time_ID       .second >= 0 ) m_analysisManager->FillNtupleDColumn( photon_time_ID       .first, photon_time_ID       .second, t_step->GetPostStepPoint()->GetGlobalTime()                           );
        if( photon_position_x_ID .second >= 0 ) m_analysisManager->FillNtupleDColumn( photon_position_x_ID .first, photon_position_x_ID .second, t_step->GetPostStepPoint()->GetPosition().x()                         );
        if( photon_position_y_ID .second >= 0 ) m_analysisManager->FillNtupleDColumn( photon_position_y_ID .first, photon_position_y_ID .second, t_step->GetPostStepPoint()->GetPosition().y()                         );
        if( photon_position_z_ID .second >= 0 ) m_analysisManager->FillNtupleDColumn( photon_position_z_ID .first, photon_position_z_ID .second, t_step->GetPostStepPoint()->GetPosition().z()                         );
        if( photon_direction_x_ID.second >= 0 ) m_analysisManager->FillNtupleDColumn( photon_direction_x_ID.first, photon_direction_x_ID.second, t_step->GetPostStepPoint()->GetMomentumDirection().x()                );
        if( photon_direction_y_ID.second >= 0 ) m_analysisManager->FillNtupleDColumn( photon_direction_y_ID.first, photon_direction_y_ID.second, t_step->GetPostStepPoint()->GetMomentumDirection().y()                );
        if( photon_direction_z_ID.second >= 0 ) m_analysisManager->FillNtupleDColumn( photon_direction_z_ID.first, photon_direction_z_ID.second, t_step->GetPostStepPoint()->GetMomentumDirection().z()                );
        if( photon_energy_ID     .second >= 0 ) m_analysisManager->FillNtupleDColumn( photon_energy_ID     .first, photon_energy_ID     .second, t_step->GetPostStepPoint()->GetKineticEnergy()                        );
        if( photon_volume_ID     .second >= 0 ) m_analysisManager->FillNtupleSColumn( photon_volume_ID     .first, photon_volume_ID     .second, t_step->GetPostStepPoint()->GetPhysicalVolume()->GetName()            );
        m_analysisManager->AddNtupleRow( photon_length_ID.first ); // assuming all photon_* columns have the same index
    } else if( t_step->GetTrack()->GetParentID() == 0 ) {
        if( primary_position_x_ID .second >= 0 ) m_analysisManager->FillNtupleDColumn( primary_position_x_ID .first, primary_position_x_ID .second, t_step->GetPostStepPoint()->GetPosition().x()                         );
        if( primary_position_y_ID .second >= 0 ) m_analysisManager->FillNtupleDColumn( primary_position_y_ID .first, primary_position_y_ID .second, t_step->GetPostStepPoint()->GetPosition().y()                         );
        if( primary_position_z_ID .second >= 0 ) m_analysisManager->FillNtupleDColumn( primary_position_z_ID .first, primary_position_z_ID .second, t_step->GetPostStepPoint()->GetPosition().z()                         );
        if( primary_direction_x_ID.second >= 0 ) m_analysisManager->FillNtupleDColumn( primary_direction_x_ID.first, primary_direction_x_ID.second, t_step->GetPostStepPoint()->GetMomentumDirection().x()                );
        if( primary_direction_y_ID.second >= 0 ) m_analysisManager->FillNtupleDColumn( primary_direction_y_ID.first, primary_direction_y_ID.second, t_step->GetPostStepPoint()->GetMomentumDirection().y()                );
        if( primary_direction_z_ID.second >= 0 ) m_analysisManager->FillNtupleDColumn( primary_direction_z_ID.first, primary_direction_z_ID.second, t_step->GetPostStepPoint()->GetMomentumDirection().z()                );
        if( primary_process_ID    .second >= 0 ) m_analysisManager->FillNtupleSColumn( primary_process_ID    .first, primary_process_ID    .second, t_step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() );
        if( primary_time_ID       .second >= 0 ) m_analysisManager->FillNtupleDColumn( primary_time_ID       .first, primary_time_ID       .second, t_step->GetPostStepPoint()->GetGlobalTime()                           );
        m_analysisManager->AddNtupleRow( primary_position_x_ID.first ); // assuming all primary_* columns have the same index
    }
}