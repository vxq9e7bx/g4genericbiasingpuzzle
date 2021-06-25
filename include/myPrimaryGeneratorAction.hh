#ifndef myPrimaryGeneratorAction_h
#define myPrimaryGeneratorAction_h 1

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4GenericMessenger;
class G4GeneralParticleSource;
class G4Event;

class myPrimaryGeneratorAction: public G4VUserPrimaryGeneratorAction
{
  public:
    myPrimaryGeneratorAction();
    ~myPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event*);

  private:
    G4GeneralParticleSource*  fGeneralParticleSource;
};

#endif

