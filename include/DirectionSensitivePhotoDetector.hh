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

#ifndef DirectionSensitivePhotoDetector_h
#define DirectionSensitivePhotoDetector_h

#include "globals.hh"
#include "G4Material.hh"
#include "G4GDMLParser.hh"

#include "GeometricObject.hh"
#include "LensSystem.hh"
#include "PhotoSensor.hh"
#include "ConstructionMessenger.hh"

class DirectionSensitivePhotoDetector
{
    public:
        DirectionSensitivePhotoDetector();
       ~DirectionSensitivePhotoDetector();

        void place( G4RotationMatrix*, G4ThreeVector, G4LogicalVolume*, G4bool );

        G4ThreeVector get_size  () const;
        G4double      get_width () const;
        G4double      get_height() const;
        G4double      get_depth () const;

    protected:
        LensSystem * m_lensSystem { new LensSystem ( true ) };
        PhotoSensor* m_photoSensor{ new PhotoSensor() };
        G4ThreeVector m_size;
        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };
        G4GDMLParser* m_parser{ new G4GDMLParser() };
};

#endif
