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

#include <cmath>

using std::max;

Lens::Lens( G4int t_nLens ) {
    G4double         surface_1_radius_x = m_constructionMessenger->get_lens_surface_1_radius_x( t_nLens );
    G4double         surface_1_radius_y = m_constructionMessenger->get_lens_surface_1_radius_y( t_nLens );
    G4double         surface_1_yLimits  = m_constructionMessenger->get_lens_surface_1_yLimits ( t_nLens );
    G4double         surface_2_radius_x = m_constructionMessenger->get_lens_surface_2_radius_x( t_nLens );
    G4double         surface_2_radius_y = m_constructionMessenger->get_lens_surface_2_radius_y( t_nLens );
    G4double         surface_2_yLimits  = m_constructionMessenger->get_lens_surface_2_yLimits ( t_nLens );
    G4double         distance           = m_constructionMessenger->get_lens_distance          ( t_nLens );
    G4double         position           = m_constructionMessenger->get_lens_position          ( t_nLens );
    G4String         material           = m_constructionMessenger->get_lens_material          ( t_nLens );
    G4bool           circular           = m_constructionMessenger->get_lens_circular          ( t_nLens );
    G4VisAttributes* visAttributes      = m_constructionMessenger->get_lens_visAttributes     ( t_nLens );
    G4double         width              = m_constructionMessenger->get_calorimeter_size_width (         );

    if( surface_1_yLimits != surface_2_yLimits )
        G4Exception( "Lens::Lens", "InvalidSetup", FatalException, "Lens yLimits are not equal" );

    G4double surface_1_position      = -surface_1_radius_x + distance / 2;
    G4double surface_2_position      = -surface_2_radius_x - distance / 2;
    G4double surface_1_xLimit        = get_xLimit( surface_1_radius_x, surface_1_radius_y, surface_1_yLimits, 0 );
    G4double surface_2_xLimit        = get_xLimit( surface_2_radius_x, surface_2_radius_y, surface_2_yLimits, 0 );
    G4double middleTube_size         = distance - abs( surface_1_radius_x - surface_1_xLimit ) - abs( surface_2_radius_x - surface_2_xLimit );
    G4int    surface_1_radius_x_sign = surface_1_radius_x > 0 ? 1 : -1;
    G4int    surface_2_radius_x_sign = surface_2_radius_x > 0 ? 1 : -1;

    G4Ellipsoid     * surface_1_init  = new G4Ellipsoid     ( "surface_1_init" , surface_1_radius_y, surface_1_radius_y, abs( surface_1_radius_x ) );
    G4Ellipsoid     * surface_2_init  = new G4Ellipsoid     ( "surface_2_init" , surface_2_radius_y, surface_2_radius_y, abs( surface_2_radius_x ) );
    G4EllipticalTube* middleTube_init = new G4EllipticalTube( "middleTube_init", surface_1_radius_y, surface_1_radius_y, middleTube_size / 2       );

    G4Box* surface_1_boxOriginCut = new G4Box( "surface_1_boxOriginCut", surface_1_radius_y, surface_1_radius_y, abs( surface_1_radius_x ) );
    G4Box* surface_2_boxOriginCut = new G4Box( "surface_2_boxOriginCut", surface_2_radius_y, surface_2_radius_y, abs( surface_2_radius_x ) );
    G4SubtractionSolid* surface_1_originCut = new G4SubtractionSolid( "surface_1_originCut", surface_1_init, surface_1_boxOriginCut, nullptr, G4ThreeVector( 0, 0, -surface_1_radius_x ) );
    G4SubtractionSolid* surface_2_originCut = new G4SubtractionSolid( "surface_2_originCut", surface_2_init, surface_2_boxOriginCut, nullptr, G4ThreeVector( 0, 0, -surface_2_radius_x ) );

    G4Box* surface_1_box  = new G4Box( "surface_1_box" , surface_1_radius_y, surface_1_radius_y, abs( surface_1_xLimit ) );
    G4Box* surface_2_box  = new G4Box( "surface_2_box" , surface_2_radius_y, surface_2_radius_y, abs( surface_2_xLimit ) );
    G4Box* middleTube_box = new G4Box( "middleTube_box", 1e-5              , 1e-5              , 1e-5                    );
    G4SubtractionSolid* surface_1_trim  = new G4SubtractionSolid( "surface_1_originCut", surface_1_originCut, surface_1_box  );
    G4SubtractionSolid* surface_2_trim  = new G4SubtractionSolid( "surface_2_originCut", surface_2_originCut, surface_2_box  );

    G4SubtractionSolid* surface_1_subtract  = nullptr;
    G4SubtractionSolid* surface_2_subtract  = nullptr;
    G4SubtractionSolid* middleTube_subtract = nullptr;

    G4double max_width    = max( surface_1_radius_y, surface_2_radius_y                    );
    G4double max_distance = max( max( surface_1_radius_x, surface_2_radius_x ), distance/2 );
    if( circular ) {
        G4EllipticalTube* tube_inner = new G4EllipticalTube( "tube_inner", surface_1_yLimits, surface_1_yLimits, max_distance*4 );
        G4EllipticalTube* tube_outer = new G4EllipticalTube( "tube_outer", max_width        , max_width        , max_distance*4 );
        G4SubtractionSolid* tube = new G4SubtractionSolid( "tube", tube_outer, tube_inner );

        surface_1_subtract  = new G4SubtractionSolid( "surface_1_subtract" , surface_1_trim , tube );
        surface_2_subtract  = new G4SubtractionSolid( "surface_2_subtract" , surface_2_trim , tube );
        middleTube_subtract = new G4SubtractionSolid( "middleTube_subtract", middleTube_init, tube );
    } else {
        G4Box* box_inner = new G4Box( "box_inner", width/2  , width/2  , max_distance*4 );
        G4Box* box_outer = new G4Box( "box_outer", max_width, max_width, max_distance*4 );
        G4SubtractionSolid* box = new G4SubtractionSolid( "surface_1", box_outer, box_inner );

        surface_1_subtract  = new G4SubtractionSolid( "surface_1_subtract" , surface_1_trim , box );
        surface_2_subtract  = new G4SubtractionSolid( "surface_2_subtract" , surface_2_trim , box );
        middleTube_subtract = new G4SubtractionSolid( "middleTube_subtract", middleTube_init, box );
    }

    G4String name = "lens_";
    name += std::to_string( t_nLens );
    G4UnionSolid* temp = new G4UnionSolid( name, middleTube_subtract, surface_1_subtract, nullptr, G4ThreeVector( 0, 0, -surface_1_xLimit + middleTube_size/2*surface_1_radius_x_sign ) );

    m_geometricObject->set_solid( new G4UnionSolid( name, temp, surface_2_subtract, nullptr, G4ThreeVector( 0, 0, -surface_2_xLimit + middleTube_size/2*surface_2_radius_x_sign ) ) );
    // m_geometricObject->set_solid( temp );
    m_geometricObject->set_material( material );
    m_geometricObject->set_visAttributes( visAttributes );
    m_geometricObject->make_logicalVolume();
}

