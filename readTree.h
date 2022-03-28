
#ifndef READ_TREE_H
#define READ_TREE_H

#include "AnalysisUtility.h"
#include "Histograms.h"
/*
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

    TH1F*   hScan_On  = new TH1F( "hScan_On", "hScan_On", 100, -5, 105);
    hScan_On->GetXaxis()->SetTitle("Pulse Frequency");
    hScan_On->GetYaxis()->SetTitle("Rate on / 100 kHz");

    TH1F*   hScan_Off  = new TH1F( "hScan_Off", "hScan_Off", 101, -0.5, 100.5);
    hScan_Off->GetXaxis()->SetTitle("Pulse Frequency");
    hScan_Off->GetYaxis()->SetTitle("Rate off / 100 kHz");

    TH1F*   hScan_Dif  = new TH1F( "hScan_Dif", "hScan_Dif ", 101, -0.5, 100.5);
    hScan_Dif->GetXaxis()->SetTitle("Pulse Frequency");
    hScan_Dif->GetYaxis()->SetTitle("Rate on - off / 100 kHz");

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
            map_on[current_fre].push_back({counts_on, period_on});
            map_off[current_fre].push_back({counts_off, period_off});
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

void readTreePos
( TString fFileName = "", Bool_t kIsX = true ) {
    if ( fFileName == "" ) {
        cout << " NO FILENAME AAAAHHHHH "  << endl;
    }
    
    TFile*  fInputFile  =   new TFile( fFileName );
    
    TTree*  tInputData  =   (TTree*)(fInputFile->Get("tree"));
    
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
    
    TH1F*   hScan_On  = new TH1F( "hScan_On", "hScan_On", 51, -5, +5 );
    hScan_On->GetXaxis()->SetTitle("position (mm)");
    hScan_On->GetYaxis()->SetTitle("rate (Hz)");
    
    TH1F*   hScan_Off  = new TH1F( "hScan_Off", "hScan_Off", 51, -5, +5 );
    hScan_Off->GetXaxis()->SetTitle("position (mm)");
    hScan_Off->GetYaxis()->SetTitle("rate (Hz)");
    
    TH1F*   hScan_Dif  = new TH1F( "hScan_Dif", "hScan_Dif", 51, -5, +5 );
    hScan_Dif->GetXaxis()->SetTitle("position (mm)");
    hScan_Dif->GetYaxis()->SetTitle("rate (Hz)");
    
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
    
    hScan_On->Write();
    hScan_Off->Write();
    hScan_Dif->Write();

    fOutputFile->Close();

    fInputFile->Close();
}

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

*/



// qui inizia la prova
struct InputData{
    float b_vol, period_off, rate_off, ratee_off, temp_off, period_on, rate_on, ratee_on, temp_on;
    int b_dac, p_vol, p_fre, thre_off, tstamp_off, counts_off, thre_on, tstamp_on, counts_on;
}
;

void decode_tree(InputData& myInput, TTree* tInputData){
    
    tInputData->SetBranchAddress("bias_voltage"   , &myInput.b_vol);
    tInputData->SetBranchAddress("bias_dac"       , &myInput.b_dac);
    tInputData->SetBranchAddress("pulse_voltage"  , &myInput.p_vol);
    tInputData->SetBranchAddress("pulse_frequency", &myInput.p_fre);
    tInputData->SetBranchAddress("threshold_off"  , &myInput.thre_off);
    tInputData->SetBranchAddress("timestamp_off"  , &myInput.tstamp_off);
    tInputData->SetBranchAddress("counts_off"     , &myInput.counts_off);
    tInputData->SetBranchAddress("period_off"     , &myInput.period_off);
    tInputData->SetBranchAddress("rate_off"       , &myInput.rate_off);
    tInputData->SetBranchAddress("ratee_off"      , &myInput.ratee_off);
    tInputData->SetBranchAddress("temperature_off", &myInput.temp_off);
    tInputData->SetBranchAddress("threshold_on"   , &myInput.thre_on);
    tInputData->SetBranchAddress("timestamp_on"   , &myInput.tstamp_on);
    tInputData->SetBranchAddress("counts_on"      , &myInput.counts_on);
    tInputData->SetBranchAddress("period_on"      , &myInput.period_on);
    tInputData->SetBranchAddress("rate_on"        , &myInput.rate_on);
    tInputData->SetBranchAddress("ratee_on"       , &myInput.ratee_on);
    tInputData->SetBranchAddress("temperature_on" , &myInput.temp_on);
    
    
}

void readTreeBias(Int_t i_pvol, Int_t i_thres, TString fFileName = ""){

    if (fFileName == "") {
        cout <<"NO FILENAME" <<endl;
    }

    TFile* fInputFile = new TFile(fFileName);
    TTree* tInputData = (TTree*)(fInputFile->Get("tree"));
    
    InputData iInputData;
    
    decode_tree(iInputData, tInputData);

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
        if(iInputData.p_vol==i_pvol && iInputData.thre_off==i_thres) {
            if (current_voltage != iInputData.b_vol) {
                current_voltage = iInputData.b_vol;
                voltages.push_back(current_voltage);
            }
            map_on[current_voltage].push_back({(float)iInputData.counts_on, iInputData.period_on});
            map_off[current_voltage].push_back({(float)iInputData.counts_off, iInputData.period_off});
        }
        if(iInputData.period_off <= 1.E-9 || iInputData.period_on <= 1.E-9){
            cout <<"ZERO!" <<endl;
        }
    }

    fillHist(hScan_On, hScan_Off, hScan_Dif, map_on, map_off, voltages);

    TFile*  fOutputFile  =   new TFile("YAY" +  fFileName + "_pvol_" + i_pvol + "_thres_" + i_thres + TString(".out.root"), "RECREATE" );
    hScan_On->Write();
    hScan_Off->Write();
    hScan_Dif->Write();

    fOutputFile->Close();
    fInputFile->Close();
}

#endif


