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

#ifndef LensHit_hh
#define LensHit_hh

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

class LensHit : public G4VHit
{
    public:
        LensHit(                                                    );
        LensHit( const G4ThreeVector    &,       G4RotationMatrix* , 
                        const G4String         &,       G4int             ,
                        const G4ThreeVector    &, const G4double         &, 
                        const G4double         &, const G4double         &, 
                        const G4ThreeVector    &, const G4ThreeVector    & );
        LensHit( const LensHit   &                           );
       ~LensHit(                                                    ) override = default;

       inline void* operator new   ( size_t );
       inline void  operator delete( void*  );

        const LensHit& operator=( const LensHit& );

        void Draw () override;
        void Print() override;

        friend std::ostream& operator<<( std::ostream&, const LensHit& );
        friend std::ostream& operator<<( std::ostream&, const LensHit* );

        void set_lens_position      (       G4ThreeVector     );
        void set_lens_rotationMatrix(       G4RotationMatrix* );
        void set_lens_name          ( const G4String&         );
        void set_lens_ID            (       G4int             );
        void set_hit_position_absolute     (       G4ThreeVector     );
        void set_hit_time                  (       G4double          );
        void set_hit_energy                (       G4double          );
        void set_hit_momentum              (       G4ThreeVector     );
        void set_hit_process               ( const G4String&         );
        void set_particle_energy           (       G4double          );
        void set_particle_momentum         (       G4ThreeVector     );
        void set_particle_position_initial (       G4ThreeVector     );
        void set_particle_transmittance    (       G4bool            );

        G4ThreeVector     get_lens_position      ();
        G4RotationMatrix* get_lens_rotationMatrix();
        G4String          get_lens_name          ();
        G4int             get_lens_ID            ();
        G4ThreeVector     get_hit_position_absolute     ();
        G4ThreeVector     get_hit_position_relative     ();
        G4double          get_hit_time                  ();
        G4double          get_hit_energy                ();
        G4ThreeVector     get_hit_momentum              ();
        G4String          get_hit_process               ();
        G4double          get_particle_energy           ();
        G4ThreeVector     get_particle_momentum         ();
        G4ThreeVector     get_particle_position_initial ();
        G4bool            get_particle_transmittance    ();

    protected:
        G4ThreeVector     m_lens_position      ;
        G4RotationMatrix* m_lens_rotationMatrix;
        G4String          m_lens_name          ;
        G4int             m_lens_ID            ;
        G4ThreeVector     m_hit_position              ;
        G4double          m_hit_time                  ;
        G4double          m_hit_energy                ;
        G4ThreeVector     m_hit_momentum              ;
        G4String          m_hit_process               ;
        G4double          m_particle_energy           ;
        G4ThreeVector     m_particle_momentum         ;
        G4ThreeVector     m_particle_position_initial ;
        G4bool            m_particle_transmittance    ;

        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };
};

using LensHitsCollection = G4THitsCollection< LensHit >;

extern G4ThreadLocal G4Allocator< LensHit >* LensHitAllocator;

inline void* LensHit::operator new( size_t ) {
    if( LensHitAllocator == nullptr )
        LensHitAllocator = new G4Allocator< LensHit >;

    return ( void* )LensHitAllocator->MallocSingle();
}

inline void LensHit::operator delete( void* t_hit ) {
    LensHitAllocator->FreeSingle( ( LensHit* ) t_hit );
}

#endif