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

#ifndef PhotoSensor_h
#define PhotoSensor_h

#include "globals.hh"
#include "G4Box.hh"

#include "GeometricObject.hh"
#include "PhotoSensorSensitiveDetector.hh"
#include "ConstructionMessenger.hh"

class PhotoSensor
{
    public:
        PhotoSensor( G4String );
       ~PhotoSensor();

        void place( G4RotationMatrix*, G4ThreeVector, G4LogicalVolume*, G4bool = false );

        G4String                      get_name             ();
        GeometricObjectBox          * get_surface          ();
        GeometricObjectBox          * get_body             ();
        PhotoSensorSensitiveDetector* get_sensitiveDetector();

        void set_sensitiveDetector( PhotoSensorSensitiveDetector* );
        void set_name             ( const G4String&               );

    protected:
        GeometricObjectBox          * m_surface              { new GeometricObjectBox()              };
        GeometricObjectBox          * m_body                 { new GeometricObjectBox()              };
        ConstructionMessenger       * m_constructionMessenger{ ConstructionMessenger::get_instance() };
        PhotoSensorSensitiveDetector* m_sensitiveDetector    { nullptr                               };

        G4RotationMatrix* m_rotationMatrix{ new G4RotationMatrix() };
        G4ThreeVector     m_position      { 0, 0, 0 };

        G4String m_name;
};

#endif
