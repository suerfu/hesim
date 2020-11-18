//
/// \file JanisDDGeneratorAction.cc
/// \brief Implementation of the JanisDDGeneratorAction class

#include "JanisDDGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include "G4RandomDirection.hh"
#include "G4IonTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


JanisDDGeneratorAction::JanisDDGeneratorAction()
 : G4VUserPrimaryGeneratorAction()
{
  fgps = new G4GeneralParticleSource();
    // GPS must be initialized here.
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisDDGeneratorAction::~JanisDDGeneratorAction()
{
  delete fgps;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDDGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    fgps->GeneratePrimaryVertex(anEvent);
}
