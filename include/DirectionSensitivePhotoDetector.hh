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

#ifndef DirectionSensitivePhotoDetector_hh
#define DirectionSensitivePhotoDetector_hh

#include "globals.hh"
#include "G4Material.hh"

#include "GeometricObject.hh"
#include "LensSystem.hh"
#include "PhotoSensor.hh"
#include "ConstructionMessenger.hh"

using std      ::to_string;
using G4StrUtil::to_lower ;

class DirectionSensitivePhotoDetector
{
    public:
        DirectionSensitivePhotoDetector( const G4String&, const G4String& );
       ~DirectionSensitivePhotoDetector();

        static G4ThreeVector get_size  ();
        static G4double      get_width ();
        static G4double      get_height();
        static G4double      get_depth ();
        
        G4String          get_name                ();
        G4RotationMatrix* get_rotationMatrix      ();
        G4LogicalVolume * get_parentLogicalVolume ();
        G4bool            get_isMany              ();
        G4ThreeVector     get_position_lensSystem ();
        G4ThreeVector     get_position_photoSensor();

        G4ThreeVector     get_position       ( const char* );
        G4ThreeVector     get_position_front (             );
        G4ThreeVector     get_position_back  (             );
        G4ThreeVector     get_position_center(             );

        static G4ThreeVector get_position_front (              G4RotationMatrix*, G4ThreeVector, const char* );
        static G4ThreeVector get_position_back  (              G4RotationMatrix*, G4ThreeVector, const char* );
        // static G4ThreeVector get_position_center(              G4RotationMatrix*, G4ThreeVector, const char* );
        // static G4ThreeVector get_position       ( const char*, G4RotationMatrix*, G4ThreeVector, const char* );

        LensSystem * get_lensSystem ();
        PhotoSensor* get_photoSensor();

        void set_name( const G4String& );
        
        void place( G4RotationMatrix*, G4ThreeVector, G4LogicalVolume*, G4bool, const char* );

    protected:
        LensSystem      * m_lensSystem         { nullptr };
        PhotoSensor     * m_photoSensor        { nullptr };

        G4String          m_name                          ;
        G4int             m_ID                            ;
        G4RotationMatrix* m_rotationMatrix     { nullptr };
        G4ThreeVector     m_position_lensSystem           ;
        G4ThreeVector     m_position_photoSensor          ;
        G4LogicalVolume * m_parentLogicalVolume{ nullptr };
        G4bool            m_isMany                        ;
};

#endif
