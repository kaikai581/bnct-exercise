/// \file NActionInitialization.hh
/// \brief Definition of the NActionInitialization class

#ifndef NActionInitialization_h
#define NActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class NActionInitialization : public G4VUserActionInitialization
{
  public:
    NActionInitialization();
    virtual ~NActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif