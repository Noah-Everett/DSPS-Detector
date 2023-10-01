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

#ifndef DetectorConstruction_h
#define DetectorConstruction_h

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
    DetectorConstruction();
   ~DetectorConstruction();

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

    void print_parameters();
    
    void make_GDMLFile( G4String );

protected:
    G4bool m_checkOverlaps{ true };

    G4ThreeVector m_axis_x{ 1.0, 0.0, 0.0 };
    G4ThreeVector m_axis_y{ 0.0, 1.0, 0.0 };
    G4ThreeVector m_axis_z{ 0.0, 0.0, 1.0 };

    G4double m_pi_2{ 0.5 * pi };
    G4double m_pi  { pi };

    ConstructionMessenger  * m_constructionMessenger  { ConstructionMessenger::get_instance() };
    Materials              * m_materials              { Materials::get_instance() };    
    G4GDMLParser           * m_GDMLParser             { new G4GDMLParser() };
    OutputManager          * m_outputManager          { OutputManager::get_instance() };
    
    G4VPhysicalVolume* m_world_physicalVolume{ nullptr };

    GeometricObjectBox* m_world             { new GeometricObjectBox() };
    GeometricObjectBox* m_detector_wall     { new GeometricObjectBox() };
    GeometricObjectBox* m_detector_medium   { new GeometricObjectBox() };

    vector< Calorimeter                    * > m_calorimeters_full;
    vector< Calorimeter                    * > m_calorimeters_middle;
    vector< DirectionSensitivePhotoDetector* > m_directionSensitivePhotoDetectors;
    vector< G4ThreeVector                    > m_directionSensitivePhotoDetectors_positions;
    vector< G4RotationMatrix                 > m_directionSensitivePhotoDetectors_rotationMatrices;

private: 
    void make_world   ();
    void make_detector();
    Calorimeter                    * make_calorimeter_full               ( G4String, G4String );
    Calorimeter                    * make_calorimeter_middle             ( G4String, G4String );
    DirectionSensitivePhotoDetector* make_directionSensitivePhotoDetector( G4String, G4String );

    void place_surface( G4ThreeVector, G4int );
};

#endif
