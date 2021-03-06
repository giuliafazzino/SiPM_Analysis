#ifndef READ_TREE_THRESHOLD_H
#define READ_TREE_THRESHOLD_H


#include "AnalysisUtility.h"
#include "Histograms.h"

void readTreeThreshold(Int_t i_pvol, Int_t i_bdac, TString fFileName = ""){

    if (fFileName == "") {
        cout <<"NO FILENAME" <<endl;
    }

    TFile* fInputFile = new TFile(fFileName);
    TTree* tInputData = (TTree*)(fInputFile->Get("tree"));

    Float_t period_off, rate_off, period_on, rate_on;
    Int_t b_dac, p_vol, counts_off, counts_on, threshold;
    tInputData->SetBranchAddress("bias_dac", &b_dac);
    tInputData->SetBranchAddress("pulse_voltage", &p_vol);
    tInputData->SetBranchAddress("threshold_off", &threshold);
    tInputData->SetBranchAddress("counts_off", &counts_off);
    tInputData->SetBranchAddress("period_off", &period_off);
    tInputData->SetBranchAddress("rate_off", &rate_off);
    tInputData->SetBranchAddress("counts_on", &counts_on);
    tInputData->SetBranchAddress("period_on", &period_on);
    tInputData->SetBranchAddress("rate_on", &rate_on);

    TH1F*   hScan_On  = new TH1F( "hScan_On", "hScan_On", 64, -0.5, 63.5);
    hScan_On->GetXaxis()->SetTitle("Threshold");
    hScan_On->GetYaxis()->SetTitle("Rate on / 100 kHz");

    TH1F*   hScan_Off  = new TH1F( "hScan_Off", "hScan_Off", 64, -0.5, 63.5);
    hScan_Off->GetXaxis()->SetTitle("Threshold");
    hScan_Off->GetYaxis()->SetTitle("Rate off / 100 kHz");

    TH1F*   hScan_Dif  = new TH1F( "hScan_Dif", "hScan_Dif ", 64, -0.5, 63.5);
    hScan_Dif->GetXaxis()->SetTitle("Threshold");
    hScan_Dif->GetYaxis()->SetTitle("Rate on - off / 100 kHz");

    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> map_on;
    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> map_off;
    std::vector<Float_t> thresholds;
    Float_t current_thres=-9999;

    for(int j = 0; j != tInputData->GetEntries(); ++j) {
        tInputData->GetEvent(j);
        if(p_vol == i_pvol && b_dac == i_bdac) {
            if (current_thres != threshold) {
                current_thres = threshold;
                thresholds.push_back(current_thres);
            }
            map_on[current_thres].push_back({counts_on, period_on});
            map_off[current_thres].push_back({counts_off, period_off});
        }

    }
    fillHist(hScan_On, hScan_Off, hScan_Dif, map_on, map_off, thresholds);

    TFile*  fOutputFile  =   new TFile( fFileName + "_pvol_" + i_pvol + "_bdac_" + i_bdac + TString(".out.root"), "RECREATE" );
    hScan_On->Write();
    hScan_Off->Write();
    hScan_Dif->Write();

    fOutputFile->Close();
    fInputFile->Close();
}

#endif
