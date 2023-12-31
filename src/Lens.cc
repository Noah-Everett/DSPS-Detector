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
using std::to_string;

Lens::Lens( G4String t_name, G4int t_nLens ) {
    m_name = t_name + "_lens_" + to_string( t_nLens );

    m_surface_1_radius_x = m_constructionMessenger->get_lens_surface_1_radius_x( t_nLens );
    m_surface_1_radius_y = m_constructionMessenger->get_lens_surface_1_radius_y( t_nLens );
    m_surface_1_yLimits  = m_constructionMessenger->get_lens_surface_1_yLimits ( t_nLens );
    m_surface_2_radius_x = m_constructionMessenger->get_lens_surface_2_radius_x( t_nLens );
    m_surface_2_radius_y = m_constructionMessenger->get_lens_surface_2_radius_y( t_nLens );
    m_surface_2_yLimits  = m_constructionMessenger->get_lens_surface_2_yLimits ( t_nLens );
    m_distance           = m_constructionMessenger->get_lens_distance          ( t_nLens );
    m_position           = m_constructionMessenger->get_lens_position          ( t_nLens );
    m_material           = m_constructionMessenger->get_lens_material          ( t_nLens );
    m_circular           = m_constructionMessenger->get_lens_circular          ( t_nLens );
    m_visAttributes      = m_constructionMessenger->get_lens_visAttributes     ( t_nLens );
    m_width              = m_constructionMessenger->get_calorimeter_size_width (         );
    if( m_surface_1_yLimits != m_surface_2_yLimits )
        G4Exception( "Lens::Lens", "InvalidSetup", FatalException, "Lens yLimits are not equal." );
    else if( m_surface_1_radius_y <= 0 || m_surface_2_radius_y <= 0 )
        G4Exception( "Lens::Lens", "InvalidSetup", FatalException, "Lens radius_y is not valid." );
    else if( m_surface_1_radius_x == 0 || m_surface_2_radius_x == 0 )
        G4Exception( "Lens::Lens", "InvalidSetup", FatalErrorInArgument,
                     "Lens radius_x are not valid. Cannot have radius=0. "
                     "Aka, planer surfaces are not allowed. "
                     "Instead, use a small number (e.g. 1e-5)." );
    else if( m_position > 0 )
        G4Exception( "Lens::Lens", "InvalidSetup", FatalException, "Lens position is positive." );

    if( m_surface_1_radius_x < 0 && m_surface_2_radius_x > 0 )
        m_shape = m_biconvex;
    else if( m_surface_1_radius_x > 0 && m_surface_2_radius_x < 0 )
        m_shape = m_biconcave;
    else if( m_surface_1_radius_x > 0 && m_surface_2_radius_x > 0 )
        m_shape = m_concave_convex;
    else if( m_surface_1_radius_x < 0 && m_surface_2_radius_x < 0 )
        m_shape = m_convex_concave;
    else
        G4Exception( "Lens::Lens", "InvalidSetup", FatalException, "Lens shape is not valid." );
    G4cout << "Making lens " << t_nLens << " with shape = " << m_lensShape_map.at( m_shape ) << G4endl;

    G4double surface_1_position_absolute = m_position - m_distance / 2 + m_surface_1_radius_x;
    G4double surface_2_position_absolute = m_position + m_distance / 2 + m_surface_2_radius_x;
    G4double surface_1_position_relative = surface_1_position_absolute - m_position;
    G4double surface_2_position_relative = surface_2_position_absolute - m_position;
    G4double surface_1_xLimit            = abs( get_surfaceX( m_surface_1_radius_x, m_surface_1_radius_y, m_surface_1_yLimits, 0 ) );
    G4double surface_2_xLimit            = abs( get_surfaceX( m_surface_2_radius_x, m_surface_2_radius_y, m_surface_2_yLimits, 0 ) );
    G4int    surface_1_radius_x_sign     = m_surface_1_radius_x > 0 ? 1 : -1;
    G4int    surface_2_radius_x_sign     = m_surface_2_radius_x > 0 ? 1 : -1;

    // G4DisplacedSolid* lens_preTrim = nullptr;
    G4DisplacedSolid* lens = nullptr;
    if( m_shape == m_biconvex ) {
        G4double middleTube_size = m_distance + ( m_surface_1_radius_x + surface_1_xLimit ) 
                                              - ( m_surface_2_radius_x - surface_2_xLimit );
        G4EllipticalTube* middleTube_init = nullptr;
        if( middleTube_size > 0 ) {
            middleTube_size /= 2;
            middleTube_init = new G4EllipticalTube( m_name + "_middleTube_init", m_surface_1_radius_y, m_surface_1_radius_y, middleTube_size );
        }

        G4Ellipsoid* surface_1_init = new G4Ellipsoid( m_name + "_surface_1_init" , m_surface_1_radius_y, m_surface_1_radius_y, abs( m_surface_1_radius_x ) );
        G4Ellipsoid* surface_2_init = new G4Ellipsoid( m_name + "_surface_2_init" , m_surface_2_radius_y, m_surface_2_radius_y, abs( m_surface_2_radius_x ) );

        G4Box* surface_1_boxOriginCut = new G4Box( m_name + "_surface_1_boxOriginCut", m_surface_1_radius_y, m_surface_1_radius_y, abs( m_surface_1_radius_x ) );
        G4Box* surface_2_boxOriginCut = new G4Box( m_name + "_surface_2_boxOriginCut", m_surface_2_radius_y, m_surface_2_radius_y, abs( m_surface_2_radius_x ) );

        G4SubtractionSolid* surface_1_originCut = new G4SubtractionSolid( m_name + "_surface_1_originCut", surface_1_init, surface_1_boxOriginCut, nullptr, G4ThreeVector( 0, 0, -m_surface_1_radius_x ) );
        G4SubtractionSolid* surface_2_originCut = new G4SubtractionSolid( m_name + "_surface_2_originCut", surface_2_init, surface_2_boxOriginCut, nullptr, G4ThreeVector( 0, 0, -m_surface_2_radius_x ) );

        G4Box* surface_1_box = new G4Box( m_name + "_surface_1_box" , m_surface_1_radius_y, m_surface_1_radius_y, surface_1_xLimit );
        G4Box* surface_2_box = new G4Box( m_name + "_surface_2_box" , m_surface_2_radius_y, m_surface_2_radius_y, surface_2_xLimit );

        G4SubtractionSolid* surface_1_trim  = new G4SubtractionSolid( m_name + "_surface_1_originCut", surface_1_originCut, surface_1_box );
        G4SubtractionSolid* surface_2_trim  = new G4SubtractionSolid( m_name + "_surface_2_originCut", surface_2_originCut, surface_2_box );

        G4SubtractionSolid* surface_1_trim_final = nullptr;
        G4SubtractionSolid* surface_2_trim_final = nullptr;
        G4SubtractionSolid* middleTube_trim_final = nullptr;
        if( m_circular ) {
            surface_1_trim_final = subtract_circular( surface_1_trim );
            surface_2_trim_final = subtract_circular( surface_2_trim );
            if( middleTube_size > 0 )
                middleTube_trim_final = subtract_circular( middleTube_init );
        } else {
            surface_1_trim_final = subtract_rectangular( surface_1_trim );
            surface_2_trim_final = subtract_rectangular( surface_2_trim );
            if( middleTube_size > 0 )
                middleTube_trim_final = subtract_rectangular( middleTube_init );
        }

        if( middleTube_size > 0 ) {
            G4UnionSolid* middle_and_surface1 = new G4UnionSolid( m_name + "_middle_and_surface1", middleTube_trim_final, surface_1_trim_final, nullptr, G4ThreeVector( 0, 0, surface_1_xLimit - middleTube_size ) );
            G4UnionSolid* lens_preTrim_preTranslation = new G4UnionSolid( m_name + "_middle_and_surface2", middle_and_surface1, surface_2_trim_final, nullptr, G4ThreeVector( 0, 0, middleTube_size - surface_2_xLimit ) ) ;
            G4double avgPos = ( m_surface_1_radius_x + surface_1_xLimit + m_surface_2_radius_x - surface_2_xLimit ) / 2;
            lens = new G4DisplacedSolid( m_name + "_preTrim", lens_preTrim_preTranslation, nullptr, G4ThreeVector( 0, 0, -avgPos ) );

            m_relativePosition_front = G4ThreeVector( 0, 0, middleTube_size - surface_2_xLimit + m_surface_2_radius_x - avgPos );
            m_relativePosition_back = G4ThreeVector( 0, 0, surface_1_xLimit - middleTube_size + m_surface_1_radius_x - avgPos );
            m_relativePosition_center = ( m_relativePosition_front + m_relativePosition_back ) / 2;
        } else {
            G4DisplacedSolid* surface_1_displaced = new G4DisplacedSolid( m_name + "_surface_1_displaced", surface_1_trim, nullptr, G4ThreeVector( 0, 0, surface_1_xLimit ) );
            G4DisplacedSolid* surface_2_displaced = new G4DisplacedSolid( m_name + "_surface_2_displaced", surface_2_trim, nullptr, G4ThreeVector( 0, 0, -surface_2_xLimit ) );
            G4UnionSolid* lens_preTranslation = new G4UnionSolid( m_name + "_preTranslation", surface_1_displaced, surface_2_displaced, nullptr, G4ThreeVector( 0, 0, 0 ) ) ;
            G4double avgPos = ( m_surface_1_radius_x + surface_1_xLimit + m_surface_2_radius_x - surface_2_xLimit ) / 2;
            lens = new G4DisplacedSolid( m_name + "_preTrim", lens_preTranslation, nullptr, G4ThreeVector( 0, 0, -avgPos ) );

            m_relativePosition_front = G4ThreeVector( 0, 0, m_surface_2_radius_x - surface_2_xLimit - avgPos );
            m_relativePosition_back = G4ThreeVector( 0, 0, m_surface_1_radius_x + surface_1_xLimit - avgPos );
            m_relativePosition_center = ( m_relativePosition_front + m_relativePosition_back ) / 2;
        }
    } else if( m_shape == m_biconcave ) {
        G4Exception( "Lens::Lens", "InvalidSetup", FatalException, "Biconcave lens is not implemented ):" );
        // G4double middleTube_size = distance;

        // G4Ellipsoid* surface_1_init = new G4Ellipsoid( m_name + "_surface_1_init" , surface_1_radius_y, surface_1_radius_y, abs( surface_1_radius_x ) );
        // G4Ellipsoid* surface_2_init = new G4Ellipsoid( m_name + "_surface_2_init" , surface_2_radius_y, surface_2_radius_y, abs( surface_2_radius_x ) );
        // G4EllipticalTube* middleTube_init = new G4EllipticalTube( m_name + "_middleTube_init", surface_1_radius_y, surface_1_radius_y, middleTube_size / 2 );
        
        // G4Box* surface_1_boxOriginCut = new G4Box( m_name + "_surface_1_boxOriginCut", surface_1_radius_y, surface_1_radius_y, abs( surface_1_radius_x ) );
        // G4Box* surface_2_boxOriginCut = new G4Box( m_name + "_surface_2_boxOriginCut", surface_2_radius_y, surface_2_radius_y, abs( surface_2_radius_x ) );

        // G4SubtractionSolid* surface_1_originCut = new G4SubtractionSolid( m_name + "_surface_1_originCut", surface_1_init, surface_1_boxOriginCut, nullptr, G4ThreeVector( 0, 0, -surface_1_radius_x ) );
        // G4SubtractionSolid* surface_2_originCut = new G4SubtractionSolid( m_name + "_surface_2_originCut", surface_2_init, surface_2_boxOriginCut, nullptr, G4ThreeVector( 0, 0, -surface_2_radius_x ) );

        // G4double surface_1_width = surface_1_radius_x - surface_1_xLimit;
        // G4double surface_2_width = surface_2_radius_x - surface_2_xLimit;
        // G4cout << "surface_1_xLimit = " << surface_1_xLimit << G4endl;
        // G4cout << "surface_2_xLimit = " << surface_2_xLimit << G4endl;
        // G4cout << "surface_1_width  = " << surface_1_width  << G4endl;
        // G4cout << "surface_2_width  = " << surface_2_width  << G4endl;
        // G4cout << "surface_1_translation = " << surface_1_width / 2 + surface_1_radius_x << G4endl;
        // G4cout << "surface_2_translation = " << surface_2_width / 2 + surface_2_radius_x << G4endl;
        // G4cout << "surface_1_translation_2 = " << middleTube_size / 2 * -surface_1_radius_x_sign - surface_1_width / 2 << G4endl;
        // G4cout << "surface_2_translation_2 = " << middleTube_size / 2 * -surface_2_radius_x_sign + surface_2_width / 2 << G4endl;

        // G4Box* surface_1_box  = new G4Box( m_name + "_surface_1_box" , surface_1_radius_y, surface_1_radius_y, abs( surface_1_width ) / 2 );
        // G4Box* surface_2_box  = new G4Box( m_name + "_surface_2_box" , surface_2_radius_y, surface_2_radius_y, abs( surface_2_width ) / 2 );

        // // G4Exception( "Lens::Lens", "InvalidSetup", FatalException, "Lens shape is not valid." );

        // G4SubtractionSolid* surface_1_trim  = new G4SubtractionSolid( m_name + "_surface_1_originCut", surface_1_box, surface_1_originCut, nullptr, G4ThreeVector( 0, 0, surface_1_width / 2 + surface_1_radius_x ) );
        // G4SubtractionSolid* surface_2_trim  = new G4SubtractionSolid( m_name + "_surface_2_originCut", surface_2_box, surface_2_originCut, nullptr, G4ThreeVector( 0, 0, surface_2_width / 2 + surface_2_radius_x ) );

        // G4UnionSolid* temp = new G4UnionSolid( m_name + "_temp", middleTube_init, surface_1_trim, nullptr, G4ThreeVector( 0, 0, middleTube_size / 2 * -surface_1_radius_x_sign - surface_1_width / 2 ) );
        // lens_preTrim = new G4UnionSolid( m_name + "_solid", temp, surface_2_trim, nullptr, G4ThreeVector( 0, 0, middleTube_size / 2 * -surface_2_radius_x_sign - surface_2_width / 2 ) ) ;
    } else if( m_shape == m_concave_convex ) {
        G4Exception( "Lens::Lens", "InvalidSetup", FatalException, "Concave-convex lens is not implemented ):" );
    } else if( m_shape == m_convex_concave ) {
        G4double middleTube_size = m_distance - abs( m_surface_1_radius_x + surface_1_xLimit )
                                              + abs( m_surface_2_radius_x + surface_2_xLimit );
        G4EllipticalTube* middleTube_init = nullptr;
        if( middleTube_size > 0 ) {
            middleTube_size /= 2;
            middleTube_init = new G4EllipticalTube( m_name + "_middleTube_init", m_surface_1_radius_y, m_surface_1_radius_y, middleTube_size );
        }

        G4Ellipsoid* surface_1_init = new G4Ellipsoid( m_name + "_surface_1_init" , m_surface_1_radius_y, m_surface_1_radius_y, abs( m_surface_1_radius_x ) );
        G4Ellipsoid* surface_2_init = new G4Ellipsoid( m_name + "_surface_2_init" , m_surface_2_radius_y, m_surface_2_radius_y, abs( m_surface_2_radius_x ) );

        G4Box* surface_1_boxOriginCut = new G4Box( m_name + "_surface_1_boxOriginCut", m_surface_1_radius_y, m_surface_1_radius_y, abs( m_surface_1_radius_x ) );
        G4SubtractionSolid* surface_1_originCut = new G4SubtractionSolid( m_name + "_surface_1_originCut", surface_1_init, surface_1_boxOriginCut, nullptr, G4ThreeVector( 0, 0, -m_surface_1_radius_x ) );
        G4Box* surface_1_box  = new G4Box( m_name + "_surface_1_box" , m_surface_1_radius_y, m_surface_1_radius_y, abs( surface_1_xLimit ) );
        G4SubtractionSolid* surface_1_trim  = new G4SubtractionSolid( m_name + "_surface_1_originCut", surface_1_originCut, surface_1_box );

        G4SubtractionSolid* surface_1_trim_final = nullptr;
        G4SubtractionSolid* surface_2_trim_final = nullptr;
        G4SubtractionSolid* middleTube_trim_final = nullptr;
        if( m_circular ) {
            surface_1_trim_final = subtract_circular( surface_1_trim );
            surface_2_trim_final = subtract_circular( surface_2_init );
            if( middleTube_size > 0 )
                middleTube_trim_final = subtract_circular( middleTube_init );
        } else {
            surface_1_trim_final = subtract_rectangular( surface_1_trim );
            surface_2_trim_final = subtract_rectangular( surface_2_init );
            if( middleTube_size > 0 )
                middleTube_trim_final = subtract_rectangular( middleTube_init );
        }

        if( middleTube_size > 0 ) {
            G4UnionSolid* surface1_and_middle = new G4UnionSolid( m_name + "_surface1_and_middle", middleTube_trim_final, surface_1_trim_final, nullptr, G4ThreeVector( 0, 0, surface_1_xLimit - middleTube_size ) );
            G4SubtractionSolid* lens_preDisplacement = new G4SubtractionSolid( m_name + "_lens_preDisplacement", surface1_and_middle, surface_2_init, nullptr, G4ThreeVector( 0, 0, surface_2_xLimit + middleTube_size ) );
            G4double avgPos = ( m_surface_1_radius_x + surface_1_xLimit + m_surface_2_radius_x + surface_2_xLimit ) / 2;
            lens = new G4DisplacedSolid( m_name + "_lens", lens_preDisplacement, nullptr, G4ThreeVector( 0, 0, -avgPos ) );

            m_relativePosition_front = G4ThreeVector( 0, 0, middleTube_size - avgPos );
            m_relativePosition_back = G4ThreeVector( 0, 0, m_surface_1_radius_x + surface_1_xLimit - middleTube_size - avgPos );
            m_relativePosition_center = ( m_relativePosition_front + m_relativePosition_back ) / 2;
        } else {
            G4DisplacedSolid* surface_1_displaced = new G4DisplacedSolid( m_name + "_surface_1_displaced", surface_1_trim_final, nullptr, G4ThreeVector( 0, 0, surface_1_xLimit ) );
            G4SubtractionSolid* lens_preDisplacement = new G4SubtractionSolid( m_name + "_surface_1_displaced", surface_1_displaced, surface_2_init, nullptr, G4ThreeVector( 0, 0, -m_distance ) );
            G4double avgPos = ( -m_distance + m_surface_1_radius_x + surface_1_xLimit ) / 2;
            lens = new G4DisplacedSolid( m_name + "_lens", lens_preDisplacement, nullptr, G4ThreeVector( 0, 0, -avgPos ) );

            m_relativePosition_front = G4ThreeVector( 0, 0, -avgPos );
            m_relativePosition_back = G4ThreeVector( 0, 0, -m_distance - avgPos );
            m_relativePosition_center = ( m_relativePosition_front + m_relativePosition_back ) / 2;
        }
    } else
        G4Exception( "Lens::Lens", "InvalidSetup", FatalException, "Lens shape is not valid." );

    m_lens->set_solid( lens );
    // m_lens->set_solid( lens_preTrim );
    m_lens->set_material( m_material );
    m_lens->set_visAttributes( m_visAttributes );
    m_lens->make_logicalVolume();

    // Check if m_relativePosition_front, back, center match the values returned by calculate_relativePositions
    if( abs( m_relativePosition_front .z() - calculate_relativePositions( t_nLens )[ 0 ].z() ) > 1e-6 ||
        abs( m_relativePosition_back  .z() - calculate_relativePositions( t_nLens )[ 1 ].z() ) > 1e-6 ||
        abs( m_relativePosition_center.z() - calculate_relativePositions( t_nLens )[ 2 ].z() ) > 1e-6   )
        G4Exception( "Lens::Lens", "InvalidSetup", FatalException, 
                     "m_relativePosition_front, back, or center do not "
                     "match the values returned by calculate_relativePositions."
                     "This is not a user error, it is a code error." 
                   );
}

