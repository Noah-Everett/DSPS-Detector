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
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4EllipticalTube.hh"

#include "LensSurface.hh"
#include "GeometricObject.hh"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using std::vector;
using std::ostream;
using std::ifstream;
using std::stringstream;

class Lens
{
    public:
        Lens( LensSurface*, LensSurface*, G4double, G4double );
       ~Lens();
        
        friend ostream& operator<<( ostream&,       Lens* );
        friend ostream& operator<<( ostream&, const Lens& );

        LensSurface                  * get_lensSurface_1                 () const { return m_lensSurface_1                 ; }
        LensSurface                  * get_lensSurface_2                 () const { return m_lensSurface_2                 ; }
        GeometricObjectEllipticalTube* get_middleTube                    () const { return m_middleTube                    ; }
        G4double                       get_distanceBetweenSurfaces       () const { return m_distanceBetweenSurfaces       ; }
        G4double                       get_distanceOfLensCenterFromOrigin() const { return m_distanceOfLensCenterFromOrigin; }

        void place( G4RotationMatrix*, G4ThreeVector, G4LogicalVolume*, G4bool = false, G4int = 0 );

    protected:
        LensSurface                  * m_lensSurface_1                                  ;
        LensSurface                  * m_lensSurface_2                                  ;
        GeometricObjectEllipticalTube* m_middleTube{ new GeometricObjectEllipticalTube };
        G4double                       m_distanceBetweenSurfaces                        ;
        G4double                       m_distanceOfLensCenterFromOrigin                 ;

};

#endif