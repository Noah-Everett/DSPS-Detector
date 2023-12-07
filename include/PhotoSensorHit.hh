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

#ifndef PhotoSensorHit_hh
#define PhotoSensorHit_hh

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include "ConstructionMessenger.hh"

class PhotoSensorHit : public G4VHit
{
    public:
        PhotoSensorHit(                                                    );
        PhotoSensorHit( const G4ThreeVector    &,       G4RotationMatrix* , 
                        const G4String         &,       G4int             ,
                        const G4ThreeVector    &, const G4double         &, 
                        const G4double         &, const G4double         &, 
                        const G4ThreeVector    &, const G4ThreeVector    & );
        PhotoSensorHit( const PhotoSensorHit   &                           );
       ~PhotoSensorHit(                                                    ) override = default;

       inline void* operator new   ( size_t );
       inline void  operator delete( void*  );

        const PhotoSensorHit& operator=( const PhotoSensorHit& );

        void Draw () override;
        void Print() override;

        friend std::ostream& operator<<( std::ostream&, const PhotoSensorHit& );
        friend std::ostream& operator<<( std::ostream&, const PhotoSensorHit* );

        void set_photoSensor_position      (       G4ThreeVector     );
        void set_photoSensor_rotationMatrix(       G4RotationMatrix* );
        void set_photoSensor_name          ( const G4String&         );
        void set_photoSensor_ID            (       G4int             );
        void set_hit_position_absolute     (       G4ThreeVector     );
        void set_hit_time                  (       G4double          );
        void set_hit_energy                (       G4double          );
        void set_hit_momentum              (       G4ThreeVector     );
        void set_hit_process               ( const G4String&         );
        void set_particle_energy           (       G4double          );
        void set_particle_momentum         (       G4ThreeVector     );
        void set_particle_position_initial (       G4ThreeVector     );

        G4ThreeVector     get_photoSensor_position      ();
        G4RotationMatrix* get_photoSensor_rotationMatrix();
        G4String          get_photoSensor_name          ();
        G4int             get_photoSensor_ID            ();
        G4ThreeVector     get_hit_position_absolute     ();
        G4ThreeVector     get_hit_position_relative     ();
        G4double          get_hit_time                  ();
        G4double          get_hit_energy                ();
        G4ThreeVector     get_hit_momentum              ();
        G4String          get_hit_process               ();
        G4double          get_particle_energy           ();
        G4ThreeVector     get_particle_momentum         ();
        G4ThreeVector     get_particle_position_initial ();

    protected:
        G4ThreeVector     m_photoSensor_position      ;
        G4RotationMatrix* m_photoSensor_rotationMatrix;
        G4String          m_photoSensor_name          ;
        G4int             m_photoSensor_ID            ;
        G4ThreeVector     m_hit_position              ;
        G4double          m_hit_time                  ;
        G4double          m_hit_energy                ;
        G4ThreeVector     m_hit_momentum              ;
        G4String          m_hit_process               ;
        G4double          m_particle_energy           ;
        G4ThreeVector     m_particle_momentum         ;
        G4ThreeVector     m_particle_position_initial ;

        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };
};

using PhotoSensorHitsCollection = G4THitsCollection< PhotoSensorHit >;

extern G4ThreadLocal G4Allocator< PhotoSensorHit >* PhotoSensorHitAllocator;

inline void* PhotoSensorHit::operator new( size_t ) {
    if( PhotoSensorHitAllocator == nullptr )
        PhotoSensorHitAllocator = new G4Allocator< PhotoSensorHit >;

    return ( void* )PhotoSensorHitAllocator->MallocSingle();
}

inline void PhotoSensorHit::operator delete( void* t_hit ) {
    PhotoSensorHitAllocator->FreeSingle( ( PhotoSensorHit* ) t_hit );
}

#endif