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

#include "ActionInitialization.hh"

ActionInitialization::ActionInitialization( DetectorConstruction* t_detectorConstruction ) 
    : m_detectorConstruction( t_detectorConstruction ) {
}

ActionInitialization::~ActionInitialization() {
}

void ActionInitialization::BuildForMaster() const {
    RunAction* runAction = new RunAction( m_detectorConstruction );
    SetUserAction( static_cast< G4UserRunAction* >( runAction ) );
}

void ActionInitialization::Build() const {
    PrimaryGeneratorAction* primaryGeneratorAction = new PrimaryGeneratorAction;
    SetUserAction( primaryGeneratorAction );
    ParticleGunMessenger::get_instance()->set_primaryGeneratorAction( primaryGeneratorAction                    );
    ParticleGunMessenger::get_instance()->set_particleGun           ( primaryGeneratorAction->get_particleGun() );

    RunAction* runAction = new RunAction( m_detectorConstruction );

    EventAction* eventAction = new EventAction( runAction, m_detectorConstruction );
    SetUserAction( static_cast< G4UserEventAction* >( eventAction ) );
    SetUserAction( static_cast< G4UserRunAction* >( runAction ) );

    SteppingAction* steppingAction = new SteppingAction( runAction );
    SetUserAction( steppingAction );

    StackingAction* stackingAction = new StackingAction();
    SetUserAction( stackingAction );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......