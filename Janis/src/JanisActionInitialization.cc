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
/// \file JanisActionInitialization.cc
/// \brief Implementation of the JanisActionInitialization class

#include "JanisActionInitialization.hh"
#include "JanisPrimaryGeneratorAction.hh"
#include "JanisRunAction.hh"
#include "JanisEventAction.hh"
#include "JanisSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisActionInitialization::JanisActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisActionInitialization::~JanisActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisActionInitialization::BuildForMaster() const
{
  JanisRunAction* runAction = new JanisRunAction;
  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisActionInitialization::Build() const
{
  SetUserAction(new JanisPrimaryGeneratorAction);

  JanisRunAction* runAction = new JanisRunAction;
  SetUserAction(runAction);

  JanisEventAction* eventAction = new JanisEventAction(runAction);
  SetUserAction(eventAction);

  SetUserAction(new JanisSteppingAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
*/


// $Id: JanisActionInitialization.cc $
//
/// \file JanisActionInitialization.cc
/// \brief Implementation of the JanisActionInitialization class

#include "JanisActionInitialization.hh"
//#include "JanisPrimaryGeneratorAction.hh"
//#include "JanisGPSGeneratorAction.hh"
#include "JanisDDGeneratorAction.hh"
#include "JanisRunAction.hh"
#include "JanisEventAction.hh"
#include "JanisSteppingAction.hh"
#include "JanisDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisActionInitialization::JanisActionInitialization
							(JanisDetectorConstruction* detConstruction)
 : G4VUserActionInitialization(),
 fDetConstruction(detConstruction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisActionInitialization::~JanisActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisActionInitialization::BuildForMaster() const
{
  SetUserAction(new JanisRunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisActionInitialization::Build() const
{
  //SetUserAction(new JanisPrimaryGeneratorAction);
  SetUserAction(new JanisDDGeneratorAction);
  //SetUserAction(new JanisGPSGeneratorAction);
  SetUserAction(new JanisRunAction);
  JanisEventAction* eventAction = new JanisEventAction;
  SetUserAction(eventAction);
  SetUserAction(new JanisSteppingAction(fDetConstruction, eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
