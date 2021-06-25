#ifndef myEventAction_h
#define myEventAction_h 1

#include "G4UserEventAction.hh"

class myRunAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


class myEventAction : public G4UserEventAction
{
    public:
        myEventAction(myRunAction* runAction);
        virtual ~myEventAction();

        virtual void  BeginOfEventAction(const G4Event*);
        virtual void    EndOfEventAction(const G4Event*);

    private:
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


