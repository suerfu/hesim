
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

    PMTAngleCmd = new G4UIcmdWithADouble("/placement/definePMTAngle", this);
    PMTAngleCmd->SetGuidance("Set azimuth angle of the PMTs");
    PMTAngleCmd->SetParameterName("pmt_angle", false);
    PMTAngleCmd->SetDefaultValue(45);
    PMTAngleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

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

    NewNaIAngle1Cmd = new G4UIcmdWithADouble("/placement/addNaIAngle1",this);
    NewNaIAngle1Cmd->SetGuidance("Add the 1st far-side NaI detector, and set the angle of the far-side detector in unit of deg.");
    NewNaIAngle1Cmd->SetParameterName("new_fsNaI_angle_1", false);
    NewNaIAngle1Cmd->AvailableForStates(G4State_Idle);
    NewNaIAngle1Cmd->SetDefaultValue(0);

    NewNaIAngle2Cmd = new G4UIcmdWithADouble("/placement/addNaIAngle2",this);
    NewNaIAngle2Cmd->SetGuidance("Add the 2nd far-side NaI detector, and set the angle of the far-side detector in unit of deg.");
    NewNaIAngle2Cmd->SetParameterName("new_fsNaI_angle_2", false);
    NewNaIAngle2Cmd->AvailableForStates(G4State_Idle);
    NewNaIAngle2Cmd->SetDefaultValue(0);

    NewNaIAngle3Cmd = new G4UIcmdWithADouble("/placement/addNaIAngle3",this);
    NewNaIAngle3Cmd->SetGuidance("Add the 3rd far-side NaI detector, and set the angle of the far-side detector in unit of deg.");
    NewNaIAngle3Cmd->SetParameterName("new_fsNaI_angle_3", false);
    NewNaIAngle3Cmd->AvailableForStates(G4State_Idle);
    NewNaIAngle3Cmd->SetDefaultValue(0);

    NewNaIAngle4Cmd = new G4UIcmdWithADouble("/placement/addNaIAngle4",this);
    NewNaIAngle4Cmd->SetGuidance("Add the 4th far-side NaI detector, and set the angle of the far-side detector in unit of deg.");
    NewNaIAngle4Cmd->SetParameterName("new_fsNaI_angle_4", false);
    NewNaIAngle4Cmd->AvailableForStates(G4State_Idle);
    NewNaIAngle4Cmd->SetDefaultValue(0);

    NewNaIAngle5Cmd = new G4UIcmdWithADouble("/placement/addNaIAngle5",this);
    NewNaIAngle5Cmd->SetGuidance("Add the 5th far-side NaI detector, and set the angle of the far-side detector in unit of deg.");
    NewNaIAngle5Cmd->SetParameterName("new_fsNaI_angle_5", false);
    NewNaIAngle5Cmd->AvailableForStates(G4State_Idle);
    NewNaIAngle5Cmd->SetDefaultValue(0);

    NewNaIAngle6Cmd = new G4UIcmdWithADouble("/placement/addNaIAngle6",this);
    NewNaIAngle6Cmd->SetGuidance("Add the 6th far-side NaI detector, and set the angle of the far-side detector in unit of deg.");
    NewNaIAngle6Cmd->SetParameterName("new_fsNaI_angle_6", false);
    NewNaIAngle6Cmd->AvailableForStates(G4State_Idle);
    NewNaIAngle6Cmd->SetDefaultValue(0);

    NewNaIAngle7Cmd = new G4UIcmdWithADouble("/placement/addNaIAngle7",this);
    NewNaIAngle7Cmd->SetGuidance("Add the 7th far-side NaI detector, and set the angle of the far-side detector in unit of deg.");
    NewNaIAngle7Cmd->SetParameterName("new_fsNaI_angle_7", false);
    NewNaIAngle7Cmd->AvailableForStates(G4State_Idle);
    NewNaIAngle7Cmd->SetDefaultValue(0);

    NewNaIAngle8Cmd = new G4UIcmdWithADouble("/placement/addNaIAngle8",this);
    NewNaIAngle8Cmd->SetGuidance("Add the 8th far-side NaI detector, and set the angle of the far-side detector in unit of deg.");
    NewNaIAngle8Cmd->SetParameterName("new_fsNaI_angle_8", false);
    NewNaIAngle8Cmd->AvailableForStates(G4State_Idle);
    NewNaIAngle8Cmd->SetDefaultValue(0);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

JanisDetectorConstructionMessenger::~JanisDetectorConstructionMessenger()
{
    delete FSDistanceCmd;
    delete PMTAngleCmd;
    delete NewAngle1Cmd;
    delete NewAngle2Cmd;
    delete NewAngle3Cmd;
    delete NewAngle4Cmd;
    delete NewAngle5Cmd;
    delete NewAngle6Cmd;
    delete NewAngle7Cmd;
    delete NewAngle8Cmd;
    delete NewNaIAngle1Cmd;
    delete NewNaIAngle2Cmd;
    delete NewNaIAngle3Cmd;
    delete NewNaIAngle4Cmd;
    delete NewNaIAngle5Cmd;
    delete NewNaIAngle6Cmd;
    delete NewNaIAngle7Cmd;
    delete NewNaIAngle8Cmd;
    delete DetectorPlacementDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void JanisDetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if(command == FSDistanceCmd){
        DetectorPlacement->setFarSideDistance(FSDistanceCmd->GetNewDoubleValue(newValue));

    } else if(command == PMTAngleCmd){
        DetectorPlacement->setPMTAngle(PMTAngleCmd->GetNewDoubleValue(newValue));

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

    } else if(command == NewNaIAngle1Cmd){
        DetectorPlacement->add1stNaIFarSideAngle(NewNaIAngle1Cmd->GetNewDoubleValue(newValue));

    } else if(command == NewNaIAngle2Cmd){
        DetectorPlacement->add2ndNaIFarSideAngle(NewNaIAngle2Cmd->GetNewDoubleValue(newValue));

    } else if(command == NewNaIAngle3Cmd){
        DetectorPlacement->add3rdNaIFarSideAngle(NewNaIAngle3Cmd->GetNewDoubleValue(newValue));

    } else if(command == NewNaIAngle4Cmd){
        DetectorPlacement->add4thNaIFarSideAngle(NewNaIAngle4Cmd->GetNewDoubleValue(newValue));

    } else if(command == NewNaIAngle5Cmd){
        DetectorPlacement->add5thNaIFarSideAngle(NewNaIAngle5Cmd->GetNewDoubleValue(newValue));

    } else if(command == NewNaIAngle6Cmd){
        DetectorPlacement->add6thNaIFarSideAngle(NewNaIAngle6Cmd->GetNewDoubleValue(newValue));

    } else if(command == NewNaIAngle7Cmd){
        DetectorPlacement->add7thNaIFarSideAngle(NewNaIAngle7Cmd->GetNewDoubleValue(newValue));

    } else if(command == NewNaIAngle8Cmd){
        DetectorPlacement->add8thNaIFarSideAngle(NewNaIAngle8Cmd->GetNewDoubleValue(newValue));
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
