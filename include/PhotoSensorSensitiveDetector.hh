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

#ifndef PhotoSensorSensitiveDetector_h
#define PhotoSensorSensitiveDetector_h

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4Event.hh"

#include "OutputMessenger.hh"
#include "OutputManager.hh"
#include "PhotoSensorHit.hh"

class PhotoSensorSensitiveDetector : public G4VSensitiveDetector 
{
    public:
        PhotoSensorSensitiveDetector( G4String );
       ~PhotoSensorSensitiveDetector() override = default;

        void Initialize( G4HCofThisEvent* ) override;
        G4bool ProcessHits( G4Step*, G4TouchableHistory* ) override;

        G4String                   get_name               (                );
        G4ThreeVector              get_position           (                );
        G4RotationMatrix         * get_rotationMatrix     (                );
        PhotoSensorHitsCollection* get_hitsCollection     ( const G4Event* );
        G4String                   get_hitsCollection_name(                );
        G4int                      get_hitsCollection_ID  (                );

        void set_position         ( G4ThreeVector     );
        void set_rotationMatrix   ( G4RotationMatrix* );
        void set_hitsCollection_ID( G4int             );
    
    protected:
        G4String          m_name;
        G4ThreeVector     m_position;
        G4RotationMatrix* m_rotationMatrix;

        PhotoSensorHitsCollection* m_photoSensorHitsCollection   { nullptr };
        G4int                      m_photoSensorHitsCollection_ID{ -1      };
};

#endif