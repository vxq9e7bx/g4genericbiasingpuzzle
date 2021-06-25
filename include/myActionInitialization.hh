#ifndef myActionInitialization_h
#define myActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "G4INCLGlobals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class myActionInitialization : public G4VUserActionInitialization
{
    public:
        myActionInitialization();
        virtual ~myActionInitialization();

        virtual void BuildForMaster() const;
        virtual void Build() const;
    private:
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


