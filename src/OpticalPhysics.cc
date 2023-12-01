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
#include "LUX_Run03.hh"

OpticalPhysics::OpticalPhysics( DetectorConstruction* t_detectorConstruction, 
                          const G4String& name                         ) :
    G4OpticalPhysics( 1, name ) {
    m_detectorConstruction = t_detectorConstruction;
}

OpticalPhysics::~OpticalPhysics() {
    G4cout << __FILE__ << " " << __LINE__ << G4endl;
    // delete m_NESTDetector;
    G4cout << __FILE__ << " " << __LINE__ << G4endl;
}

void OpticalPhysics::ConstructProcess() {
    if(verboseLevel > 0)
        G4cout << "LXeOpticalPhysics:: Add Optical Physics Processes using NEST" << G4endl;

    G4OpticalParameters* opticalParameters = G4OpticalParameters::Instance();

    G4ProcessManager* processManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
    if( !processManager )
        G4Exception( "G4OpticalPhysics::ConstructProcess()", "", FatalException, "Optical Photon without a Process Manager" );

    G4OpAbsorption* absorption = new G4OpAbsorption();
    if( opticalParameters->GetProcessActivation( "OpAbsorption" ) )
        processManager->AddDiscreteProcess( absorption );

    G4OpRayleigh* rayleigh = new G4OpRayleigh();
    if( opticalParameters->GetProcessActivation( "OpRayleigh" ) )
        processManager->AddDiscreteProcess( rayleigh );

    G4OpMieHG* mie = new G4OpMieHG();
    if( opticalParameters->GetProcessActivation( "OpMieHG" ) )
        processManager->AddDiscreteProcess( mie );

    G4OpBoundaryProcess* boundary = new G4OpBoundaryProcess();
    if( opticalParameters->GetProcessActivation( "OpBoundary" ) )
        processManager->AddDiscreteProcess( boundary );

    G4OpWLS* wls = new G4OpWLS();
    if( opticalParameters->GetProcessActivation( "OpWLS" ) )
        processManager->AddDiscreteProcess( wls );

    G4OpWLS2* wls2 = new G4OpWLS2();
    if( opticalParameters->GetProcessActivation( "OpWLS2" ) )
        processManager->AddDiscreteProcess( wls2 );

    NESTProc* scintillation = new NESTProc( "S1", fElectromagnetic, new NESTDetector( m_detectorConstruction ) );
    // NESTProc* scintillation = new NESTProc( "S1", fElectromagnetic, new DetectorExample_LUX_RUN03() );

    G4ParticleTable::G4PTblDicIterator* particleIterator = GetParticleIterator();
    particleIterator->reset();

    while( ( *particleIterator )() ) {
        G4ParticleDefinition* particle = particleIterator->value();

        processManager = particle->GetProcessManager();
        if( !processManager ) {
            G4ExceptionDescription exceptionDescription;
            exceptionDescription << "Particle " << particle->GetParticleName() << " without a Process Manager";
            G4Exception( "G4OpticalPhysics::ConstructProcess()", "", FatalException, exceptionDescription );
        }

        G4Cerenkov* cerenkov = new G4Cerenkov();
        if( cerenkov->IsApplicable( *particle ) && opticalParameters->GetProcessActivation( "Cerenkov" ) ) {
            processManager->AddProcess( cerenkov );
            processManager->SetProcessOrdering( cerenkov, idxPostStep );
        }

        if( scintillation->IsApplicable( *particle ) && opticalParameters->GetProcessActivation( "Scintillation" ) ) {
            processManager->AddProcess( scintillation );
            processManager->SetProcessOrderingToLast( scintillation, idxAtRest   );
            processManager->SetProcessOrderingToLast( scintillation, idxPostStep );
        }

        if( boundary->IsApplicable( *particle ) && opticalParameters->GetProcessActivation( "OpBoundary" ) ) {
            processManager->SetProcessOrderingToLast( boundary, idxPostStep );
        }
    }

    if( verboseLevel > 1 )
        PrintStatistics();
}

void OpticalPhysics::ConstructParticle() {
    G4OpticalPhoton::OpticalPhotonDefinition();
    NESTThermalElectron::Definition();
}