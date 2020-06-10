//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef NDetectorMessenger_h
#define NDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class NDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NDetectorMessenger: public G4UImessenger
{
public:
    NDetectorMessenger(NDetectorConstruction*);
    ~NDetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
private:
    NDetectorConstruction*     myDetector;
    
    G4UIdirectory*             NDir;
    G4UIdirectory*             detDir;
    G4UIcmdWithADoubleAndUnit* TargetLenCmd;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif