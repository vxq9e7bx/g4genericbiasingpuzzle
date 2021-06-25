#include "myActionInitialization.hh"
#include "myPrimaryGeneratorAction.hh"
#include "myEventAction.hh"
#include "myRunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myActionInitialization::myActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myActionInitialization::~myActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myActionInitialization::BuildForMaster() const {
  myRunAction* runAction = new myRunAction();
  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myActionInitialization::Build() const {
    SetUserAction(new myPrimaryGeneratorAction());

    myRunAction* runAction = new myRunAction();
    SetUserAction(runAction);

    myEventAction* eventAction = new myEventAction(runAction);
    SetUserAction(eventAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