Lens::~Lens() {
    if( m_lens ) delete m_lens;
}

ostream& operator<<( ostream& t_os, Lens* t_lens )
{
    t_os << *t_lens;
    return t_os;
}

ostream& operator<<( ostream& t_os, const Lens& t_lens ) {
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
    m_rotationMatrix = t_rotationMatrix;
    m_translation    = t_translation;
    m_lens->place( t_rotationMatrix, t_translation, t_motherLogicalVolume, t_isMany );
}

G4double Lens::get_surfaceX( G4double t_radius_x, G4double t_radius_y, 
                           G4double t_yLimits , G4double t_position ) {
    G4int    sign = t_radius_x / abs( t_radius_x );
    G4double c_1  = 1 / pow( t_radius_x, 2 );
    G4double c_2  = -2 * t_position / pow( t_radius_x, 2 );
    G4double c_3  = pow( t_position, 2 ) / pow( t_radius_x, 2 ) + pow( t_yLimits, 2 ) / pow( t_radius_y, 2 ) - 1;

    return ( -c_2 + sign * sqrt( pow( c_2, 2 ) -4 * c_1 * c_3 ) ) / ( 2 * c_1 );
}

// GeometricObjectSubtractionSolid* Lens::get_geometricObject() {
GeometricObjectDisplacedSolid* Lens::get_geometricObject() {
    return m_lens;
}

