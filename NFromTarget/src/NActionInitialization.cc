/// \file NActionInitialization.cc
/// \brief Implementation of the NActionInitialization class

#include "NActionInitialization.hh"
#include "NPrimaryGeneratorAction.hh"
#include "NRunAction.hh"
#include "NEventAction.hh"
#include "NSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NActionInitialization::NActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NActionInitialization::~NActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NActionInitialization::BuildForMaster() const
{
  NRunAction* runAction = new NRunAction;
  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NActionInitialization::Build() const
{
  SetUserAction(new NPrimaryGeneratorAction);

  NRunAction* runAction = new NRunAction;
  SetUserAction(runAction);
  
  NEventAction* eventAction = new NEventAction(runAction);
  SetUserAction(eventAction);
  
  SetUserAction(new NSteppingAction(eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......