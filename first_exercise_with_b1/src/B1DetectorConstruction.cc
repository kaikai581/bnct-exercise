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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//#include "globals.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"

#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include <cmath>

//#include "globals.hh"
//#include "G4ThreeVector.hh"
//#include <CLHEP/Vector/Rotation.h>

// Eye candy headers
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  // G4double env_sizeXY = 20*cm, env_sizeZ = 30*cm;
  // G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 80.*cm;
  G4double world_sizeZ  = 120.*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  // G4Box* solidEnv =    
  //   new G4Box("Envelope",                    //its name
  //       0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  // G4LogicalVolume* logicEnv =                         
  //   new G4LogicalVolume(solidEnv,            //its solid
  //                       env_mat,             //its material
  //                       "Envelope");         //its name
               
  // new G4PVPlacement(0,                       //no rotation
  //                   G4ThreeVector(),         //at (0,0,0)
  //                   logicEnv,                //its logical volume
  //                   "Envelope",              //its name
  //                   logicWorld,              //its mother  volume
  //                   false,                   //no boolean operation
  //                   0,                       //copy number
  //                   checkOverlaps);          //overlaps checking
 
  //     
  // Shifter
  //  
  G4Material* shifter_mat = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
  G4ThreeVector pos_shifter = G4ThreeVector(0, 0, (14/2)*cm);

  //beam(10cm diameter, 82cm long) plus target(2cm lithium)
   G4double BeaminnerRadius = 0.*cm; G4double BeamouterRadius = 10/2.*cm; G4double Beamhz = (82+2)/2.*cm;
   G4double BeamstartAngle = 0.*deg; G4double BeamspanningAngle = 360.*deg;
   G4Tubs* BeamPlusTar
     = new G4Tubs("BeamPlusTar",
               BeaminnerRadius,
               BeamouterRadius,
               Beamhz,
               BeamstartAngle,
               BeamspanningAngle);

  //positon and orientation of BeamTarget relative to shiter cone 
   G4RotationMatrix* yBeamRot = new G4RotationMatrix; // Rotates X and Z axes only 
    yBeamRot->rotateY(-M_PI/4.*rad); // Rotates 45 degrees 
  // yBeamRot->rotateY(0.*rad); // Rotates 45 degrees 
   // G4ThreeVector zBeamTrans(0, 0, 0);   
   //G4ThreeVector zBeamTrans(((((-84-2)/2.)+1.)/sqrt(2))*cm, 0, (-((14+82+2)/2.)+1.)*cm); 
   
   G4ThreeVector zxBeamTrans(-(41./(sqrt(2)))*cm, 0, (-((14+82+2)/2.)+1.)*cm);
        
  // Conical section for shifter       
  G4double shifterwosubeam_rmina =  0.*cm, shifterwosubeam_rmaxb = (6+43/3.)*cm;
  G4double shifterwosubeam_rminb =  0.*cm, shifterwosubeam_rmaxa = 25.*cm;
  G4double shifterwosubeam_hz = (14/2)*cm;
  G4double shifterwosubeam_phimin = 0.*deg, shifterwosubeam_phimax = 360.*deg;
  G4Cons* solidShifterwosubeam =    
    new G4Cons("Shifterwosubeam", 
    shifterwosubeam_rmina, shifterwosubeam_rmaxa, shifterwosubeam_rminb, shifterwosubeam_rmaxb, 
    shifterwosubeam_hz, shifterwosubeam_phimin, shifterwosubeam_phimax);

  //Conical section for shifter subtract beamplustatget
  G4SubtractionSolid* solidShifter =
    new G4SubtractionSolid("solidShifter", solidShifterwosubeam, BeamPlusTar, yBeamRot, zxBeamTrans);


                      
  G4LogicalVolume* logicShifter =                         
    new G4LogicalVolume(solidShifter,         //its solid
                        shifter_mat,          //its material
                        "Shifter");           //its name
  logicShifter->SetVisAttributes(new G4VisAttributes(G4Colour::Grey()));
               
  new G4PVPlacement(0,                       //no rotation
                    pos_shifter,                    //at position
                    logicShifter,             //its logical volume
                    "Shifter",                //its name
                    logicWorld,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking






  // //     
  // // Shape 2
  // //
  // G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
  // G4ThreeVector pos2 = G4ThreeVector(0, -1*cm, 7*cm);

  // // Trapezoid shape       
  // G4double shape2_dxa = 12*cm, shape2_dxb = 12*cm;
  // G4double shape2_dya = 10*cm, shape2_dyb = 16*cm;
  // G4double shape2_dz  = 6*cm;      
  // G4Trd* solidShape2 =    
  //   new G4Trd("Shape2",                      //its name
  //             0.5*shape2_dxa, 0.5*shape2_dxb, 
  //             0.5*shape2_dya, 0.5*shape2_dyb, 0.5*shape2_dz); //its size
                
  // G4LogicalVolume* logicShape2 =                         
  //   new G4LogicalVolume(solidShape2,         //its solid
  //                       shape2_mat,          //its material
  //                       "Shape2");           //its name
               
  // new G4PVPlacement(0,                       //no rotation
  //                   pos2,                    //at position
  //                   logicShape2,             //its logical volume
  //                   "Shape2",                //its name
  //                   logicWorld,                //its mother  volume
  //                   false,                   //no boolean operation
  //                   0,                       //copy number
  //                   checkOverlaps);          //overlaps checking

  //     
  // Fast_Neutron_Filter
  //  
  G4Material* fnfilter_mat = nist->FindOrBuildMaterial("G4_Ni"); //Ni
  G4ThreeVector pos_fnfilter = G4ThreeVector(0, 0, (14+30/2)*cm);
        
  // Conical section for fast neutron filter       
  G4double fnfilter_rmina =  0.*cm, fnfilter_rmaxb = (6+13/3.)*cm;
  G4double fnfilter_rminb =  0.*cm, fnfilter_rmaxa = (6+43/3.)*cm;
  G4double fnfilter_hz = (30/2)*cm;
  G4double fnfilter_phimin = 0.*deg, fnfilter_phimax = 360.*deg;
  G4Cons* solidFnfilter =    
    new G4Cons("Fnfilter", 
    fnfilter_rmina, fnfilter_rmaxa, fnfilter_rminb, fnfilter_rmaxb, fnfilter_hz,
    fnfilter_phimin, fnfilter_phimax);
                      
  G4LogicalVolume* logicFnfilter =                         
    new G4LogicalVolume(solidFnfilter,         //its solid
                        fnfilter_mat,          //its material
                        "Fnfilter");           //its name
  logicFnfilter->SetVisAttributes(new G4VisAttributes(G4Colour::Yellow()));
               
  new G4PVPlacement(0,                       //no rotation
                    pos_fnfilter,                    //at position
                    logicFnfilter,             //its logical volume
                    "Fnfilter",                //its name
                    logicWorld,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


// Gamma_Ray_Sheilding
  //  
  G4Material* grshielding_mat = nist->FindOrBuildMaterial("G4_Bi"); //Bi
  G4ThreeVector pos_grshielding = G4ThreeVector(0, 0, (14+30+3.5/2)*cm);
        
  // Conical section for Gamma_Ray_Sheilding       
  G4double grshielding_rmina =  0.*cm, grshielding_rmaxa = (6+13/3.)*cm;
  G4double grshielding_rminb =  0.*cm, grshielding_rmaxb = (6+9.5/3.)*cm;
  G4double grshielding_hz = (3.5/2)*cm;
  G4double grshielding_phimin = 0.*deg, grshielding_phimax = 360.*deg;
  G4Cons* solidGrshielding =    
    new G4Cons("Grshielding", 
    grshielding_rmina, grshielding_rmaxa, grshielding_rminb, grshielding_rmaxb, grshielding_hz,
    grshielding_phimin, grshielding_phimax);
                      
  G4LogicalVolume* logicGrshielding =                         
    new G4LogicalVolume(solidGrshielding,         //its solid
                        grshielding_mat,          //its material
                        "Grshielding");           //its name
  logicGrshielding->SetVisAttributes(new G4VisAttributes(G4Colour(255/255.,183/255.,169/255.)));

  new G4PVPlacement(0,                       //no rotation
                    pos_grshielding,                    //at position
                    logicGrshielding,             //its logical volume
                    "Grshielding",                //its name
                    logicWorld,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


// thermal_neutron_absorber
  //  
  G4Material* thneuabs_mat = nist->FindOrBuildMaterial("G4_Cd"); //Cd
  G4ThreeVector pos_thneuabs = G4ThreeVector(0, 0, (14+30+3.5+0.1/2)*cm);
        
  // Conical section for thermal_neutron_absorber      
  G4double thneuabs_rmina =  0.*cm, thneuabs_rmaxb = (6+9.4/3.)*cm;
  G4double thneuabs_rminb =  0.*cm, thneuabs_rmaxa = (6+9.5/3.)*cm;
  G4double thneuabs_hz = (0.1/2)*cm;
  G4double thneuabs_phimin = 0.*deg, thneuabs_phimax = 360.*deg;
  G4Cons* solidThneuabs =    
    new G4Cons("Thneuabs", 
    thneuabs_rmina, thneuabs_rmaxa, thneuabs_rminb, thneuabs_rmaxb, thneuabs_hz,
    thneuabs_phimin, thneuabs_phimax);
                      
  G4LogicalVolume* logicThneuabs =                         
    new G4LogicalVolume(solidThneuabs,         //its solid
                        thneuabs_mat,          //its material
                        "Thneuabs");           //its name
  logicThneuabs->SetVisAttributes(new G4VisAttributes(G4Colour::Red()));

               
  new G4PVPlacement(0,                       //no rotation
                    pos_thneuabs,                    //at position
                    logicThneuabs,             //its logical volume
                    "Thneuabs",                //its name
                    logicWorld,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking



// full cone minus poly-Li(delimiter)
  //  
  //G4Material* aircone_mat = nist->FindOrBuildMaterial("G4_Cd"); //Cd
  G4ThreeVector pos_fullcone_minus_polyLi = G4ThreeVector(0, 0, ((14+30+3.5+0.1+9.4-2.0)/2)*cm);
        
  // Conical section minus poly-Li(delimiter)       
  G4double fullconemp_rmina =  0.*cm, fullconemp_rmaxa = (50/2.)*cm;
  G4double fullconemp_rminb =  0.*cm, fullconemp_rmaxb = (6+2/3)*cm;
  G4double fullconemp_hz = ((14+30+3.5+0.1+9.4-2.0)/2)*cm;
  G4double fullconemp_phimin = 0.*deg, fullconemp_phimax = 360.*deg;
  G4Cons* solidFullconemp =    
    new G4Cons("Fullconemp", 
    fullconemp_rmina, fullconemp_rmaxa, fullconemp_rminb, fullconemp_rmaxb, fullconemp_hz,
    fullconemp_phimin, fullconemp_phimax);


  // right side Pd(reflector) minus poly-Li(delimiter)
  //  
  G4Material* rside_reflector = nist->FindOrBuildMaterial("G4_Pb"); //Pb
  G4ThreeVector rside_refle_minus_polyLi = G4ThreeVector(0, 0, ((14+30+3.5+0.1+9.4-2.0)/2)*cm);
        
  // right side box minus poly-Li(delimiter) length
  G4double rsbmp_sizeXY = 70.*cm;
  G4double rsbmp_sizeZ  = (14+30+3.5+0.1+9.4-2.0)*cm;      

  G4Box* solidRsbmp =    
   new G4Box("RsideEnvelopemp",                    //its name
       0.5*rsbmp_sizeXY, 0.5*rsbmp_sizeXY, 0.5*rsbmp_sizeZ); //its size

  G4SubtractionSolid* RsbsubtconeminusPL =
   new G4SubtractionSolid("RsbsubtconeminusPL", solidRsbmp, solidFullconemp);


  G4LogicalVolume* logicRsbsubtconeminusPL =                         
    new G4LogicalVolume(RsbsubtconeminusPL,         //its solid
                        rside_reflector,          //its material
                        "RsbsubtconeminusPL");               //its name
  logicRsbsubtconeminusPL->SetVisAttributes(new G4VisAttributes(G4Colour(204/255.,0/255.,204/255.)));
              
  new G4PVPlacement(0,                       //no rotation
                    rside_refle_minus_polyLi,                    //at position
                    logicRsbsubtconeminusPL,             //its logical volume
                    "RsbsubtconeminusPL",                //its name
                    logicWorld,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking



// poly-Li(delimiter) (polyethylene)
  //  
  G4Material* delimiter_mat = nist->FindOrBuildMaterial("G4_Pb"); //Pb
  G4ThreeVector pos_delimiter = G4ThreeVector(0, 0, (14+30+3.5+0.1+9.4-2.0+(2.0)/2)*cm);
        
  // poly-Li(delimiter) length
  G4double delimiter_sizeXY = 70.*cm;
  G4double delimiter_sizeZ  = 2.*cm;   

  // last section cone 
  //G4ThreeVector pos_fullcone_minus_polyLi = G4ThreeVector(0, 0, ((14+30+3.5+0.1+9.4-2.0)/2)*cm);
      
  G4double lasectioncone_rmina =  0.*cm, lasectioncone_rmaxb = (12/2.)*cm;
  G4double lasectioncone_rminb =  0.*cm, lasectioncone_rmaxa = (6+2/3)*cm;
  G4double lasectioncone_hz = (2/2.)*cm;
  G4double lasectioncone_phimin = 0.*deg, lasectioncone_phimax = 360.*deg;
  G4Cons* solidLasectioncone =    
    new G4Cons("Lasectioncone", 
    lasectioncone_rmina, lasectioncone_rmaxa, lasectioncone_rminb, lasectioncone_rmaxb, lasectioncone_hz,
    lasectioncone_phimin, lasectioncone_phimax);  


  G4Box* solidlastlid =    
   new G4Box("lastlid",                    //its name
       0.5*delimiter_sizeXY, 0.5*delimiter_sizeXY, 0.5*delimiter_sizeZ); //its size

  G4SubtractionSolid* DelimiterLid =
   new G4SubtractionSolid("DelimiterLid", solidlastlid, solidLasectioncone);


  G4LogicalVolume* logicDelimiterLid =                         
    new G4LogicalVolume(DelimiterLid,         //its solid
                        delimiter_mat,          //its material
                        "DelimiterLid");               //its name
  logicDelimiterLid->SetVisAttributes(new G4VisAttributes(G4Colour(255/255.,0/255.,127/255.)));
              
  new G4PVPlacement(0,                       //no rotation
                    pos_delimiter,                    //at position
                    logicDelimiterLid,             //its logical volume
                    "DelimiterLid",                //its name
                    logicWorld,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


// left side reflector
  
  G4ThreeVector lside_refle_minus_beam = G4ThreeVector(0, 0, (-(14+30+3.5+0.1+9.4)/2)*cm);
  //positon and orientation of BeamTarget relative to left box 
    G4RotationMatrix* LsideyBeamRot = new G4RotationMatrix; // Rotates X and Z axes only 
    LsideyBeamRot->rotateY(-(M_PI/4.)*rad); // Rotates 45 degrees 
    // yBeamRot->rotateY(0.*rad); // Rotates 45 degrees 
    // G4ThreeVector zBeamTrans(0, 0, 0);   
    //G4ThreeVector zBeamTrans(((((-84-2)/2.)+1.)/sqrt(2))*cm, 0, (-((14+82+2)/2.)+1.)*cm); 
  
    G4ThreeVector LsidezxBeamTrans(-(41.0/(sqrt(2)))*cm, 0, 0.*cm);
  //  G4ThreeVector LsidezxBeamTrans(-(12.5/(sqrt(2)))*cm, 0, (1.-((82+2-57)/2.))*cm);    


  // left side box 
  G4double leftsb_sizeXY = 70.*cm;
  G4double leftsb_sizeZ  = (14+30+3.5+0.1+9.4)*cm;      

  G4Box* solidLeftsb =    
   new G4Box("Leftsb",                    //its name
       0.5*leftsb_sizeXY, 0.5*leftsb_sizeXY, 0.5*leftsb_sizeZ); //its size

  //Left side box subtract beamplustatget
  G4SubtractionSolid* LeftsbminusBeam =
    new G4SubtractionSolid("LeftsbminusBeam", solidLeftsb, BeamPlusTar, LsideyBeamRot, LsidezxBeamTrans);
     




  G4LogicalVolume* logicLeftsbminusBeam =                         
    new G4LogicalVolume(LeftsbminusBeam,         //its solid
                        rside_reflector,          //its material
                        "LeftsbminusBeam");               //its name
  logicLeftsbminusBeam->SetVisAttributes(new G4VisAttributes(G4Colour(204/255.,0/255.,204/255.)));
              
  new G4PVPlacement(0,                       //no rotation
                    lside_refle_minus_beam,                    //at position
                    logicLeftsbminusBeam,             //its logical volume
                    "LeftsbminusBeam",                //its name
                    logicWorld,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking





  // Set Shape2 as scoring volume
  //
  fScoringVolume = logicShifter;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
