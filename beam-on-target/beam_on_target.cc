// --------------------------------------------------------------
//  GEANT 4 - The simplest possible beam on target simulation
// --------------------------------------------------------------
// Project started in 2020 November
// Shih-Kai Lin shihkailin78@gmail.com

#include "G4MTRunManager.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh" 
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

// Physics list
#include "QGSP_BERT_HP.hh"
#include "QGSP_BIC_HP.hh"

// User headers
#include "BeamOnTargetDetectorConstruction.hh"
#include "BeamOnTargetActionInitialization.hh"

int main(int argc, char** argv)
{
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(4); // Is equal to 2 by default
#else
    G4RunManager* runManager = new G4RunManager;
#endif

    //*** Set mandatory initialization classes:
    // - Detector construction
    // - Physics list
    // - User actions

    // First, detector construction
    BeamOnTargetDetectorConstruction* det = new BeamOnTargetDetectorConstruction();
    runManager->SetUserInitialization(det);
    
    // Physics list
    G4VModularPhysicsList* physicsList = new QGSP_BERT_HP;
    // G4VModularPhysicsList* physicsList = new QGSP_BIC_HP;
    physicsList->SetVerboseLevel(1);
    runManager->SetUserInitialization(physicsList);

    // User action initialization
    // In this steps, the following items are set up
    // - Particle gun
    runManager->SetUserInitialization(new BeamOnTargetActionInitialization());

    //*** Done with mandatory initializations

    // Initialize visualization manager
    G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    // Initialize : beam parameters + simulation parameters (geometry)
    // UImanager->ApplyCommand("/control/execute Macro/init_parameters.mac");

    if (argc!=1) {
        // batch mode
        //to apply the command : sahmri_simulation run.mac
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
        }

        else {
        // interactive mode : define UI session

        G4UIExecutive* ui = new G4UIExecutive(argc, argv);

        UImanager->ApplyCommand("/control/execute init_vis.mac");

        if (ui->IsGUI())
            // UImanager->ApplyCommand("/control/execute gui.mac");
            ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;

    return 0;
}
