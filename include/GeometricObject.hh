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

#ifndef GeometricObject_h
#define GeometricObject_h

#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VSensitiveDetector.hh"

class GeometricObject
{
    public:
        GeometricObject();
        GeometricObject( G4VSolid*, G4Material*, G4VSensitiveDetector* = nullptr );
       ~GeometricObject();

        void set_material         ( G4Material          * );
        void set_material         ( G4String              );
        void set_sensitiveDetector( G4VSensitiveDetector* );
        void set_solid            ( G4VSolid            * );
        void set_logicalVolume    ( G4LogicalVolume     * );

        void make_logicalVolume();

        G4Material          * get_material         () const;
        G4String              get_material_name    () const;
        G4VSensitiveDetector* get_sensitiveDetector() const;
        G4VSolid            * get_solid            () const;
        G4LogicalVolume     * get_logicalVolume    () const;

    private:
        G4String              m_name             { ""      };
        G4Material          * m_material         { nullptr };
        G4VSensitiveDetector* m_sensitiveDetector{ nullptr };
        G4VSolid            * m_solid            { nullptr };
        G4LogicalVolume     * m_logicalVolume    { nullptr };
};

#endif