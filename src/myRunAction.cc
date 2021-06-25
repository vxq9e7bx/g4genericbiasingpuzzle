#include "myRunAction.hh"
#include "G4Run.hh"

myRunAction::myRunAction()
: G4UserRunAction()
{}

myRunAction::~myRunAction() {
}

void myRunAction::BeginOfRunAction(const G4Run* /*run*/)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myRunAction::EndOfRunAction(const G4Run* /*run*/)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
