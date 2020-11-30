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
/// \file BeamOnTargetPrimaryGeneratorAction.cc
/// \brief Implementation of the BeamOnTargetPrimaryGeneratorAction class

#include "BeamOnTargetPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BeamOnTargetPrimaryGeneratorAction::BeamOnTargetPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0), 
  fTargetDisk(0)
{
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);

    // default particle kinematic
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition* particle
        = particleTable->FindParticle(particleName="proton");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    fParticleGun->SetParticleEnergy(5*MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BeamOnTargetPrimaryGeneratorAction::~BeamOnTargetPrimaryGeneratorAction()
{
    delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BeamOnTargetPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // this function is called at the begining of ecah event
    //

    // In order to avoid dependence of PrimaryGeneratorAction
    // on DetectorConstruction class we get Envelope volume
    // from G4LogicalVolumeStore.
    
    G4double targetThickness = 0;

    if (!fTargetDisk)
    {
        G4LogicalVolume* targetLV
        = G4LogicalVolumeStore::GetInstance()->GetVolume("Target");
        if ( targetLV ) fTargetDisk = (G4Tubs*)(targetLV->GetSolid());
    }

    if ( fTargetDisk )
        targetThickness = fTargetDisk->GetZHalfLength()*2.;
    else  {
        G4ExceptionDescription msg;
        msg << "Envelope volume of box shape not found.\n"; 
        msg << "Perhaps you have changed geometry.\n";
        msg << "The gun will be place at the center.";
        G4Exception("NPrimaryGeneratorAction::GeneratePrimaries()",
        "MyCode0002",JustWarning,msg);
    }
    
    fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, -targetThickness/2));

    fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