G4String Lens::get_name() {
    return m_name;
}

G4LogicalVolume* Lens::get_logicalVolume() {
    return m_lens->get_logicalVolume();
}

LensSensitiveDetector* Lens::get_sensitiveDetector() {
    return m_lensSensitiveDetector;
}

void Lens::set_name( const G4String& t_name ) {
    m_name = t_name;
}

G4int Lens::get_shape_int() {
    return m_shape;
}

G4String Lens::get_shape_string() {
    return m_lensShape_map.at( m_shape );
}

G4SubtractionSolid* Lens::subtract_circular( G4VSolid* t_solid ) {
    G4double max_width    = m_surface_1_radius_y + m_surface_2_radius_y;
    G4double max_distance = abs( m_surface_1_radius_x ) + abs( m_surface_2_radius_x ) + abs( m_distance );

    G4EllipticalTube* tube_inner = new G4EllipticalTube( m_name + "_tube_inner", m_surface_1_yLimits, m_surface_1_yLimits, max_distance*4 );
    G4EllipticalTube* tube_outer = new G4EllipticalTube( m_name + "_tube_outer", max_width          , max_width          , max_distance*4 );

    G4SubtractionSolid* lens_trimmer = new G4SubtractionSolid( m_name + "_tube", tube_outer, tube_inner );
    return new G4SubtractionSolid( m_name + "_lens", t_solid, lens_trimmer );
}

