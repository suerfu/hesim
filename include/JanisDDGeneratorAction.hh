
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

class JanisDDGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  JanisDDGeneratorAction();
  virtual ~JanisDDGeneratorAction();

  virtual void GeneratePrimaries(G4Event* event);

private:

  G4GeneralParticleSource*  fgps;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
