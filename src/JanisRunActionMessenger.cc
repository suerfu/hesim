// $Id: JanisRunActionMessenger.cc $
//
/// \file JanisRunActionMessenger.cc
/// \brief Definition of the JanisRunActionMessenger class

#include "JanisRunActionMessenger.hh"
#include "JanisRunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

JanisRunActionMessenger::JanisRunActionMessenger(JanisRunAction* filename)
  : G4UImessenger(),
    SetFileName(filename)
{
    SetFileNameDir = new G4UIdirectory("/filename");
    SetFileNameDir->SetGuidance("Set the filename of the out put file (Please write in form of <somename.root>)");

    FileNameCmd = new G4UIcmdWithAString("/filename/setFilename", this);
    FileNameCmd->SetGuidance("Set the filename of the out put file (Please write in form of <somename.root>)");
    FileNameCmd->SetParameterName("newname", false);
    FileNameCmd->AvailableForStates(G4State_Idle);
    FileNameCmd->SetDefaultValue("default_output_filename.root");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

JanisRunActionMessenger::~JanisRunActionMessenger()
{
    delete FileNameCmd;
    delete SetFileNameDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void JanisRunActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    SetFileName->setOutputFileName(newValue);
}
