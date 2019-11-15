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

struct PlotCollection{

	PlotCollection(string name){
		fname = name;

		colors["DD neutron"] = 15;

	}

	string fname;

	int energy_min = 2;
	int energy_max = 3;
	int nbins_angle = 100;
	int angle_min = 0;
	int angle_max = 90;

	TH2F *hXY = new TH2F("","", 100, angle_min , angle_max, 100, energy_min, energy_max);  // source energy VS detector angle
	TH1D *hR = new TH1D("", "", 100, 0, 50000);
	TH1D *hZ = new TH1D("", "", 100, -25000, 25000);
	map<pair<string,int>, TH1D*> angle_histo;	// source angle distribution


	map<string, int> colors;

	// Counts of the off-curve points
	int abnormal = 0;
	//Parameters of Energy_vs_Angle fitted into a 4th order polynomial
	double e1 =  -3.76446665e-10;
	double e2 =  3.75645182e-07;
	double e3 =  -7.76735734e-05;
	double e4 = 5.23749916e-04;
	double e5 =  2.80130909e+00;


	void AddEvent(string label, double pid, double E, double x, double y, double z, double x_mom, double y_mom, double z_mom, double w){

		// Key formed by file label and particle type
		pair <string, int> key (label, abs(pid));

		// Set up new histograms if they don't exist yet
		if(angle_histo.count(key) == 0){
			angle_histo[key] = new TH1D("", "", nbins_angle, angle_min, angle_max);
		}

		// Fill histograms with event
		double angle_filled = TVector3(x_mom, y_mom, z_mom).Angle(TVector3(1,0,0)) * 180. / TMath::Pi();
		double radius = TMath::Power(TMath::Power(x,2) + TMath::Power(y,2),0.5);
			//TMath::ACos(x_mom/TMath::Power(TMath::Power(x_mom,2) + TMath::Power(y_mom,2) + TMath::Power(z_mom,2),0.5)) * 180.0 / TMath::Pi();
		//cout << endl << "The detected angle of " << fname << ": " << angle_filled << endl << endl;
		// if(x_mom < 0){cout << x_mom << endl;}
		angle_histo[key]->Fill(angle_filled, w);
		hXY->Fill(angle_filled, E);
		if(E != e1*angle_filled*angle_filled*angle_filled*angle_filled + e2*angle_filled*angle_filled*angle_filled + e3*angle_filled*angle_filled + e4*angle_filled + e5){
			abnormal += 1;
			cout<<abnormal<<endl;
			cout<<E - (e1*angle_filled*angle_filled*angle_filled*angle_filled + e2*angle_filled*angle_filled*angle_filled + e3*angle_filled*angle_filled + e4*angle_filled + e5)<<endl;
			cout<<x<<' '<<y<<' '<<z<<endl;
			cout<<x_mom<<' '<<y_mom<<' '<<z_mom<<endl;
			hR->Fill(radius);
			hZ->Fill(z);
		}
	}

	void DrawAngleDistribution(){
		THStack* hs = new THStack;
		TLegend* leg = new TLegend(0.75, 0.65, 0.99, 0.85);

		for(auto it : angle_histo){
			string ll = it.first.first;
			if (it.first.second == 2112){
				ll += "";
			} else {
				cout << "unsorted PID: " << it.first.second << endl;
			}

			int color = colors[ll];
			it.second->SetLineColor(color);
			it.second->SetLineWidth(3);

			it.second->Scale(1/it.second->GetBinWidth(1), "noSumw2");

			if(it.first.second == 2112){
				hs->Add(it.second);
				leg->AddEntry(it.second, ll.c_str(), "F");
				//cout << ll << ": " << it.second->Integral(0, nbins_energy, "width") << " cts / sec " << endl;
			}

			for(int ibin=1; ibin<= it.second->GetNbinsX(); ++ibin){
        		it.second->SetBinError(ibin, 0);
      		}
		}

		TCanvas* c = new TCanvas();
		c->SetLogy();


		hs->Draw("nostack");

		hs->SetTitle(fname.c_str());

		hs->GetXaxis()->SetTitle("Angle [deg.]");
		hs->GetYaxis()->SetTitle("Counts");

		//leg->SetTextSize(0.03);
    	//leg->Draw("same");

    	// c->SaveAs(("angle_distribution_" + fname + ".pdf").c_str());
        c->SaveAs((fname + ".png").c_str());
	}

