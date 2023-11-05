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
    m_name = t_name;
    collectionName.insert( "PhotoSensorSensitiveDetector" );
}

void PhotoSensorSensitiveDetector::Initialize( G4HCofThisEvent* t_hitCollectionOfThisEvent ) {
    m_photoSensorHitsCollection = new PhotoSensorHitsCollection( SensitiveDetectorName, collectionName[ 0 ] );
    if( m_photoSensorHitsCollectionID < 0 )
        m_photoSensorHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID( m_photoSensorHitsCollection );
    t_hitCollectionOfThisEvent->AddHitsCollection( m_photoSensorHitsCollectionID, m_photoSensorHitsCollection );
}

G4bool PhotoSensorSensitiveDetector::ProcessHits( G4Step* t_step, G4TouchableHistory* t_hist ) {
    // m_outputManager->save_step_photoSensor_hits( t_step, m_name, m_position, m_rotationMatrix, false );
    PhotoSensorHit* hit = new PhotoSensorHit();

    hit->set_photoSensor_position      ( m_position                                     );
    hit->set_photoSensor_rotationMatrix( m_rotationMatrix                               );
    hit->set_photoSensor_name          ( m_name                                         );
    hit->set_hit_position              ( t_step->GetPostStepPoint()->GetPosition     () );
    hit->set_hit_time                  ( t_step->GetPostStepPoint()->GetGlobalTime   () );
    hit->set_hit_energy                ( t_step->GetPostStepPoint()->GetKineticEnergy() );
    hit->set_hit_momentum              ( t_step->GetPostStepPoint()->GetMomentum     () );
    hit->set_particle_energy           ( t_step->GetTrack        ()->GetKineticEnergy() );
    hit->set_particle_momentum         ( t_step->GetTrack        ()->GetMomentum     () );

    m_photoSensorHitsCollection->insert( hit );

    // G4cout << "Hit: " << hit << G4endl << G4endl;

    return true;
}

G4String PhotoSensorSensitiveDetector::get_name() {
    return m_name;
}


void PhotoSensorSensitiveDetector::set_position( G4ThreeVector t_position ) {
    m_position = t_position;
}

void PhotoSensorSensitiveDetector::set_rotationMatrix( G4RotationMatrix* t_rotationMatrix ) {
    m_rotationMatrix = t_rotationMatrix;
}

G4ThreeVector PhotoSensorSensitiveDetector::get_position() {
    return m_position;
}

G4RotationMatrix* PhotoSensorSensitiveDetector::get_rotationMatrix() {
    return m_rotationMatrix;
}