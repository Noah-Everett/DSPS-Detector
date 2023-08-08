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

#include "PhotoSensorSensitiveDetector.hh"

PhotoSensorSensitiveDetector::PhotoSensorSensitiveDetector( G4String t_name ) 
    : G4VSensitiveDetector( t_name ) {
    // m_outputManager->add_histogram_2D( t_name, t_name, 100, 0, 100, 100, 0, 100 );
}

PhotoSensorSensitiveDetector::~PhotoSensorSensitiveDetector() {
}

void PhotoSensorSensitiveDetector::Initialize( G4HCofThisEvent* hce ) {
}

G4bool PhotoSensorSensitiveDetector::ProcessHits( G4Step* t_step, G4TouchableHistory* t_hist ) {
    // G4int sensorHistogramID = m_outputManager->get_histogram_id( m_name );
    // G4cout << "HERE" << G4endl;
    // m_analysisManager->FillH2( sensorHistogramID, t_step->GetPreStepPoint()->GetPosition().x(), t_step->GetPreStepPoint()->GetPosition().y() );
}

G4String PhotoSensorSensitiveDetector::get_name() {
    return m_name;
}