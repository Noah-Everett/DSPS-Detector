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

#ifndef PhotoSensor_hh
#define PhotoSensor_hh

#include "globals.hh"
#include "G4Box.hh"

#include "GeometricObject.hh"
#include "PhotoSensorSensitiveDetector.hh"
#include "ConstructionMessenger.hh"

class PhotoSensor
{
    public:
        PhotoSensor( G4String );
       ~PhotoSensor();

        void place( G4RotationMatrix*, G4ThreeVector, G4LogicalVolume*, G4bool = false );

        G4String                      get_name             ();
        GeometricObjectBox          * get_surface          ();
        GeometricObjectBox          * get_body             ();
        PhotoSensorSensitiveDetector* get_sensitiveDetector();

        void set_sensitiveDetector( PhotoSensorSensitiveDetector* );
        void set_name             ( const G4String&               );

    protected:
        GeometricObjectBox          * m_surface              { new GeometricObjectBox()              };
        GeometricObjectBox          * m_body                 { new GeometricObjectBox()              };
        ConstructionMessenger       * m_constructionMessenger{ ConstructionMessenger::get_instance() };
        PhotoSensorSensitiveDetector* m_sensitiveDetector    { nullptr                               };

        G4RotationMatrix* m_rotationMatrix{ new G4RotationMatrix() };
        G4ThreeVector     m_position      { 0, 0, 0 };

        G4String m_name;
};

#endif
