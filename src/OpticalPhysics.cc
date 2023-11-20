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

#include "OpticalPhysics.hh"

OpticalPhysics::OpticalPhysics( DetectorConstruction* t_detectorConstruction, 
                          const G4String& name                         ) :
    G4OpticalPhysics( 1, name ) {
    m_NESTDetector = new NESTDetector( t_detectorConstruction );
}

OpticalPhysics::~OpticalPhysics() {
    delete m_detector;
}

void OpticalPhysics::ConstructProcess() {
    G4OpticalPhysics::ConstructProcess();

    NESTProc* theNEST2ScintillationProcess = new NESTProc( "S1", fElectromagnetic, m_NESTDetector );

    G4PTblDicIterator* particleIterator = GetParticleIterator();
    particleIterator->reset();
    G4ProcessManager* processManager{ nullptr };
    for( G4PTblDicIterator* particle = particleIterator->begin(); particle != particleIterator->end(); ++particle ) {
        processManager = particle->GetProcessManager();
        if( theNEST2ScintillationProcess->IsApplicable( *particle ) )
            processManager->AddProcess( theNEST2ScintillationProcess );
            processManager->SetProcessOrderingToLast( theNEST2ScintillationProcess, idxAtRest   );
            processManager->SetProcessOrderingToLast( theNEST2ScintillationProcess, idxPostStep );
    }
}

void OpticalPhysics::ConstructParticle() {
    G4OpticalPhoton::OpticalPhotonDefinition();
    NESTThermalElectron::Definition();
}