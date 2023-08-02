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

#ifndef Lens_h
#define Lens_h

#include "globals.hh"

#include "GeometricObject.hh"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using std::vector;
using std::ostream;
using std::ifstream;
using std::stringstream;

class LensSurface
{
    public:
        Surface( G4double t_radius_x, G4double t_radius_y, G4double t_yLimit_min, G4double t_yLimit_max );
       ~Surface();

        friend ostream& operator<<(ostream& os, const Surface& surface)
        {
            os << "(" << surface.r_x << ", " << surface.r_y << ", " << surface.y_min << ", " << surface.y_max << ")";
            return os;
        }

    protected:
        G4double         m_radius_x     ;
        G4double         m_radius_y     ;
        G4double         m_yLimit_min   ;
        G4double         m_yLimit_max   ;
        G4String         m_material_name;
        GeometricObject* m_geometricObject{ new GeometricObject() };

};

class Lens
{
    public:
        Lens(const Surface& surface_1, G4double d, G4double n, const Surface& surface_2, G4double x_l)
            : surface_1(surface_1), d(d), n(n), surface_2(surface_2), x_l(x_l) {}

        Surface surface_1;
        G4double d;
        G4double n;
        Surface surface_2;
        G4double x_l;

        friend ostream& operator<<(ostream& os, const Lens& lens)
        {
            os << "[" << lens.surface_1 << ", " << lens.d << ", " << lens.n << ", " << lens.surface_2 << ", " << lens.x_l << "]";
            return os;
        }
};