G4SubtractionSolid* Lens::subtract_rectangular( G4VSolid* t_solid ) {
    G4double max_width    = m_surface_1_radius_y + m_surface_2_radius_y;
    G4double max_distance = abs( m_surface_1_radius_x ) + abs( m_surface_2_radius_x ) + abs( m_distance );

    G4Box* box_inner = new G4Box( m_name + "_box_inner", m_width / 2, m_width / 2, max_distance * 4 );
    G4Box* box_outer = new G4Box( m_name + "_box_outer", max_width  , max_width  , max_distance * 4 );

    G4SubtractionSolid* lens_trimmer = new G4SubtractionSolid( m_name + "_box", box_outer, box_inner );
    return new G4SubtractionSolid( m_name + "_lens", t_solid, lens_trimmer );
}

G4ThreeVector Lens::get_position( const char* t_relativePosition ) {
    G4String relativePosition( t_relativePosition );
    if( relativePosition == "front" || relativePosition == "f" )
        return get_position_front();
    else if( relativePosition == "back" || relativePosition == "b" )
        return get_position_back();
    else if( relativePosition == "center" || relativePosition == "c" )
        return get_position_center();
    else
        G4Exception( "Lens::get_position", "InvalidSetup", FatalException, "t_relativePosition is not valid." );
    return G4ThreeVector();
}

