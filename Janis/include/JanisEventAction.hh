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

class JanisEventAction : public G4UserEventAction
{
  public:
    JanisEventAction(JanisRunAction* input_run_action);
    virtual ~JanisEventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);

    vector<StepInfo>& GetStepCollection();

  private:
    // methods
    void PrintEventStatistics() const;

    vector<StepInfo> stepCollection;

    JanisRunAction* run_action;

    int eventID;
    int trackID;
    int stepID;
    int parentID;

    // flags used to check whether a track passes both helium target and a specific farside detector
    int if_helium = 0;
    int if_farside = 0;

    string particle_name;
    string volume_name;
    int volume_copy_number;
    double energy;
    double deposited_energy;

    G4ThreeVector position;
    G4ThreeVector momentum;

    double global_time;
    string process_name;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
