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
// $Id: JanisDetectorConstructionMessenger.cc $
//
/// \file JanisDetectorConstructionMessenger.cc
/// \brief Definition of the JanisDetectorConstructionMessenger class

#include "JanisDetectorConstructionMessenger.hh"
#include "JanisDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

JanisDetectorConstructionMessenger::JanisDetectorConstructionMessenger(JanisDetectorConstruction* placement)
  : G4UImessenger(),
    DetectorPlacement(placement)
{
    DetectorPlacementDir = new G4UIdirectory("/placement/");
    DetectorPlacementDir->SetGuidance("Placement (angle and distance) of the farside detector.");

    AngleCmd = new G4UIcmdWithADouble("/placement/setAngle",this);
    AngleCmd->SetGuidance("Set the angle of the far-side detector in unit of deg.");
    AngleCmd->SetParameterName("fs_angle", false);
    AngleCmd->AvailableForStates(G4State_Idle);
	AngleCmd->SetDefaultValue(0);

    FSDistanceCmd = new G4UIcmdWithADouble("/placement/SetFSDistance", this);
    FSDistanceCmd->SetGuidance("Set the distance between the target and the far-side detector in unit of cm.");
    FSDistanceCmd->SetParameterName("fs_distance", false);
    FSDistanceCmd->AvailableForStates(G4State_Idle);
    FSDistanceCmd->SetDefaultValue(140);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

JanisDetectorConstructionMessenger::~JanisDetectorConstructionMessenger()
{
    delete AngleCmd;
    delete FSDistanceCmd;
    delete DetectorPlacementDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void JanisDetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if(command == AngleCmd){
        DetectorPlacement->setFarSideAngle(AngleCmd->GetNewDoubleValue(newValue));

    } else if(command == FSDistanceCmd){
        DetectorPlacement->setFarSideDistance(FSDistanceCmd->GetNewDoubleValue(newValue));
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
