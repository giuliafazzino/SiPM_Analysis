#ifndef READ_TREE_BIAS_H
#define READ_TREE_BIAS_H

#include "AnalysisUtility.h"
#include "Histograms.h"

void readTreeBias(Int_t i_pvol, Int_t i_thres, TString fFileName = ""){

    if (fFileName == "") {
        cout <<"NO FILENAME" <<endl;
    }

    TFile* fInputFile = new TFile(fFileName);
    TTree* tInputData = (TTree*)(fInputFile->Get("tree"));

    Float_t voltage, period_on, period_off, rate_on, rate_off;
    Int_t p_vol, counts_on, counts_off,threshold;
    tInputData->SetBranchAddress("bias_voltage", &voltage);
    tInputData->SetBranchAddress("pulse_voltage", &p_vol);
    tInputData->SetBranchAddress("counts_on", &counts_on);
    tInputData->SetBranchAddress("counts_off", &counts_off);
    tInputData->SetBranchAddress("period_on", &period_on);
    tInputData->SetBranchAddress("period_off", &period_off);
    tInputData->SetBranchAddress("rate_on", &rate_on);
    tInputData->SetBranchAddress("rate_off", &rate_off);
    tInputData->SetBranchAddress("threshold_off", &threshold);

    TH1F*   hScan_On  = new TH1F( "hScan_On", "hScan_On", 135, 48.27, 56.37);
    hScan_On->GetXaxis()->SetTitle("Bias Voltage (V)");
    hScan_On->GetYaxis()->SetTitle("Rate on / 100 kHz");

    TH1F*   hScan_Off  = new TH1F( "hScan_Off", "hScan_Off", 135, 48.27, 56.37);
    hScan_Off->GetXaxis()->SetTitle("Bias Voltage (V)");
    hScan_Off->GetYaxis()->SetTitle("Rate off / 100 kHz");

    TH1F*   hScan_Dif  = new TH1F( "hScan_Dif", "hScan_Dif", 135, 48.27, 56.37);
    hScan_Dif->GetXaxis()->SetTitle("Bias Voltage (V)");
    hScan_Dif->GetYaxis()->SetTitle("Rate on-off / 100 kHz");

    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> map_on;
    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> map_off;
    std::vector<Float_t> voltages;
    Float_t current_voltage=-9999;

    for(int j=0; j!=tInputData->GetEntries(); ++j) {
        tInputData->GetEvent(j);
        if(p_vol==i_pvol && threshold==i_thres) {
            if (current_voltage != voltage) {
                current_voltage = voltage;
                voltages.push_back(current_voltage);
            }
            map_on[current_voltage].push_back({(float) counts_on, period_on});
            map_off[current_voltage].push_back({(float) counts_off, period_off});
        }
        if(period_off <= 1.E-9 || period_on <= 1.E-9){
            cout <<"ZERO!" <<endl;
        }
    }

    fillHist(hScan_On, hScan_Off, hScan_Dif, map_on, map_off, voltages);

    TFile*  fOutputFile  =   new TFile( fFileName + "_pvol_" + i_pvol + "_thres_" + i_thres + TString(".out.root"), "RECREATE" );
    hScan_On->Write();
    hScan_Off->Write();
    hScan_Dif->Write();

    fOutputFile->Close();
    fInputFile->Close();
}

#endif
