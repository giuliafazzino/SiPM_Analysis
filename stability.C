#include "AnalysisUtility.h"

void stability(Int_t pvol, Int_t dac, TString fFileName = ""){
    gStyle->SetOptStat(0);
    if (fFileName == ""){
        cout <<"NO FILE NAME" <<endl;
    }

    TFile *fInputFile = new TFile (fFileName);
    TTree* tInputData = (TTree*)(fInputFile->Get("tree"));

    Float_t b_vol, temp_on, temp_off, period_on, period_off;
    Int_t b_dac, p_vol, tstamp_on, tstamp_off, counts_on, counts_off;
    tInputData->SetBranchAddress("bias_voltage", &b_vol);
    tInputData->SetBranchAddress("bias_dac", &b_dac);
    tInputData->SetBranchAddress("pulse_voltage", &p_vol);
    tInputData->SetBranchAddress("temperature_on", &temp_on);
    tInputData->SetBranchAddress("temperature_off", &temp_off);
    tInputData->SetBranchAddress("timestamp_on", &tstamp_on);
    tInputData->SetBranchAddress("timestamp_off", &tstamp_off);
    tInputData->SetBranchAddress("counts_on", &counts_on);
    tInputData->SetBranchAddress("counts_off", &counts_off);
    tInputData->SetBranchAddress("period_on", &period_on);
    tInputData->SetBranchAddress("period_off", &period_off);

    TGraphErrors *gTempOn = new TGraphErrors();
    gTempOn->GetXaxis()->SetTitle("Temperature oC");
    gTempOn->GetYaxis()->SetTitle("Rate on / 100kHz");
    gTempOn->SetName("gTempOn");

    TGraphErrors *gTempOff = new TGraphErrors();
    gTempOff->GetXaxis()->SetTitle("Temperature oC");
    gTempOff->GetYaxis()->SetTitle("Rate off / 100kHz");
    gTempOff->SetName("gTempOff");

    TGraphErrors *gTempDif = new TGraphErrors();
    gTempDif->GetXaxis()->SetTitle("Temperature oC");
    gTempDif->GetYaxis()->SetTitle("Rate on - off / 100kHz");
    gTempDif->SetName("gTempDif");

    TGraphErrors *gTempRatioOn = new TGraphErrors();
    gTempRatioOn->GetXaxis()->SetTitle("Temperature oC");
    gTempRatioOn->GetYaxis()->SetTitle("Rate on / off");
    gTempRatioOn->SetName("gTempRatioOn");

    TGraphErrors *gTempRatioDif = new TGraphErrors();
    gTempRatioDif->GetXaxis()->SetTitle("Temperature oC");
    gTempRatioDif->GetYaxis()->SetTitle("Rate on - off / off");
    gTempRatioDif->SetName("gTempRatioDif");

    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> tmap_on;
    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> tmap_off;
    std::vector<Float_t> temps;
    bool is_same;
    for(int j=0; j!=tInputData->GetEntries(); ++j) {
        tInputData->GetEvent(j);
        if(p_vol == pvol && b_dac==dac) {
            is_same = false;
            for (int i = 0; i != temps.size(); ++i) {
                if (temp_on == temps[i]) {
                    is_same = true;
                }
            }
            if (!is_same) {
                temps.push_back(temp_on);
            }
            tmap_on[temp_on].push_back({(float) counts_on, period_on});
            tmap_off[temp_on].push_back({(float) counts_off, period_off});
        }
    }

    std::sort(temps.begin(), temps.end());
    for (auto kCurrentTemp : temps) {
        auto kRateOff = uCalculateRate(tmap_off[kCurrentTemp]);
        auto kRateOn = uCalculateRate(tmap_on[kCurrentTemp]);

        auto npt = gTempOn->GetN();
        gTempOn->SetPoint(npt, kCurrentTemp, kRateOn.first/1E5);
        gTempOn->SetPointError(npt, 0, kRateOn.second/1E5);
        gTempOff->SetPoint(npt, kCurrentTemp, kRateOff.first/1E5);
        gTempOff->SetPointError(npt, 0, kRateOff.second/1E5);
        gTempDif->SetPoint(npt, kCurrentTemp, kRateOn.first/1E5 - kRateOff.first/1E5);
        gTempDif->SetPointError(npt, 0,sqrt(kRateOff.second/1E5 * kRateOff.second/1E5 +
                                            kRateOn.second/1E5 * kRateOn.second/1E5));
        gTempRatioOn->SetPoint(npt, kCurrentTemp, kRateOn.first/kRateOff.first);
        gTempRatioOn->SetPointError(npt, 0, sqrt(pow(kRateOn.second/kRateOff.first,2)
                                                 + pow(kRateOff.second*kRateOn.first/(kRateOff.first*kRateOff.first),2)));
        gTempRatioDif->SetPoint(npt, kCurrentTemp, (kRateOn.first-kRateOff.first)/kRateOff.first);
        gTempRatioDif->SetPointError(npt, 0, sqrt(pow(kRateOn.second/kRateOff.first,2)
                                                  + pow(kRateOff.second*kRateOn.first/(kRateOff.first*kRateOff.first),2)));
    }


    TFile* fOutputFile = new TFile(fFileName + "_p_vol_" + pvol + "_dac_" + dac + TString(".out.root"), "RECREATE");
    gTempOn->Write();
    gTempOff->Write();
    gTempDif->Write();
    gTempRatioOn->Write();
    gTempRatioDif->Write();

    fOutputFile->Close();
    fInputFile->Close();
}
