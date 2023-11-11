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

#include "GeometricObject.hh"
#include "LensSystem.hh"
#include "PhotoSensor.hh"
#include "ConstructionMessenger.hh"

using std      ::to_string;
using G4StrUtil::to_lower ;

class DirectionSensitivePhotoDetector
{
    public:
        DirectionSensitivePhotoDetector( const G4String&, const G4String& );
       ~DirectionSensitivePhotoDetector();

        static G4ThreeVector get_size  ();
        static G4double      get_width ();
        static G4double      get_height();
        static G4double      get_depth ();
        
        G4String          get_name                ();
        G4RotationMatrix* get_rotationMatrix      ();
        G4LogicalVolume * get_parentLogicalVolume ();
        G4bool            get_isMany              ();
        G4ThreeVector     get_position_lensSystem ();
        G4ThreeVector     get_position_photoSensor();
        G4ThreeVector     get_position            ( const char* );
        G4ThreeVector     get_position_front      ();
        G4ThreeVector     get_position_back       ();
        G4ThreeVector     get_position_center     ();

        static G4ThreeVector get_position_front (              G4RotationMatrix*, G4ThreeVector, const char* );
        static G4ThreeVector get_position_back  (              G4RotationMatrix*, G4ThreeVector, const char* );
        static G4ThreeVector get_position_center(              G4RotationMatrix*, G4ThreeVector, const char* );
        static G4ThreeVector get_position       ( const char*, G4RotationMatrix*, G4ThreeVector, const char* );

        LensSystem * get_lensSystem ();
        PhotoSensor* get_photoSensor();

        void set_name( const G4String& );
        
        void place( G4RotationMatrix*, G4ThreeVector, G4LogicalVolume*, G4bool, const char* );

    protected:
        LensSystem      * m_lensSystem         { nullptr };
        PhotoSensor     * m_photoSensor        { nullptr };

        G4String          m_name                          ;
        G4int             m_ID                            ;
        G4RotationMatrix* m_rotationMatrix     { nullptr };
        G4ThreeVector     m_position_lensSystem           ;
        G4ThreeVector     m_position_photoSensor          ;
        G4LogicalVolume * m_parentLogicalVolume{ nullptr };
        G4bool            m_isMany                        ;
};

#endif
