
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>

#include "TTree.h"
#include "TFile.h"

using namespace std;

// structure to hold information read from input ROOT file.
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


// structure to hold information for ROOT output
struct event_info{
    char file_name[128];

    Int_t ID = -1;
    Int_t evtID = -1;

    Double_t edep_he_e = 0;
    Double_t edep_he_n = 0;
    Double_t time_he = -1;

    Double_t edep_fs_e[10];
    Double_t edep_fs_n[10];
    Double_t time_fs[10] = {-1};

    Double_t time_floor = -1;
/*
    vector<double> E_he_e;
    vector<double> E_he_n;
    vector<double> T_he_n;

    vector<double> E_fs_e;
    vector<double> E_fs_n;
    vector<double> T_fs_n;
*/
};


// reset event info.
void ResetEventInfo( event_info* wdata ){
    wdata->evtID = -1;
    wdata->ID = -1;

    wdata->edep_he_e = 0;
    wdata->edep_he_n = 0;
    wdata->time_he = -1;

    for( unsigned int j=0; j<10; j++){
        wdata->edep_fs_e[j] = 0;
        wdata->edep_fs_n[j] = 0;
        wdata->time_fs[j] = -1;
    }
    
    wdata->time_floor = -1;
}



int main( int argc, char* argv[]){ 

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
        return 0;
    }

    string output_name = argv[argc-1];
    TFile* outfile = new TFile( output_name.c_str(), "NEW");
    if( !outfile ){
        cout << "Error creating file " << output_name << endl;
        return -2;
    }

    TTree* tree = new TTree( "events", "MC simulation for Compton scattering");
    event_info wdata;   // data for writing

    tree->Branch( "file", &wdata.file_name, "file[128]/C");
    tree->Branch( "ID", &wdata.ID, "ID/I");
    tree->Branch( "evtID", &wdata.evtID, "evtID/I");
    tree->Branch( "edep_he_e", &wdata.edep_he_e, "edep_he_e/D" );
    tree->Branch( "edep_he_n", &wdata.edep_he_n, "edep_he_n/D" );
    tree->Branch( "time_he", &wdata.time_he, "time_he/D");
    tree->Branch( "edep_fs_e", &wdata.edep_fs_e, "edep_fs_e[10]/D" );
    tree->Branch( "edep_fs_n", &wdata.edep_fs_n, "edep_fs_n[10]/D" );
    tree->Branch( "time_fs", &wdata.time_fs, "time_fs[10]/D");
    
    int floor_flag = 0;
    tree->Branch( "floor_flag", &floor_flag, "floor_flag/I");

    // ************************** //
    // * Process the input file * //
    // ************************** //


    for( int t = 1; t<argc-1; t++ ){

        string filename( argv[t] );
        TFile* infile = TFile::Open( filename.c_str(), "READ");

        if( !infile ){
            cout << "ERROR reading file " << filename << endl;
        }
        else{
            cout << "Processing " << filename << endl;
        }

        strncpy( wdata.file_name, argv[t], 128);

        TTree* events = (TTree*)infile->Get("Janis");
        int nentries = events->GetEntries();

        track_info data;    // data for reading
        events -> SetBranchAddress("eventID", &data.eventID);
        events -> SetBranchAddress("trackID", &data.trackID);
        events -> SetBranchAddress("stepID", &data.stepID);
        events -> SetBranchAddress("parentID", &data.parentID);
        events -> SetBranchAddress("particle", &data.particle_name);
        events -> SetBranchAddress("volume", &data.volume_name);
        events -> SetBranchAddress("Eki", &data.Eki);
        events -> SetBranchAddress("Ekf", &data.Ekf);
        events -> SetBranchAddress("Edep", &data.edep);
        events -> SetBranchAddress("t", &data.gtime);
        events -> SetBranchAddress("process", &data.proc_name);


        // ************************** //
        // * Process the input file * //
        // ************************** //

        int evt_counter = -1;
        for( unsigned int i=0; i<nentries; i++){
            
            events->GetEntry(i);

            if( (data.parentID==0 && strncmp( data.proc_name, "initStep", 8)==0) || i==nentries-1 ){

                if( i!=0 ){
                    //ProcessEventInfo( &wdata );
                    double time_fs = -1;
                    for( unsigned int i=0; i<10; i++){
                        if( time_fs<0 ){
                            if ( wdata.time_fs[i]>0 ){
                                time_fs = wdata.time_fs[i];
                            }
                        }
                        else if ( wdata.time_fs[i]>0 ){
                            time_fs = time_fs < wdata.time_fs[i] ? time_fs : wdata.time_fs[i];
                        }
                    }

                    if( wdata.time_floor<0 ){
                        floor_flag = 0;
                    }
                    else if( wdata.time_floor < wdata.time_he ){
                        floor_flag = 1;
                    }
                    else if( wdata.time_floor > wdata.time_he  && wdata.time_floor < time_fs ){
                        floor_flag = 2;
                    }
                    else if( wdata.time_floor > time_fs ){
                        floor_flag = 3;
                    }

                    tree->Fill();
                }
                
                ResetEventInfo( &wdata );
                floor_flag = 0;

                evt_counter++;
                wdata.ID = evt_counter;
                wdata.evtID = data.eventID;
            }

            if( strncmp( data.volume_name, "liquid helium", 13)==0 ){
                if( wdata.time_he<0 ){
                    wdata.time_he = data.gtime;
                }
                if( strncmp( data.proc_name, "eIoni", 5)==0 ){
                    wdata.edep_he_e += data.edep;
                }
                else{
                    wdata.edep_he_n += data.edep;
                }
            }
            else if( strncmp( data.volume_name, "NaI", 3)==0 || strncmp( data.volume_name, "LS", 2)==0 ){
                
                // * First obtain the index of the far-side detector from volume name.* //
                unsigned int nchar = 0;
                    // number of characters in volume name until integer.
                if( strncmp( data.volume_name, "NaI", 3)==0 ){
                    nchar = 3;
                }
                else{
                    nchar =2;
                }

                stringstream ss( data.volume_name);
                char foo;
                for( unsigned int i=0; i<nchar; i++){
                    ss >> foo;
                }

                unsigned int index = 0;
                ss >> index;
                index -= 1;
                    
                if( strncmp( data.proc_name, "eIoni", 5)==0 ){
                    wdata.edep_fs_e[index] += data.edep;
                }
                else if( strncmp( data.proc_name, "hIoni", 5)==0 || strncmp( data.proc_name, "ionIoni", 7)==0 ){
                    wdata.edep_fs_n[index] += data.edep;
                    if( wdata.time_fs[index]<0 ){
                        wdata.time_fs[index] = data.gtime;
                    }
                }
            }
            else if( strncmp( data.volume_name, "floor", 5)==0 ){
                if( strcmp(data.particle_name, "neutron")==0 && wdata.time_floor<0 ){
                    wdata.time_floor = data.gtime;
                }
            }
        }

        infile->Close();
    }

    outfile->cd();
    tree->Write();
    outfile->Close();

    return 0;
}