G4ThreeVector Lens::get_position_front() {
    return m_translation + *m_rotationMatrix * m_relativePosition_front;
}

G4ThreeVector Lens::get_position_back() {
    return m_translation + *m_rotationMatrix * m_relativePosition_back;
}

G4ThreeVector Lens::get_position_center() {
    return m_translation + *m_rotationMatrix * m_relativePosition_center;
}

G4ThreeVector Lens::get_position( const char      * t_relativePosition_given,
                                  G4RotationMatrix* t_rotationMatrix        ,
                                  G4ThreeVector     t_translation           ,
                                  const char      * t_relativePosition_want ,
                                  G4int             t_nLens                  ) {
    G4String relativePosition_given( t_relativePosition_given );
    to_lower( relativePosition_given );

    if( relativePosition_given == "front" || relativePosition_given == "f" )
        return get_position_front( t_rotationMatrix, t_translation, t_relativePosition_want, t_nLens );
    else if( relativePosition_given == "back" || relativePosition_given == "b" )
        return get_position_back( t_rotationMatrix, t_translation, t_relativePosition_want, t_nLens );
    else if( relativePosition_given == "center" || relativePosition_given == "c" )
        return get_position_center( t_rotationMatrix, t_translation, t_relativePosition_want, t_nLens );
    else
        G4Exception( "Lens::get_position", "InvalidSetup", FatalException, "t_relativePosition_given is not valid." );

    return G4ThreeVector();
}

