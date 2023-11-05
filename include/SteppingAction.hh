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

#ifndef SteppingAction_h
#define SteppingAction_h

#include "G4Step.hh"
#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4AnalysisManager.hh"

#include "OutputManager.hh"
#include "OutputMessenger.hh"

class G4LogicalVolume;

class EventAction;

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction( RunAction* )         ;
   ~SteppingAction(            ) override;

    void UserSteppingAction( const G4Step* ) override;
    
private:
    RunAction        * m_runAction      { nullptr                           };
    OutputManager    * m_outputManager  { nullptr                           };
    OutputMessenger  * m_outputMessenger{ OutputMessenger  ::get_instance() };
    G4AnalysisManager* m_analysisManager{ nullptr                           };
        
    pair< G4int, G4int > photon_length_ID      ;
    pair< G4int, G4int > photon_process_ID     ;
    pair< G4int, G4int > photon_time_ID        ;
    pair< G4int, G4int > photon_position_x_ID  ;
    pair< G4int, G4int > photon_position_y_ID  ;
    pair< G4int, G4int > photon_position_z_ID  ;
    pair< G4int, G4int > photon_direction_x_ID ;
    pair< G4int, G4int > photon_direction_y_ID ;
    pair< G4int, G4int > photon_direction_z_ID ;
    pair< G4int, G4int > photon_energy_ID      ;
    pair< G4int, G4int > photon_volume_ID      ;

    pair< G4int, G4int > primary_position_x_ID ;
    pair< G4int, G4int > primary_position_y_ID ;
    pair< G4int, G4int > primary_position_z_ID ;
    pair< G4int, G4int > primary_direction_x_ID;
    pair< G4int, G4int > primary_direction_y_ID;
    pair< G4int, G4int > primary_direction_z_ID;
    pair< G4int, G4int > primary_process_ID    ;
    pair< G4int, G4int > primary_time_ID       ;
};

#endif