Lens::~Lens() {
    if( m_geometricObject ) delete m_geometricObject;
}

ostream& operator<<( ostream& t_os, Lens* t_lens )
{
    t_os << *t_lens;
    return t_os;
}

ostream& operator<<( ostream& t_os, const Lens& t_lens )
{
    // t_os << "[" << t_lens.get_lensSurface_1                 () << ", "
    //             << t_lens.get_lensSurface_2                 () << ", "
    //             << t_lens.get_distanceBetweenSurfaces       () << ", "
    //             << t_lens.get_distanceOfLensCenterFromOrigin() 
    //      << "]";
    return t_os;
}

void Lens::place( G4RotationMatrix * t_rotationMatrix     , 
                  G4ThreeVector      t_translation        ,
                  G4LogicalVolume  * t_motherLogicalVolume,
                  G4bool             t_isMany              ) {
    m_geometricObject->place( t_rotationMatrix, t_translation, t_motherLogicalVolume, t_isMany );
}

G4double Lens::get_xLimit( G4double t_radius_x, G4double t_radius_y, G4double t_yLimits, G4double t_position ) {
    G4int    sign     = t_radius_x / abs( t_radius_x );
    G4double c_1      = 1 / pow( t_radius_x, 2 );
    G4double c_2      = -2 * t_position / pow( t_radius_x, 2 );
    G4double c_3      = pow( t_position, 2 ) / pow( t_radius_x, 2 ) + pow( abs( t_yLimits ), 2 ) / pow( t_radius_y, 2 ) - 1;

    return ( -c_2 + sign * sqrt( pow( c_2, 2 ) -4 * c_1 * c_3 ) ) / ( 2 * c_1 );
}

GeometricObjectUnionSolid* Lens::get_geometricObject() const {
    return m_geometricObject;
}