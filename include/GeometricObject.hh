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

#ifndef GeometricObject_hh
#define GeometricObject_hh

#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VSensitiveDetector.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Ellipsoid.hh"
#include "G4EllipticalTube.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4DisplacedSolid.hh"

#include "ConstructionMessenger.hh"

#define GeometricObjectBox              GeometricObject< G4Box              >
#define GeometricObjectEllipsoid        GeometricObject< G4Ellipsoid        >
#define GeometricObjectEllipticalTube   GeometricObject< G4EllipticalTube   >
#define GeometricObjectSubtractionSolid GeometricObject< G4SubtractionSolid >
#define GeometricObjectUnionSolid       GeometricObject< G4UnionSolid       >
#define GeometricObjectDisplacedSolid   GeometricObject< G4DisplacedSolid   >

template< class SolidType >
class GeometricObject
{
    public:
        GeometricObject();
        GeometricObject( SolidType*, G4Material*, G4VSensitiveDetector* = nullptr, G4bool = true, G4VisAttributes* = nullptr );
       ~GeometricObject();

        void set_material         ( G4Material          * );
        void set_material         ( G4String              );
        void set_sensitiveDetector( G4VSensitiveDetector* );
        void set_solid            ( SolidType           * );
        void set_logicalVolume    ( G4LogicalVolume     * );
        void set_visibility       ( G4bool                );
        void set_visAttributes    ( G4VisAttributes     * );
        void set_colour           ( G4String              );
        void set_colour           ( G4Colour              );
        void set_alpha            ( G4double              );
        void set_forceSolid       ( G4bool                );

        void make_logicalVolume();

        G4PVPlacement* place( G4RotationMatrix*, G4ThreeVector, G4LogicalVolume*, G4bool = false );

        G4Material          * get_material         () const;
        G4String              get_material_name    () const;
        G4VSensitiveDetector* get_sensitiveDetector() const;
        SolidType           * get_solid            () const;
        G4LogicalVolume     * get_logicalVolume    () const;
        G4bool                get_visibility       () const;
        G4VisAttributes     * get_visAttributes    () const;
        G4String              get_name             () const;

    private:
        G4String              m_name             { ""      };
        G4Material          * m_material         { nullptr };
        G4VSensitiveDetector* m_sensitiveDetector{ nullptr };
        SolidType           * m_solid            { nullptr };
        G4LogicalVolume     * m_logicalVolume    { nullptr };
        G4int                 m_copyNumber       { 0       };
        G4bool                m_visibility       { true    };
        G4VisAttributes     * m_visAttributes    { nullptr };
        G4RotationMatrix    * m_rotationMatrix   { nullptr };
        G4ThreeVector         m_translationVector{ 0       };

        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };
};

#include "GeometricObject.inl"

#endif