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
