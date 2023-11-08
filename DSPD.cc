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

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "ConstructionMessenger.hh"
#include "UIMessenger.hh"
#include "Materials.hh"
#include "OutputMessenger.hh"
#include "OutputManager.hh"
#include "PhotonCreator.inl"

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
    auto runManager = G4RunManagerFactory::CreateRunManager( G4RunManagerType::Default );

    // Build the detector and initialize SDs
    G4SDManager::GetSDMpointer()->SetVerboseLevel( 1 );
    Materials* materials = Materials::get_instance();
    DetectorConstruction* detectorConstruction = new DetectorConstruction( ( ui ) ? false : true );
    runManager->SetUserInitialization( detectorConstruction );

    // Initialize the physics list
    G4VModularPhysicsList* physicsList = new FTFP_BERT;
    physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());

    // Initialize the optical physics
    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
    auto opticalParams = G4OpticalParameters::Instance();
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
        G4cout << __FILE__ << " " << __LINE__ << G4endl;
    if( ui ) {
        G4cout << __FILE__ << " " << __LINE__ << G4endl;
        UImanager->ApplyCommand( "/control/execute macros/vis.mac" );
        G4cout << __FILE__ << " " << __LINE__ << G4endl;
        if( ui->IsGUI() ) {
            UImanager->ApplyCommand( "/control/execute macros/gui.mac" );
        G4cout << __FILE__ << " " << __LINE__ << G4endl;
            }
        G4cout << __FILE__ << " " << __LINE__ << G4endl;
        ui->SessionStart();
        G4cout << __FILE__ << " " << __LINE__ << G4endl;
        delete ui;
        G4cout << __FILE__ << " " << __LINE__ << G4endl;
    } else if( argc == 2 )
        UImanager->ApplyCommand( G4String( "/control/execute " ) + argv[ 1 ] );
    else if( argc == 4 )
        UImanager->ApplyCommand( G4String( "/control/execute " ) + argv[ 3 ] );
    
    // Save GDML file if needed
    if( outputMessenger->get_GDML_save() )
        detectorConstruction->make_GDMLFile( outputMessenger->get_GDML_fileName() );

    // Terminate job
    delete visManager;
    delete runManager;
    OutputMessenger      ::delete_instance();
    ConstructionMessenger::delete_instance();
    UIMessenger          ::delete_instance();
    Materials            ::delete_instance();

    // DONE!
    G4cout << '\n' << "####################"
           << '\n' << "### Job done! (: ###"
           << '\n' << "### Goodbye!     ###"
           << '\n' << "####################" << G4endl;
    return 0;
}