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

#ifndef Calorimeter_hh
#define Calorimeter_hh

#include "globals.hh"
#include "G4Material.hh"

#include "GeometricObject.hh"
#include "ConstructionMessenger.hh"
#include "CalorimeterSensitiveDetector.hh"

class Calorimeter
{
    public:
        Calorimeter( G4String, G4String, G4ThreeVector );
       ~Calorimeter();

        static G4ThreeVector get_size  ();
        static G4double      get_width ();
        static G4double      get_height();
        static G4double      get_depth ();
        
        G4String         get_name         ();
        G4LogicalVolume* get_logicalVolume();

        void place( G4RotationMatrix*, G4ThreeVector, G4LogicalVolume*, G4bool );

    protected:
        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };

        GeometricObjectBox* m_calorimeter{ nullptr };
        CalorimeterSensitiveDetector* m_calorimeterSensitiveDetector{ nullptr };

        G4String m_name;
};

#endif
