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
// $Id: JanisDDGeneratorMessenger.cc $
//
/// \file JanisDDGeneratorMessenger.cc
/// \brief Definition of the JanisDDGeneratorMessenger class

#include "JanisDDGeneratorMessenger.hh"

#include "JanisDDGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"

JanisDDGeneratorMessenger::JanisDDGeneratorMessenger(JanisDDGeneratorAction* generator)
  : G4UImessenger(),
    primaryGenerator(generator),
    primaryGeneratorDir(0),
    generatorDistanceCmd(0)
{
	primaryGeneratorDir = new G4UIdirectory("/generator/");
	primaryGeneratorDir->SetGuidance("Generator distance control.");

	generatorDistanceCmd = new G4UIcmdWithADouble("/generator/setDistance", this);
	generatorDistanceCmd->SetGuidance("Set distance between the target helium and DD generator in unit of cm");
	generatorDistanceCmd->SetParameterName("distance", false);
	generatorDistanceCmd->SetDefaultValue(0);
	generatorDistanceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

JanisDDGeneratorMessenger::~JanisDDGeneratorMessenger()
{
	delete generatorDistanceCmd;
	delete primaryGeneratorDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void JanisDDGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
	if( command == generatorDistanceCmd )
	{
		G4cout << "Setting generator distance to " << newValue << G4endl;
		primaryGenerator->setGeneratorDistance(generatorDistanceCmd->GetNewDoubleValue(newValue));
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
