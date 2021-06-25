#include "myPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"

#include "G4INCLRandom.hh"

#include <filesystem>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPrimaryGeneratorAction::myPrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(),
    fGeneralParticleSource(nullptr)
{
  fGeneralParticleSource  = new G4GeneralParticleSource();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPrimaryGeneratorAction::~myPrimaryGeneratorAction() {
    delete fGeneralParticleSource;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

