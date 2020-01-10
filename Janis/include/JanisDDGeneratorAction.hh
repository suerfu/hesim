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

  // set methods
  void SetRandomFlag(G4bool value);

private:
  JanisDDGeneratorMessenger* primaryGeneratorMessenger;

  G4double generator_distance;
  G4double generator_angle;

  G4ParticleGun*  fParticleSource;

  G4double DD_dist(G4double w1, G4double w2, G4double w3, G4double w4, G4double w5);

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
