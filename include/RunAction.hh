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

#ifndef RunAction_h
#define RunAction_h

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include "G4AnalysisManager.hh"

#include <vector>

class G4Run;

class RunAction : public G4UserRunAction
{
public:
    RunAction();
   ~RunAction() override;

    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;

    inline void incrament_hist_dEdX_nEnteries             () { ++m_hist_dEdX_nEnteries             ; }
    inline void incrament_hist_emission_energies_nEnteries() { ++m_hist_emission_energies_nEnteries; }
    inline void incrament_hist_transmittance_nEnteries    () { ++m_hist_transmittance_nEnergies    ; }

    G4double hist_dEdX_nBins                 ;
    G4double hist_dEdX_min                   ;
    G4double hist_dEdX_max                   ;
    G4double hist_emission_distance_nBins  ;
    G4double hist_emission_distance_min    ;
    G4double hist_emission_distance_max    ;
    G4double hist_emission_angle_nBins     ;
    G4double hist_emission_angle_min       ;
    G4double hist_emission_angle_max       ;
    G4double hist_transmittance_nBins             ;
    G4double hist_transmittance_min               ;
    G4double hist_transmittance_max               ;

private:
    G4AnalysisManager* m_analysisManager = G4AnalysisManager::Instance();

    G4int m_hist_dEdX_nEnteries             { 0 };
    G4int m_hist_emission_energies_nEnteries{ 0 };
    G4int m_hist_transmittance_nEnergies    { 0 };
};

#endif