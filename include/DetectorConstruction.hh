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

#ifndef DetectorConstruction_hh
#define DetectorConstruction_hh

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GDMLParser.hh"

#include "ConstructionMessenger.hh"
#include "Materials.hh"
#include "GeometricObject.hh"
#include "Lens.hh"
#include "LensSystem.hh"
#include "PhotoSensor.hh"
#include "DirectionSensitivePhotoDetector.hh"
#include "Calorimeter.hh"
#include "OutputManager.hh"

#include <vector>
#include <string>

using std::vector;
using std::to_string;
using CLHEP::pi;

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        DetectorConstruction( G4bool = true );
       ~DetectorConstruction();

        G4VPhysicalVolume* Construct() override;
        void ConstructSDandField() override;

        void print_parameters();
        
        void make_GDMLFile( const G4String& );

        vector< Calorimeter                    * > get_calorimeters_full               () const;
        vector< Calorimeter                    * > get_calorimeters_middle             () const;
        vector< DirectionSensitivePhotoDetector* > get_directionSensitivePhotoDetectors() const;
        G4bool                                     get_make_SDandField                 () const;

    protected:
        G4bool m_checkOverlaps  { true };
        G4bool m_make_SDandField{ true };

        G4ThreeVector m_axis_x{ 1.0, 0.0, 0.0 };
        G4ThreeVector m_axis_y{ 0.0, 1.0, 0.0 };
        G4ThreeVector m_axis_z{ 0.0, 0.0, 1.0 };

        G4double m_pi_2{ 0.5 * pi };
        G4double m_pi  { pi };

        ConstructionMessenger  * m_constructionMessenger  { ConstructionMessenger::get_instance() };
        Materials              * m_materials              { Materials::get_instance() };    
        G4GDMLParser           * m_GDMLParser             { new G4GDMLParser() };
        
        G4VPhysicalVolume* m_world_physicalVolume{ nullptr };

        GeometricObjectBox* m_world             { new GeometricObjectBox() };
        GeometricObjectBox* m_detector_wall     { new GeometricObjectBox() };
        GeometricObjectBox* m_detector_medium   { new GeometricObjectBox() };

        vector< Calorimeter                    * > m_calorimeters_full;
        vector< Calorimeter                    * > m_calorimeters_middle;
        vector< DirectionSensitivePhotoDetector* > m_directionSensitivePhotoDetectors;

    private: 
        void make_world   ();
        void make_detector();
        Calorimeter                    * make_calorimeter_full               ( const G4String&, const G4String& );
        Calorimeter                    * make_calorimeter_middle             ( const G4String&, const G4String& );
        DirectionSensitivePhotoDetector* make_directionSensitivePhotoDetector( const G4String&, const G4String& );

        void place_surface( G4ThreeVector, G4int );
};

#endif
