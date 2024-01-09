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

#ifndef MediumSensitiveDetector_hh
#define MediumSensitiveDetector_hh

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4Event.hh"

#include "OutputMessenger.hh"
#include "OutputManager.hh"
#include "MediumHit.hh"

using std::to_string;

class MediumSensitiveDetector : public G4VSensitiveDetector 
{
    public:
        MediumSensitiveDetector( G4String, G4int );
       ~MediumSensitiveDetector() override = default;

        void Initialize( G4HCofThisEvent* ) override;
        G4bool ProcessHits( G4Step*, G4TouchableHistory* ) override;

        G4ThreeVector         get_position           (                );
        G4RotationMatrix    * get_rotationMatrix     (                );
        MediumHitsCollection* get_hitsCollection     ( const G4Event* );
        G4String              get_hitsCollection_name(                );
        G4int                 get_hitsCollection_ID  (                );
        G4int                 get_ID                 (                );
        G4String              get_name               (                );

        void set_position         ( G4ThreeVector     );
        void set_rotationMatrix   ( G4RotationMatrix* );
        void set_hitsCollection_ID( G4int             );
    
    protected:
        G4String          m_name;
        G4ThreeVector     m_position;
        G4RotationMatrix* m_rotationMatrix;

        MediumHitsCollection* m_mediumHitsCollection   { nullptr };
        G4int                 m_mediumHitsCollection_ID{ -1      };

        G4int m_ID;
};

#endif