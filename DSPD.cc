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
#include "UIMessenger.hh"
#include "Materials.hh"
#include "OutputMessenger.hh"
#include "OutputManager.hh"
#include "PhotonCreator.inl"
#include "OpticalPhysics.hh"

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
    UIMessenger          * UImessenger           = UIMessenger          ::get_instance();
    OutputMessenger      * outputMessenger       = OutputMessenger      ::get_instance();

    // Execute parameter macros
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    if( argc == 3 ) {
        UImanager->ApplyCommand( G4String( "/control/execute " ) + argv[ 1 ] );
        UImanager->ApplyCommand( G4String( "/control/execute " ) + argv[ 2 ] );
        UImanager->ApplyCommand( G4String( "/control/execute " ) + argv[ 3 ] );
    } else {
        UImanager->ApplyCommand( G4String( "/control/execute macros/parameters_detector.mac" ) );
        UImanager->ApplyCommand( G4String( "/control/execute macros/parameters_GUI.mac"      ) );
        UImanager->ApplyCommand( G4String( "/control/execute macros/parameters_output.mac"   ) );
    }

    // Initialize UI if needed
    G4UIExecutive* ui = nullptr;
    if( UImessenger->get_showGUI() || argc == 1 )
        ui = new G4UIExecutive( argc, argv );

    // Initialize the run manager
    G4RunManager* runManager = G4RunManagerFactory::CreateRunManager( G4RunManagerType::Default );
    runManager->SetVerboseLevel( 3 );

    // G4Exception("main", "main", FatalException, "Fatal exception test.");

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
    } else if( argc == 2 )
        UImanager->ApplyCommand( G4String( "/control/execute " ) + argv[ 1 ] );
    else if( argc == 4 )
        UImanager->ApplyCommand( G4String( "/control/execute " ) + argv[ 3 ] );
    
    // Save GDML file if needed
    if( outputMessenger->get_GDML_save() )
        detectorConstruction->make_GDMLFile( outputMessenger->get_GDML_fileName() );

    // Terminate job
    G4cout << __FILE__ << " " << __LINE__ << G4endl;
    delete visManager;
    G4cout << __FILE__ << " " << __LINE__ << G4endl;
    delete runManager;
    G4cout << __FILE__ << " " << __LINE__ << G4endl;
    OutputMessenger      ::delete_instance();
    G4cout << __FILE__ << " " << __LINE__ << G4endl;
    ConstructionMessenger::delete_instance();
    G4cout << __FILE__ << " " << __LINE__ << G4endl;
    UIMessenger          ::delete_instance();
    G4cout << __FILE__ << " " << __LINE__ << G4endl;
    Materials            ::delete_instance();
    G4cout << __FILE__ << " " << __LINE__ << G4endl;

    // DONE!
    G4cout << '\n' << "####################"
           << '\n' << "### Job done! (: ###"
           << '\n' << "### Goodbye!     ###"
           << '\n' << "####################" << G4endl;
    return 0;
}