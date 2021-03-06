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
//
/// \file NRunAction.cc
/// \brief Implementation of the NRunAction class

#include "NRunAction.hh"
#include "NPrimaryGeneratorAction.hh"
#include "NDetectorConstruction.hh"
// #include "NRun.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NRunAction::NRunAction()
: G4UserRunAction(),
  fEdep(0.),
  fEdep2(0.)
{ 
    // add new units for dose
    // 
    const G4double milligray = 1.e-3*gray;
    const G4double microgray = 1.e-6*gray;
    const G4double nanogray  = 1.e-9*gray;  
    const G4double picogray  = 1.e-12*gray;
    
    new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
    new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
    new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
    new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray); 

    // Register accumulable to the accumulable manager
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->RegisterAccumulable(fEdep);
    accumulableManager->RegisterAccumulable(fEdep2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NRunAction::~NRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NRunAction::BeginOfRunAction(const G4Run*)
{ 
    // inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(true);

    // reset accumulables to their initial values
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Reset();

    // Print the target thickness
    G4LogicalVolume* targetLV
        = G4LogicalVolumeStore::GetInstance()->GetVolume("Target");
    G4double targetThickness = 0;
    if(targetLV)
        targetThickness = ((G4Tubs*)(targetLV->GetSolid()))->GetZHalfLength()*2.;
    G4cout << "Target thickness: " << targetThickness << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NRunAction::EndOfRunAction(const G4Run* run)
{
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;

    // Merge accumulables 
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Merge();

    // Compute dose = total energy deposit in a run and its variance
    //
    G4double edep  = fEdep.GetValue();
    G4double edep2 = fEdep2.GetValue();
    
    G4double rms = edep2 - edep*edep/nofEvents;
    if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;  

    const NDetectorConstruction* detectorConstruction
    = static_cast<const NDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
    G4double dose = edep/mass;
    G4double rmsDose = rms/mass;

    // Run conditions
    //  note: There is no primary generator action object for "master"
    //        run manager for multi-threaded mode.
    const NPrimaryGeneratorAction* generatorAction
    = static_cast<const NPrimaryGeneratorAction*>
        (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
    G4String runCondition;
    if (generatorAction)
    {
        const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
        runCondition += particleGun->GetParticleDefinition()->GetParticleName();
        runCondition += " of ";
        G4double particleEnergy = particleGun->GetParticleEnergy();
        runCondition += G4BestUnit(particleEnergy,"Energy");
    }
            
    // Print
    //  
    if (IsMaster()) {
        G4cout
        << G4endl
        << "--------------------End of Global Run-----------------------";
    }
    else {
        G4cout
        << G4endl
        << "--------------------End of Local Run------------------------";
    }
    
    G4cout
        << G4endl
        << " The run consists of " << nofEvents << " "<< runCondition
        << G4endl
        << " Cumulated dose per run, in scoring volume : " 
        << G4BestUnit(dose,"Dose") << " rms = " << G4BestUnit(rmsDose,"Dose")
        << G4endl
        << "------------------------------------------------------------"
        << G4endl
        << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NRunAction::AddEdep(G4double edep)
{
    fEdep  += edep;
    fEdep2 += edep*edep;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

