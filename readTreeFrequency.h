
#ifndef READ_TREE_FREQUENCY_H
#define READ_TREE_FREQUENCY_H

#include "AnalysisUtility.h"
#include "Histograms.h"

void readTreeFrequency(Int_t i_pvol, Int_t i_bdac, Int_t i_thre, TString fFileName = ""){

    if (fFileName == "") {
        cout <<"NO FILENAME" <<endl;
    }

    TFile* fInputFile = new TFile(fFileName);
    TTree* tInputData = (TTree*)(fInputFile->Get("tree"));

    Float_t period_off, rate_off, period_on, rate_on;
    Int_t b_dac, p_vol, p_fre, counts_off, counts_on, threshold;
    tInputData->SetBranchAddress("bias_dac", &b_dac);
    tInputData->SetBranchAddress("pulse_voltage", &p_vol);
    tInputData->SetBranchAddress("pulse_frequency", &p_fre);
    tInputData->SetBranchAddress("threshold_off", &threshold);
    tInputData->SetBranchAddress("counts_off", &counts_off);
    tInputData->SetBranchAddress("period_off", &period_off);
    tInputData->SetBranchAddress("rate_off", &rate_off);
    tInputData->SetBranchAddress("counts_on", &counts_on);
    tInputData->SetBranchAddress("period_on", &period_on);
    tInputData->SetBranchAddress("rate_on", &rate_on);

    TH1F*   hScan_On  = new TH1F( "hScan_On", "hScan_On", 121, -0.5, 120.5);
    hScan_On->GetXaxis()->SetTitle("Pulse Frequency");
    hScan_On->GetYaxis()->SetTitle("Rate on / Pulser rate");

    TH1F*   hScan_Off  = new TH1F( "hScan_Off", "hScan_Off", 121, -0.5, 120.5);
    hScan_Off->GetXaxis()->SetTitle("Pulse Frequency");
    hScan_Off->GetYaxis()->SetTitle("Rate off / Pulser rate");

    TH1F*   hScan_Dif  = new TH1F( "hScan_Dif", "hScan_Dif ", 121, -0.5, 120.5);
    hScan_Dif->GetXaxis()->SetTitle("Pulse Frequency");
    hScan_Dif->GetYaxis()->SetTitle("Rate on - off / Pulser rate");

    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> map_on;
    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> map_off;
    std::vector<Float_t> freqs;
    Float_t current_fre=-9999;

    for(int j = 0; j != tInputData->GetEntries(); ++j) {
        tInputData->GetEvent(j);
        if(p_vol == i_pvol && b_dac == i_bdac && threshold == i_thre) {
            if (current_fre != p_fre) {
                current_fre = p_fre;
                freqs.push_back(current_fre);
            }
            map_on[current_fre].push_back({counts_on * 1E5 / (p_fre * 1E3) , period_on});
            map_off[current_fre].push_back({counts_off * 1E5 / (p_fre * 1E3) , period_off});
        }

    }
    fillHist(hScan_On, hScan_Off, hScan_Dif, map_on, map_off, freqs);

    TFile*  fOutputFile  =   new TFile( fFileName + "_pvol_" + i_pvol + "_bdac_" + i_bdac + "_thre_" + i_thre + TString(".out.root"), "RECREATE" );
    hScan_On->Write();
    hScan_Off->Write();
    hScan_Dif->Write();

    fOutputFile->Close();
    fInputFile->Close();
}



#endif
