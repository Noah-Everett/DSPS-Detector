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

#ifndef Calorimeter_h
#define Calorimeter_h

#include "globals.hh"
#include "G4Material.hh"

#include "GeometricObject.hh"
#include "ConstructionMessenger.hh"
#include "CalorimeterSensitiveDetector.hh"

class Calorimeter
{
    public:
        Calorimeter( G4String t_name, G4ThreeVector t_size );
       ~Calorimeter();

        static G4ThreeVector get_size  ();
        static G4double      get_width ();
        static G4double      get_height();
        static G4double      get_depth ();

        void place( G4RotationMatrix*, G4ThreeVector, G4LogicalVolume*, G4bool );

    protected:
        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };

        GeometricObjectBox* m_calorimeter{ nullptr };
        CalorimeterSensitiveDetector* m_calorimeterSensitiveDetector{ nullptr };
};

#endif
