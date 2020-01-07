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

    FSDistanceCmd = new G4UIcmdWithADouble("/placement/setDistance", this);
    FSDistanceCmd->SetGuidance("Set the distance between the target and the latest added far-side detector in unit of cm.");
    FSDistanceCmd->SetParameterName("fs_distance", false);
    FSDistanceCmd->AvailableForStates(G4State_Idle);
    FSDistanceCmd->SetDefaultValue(140);

    NewAngle1Cmd = new G4UIcmdWithADouble("/placement/addAngle1",this);
    NewAngle1Cmd->SetGuidance("Add the 1st far-side detector, and set the angle of the far-side detector in unit of deg.");
    NewAngle1Cmd->SetParameterName("new_fs_angle_1", false);
    NewAngle1Cmd->AvailableForStates(G4State_Idle);
    NewAngle1Cmd->SetDefaultValue(0);

    NewAngle2Cmd = new G4UIcmdWithADouble("/placement/addAngle2",this);
    NewAngle2Cmd->SetGuidance("Add the 2nd far-side detector, and set the angle of the far-side detector in unit of deg.");
    NewAngle2Cmd->SetParameterName("new_fs_angle_2", false);
    NewAngle2Cmd->AvailableForStates(G4State_Idle);
    NewAngle2Cmd->SetDefaultValue(0);

    NewAngle3Cmd = new G4UIcmdWithADouble("/placement/addAngle3",this);
    NewAngle3Cmd->SetGuidance("Add the 3rd far-side detector, and set the angle of the far-side detector in unit of deg.");
    NewAngle3Cmd->SetParameterName("new_fs_angle_3", false);
    NewAngle3Cmd->AvailableForStates(G4State_Idle);
    NewAngle3Cmd->SetDefaultValue(0);

    NewAngle4Cmd = new G4UIcmdWithADouble("/placement/addAngle4",this);
    NewAngle4Cmd->SetGuidance("Add the 4th far-side detector, and set the angle of the far-side detector in unit of deg.");
    NewAngle4Cmd->SetParameterName("new_fs_angle_4", false);
    NewAngle4Cmd->AvailableForStates(G4State_Idle);
    NewAngle4Cmd->SetDefaultValue(0);

    NewAngle5Cmd = new G4UIcmdWithADouble("/placement/addAngle5",this);
    NewAngle5Cmd->SetGuidance("Add the 5th far-side detector, and set the angle of the far-side detector in unit of deg.");
    NewAngle5Cmd->SetParameterName("new_fs_angle_5", false);
    NewAngle5Cmd->AvailableForStates(G4State_Idle);
    NewAngle5Cmd->SetDefaultValue(0);

    NewAngle6Cmd = new G4UIcmdWithADouble("/placement/addAngle6",this);
    NewAngle6Cmd->SetGuidance("Add the 6th far-side detector, and set the angle of the far-side detector in unit of deg.");
    NewAngle6Cmd->SetParameterName("new_fs_angle_6", false);
    NewAngle6Cmd->AvailableForStates(G4State_Idle);
    NewAngle6Cmd->SetDefaultValue(0);

    NewAngle7Cmd = new G4UIcmdWithADouble("/placement/addAngle7",this);
    NewAngle7Cmd->SetGuidance("Add the 7th far-side detector, and set the angle of the far-side detector in unit of deg.");
    NewAngle7Cmd->SetParameterName("new_fs_angle_7", false);
    NewAngle7Cmd->AvailableForStates(G4State_Idle);
    NewAngle7Cmd->SetDefaultValue(0);

    NewAngle8Cmd = new G4UIcmdWithADouble("/placement/addAngle8",this);
    NewAngle8Cmd->SetGuidance("Add the 8th far-side detector, and set the angle of the far-side detector in unit of deg.");
    NewAngle8Cmd->SetParameterName("new_fs_angle_8", false);
    NewAngle8Cmd->AvailableForStates(G4State_Idle);
    NewAngle8Cmd->SetDefaultValue(0);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

JanisDetectorConstructionMessenger::~JanisDetectorConstructionMessenger()
{
    delete FSDistanceCmd;
    delete NewAngle1Cmd;
    delete NewAngle2Cmd;
    delete NewAngle3Cmd;
    delete NewAngle4Cmd;
    delete NewAngle5Cmd;
    delete NewAngle6Cmd;
    delete NewAngle7Cmd;
    delete NewAngle8Cmd;
    delete DetectorPlacementDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void JanisDetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if(command == FSDistanceCmd){
        DetectorPlacement->setFarSideDistance(FSDistanceCmd->GetNewDoubleValue(newValue));

    } else if(command == NewAngle1Cmd){
        DetectorPlacement->add1stFarSideAngle(NewAngle1Cmd->GetNewDoubleValue(newValue));

    } else if(command == NewAngle2Cmd){
        DetectorPlacement->add2ndFarSideAngle(NewAngle2Cmd->GetNewDoubleValue(newValue));

    } else if(command == NewAngle3Cmd){
        DetectorPlacement->add3rdFarSideAngle(NewAngle3Cmd->GetNewDoubleValue(newValue));

    } else if(command == NewAngle4Cmd){
        DetectorPlacement->add4thFarSideAngle(NewAngle4Cmd->GetNewDoubleValue(newValue));

    } else if(command == NewAngle5Cmd){
        DetectorPlacement->add5thFarSideAngle(NewAngle5Cmd->GetNewDoubleValue(newValue));

    } else if(command == NewAngle6Cmd){
        DetectorPlacement->add6thFarSideAngle(NewAngle6Cmd->GetNewDoubleValue(newValue));

    } else if(command == NewAngle7Cmd){
        DetectorPlacement->add7thFarSideAngle(NewAngle7Cmd->GetNewDoubleValue(newValue));

    } else if(command == NewAngle8Cmd){
        DetectorPlacement->add8thFarSideAngle(NewAngle8Cmd->GetNewDoubleValue(newValue));
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
