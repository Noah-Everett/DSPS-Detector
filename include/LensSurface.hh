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

#ifndef LensSurface_h
#define LensSurface_h

#include "globals.hh"
#include "G4PVPlacement.hh"
#include "G4Ellipsoid.hh"
#include "G4Exception.hh"
#include "G4SubtractionSolid.hh"

#include "GeometricObject.hh"

#include <iostream>
#include <cmath>

using std::ostream;

class LensSurface
{
    public:
        LensSurface( G4double t_radius_x  , G4double t_radius_y  , 
                     G4double t_yLimit_min, G4double t_yLimit_max,
                     G4String t_material_name, G4VisAttributes* t_visAttributes );
       ~LensSurface();

        friend ostream& operator<<( ostream& ,       LensSurface* );
        friend ostream& operator<<( ostream& , const LensSurface& );

        G4double                         get_radius_x       () const;
        G4double                         get_radius_y       () const;
        G4double                         get_yLimit_min     () const;
        G4double                         get_yLimit_max     () const;
        G4double                         get_yLimit         () const;
        G4double                         get_xLimit         () const;
        GeometricObjectSubtractionSolid* get_geometricObject() const;

        G4PVPlacement* place( G4RotationMatrix*, G4ThreeVector , G4LogicalVolume*, G4bool = false );

    protected:
        G4double m_radius_x  ;
        G4double m_radius_y  ;
        G4double m_yLimit_min;
        G4double m_yLimit_max;
        G4double m_xLimit    ;
        G4ThreeVector m_size ;

        GeometricObjectSubtractionSolid* m_geometricObject{ new GeometricObjectSubtractionSolid() };
};

#endif