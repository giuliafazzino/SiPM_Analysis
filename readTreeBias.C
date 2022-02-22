#include "AnalysisUtility.h"
#include "Histograms.h"

void readTreeBias(TString fFileName = ""){

    if (fFileName == "") {
        cout <<"NO FILENAME" <<endl;
    }

    TFile* fInputFile = new TFile(fFileName);
    TTree* tInputData = (TTree*)(fInputFile->Get("tree"));

    Float_t voltage, counts_on, counts_off, period_on, period_off, rate_on, rate_off;
    tInputData->SetBranchAddress("bias_voltage", &voltage);
    tInputData->SetBranchAddress("counts_on", &counts_on);
    tInputData->SetBranchAddress("counts_off", &counts_off);
    tInputData->SetBranchAddress("period_on", &period_on);
    tInputData->SetBranchAddress("period_off", &period_off);
    tInputData->SetBranchAddress("rate_on", &rate_on);
    tInputData->SetBranchAddress("rate_off", &rate_off);

    TH1F*   hScan_On  = new TH1F( "hScan_On", "hScan_On", 51, 47.2, 57.4);
    hScan_On->GetXaxis()->SetTitle("Bias Voltage (V)");
    hScan_On->GetYaxis()->SetTitle("Rate on / 100 kHz");

    TH1F*   hScan_Off  = new TH1F( "hScan_Off", "hScan_Off", 51, 47.2, 57.4);
    hScan_Off->GetXaxis()->SetTitle("Bias Voltage (V)");
    hScan_Off->GetYaxis()->SetTitle("Rate off / 100 kHz");

    TH1F*   hScan_Dif  = new TH1F( "hScan_Dif", "hScan_Dif", 51, 47.2, 57.4);
    hScan_Dif->GetXaxis()->SetTitle("Bias Voltage (V)");
    hScan_Dif->GetYaxis()->SetTitle("Rate on-off / 100 kHz");

    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> map_on;
    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> map_off;
    std::vector<Float_t> voltages;
    Float_t current_voltage=-9999;

    for(int j=0; j!=tInputData->GetEntries(); ++j) {
        tInputData->GetEvent(j);
        if (current_voltage != voltage) {
            current_voltage = voltage;
            voltages.push_back(current_voltage);
        }
        map_on[current_voltage].push_back({counts_on, period_on});
        map_off[current_voltage].push_back({counts_off, period_off});

    }
    fillHist(hScan_On, hScan_Off, hScan_Dif, map_on, map_off, voltages);

    TFile*  fOutputFile  =   new TFile( fFileName + TString(".out.root"), "RECREATE" );
    hScan_On->Write();
    hScan_Off->Write();
    hScan_Dif->Write();

    fOutputFile->Close();
    fInputFile->Close();
}