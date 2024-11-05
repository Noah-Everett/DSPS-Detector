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
#include "DetectorConstruction.hh"
#include "ConstructionMessenger.hh"
#include "Materials.hh"
#include "OutputMessenger.hh"
#include "OutputManager.hh"
#include "PhotonCreator.inl"
#include "OpticalPhysics.hh"
#include "ParticleGunMessenger.hh"
#include "CommandLineArgumentManager.hh"

#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalParameters.hh"
#include "G4OpticalPhysics.hh"
#include "G4RunManagerFactory.hh"
#include "G4String.hh"
#include "G4Types.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4SDManager.hh"

int main(int argc, char** argv)
{
    // Initialize the messengers
    ConstructionMessenger* constructionMessenger = ConstructionMessenger::get_instance();
    OutputMessenger      * outputMessenger       = OutputMessenger      ::get_instance();
    ParticleGunMessenger * particleGunMessenger  = ParticleGunMessenger ::get_instance();

    // Initialize the UI manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // Read in arguments
    CommandLineArgumentManager* CLAManager = new CommandLineArgumentManager( argc, argv, { "-g", "-d", "-o", "-e" } );
    G4bool showGUI = ( argc == 1 || CLAManager->findArgument( "-g" ) ) ? true : false;
    if( CLAManager->findArgument_string( "-d" ) )
        UImanager->ApplyCommand( "/control/execute " + CLAManager->getArgument_string( "-d" ) );
    else
        UImanager->ApplyCommand( "/control/execute macros/parameters_detector.mac" );
    if( CLAManager->findArgument_string( "-o" ) )
        UImanager->ApplyCommand( "/control/execute " + CLAManager->getArgument_string( "-o" ) );
    else
        UImanager->ApplyCommand( "/control/execute macros/parameters_output.mac" );
    G4String pathToEventMacro;
    if( argc == 2 )
        pathToEventMacro = argv[ 1 ];
    else
        pathToEventMacro = ( CLAManager->findArgument_string( "-e" ) ) ? CLAManager->getArgument_string( "-e" ) : "macros/event.mac";

    // Initialize UI if needed
    G4UIExecutive* ui = nullptr;
    if( showGUI )
        ui = new G4UIExecutive( argc, argv );

    // Initialize the run manager
    G4RunManager* runManager = G4RunManagerFactory::CreateRunManager( G4RunManagerType::Default );
    runManager->SetVerboseLevel( 3 );

    // Build the detector and initialize SDs
    G4SDManager::GetSDMpointer()->SetVerboseLevel( 1 );
    Materials* materials = Materials::get_instance();
    DetectorConstruction* detectorConstruction = new DetectorConstruction( ( ui ) ? false : true );
    runManager->SetUserInitialization( detectorConstruction );

    // Initialize the physics list
    G4VModularPhysicsList* physicsList = new FTFP_BERT;
    physicsList->ReplacePhysics( new G4EmStandardPhysics_option4() );

    // Initialize the optical physics (with NEST)
    OpticalPhysics* opticalPhysics = new OpticalPhysics( detectorConstruction );
    // G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
    G4OpticalParameters* opticalParams = G4OpticalParameters::Instance();
    opticalParams->SetWLSTimeProfile( "delta" );
    opticalParams->SetScintTrackSecondariesFirst( true );
    opticalParams->SetCerenkovMaxPhotonsPerStep( 100 );
    opticalParams->SetCerenkovMaxBetaChange( 10.0 );
    opticalParams->SetCerenkovTrackSecondariesFirst( true );
    physicsList->RegisterPhysics( opticalPhysics );
    runManager->SetUserInitialization( physicsList );

    // Make photonCreator particle
    make_photonCreator();

    // Initialize the action initialization
    runManager->SetUserInitialization( new ActionInitialization( detectorConstruction ) );
    
    // Initialize the visulization manager
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    
    // Start the visualization if needed
    if( ui ) {
        UImanager->ApplyCommand( "/control/execute macros/vis.mac" );
        if( ui->IsGUI() )
            UImanager->ApplyCommand( "/control/execute macros/gui.mac" );
        ui->SessionStart();
        delete ui;
    } else
        UImanager->ApplyCommand( "/control/execute " + pathToEventMacro );
    
    // Save GDML file if needed
    if( outputMessenger->get_GDML_save() )
        detectorConstruction->make_GDMLFile( outputMessenger->get_GDML_fileName() );

    // Terminate job
    if( visManager )
        delete visManager;
    if( runManager )
        delete runManager;
    OutputMessenger      ::delete_instance();
    ConstructionMessenger::delete_instance();
    ParticleGunMessenger ::delete_instance();
    Materials            ::delete_instance();

    // DONE!
    G4cout << '\n' << "####################"
           << '\n' << "### Job done! (: ###"
           << '\n' << "### Goodbye!     ###"
           << '\n' << "####################" << G4endl;
    return 0;
}