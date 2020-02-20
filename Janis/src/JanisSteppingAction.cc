
/// \file JanisSteppingAction.cc
/// \brief Implementation of the JanisSteppingAction class

#include "JanisSteppingAction.hh"
#include "JanisEventAction.hh"
#include "JanisDetectorConstruction.hh"
#include "JanisAnalysis.hh"

#include "G4Neutron.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "StepInfo.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisSteppingAction::JanisSteppingAction(
                      const JanisDetectorConstruction* detectorConstruction,
                      JanisEventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisSteppingAction::~JanisSteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisSteppingAction::UserSteppingAction(const G4Step* step)
{
  // Collect energy and number of scatters step by step
  // Don't save the out of world step
  if(!step->GetPostStepPoint()->GetPhysicalVolume()) return;

  if( step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()!="Transportation" )
      fEventAction->GetStepCollection().push_back(StepInfo(step));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