G4ThreeVector Lens::get_position_front( G4RotationMatrix* t_rotationMatrix  ,
                                        G4ThreeVector     t_translation     ,
                                        const char      * t_relativePosition,
                                        G4int             t_nLens            ) {
    G4String relativePosition( t_relativePosition );
    to_lower( relativePosition );

    vector< G4ThreeVector > relativePositions = calculate_relativePositions( t_nLens );
    G4ThreeVector relativePosition_front  = relativePositions[ 0 ];
    G4ThreeVector relativePosition_back   = relativePositions[ 1 ];
    G4ThreeVector relativePosition_center = relativePositions[ 2 ];

    G4ThreeVector position = G4ThreeVector( 0, 0, ConstructionMessenger::get_instance()->get_lens_position( t_nLens ) );

    if( relativePosition == "front" || relativePosition == "f" )
        return t_translation;
    else if( relativePosition == "back" || relativePosition == "b" )
        return t_translation + *t_rotationMatrix * ( position - relativePosition_front );
    else if( relativePosition == "center" || relativePosition == "c" )
        return t_translation + *t_rotationMatrix * ( position - relativePosition_center + relativePosition_front );
    else if( relativePosition == "standard" || relativePosition == "s" )
        return t_translation + *t_rotationMatrix * ( position - relativePosition_back );
    else
        G4Exception( "Lens::get_position", "InvalidSetup", FatalException, "t_relativePosition is not valid." );

    return G4ThreeVector();
}

