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

// $Id: JanisRunAction.cc $
//
/// \file JanisRunAction.cc
/// \brief Implementation of the JanisRunAction class


#include "JanisRunAction.hh"
#include "JanisRunActionMessenger.hh"
//#include "JanisAnalysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "TFile.h"
#include "TTree.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisRunAction::JanisRunAction()
 : G4UserRunAction(),
   data_tree(),
   fRunActionMessenger(0)
{
  // set printing event number per each event
  fRunActionMessenger = new JanisRunActionMessenger(this);
  G4RunManager::GetRunManager()->SetPrintProgress(1);

/*
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in JanisAnalysis.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Default Settings
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("Janis");

  analysisManager->SetVerboseLevel(1);

  // Creating ntuple
  //
  analysisManager->CreateNtuple("Janis", "Particles");
  //analysisManager->CreateNtupleIColumn("EventID");
  analysisManager->CreateNtupleIColumn("TrackID");
  analysisManager->CreateNtupleIColumn("StepID");
  analysisManager->CreateNtupleIColumn("ParticleType");
  analysisManager->CreateNtupleIColumn("TestVolume");
  analysisManager->CreateNtupleDColumn("ParticleE");
  analysisManager->CreateNtupleDColumn("Xpos");
  analysisManager->CreateNtupleDColumn("Ypos");
  analysisManager->CreateNtupleDColumn("Zpos");
  analysisManager->CreateNtupleDColumn("Time");
  analysisManager->CreateNtupleDColumn("Xmom");
  analysisManager->CreateNtupleDColumn("Ymom");
  analysisManager->CreateNtupleDColumn("Zmom");
  analysisManager->FinishNtuple();

 */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JanisRunAction::~JanisRunAction()
{
  delete fRunActionMessenger;
  //delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisRunAction::setOutputFileName(G4String newname)
{
  output_name = newname;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisRunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  // Get analysis manager
  //G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //analysisManager->OpenFile();
  //

  //output_name = "test10096.root";
  output_file = new TFile(output_name, "RECREATE");

  output_file->cd();
  data_tree = new TTree("Janis", "step info for the run");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JanisRunAction::EndOfRunAction(const G4Run* /*run*/)
{
/*
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // save ntuple
  analysisManager->Write();
  analysisManager->CloseFile();
*/
    output_file->Write();
    output_file->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TTree* JanisRunAction::GetDataTree()
{
  return data_tree;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
