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

#ifndef EventAction_h
#define EventAction_h

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "OutputMessenger.hh"
#include "ConstructionMessenger.hh"
#include "RunAction.hh"

using std::to_string;

class EventAction : public G4UserEventAction
{
    public:
        EventAction( RunAction*, DetectorConstruction* )         ;
       ~EventAction(                                   ) override;

        void BeginOfEventAction( const G4Event* ) override;
        void EndOfEventAction  ( const G4Event* ) override;

    private:
        RunAction            * m_runAction            { nullptr                               };
        OutputMessenger      * m_outputMessenger      { OutputMessenger      ::get_instance() };
        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };
        DetectorConstruction * m_detectorConstruction { nullptr                               };
        OutputManager        * m_outputManager        { nullptr                               };
        G4AnalysisManager    * m_analysisManager      { nullptr                               };
        G4SDManager          * m_SDManager            { nullptr                               };

        pair< G4int, G4int > m_photoSensor_hits_position_absolute_ID;
        pair< G4int, G4int > m_photoSensor_hits_position_relative_ID;
        pair< G4int, G4int > m_photoSensor_hits_time_ID             ;
        pair< G4int, G4int > m_photoSensor_hits_process_ID          ;
        pair< G4int, G4int > m_photoSensor_hits_photoSensorID_ID    ;
        pair< G4int, G4int > m_photoSensor_hits_energy_ID           ;

        // vector< G4int > m_hitsCollection_IDs_photoSensor;
};

#endif