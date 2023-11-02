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

#ifndef PhotoSensorHit_h
#define PhotoSensorHit_h

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class PhotoSensorHit : public G4VHit
{
    public:
        PhotoSensorHit(                                                    );
        PhotoSensorHit( const G4ThreeVector    &, const G4String         &,
                              G4RotationMatrix* , const G4ThreeVector    &,
                        const G4double         &, const G4double         &,
                        const G4double         &, const G4ThreeVector    &,
                        const G4ThreeVector    &                           );
        PhotoSensorHit( const PhotoSensorHit   &                           );
       ~PhotoSensorHit(                                                    ) override = default;

       inline void* operator new   ( size_t );
       inline void  operator delete( void*  );

        const PhotoSensorHit& operator=( const PhotoSensorHit& );

        void Draw () override;
        void Print() override;

        friend std::ostream& operator<<( std::ostream&, const PhotoSensorHit& );
        friend std::ostream& operator<<( std::ostream&, const PhotoSensorHit* );

        void set_photoSensor_position      ( G4ThreeVector     );
        void set_photoSensor_name          ( G4String          );
        void set_photoSensor_rotationMatrix( G4RotationMatrix* );
        void set_hit_position              ( G4ThreeVector     );
        void set_hit_time                  ( G4double          );
        void set_hit_energy                ( G4double          );
        void set_hit_momentum              ( G4ThreeVector     );
        void set_particle_energy           ( G4double          );
        void set_particle_momentum         ( G4ThreeVector     );

        G4ThreeVector     get_photoSensor_position      ();
        G4String          get_photoSensor_name          ();
        G4RotationMatrix* get_photoSensor_rotationMatrix();
        G4ThreeVector     get_hit_position              ();
        G4double          get_hit_time                  ();
        G4double          get_hit_energy                ();
        G4ThreeVector     get_hit_momentum              ();
        G4double          get_particle_energy           ();
        G4ThreeVector     get_particle_momentum         ();

    private:
        G4ThreeVector     m_photoSensor_position      ;
        G4RotationMatrix* m_photoSensor_rotationMatrix;
        G4String          m_photoSensor_name          ;
        G4ThreeVector     m_hit_position              ;
        G4double          m_hit_time                  ;
        G4double          m_hit_energy                ;
        G4ThreeVector     m_hit_momentum              ;
        G4double          m_particle_energy           ;
        G4ThreeVector     m_particle_momentum         ;
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