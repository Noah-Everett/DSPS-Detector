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

#ifndef Medium_hh
#define Medium_hh

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4EllipticalTube.hh"

#include "GeometricObject.hh"
#include "ConstructionMessenger.hh"
#include "MediumSensitiveDetector.hh"

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

class Medium
{
    public:
        Medium( G4String, G4int, G4ThreeVector );
       ~Medium();
        
        friend ostream& operator<<( ostream&,       Medium* );
        friend ostream& operator<<( ostream&, const Medium& );

        void place( G4RotationMatrix*, G4ThreeVector, G4LogicalVolume*, G4bool = false );

        G4String                 get_name             ();
        G4LogicalVolume        * get_logicalVolume    ();
        GeometricObjectBox     * get_geometricObject  ();
        MediumSensitiveDetector* get_sensitiveDetector();
        G4RotationMatrix       * get_rotationMatrix   ();
        G4ThreeVector            get_position         ();
        G4ThreeVector            get_size             ();
        G4double                 get_size_x           ();
        G4double                 get_size_y           ();
        G4double                 get_size_z           ();

        void set_name             ( const G4String&          );
        void set_sensitiveDetector( MediumSensitiveDetector* );

    protected:
        GeometricObjectBox     * m_medium                 { nullptr };
        MediumSensitiveDetector* m_mediumSensitiveDetector{ nullptr };
        ConstructionMessenger  * m_constructionMessenger  { ConstructionMessenger::get_instance() };
        G4ThreeVector            m_size;
        G4String                 m_name;
        G4RotationMatrix       * m_rotationMatrix         { nullptr };
        G4ThreeVector            m_translation;
};

#endif