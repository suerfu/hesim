
//
/// \file JanisDDGeneratorAction.cc
/// \brief Implementation of the JanisDDGeneratorAction class

#include "JanisDDGeneratorAction.hh"
#include "JanisDDGeneratorMessenger.hh"

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
   fParticleSource(0),
   primaryGeneratorMessenger(0),
   generator_distance(60),
   generator_angle(45)
{
  fParticleSource = new G4ParticleGun();

  primaryGeneratorMessenger = new JanisDDGeneratorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisDDGeneratorAction::~JanisDDGeneratorAction()
{
  delete fParticleSource;
  delete primaryGeneratorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDDGeneratorAction::setGeneratorDistance(G4double distance)
{
    generator_distance = distance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDDGeneratorAction::setGeneratorAngle(G4double pmt_angle)
{
    generator_angle = pmt_angle * deg;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisDDGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //Parameters of Energy_vs_Angle fitted into a 4th order polynomial
  G4double e1 =  -3.76446665e-10;
  G4double e2 =  3.75645182e-07;
  G4double e3 =  -7.76735734e-05;
  G4double e4 = 5.23749916e-04;
  G4double e5 =  2.80130909e+00;

  //Parameters of the Differential_crossection_vs_Angle fitted into a 4th order polynomial
  G4double d1 = -2.85692044e-10 ;
  G4double d2 = 1.00118550e-07;
  G4double d3 = -8.89313307e-06;
  G4double d4 = -3.25438874e-05;
  G4double d5 = 3.97964858e-02;


G4double phi = G4UniformRand()*2*3.14159265358979323846*radian;
G4double angle = DD_dist(d1, d2, d3, d4, d5);
G4double theta = angle*(3.14159265358979323846/180)*radian;
G4ThreeVector neutronDirection;
neutronDirection.setRhoPhiTheta(1.0,phi,theta);//1 is needed because we are pointing it towards -ve z direction.
neutronDirection.rotateY(90*deg).rotateZ(-generator_angle);

// set particle parameters
fParticleSource->SetParticleMomentumDirection(neutronDirection);
fParticleSource->SetParticleEnergy(e1*angle*angle*angle*angle + e2*angle*angle*angle + e3*angle*angle + e4*angle + e5);
 G4ParticleDefinition* particleDefinition
   = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
fParticleSource->SetParticleDefinition(particleDefinition);

// Set source position
 fParticleSource->SetParticlePosition(G4ThreeVector(- generator_distance * cos(generator_angle) * cm , generator_distance * sin(generator_angle) * cm, -10. * cm));
 fParticleSource->GeneratePrimaryVertex(anEvent);

}
G4double JanisDDGeneratorAction::DD_dist(G4double w1, G4double w2, G4double w3, G4double w4, G4double w5)
{
  G4bool flag = FALSE;
  G4double x;
  while(flag == FALSE){
    G4double weight = G4UniformRand()*0.04;
    x = G4UniformRand()*90.0;
    G4double actual_weight = w1*x*x*x*x + w2*x*x*x + w3*x*x + w4*x + w5;
    if(weight<actual_weight){
       flag = TRUE;
    }
  }
  return x;
}
