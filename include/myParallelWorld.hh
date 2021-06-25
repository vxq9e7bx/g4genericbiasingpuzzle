#ifndef myParallelWorld_h
#define myParallelWorld_h 1

#include "G4VUserDetectorConstruction.hh"

#include "globals.hh"
#include <vector>

#include "G4VUserParallelWorld.hh"

class G4VLogicalVolume;
class G4VPhysicalVolume;
class G4VIStore;

class myParallelWorld : public G4VUserParallelWorld
{
public:
  myParallelWorld(G4String worldName);
  virtual ~myParallelWorld();

private:
  virtual void Construct();
  virtual void ConstructSD();

  std::vector<G4LogicalVolume *> fLogicalVolumeVector;
  std::vector<G4VPhysicalVolume *> fPhysicalVolumeVector;
  std::vector<G4double> fImportanceVector;
};

#endif



