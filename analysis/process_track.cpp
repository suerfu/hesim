
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>

#include "TTree.h"
#include "TGraph.h"
#include "TFile.h"

using namespace std;

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
    double x;
    double y;
    double z;
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
    Double_t ncrkv;
};

class CerenkovCalculator{

public:

    CerenkovCalculator();
    ~CerenkovCalculator();

    void LoadData(  string file_nr, string file_qe );
    float GetPhD( double E );
        // returns number of detectable photons produced per unit length in nm.

private:

    vector<float> qex;
    vector<float> qey;
    TGraph* qe;

    vector<float> nx;
    vector<float> ny;
    TGraph* nref;

    vector<float> w_eval;
    vector<float> qe_eval;
    vector<float> nref_eval;

    int Nsteps;
};

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

    CerenkovCalculator cerenkov;
    cerenkov.LoadData( "Nrefr_FusedSilica.txt",  "QE_R8520.txt");

    string output_name = argv[argc-1];
    TFile* outfile = new TFile( output_name.c_str(), "RECREATE");
    if( !outfile ){
        cout << "Error creating file " << output_name << endl;
        return -2;
    }

    TTree* tree = new TTree( "events", "MC simulation for Compton scattering");
    event_info wdata;   // data for writing

    tree->Branch( "ID", &wdata.ID, "ID/I");
    tree->Branch( "evtID", &wdata.evtID, "evtID/I");
    tree->Branch( "edep_he", &wdata.edep_he, "edep_he/D" );
    tree->Branch( "time_he", &wdata.time_he, "time_he/D");
    tree->Branch( "edep_nai", &wdata.edep_nai, "edep_nai[8]/D" );
    tree->Branch( "time_nai", &wdata.time_nai, "time_nai[8]/D");
    tree->Branch( "ncrkv", &wdata.ncrkv, "ncrkv/D" );

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
        events -> SetBranchAddress("x", &data.x);
        events -> SetBranchAddress("y", &data.y);
        events -> SetBranchAddress("z", &data.z);
        events -> SetBranchAddress("t", &data.gtime);
        events -> SetBranchAddress("process", &data.proc_name);

        //cout << nentries << " events in the event tree.\n";


    //    infile->cd();

        double Ecrkv = 0;   double Xcrkv = 0;   double Ycrkv = 0;   double Zcrkv = 0;
        int track_crkv = -1;

        int evt_counter = -1;
        for( unsigned int i=0; i<nentries; i++){

            events->GetEntry(i);

            // parent ID equals 0 and initStep marks a new event. Fill previous event and initialize variables.
            if( data.parentID==0 && strncmp( data.proc_name, "initStep", 8)==0 ){
                if( i!=0 ){
                    //cout << "filling tree at i=" << i << endl;
                    tree->Fill();
                }

                wdata.ID = i;
                wdata.edep_he = 0;
                //wdata.edep_he_n = 0;
                //wdata.edep_he_a = 0;
                wdata.time_he = -1;

                for( int j=0; j<8; j++){
                    wdata.edep_nai[j] = 0;
                    wdata.time_nai[j] = -1;
                }
                /*
                for( int j=0; j<9; j++){
                    wdata.edep_ls[j] = 0;
                    wdata.time_ls[j] = -1;
                }
                */

                evt_counter++;
                wdata.ID = evt_counter;
                wdata.evtID = data.eventID;

                wdata.ncrkv = 0;
            }

            /* Process liuid helium */
            if( strncmp( data.volume_name, "liquid helium", 13)==0 ){
                wdata.edep_he += data.edep;
                if( wdata.time_he<0 || wdata.time_he>data.gtime )
                    wdata.time_he = data.gtime;
            }
            /* Process far-side NaI detector */
            else if( strncmp( data.volume_name, "NaI", 3)==0 ){
                int index = (data.volume_name)[3] - '1';
                //cout << " index is " << index << endl;
                wdata.edep_nai[index] += data.edep;
                if( wdata.time_nai[index]<0 || wdata.time_nai[index]>data.gtime )
                    wdata.time_nai[index] = data.gtime;
            }

            /* Process cerenkov */
            // For Cerenkov events, multiple electrons will have all initStep process name within the same event frame.

            if( strncmp( data.particle_name, "e-", 2)==0 ){

                if( strncmp( data.proc_name, "initStep", 8)!=0 ){
                    
                    if( strncmp( data.volume_name, "quartz_cell", 11)==0 || strncmp( data.volume_name, "pmt_Window", 10)==0 ){
    
                        // Get current position and compute the distance traveled.
                        float dx = data.x-Xcrkv;
                        float dy = data.y-Ycrkv;
                        float dz = data.z-Zcrkv;
                        float dl = sqrt( dx*dx + dy*dy + dz*dz )*1000000;
                        //cout << i << '\t' << data.x << '\t' << data.y << '\t' << data.z << '\t' << dl/1000000 << endl;

                        // Compute number of cerenkov photons expected, using the initial energy.
                        wdata.ncrkv += cerenkov.GetPhD(Ecrkv)*dl;
                    }
                }
                // Update the energy of the particle, regardless of Cerenkov or not.
                Ecrkv = data.Eki;
                Xcrkv = data.x;  Ycrkv = data.y;  Zcrkv = data.z;
            }

            /*
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

    //cout << "last step\n";
    outfile->cd();
    tree->Write();
    cout << "tree written\n";
    outfile->Close();

    return 0;
}



CerenkovCalculator::CerenkovCalculator(){
    qe = 0;
    nref = 0;
    Nsteps = 100;
}



CerenkovCalculator::~CerenkovCalculator(){}



void CerenkovCalculator::LoadData(  string file_nr, string file_qe ){

    ifstream input;
    input.open( file_nr.c_str() );

    if( input.good() ){
        while( !isdigit( input.peek() ) ){
            string foo;
            getline( input, foo);
        }
        while( input.good() ){
            float tx, ty;
            input >> tx >> ty;
            nx.push_back(tx);
            ny.push_back(ty);
        }
        nref = new TGraph(nx.size(), &nx[0], &ny[0]);
    }
    input.close();

    input.open( file_qe.c_str() );
    if( input.good() ){
        while( !isdigit( input.peek() ) ){
            string foo;
            getline( input, foo);
        }
        while( input.good() ){
            float tx, ty;
            input >> tx >> ty;
            qex.push_back(tx);
            qey.push_back(ty);
        }
        qe = new TGraph(qex.size(), &qex[0], &qey[0]);
    }

    float wmin = *qex.begin();
    float wmax = ( *(qex.end()-1) > *(nx.end()-1) ) ? *(nx.end()-1) : *(qex.end()-1);
    
    for( int i=0; i<Nsteps; i++){
        float wavelength = wmin + i*(wmax-wmin)/Nsteps;
        w_eval.push_back( wavelength );
        qe_eval.push_back( qe->Eval( wavelength ) );
        nref_eval.push_back( nref->Eval( wavelength ) );
    }
    
    /*
    for( int i=0; i<w_eval.size(); i++){
        cout << w_eval[i] << '\t' << qe_eval[i] << '\t' << nref_eval[i] << endl;
    }
    */
}


// returns number of detectable photons produced per unit length in nm.
float CerenkovCalculator::GetPhD( double E ){
    
    if( qe==0 || nref==0 )
        return -1;

    float norm = 0.0458506184;
    float gamma = E/0.511+1;
    float beta = sqrt(gamma*gamma-1)/gamma;

    float sum = 0;
    for( unsigned int i=0; i<w_eval.size(); i++){
        float c = ( 1 - 1/(beta*beta*nref_eval[i]*nref_eval[i]) );
        if( c>0 )
            sum += 1/(w_eval[i]*w_eval[i]) * c * ( w_eval[1] - w_eval[0] );
    }
    return sum*norm;
}

