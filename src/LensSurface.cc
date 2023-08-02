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

#include "LensSurface.hh"


LensSurface::LensSurface( G4double t_radius_x  , G4double t_radius_y  , 
                          G4double t_yLimit_min, G4double t_yLimit_max,
                          G4String t_material_name                     ) :
    m_radius_x     ( t_radius_x      ),
    m_radius_y     ( t_radius_y      ),
    m_yLimit_min   ( t_yLimit_min    ),
    m_yLimit_max   ( t_yLimit_max    ),
    m_material_name( t_material_name ) {
    
    m_geometricObject->set_material( t_material_name );
    G4Ellipsoid* ellipsoid = new G4Ellipsoid( "surface_ellipsoid", m_radius_x, m_radius_y, m_radius_y );
}