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

#include "PhotoSensorSD.hh"

PhotoSensorSD::PhotoSensorSD(const G4String& name) : G4VSensitiveDetector(name) {}

PhotoSensorSD::~PhotoSensorSD() {}

void PhotoSensorSD::Initialize(G4HCofThisEvent* hce) {}

G4bool PhotoSensorSD::ProcessHits(G4Step* step, G4TouchableHistory* hist) {
    // Process hits when energy deposition occurs in the sensitive volume.
    // You can access the energy deposition information from the step and store it for further analysis.
    // For example:
    G4double energyDeposition = step->GetTotalEnergyDeposit();

    // Your custom implementation to store the energyDeposition or perform other actions.

    return true;
}