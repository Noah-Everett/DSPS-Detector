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

#ifndef LensSystem_hh
#define LensSystem_hh

#include "G4PVPlacement.hh"

#include "GeometricObject.hh"
#include "Lens.hh"
#include "ConstructionMessenger.hh"

#include <vector>

using std::vector;

class LensSystem
{
    public:
        LensSystem( const G4String&, G4bool = true );
       ~LensSystem();

        void add_lens( Lens* );

        void place( G4RotationMatrix*, G4ThreeVector , G4LogicalVolume*, G4bool = false );

        vector< Lens* >  get_lenses(       ) const;
        Lens           * get_lens  ( G4int ) const;
        G4String         get_name  (       ) const;
        
        void sort_lenses();

        G4ThreeVector get_position       ( const char*  );
        G4ThreeVector get_position_front (              );
        G4ThreeVector get_position_center(              );
        G4ThreeVector get_position_back  (              );

        // static G4ThreeVector get_position       ( const char      *, 
        //                                           G4RotationMatrix*, 
        //                                           G4ThreeVector    , 
        //                                           const char      * );
        static G4ThreeVector get_position_front ( G4RotationMatrix*, 
                                                  G4ThreeVector    , 
                                                  const char      * );
        // static G4ThreeVector get_position_center( G4RotationMatrix*, 
        //                                           G4ThreeVector    , 
        //                                           const char      * );
        // static G4ThreeVector get_position_back  ( G4RotationMatrix*, 
        //                                           G4ThreeVector    , 
        //                                           const char      * );

        void set_name( const G4String& );

    protected:
        vector< Lens* > m_lenses;
        G4String        m_name;
        ConstructionMessenger* m_constructionMessenger{ ConstructionMessenger::get_instance() };

        static vector< G4int         > calculate_lensOrder        ();
        static vector< G4ThreeVector > calculate_relativePositions();
        static vector< G4ThreeVector > calculate_relativePositions( const vector< G4int >& );
        G4ThreeVector m_relativePosition_front ;
        G4ThreeVector m_relativePosition_center;
        G4ThreeVector m_relativePosition_back  ;

        G4RotationMatrix* m_rotationMatrix;
        G4ThreeVector     m_translationVector;
};

#endif
