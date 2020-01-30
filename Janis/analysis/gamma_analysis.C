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

void gamma_analysis(){
  // Read source root file
  string fname;
  string filename = "../../Janis-result/WithFloor_gamma/withfloor_compton.root";
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

  // energy deposit in helium (mixed)
  TH1D *h1 = new TH1D("h1","mixed", 100, 0, 3);
  TH1D *h2 = new TH1D("h2","mixed", 100, 0, 3);
  TH1D *h3 = new TH1D("h3","mixed", 100, 0, 3);
  TH1D *h4 = new TH1D("h4","mixed", 100, 0, 3);
  TH1D *h5 = new TH1D("h5","mixed", 100, 0, 3);
  TH1D *h6 = new TH1D("h6","mixed", 100, 0, 3);
  TH1D *h7 = new TH1D("h7","mixed", 100, 0, 3);
  TH1D *h8 = new TH1D("h8","mixed", 100, 0, 3);

  // gamma scattering times in helium
  TH1D *he1 = new TH1D("","", 6, 1, 6);
  TH1D *he2 = new TH1D("","", 6, 1, 6);
  TH1D *he3 = new TH1D("","", 6, 1, 6);
  TH1D *he4 = new TH1D("","", 6, 1, 6);
  TH1D *he5 = new TH1D("","", 6, 1, 6);
  TH1D *he6 = new TH1D("","", 6, 1, 6);
  TH1D *he7 = new TH1D("","", 6, 1, 6);
  TH1D *he8 = new TH1D("","", 6, 1, 6);

  // gamma scattering times in non-cylindricaly symmetric volumes
  TH1D *sq1 = new TH1D("","", 10, 0, 10);
  TH1D *sq2 = new TH1D("","", 10, 0, 10);
  TH1D *sq3 = new TH1D("","", 10, 0, 10);
  TH1D *sq4 = new TH1D("","", 10, 0, 10);
  TH1D *sq5 = new TH1D("","", 10, 0, 10);
  TH1D *sq6 = new TH1D("","", 10, 0, 10);
  TH1D *sq7 = new TH1D("","", 10, 0, 10);
  TH1D *sq8 = new TH1D("","", 10, 0, 10);

  // Setting up variables to sum over the same track
  double total_energy_deposit;
  int j; // used for looping in the same track in helium
  int k; // used for looping in the same track
  int this_eventID; // The current event ID
  int fs; // which far side detector has it pass
  bool new_event = false;

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

  int Nfloor; // Counter of scatterings in the floor

  int Nfs;
  int Nhelium; // neutron only
  int NelseCy; // Counter of gamma scatterings in other volumes with cylindrical symmetry
  int NelseSq; // Counter of gamma scatterings in other volumes without cylindrical symmetry
  int NelseShell; // Far side gamma scintillator metal shell

  int processed_eventID; // new eventID for processed events. Note that a old event maybe seperated into several processed events

  // Setting up a mapping for eventID <-> farside channel
  map<int,int> event_fs;

  // Looping events in the tree and fill the histogram
  for (int i=0; i<nentries; ++i){
    // Just to monitor how fast we process the data
    cout << "progress: " << (i+1) << "/" << nentries << endl;

    events -> GetEntry(i);

    // To see whether this is a start of a new event
    if (trackID == 1 && stepID == 0){
      new_event = true;
      processed_eventID += 1;
    } else{
      new_event = false;
    }

    total_energy_deposit = 0;
    fs = 0;

    // Count Nscattering
    if (deposited_energy!=0 && *particle_name == "gamma"){
      if (*volume_name=="liquid helium"){
        Nhelium += 1;
      }

      else if (*volume_name=="fs1_head_inner_1r"){
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

      else if (*volume_name == "pmt" || *volume_name == "pmt_Body" || *volume_name == "pmt_Window" ||
               *volume_name == "pmt_interior" || *volume_name == "pmt_array" || *volume_name == "quartz_cell" ||
               *volume_name == "pmt_base1" || *volume_name == "pmt_base2" || *volume_name == "pmt_base3" ||
               *volume_name == "pmt_base4"){
        NelseSq += 1;
      }

      else if (!(*volume_name == "fs1_head_outer_1" || *volume_name == "fs2_head_outer_2" || *volume_name == "fs3_head_outer_3" ||
                *volume_name == "fs4_head_outer_4" || *volume_name == "fs5_head_outer_5" || *volume_name == "fs6_head_outer_6" ||
                *volume_name == "fs7_head_outer_7" || *volume_name == "fs8_head_outer_8")){
        NelseCy += 1;
      }

      else{
        NelseShell += 1;
      }

    }

    // if the first step in helium, no matter what kind of particle
    // All the stepID for index i below should be the first step in helium cube
    if (*volume_name == "liquid helium" && deposited_energy==0){
      total_energy_deposit += deposited_energy;

      if (i != nentries - 2){
        j = i + 1;
        k = i + 1;
        this_eventID = eventID;
        events -> GetEntry(j);

        // Loop inside the helium cube to sum energy for multiple scattering

        while (*volume_name == "liquid helium"){

          total_energy_deposit += deposited_energy;
          j++;

          events -> GetEntry(j); // next step
        }

        events -> GetEntry(k);

        // label the tracks with different far side detector as destination
        // while the step is still in the first track
        // original neutron only (excluded alpha and other particles)
        // The logic here to determine NfsAbnormal is to see if a neutron scatters in a fs which is not the same as its fs label
        event_fs[eventID] = 0; // which means that it hasn't been assigned

        while (eventID == this_eventID && trackID == 1){
          if (*volume_name == "fsNaI1_scintillator"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 1 && deposited_energy!=0){
              NfsAbnormal1 += 1;
              cout << "NfsAbnormal1 = " << NfsAbnormal1 << endl;
            }
            // First step in far side detector
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 1;
            }
          }
          if (*volume_name == "fsNaI2_scintillator"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 2 && deposited_energy!=0){
              NfsAbnormal2 += 1;
              cout << "NfsAbnormal2 = " << NfsAbnormal2 << endl;
            }
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 2;
            }
          }
          if (*volume_name == "fsNaI3_scintillator"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 3 && deposited_energy!=0){
              NfsAbnormal3 += 1;
              cout << "NfsAbnormal3 = " << NfsAbnormal3 << endl;
            }
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 3;
            }
          }
          if (*volume_name == "fsNaI4_scintillator"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 4 && deposited_energy!=0){
              NfsAbnormal4 += 1;
              cout << "NfsAbnormal4 = " << NfsAbnormal4 << endl;
            }
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 4;
            }
          }
          if (*volume_name == "fsNaI5_scintillator"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 5 && deposited_energy!=0){
              NfsAbnormal5 += 1;
              cout << "NfsAbnormal5 = " << NfsAbnormal5 << endl;
            }
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 5;
            }
          }
          if (*volume_name == "fsNaI6_scintillator"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 6 && deposited_energy!=0){
              NfsAbnormal6 += 1;
              cout << "NfsAbnormal6 = " << NfsAbnormal6 << endl;
            }
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 6;
            }
          }
          if (*volume_name == "fsNaI7_scintillator"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 7 && deposited_energy!=0){
              NfsAbnormal7 += 1;
              cout << "NfsAbnormal7 = " << NfsAbnormal7 << endl;
            }
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 7;
            }
          }
          if (*volume_name == "fsNaI8_scintillator"){
            if (event_fs[eventID] != 0 && event_fs[eventID] != 8 && deposited_energy!=0){
              NfsAbnormal8 += 1;
              cout << "NfsAbnormal8 = " << NfsAbnormal8 << endl;
            }
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 8;
            }
          }

          k ++;
          events -> GetEntry(k); // next volume
        }
      }

      // Filling the deposit to histograms, excluding double counted alphas
      events -> GetEntry(i);

      // Note that we may fill data with the same eventID several times. This is event (wanted scatterings) based not eventID based
      if (true){


        // 0.0001MeV energy limit is determined by detection efficiency. (suggested by Junsong)
        if (total_energy_deposit > 0.0001){

          // problematic sq: we may have double-counted them
          //cout<<Nhelium<<endl;
          if (fs == 1){
            h1 -> Fill(total_energy_deposit);
            he1 -> Fill(Nhelium);
            sq1 -> Fill(NelseSq);
          }
          if (fs == 2){
            h2 -> Fill(total_energy_deposit);
            he2 -> Fill(Nhelium);
            sq2 -> Fill(NelseSq);
          }
          if (fs == 3){
            h3 -> Fill(total_energy_deposit);
            he3 -> Fill(Nhelium);
            sq3 -> Fill(NelseSq);
          }
          if (fs == 4){
            h4 -> Fill(total_energy_deposit);
            he4 -> Fill(Nhelium);
            sq4 -> Fill(NelseSq);
          }
          if (fs == 5){
            h5 -> Fill(total_energy_deposit);
            he5 -> Fill(Nhelium);
            sq5 -> Fill(NelseSq);
          }
          if (fs == 6){
            h6 -> Fill(total_energy_deposit);
            he6 -> Fill(Nhelium);
            sq6 -> Fill(NelseSq);
          }
          if (fs == 7){
            h7 -> Fill(total_energy_deposit);
            he7 -> Fill(Nhelium);
            sq7 -> Fill(NelseSq);
          }
          if (fs == 8){
            h8 -> Fill(total_energy_deposit);
            he8 -> Fill(Nhelium);
            sq8 -> Fill(NelseSq);
          }

          Nfs = 0;
          Nhelium = 0;
          NelseCy = 0;
          NelseSq = 0;

          Nfs1 = 0;
          Nfs2 = 0;
          Nfs3 = 0;
          Nfs4 = 0;
          Nfs5 = 0;
          Nfs6 = 0;
          Nfs7 = 0;
          Nfs8 = 0;
          NfsAbnormal1 = 0;
          NfsAbnormal2 = 0;
          NfsAbnormal3 = 0;
          NfsAbnormal4 = 0;
          NfsAbnormal5 = 0;
          NfsAbnormal6 = 0;
          NfsAbnormal7 = 0;
          NfsAbnormal8 = 0;

        }
      }

    }

  }

  // Total number of GEANT events recorded
  cout << "Total number of GEANT events recorded is " << event_fs.size() << endl;

  // Total scattering times in floor
  cout << "Total scattering times in floor is " << Nfloor << endl;


  cout << "Data have been processed, now ready to plot" << endl;

  // 1st far side detector
  TCanvas* c1 = new TCanvas();
  c1 -> SetLogy();
  // output file name
  fname = "(No floor) 1st far-side channel Edep in helium (mixed)";
  h1 -> Draw();
  h1->SetTitle(fname.c_str());
  // Set Axis Title
  h1->GetXaxis()->SetTitle("energy deposit (MeV)");
  h1->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c1->SaveAs((fname + ".png").c_str());

  TCanvas* e1 = new TCanvas();
  // output file name
  fname = "(No floor) 1st far-side channel in-helium neutron scattering #";
  he1 -> Draw();
  he1->SetTitle(fname.c_str());
  // Set Axis Title
  he1->GetXaxis()->SetTitle("scattering times");
  he1->GetYaxis()->SetTitle("Counts");
  // Save the figure
  e1->SaveAs((fname + ".png").c_str());

  TCanvas* f1 = new TCanvas();
  // output file name
  fname = "(No floor) 1st far-side channel in-square neutron scattering #";
  sq1 -> Draw();
  sq1->SetTitle(fname.c_str());
  // Set Axis Title
  sq1->GetXaxis()->SetTitle("scattering times");
  sq1->GetYaxis()->SetTitle("Counts");
  // Save the figure
  f1->SaveAs((fname + ".png").c_str());


  // 2nd far side detector
  TCanvas* c2 = new TCanvas();
  c2 -> SetLogy();
  // output file name
  fname = "(No floor) 2nd far-side channel Edep in helium (mixed)";
  h2 -> Draw();
  h2->SetTitle(fname.c_str());
  // Set Axis Title
  h2->GetXaxis()->SetTitle("energy deposit (MeV)");
  h2->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c2->SaveAs((fname + ".png").c_str());

  TCanvas* e2 = new TCanvas();
  // output file name
  fname = "(No floor) 2nd far-side channel in-helium neutron scattering #";
  he2 -> Draw();
  he2->SetTitle(fname.c_str());
  // Set Axis Title
  he2->GetXaxis()->SetTitle("scattering times");
  he2->GetYaxis()->SetTitle("Counts");
  // Save the figure
  e2->SaveAs((fname + ".png").c_str());

  TCanvas* f2 = new TCanvas();
  // output file name
  fname = "(No floor) 2nd far-side channel in-square neutron scattering #";
  sq2 -> Draw();
  sq2->SetTitle(fname.c_str());
  // Set Axis Title
  sq2->GetXaxis()->SetTitle("scattering times");
  sq2->GetYaxis()->SetTitle("Counts");
  // Save the figure
  f2->SaveAs((fname + ".png").c_str());

  // 3rd far side detector
  TCanvas* c3 = new TCanvas();
  c3 -> SetLogy();
  // output file name
  fname = "(No floor) 3rd far-side channel Edep in helium (mixed)";
  h3 -> Draw();
  h3->SetTitle(fname.c_str());
  // Set Axis Title
  h3->GetXaxis()->SetTitle("energy deposit (MeV)");
  h3->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c3->SaveAs((fname + ".png").c_str());

  TCanvas* e3 = new TCanvas();
  // output file name
  fname = "(No floor) 3rd far-side channel in-helium neutron scattering #";
  he3 -> Draw();
  he3->SetTitle(fname.c_str());
  // Set Axis Title
  he3->GetXaxis()->SetTitle("scattering times");
  he3->GetYaxis()->SetTitle("Counts");
  // Save the figure
  e3->SaveAs((fname + ".png").c_str());

  TCanvas* f3 = new TCanvas();
  // output file name
  fname = "(No floor) 3rd far-side channel in-square neutron scattering #";
  sq3 -> Draw();
  sq3->SetTitle(fname.c_str());
  // Set Axis Title
  sq3->GetXaxis()->SetTitle("scattering times");
  sq3->GetYaxis()->SetTitle("Counts");
  // Save the figure
  f3->SaveAs((fname + ".png").c_str());

  // 4th far side detector
  TCanvas* c4 = new TCanvas();
  c4 -> SetLogy();
  // output file name
  fname = "(No floor) 4th far-side channel Edep in helium (mixed)";
  h4 -> Draw();
  h4->SetTitle(fname.c_str());
  // Set Axis Title
  h4->GetXaxis()->SetTitle("energy deposit (MeV)");
  h4->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c4->SaveAs((fname + ".png").c_str());

  TCanvas* e4 = new TCanvas();
  // output file name
  fname = "(No floor) 4th far-side channel in-helium neutron scattering #";
  he4 -> Draw();
  he4->SetTitle(fname.c_str());
  // Set Axis Title
  he4->GetXaxis()->SetTitle("scattering times");
  he4->GetYaxis()->SetTitle("Counts");
  // Save the figure
  e4->SaveAs((fname + ".png").c_str());

  TCanvas* f4 = new TCanvas();
  // output file name
  fname = "(No floor) 4th far-side channel in-square neutron scattering #";
  sq4 -> Draw();
  sq4->SetTitle(fname.c_str());
  // Set Axis Title
  sq4->GetXaxis()->SetTitle("scattering times");
  sq4->GetYaxis()->SetTitle("Counts");
  // Save the figure
  f4->SaveAs((fname + ".png").c_str());

  // 5th far side detector
  TCanvas* c5 = new TCanvas();
  c5 -> SetLogy();
  // output file name
  fname = "(No floor) 5th far-side channel Edep in helium (mixed)";
  h5 -> Draw();
  h5->SetTitle(fname.c_str());
  // Set Axis Title
  h5->GetXaxis()->SetTitle("energy deposit (MeV)");
  h5->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c5->SaveAs((fname + ".png").c_str());

  TCanvas* e5 = new TCanvas();
  // output file name
  fname = "(No floor) 1st far-side channel in-helium neutron scattering #";
  he5 -> Draw();
  he5->SetTitle(fname.c_str());
  // Set Axis Title
  he5->GetXaxis()->SetTitle("scattering times");
  he5->GetYaxis()->SetTitle("Counts");
  // Save the figure
  e5->SaveAs((fname + ".png").c_str());

  TCanvas* f5 = new TCanvas();
  // output file name
  fname = "(No floor) 5th far-side channel in-square neutron scattering #";
  sq5 -> Draw();
  sq5->SetTitle(fname.c_str());
  // Set Axis Title
  sq5->GetXaxis()->SetTitle("scattering times");
  sq5->GetYaxis()->SetTitle("Counts");
  // Save the figure
  f5->SaveAs((fname + ".png").c_str());

  // 6th far side detector
  TCanvas* c6 = new TCanvas();
  c6 -> SetLogy();
  // output file name
  fname = "(No floor) 6th far-side channel Edep in helium (mixed)";
  h6 -> Draw();
  h6->SetTitle(fname.c_str());
  // Set Axis Title
  h6->GetXaxis()->SetTitle("energy deposit (MeV)");
  h6->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c6->SaveAs((fname + ".png").c_str());

  TCanvas* e6 = new TCanvas();
  // output file name
  fname = "(No floor) 6th far-side channel in-helium neutron scattering #";
  he6 -> Draw();
  he6->SetTitle(fname.c_str());
  // Set Axis Title
  he6->GetXaxis()->SetTitle("scattering times");
  he6->GetYaxis()->SetTitle("Counts");
  // Save the figure
  e6->SaveAs((fname + ".png").c_str());

  TCanvas* f6 = new TCanvas();
  // output file name
  fname = "(No floor) 6th far-side channel in-square neutron scattering #";
  sq6 -> Draw();
  sq6->SetTitle(fname.c_str());
  // Set Axis Title
  sq6->GetXaxis()->SetTitle("scattering times");
  sq6->GetYaxis()->SetTitle("Counts");
  // Save the figure
  f6->SaveAs((fname + ".png").c_str());

  // 7th far side detector
  TCanvas* c7 = new TCanvas();
  c7 -> SetLogy();
  // output file name
  fname = "(No floor) 7th far-side channel Edep in helium (mixed)";
  h7 -> Draw();
  h7->SetTitle(fname.c_str());
  // Set Axis Title
  h7->GetXaxis()->SetTitle("energy deposit (MeV)");
  h7->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c7->SaveAs((fname + ".png").c_str());

  TCanvas* e7 = new TCanvas();
  // output file name
  fname = "(No floor) 7th far-side channel in-helium neutron scattering #";
  he7 -> Draw();
  he7->SetTitle(fname.c_str());
  // Set Axis Title
  he7->GetXaxis()->SetTitle("scattering times");
  he7->GetYaxis()->SetTitle("Counts");
  // Save the figure
  e7->SaveAs((fname + ".png").c_str());

  TCanvas* f7 = new TCanvas();
  // output file name
  fname = "(No floor) 7th far-side channel in-square neutron scattering #";
  sq7 -> Draw();
  sq7->SetTitle(fname.c_str());
  // Set Axis Title
  sq7->GetXaxis()->SetTitle("scattering times");
  sq7->GetYaxis()->SetTitle("Counts");
  // Save the figure
  f7->SaveAs((fname + ".png").c_str());

  // 8th far side detector
  TCanvas* c8 = new TCanvas();
  c8 -> SetLogy();
  // output file name
  fname = "(No floor) 8th far-side channel Edep in helium (mixed)";
  h8 -> Draw();
  h8->SetTitle(fname.c_str());
  // Set Axis Title
  h8->GetXaxis()->SetTitle("energy deposit (MeV)");
  h8->GetYaxis()->SetTitle("Counts");
  // Save the figure
  c8->SaveAs((fname + ".png").c_str());

  TCanvas* e8 = new TCanvas();
  // output file name
  fname = "(No floor) 8th far-side channel in-helium neutron scattering #";
  he8 -> Draw();
  he8->SetTitle(fname.c_str());
  // Set Axis Title
  he8->GetXaxis()->SetTitle("scattering times");
  he8->GetYaxis()->SetTitle("Counts");
  // Save the figure
  e8->SaveAs((fname + ".png").c_str());

  TCanvas* f8 = new TCanvas();
  // output file name
  fname = "(No floor) 8th far-side channel in-square neutron scattering #";
  sq8 -> Draw();
  sq8->SetTitle(fname.c_str());
  // Set Axis Title
  sq8->GetXaxis()->SetTitle("scattering times");
  sq8->GetYaxis()->SetTitle("Counts");
  // Save the figure
  f8->SaveAs((fname + ".png").c_str());
}
