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

#include "BeamOnTargetAnalysis.hh"
#include "BeamOnTargetSteppingAction.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4HadronicProcess.hh"
#include "G4Proton.hh"
#include "G4DNAGenericIonsManager.hh"
#include "G4SystemOfUnits.hh"

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
    // get the analysis manager to output data
    // auto analysisManager = G4AnalysisManager::Instance();
    // retrieve event id
    int eid = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    const G4StepPoint* endPoint = aStep->GetPostStepPoint();
    G4String procName = endPoint->GetProcessDefinedStep()->GetProcessName();
  
    const std::vector<const G4Track*>* secondary = aStep->GetSecondaryInCurrentStep();
    
    
    // Construct the process equation.
    // ref: https://gitlab.cern.ch/geant4/geant4/-/blob/master/examples/extended/hadronic/Hadr03/src/SteppingAction.cc
    G4VProcess* process = const_cast<G4VProcess*>(endPoint->GetProcessDefinedStep());

    // check whether a real interaction took place (eg. not a transportation)
    G4StepStatus stepStatus = endPoint->GetStepStatus();
    G4bool transmit = (stepStatus == fGeomBoundary || stepStatus == fWorldBoundary);
    if (transmit) return;

    //initialisation of the nuclear channel identification
    //
    G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition();
    G4String partName = particle->GetParticleName();
    G4String nuclearChannel = partName;
    G4HadronicProcess* hproc = dynamic_cast<G4HadronicProcess*>(process);
    const G4Isotope* target = NULL;
    if (hproc) target = hproc->GetTargetIsotope();
    G4String targetName = "XXXX";  
    if (target) targetName = target->GetName();
    if (targetName != "XXXX")
    {
        nuclearChannel += " + " + targetName + " --> ";

        //scattered primary particle (if any)
        //
        if (aStep->GetTrack()->GetTrackStatus() == fAlive)
            nuclearChannel += partName + " + ";
        
        //secondaries
        //
        std::map<G4ParticleDefinition*, G4int> particleFlag;
        for (size_t lp=0; lp<(*secondary).size(); lp++) {
            particle = (*secondary)[lp]->GetDefinition(); 
            //count e- from internal conversion together with gamma
            if (particle == G4Electron::Electron()) particle = G4Gamma::Gamma();
            //particle flag
            particleFlag[particle]++;
        }

        // nuclear channel
        const G4int kMax = 16;  
        const G4String conver[] = {"0","","2 ","3 ","4 ","5 ","6 ","7 ","8 ","9 ",
                                "10 ","11 ","12 ","13 ","14 ","15 ","16 "};
        std::map<G4ParticleDefinition*, G4int>::iterator ip;               
        for (ip = particleFlag.begin(); ip != particleFlag.end(); ip++) {
            particle = ip->first;
            G4String name = particle->GetParticleName();      
            G4int nb = ip->second;
            if (nb > kMax) nb = kMax;   
            G4String Nb = conver[nb];    
            if (particle == G4Gamma::Gamma()) {
                Nb = "N ";
                name = "gamma or e-";
            } 
            if (ip != particleFlag.begin()) nuclearChannel += " + ";
            nuclearChannel += Nb + name;
        }

        // analysis manager for outputting ntuple
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillNtupleIColumn(1, 0, eid);
        analysisManager->FillNtupleSColumn(1, 1, nuclearChannel);
        // When adding a row, one has to proved the correct ntuple ID.
        // ref: https://geant4-forum.web.cern.ch/t/problem-in-saving-data-to-ntuple-at-end-of-step/3230/2
        analysisManager->AddNtupleRow(1);
    }
    
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