	void DrawEnergyDistribution(){

		TCanvas* c = new TCanvas();
		c->SetLogy();


		hXY->Draw();

		hXY->SetTitle(fname.c_str());

		hXY->GetXaxis()->SetTitle("Angle [deg.]");
		hXY->GetYaxis()->SetTitle("Energy (MeV)");

        c->SaveAs((fname + "_trackID.png").c_str());
	}

	void DrawProblematicRadius(){

		TCanvas* c = new TCanvas();
		c->SetLogy();


		hR->Draw();

		hR->SetTitle(fname.c_str());

		hR->GetXaxis()->SetTitle("Radius [mm]");
		hR->GetYaxis()->SetTitle("Counts");

        c->SaveAs((fname + "_ProblematicRadius.png").c_str());
	}

	void DrawProblematicZ(){

		TCanvas* c = new TCanvas();
		c->SetLogy();


		hZ->Draw();

		hZ->SetTitle(fname.c_str());

		hZ->GetXaxis()->SetTitle("Z [mm]");
		hZ->GetYaxis()->SetTitle("Counts");

		c->SaveAs((fname + "_ProblematicZ.png").c_str());
	}
};

void source_check(){

	map<string, string> files;

	files["DD neutron"] = "JanisTest.root";

	map<string, double> weights;

	weights["DD neutron"] = 1;//1e9 / 1e9; // activity/simulated events

	cout << "DD neutron scale factor is " << weights["DD neutron"] << endl;

	PlotCollection plots_1("Angular Distribution of DD Neutron");
	PlotCollection plots_2("Energy Angular Distribution of DD Neutron");
	PlotCollection plots_3("Problematic Radius Distribution");
	PlotCollection plots_4("Problematic Z Distribution");

	for(auto it : files){

		string file_label = it.first;
		string filename = it.second;

		TFile* f = TFile::Open(filename.c_str());

		if(!f){
			cout << "ERROR reading file " << filename << endl;
			continue;
		} else {
			cout << "Read from " << filename << endl;
		}

		TTree* events = (TTree*)f->Get("Janis");
		int nentries = events->GetEntries();

    	int track_ID;
    	events->SetBranchAddress("TrackID",&track_ID);
		int step_ID;
		events->SetBranchAddress("StepID",&step_ID);
    	int pid;
    	events->SetBranchAddress("ParticleType",&pid);
    	int volume_ID;
    	events->SetBranchAddress("TestVolume",&volume_ID);
    	double energy;
    	events->SetBranchAddress("ParticleE",&energy);
   	 	double x_pos;
   	 	events->SetBranchAddress("Xpos",&x_pos);
    	double y_pos;
    	events->SetBranchAddress("Ypos",&y_pos);
    	double z_pos;
    	events->SetBranchAddress("Zpos",&z_pos);
    	double x_mom;
    	events->SetBranchAddress("Xmom",&x_mom);
    	double y_mom;
    	events->SetBranchAddress("Ymom",&y_mom);
    	double z_mom;
    	events->SetBranchAddress("Zmom",&z_mom);

    	for(int i=0; i<nentries; ++i){
    		events->GetEntry(i);
    		if(step_ID==1 && track_ID==1){
			plots_1.AddEvent(file_label, pid, energy, x_pos, y_pos, z_pos, x_mom, y_mom, z_mom, weights[it.first]);
			plots_2.AddEvent(file_label, pid, energy, x_pos, y_pos, z_pos, x_mom, y_mom, z_mom, weights[it.first]);
			plots_3.AddEvent(file_label, pid, energy, x_pos, y_pos, z_pos, x_mom, y_mom, z_mom, weights[it.first]);
			plots_4.AddEvent(file_label, pid, energy, x_pos, y_pos, z_pos, x_mom, y_mom, z_mom, weights[it.first]);
			}
		}

	}

    plots_1.DrawAngleDistribution();
	plots_2.DrawEnergyDistribution();
	plots_3.DrawProblematicRadius();
	plots_4.DrawProblematicZ();

}
