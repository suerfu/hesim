
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>

#include "TTree.h"
#include "TFile.h"

struct track_info{
    int eventID;
    int trackID;
    int stepID;
    int parentID;
    char particle_name[16];
    char volume_name[16];
    int volume_copy_number;
    double Eki;
    double Ekf;
    double edep;
    double gtime;
    char proc_name[16];
};


struct event_info{
    Int_t ID = -1;
    Int_t evtID = -1;

    Double_t edep_he = 0;
//    double edep_he_n = 0;
//    double edep_he_a = 0;
    Double_t time_he = 1.e10;

    Double_t edep_nai[8];
    Double_t time_nai[8];

//    double edep_ls[9];
//    double time_ls[9];
};

using namespace std;

int main( int argc, char* argv[]){ 
    //process_track(string filename, string output_name){

    bool print_usage = false;

    if( argc==1 ){
        print_usage = true;
    }
    
    else{
        for( int i=1; i<argc; i++){
            if( strncmp( argv[i], "-h", 2) ==0 || strncmp( argv[i], "--help", 6) ==0 ){
                print_usage = true;
            }
        }
    }

    if( print_usage){
        cout << "usage: " << argv[0] << " File-to-process-0 [ File-to-Process-1 ...] Output-ROOT-File\n";
        return -1;
    }


    TTree* tree = new TTree( "events", "MC simulation for Compton scattering");
    event_info wdata;   // data for writing

    tree->Branch( "ID", &wdata.ID, "ID/I");
    tree->Branch( "evtID", &wdata.evtID, "evtID/I");
    tree->Branch( "edep_he", &wdata.edep_he, "edep_he/D" );
    tree->Branch( "time_he", &wdata.time_he, "time_he/D");
    tree->Branch( "edep_nai", &wdata.edep_nai, "edep_nai[8]/D" );
    tree->Branch( "time_nai", &wdata.time_nai, "time_nai[8]/D");

    for( int t = 1; t<argc-1; t++ ){

        string filename( argv[t] );
        TFile* infile = TFile::Open( filename.c_str());
        if( !infile ){
            cout << "ERROR reading file " << filename << endl;
        }
        else
            cout << "Processing " << filename << endl;

        TTree* events = (TTree*)infile->Get("Janis");
        int nentries = events->GetEntries();

        track_info data;    // data for reading

        events -> SetBranchAddress("eventID", &data.eventID);
        events -> SetBranchAddress("trackID", &data.trackID);
        events -> SetBranchAddress("stepID", &data.stepID);
        events -> SetBranchAddress("parentID", &data.parentID);
        events -> SetBranchAddress("particle", &data.particle_name);
        events -> SetBranchAddress("volume", &data.volume_name);
//        events -> SetBranchAddress("volume_copy_nr", &data.volume_copy_number);
        events -> SetBranchAddress("Eki", &data.Eki);
        events -> SetBranchAddress("Ekf", &data.Ekf);
        events -> SetBranchAddress("Edep", &data.edep);
        events -> SetBranchAddress("t", &data.gtime);
        events -> SetBranchAddress("process", &data.proc_name);

        cout << nentries << " events in the event tree.\n";


    //    infile->cd();

        int evt_counter = -1;
        for( unsigned int i=0; i<nentries; i++){
            //cout << i << endl;
            events->GetEntry(i);
            if( i%1000==0 )
                cout << "processed " << i << " entries" << endl;

            if( data.parentID==0 && strncmp( data.proc_name, "initStep", 8)==0 ){
                if( i!=0 ){
                    //cout << "filling tree at i=" << i << endl;
                    tree->Fill();
                }

                wdata.ID = i;
                wdata.edep_he = 0;
    //            wdata.edep_he_n = 0;
    //            wdata.edep_he_a = 0;
                wdata.time_he = -1;

                for( int j=0; j<8; j++){
                    wdata.edep_nai[j] = 0;
                    wdata.time_nai[j] = -1;
                }/*
                for( int j=0; j<9; j++){
                    wdata.edep_ls[j] = 0;
                    wdata.time_ls[j] = -1;
                }*/

                evt_counter++;
                wdata.ID = evt_counter;
                wdata.evtID = data.eventID;
            }

            if( strncmp( data.volume_name, "liquid helium", 13)==0 ){
                wdata.edep_he += data.edep;
                if( wdata.time_he<0 || wdata.time_he>data.gtime )
                    wdata.time_he = data.gtime;
            }
            else if( strncmp( data.volume_name, "NaI", 3)==0 ){
                int index = (data.volume_name)[3] - '1';
                //cout << " index is " << index << endl;
                wdata.edep_nai[index] += data.edep;
                if( wdata.time_nai[index]<0 || wdata.time_nai[index]>data.gtime )
                    wdata.time_nai[index] = data.gtime;
            }/*
            else if( data.volume_name->find("LS")==0 ){
                int index = (*data.volume_name)[2] - '0';
                wdata.edep_nai[index] += data.edep;
                if( wdata.time_nai[index]>data.gtime )
                    wdata.time_nai[index] = data.gtime;
            }

            int index_first = -1;
            double time_first = -1;
            for( int j=0; j<8; j++){
                if( time_first<0 || time_first>wdata.time_nai[j] ){
                    index_first = j;
                    time_first = wdata.time_nai[j];
                }
            }
            cout << index_first << '\t' << time_first << endl;
            for( int j=0; j<8; j++){
                if( j!=index_first )
                    wdata.time_nai[j] = -1;
            }
            */

        }

        infile->Close();
    }

    string output_name = argv[argc-1];
    TFile* outfile = new TFile( output_name.c_str(), "RECREATE");
    if( !outfile ){
        cout << "Error creating file " << output_name << endl;
        return -2;
    }
    cout << "last step\n";
    //outfile->cd();
    tree->Write();
    cout << "tree written\n";
    outfile->Close();

    return 0;
}


