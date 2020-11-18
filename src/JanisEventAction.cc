
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

JanisEventAction::JanisEventAction( JanisRunAction* input_run_action )
 : G4UserEventAction(),
   stepCollection(),
   run_action(input_run_action),
   eventID(0),
   trackID(0),
   stepID(0),
   parentID(0),
   //particle_name(""),
   //volume_name(""),
   volume_copy_number(0),
   Eki(0),
   Ekf(0),
   edep(0),
   position(0),
   momentum(0),
   x(0),
   y(0),
   z(0),
   px(0),
   py(0),
   pz(0),
   global_time(0),
   //process_name("")
   tmp_particle_name(""),
   tmp_volume_name(""),
   tmp_process_name("")
{
    max_char_len = 15;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


JanisEventAction::~JanisEventAction(){}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void JanisEventAction::PrintEventStatistics() const {}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void JanisEventAction::BeginOfEventAction(const G4Event*){
    
    // If pointer to ROOT tree is empty, then ask RunAction to create the ROOT tree
    // and assign address of variables for output.
    if( data_tree==0 ){

        data_tree = run_action->GetDataTree();

        // Proceed only if data output is enabled.
        if( data_tree!=0 ){
            // information about its order in the event/run sequence
            data_tree->Branch("eventID", &eventID, "eventID/I");

            // dynamic information
            data_tree->Branch("Edep", &edep, "Edep/D"); // energy deposit in liquid helium
            data_tree->Branch("Edep2", &Ekf, "Edep2/D"); // energy deposit in quartz
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisEventAction::EndOfEventAction(const G4Event* event){

    if( data_tree!=0 ){

        // Print per event (modulo n)
        G4int evtID = event->GetEventID();
        G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
        if ( ( printModulo > 0 ) && ( evtID % printModulo == 0 ) ) {
            G4cout << "---> End of event: " << evtID << G4endl;
        }

        if_helium = 0;
        if_farside = 0;

        // Select the tracks of interest
        for( size_t i=0; i < stepCollection.size(); ++i ){

            eventID = stepCollection[i].GetEventID();
            tmp_volume_name = stepCollection[i].GetVolumeName();
            if( tmp_volume_name=="liquid helium" ){
                edep += stepCollection[i].GetDepositedEnergy();
            }
            else if( tmp_volume_name=="quartz_cell"){
                Ekf += stepCollection[i].GetDepositedEnergy();
            }

        }
        if( edep>1.e-9 )
            data_tree->Fill();
        edep = 0;
        Ekf = 0;
    }

    stepCollection.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

vector<StepInfo>& JanisEventAction::GetStepCollection(){
    return stepCollection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
