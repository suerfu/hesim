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


/// \file JanisEventAction.cc
/// \brief Implementation of the JanisEventAction class

#include "JanisEventAction.hh"
#include "JanisRunAction.hh"
#include "JanisAnalysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

#include "StepInfo.hh"
#include "G4ThreeVector.hh"

#include "TTree.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisEventAction::JanisEventAction(JanisRunAction* input_run_action)
 : G4UserEventAction(),
   stepCollection(),
   run_action(input_run_action),
   eventID(0),
   trackID(0),
   stepID(0),
   parentID(0),
   particle_name(""),
   volume_name(""),
   volume_copy_number(0),
   energy(0),
   deposited_energy(0),
   position(0),
   momentum(0),
   global_time(0),
   process_name("")
{
    std::cout << "CREATE EVENT" << std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisEventAction::~JanisEventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisEventAction::PrintEventStatistics() const
{
  // print event statistics
  G4cout
     << " nothing so far "
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisEventAction::BeginOfEventAction(const G4Event*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisEventAction::EndOfEventAction(const G4Event* event)
{
  // Print per event (modulo n)
  //
  G4int eventID = event->GetEventID();
  G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;
  }

    TTree* data_tree = run_action->GetDataTree();

    data_tree->Branch("eventID", &eventID, "eventID/I");
    data_tree->Branch("trackID", &trackID, "trackID/I");
    data_tree->Branch("stepID", &stepID, "stepID/I");
    data_tree->Branch("parentID", &parentID, "parentID/I");
    data_tree->Branch("particle_name", &particle_name);
    data_tree->Branch("volume_name", &volume_name);
    data_tree->Branch("volume_copy_number", &volume_copy_number, "volume_copy_number/I");
    data_tree->Branch("energy", &energy, "energy/D");
    data_tree->Branch("deposited_energy", &deposited_energy, "deposited_energy/D");
    data_tree->Branch("position", &position, "position[3]/D");
    data_tree->Branch("momentum", &momentum, "momentum[3]/D");
    data_tree->Branch("global_time", &global_time, "global_time/D");
    data_tree->Branch("process_name", &process_name);

    G4int j = 0; // used to refresh step_ID when track changes

    if_helium = 0;
    if_farside = 0;

    // Select the tracks we are caring about
    for( size_t i=0; i < stepCollection.size(); ++i ){

      eventID = stepCollection[i].GetEventID();
      trackID = stepCollection[i].GetTrackID();
      if(i!=0)
      {
          if(trackID==stepCollection[i-1].GetTrackID())
          {
              stepID = j;
              j++;
          }
          else
          {
              j = 0;
              stepID = j;
              j++;
          }
      }
      else
      {
          stepID = 0;
          j++;
      }

      parentID = stepCollection[i].GetParentID();

      particle_name = stepCollection[i].GetParticleName();
      volume_name = stepCollection[i].GetVolumeName();
      volume_copy_number = stepCollection[i].GetVolumeCopyNumber();
      energy = stepCollection[i].GetEnergy();
      deposited_energy = stepCollection[i].GetDepositedEnergy();

      position = stepCollection[i].GetPosition();
      momentum = stepCollection[i].GetMomentumDirection();

      global_time = stepCollection[i].GetGlobalTime();
      process_name = stepCollection[i].GetProcessName();

      if(trackID==1){
          if(volume_name=="liquid helium"){
              if_helium = 1;
          }
          if(volume_name=="fs_head_inner" || volume_name =="fs_head_inner_1" || volume_name =="fs_head_inner_2" || volume_name =="fs_head_inner_3" || volume_name =="fs_head_inner_4" || volume_name =="fs_head_inner_5" || volume_name =="fs_head_inner_6" || volume_name =="fs_head_inner_7" || volume_name =="fs_head_inner_8"){
              if_farside = 1;
          }
      }
    }

    // Fill the wanted tracks
    for( size_t i=0; i < stepCollection.size(); ++i ){

      if(if_farside==1 && if_helium==1){

          eventID = stepCollection[i].GetEventID();
          trackID = stepCollection[i].GetTrackID();

          // Refresh stepID
          if(i!=0)
          {
              if(trackID==stepCollection[i-1].GetTrackID())
              {
                  stepID = j;
                  j++;
              }
              else
              {
                  j = 0;
                  stepID = j;
                  j++;
              }
          }
          else
          {
              stepID = 0;
              j++;
          }

          parentID = stepCollection[i].GetParentID();

          particle_name = stepCollection[i].GetParticleName();
          volume_name = stepCollection[i].GetVolumeName();
          volume_copy_number = stepCollection[i].GetVolumeCopyNumber();
          energy = stepCollection[i].GetEnergy();
          deposited_energy = stepCollection[i].GetDepositedEnergy();

          position = stepCollection[i].GetPosition();
          momentum = stepCollection[i].GetMomentumDirection();

          global_time = stepCollection[i].GetGlobalTime();
          process_name = stepCollection[i].GetProcessName();

          data_tree->Fill();
      }
    }
    /*for( size_t i=0; i < stepCollection.size(); ++i ){
      StepInfo stepInfo = stepCollection[i];
      std::cout << "eventID: " << stepInfo.GetEventID() << " trackID: " << stepInfo.GetTrackID() << " stepID: ";
      std::cout << stepInfo.GetStepID() << " parentID: " << stepInfo.GetParentID() << " Name : ";
      std::cout << stepInfo.GetParticleName() << " Volume: " << stepInfo.GetVolumeName() << " Copy #: ";
      std::cout << stepInfo.GetVolumeCopyNumber() << " Energy: " << stepInfo.GetEnergy() << " Position: (";
      std::cout << stepInfo.GetPosition().x() << ", " << stepInfo.GetPosition().y() << ", ";
      std::cout << stepInfo.GetPosition().z() << ") Momentum: (" << stepInfo.GetMomentumDirection().x();
      std::cout << ", " << stepInfo.GetMomentumDirection().y() <<  ", " << stepInfo.GetMomentumDirection().z();
      std::cout << ") Time: " << stepInfo.GetGlobalTime() << " Process: " << stepInfo.GetProcessName() << std::endl;
    }*/
    //PrintEventStatistics(fEnergyHe, nScattersHe);
  stepCollection.clear();
  if_helium = 0;
  if_farside = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

vector<StepInfo>& JanisEventAction::GetStepCollection()
{
  return stepCollection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
