#include"AnalysisUtility.h"

void readTree_Vol(TString fFileName = ""){

    if (fFileName == "") {
        cout <<"NO FILENAME" <<endl;
    }

    TFile* fInputFile = new TFile(fFileName);
    TTree* tInputData = (TTree*)(fInputFile->Get("tree"));

    Float_t voltage, counts_on, counts_off, period_on, period_off, rate_on, rate_off;
    tInputData->SetBranchAddress("pulse_voltage", &voltage);
    tInputData->SetBranchAddress("counts_on", &counts_on);
    tInputData->SetBranchAddress("counts_off", &counts_off);
    tInputData->SetBranchAddress("period_on", &period_on);
    tInputData->SetBranchAddress("period_off", &period_off);
    tInputData->SetBranchAddress("rate_on", &rate_on);
    tInputData->SetBranchAddress("rate_off", &rate_off);

    TH1F*   hScan_Off  = new TH1F( "hScan_Off", "hScan_Off", 61, 800, 1100);
    hScan_Off->GetXaxis()->SetTitle("Voltage (mV)");
    hScan_Off->GetYaxis()->SetTitle("Rate (Hz)");

    TH1F*   hScan_On  = new TH1F( "hScan_On", "hScan_On", 61, 800, 1100);
    hScan_On->GetXaxis()->SetTitle("Voltage (mV)");
    hScan_On->GetYaxis()->SetTitle("Rate (Hz)");

    TH1F*   hScan_Dif  = new TH1F( "hScan_Dif", "hScan_Dif", 61, 800, 1100);
    hScan_Dif->GetXaxis()->SetTitle("Voltage (mV)");
    hScan_Dif->GetYaxis()->SetTitle("Rate (Hz)");

    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> map_off;
    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> map_on;
    std::vector<Float_t> voltages;
    Float_t current_voltage=-9999;

    for(int j=0; j!=tInputData->GetEntries(); ++j){
        tInputData->GetEvent(j);
        if (current_voltage != voltage) {
            current_voltage = voltage;
            voltages.push_back(current_voltage);
        }
        map_off[current_voltage].push_back({counts_off, period_off});
        map_on[current_voltage].push_back({counts_on, period_on});

        auto kRateOff = uCalculateRate(map_off[current_voltage]);
        auto kRateOn = uCalculateRate(map_on[current_voltage]);

        auto kCurrent_Bin = hScan_Off->GetXaxis()->FindBin(voltage);

        hScan_Off->SetBinContent(kCurrent_Bin, kRateOff.first);
        hScan_Off->SetBinError(kCurrent_Bin, kRateOff.second);
        hScan_On->SetBinContent(kCurrent_Bin, kRateOn.first);
        hScan_On->SetBinError(kCurrent_Bin, kRateOn.second);
        hScan_Dif->SetBinContent(kCurrent_Bin, kRateOn.first - kRateOff.first);
        hScan_Dif->SetBinError(kCurrent_Bin, sqrt(kRateOff.second*kRateOff.second + kRateOn.second*kRateOn.second));
    }

    TFile*  fOutputFile  =   new TFile( fFileName + TString(".out.root"), "RECREATE" );
    hScan_On->Write();
    hScan_Off->Write();
    hScan_Dif->Write();

    fOutputFile->Close();
    fInputFile->Close();
}