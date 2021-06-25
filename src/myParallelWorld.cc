#include "globals.hh"
#include "G4INCLGlobals.hh"

#include "myParallelWorld.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

// for importance biasing
#include "myOptrSplitAndKillByImportance.hh"


myParallelWorld::myParallelWorld(G4String worldName)
    :G4VUserParallelWorld(worldName),fLogicalVolumeVector(),fPhysicalVolumeVector(),fImportanceVector()
{;}

myParallelWorld::~myParallelWorld() {
    fLogicalVolumeVector.clear();
    fPhysicalVolumeVector.clear();
    fImportanceVector.clear();
}

void myParallelWorld::Construct() {
  G4Material* dummyMat = nullptr;

  // -- Obtain clone of mass geometry world from GetWorld() base class utility:
  G4VPhysicalVolume* physicalParallelWorld = GetWorld();
  G4LogicalVolume* logicalParallelWorld = physicalParallelWorld->GetLogicalVolume();

  fLogicalVolumeVector.push_back(logicalParallelWorld);
  fPhysicalVolumeVector.push_back(physicalParallelWorld);
  fImportanceVector.push_back(1.);



  G4Box *outsideBox = new G4Box("outsideBox", 10.*cm, 10.*cm, 10.*cm);
  G4LogicalVolume *l_outsideBox = new G4LogicalVolume(outsideBox, dummyMat, "outsideBox");
  G4VPhysicalVolume *p_outsideBox =
          new G4PVPlacement(nullptr,
                            G4ThreeVector(0,0,0),
                            l_outsideBox,
                            "outsideBox",
                            logicalParallelWorld,
                            false,
                            0);
  fLogicalVolumeVector.push_back(l_outsideBox);
  fPhysicalVolumeVector.push_back(p_outsideBox);
  fImportanceVector.push_back(2.);

  G4Box *insideBox = new G4Box("insideBox", 5.*cm, 5.*cm, 5.*cm);
  G4LogicalVolume *l_insideBox = new G4LogicalVolume(insideBox, dummyMat, "insideBox");
  G4VPhysicalVolume *p_insideBox =
          new G4PVPlacement(nullptr,
                            G4ThreeVector(0,0,0),
                            l_insideBox,
                            "insideBox",
                            l_outsideBox,
                            false,
                            0);
  fLogicalVolumeVector.push_back(l_insideBox);
  fPhysicalVolumeVector.push_back(p_insideBox);
  fImportanceVector.push_back(6.);
}

void myParallelWorld::ConstructSD()
{
  // -- Create the biasing operator:
  auto biasingOperator = new myOptrSplitAndKillByImportance("gamma","parallelOptr");
  // -- Tell it it is active for this parallel geometry, passing the world
  // -- volume of this geometry :
  biasingOperator->SetParallelWorld( GetWorld() );

  // -- Attach to the logical volume where the biasing has to be applied:
  std::vector<G4LogicalVolume *>::iterator logVolIt;
  std::vector<G4double>::iterator imp = fImportanceVector.begin();
  for (logVolIt = fLogicalVolumeVector.begin(); logVolIt != fLogicalVolumeVector.end(); logVolIt++){
      biasingOperator->AttachTo((*logVolIt));
      (biasingOperator->GetImportanceMap())[ (*logVolIt)->GetInstanceID() ] = G4double(*imp);

      G4cerr << "Importance Map: " << (*logVolIt)->GetInstanceID() << " " << (biasingOperator->GetImportanceMap())[ (*logVolIt)->GetInstanceID() ] << G4endl;
      imp++;
  }
}


