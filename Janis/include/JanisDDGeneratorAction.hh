
// $Id: JanisDDGeneratorAction.hh $
//
/// \file JanisDDGeneratorAction.hh
/// \brief Definition of the JanisDDGeneratorAction class

#ifndef JanisDDGeneratorAction_h
#define JanisDDGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"

class G4GeneralParticleSource;
class G4ParticleGun;
class G4Event;
class JanisDDGeneratorMessenger;

class JanisDDGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  JanisDDGeneratorAction();
  virtual ~JanisDDGeneratorAction();

  virtual void GeneratePrimaries(G4Event* event);

  void setGeneratorDistance(G4double);
  void setGeneratorAngle(G4double);
  void SetGeneratorMode(G4String);

  // set methods
  void SetRandomFlag(G4bool value);

private:
  JanisDDGeneratorMessenger* primaryGeneratorMessenger;

  G4double generator_distance;
  G4double generator_angle;
  G4String generator_mode;

  G4ParticleGun*  fParticleSource;
  G4GeneralParticleSource*  fgps;

  G4double DD_dist(G4double w1, G4double w2, G4double w3, G4double w4, G4double w5);

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
