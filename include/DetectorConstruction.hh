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

#include "ConstructionMessenger.hh"
#include "Materials.hh"
#include "LensParameterFileReader.hh"
#include "PhotoSensorSensativeDetector.hh"
#include "GeometricObject.hh"
#include "LensSurface.hh"
#include "Lens.hh"
#include "LensSystem.hh"
#include "PhotoSensor.hh"
#include "LensSensativeDetector.hh"

#include <vector>
using std::vector;

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction( ConstructionMessenger*, Materials* );
   ~DetectorConstruction();

    G4VPhysicalVolume* Construct() override;
    
    void get_lensParameters();

    void print_parameters();

    void make_world();
    void make_detector();
    void make_calorimeter();
    void make_DSPD();

protected:
    G4bool m_checkOverlaps{ true };

    ConstructionMessenger  * m_constructionMessenger  { nullptr };
    Materials              * m_materials              { nullptr };
    LensParameterFileReader* m_lensParameterFileReader{ nullptr };
    
    GeometricObject* m_world              { new GeometricObject() };
    GeometricObject* m_detector_wall      { new GeometricObject() };
    GeometricObject* m_detector_medium    { new GeometricObject() };
    GeometricObject* m_detector           { new GeometricObject() };
    GeometricObject* m_calorimeter        { new GeometricObject() };
    GeometricObject* m_lensSystem         { new GeometricObject() };
    GeometricObject* m_photoSensor_surface{ new GeometricObject() };
    GeometricObject* m_photoSensor_body   { new GeometricObject() };
    GeometricObject* m_photoSensor        { new GeometricObject() };
};

#endif
