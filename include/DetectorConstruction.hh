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

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction( ConstructionMessenger*, Materials* );
   ~DetectorConstruction();

    G4VPhysicalVolume* Construct() override;
    
    void set_material( G4Material*& t_material, G4String t_name );

    void print_sizes();
    
    void make_world();
    void make_detector();
    void make_calorimeter();
    void make_DSPD();

protected:
    G4bool m_checkOverlaps{ true };

    ConstructionMessenger* m_constructionMessenger{ nullptr };
    Materials            * m_materials            { nullptr };
    
    G4Material* m_world_medium_material       { nullptr };
    G4Material* m_detector_wall_material      { nullptr };
    G4Material* m_calorimeter_material        { nullptr };
    G4Material* m_detector_medium_material    { nullptr };
    G4Material* m_lens_material               { nullptr };
    G4Material* m_photoSensor_surface_material{ nullptr };

    G4Box            * m_world_s               { nullptr };
    G4LogicalVolume  * m_world_lv              { nullptr };
    G4VPhysicalVolume* m_world_pv              { nullptr };
    G4Box            * m_detector_wall_s       { nullptr };
    G4LogicalVolume  * m_detector_wall_lv      { nullptr };
    G4VPhysicalVolume* m_detector_wall_pv      { nullptr };
    G4Box            * m_detector_medium_s     { nullptr };
    G4LogicalVolume  * m_detector_medium_lv    { nullptr };
    G4VPhysicalVolume* m_detector_medium_pv    { nullptr };
    G4Box            * m_calorimeter_s         { nullptr };
    G4LogicalVolume  * m_calorimeter_lv        { nullptr };
    G4VPhysicalVolume* m_calorimeter_pv        { nullptr };
    G4Box            * m_lens_s                { nullptr };
    G4LogicalVolume  * m_lens_lv               { nullptr };
    G4VPhysicalVolume* m_lens_pv               { nullptr };
    G4Box            * m_photoSensor_surface_s { nullptr };
    G4LogicalVolume  * m_photoSensor_surface_lv{ nullptr };
    G4VPhysicalVolume* m_photoSensor_surface_pv{ nullptr };
};

#endif
