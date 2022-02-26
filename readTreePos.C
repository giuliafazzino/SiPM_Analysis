#include"AnalysisUtility.h"
#include "Histograms.h"

void
readTree
( TString fFileName = "", Bool_t kIsX = true ) {
    if ( fFileName == "" ) {
        cout << " NO FILENAME AAAAHHHHH "  << endl;
    }
    //
    TFile*  fInputFile  =   new TFile( fFileName );
    //
    TTree*  tInputData  =   (TTree*)(fInputFile->Get("tree"));
    //
    Float_t position, counts_on, counts_off, period_on, period_off;
    if (kIsX) {
        tInputData->SetBranchAddress("position_x", &position);
    } else {
        tInputData->SetBranchAddress("position_y", &position);
    }
    tInputData->SetBranchAddress( "counts_on", &counts_on );
    tInputData->SetBranchAddress( "counts_off", &counts_off );
    tInputData->SetBranchAddress( "period_on", &period_on );
    tInputData->SetBranchAddress( "period_off", &period_off );
    //
    TH1F*   hScan_On  = new TH1F( "hScan_On", "hScan_On", 51, -5, +5 );
    hScan_On->GetXaxis()->SetTitle("position (mm)");
    hScan_On->GetYaxis()->SetTitle("rate (Hz)");
    //
    TH1F*   hScan_Off  = new TH1F( "hScan_Off", "hScan_Off", 51, -5, +5 );
    hScan_Off->GetXaxis()->SetTitle("position (mm)");
    hScan_Off->GetYaxis()->SetTitle("rate (Hz)");
    //
    TH1F*   hScan_Dif  = new TH1F( "hScan_Dif", "hScan_Dif", 51, -5, +5 );
    hScan_Dif->GetXaxis()->SetTitle("position (mm)");
    hScan_Dif->GetYaxis()->SetTitle("rate (Hz)");
    //
    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> map_off;
    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> map_on;
    std::vector<Float_t> positions;
    Float_t current_position=-9999;
    for ( Int_t iMes = 0; iMes < tInputData->GetEntries(); iMes++) {
        tInputData->GetEvent(iMes);
        float pos = position / 800.;
        if (current_position != pos) {
            current_position = pos;
            positions.push_back(current_position);
        }
        map_off[current_position].push_back({counts_off, period_off});
        map_on[current_position].push_back({counts_on, period_on});
    }
    fillHist(hScan_On, hScan_Off, hScan_Dif, map_on, map_off, positions);

    TFile*  fOutputFile  =   new TFile( fFileName + TString(".out.root"), "RECREATE" );
    //
    hScan_On->Write();
    hScan_Off->Write();
    hScan_Dif->Write();
    //

    fOutputFile->Close();

    fInputFile->Close();
}
