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

#include "OutputMessenger.hh"
#include "OutputManager.hh"
#include "DetectorConstruction.hh"
#include "ConstructionMessenger.hh"

using std::to_string;

class RunAction : public G4UserRunAction
{
    public:
        RunAction( DetectorConstruction* )         ;
       ~RunAction(                       ) override;

        void BeginOfRunAction( const G4Run* ) override;
        void   EndOfRunAction( const G4Run* ) override;

        OutputManager* get_outputManager();

    private:
        G4AnalysisManager    * m_analysisManager      { G4AnalysisManager    ::Instance    () };
        OutputMessenger      * m_outputMessenger      { OutputMessenger      ::get_instance() };
        // OutputManager        * m_outputManager        { OutputManager        ::get_instance() };
        OutputManager        * m_outputManager        { new OutputManager()                   };
        DetectorConstruction * m_detectorConstruction { nullptr                               };
        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };
};

#endif