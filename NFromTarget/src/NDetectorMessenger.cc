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
// $Id: ExN02DetectorMessenger.cc,v 1.12 2008-09-22 16:41:20 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NDetectorMessenger.hh"

#include "NDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NDetectorMessenger::NDetectorMessenger(NDetectorConstruction* myDet)
:myDetector(myDet)
{ 

    NDir = new G4UIdirectory("/N/");
    NDir->SetGuidance("UI commands specific to this study.");
    
    detDir = new G4UIdirectory("/N/det/");
    detDir->SetGuidance("detector control.");

    TargetLenCmd = new G4UIcmdWithADoubleAndUnit("/N/det/setTargetThickness", this);
    TargetLenCmd->SetGuidance("Specify the thickness of the target.");
    TargetLenCmd->SetParameterName("thickness", false);
    TargetLenCmd->SetUnitCategory("Length");
    TargetLenCmd->AvailableForStates(G4State_PreInit, G4State_Idle);   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NDetectorMessenger::~NDetectorMessenger()
{
    delete TargetLenCmd;  
    delete detDir;
    delete NDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
    if( command == TargetLenCmd )
        myDetector->SetThickness(TargetLenCmd->GetNewDoubleValue(newValue));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
