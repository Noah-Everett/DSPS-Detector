//*/////////////////////////////////////////////////////////////////////////*//
//*//                    G4-DSPS-Detector-Simulation                      //*//
//*/////////////////////////////////////////////////////////////////////////*//
//*//                                                                     //*//
//*// Author:                                                             //*//
//*//   Noah Everett (noah.everett@mines.sdsmt.edu)                       //*//
//*//                                                                     //*//
//*/////////////////////////////////////////////////////////////////////////*//
//*//                                                                     //*//
//*/////////////////////////////////////////////////////////////////////////*//

#ifndef Lens_hh
#define Lens_hh

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4EllipticalTube.hh"

#include "GeometricObject.hh"
#include "ConstructionMessenger.hh"
#include "LensSensitiveDetector.hh"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using CLHEP    ::pi;
using std      ::vector;
using std      ::ostream;
using std      ::ifstream;
using std      ::stringstream;
using std      ::map;
using std      ::swap;
using G4StrUtil::to_lower ;
class Lens
{
    public:
        Lens( G4String, G4int );
       ~Lens();
        
        friend ostream& operator<<( ostream&,       Lens* );
        friend ostream& operator<<( ostream&, const Lens& );

        void place( G4RotationMatrix*, G4ThreeVector, G4LogicalVolume*, G4bool = false );

        G4String                         get_name             ();
        G4LogicalVolume                * get_logicalVolume    ();
        // GeometricObjectSubtractionSolid* get_geometricObject  ();
        GeometricObjectDisplacedSolid  * get_geometricObject  ();
        LensSensitiveDetector          * get_sensitiveDetector();
        G4int                            get_shape_int        ();
        G4String                         get_shape_string     ();
        G4RotationMatrix               * get_rotationMatrix   ();

        G4ThreeVector get_position        ( const char      * );
        G4ThreeVector get_position_front  (                   );
        G4ThreeVector get_position_center (                   );
        G4ThreeVector get_position_back   (                   );
        static G4ThreeVector get_position        ( const char      *, 
                                                   G4RotationMatrix*, 
                                                   G4ThreeVector    , 
                                                   const char      *,
                                                   G4int             );
        static G4ThreeVector get_position_front  ( G4RotationMatrix*, 
                                                   G4ThreeVector    , 
                                                   const char      *,
                                                   G4int             );
        static G4ThreeVector get_position_center ( G4RotationMatrix*,
                                                   G4ThreeVector    , 
                                                   const char      *,
                                                   G4int             );
        static G4ThreeVector get_position_back   ( G4RotationMatrix*,
                                                   G4ThreeVector    , 
                                                   const char      *,
                                                   G4int             );

        void set_name             ( const G4String&        );
        void set_sensitiveDetector( LensSensitiveDetector* );

    protected:
        // GeometricObjectSubtractionSolid* m_lens                 { new GeometricObjectSubtractionSolid() };
        // GeometricObjectUnionSolid      * m_lens                 { new GeometricObjectUnionSolid      () };
        GeometricObjectDisplacedSolid  * m_lens                 { new GeometricObjectDisplacedSolid  () };
        LensSensitiveDetector          * m_lensSensitiveDetector{ nullptr };
        ConstructionMessenger          * m_constructionMessenger{ ConstructionMessenger::get_instance() };
        G4ThreeVector                    m_size;
        G4String                         m_name;
        G4int                            m_shape;
        G4int                            m_surface_1_shape;
        G4int                            m_surface_2_shape;
        G4RotationMatrix               * m_rotationMatrix       { nullptr };
        G4ThreeVector                    m_translation;

        G4double         m_surface_1_radius_x;
        G4double         m_surface_1_radius_y;
        G4double         m_surface_1_yLimits ;
        G4double         m_surface_2_radius_x;
        G4double         m_surface_2_radius_y;
        G4double         m_surface_2_yLimits ;
        G4double         m_distance          ;
        G4double         m_position          ;
        G4String         m_material          ;
        G4bool           m_circular          ;
        G4VisAttributes* m_visAttributes     ;
        G4double         m_width             ;

        G4ThreeVector m_relativePosition_front ;
        G4ThreeVector m_relativePosition_center;
        G4ThreeVector m_relativePosition_back  ;
        static vector< G4ThreeVector > calculate_relativePositions( G4int );

        G4ThreeVector m_axis_y{ 0, 1, 0 };
        G4double m_pi_2 = 0.5 * pi;
        G4double m_pi   =       pi;

        G4SubtractionSolid* subtract_circular   ( G4VSolid* );
        G4SubtractionSolid* subtract_rectangular( G4VSolid* );

        static G4double get_surfaceX( G4double, G4double, G4double, G4double );

        enum m_lensShape_enum {
            m_biconvex,
            m_biconcave,
            m_convex_concave,
            m_concave_convex
        };

        const map< G4int, G4String > m_lensShape_map {
            { m_biconvex      , "biconvex"       },
            { m_biconcave     , "biconcave"      },
            { m_convex_concave, "convex_concave" },
            { m_concave_convex, "concave_convex" }
        };
};

#endif