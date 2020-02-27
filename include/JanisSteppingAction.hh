//
/// \file JanisSteppingAction.hh
/// \brief Definition of the JanisSteppingAction class

#ifndef JanisSteppingAction_h
#define JanisSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class JanisDetectorConstruction;
class JanisEventAction;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track
/// lengths of charged particles in Absober and Gap layers and
/// updated in JanisEventAction.

class JanisSteppingAction : public G4UserSteppingAction
{
public:
  JanisSteppingAction(const JanisDetectorConstruction* detectorConstruction,
                    JanisEventAction* eventAction);
  virtual ~JanisSteppingAction();

  virtual void UserSteppingAction(const G4Step* step);

private:
  const JanisDetectorConstruction* fDetConstruction;
  JanisEventAction* fEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
