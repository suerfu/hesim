
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
   particle_name(""),
   volume_name(""),
   volume_copy_number(0),
   Eki(0),
   Ekf(0),
   edep(0),
   position(0),
   momentum(0),
   global_time(0),
   process_name(""),
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
            data_tree->Branch("trackID", &trackID, "trackID/I");
            data_tree->Branch("stepID", &stepID, "stepID/I");

            // information about its idenity
            data_tree->Branch("particle_name", particle_name, "particle_name[16]/C");
            data_tree->Branch("parentID", &parentID, "parentID/I");

            // geometric information
            data_tree->Branch("volume_name", volume_name, "volume_name[16]/C");
            data_tree->Branch("volume_copy_number", &volume_copy_number, "volume_copy_number/I");
            data_tree->Branch("position", &position, "position[3]/D");
            data_tree->Branch("momentum", &momentum, "momentum[3]/D");

            // dynamic information
            data_tree->Branch("global_time", &global_time, "global_time/D");
            data_tree->Branch("Eki", &Eki, "Eki/D"); // initial kinetic energy before the step
            data_tree->Branch("Ekf", &Ekf, "Ekf/D"); // final kinetic energy after the step
            data_tree->Branch("edep", &edep, "edep/D"); // energy deposit calculated by Geant4
            data_tree->Branch("proc_name", process_name, "proc_name[16]/C");
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisEventAction::EndOfEventAction(const G4Event* event){

    if( data_tree ){

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

            trackID = stepCollection[i].GetTrackID();
            tmp_volume_name = stepCollection[i].GetVolumeName();
            edep = stepCollection[i].GetDepositedEnergy();
            tmp_particle_name = stepCollection[i].GetParticleName();

            if( tmp_volume_name=="liquid helium" && edep!=0){
                if_helium = 1;
                if_farside = 1;
            }
            if(edep!=0 && ( tmp_volume_name.find("NaI")==0 || tmp_volume_name.find("LS")==0) ){
                // the volume name start with NaI or LS
                if_farside = 1;
            }
        }

        // There is coincidence. Fill the wanted tracks
        if(if_farside==1 && if_helium==1){

            for( size_t i=0; i < stepCollection.size(); ++i ){
                eventID = stepCollection[i].GetEventID();
                trackID = stepCollection[i].GetTrackID();
                stepID = stepCollection[i].GetStepID();
                edep = stepCollection[i].GetDepositedEnergy();

                // This paragraph is used to refresh step_ID when track_ID changes, which is a minor problem in step_info
                /*
                if( i!=0 ){
                    if( trackID==stepCollection[i-1].GetTrackID() ){
                        stepID = j;
                        j++;
                    }
                    else{
                        j = 0;
                        stepID = j;
                        j++;
                    }
                }
                else{
                    stepID = 0;
                    j++;
                }
                */

                parentID = stepCollection[i].GetParentID();

                tmp_particle_name = stepCollection[i].GetParticleName();
                tmp_volume_name = stepCollection[i].GetVolumeName();
                tmp_process_name = stepCollection[i].GetProcessName();

                strncpy( particle_name, tmp_particle_name.c_str(), max_char_len);
                strncpy( process_name, tmp_process_name.c_str(), max_char_len);
                strncpy( volume_name, tmp_volume_name.c_str(), max_char_len);

                volume_copy_number = stepCollection[i].GetVolumeCopyNumber();
                Eki = stepCollection[i].GetEki();
                Ekf = stepCollection[i].GetEkf();

                position = stepCollection[i].GetPosition();
                momentum = stepCollection[i].GetMomentumDirection();

                global_time = stepCollection[i].GetGlobalTime();

                data_tree->Fill();
            }
        }
    }

    stepCollection.clear();
    if_helium = 0;
    if_farside = 1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

vector<StepInfo>& JanisEventAction::GetStepCollection(){
    return stepCollection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
