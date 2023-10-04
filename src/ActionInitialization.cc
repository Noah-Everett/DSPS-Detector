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

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"

ActionInitialization::ActionInitialization() {
}

ActionInitialization::~ActionInitialization() {
}

void ActionInitialization::BuildForMaster() const {
    RunAction* runAction = new RunAction();
    G4cout << "HERE0" << G4endl;
    SetUserAction( static_cast< G4UserRunAction* >( runAction ) );
}

void ActionInitialization::Build() const {
    PrimaryGeneratorAction* primaryGeneratorAction = new PrimaryGeneratorAction;
    SetUserAction( primaryGeneratorAction );

    RunAction* runAction = new RunAction();
    G4cout << "HERE1" << G4endl;
    SetUserAction( static_cast< G4UserRunAction* >( runAction ) );
    G4cout << "HERE2" << G4endl;

    EventAction* eventAction = new EventAction( runAction );
    SetUserAction( eventAction );
    G4cout << "HERE3" << G4endl;

    SteppingAction* steppingAction = new SteppingAction();
    SetUserAction( steppingAction );
    G4cout << "HERE4" << G4endl;

    StackingAction* stackingAction = new StackingAction();
    SetUserAction( stackingAction );
    G4cout << "HERE5" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......