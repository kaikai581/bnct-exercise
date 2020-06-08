/// \file NDetectorConstruction.hh
/// \brief Definition of the NDetectorConstruction class

#ifndef NDetectorConstruction_h
#define NDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.
class NDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    NDetectorConstruction();
    virtual ~NDetectorConstruction();
    virtual G4VPhysicalVolume* Construct();
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

protected:
    G4LogicalVolume*  fScoringVolume;
};

#endif
