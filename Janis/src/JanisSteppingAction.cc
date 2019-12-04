//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//


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

    StepInfo* stepInfo = new StepInfo(step);
    fEventAction->GetStepCollection().push_back(*stepInfo);

    /*
    G4StepPoint* preStep = step->GetPreStepPoint();
    G4StepPoint* postStep = step->GetPostStepPoint();

    G4Track* track = step->GetTrack();


    // get volume of the current step
    const G4VPhysicalVolume* volume = postStep->GetPhysicalVolume();
    G4String volume_name = volume->GetName();

    G4int volume_copy_number = volume->GetCopyNo();

    const G4ParticleDefinition* particle = track->GetDefinition();
    G4String process_name = postStep->GetProcessDefinedStep()->GetProcessName();

    G4ThreeVector position = postStep->GetPosition();
    G4ThreeVector momentum_direction = postStep->GetMomentumDirection();

    G4double global_time = postStep->GetGlobalTime();

    // energy deposit
    G4double energy = postStep->GetKineticEnergy();

    // get analysis manager
    //G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    G4int trackID = track->GetTrackID();
    G4int parentID = track->GetParentID();

    G4String particle_name = track->GetParticleDefinition()->GetParticleName();
    G4int particle_ID = track->GetParticleDefinition()->GetPDGEncoding();

    // G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    if(true){
    stepInfo->SetEventID(eventID);
    stepInfo->SetTrackID(trackID);
    stepInfo->SetStepID(0);
    stepInfo->SetParentID(parentID);
    stepInfo->SetParticleName(particle_name);
    stepInfo->SetVolumeName(volume_name);
    stepInfo->SetVolumeCopyNumber(volume_copy_number);
    stepInfo->SetEnergy(energy);
    stepInfo->SetPosition(position);
    stepInfo->SetMomentumDirection(momentum_direction);
    stepInfo->SetGlobalTime(global_time);
    stepInfo->SetProcessName(process_name);

    // analysisManager->FillNtupleIColumn(0, energy);
    }
    //std::cout << "eventID: " << stepInfo->GetEventID() << " trackID: " << stepInfo->GetTrackID() << " stepID: " << stepInfo->GetStepID() << " parentID: " << stepInfo->GetParentID() << " Name : " << stepInfo->GetParticleName() << " Volume: " << stepInfo->GetVolumeName() << " Copy #: " << stepInfo->GetVolumeCopyNumber() << " Energy: " << stepInfo->GetEnergy() << " Position: (" << stepInfo->GetPosition().x() << ", " << stepInfo->GetPosition().y() << ", " << stepInfo->GetPosition().z() << ") Momentum: (" << stepInfo->GetMomentumDirection().x() << ", " << stepInfo->GetMomentumDirection().y() <<  ", " << stepInfo->GetMomentumDirection().z() << ") Time: " << stepInfo->GetGlobalTime() << " Process: " << stepInfo->GetProcessName() << std::endl;
    */
// The abandoned version
/*
  G4StepPoint* preStep = step->GetPreStepPoint();
  G4StepPoint* postStep = step->GetPostStepPoint();

  G4Track* track = step->GetTrack();

  // get volume of the current step
  const G4VPhysicalVolume* volume = postStep->GetTouchableHandle()->GetVolume();
  //const G4ParticleDefinition* particle = track->GetDefinition();
  const G4String processName = postStep->GetProcessDefinedStep()->GetProcessName();

  G4int test_volumeID;

  if (volume == fDetConstruction->GetLHePV()){
      test_volumeID = 1;
  }
  else{
      test_volumeID =0;
  }

        // energy deposit
        G4double energy = preStep->GetKineticEnergy();

        // get analysis manager
        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        //G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
        G4int trackID = track->GetTrackID();
        G4int stepID = track->GetCurrentStepNumber();
        G4String particle_name = track->GetParticleDefinition()->GetParticleName();
        G4int particle_ID = track->GetParticleDefinition()->GetPDGEncoding();

        // TODO: turn this into a tree that gets filled
        if( particle_ID == 2112 ){
        //analysisManager->FillNtupleIColumn(0, eventID);
        analysisManager->FillNtupleIColumn(0, trackID);
        analysisManager->FillNtupleIColumn(1, stepID);
        analysisManager->FillNtupleIColumn(2, particle_ID);
        analysisManager->FillNtupleIColumn(3, test_volumeID);
        analysisManager->FillNtupleDColumn(4, energy);
        analysisManager->FillNtupleDColumn(5, track->GetPosition().x());
        analysisManager->FillNtupleDColumn(6, track->GetPosition().y());
        analysisManager->FillNtupleDColumn(7, track->GetPosition().z());
        analysisManager->FillNtupleDColumn(8, track->GetGlobalTime());
        analysisManager->FillNtupleDColumn(9, track->GetMomentumDirection().x());
        analysisManager->FillNtupleDColumn(10, track->GetMomentumDirection().y());
        analysisManager->FillNtupleDColumn(11, track->GetMomentumDirection().z());
        analysisManager->AddNtupleRow();
        }

//  }
*/

  //std::cout << "eventID: " << stepInfo->GetEventID() << " trackID: " << stepInfo->GetTrackID() << " stepID: " << stepInfo->GetStepID() << " parentID: " << stepInfo->GetParentID() << " Name : " << stepInfo->GetParticleName() << " Volume: " << stepInfo->GetVolumeName() << " Copy #: " << stepInfo->GetVolumeCopyNumber() << " Energy: " << stepInfo->GetEnergy() << " Position: (" << stepInfo->GetPosition().x() << ", " << stepInfo->GetPosition().y() << ", " << stepInfo->GetPosition().z() << ") Momentum: (" << stepInfo->GetMomentumDirection().x() << ", " << stepInfo->GetMomentumDirection().y() <<  ", " << stepInfo->GetMomentumDirection().z() << ") Time: " << stepInfo->GetGlobalTime() << " Process: " << stepInfo->GetProcessName() << std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
