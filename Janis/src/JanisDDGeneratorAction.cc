
//
/// \file JanisDDGeneratorAction.cc
/// \brief Implementation of the JanisDDGeneratorAction class

#include "JanisDDGeneratorAction.hh"

#include "G4RunManager.hh"
//#include "G4Navigator.hh"
//#include "G4PhysicalVolumeStore.hh"
//#include "G4VPhysicalVolume.hh"
//#include "G4SolidStore.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include "G4RandomDirection.hh"
//#include "G4Neutron.hh"
//#include "G4TransportationManager.hh"
//#include "G4Navigator.hh"
#//include "G4GenericIon.hh"
//#include "G4IonTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


JanisDDGeneratorAction::JanisDDGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleSource(0)
{
  fParticleSource = new G4ParticleGun();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisDDGeneratorAction::~JanisDDGeneratorAction()
{
  delete fParticleSource;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDDGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //Parameters of Energy_vs_Angle fitted into a cubic polynomial
  G4double e1 =  -3.76446665e-10;
  G4double e2 =  3.75645182e-07;
  G4double e3 =  -7.76735734e-05;
  G4double e4 = 5.23749916e-04;
  G4double e5 =  2.80130909e+00;

  //Parameters of the Differential_crossection_vs_Angle fitted into a cubic polynomial
  G4double d1 = -9.78454219e-11 ;
  G4double d2 = 2.11642813e-07;
  G4double d3 = -5.71078133e-05;
  G4double d4 = 9.45413306e-03;
  G4double d5 = -7.72571642e-03;


G4double phi = G4UniformRand()*2*3.14159265358979323846*radian;
G4double angle = DD_dist(d1, d2, d3, d4, d5);
G4double theta = angle*(3.14159265358979323846/180)*radian;
G4ThreeVector neutronDirection;
neutronDirection.setRhoPhiTheta(-1.0,phi,theta);//-1 is needed because we are pointing it towards -ve z direction.

// set particle parameters
fParticleSource->SetParticleMomentumDirection(neutronDirection);
fParticleSource->SetParticleEnergy(e1*angle*angle*angle*angle + e2*angle*angle*angle + e3*angle*angle + e4*angle + e5);
 G4ParticleDefinition* particleDefinition
   = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
 fParticleSource->SetParticleDefinition(particleDefinition);

// Set source position
 fParticleSource->SetParticlePosition(G4ThreeVector(-140. * cm , 0., -10. * cm));
 fParticleSource->GeneratePrimaryVertex(anEvent);

}
G4double JanisDDGeneratorAction::DD_dist(G4double w1, G4double w2, G4double w3, G4double w4, G4double w5)
{
  G4bool flag = FALSE;
  G4double x;
  while(flag == FALSE){
    G4double weight = G4UniformRand();
    x = G4UniformRand()*180.0;
    G4double actual_weight = w1*x*x*x*x + w2*x*x*x + w3*x*x + w4*x + w5;
    if(weight<actual_weight){
       flag = TRUE;
    }
  }
  return x;
}
