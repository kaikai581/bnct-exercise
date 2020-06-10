/// \file NDetectorConstruction.cc
/// \brief Implementation of the NDetectorConstruction class

#include "NDetectorConstruction.hh"
#include "NDetectorMessenger.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include <cmath>

// Eye candy headers
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NDetectorConstruction::NDetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0),
  fThickness(1e-6*m)
{
    detectorMessenger = new NDetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NDetectorConstruction::~NDetectorConstruction()
{
    delete detectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* NDetectorConstruction::Construct()
{  
    // Get nist material manager
    G4NistManager* nist = G4NistManager::Instance();

    // Option to switch on/off checking of volumes overlaps
    //
    G4bool checkOverlaps = true;

    //     
    // World
    //
    G4double world_sizeXY = 80.*cm;
    G4double world_sizeZ  = 120.*cm;
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

    G4Box* solidWorld = new G4Box("World",                       //its name
                                  0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,          //its solid
                                                      world_mat,           //its material
                                                      "World");            //its name
                                   
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0,                     //no rotation
                                                     G4ThreeVector(),       //at (0,0,0)
                                                     logicWorld,            //its logical volume
                                                     "World",               //its name
                                                     0,                     //its mother  volume
                                                     false,                 //no boolean operation
                                                     0,                     //copy number
                                                     checkOverlaps);        //overlaps checking
                     
 
    //     
    // The disk target
    //  
    G4Material* target_mat = nist->FindOrBuildMaterial("G4_Li");

    // In micrometer. The first multiplicand is the target length in centimeter.
    G4cout << "thickness of target " << fThickness << G4endl;
    G4double targetHalfLen = fThickness/2;
    fSolidTarget = new G4Tubs("Target",
                              0*cm,
                              40*cm,
                              targetHalfLen,
                              0*deg,
                              360*deg);
                      
    G4LogicalVolume* logicTarget = new G4LogicalVolume(fSolidTarget,        //its solid
                                                       target_mat,          //its material
                                                       "Target");           //its name
    logicTarget->SetVisAttributes(new G4VisAttributes(G4Colour::Grey()));
               
    new G4PVPlacement(0,                                   //no rotation
                      G4ThreeVector(0, 0, targetHalfLen),  //at position
                      logicTarget,                         //its logical volume
                      "Target",                            //its name
                      logicWorld,                          //its mother volume
                      false,                               //no boolean operation
                      0,                                   //copy number
                      checkOverlaps);                      //overlaps checking

    // Set Shape2 as scoring volume
    //
    fScoringVolume = logicTarget;

    //
    //always return the physical World
    //
    return physWorld;
}

void NDetectorConstruction::SetThickness(G4double thickness)
{
    G4cout << "configured thickness " << thickness << G4endl;
    if (fSolidTarget && thickness > 0.)
    {
        fSolidTarget->SetZHalfLength(thickness/2);
        G4RunManager::GetRunManager()->GeometryHasBeenModified();
    }
}