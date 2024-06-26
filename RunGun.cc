#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "MyRunAction.hh"


#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "PhysicsList.hh"
//#include "QBBC.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "Randomize.hh"



int main(int argc,char** argv)
{

  // Detect interactive mode (if no arguments) and define UI session
  //

  G4UIExecutive* ui = nullptr;
 if ( argc == 1 ) { ui = new G4UIExecutive(argc, argv); }
 
  //=========== new random  engine and  random seed====
  CLHEP::HepRandom::setTheEngine(new CLHEP::MTwistEngine);
  //CLHEP::HepRandom::setTheSeed(time(NULL));
  CLHEP::HepRandom::setTheSeed((unsigned)clock());
 //==================================================== 
  
  // Optionally: choose a different Random engine...
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);

  //use G4SteppingVerboseWithUnits
  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

  // Construct the default run manager

  auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Detector construction
  DetectorConstruction* detector = new DetectorConstruction;

  // Set mandatory initialization classes
  runManager->SetUserInitialization(detector);

 PhysicsList* phys = new PhysicsList;
 runManager->SetUserInitialization(phys);

  // User action initialization
  runManager->SetUserInitialization(new MyActionInitialization());

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

