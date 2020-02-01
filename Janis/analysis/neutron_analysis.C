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

  // energy deposit in helium (mixed)
  TH1D *h1 = new TH1D("h1","mixed", 100, 0, 3);
  TH1D *h2 = new TH1D("h2","mixed", 100, 0, 3);
  TH1D *h3 = new TH1D("h3","mixed", 100, 0, 3);
  TH1D *h4 = new TH1D("h4","mixed", 100, 0, 3);
  TH1D *h5 = new TH1D("h5","mixed", 100, 0, 3);
  TH1D *h6 = new TH1D("h6","mixed", 100, 0, 3);
  TH1D *h7 = new TH1D("h7","mixed", 100, 0, 3);
  TH1D *h8 = new TH1D("h8","mixed", 100, 0, 3);

  // energy deposit in helium (NR only)
  TH1D *g1 = new TH1D("g1","NR only", 100, 0, 3);
  TH1D *g2 = new TH1D("g2","NR only", 100, 0, 3);
  TH1D *g3 = new TH1D("g3","NR only", 100, 0, 3);
  TH1D *g4 = new TH1D("g4","NR only", 100, 0, 3);
  TH1D *g5 = new TH1D("g5","NR only", 100, 0, 3);
  TH1D *g6 = new TH1D("g6","NR only", 100, 0, 3);
  TH1D *g7 = new TH1D("g7","NR only", 100, 0, 3);
  TH1D *g8 = new TH1D("g8","NR only", 100, 0, 3);

  // tof
  TH1D *t1 = new TH1D("","", 100, 0, 200);
  TH1D *t2 = new TH1D("","", 100, 0, 200);
  TH1D *t3 = new TH1D("","", 100, 0, 200);
  TH1D *t4 = new TH1D("","", 100, 0, 200);
  TH1D *t5 = new TH1D("","", 100, 0, 200);
  TH1D *t6 = new TH1D("","", 100, 0, 200);
  TH1D *t7 = new TH1D("","", 100, 0, 200);
  TH1D *t8 = new TH1D("","", 100, 0, 200);

  // neutron scattering times in helium
  TH1D *he1 = new TH1D("","", 6, 1, 6);
  TH1D *he2 = new TH1D("","", 6, 1, 6);
  TH1D *he3 = new TH1D("","", 6, 1, 6);
  TH1D *he4 = new TH1D("","", 6, 1, 6);
  TH1D *he5 = new TH1D("","", 6, 1, 6);
  TH1D *he6 = new TH1D("","", 6, 1, 6);
  TH1D *he7 = new TH1D("","", 6, 1, 6);
  TH1D *he8 = new TH1D("","", 6, 1, 6);

  // neutron scattering times in non-cylindricaly symmetric volumes
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

  int Nfloor; // Counter of scatterings in the floor

  int Nfs;
  int Nhelium; // neutron only
  int NelseCy; // Counter of neutron scatterings in other volumes with cylindrical symmetry
  int NelseSq; // Counter of neutron scatterings in other volumes without cylindrical symmetry
  int NelseShell; // Far side neutron scintillator metal shell

  int processed_eventID; // new eventID for processed events. Note that a old event maybe seperated into several processed events

  // Setting up a mapping for eventID <-> farside channel
  map<int,int> event_fs;

  // Looping events in the tree and fill the histogram
  for (int i=0; i<nentries; ++i){
    // Just to monitor how fast we process the data
    cout << "progress: " << (i+1) << "/" << nentries << endl;

    events -> GetEntry(i);

    /*
    if (new_event){
      cout << processed_eventID << endl;
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
    }
    */

    total_energy_deposit = 0;
    fs = 0;
    n = 0;
    a = 0;

    // Count Nscattering
    if (deposited_energy!=0 && *particle_name == "neutron"){
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
      he_time = global_time;

      // In case of the last entry, which may lead to index problem.
      if (i != nentries - 2){
        j = i + 1;
        k = i + 1;
        this_eventID = eventID;
        events -> GetEntry(j);

        // Loop inside the helium cube to sum energy for multiple scattering

        while (*volume_name == "liquid helium"){

          // Compare the Edep in helium and initial energy of alphas and exclude the alphas that match to avoid double count
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
            // First step in far side detector
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 1;
              fs_time = global_time;
            }
          }
          if (*volume_name == "fs2_head_inner_2"){
            // First step in far side detector
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 2;
              fs_time = global_time;
            }
          }
          if (*volume_name == "fs3_head_inner_3"){
            // First step in far side detector
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 3;
              fs_time = global_time;
            }
          }
          if (*volume_name == "fs4_head_inner_4"){
            // First step in far side detector
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 4;
              fs_time = global_time;
            }
          }
          if (*volume_name == "fs5_head_inner_5"){
            // First step in far side detector
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 5;
              fs_time = global_time;
            }
          }
          if (*volume_name == "fs6_head_inner_6"){
            // First step in far side detector
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 6;
              fs_time = global_time;
            }
          }
          if (*volume_name == "fs7_head_inner_7"){
            // First step in far side detector
            if (event_fs[eventID] == 0){
              event_fs[eventID] = 7;
              fs_time = global_time;
            }
          }
          if (*volume_name == "fs8_head_inner_8"){
            // First step in far side detector
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

      // Note that we may fill data with the same eventID several times. This is event (wanted scatterings) based not eventID based
      if (!(double_count && *particle_name=="alpha")){
        fs = event_fs[eventID];

        // 0.0001MeV energy limit is determined by detection efficiency. (suggested by Junsong)
        if (total_energy_deposit > 0.0001){

          // problematic sq: we may have double-counted them
          //cout<<Nhelium<<endl;
          if (fs == 1){
            h1 -> Fill(total_energy_deposit);
            t1 -> Fill(tof);
            he1 -> Fill(Nhelium);
            sq1 -> Fill(NelseSq);
            if (*particle_name== "neutron"){
              g1 -> Fill(total_energy_deposit);
            }
          }
          if (fs == 2){
            h2 -> Fill(total_energy_deposit);
            t2 -> Fill(tof);
            he2 -> Fill(Nhelium);
            sq2 -> Fill(NelseSq);
            if (*particle_name== "neutron"){
              g2 -> Fill(total_energy_deposit);
            }
          }
          if (fs == 3){
            h3 -> Fill(total_energy_deposit);
            t3 -> Fill(tof);
            he3 -> Fill(Nhelium);
            sq3 -> Fill(NelseSq);
            if (*particle_name== "neutron"){
              g3 -> Fill(total_energy_deposit);
            }
          }
          if (fs == 4){
            h4 -> Fill(total_energy_deposit);
            t4 -> Fill(tof);
            he4 -> Fill(Nhelium);
            sq4 -> Fill(NelseSq);
            if (*particle_name== "neutron"){
              g4 -> Fill(total_energy_deposit);
            }
          }
          if (fs == 5){
            h5 -> Fill(total_energy_deposit);
            t5 -> Fill(tof);
            he5 -> Fill(Nhelium);
            sq5 -> Fill(NelseSq);
            if (*particle_name== "neutron"){
              g5 -> Fill(total_energy_deposit);
            }
          }
          if (fs == 6){
            h6 -> Fill(total_energy_deposit);
            t6 -> Fill(tof);
            he6 -> Fill(Nhelium);
            sq6 -> Fill(NelseSq);
            if (*particle_name== "neutron"){
              g6 -> Fill(total_energy_deposit);
            }
          }
          if (fs == 7){
            h7 -> Fill(total_energy_deposit);
            t7 -> Fill(tof);
            he7 -> Fill(Nhelium);
            sq7 -> Fill(NelseSq);
            if (*particle_name== "neutron"){
              g7 -> Fill(total_energy_deposit);
            }
          }
          if (fs == 8){
            h8 -> Fill(total_energy_deposit);
            t8 -> Fill(tof);
            he8 -> Fill(Nhelium);
            sq8 -> Fill(NelseSq);
            if (*particle_name== "neutron"){
              g8 -> Fill(total_energy_deposit);
            }
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

  TCanvas* d1 = new TCanvas();
  d1 -> SetLogy();
  // output file name
  fname = "(No floor) 1st far-side channel Edep in helium (NR only)";
  g1 -> Draw();
  g1->SetTitle(fname.c_str());
  // Set Axis Title
  g1->GetXaxis()->SetTitle("energy deposit (MeV)");
  g1->GetYaxis()->SetTitle("Counts");
  // Save the figure
  d1->SaveAs((fname + ".png").c_str());

  TCanvas* b1 = new TCanvas();
  b1 -> SetLogy();
  // output file name
  fname = "(No floor) 1st far-side channel ToF (mixed)";
  t1 -> Draw();
  t1->SetTitle(fname.c_str());
  // Set Axis Title
  t1->GetXaxis()->SetTitle("Time of Fly (ns)");
  t1->GetYaxis()->SetTitle("Counts");
  // Save the figure
  b1->SaveAs((fname + ".png").c_str());

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

  TCanvas* d2 = new TCanvas();
  d2 -> SetLogy();
  // output file name
  fname = "(No floor) 2nd far-side channel Edep in helium (NR only)";
  g2 -> Draw();
  g2->SetTitle(fname.c_str());
  // Set Axis Title
  g2->GetXaxis()->SetTitle("energy deposit (MeV)");
  g2->GetYaxis()->SetTitle("Counts");
  // Save the figure
  d2->SaveAs((fname + ".png").c_str());

  TCanvas* b2 = new TCanvas();
  b2 -> SetLogy();
  // output file name
  fname = "(No floor) 2nd far-side channel ToF (mixed)";
  t2 -> Draw();
  t2->SetTitle(fname.c_str());
  // Set Axis Title
  t2->GetXaxis()->SetTitle("Time of Fly (ns)");
  t2->GetYaxis()->SetTitle("Counts");
  // Save the figure
  b2->SaveAs((fname + ".png").c_str());

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

  TCanvas* d3 = new TCanvas();
  d3 -> SetLogy();
  // output file name
  fname = "(No floor) 3rd far-side channel Edep in helium (NR only)";
  g3 -> Draw();
  g3->SetTitle(fname.c_str());
  // Set Axis Title
  g3->GetXaxis()->SetTitle("energy deposit (MeV)");
  g3->GetYaxis()->SetTitle("Counts");
  // Save the figure
  d3->SaveAs((fname + ".png").c_str());

  TCanvas* b3 = new TCanvas();
  b3 -> SetLogy();
  // output file name
  fname = "(No floor) 3rd far-side channel ToF (mixed)";
  t3 -> Draw();
  t3->SetTitle(fname.c_str());
  // Set Axis Title
  t3->GetXaxis()->SetTitle("Time of Fly (ns)");
  t3->GetYaxis()->SetTitle("Counts");
  // Save the figure
  b3->SaveAs((fname + ".png").c_str());

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

  TCanvas* d4 = new TCanvas();
  d4 -> SetLogy();
  // output file name
  fname = "(No floor) 4th far-side channel Edep in helium (NR only)";
  g4 -> Draw();
  g4->SetTitle(fname.c_str());
  // Set Axis Title
  g4->GetXaxis()->SetTitle("energy deposit (MeV)");
  g4->GetYaxis()->SetTitle("Counts");
  // Save the figure
  d4->SaveAs((fname + ".png").c_str());

  TCanvas* b4 = new TCanvas();
  b4 -> SetLogy();
  // output file name
  fname = "(No floor) 4th far-side channel ToF (mixed)";
  t4 -> Draw();
  t4->SetTitle(fname.c_str());
  // Set Axis Title
  t4->GetXaxis()->SetTitle("Time of Fly (ns)");
  t4->GetYaxis()->SetTitle("Counts");
  // Save the figure
  b4->SaveAs((fname + ".png").c_str());

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

  TCanvas* d5 = new TCanvas();
  d5 -> SetLogy();
  // output file name
  fname = "(No floor) 5th far-side channel Edep in helium (NR only)";
  g5 -> Draw();
  g5->SetTitle(fname.c_str());
  // Set Axis Title
  g5->GetXaxis()->SetTitle("energy deposit (MeV)");
  g5->GetYaxis()->SetTitle("Counts");
  // Save the figure
  d5->SaveAs((fname + ".png").c_str());

  TCanvas* b5 = new TCanvas();
  b5 -> SetLogy();
  // output file name
  fname = "(No floor) 5th far-side channel ToF (mixed)";
  t5 -> Draw();
  t5->SetTitle(fname.c_str());
  // Set Axis Title
  t5->GetXaxis()->SetTitle("Time of Fly (ns)");
  t5->GetYaxis()->SetTitle("Counts");
  // Save the figure
  b5->SaveAs((fname + ".png").c_str());

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

  TCanvas* d6 = new TCanvas();
  d6 -> SetLogy();
  // output file name
  fname = "(No floor) 6th far-side channel Edep in helium (NR only)";
  g6 -> Draw();
  g6->SetTitle(fname.c_str());
  // Set Axis Title
  g6->GetXaxis()->SetTitle("energy deposit (MeV)");
  g6->GetYaxis()->SetTitle("Counts");
  // Save the figure
  d6->SaveAs((fname + ".png").c_str());

  TCanvas* b6 = new TCanvas();
  b6 -> SetLogy();
  // output file name
  fname = "(No floor) 6th far-side channel ToF (mixed)";
  t6 -> Draw();
  t6->SetTitle(fname.c_str());
  // Set Axis Title
  t6->GetXaxis()->SetTitle("Time of Fly (ns)");
  t6->GetYaxis()->SetTitle("Counts");
  // Save the figure
  b6->SaveAs((fname + ".png").c_str());

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

  TCanvas* d7 = new TCanvas();
  d7 -> SetLogy();
  // output file name
  fname = "(No floor) 7th far-side channel Edep in helium (NR only)";
  g7 -> Draw();
  g7->SetTitle(fname.c_str());
  // Set Axis Title
  g7->GetXaxis()->SetTitle("energy deposit (MeV)");
  g7->GetYaxis()->SetTitle("Counts");
  // Save the figure
  d7->SaveAs((fname + ".png").c_str());

  TCanvas* b7 = new TCanvas();
  b7 -> SetLogy();
  // output file name
  fname = "(No floor) 7th far-side channel ToF (mixed)";
  t7 -> Draw();
  t7->SetTitle(fname.c_str());
  // Set Axis Title
  t7->GetXaxis()->SetTitle("Time of Fly (ns)");
  t7->GetYaxis()->SetTitle("Counts");
  // Save the figure
  b1->SaveAs((fname + ".png").c_str());

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

  TCanvas* d8 = new TCanvas();
  d8 -> SetLogy();
  // output file name
  fname = "(No floor) 8th far-side channel Edep in helium (NR only)";
  g8 -> Draw();
  g8->SetTitle(fname.c_str());
  // Set Axis Title
  g8->GetXaxis()->SetTitle("energy deposit (MeV)");
  g8->GetYaxis()->SetTitle("Counts");
  // Save the figure
  d8->SaveAs((fname + ".png").c_str());

  TCanvas* b8 = new TCanvas();
  b8 -> SetLogy();
  // output file name
  fname = "(No floor) 8th far-side channel ToF (mixed)";
  t8 -> Draw();
  t8->SetTitle(fname.c_str());
  // Set Axis Title
  t8->GetXaxis()->SetTitle("Time of Fly (ns)");
  t8->GetYaxis()->SetTitle("Counts");
  // Save the figure
  b8->SaveAs((fname + ".png").c_str());

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
