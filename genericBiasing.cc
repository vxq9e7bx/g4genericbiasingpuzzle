#ifndef GIT_COMMIT_HASH
#define GIT_COMMIT_HASH "0000000"
#endif

#include "myDetectorConstruction.hh"
#include "myActionInitialization.hh"
#include "myPhysicsList.hh"
#include "myParallelWorld.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

#include "G4GeometryManager.hh"

#include "G4GenericBiasingPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) {
  // Detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  //Choose the Random engine and reset the seed (before the RunManager)
  G4Random::setTheEngine(new CLHEP::MixMaxRng);
  srand(time(NULL));
//  G4long seed = uint64_t(time(NULL))*uint64_t(rand());
  G4long seed = 12345;
  G4cout << "Random seed: " << seed << G4endl;
  CLHEP::HepRandom::setTheSeed(seed);

  // Construct the default run manager
  auto runManager = G4RunManagerFactory::CreateRunManager();
  runManager->SetNumberOfThreads(1);


  // -- Create geometry:
  myDetectorConstruction* detector = new myDetectorConstruction();
  // -- Create parallel world:
  myParallelWorld* parallelWorld = new myParallelWorld("parallelWorld");
  // -- and "augment" detector geometry with the parallelWorld one:
  detector->RegisterParallelWorld( parallelWorld );
  runManager->SetUserInitialization(detector);

  // -- Select a physics list:
  myPhysicsList* physicsList = new myPhysicsList(0);
  // -- and augment it with biasing facilities:
  G4GenericBiasingPhysics* biasingPhysics = new G4GenericBiasingPhysics();
  biasingPhysics->BeVerbose();
  biasingPhysics->NonPhysicsBias("gamma");
  biasingPhysics->AddParallelGeometry("gamma","parallelWorld");

//  biasingPhysics->NonPhysicsBias("e-");
//  biasingPhysics->AddParallelGeometry("e-","parallelWorld");

  physicsList->RegisterPhysics(biasingPhysics);
  runManager->SetUserInitialization(physicsList);


  // User action initialization
  myActionInitialization* action = new myActionInitialization();
  runManager->SetUserInitialization(action);

//  runManager->Initialize();
//  pdet->CreateImportanceStore();

  runManager->Initialize();

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  UImanager->ApplyCommand("/control/macroPath macros");
  if ( ! ui ) { // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  } else { // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  // open geometry for biasing stores clean-up
  G4GeometryManager::GetInstance()->OpenGeometry();

  delete visManager;
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
