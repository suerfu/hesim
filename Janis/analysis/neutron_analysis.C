#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1D.h"
#include "THStack.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TMinuit.h"
#include "TColor.h"
#include "TLine.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TVector3.h"

using namespace std;

void neutron_analysis(){
  // Read source root file
  string fname;
  string filename = "savio10million.root";
  TFile* f = TFile::Open(filename.c_str());
  if(!f){
    cout << "ERROR reading file " << filename << endl;
  } else {
    cout << "Read from " << filename << endl;
  }

  // Select a specific tree
  TTree* events = (TTree*)f->Get("Janis");
  int nentries = events->GetEntries();

  // Set Branch Address
  int eventID;
  events -> SetBranchAddress("eventID", &eventID);
  int trackID;
  events -> SetBranchAddress("trackID", &trackID);
  int stepID;
  events -> SetBranchAddress("stepID", &stepID);
  int parentID;
  events -> SetBranchAddress("parentID", &parentID);
  string *particle_name = 0;
  events -> SetBranchAddress("particle_name", &particle_name);
  string *volume_name = 0;
  events -> SetBranchAddress("volume_name", &volume_name);
  int volume_copy_number;
  events -> SetBranchAddress("volume_copy_number", &volume_copy_number);
  double energy;
  events -> SetBranchAddress("energy", &energy);
  double deposited_energy;
  events -> SetBranchAddress("deposited_energy", &deposited_energy);
  double global_time;
  events -> SetBranchAddress("global_time", &global_time);
  string *process_name = 0;
  events -> SetBranchAddress("process_name", &process_name);

  // Setting up a new figure
  TH1D *h = new TH1D("","", 100, 0, 3);
  TH1D *h1 = new TH1D("","", 100, 0, 3);
  TH1D *h2 = new TH1D("","", 100, 0, 3);
  TH1D *h3 = new TH1D("","", 100, 0, 3);
  TH1D *h4 = new TH1D("","", 100, 0, 3);
  TH1D *h5 = new TH1D("","", 100, 0, 3);
  TH1D *h6 = new TH1D("","", 100, 0, 3);
  TH1D *h7 = new TH1D("","", 100, 0, 3);
  TH1D *h8 = new TH1D("","", 100, 0, 3);

  // Setting up variables to sum over the same track
  double total_energy_deposit;
  int j; // used for looping in the same track in helium
  int k; // used for looping in the same track
  int this_eventID; // The current event ID
  int fs; // which far side detector has it pass

  // Looping events in the tree and fill the histogram
  for (int i=0; i<nentries; ++i){
    events -> GetEntry(i);
    total_energy_deposit = 0;
    fs = 0;

    // if the first step in helium
    if (*volume_name == "liquid helium" && deposited_energy==0){
      total_energy_deposit += deposited_energy;

      if (i!= nentries - 1){
        j = i + 1;
        k = i + 1;
        this_eventID = eventID;
        events -> GetEntry(j);

        // label the tracks with different far side detector as destination
        while (eventID == this_eventID){
          if (*volume_name == "fs1_head_inner_1"){
            fs = 1;
          }
          if (*volume_name == "fs2_head_inner_2"){
            fs = 2;
          }
          if (*volume_name == "fs3_head_inner_3"){
            fs = 3;
          }
          if (*volume_name == "fs4_head_inner_4"){
            fs = 4;
          }
          if (*volume_name == "fs5_head_inner_5"){
            fs = 5;
          }
          if (*volume_name == "fs6_head_inner_6"){
            fs = 6;
          }
          if (*volume_name == "fs7_head_inner_7"){
            fs = 7;
          }
          if (*volume_name == "fs8_head_inner_8"){
            fs = 8;
          }

          k ++;
          events -> GetEntry(k);
        }

        // Loop inside the helium cube
        while (*volume_name == "liquid helium"){
          total_energy_deposit += deposited_energy;
          j++;
          events -> GetEntry(j);
        }
      }

      h -> Fill(total_energy_deposit);

      if (fs == 1){
        h1 -> Fill(total_energy_deposit);
      }
      if (fs == 2){
        h2 -> Fill(total_energy_deposit);
      }
      if (fs == 3){
        h3 -> Fill(total_energy_deposit);
      }
      if (fs == 4){
        h4 -> Fill(total_energy_deposit);
      }
      if (fs == 5){
        h5 -> Fill(total_energy_deposit);
      }
      if (fs == 6){
        h6 -> Fill(total_energy_deposit);
      }
      if (fs == 7){
        h7 -> Fill(total_energy_deposit);
      }
      if (fs == 8){
        h8 -> Fill(total_energy_deposit);
      }

    }

  }

  // All 8 far side detectors
  TCanvas* c = new TCanvas();
  c -> SetLogy();
  // output file name
  fname = "All 8 far side detectors";
  h -> Draw();
  h->SetTitle(fname.c_str());
  // Set Axis Title
  h->GetXaxis()->SetTitle("energy deposit (MeV)");
  h->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c->SaveAs((fname + ".png").c_str());

  // 1st far side detector
  TCanvas* c1 = new TCanvas();
  c1 -> SetLogy();
  // output file name
  fname = "1st far side detector";
  h1 -> Draw();
  h1->SetTitle(fname.c_str());
  // Set Axis Title
  h1->GetXaxis()->SetTitle("energy deposit (MeV)");
  h1->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c1->SaveAs((fname + ".png").c_str());

  // 2nd far side detector
  TCanvas* c2 = new TCanvas();
  c2 -> SetLogy();
  // output file name
  fname = "2nd far side detector";
  h2 -> Draw();
  h2->SetTitle(fname.c_str());
  // Set Axis Title
  h2->GetXaxis()->SetTitle("energy deposit (MeV)");
  h2->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c2->SaveAs((fname + ".png").c_str());

  // 3rd far side detector
  TCanvas* c3 = new TCanvas();
  c3 -> SetLogy();
  // output file name
  fname = "3rd far side detector";
  h3 -> Draw();
  h3->SetTitle(fname.c_str());
  // Set Axis Title
  h3->GetXaxis()->SetTitle("energy deposit (MeV)");
  h3->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c3->SaveAs((fname + ".png").c_str());

  // 4th far side detector
  TCanvas* c4 = new TCanvas();
  c4 -> SetLogy();
  // output file name
  fname = "4th far side detector";
  h4 -> Draw();
  h4->SetTitle(fname.c_str());
  // Set Axis Title
  h4->GetXaxis()->SetTitle("energy deposit (MeV)");
  h4->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c4->SaveAs((fname + ".png").c_str());

  // 5th far side detector
  TCanvas* c5 = new TCanvas();
  c5 -> SetLogy();
  // output file name
  fname = "5th far side detector";
  h5 -> Draw();
  h5->SetTitle(fname.c_str());
  // Set Axis Title
  h5->GetXaxis()->SetTitle("energy deposit (MeV)");
  h5->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c5->SaveAs((fname + ".png").c_str());

  // 6th far side detector
  TCanvas* c6 = new TCanvas();
  c6 -> SetLogy();
  // output file name
  fname = "6th far side detector";
  h6 -> Draw();
  h6->SetTitle(fname.c_str());
  // Set Axis Title
  h6->GetXaxis()->SetTitle("energy deposit (MeV)");
  h6->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c6->SaveAs((fname + ".png").c_str());

  // 7th far side detector
  TCanvas* c7 = new TCanvas();
  c7 -> SetLogy();
  // output file name
  fname = "7th far side detector";
  h7 -> Draw();
  h7->SetTitle(fname.c_str());
  // Set Axis Title
  h7->GetXaxis()->SetTitle("energy deposit (MeV)");
  h7->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c7->SaveAs((fname + ".png").c_str());

  // 8th far side detector
  TCanvas* c8 = new TCanvas();
  c8 -> SetLogy();
  // output file name
  fname = "8th far side detector";
  h8 -> Draw();
  h8->SetTitle(fname.c_str());
  // Set Axis Title
  h8->GetXaxis()->SetTitle("energy deposit (MeV)");
  h8->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c8->SaveAs((fname + ".png").c_str());
}
