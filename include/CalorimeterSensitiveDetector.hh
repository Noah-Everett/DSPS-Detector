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

#ifndef CalorimeterSensitiveDetector_hh
#define CalorimeterSensitiveDetector_hh

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4Event.hh"

#include "OutputMessenger.hh"
#include "OutputManager.hh"
#include "CalorimeterHit.hh"
#include "Track.hh"

using std::to_string;

class CalorimeterSensitiveDetector : public G4VSensitiveDetector 
{
    public:
        CalorimeterSensitiveDetector( G4String, G4int );
       ~CalorimeterSensitiveDetector() override = default;

        void Initialize( G4HCofThisEvent* ) override;
        G4bool ProcessHits( G4Step*, G4TouchableHistory* ) override;

        G4String                   get_name               (                );
        G4int                      get_ID                 (                );
        G4ThreeVector              get_position           (                );
        G4RotationMatrix         * get_rotationMatrix     (                );
        CalorimeterHitsCollection* get_hitsCollection     ( const G4Event* );
        G4String                   get_hitsCollection_name(                );
        G4int                      get_hitsCollection_ID  (                );

        void set_position         ( G4ThreeVector     );
        void set_rotationMatrix   ( G4RotationMatrix* );
        void set_hitsCollection_ID( G4int             );
    
    protected:
        G4String          m_name;
        G4ThreeVector     m_position;
        G4RotationMatrix* m_rotationMatrix;

        CalorimeterHitsCollection* m_calorimeterHitsCollection   { nullptr };
        G4int                      m_calorimeterHitsCollection_ID{ -1      };

        G4int m_ID;
};

#endif