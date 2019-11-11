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
  /*
        if(volume == fDetConstruction->GetairlayersolidPV()){
          test_volumeID = 0;
        }
        else if(volume == fDetConstruction->GetinnershieldPV()){
          test_volumeID = 1;
        }
            else if(volume == fDetConstruction->GetDTsolidPV()){
          test_volumeID = 2;
        }
        else if(volume == fDetConstruction->GetshieldcapironPV()){
          test_volumeID = 3;
        }
        else if(volume == fDetConstruction->Getmoderatoriron1PV()){
          test_volumeID = 4;
        }
        else if(volume == fDetConstruction->GetfilteraluminumPV()){
          test_volumeID = 5;
        }
        else if(volume == fDetConstruction->GetIronsolidPV()){
          test_volumeID = 6;
        }
        else if(volume == fDetConstruction->GetshieldingleadPV()){
          test_volumeID = 7;
        }
        else {
          test_volumeID =8;
        }
    */
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
