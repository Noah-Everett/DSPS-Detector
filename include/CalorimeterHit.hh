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

#ifndef CalorimeterHit_hh
#define CalorimeterHit_hh

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

class CalorimeterHit : public G4VHit
{
    public:
        CalorimeterHit(                                                    );
        CalorimeterHit( const G4ThreeVector    &,       G4RotationMatrix* , 
                        const G4String         &,       G4int             ,
                        const G4ThreeVector    &, const G4double         &, 
                        const G4double         &, const G4double         &, 
                        const G4ThreeVector    &, const G4ThreeVector    & );
        CalorimeterHit( const CalorimeterHit   &                           );
       ~CalorimeterHit(                                                    ) override = default;

       inline void* operator new   ( size_t );
       inline void  operator delete( void*  );

        const CalorimeterHit& operator=( const CalorimeterHit& );

        void Draw () override;
        void Print() override;

        friend std::ostream& operator<<( std::ostream&, const CalorimeterHit& );
        friend std::ostream& operator<<( std::ostream&, const CalorimeterHit* );

        void set_calorimeter_position      (       G4ThreeVector     );
        void set_calorimeter_rotationMatrix(       G4RotationMatrix* );
        void set_calorimeter_name          ( const G4String&         );
        void set_calorimeter_ID            (       G4int             );
        void set_hit_position_absolute     (       G4ThreeVector     );
        void set_hit_time                  (       G4double          );
        void set_hit_energy                (       G4double          );
        void set_hit_momentum              (       G4ThreeVector     );
        void set_hit_process               ( const G4String&         );
        void set_particle_energy           (       G4double          );
        void set_particle_momentum         (       G4ThreeVector     );
        void set_particle_position_initial (       G4ThreeVector     );

        G4ThreeVector     get_calorimeter_position      ();
        G4RotationMatrix* get_calorimeter_rotationMatrix();
        G4String          get_calorimeter_name          ();
        G4int             get_calorimeter_ID            ();
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
        G4ThreeVector     m_calorimeter_position      ;
        G4RotationMatrix* m_calorimeter_rotationMatrix;
        G4String          m_calorimeter_name          ;
        G4int             m_calorimeter_ID            ;
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

using CalorimeterHitsCollection = G4THitsCollection< CalorimeterHit >;

extern G4ThreadLocal G4Allocator< CalorimeterHit >* CalorimeterHitAllocator;

inline void* CalorimeterHit::operator new( size_t ) {
    if( CalorimeterHitAllocator == nullptr )
        CalorimeterHitAllocator = new G4Allocator< CalorimeterHit >;

    return ( void* )CalorimeterHitAllocator->MallocSingle();
}

inline void CalorimeterHit::operator delete( void* t_hit ) {
    CalorimeterHitAllocator->FreeSingle( ( CalorimeterHit* ) t_hit );
}

#endif