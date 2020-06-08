/// \file neutron_from_target.cc
/// \brief Main program of the simple study of neutron spectrum out of protons on target.

#include "NDetectorConstruction.hh"

// Multithreading support
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

// UI
#include "G4UIExecutive.hh"

int main(int argc, char** argv)
{
    // Detect interactive mode (if no arguments) and define UI session
    G4UIExecutive* ui = 0;
    if ( argc == 1 )
        ui = new G4UIExecutive(argc, argv);

    // Construct the default run manager
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
#else
    G4RunManager* runManager = new G4RunManager;
#endif

    // Set mandatory initialization classes
    // Detector construction
    runManager->SetUserInitialization(new NDetectorConstruction());
}