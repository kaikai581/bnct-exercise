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
/// \file BeamOnTargetStackingAction.cc
/// \brief Implementation of the BeamOnTargetStackingAction class

#include "BeamOnTargetAnalysis.hh"
#include "BeamOnTargetStackingAction.hh"

#include "G4Track.hh"
#include "G4NeutrinoE.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BeamOnTargetStackingAction::BeamOnTargetStackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BeamOnTargetStackingAction::~BeamOnTargetStackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
BeamOnTargetStackingAction::ClassifyNewTrack(const G4Track* track)
{
    auto analysisManager = G4AnalysisManager::Instance();
    int eid = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4cout << "Stacking action called on event " << eid << G4endl;
    //keep primary particle
    if (track->GetParentID() == 0) return fUrgent;

    // print some debug info
    G4cout << "event: " << eid << "\tparent track: " << track->GetParentID() << "\tcurrent track: " << track->GetTrackID() << "\tparticle name: " << track->GetDefinition()->GetParticleName();

    // save truth to file
    analysisManager->FillNtupleIColumn(0, 0, eid);
    analysisManager->FillNtupleIColumn(0, 1, track->GetParentID());
    analysisManager->FillNtupleIColumn(0, 2, track->GetTrackID());
    analysisManager->FillNtupleSColumn(0, 3, track->GetDefinition()->GetParticleName());
    analysisManager->FillNtupleSColumn(0, 4, track->GetCreatorProcess()->GetProcessName());
    analysisManager->FillNtupleIColumn(0, 5, track->GetCreatorProcess()->GetProcessSubType());
    analysisManager->FillNtupleDColumn(0, 6, track->GetPosition().x()/m);
    analysisManager->FillNtupleDColumn(0, 7, track->GetPosition().y()/m);
    analysisManager->FillNtupleDColumn(0, 8, track->GetPosition().z()/m);
    analysisManager->FillNtupleDColumn(0, 9, track->GetGlobalTime()/s);
    analysisManager->FillNtupleDColumn(0, 10, track->GetMomentum().x()/MeV);
    analysisManager->FillNtupleDColumn(0, 11, track->GetMomentum().y()/MeV);
    analysisManager->FillNtupleDColumn(0, 12, track->GetMomentum().z()/MeV);
    analysisManager->FillNtupleDColumn(0, 13, track->GetKineticEnergy()/MeV);
    analysisManager->FillNtupleDColumn(0, 14, track->GetTotalEnergy()/MeV);
    analysisManager->FillNtupleDColumn(0, 15, track->GetMomentum().theta());
    analysisManager->FillNtupleDColumn(0, 16, track->GetMomentum().cosTheta());
    analysisManager->FillNtupleDColumn(0, 17, track->GetMomentumDirection().x());
    analysisManager->FillNtupleDColumn(0, 18, track->GetMomentumDirection().y());
    analysisManager->FillNtupleDColumn(0, 19, track->GetMomentumDirection().z());
    analysisManager->AddNtupleRow(0);

    return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
