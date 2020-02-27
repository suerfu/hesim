//
/// \file Janis/include/JanisTrackingAction.hh
/// \brief Definition of the JanisTrackingAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef JanisTrackingAction_h
#define JanisTrackingAction_h 1

#include "G4UserTrackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class JanisEventAction;

class JanisTrackingAction : public G4UserTrackingAction {

public:
  JanisTrackingAction(JanisEventAction*);
  virtual ~JanisTrackingAction() {};

  virtual void PreUserTrackingAction(const G4Track*);

private:
  JanisEventAction* fEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
