
// $Id: Janis.cc $
//
/// \file Janis.cc
/// \brief Main program of the Janis example

#include "JanisDetectorConstruction.hh"
#include "JanisActionInitialization.hh"

#include "JanisDDGeneratorAction.hh"
#include "JanisRunAction.hh"
#include "JanisEventAction.hh"
#include "JanisTrackingAction.hh"
#include "JanisSteppingAction.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "Shielding.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " Janis [-m macro ] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Evaluate arguments
  //
  if ( argc > 7 ) {
    PrintUsage();
    return 1;
  }

  G4String macro;
  G4String session;

  G4String filename = "";
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
    else if ( G4String(argv[i]) == "-f" ) filename = G4String(argv[i+1]);
#ifdef G4MULTITHREADED
    else if ( G4String(argv[i]) == "-t" ) {
      nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
#endif
    else {
      PrintUsage();
      return 1;
    }
  }

  // Detect interactive mode (if no macro provided) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( ! macro.size() ) {
    ui = new G4UIExecutive(argc, argv, session);
  }

  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4long seeds[2];
  time_t systime = time(NULL);
  seeds[0] = (long) systime;
  seeds[1] = (long) (systime*G4UniformRand());
  G4Random::setTheSeeds(seeds);

  G4cout << "Seeds for random generator are " << seeds[0] << ", " << seeds[1] << G4endl;

  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
  if ( nThreads > 0 ) {
    runManager->SetNumberOfThreads(nThreads);
  }
#else
  G4RunManager * runManager = new G4RunManager;
#endif

  // Geometry
  JanisDetectorConstruction* detConstruction = new JanisDetectorConstruction();
  runManager->SetUserInitialization(detConstruction);
  
  // Physics list
  G4VModularPhysicsList* physicsList = new Shielding;
  runManager->SetUserInitialization(physicsList);
  
  // avoid using ActionInitialization for now
  /*
  JanisActionInitialization* actionInitialization
     = new JanisActionInitialization( detConstruction, filename);
  runManager->SetUserInitialization( actionInitialization );
  */

  // Primary generator
  runManager->SetUserAction( new JanisDDGeneratorAction);

  // Run action
  JanisRunAction* runAction = new JanisRunAction;
  runAction->setOutputFileName( filename );
  runAction->AddRandomSeeds( seeds, 2);
  runManager->SetUserAction( runAction );

  // Event action
  JanisEventAction* eventAction = new JanisEventAction(runAction);
  runManager->SetUserAction( eventAction );

  // Tracking and stepping
  runManager->SetUserAction(new JanisTrackingAction(eventAction));
  runManager->SetUserAction(new JanisSteppingAction(detConstruction, eventAction));

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  
  if ( macro.size() ) {
    // batch mode
    // actionInitialization
    runAction->AddMacro( macro );
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  }
  else  {
    // interactive mode : define UI session
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    UImanager->ApplyCommand("/control/execute init_vis.mac");
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();

    delete visManager;
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
