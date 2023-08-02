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

#include "Lens.hh"

Lens::Lens( Surface * t_surface_1                     , 
            Surface * t_surface_2                     , 
            G4double  t_distanceBetweenSurfaces       , 
            G4double  t_distanceOfLensCenterFromOrigin ) :
    m_lensSurface_1                 ( t_lensSurface_1                  ),
    m_lensSurface_2                 ( t_lensSurface_2                  ),
    m_distanceBetweenSurfaces       ( t_distanceBetweenSurfaces        ),
    m_distanceOfLensCenterFromOrigin( t_distanceOfLensCenterFromOrigin ) {

    G4EllipticalTube* lensMiddle = new G4EllipticalTube( "lensMiddle"                       , 
                                                         0.5 * m_lensSurface_1->get_yLimit(),
                                                         0.5 * m_lensSurface_1->get_yLimit(),
                                                         0.5 * m_distanceBetweenSurfaces     );
    m_middleTube->set_solid   ( lensMiddle                      );
    m_middleTube->set_material( m_lensSurface_1->get_material() );
    m_middleTube->make_logicalVolume();
}

friend ostream& Lens::operator<<( ostream& t_os, Lens* t_lens )
{
    t_os << t_lens*;
    return t_os;
}

friend ostream& Lens::operator<<( ostream& t_os, const Lens& t_lens )
{
    t_os << "[" << t_lens.m_surface_1                      << ", "
                << t_lens.m_surface_2                      << ", "
                << t_lens.m_distanceBetweenSurfaces        << ", "
                << t_lens.m_distanceOfLensCenterFromOrigin 
         << "]";
    return t_os;
}

void Lens::place_lens( G4RotationMatrix * t_rotationMatrix     , 
                       G4ThreeVector    * t_translation        ,
                       G4LogicalVolume  * t_motherLogicalVolume,
                       G4bool             t_isMany             ,
                       G4int              t_copyNumber          ) {
    
}