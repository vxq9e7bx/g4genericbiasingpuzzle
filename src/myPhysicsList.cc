#include "myPhysicsList.hh"

#include "G4EmPenelopePhysics.hh"
#include "G4SystemOfUnits.hh"

myPhysicsList::myPhysicsList(G4int ver) : G4VModularPhysicsList() {
    RegisterPhysics(new G4EmPenelopePhysics(ver));
}

void myPhysicsList::SetCuts() {
  // Secondary particles with a range bigger than 100 um
  // are generated; otherwise their energy is considered deposited locally
  defaultCutValue = 100. * um;

  const G4double cutForGamma = defaultCutValue;
  const G4double cutForElectron = defaultCutValue;
  const G4double cutForPositron = defaultCutValue;

  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");

  // Set the secondary production cut lower than 990. eV
  // Very important for high precision of lowenergy processes at low energies
  G4double lowLimit = 250. * eV;
  G4double highLimit = 100. * GeV;
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowLimit, highLimit);

  if (verboseLevel>0) DumpCutValuesTable();
}
