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

SteppingAction::SteppingAction(EventAction* eventAction, RunAction* runAction, PrimaryGeneratorAction* t_primaryGeneratorAction )
: m_eventAction(eventAction), m_runAction(runAction), m_primaryGeneratorAction{ t_primaryGeneratorAction }
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction( const G4Step* t_step )
{
    G4Track* track = t_step->GetTrack();
    G4StepPoint* stepPoint_pre  = t_step->GetPreStepPoint();
    G4StepPoint* stepPoint_post = t_step->GetPostStepPoint();

    G4String particleName = track->GetDefinition()->GetParticleName();
    G4String processName  = stepPoint_post->GetProcessDefinedStep()->GetProcessName();
}