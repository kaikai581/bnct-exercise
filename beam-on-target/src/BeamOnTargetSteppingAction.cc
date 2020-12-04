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
//
/// \file BeamOnTargetSteppingAction.cc
/// \brief Implementation of the BeamOnTargetSteppingAction class

#include "BeamOnTargetSteppingAction.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4Proton.hh"
#include "G4DNAGenericIonsManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BeamOnTargetSteppingAction::BeamOnTargetSteppingAction()
:G4UserSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BeamOnTargetSteppingAction::~BeamOnTargetSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BeamOnTargetSteppingAction::UserSteppingAction(const G4Step* aStep)
{
    const G4StepPoint* endPoint = aStep->GetPostStepPoint();
    G4String procName = endPoint->GetProcessDefinedStep()->GetProcessName();
  
    const std::vector<const G4Track*>* secondary 
                                    = aStep->GetSecondaryInCurrentStep();
    size_t nbtrk = (*secondary).size();
    if (nbtrk) {
        G4cout << "\n    :----- List of secondaries ----------------" << G4endl;
        G4cout << "    :----- Process Name: " << procName << " ----------------" << G4endl;
        G4cout.precision(4);
        for (size_t lp=0; lp<(*secondary).size(); lp++) {
            G4cout << "   "
                    << std::setw(13)                 
                    << (*secondary)[lp]->GetDefinition()->GetParticleName()
                    << ":  energy ="
                    << std::setw(6)
                    << G4BestUnit((*secondary)[lp]->GetKineticEnergy(),"Energy")
                    << "  time ="
                    << std::setw(6)
                    << G4BestUnit((*secondary)[lp]->GetGlobalTime(),"Time");
            G4cout << G4endl;
    }
            
    G4cout << "    :------------------------------------------\n" << G4endl;
    }

}
