#ifndef myRunAction_h
#define myRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class myRunAction : public G4UserRunAction
{
  public:
      myRunAction();
      virtual ~myRunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

  private:
};

#endif

