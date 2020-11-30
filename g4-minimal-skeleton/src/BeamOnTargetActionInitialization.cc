/// \file BeamOnTargetActionInitialization.cc
/// \brief Implementation of the BeamOnTargetActionInitialization class

#include "BeamOnTargetActionInitialization.hh"
#include "BeamOnTargetPrimaryGeneratorAction.hh"
// #include "BeamOnTargetRunAction.hh"
// #include "BeamOnTargetEventAction.hh"
// #include "BeamOnTargetSteppingAction.hh"

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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BeamOnTargetActionInitialization::Build() const
{
    SetUserAction(new BeamOnTargetPrimaryGeneratorAction);

    // NRunAction* runAction = new NRunAction;
    // SetUserAction(runAction);
    
    // NEventAction* eventAction = new NEventAction(runAction);
    // SetUserAction(eventAction);
    
    // SetUserAction(new NSteppingAction(eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
