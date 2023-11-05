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

#include<cmath>

SteppingAction::SteppingAction( RunAction* t_runAction ) {
    m_runAction     = t_runAction;
    m_outputManager = m_runAction->get_outputManager();

    photon_length_ID       = m_outputManager->get_tuple_column_ID( "photon_length"       );
    G4cout << "photon_length_ID      = " << photon_length_ID      .first << ", " << photon_length_ID      .second << G4endl;
    photon_process_ID      = m_outputManager->get_tuple_column_ID( "photon_process"      );
    G4cout << "photon_process_ID     = " << photon_process_ID     .first << ", " << photon_process_ID     .second << G4endl;
    photon_time_ID         = m_outputManager->get_tuple_column_ID( "photon_time"         );
    G4cout << "photon_time_ID        = " << photon_time_ID        .first << ", " << photon_time_ID        .second << G4endl;
    photon_position_x_ID   = m_outputManager->get_tuple_column_ID( "photon_position_x"   );
    G4cout << "photon_position_x_ID  = " << photon_position_x_ID  .first << ", " << photon_position_x_ID  .second << G4endl;
    photon_position_y_ID   = m_outputManager->get_tuple_column_ID( "photon_position_y"   );
    G4cout << "photon_position_y_ID  = " << photon_position_y_ID  .first << ", " << photon_position_y_ID  .second << G4endl;
    photon_position_z_ID   = m_outputManager->get_tuple_column_ID( "photon_position_z"   );
    G4cout << "photon_position_z_ID  = " << photon_position_z_ID  .first << ", " << photon_position_z_ID  .second << G4endl;
    photon_direction_x_ID  = m_outputManager->get_tuple_column_ID( "photon_direction_x"  );
    G4cout << "photon_direction_x_ID = " << photon_direction_x_ID .first << ", " << photon_direction_x_ID .second << G4endl;
    photon_direction_y_ID  = m_outputManager->get_tuple_column_ID( "photon_direction_y"  );
    G4cout << "photon_direction_y_ID = " << photon_direction_y_ID .first << ", " << photon_direction_y_ID .second << G4endl;
    photon_direction_z_ID  = m_outputManager->get_tuple_column_ID( "photon_direction_z"  );
    G4cout << "photon_direction_z_ID = " << photon_direction_z_ID .first << ", " << photon_direction_z_ID .second << G4endl;
    photon_energy_ID       = m_outputManager->get_tuple_column_ID( "photon_energy"       );
    G4cout << "photon_energy_ID      = " << photon_energy_ID      .first << ", " << photon_energy_ID      .second << G4endl;
    photon_volume_ID       = m_outputManager->get_tuple_column_ID( "photon_volume"       );
    G4cout << "photon_volume_ID      = " << photon_volume_ID      .first << ", " << photon_volume_ID      .second << G4endl;

    primary_position_x_ID  = m_outputManager->get_tuple_column_ID( "primary_position_x"  );
    G4cout << "primary_position_x_ID = " << primary_position_x_ID .first << ", " << primary_position_x_ID .second << G4endl;
    primary_position_y_ID  = m_outputManager->get_tuple_column_ID( "primary_position_y"  );
    G4cout << "primary_position_y_ID = " << primary_position_y_ID .first << ", " << primary_position_y_ID .second << G4endl;
    primary_position_z_ID  = m_outputManager->get_tuple_column_ID( "primary_position_z"  );
    G4cout << "primary_position_z_ID = " << primary_position_z_ID .first << ", " << primary_position_z_ID .second << G4endl;
    primary_direction_x_ID = m_outputManager->get_tuple_column_ID( "primary_direction_x" );
    G4cout << "primary_direction_x_ID= " << primary_direction_x_ID.first << ", " << primary_direction_x_ID.second << G4endl;
    primary_direction_y_ID = m_outputManager->get_tuple_column_ID( "primary_direction_y" );
    G4cout << "primary_direction_y_ID= " << primary_direction_y_ID.first << ", " << primary_direction_y_ID.second << G4endl;
    primary_direction_z_ID = m_outputManager->get_tuple_column_ID( "primary_direction_z" );
    G4cout << "primary_direction_z_ID= " << primary_direction_z_ID.first << ", " << primary_direction_z_ID.second << G4endl;
    primary_process_ID     = m_outputManager->get_tuple_column_ID( "primary_process"     );
    G4cout << "primary_process_ID    = " << primary_process_ID    .first << ", " << primary_process_ID    .second << G4endl;
    primary_time_ID        = m_outputManager->get_tuple_column_ID( "primary_time"        );
    G4cout << "primary_time_ID       = " << primary_time_ID       .first << ", " << primary_time_ID       .second << G4endl;
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
        // G4cout << "Saving photon step" << G4endl;
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
        m_analysisManager->AddNtupleRow();
    } else if( t_step->GetTrack()->GetParentID() == 0 ) {
        // G4cout << "Saving primary step" << G4endl;
        if( primary_position_x_ID .second >= 0 ) m_analysisManager->FillNtupleDColumn( primary_position_x_ID .first, primary_position_x_ID .second, t_step->GetPostStepPoint()->GetPosition().x()                         );
        if( primary_position_y_ID .second >= 0 ) m_analysisManager->FillNtupleDColumn( primary_position_y_ID .first, primary_position_y_ID .second, t_step->GetPostStepPoint()->GetPosition().y()                         );
        if( primary_position_z_ID .second >= 0 ) m_analysisManager->FillNtupleDColumn( primary_position_z_ID .first, primary_position_z_ID .second, t_step->GetPostStepPoint()->GetPosition().z()                         );
        if( primary_direction_x_ID.second >= 0 ) m_analysisManager->FillNtupleDColumn( primary_direction_x_ID.first, primary_direction_x_ID.second, t_step->GetPostStepPoint()->GetMomentumDirection().x()                );
        if( primary_direction_y_ID.second >= 0 ) m_analysisManager->FillNtupleDColumn( primary_direction_y_ID.first, primary_direction_y_ID.second, t_step->GetPostStepPoint()->GetMomentumDirection().y()                );
        if( primary_direction_z_ID.second >= 0 ) m_analysisManager->FillNtupleDColumn( primary_direction_z_ID.first, primary_direction_z_ID.second, t_step->GetPostStepPoint()->GetMomentumDirection().z()                );
        if( primary_process_ID    .second >= 0 ) m_analysisManager->FillNtupleSColumn( primary_process_ID    .first, primary_process_ID    .second, t_step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() );
        if( primary_time_ID       .second >= 0 ) m_analysisManager->FillNtupleDColumn( primary_time_ID       .first, primary_time_ID       .second, t_step->GetPostStepPoint()->GetGlobalTime()                           );
        m_analysisManager->AddNtupleRow();
    }
}