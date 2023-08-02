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

#include "G4Exception.hh"

#include <cmath>

LensSurface::LensSurface( G4double t_radius_x  , G4double t_radius_y  , 
                          G4double t_yLimit_min, G4double t_yLimit_max,
                          G4String t_material_name                     ) :
    m_radius_x     ( t_radius_x      ),
    m_radius_y     ( t_radius_y      ),
    m_yLimit_min   ( t_yLimit_min    ),
    m_yLimit_max   ( t_yLimit_max    ) {

    if( abs( m_yLimit_min ) != abs( m_yLimit_max ) ) {
        G4Exception( "LensSurface::LensSurface", "LensSurface001", FatalException, "abs( yLimit_max ) != abs( yLimit_min )" );
    }

    G4double m_x = 0;
    G4int    sign     = m_radius_x / abs( m_radius_x );
    G4double c_1      = 1 / pow( m_radius_x, 2 );
    G4double c_2      = -2 * m_x / pow( m_radius_x, 2 );
    G4double c_3      = pow( m_x, 2 ) / pow( m_radius_x, 2 ) + pow( abs( m_yLimit_min ), 2 ) / pow( m_radius_y, 2 ) - 1;
             m_xLimit = ( -c_2 + sign * sqrt( pow( c_2, 2 ) -4 * c_1 * c_3 ) ) / ( 2 * c_1 );

    m_geometricObject->set_material( t_material_name );
    G4Ellipsoid* ellipsoid = new G4Ellipsoid( "lensSurface", m_radius_y, m_radius_y, m_radius_x, z_min, 0.0 );
    m_geometricObject->set_solid( ellipsoid );
    m_geometricObject->make_logicalVolume();
}

LensSurface::~LensSurface() {
    delete m_geometricObject;
}
        
friend ostream& LensSurface::operator<<( ostream& t_os, LensSurface* t_surface ) {
    t_os << t_surface*;
    return t_os;
}

friend ostream& LensSurface::operator<<( ostream& t_os, const LensSurface& t_surface ) {
    t_os << "(" << t_surface.get_radius_x  () << ", " 
                << t_surface.get_radius_y  () << ", " 
                << t_surface.get_yLimit_min() << ", " 
                << t_surface.get_yLimit_max() 
         << ")";
    return t_os;
}

G4double LensSurface::get_radius_x() { 
    return m_radius_x;
}

G4double LensSurface::get_radius_y() { 
    return m_radius_y;
}

G4double LensSurface::get_yLimit_min() { 
    return m_yLimit_min;
}

G4double LensSurface::get_yLimit_max() { 
    return m_yLimit_max;
}
G4double LensSurface::get_yLimit() { 
    if( abs( m_yLimit_min ) == abs( m_yLimit_max ) )
        return abs( m_yLimit_min );
    else 
        G4Exception( "LensSurface::get_yLimit", "LensSurface002", FatalException, "abs( yLimit_max ) != abs( yLimit_min )" );

G4double LensSurface::get_xLimit() { 
    return m_xLimit;
}

GeometricObject* LensSurface::get_geometricObject() { 
    return m_geometricObject;
}