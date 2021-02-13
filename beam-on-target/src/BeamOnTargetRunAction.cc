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
// $Id: B4RunAction.cc 100946 2016-11-03 11:28:08Z gcosmo $
//
/// \file B4RunAction.cc
/// \brief Implementation of the B4RunAction class

#include "BeamOnTargetRunAction.hh"
#include "BeamOnTargetAnalysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BeamOnTargetRunAction::BeamOnTargetRunAction()
 : G4UserRunAction()
{ 
    // set printing event number per each event
    G4RunManager::GetRunManager()->SetPrintProgress(1);     

    // Create analysis manager
    // The choice of analysis technology is done via selectin of a namespace
    // in B4Analysis.hh
    auto analysisManager = G4AnalysisManager::Instance();
    G4cout << "Using " << analysisManager->GetType() << G4endl;

    // Create directories 
    //analysisManager->SetHistoDirectoryName("histograms");
    //analysisManager->SetNtupleDirectoryName("ntuple");
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetNtupleMerging(true);
        // Note: merging ntuples is available only with Root output

    // Book ntuple

    // Creating ntuple
    //
    // Create 1st ntuple (id = 0)
    analysisManager->CreateNtuple("secondary", "Secondary particles");
    analysisManager->CreateNtupleIColumn("event_id");
    analysisManager->CreateNtupleIColumn("parent_track_id");
    analysisManager->CreateNtupleIColumn("track_id");
    analysisManager->CreateNtupleSColumn("particle_name");
    analysisManager->CreateNtupleSColumn("process_name");
    analysisManager->CreateNtupleIColumn("process_subtype");
    analysisManager->CreateNtupleDColumn("posx");
    analysisManager->CreateNtupleDColumn("posy");
    analysisManager->CreateNtupleDColumn("posz");
    analysisManager->CreateNtupleDColumn("time");
    analysisManager->CreateNtupleDColumn("px");
    analysisManager->CreateNtupleDColumn("py");
    analysisManager->CreateNtupleDColumn("pz");
    analysisManager->CreateNtupleDColumn("ke");
    analysisManager->CreateNtupleDColumn("tot_e");
    analysisManager->CreateNtupleDColumn("theta_p");
    analysisManager->CreateNtupleDColumn("costheta_p");
    analysisManager->CreateNtupleDColumn("dirx");
    analysisManager->CreateNtupleDColumn("diry");
    analysisManager->CreateNtupleDColumn("dirz");
    analysisManager->FinishNtuple();
    // Create 2nd ntuple (id = 1)
    //
    analysisManager->CreateNtuple("nuclear_process", "Nuclear process the primary particle undergoes");
    analysisManager->CreateNtupleIColumn("event_id"); // column Id = 0
    analysisManager->CreateNtupleSColumn("equation"); // column Id = 1
    analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BeamOnTargetRunAction::~BeamOnTargetRunAction()
{
    delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BeamOnTargetRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
    //inform the runManager to save random number seed
    //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    
    // Get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Open an output file
    //
    G4String fileName = "p_on_be";
    analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BeamOnTargetRunAction::EndOfRunAction(const G4Run* /*run*/)
{
    // print histogram statistics
    //
    auto analysisManager = G4AnalysisManager::Instance();

    // save histograms & ntuple
    //
    analysisManager->Write();
    analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
