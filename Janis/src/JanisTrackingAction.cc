//
/// \file Janis/src/JanisTrackingAction.cc
/// \brief Implementation of the JanisTrackingAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "JanisTrackingAction.hh"
#include "JanisEventAction.hh"

#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"

#include "StepInfo.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisTrackingAction::JanisTrackingAction(JanisEventAction* eventAction)
  : G4UserTrackingAction(),
    fEventAction(eventAction)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisTrackingAction::PreUserTrackingAction(const G4Track* track)
{

  //if(!track->GetVolume()) std::cout << "HELP!!!" << std::endl;

  // We have to set up the initStep by hand
  StepInfo stepInfo;

  stepInfo.SetEventID(G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID());
  stepInfo.SetTrackID(track->GetTrackID());
  stepInfo.SetStepID(0);
  stepInfo.SetParentID(track->GetParentID());
  stepInfo.SetParticleName(track->GetParticleDefinition()->GetParticleName());
  stepInfo.SetVolumeName(track->GetVolume()->GetName());
  stepInfo.SetVolumeCopyNumber(track->GetVolume()->GetCopyNo());
  stepInfo.SetEnergy(track->GetKineticEnergy());
  stepInfo.SetPosition(track->GetPosition());
  stepInfo.SetMomentumDirection(track->GetMomentumDirection());
  stepInfo.SetGlobalTime(track->GetGlobalTime());
  stepInfo.SetProcessName("initStep");

  fEventAction->GetStepCollection().push_back(stepInfo);

  //std::cout << "eventID: " << stepInfo->GetEventID() << " trackID: " << stepInfo->GetTrackID() << " stepID: " << stepInfo->GetStepID() << " parentID: " << stepInfo->GetParentID() << " Name : " << stepInfo->GetParticleName() << " Volume: " << stepInfo->GetVolumeName() << " Copy #: " << stepInfo->GetVolumeCopyNumber() << " Energy: " << stepInfo->GetEnergy() << " Position: (" << stepInfo->GetPosition().x() << ", " << stepInfo->GetPosition().y() << ", " << stepInfo->GetPosition().z() << ") Momentum: (" << stepInfo->GetMomentumDirection().x() << ", " << stepInfo->GetMomentumDirection().y() <<  ", " << stepInfo->GetMomentumDirection().z() << ") Time: " << stepInfo->GetGlobalTime() << " Process: " << stepInfo->GetProcessName() << std::endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