G4ThreeVector Lens::get_position_back( G4RotationMatrix* t_rotationMatrix  ,
                                       G4ThreeVector     t_translation     ,
                                       const char      * t_relativePosition,
                                       G4int             t_nLens            ) {
    G4String relativePosition( t_relativePosition );
    to_lower( relativePosition );

    vector< G4ThreeVector > relativePositions = calculate_relativePositions( t_nLens );
    G4ThreeVector relativePosition_front  = relativePositions[ 0 ];
    G4ThreeVector relativePosition_back   = relativePositions[ 1 ];
    G4ThreeVector relativePosition_center = relativePositions[ 2 ];

    G4ThreeVector position = G4ThreeVector( 0, 0, ConstructionMessenger::get_instance()->get_lens_position( t_nLens ) );

    if( relativePosition == "front" || relativePosition == "f" )
        return t_translation + *t_rotationMatrix * ( -relativePosition_front + relativePosition_back );
    else if( relativePosition == "back" || relativePosition == "b" )
        return t_translation + *t_rotationMatrix * ( position + relativePosition_back );
    else if( relativePosition == "center" || relativePosition == "c" )
        return t_translation + *t_rotationMatrix * ( -relativePosition_center + relativePosition_back );
    else
        G4Exception( "Lens::get_position", "InvalidSetup", FatalException, "t_relativePosition is not valid." );

    return G4ThreeVector();
}

G4ThreeVector Lens::get_position_center( G4RotationMatrix* t_rotationMatrix  ,
                                         G4ThreeVector     t_translation     ,
                                         const char      * t_relativePosition,
                                         G4int             t_nLens            ) {
    G4String relativePosition( t_relativePosition );
    to_lower( relativePosition );

    vector< G4ThreeVector > relativePositions = calculate_relativePositions( t_nLens );
    G4ThreeVector relativePosition_front  = relativePositions[ 0 ];
    G4ThreeVector relativePosition_back   = relativePositions[ 1 ];
    G4ThreeVector relativePosition_center = relativePositions[ 2 ];

    if( relativePosition == "front" || relativePosition == "f" )
        return t_translation + *t_rotationMatrix * ( -relativePosition_front + relativePosition_center );
    else if( relativePosition == "back" || relativePosition == "b" )
        return t_translation + *t_rotationMatrix * ( -relativePosition_back + relativePosition_center );
    else if( relativePosition == "center" || relativePosition == "c" )
        return t_translation;
    else
        G4Exception( "Lens::get_position", "InvalidSetup", FatalException, "t_relativePosition is not valid." );

    return G4ThreeVector();
}

