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
#include "string.h"

using namespace std;

void neutron_analysis(){
  // Read source root file
  string fname;
  string filename = "../../Janis-result/NoTOF_NoFloor_neutron/savio100million1.root";
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

  double neutron_Edep[10]; // Used to compare with alpha_E to avoid double count in case that alpha take all the recoil energy from the original neutron
  double alpha_E[10]; // Used to compare with neutron_Edep to avoid double count in case that alpha take all the recoil energy from the original neutron
  int n; // index for the neutron_Edep
  int a; // index for alpha_E
  bool double_count; // whether we should avoid recording this alpha's energy deposit
  double he_time;
  double fs_time;
  double tof; // Time of flight:

  // Scattering counters
  int Nfs1;
  int Nfs2;
  int Nfs3;
  int Nfs4;
  int Nfs5;
  int Nfs6;
  int Nfs7;
  int Nfs8;
  int NfsAbnormal1; // channel 1 scattering from another fs detector
  int NfsAbnormal2; // channel 2 scattering from another fs detector
  int NfsAbnormal3; // channel 3 scattering from another fs detector
  int NfsAbnormal4; // channel 4 scattering from another fs detector
  int NfsAbnormal5; // channel 5 scattering from another fs detector
  int NfsAbnormal6; // channel 6 scattering from another fs detector
  int NfsAbnormal7; // channel 7 scattering from another fs detector
  int NfsAbnormal8; // channel 8 scattering from another fs detector
  int Nhelium;
  int Nfloor;
  int Nelse;
  int Nfs; // total fs scattering events

  // Setting up a mapping for eventID <-> farside channel
  map<int,int> event_fs;

  // Looping events in the tree and fill the histogram
  for (int i=0; i<nentries; ++i){

    // Just to monitor how fast we process the data
    cout << "progress: " << (i+1) << "/" << nentries << endl;

    events -> GetEntry(i);
    total_energy_deposit = 0;
    fs = 0;
    n = 0;
    a = 0;

    // Count Nscattering
    if (deposited_energy!=0){
      if (*volume_name=="liquid helium"){
        Nhelium += 1;
      }

      else if (*volume_name=="fs1_head_inner_1"){
        Nfs1 += 1;
      } else if (*volume_name=="fs2_head_inner_2"){
        Nfs2 += 1;
      } else if (*volume_name=="fs3_head_inner_3"){
        Nfs3 += 1;
      } else if (*volume_name=="fs4_head_inner_4"){
        Nfs4 += 1;
      } else if (*volume_name=="fs5_head_inner_5"){
        Nfs5 += 1;
      } else if (*volume_name=="fs6_head_inner_6"){
        Nfs6 += 1;
      } else if (*volume_name=="fs7_head_inner_7"){
        Nfs7 += 1;
      } else if (*volume_name=="fs8_head_inner_8"){
        Nfs8 += 1;
      }

      else if (*volume_name=="floor"){
        Nfloor += 1;
      }

      else{
        Nelse += 1;
      }

    }

    // if the first step in helium, no matter what kind of particle
    // All the stepID for index i below should be the first step in helium cube
    if (*volume_name == "liquid helium" && deposited_energy==0){
      total_energy_deposit += deposited_energy;
      he_time = global_time;

      if (i != nentries - 2){
        j = i + 1;
        k = i + 1;
        this_eventID = eventID;
        events -> GetEntry(j);

        // Loop inside the helium cube to sum energy for multiple scattering
        while (*volume_name == "liquid helium"){

          if (*particle_name == "neutron" && deposited_energy != 0){
            neutron_Edep[n] = deposited_energy;
            n++;
          }
          if (*particle_name == "alpha" && energy != 0){
            alpha_E[a] = energy;
            a++;
          }

          total_energy_deposit += deposited_energy;
          j++;

          events -> GetEntry(j); // next step
        }

        events -> GetEntry(k);

        // label the tracks with different far side detector as destination
        // while the step is still in the first track
        // original neutron only (excluded alpha and other particles)
        event_fs[eventID] = 0; // which means that it hasn't been assigned

        while (eventID == this_eventID && trackID == 1){
          if (*volume_name == "fs1_head_inner_1"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 1){
              NfsAbnormal1 += 1;
            }
            // First step in far side detector
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 1;
              fs_time = global_time;
            }
          }
          if (*volume_name == "fs2_head_inner_2"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 2){
              NfsAbnormal2 += 1;
            }
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 2;
              fs_time = global_time;
            }
          }
          if (*volume_name == "fs3_head_inner_3"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 3){
              NfsAbnormal3 += 1;
            }
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 3;
              fs_time = global_time;
            }
          }
          if (*volume_name == "fs4_head_inner_4"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 4){
              NfsAbnormal4 += 1;
            }
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 4;
              fs_time = global_time;
            }
          }
          if (*volume_name == "fs5_head_inner_5"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 5){
              NfsAbnormal5 += 1;
            }
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 5;
              fs_time = global_time;
            }
          }
          if (*volume_name == "fs6_head_inner_6"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 6){
              NfsAbnormal6 += 1;
            }
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 6;
              fs_time = global_time;
            }
          }
          if (*volume_name == "fs7_head_inner_7"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 7){
              NfsAbnormal7 += 1;
            }
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 7;
              fs_time = global_time;
            }
          }
          if (*volume_name == "fs8_head_inner_8"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 8){
              NfsAbnormal1 += 1;
            }
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 8;
              fs_time = global_time;
            }
          }

          k ++;
          events -> GetEntry(k); // next volume
        }
      }

      // Compare the energy deposit of neutron one by one to the alphas' initial energy. If anyone matches, than it is considered double-counted
      for (int b = 0; b < 10; b++){
        for (int c = 0; c < 10; c++){
          if (neutron_Edep[b]==alpha_E[c] && neutron_Edep[b]!=0){
            double_count = true;
          }
        }
      }

      // Filling the deposit to histograms, excluding double counted alphas
      events -> GetEntry(i);
      tof = fs_time - he_time;
      if (!(double_count && *particle_name=="alpha") && tof < 130){
        fs = event_fs[eventID];

        if (total_energy_deposit > 0.0001){
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

    }

  }

  // Nscattering

  // Average scattering times in helium
  cout << "Average scattering times in helium is " << Nhelium/event_fs.size() << endl;
  // Average scattering times in floor
  cout << "Average scattering times in floor is " << Nfloor/event_fs.size() << endl;
  // Average scattering times in far side detector 1
  cout << "Average scattering times in far side detector 1 is " << 8*Nfs1/event_fs.size() << endl;
  // Average scattering times in far side detector 2
  cout << "Average scattering times in far side detector 2 is " << 8*Nfs2/event_fs.size() << endl;
  // Average scattering times in far side detector 3
  cout << "Average scattering times in far side detector 3 is " << 8*Nfs3/event_fs.size() << endl;
  // Average scattering times in far side detector 4
  cout << "Average scattering times in far side detector 4 is " << 8*Nfs4/event_fs.size() << endl;
  // Average scattering times in far side detector 5
  cout << "Average scattering times in far side detector 5 is " << 8*Nfs5/event_fs.size() << endl;
  // Average scattering times in far side detector 6
  cout << "Average scattering times in far side detector 6 is " << 8*Nfs6/event_fs.size() << endl;
  // Average scattering times in far side detector 7
  cout << "Average scattering times in far side detector 7 is " << 8*Nfs7/event_fs.size() << endl;
  // Average scattering times in far side detector 8
  cout << "Average scattering times in far side detector 8 is " << 8*Nfs8/event_fs.size() << endl;
  // Average unphysical scattering times in far side detector 1
  cout << "Average unphysical scattering times in far side detector 1 is " << 8*NfsAbnormal1/event_fs.size() << ", and the unphysical scattering ratio is " << NfsAbnormal1/Nfs1 << endl;
  // Average unphysical scattering times in far side detector 2
  cout << "Average unphysical scattering times in far side detector 2 is " << 8*NfsAbnormal2/event_fs.size() << ", and the unphysical scattering ratio is " << NfsAbnormal2/Nfs2 << endl;
  // Average unphysical scattering times in far side detector 3
  cout << "Average unphysical scattering times in far side detector 3 is " << 8*NfsAbnormal3/event_fs.size() << ", and the unphysical scattering ratio is " << NfsAbnormal3/Nfs3 << endl;
  // Average unphysical scattering times in far side detector 4
  cout << "Average unphysical scattering times in far side detector 4 is " << 8*NfsAbnormal4/event_fs.size() << ", and the unphysical scattering ratio is " << NfsAbnormal4/Nfs4 << endl;
  // Average unphysical scattering times in far side detector 5
  cout << "Average unphysical scattering times in far side detector 5 is " << 8*NfsAbnormal5/event_fs.size() << ", and the unphysical scattering ratio is " << NfsAbnormal5/Nfs5 << endl;
  // Average unphysical scattering times in far side detector 6
  cout << "Average unphysical scattering times in far side detector 6 is " << 8*NfsAbnormal6/event_fs.size() << ", and the unphysical scattering ratio is " << NfsAbnormal6/Nfs6 << endl;
  // Average unphysical scattering times in far side detector 7
  cout << "Average unphysical scattering times in far side detector 7 is " << 8*NfsAbnormal7/event_fs.size() << ", and the unphysical scattering ratio is " << NfsAbnormal7/Nfs7 << endl;
  // Average unphysical scattering times in far side detector 8
  cout << "Average unphysical scattering times in far side detector 8 is " << 8*NfsAbnormal8/event_fs.size() << ", and the unphysical scattering ratio is " << NfsAbnormal8/Nfs8 << endl;
  // Average scattering in everywhere else
  cout << "Average scattering in everywhere else is " << Nelse/event_fs.size() << endl;

  // All 8 far side detectors
  cout << "Data have been processed, now ready to plot" << endl;

  TCanvas* c = new TCanvas();

  cout << "New TCavas c constructed" << endl;

  c -> SetLogy();
  // output file name
  fname = "(No TOF No floor) All 8 far side detectors 1";
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
  fname = "(No TOF No floor) 1st far side detector 1";
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
  fname = "(No TOF No floor) 2nd far side detector 1";
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
  fname = "(No TOF No floor) 3rd far side detector 1";
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
  fname = "(No TOF No floor) 4th far side detector 1";
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
  fname = "(No TOF No floor) 5th far side detector 1";
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
  fname = "(No TOF No floor) 6th far side detector 1";
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
  fname = "(No TOF No floor) 7th far side detector 1";
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
  fname = "(No TOF No floor) 8th far side detector 1";
  h8 -> Draw();
  h8->SetTitle(fname.c_str());
  // Set Axis Title
  h8->GetXaxis()->SetTitle("energy deposit (MeV)");
  h8->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c8->SaveAs((fname + ".png").c_str());
}
