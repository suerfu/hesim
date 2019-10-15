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
/*
/// \file JanisPrimaryGeneratorAction.hh
/// \brief Definition of the JanisPrimaryGeneratorAction class

#ifndef JanisPrimaryGeneratorAction_h
#define JanisPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 6 MeV gamma, randomly distribued
/// in front of the phantom across 80% of the (X,Y) phantom size.

class JanisPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    JanisPrimaryGeneratorAction();
    virtual ~JanisPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);

    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

  private:
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    G4Box* fEnvelopeBox;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

*/

// $Id: JanisPrimaryGeneratorAction.hh $
//
/// \file JanisPrimaryGeneratorAction.hh
/// \brief Definition of the JanisPrimaryGeneratorAction class

#ifndef JanisPrimaryGeneratorAction_h
#define JanisPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"

class G4ParticleGun;
class G4Event;
class G4VSolid;
class G4Navigator;
class G4ParticleDefinition;

class JanisPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  JanisPrimaryGeneratorAction();
  virtual ~JanisPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* event);

  // set methods
  void SetRandomFlag(G4bool value);

private:
  G4ParticleGun*  fParticleSource;

  G4double gamma_energy;
  G4Navigator* gNavigator;

  G4String generator_mode;
  G4bool first_event;

  G4ThreeVector disk_global_position;
  G4VSolid* source_PV;

  G4double boundingSphereRadius;
  G4double process_threshold;
  G4double target_mass;

  G4ParticleDefinition* particleDefinition;
  G4ThreeVector event_position;

  G4bool accepted;
  G4ThreeVector gamma_direction;
  G4ThreeVector neutron_direction;
  G4double neutron_angle;
  G4double neutron_energy;

  G4bool GenerateNeutronPoint(G4double sphereRadius, G4ThreeVector sourcePosition,
  							G4Navigator* navigator, G4VSolid* parentVolume,
  							G4ThreeVector& gammaDirection, G4ThreeVector& interactionPoint);

  G4double PhotoneutronEnergy(G4double threshold, G4double scatteringAngle,
  								G4double gammaEnergy, G4double nucleusMass);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
