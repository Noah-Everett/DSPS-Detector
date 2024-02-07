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

#ifndef MediumHit_hh
#define MediumHit_hh

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

class MediumHit : public G4VHit
{
    public:
        MediumHit(                                                );
        MediumHit( const G4ThreeVector&,       G4RotationMatrix* , 
                   const G4String     &,       G4int             ,
                   const G4ThreeVector&, const G4double         &, 
                   const G4double     &, const G4ThreeVector    &, 
                   const G4ThreeVector&,       G4bool             );
        MediumHit( const MediumHit    &                           );
       ~MediumHit(                                                ) override = default;

       inline void* operator new   ( size_t );
       inline void  operator delete( void*  );

        const MediumHit& operator=( const MediumHit& );

        void Draw () override;
        void Print() override;

        friend std::ostream& operator<<( std::ostream&, const MediumHit& );
        friend std::ostream& operator<<( std::ostream&, const MediumHit* );

        void set_medium_position          (       G4ThreeVector     );
        void set_medium_rotationMatrix    (       G4RotationMatrix* );
        void set_medium_name              ( const G4String&         );
        void set_medium_ID                (       G4int             );
        void set_hit_position_absolute    (       G4ThreeVector     );
        void set_hit_time                 (       G4double          );
        void set_hit_process              ( const G4String&         );
        void set_particle_energy          (       G4double          );
        void set_particle_momentum        (       G4ThreeVector     );
        void set_particle_position_initial(       G4ThreeVector     );
        void set_particle_transmittance   (       G4bool            );

        G4ThreeVector     get_medium_position          ();
        G4RotationMatrix* get_medium_rotationMatrix    ();
        G4String          get_medium_name              ();
        G4int             get_medium_ID                ();
        G4ThreeVector     get_hit_position_absolute    ();
        G4double          get_hit_time                 ();
        G4String          get_hit_process              ();
        G4double          get_particle_energy          ();
        G4ThreeVector     get_particle_momentum        ();
        G4ThreeVector     get_particle_position_initial();
        G4bool            get_particle_transmittance   ();

    protected:
        G4ThreeVector     m_medium_position          ;
        G4RotationMatrix* m_medium_rotationMatrix    ;
        G4String          m_medium_name              ;
        G4int             m_medium_ID                ;
        G4ThreeVector     m_hit_position_absolute    ;
        G4double          m_hit_time                 ;
        G4String          m_hit_process              ;
        G4double          m_particle_energy          ;
        G4ThreeVector     m_particle_momentum        ;
        G4ThreeVector     m_particle_position_initial;
        G4bool            m_particle_transmittance   ;

        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };
};

using MediumHitsCollection = G4THitsCollection< MediumHit >;

extern G4ThreadLocal G4Allocator< MediumHit >* MediumHitAllocator;

inline void* MediumHit::operator new( size_t ) {
    if( MediumHitAllocator == nullptr )
        MediumHitAllocator = new G4Allocator< MediumHit >;

    return ( void* )MediumHitAllocator->MallocSingle();
}

inline void MediumHit::operator delete( void* t_hit ) {
    MediumHitAllocator->FreeSingle( ( MediumHit* ) t_hit );
}

#endif