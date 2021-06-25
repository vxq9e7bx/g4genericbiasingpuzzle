#ifndef myDetectorConstruction_h
#define myDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class myDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    myDetectorConstruction();
    ~myDetectorConstruction();

    virtual void ConstructSDandField();

  public:
    virtual G4VPhysicalVolume* Construct();

  private:
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
