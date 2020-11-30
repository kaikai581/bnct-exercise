/// \file BeamOnTargetActionInitialization.hh
/// \brief Definition of the BeamOnTargetActionInitialization class

#ifndef BeamOnTargetActionInitialization_h
#define BeamOnTargetActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class BeamOnTargetActionInitialization : public G4VUserActionInitialization
{
  public:
    BeamOnTargetActionInitialization();
    virtual ~BeamOnTargetActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