vector< G4ThreeVector > Lens::calculate_relativePositions( G4int t_nLens ) {
    ConstructionMessenger* constructionMessenger = ConstructionMessenger::get_instance();

    if( t_nLens >= constructionMessenger->get_lens_amount() )
        G4Exception( "Lens::calculate_relativePositions", "InvalidSetup", FatalException, 
                     "t_nLens is greater than the number of lenses." );

    G4double surface_1_radius_x = constructionMessenger->get_lens_surface_1_radius_x( t_nLens );
    G4double surface_1_radius_y = constructionMessenger->get_lens_surface_1_radius_y( t_nLens );
    G4double surface_1_yLimits  = constructionMessenger->get_lens_surface_1_yLimits ( t_nLens );
    G4double surface_2_radius_x = constructionMessenger->get_lens_surface_2_radius_x( t_nLens );
    G4double surface_2_radius_y = constructionMessenger->get_lens_surface_2_radius_y( t_nLens );
    G4double surface_2_yLimits  = constructionMessenger->get_lens_surface_2_yLimits ( t_nLens );
    G4double distance           = constructionMessenger->get_lens_distance          ( t_nLens );

    G4double surface_1_xLimit = abs( get_surfaceX( surface_1_radius_x, surface_1_radius_y, surface_1_yLimits, 0 ) );
    G4double surface_2_xLimit = abs( get_surfaceX( surface_2_radius_x, surface_2_radius_y, surface_2_yLimits, 0 ) );

    G4double surface_1_radius_x_sign = surface_1_radius_x > 0 ? 1 : -1;
    G4double surface_2_radius_x_sign = surface_2_radius_x > 0 ? 1 : -1;

    G4int shape = -1;
    if( surface_1_radius_x < 0 && surface_2_radius_x > 0 )
        shape = m_biconvex;
    else if( surface_1_radius_x > 0 && surface_2_radius_x < 0 )
        shape = m_biconcave;
    else if( surface_1_radius_x > 0 && surface_2_radius_x > 0 )
        shape = m_concave_convex;
    else if( surface_1_radius_x < 0 && surface_2_radius_x < 0 )
        shape = m_convex_concave;
    else
        G4Exception( "Lens::calculate_relativePositions", "InvalidSetup", FatalException, "Lens shape is not valid." );

    G4ThreeVector relativePosition_front ;
    G4ThreeVector relativePosition_center;
    G4ThreeVector relativePosition_back  ;
    G4ThreeVector relativePosition_avg   ;

    if( shape == m_biconvex ) {
        G4double middleTube_size = distance + ( surface_1_radius_x + surface_1_xLimit ) 
                                            - ( surface_2_radius_x - surface_2_xLimit );
        middleTube_size /= 2;

       if( middleTube_size > 0 ) {
            G4double avgPos = ( surface_1_radius_x + surface_1_xLimit + surface_2_radius_x - surface_2_xLimit ) / 2;
            relativePosition_front = G4ThreeVector( 0, 0, middleTube_size - surface_2_xLimit + surface_2_radius_x - avgPos );
            relativePosition_back = G4ThreeVector( 0, 0, surface_1_xLimit - middleTube_size + surface_1_radius_x - avgPos );
            relativePosition_center = ( relativePosition_front + relativePosition_back ) / 2;
            relativePosition_avg = G4ThreeVector( 0, 0, avgPos );
        } else {
            G4double avgPos = ( surface_1_radius_x + surface_1_xLimit + surface_2_radius_x - surface_2_xLimit ) / 2;
            relativePosition_front = G4ThreeVector( 0, 0, surface_2_radius_x - surface_2_xLimit - avgPos );
            relativePosition_back = G4ThreeVector( 0, 0, surface_1_radius_x + surface_1_xLimit - avgPos );
            relativePosition_center = ( relativePosition_front + relativePosition_back ) / 2;
            relativePosition_avg = G4ThreeVector( 0, 0, avgPos );
        }
    } else if( shape == m_biconcave ) {
        G4Exception( "Lens::calculate_relativePositions", "InvalidSetup", FatalException, "Biconcave lens is not implemented ):" );
    } else if( shape == m_concave_convex ) {
        G4Exception( "Lens::calculate_relativePositions", "InvalidSetup", FatalException, "Concave-convex lens is not implemented ):" );
    } else if( shape == m_convex_concave ) {
        G4double middleTube_size = distance - abs( surface_1_radius_x + surface_1_xLimit )
                                            + abs( surface_2_radius_x + surface_2_xLimit );
        middleTube_size /= 2;

       if( middleTube_size > 0 ) {
            G4double avgPos = ( surface_1_radius_x + surface_1_xLimit + surface_2_radius_x + surface_2_xLimit ) / 2;
            relativePosition_front = G4ThreeVector( 0, 0, middleTube_size - avgPos );
            relativePosition_back = G4ThreeVector( 0, 0, surface_1_radius_x + surface_1_xLimit - middleTube_size - avgPos );
            relativePosition_center = ( relativePosition_front + relativePosition_back ) / 2;
            relativePosition_avg = G4ThreeVector( 0, 0, avgPos );
        } else {
            G4double avgPos = ( -distance + surface_1_radius_x + surface_1_xLimit ) / 2;
            relativePosition_front = G4ThreeVector( 0, 0, -avgPos );
            relativePosition_back = G4ThreeVector( 0, 0, -distance - avgPos );
            relativePosition_center = ( relativePosition_front + relativePosition_back ) / 2;
            relativePosition_avg = G4ThreeVector( 0, 0, avgPos );
        }    
    } else
        G4Exception( "Lens::calculate_relativePositions", "InvalidSetup", FatalException, "Lens shape is not valid." );

    return { relativePosition_front, relativePosition_back, relativePosition_center, relativePosition_avg };
}

G4RotationMatrix* Lens::get_rotationMatrix() {
    return m_rotationMatrix;
}

void Lens::set_sensitiveDetector( LensSensitiveDetector* t_lensSensitiveDetector ) {
    m_lensSensitiveDetector = t_lensSensitiveDetector;
    m_lens->set_sensitiveDetector( t_lensSensitiveDetector );
}