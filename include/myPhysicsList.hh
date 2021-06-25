#ifndef myPhysicsList_h
#define myPhysicsList_h 1

#include "G4VModularPhysicsList.hh"

class myPhysicsList: public G4VModularPhysicsList
{
public:
  myPhysicsList(G4int ver = 1);
  virtual ~myPhysicsList()=default;

  virtual void SetCuts();

  myPhysicsList(const myPhysicsList &) = delete;
  myPhysicsList & operator=(const myPhysicsList &)=delete;

};

#endif
