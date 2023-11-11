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
}

SteppingAction::~SteppingAction() {
}

void SteppingAction::UserSteppingAction( const G4Step* t_step ) {
    if( !t_step->GetPostStepPoint()->GetPhysicalVolume() ) {
        t_step->GetTrack()->SetTrackStatus( fKillTrackAndSecondaries );
        return;
    }

    if( abs( t_step->GetTrack()->GetDefinition()->GetPDGEncoding() ) == 0  || 
        abs( t_step->GetTrack()->GetDefinition()->GetPDGEncoding() ) == 22    ) {
        m_outputManager->fill_tuple_column_double( "photon_length"     , t_step->GetStepLength()                                               );
        m_outputManager->fill_tuple_column_string( "photon_process"    , t_step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() );
        m_outputManager->fill_tuple_column_double( "photon_time"       , t_step->GetPostStepPoint()->GetGlobalTime()                           );
        m_outputManager->fill_tuple_column_double( "photon_position_x" , t_step->GetPostStepPoint()->GetPosition().x()                         );
        m_outputManager->fill_tuple_column_double( "photon_position_y" , t_step->GetPostStepPoint()->GetPosition().y()                         );
        m_outputManager->fill_tuple_column_double( "photon_position_z" , t_step->GetPostStepPoint()->GetPosition().z()                         );
        m_outputManager->fill_tuple_column_double( "photon_direction_x", t_step->GetPostStepPoint()->GetMomentumDirection().x()                );
        m_outputManager->fill_tuple_column_double( "photon_direction_y", t_step->GetPostStepPoint()->GetMomentumDirection().y()                );
        m_outputManager->fill_tuple_column_double( "photon_direction_z", t_step->GetPostStepPoint()->GetMomentumDirection().z()                );
        m_outputManager->fill_tuple_column_double( "photon_energy"     , t_step->GetPostStepPoint()->GetKineticEnergy()                        );
        m_outputManager->fill_tuple_column_string( "photon_volume"     , t_step->GetPostStepPoint()->GetPhysicalVolume()->GetName()            );
        m_outputManager->fill_tuple_column       ( "photon" );
    } else if( t_step->GetTrack()->GetParentID() == 0 ) {
        m_outputManager->fill_tuple_column_double( "primary_position_x" , t_step->GetPostStepPoint()->GetPosition().x()                         );
        m_outputManager->fill_tuple_column_double( "primary_position_y" , t_step->GetPostStepPoint()->GetPosition().y()                         );
        m_outputManager->fill_tuple_column_double( "primary_position_z" , t_step->GetPostStepPoint()->GetPosition().z()                         );
        m_outputManager->fill_tuple_column_double( "primary_direction_x", t_step->GetPostStepPoint()->GetMomentumDirection().x()                );
        m_outputManager->fill_tuple_column_double( "primary_direction_y", t_step->GetPostStepPoint()->GetMomentumDirection().y()                );
        m_outputManager->fill_tuple_column_double( "primary_direction_z", t_step->GetPostStepPoint()->GetMomentumDirection().z()                );
        m_outputManager->fill_tuple_column_string( "primary_process"    , t_step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() );
        m_outputManager->fill_tuple_column_double( "primary_time"       , t_step->GetPostStepPoint()->GetGlobalTime()                           );
        m_outputManager->fill_tuple_column       ( "primary" );
    }
}