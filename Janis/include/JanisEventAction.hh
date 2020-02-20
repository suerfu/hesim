
// $Id: JanisEventAction.hh $
//
/// \file JanisEventAction.hh
/// \brief Definition of the JanisEventAction class

#ifndef JanisEventAction_h
#define JanisEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "StepInfo.hh"
#include "JanisRunAction.hh"

class JanisEventAction : public G4UserEventAction{

public:

    JanisEventAction( JanisRunAction* input_run_action );
    virtual ~JanisEventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);

    vector<StepInfo>& GetStepCollection();

private:
     
    JanisRunAction* run_action;
    
    TTree* data_tree;
    
    // methods
    void PrintEventStatistics() const;

    vector<StepInfo> stepCollection;

    // flags used to check whether a track passes both LHe and backing detector.
    int if_helium = 0;
    int if_farside = 0;

    int eventID;
    int trackID;
    int stepID;
    int parentID;

    int max_char_len;
    char particle_name[16];
    char volume_name[16];
    char process_name[16];

    G4String tmp_particle_name;
    G4String tmp_volume_name;
    G4String tmp_process_name;

    int volume_copy_number;
    double Eki;
    double Ekf;
    double edep;

    G4ThreeVector position;
    G4ThreeVector momentum;

    double global_time;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
