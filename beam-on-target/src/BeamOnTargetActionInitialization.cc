/// \file BeamOnTargetActionInitialization.cc
/// \brief Implementation of the BeamOnTargetActionInitialization class

#include "BeamOnTargetActionInitialization.hh"
#include "BeamOnTargetPrimaryGeneratorAction.hh"
#include "BeamOnTargetRunAction.hh"
// #include "BeamOnTargetEventAction.hh"
#include "BeamOnTargetSteppingAction.hh"
#include "BeamOnTargetStackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BeamOnTargetActionInitialization::BeamOnTargetActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BeamOnTargetActionInitialization::~BeamOnTargetActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BeamOnTargetActionInitialization::BuildForMaster() const
{
    // BeamOnTargetRunAction* runAction = new BeamOnTargetRunAction;
    // SetUserAction(runAction);
    SetUserAction(new BeamOnTargetRunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BeamOnTargetActionInitialization::Build() const
{
    SetUserAction(new BeamOnTargetPrimaryGeneratorAction);

    // NRunAction* runAction = new NRunAction;
    // SetUserAction(runAction);
    SetUserAction(new BeamOnTargetRunAction);
    
    // NEventAction* eventAction = new NEventAction(runAction);
    // SetUserAction(eventAction);
    
    SetUserAction(new BeamOnTargetSteppingAction);
    //***** The stacking action below leads to core dump.
    //***** Exactly same thing happens to my Geant4Py python script as well.
    // SetUserAction(new BeamOnTargetStackingAction);
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
