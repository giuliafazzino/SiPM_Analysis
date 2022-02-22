#include "AnalysisUtility.h"

void stability(TString fFileName = ""){

   if (fFileName == ""){
       cout <<"NO FILE NAME" <<endl;
   }

   TFile *fInputFile = new TFile (fFileName);
   TTree* tInputData = (TTree*)(fInputFile->Get("tree"));

   Float_t b_vol, temp_on, temp_off, period_on, period_off;
   Int_t  p_vol, tstamp_on, tstamp_off, counts_on, counts_off;
   tInputData->SetBranchAddress("bias_voltage", &b_vol);
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
   TGraphErrors *gTempOff = new TGraphErrors();
   gTempOff->GetXaxis()->SetTitle("Temperature oC");
   gTempOff->GetYaxis()->SetTitle("Rate off / 100kHz");
   TGraphErrors *gTempDif = new TGraphErrors();
   gTempDif->GetXaxis()->SetTitle("Temperature oC");
   gTempDif->GetYaxis()->SetTitle("Rate on - off / 100kHz");

    TGraphErrors *gTStampOn = new TGraphErrors();
    gTStampOn->GetXaxis()->SetTitle("Timestamp");
    gTStampOn->GetYaxis()->SetTitle("Rate on / 100kHz");
    TGraphErrors *gTStampOff = new TGraphErrors();
    gTStampOff->GetXaxis()->SetTitle("Timestamp");
    gTStampOff->GetYaxis()->SetTitle("Rate off / 100kHz");
    TGraphErrors *gTStampDif = new TGraphErrors();
    gTStampDif->GetXaxis()->SetTitle("Timestamp");
    gTStampDif->GetYaxis()->SetTitle("Rate on - off / 100kHz");

    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> tmap_on;
    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> tmap_off;
    std::vector<Float_t> temps;
    bool is_same;
    for(int j=0; j!=tInputData->GetEntries(); ++j) {
        tInputData->GetEvent(j);
        is_same = false;
        for(int i=0; i!=temps.size(); ++i) {
            if (temp_on == temps[i]){
                is_same = true;
            }
        }
        if (!is_same) {
            temps.push_back(temp_on);
        }
        tmap_on[temp_on].push_back({(float) counts_on, period_on});
        tmap_off[temp_on].push_back({(float) counts_off, period_off});
    }

    std::sort(temps.begin(), temps.end());
    for (auto kCurrentTemp : temps) {
        auto kRateOff = uCalculateRate(tmap_off[kCurrentTemp]);
        auto kRateOn = uCalculateRate(tmap_on[kCurrentTemp]);

        auto npt = gTempOn->GetN();
        gTempOn->SetPoint(npt, kCurrentTemp, kRateOn.first/1E5);
        gTempOn->SetPointError(npt, kCurrentTemp, kRateOn.second/1E5);
        gTempOff->SetPoint(npt, kCurrentTemp, kRateOff.first/1E5);
        gTempOff->SetPointError(npt, kCurrentTemp, kRateOff.second/1E5);
        gTempDif->SetPoint(npt, kCurrentTemp, kRateOn.first/1E5 - kRateOff.first/1E5);
        gTempDif->SetPointError(npt, kCurrentTemp,sqrt(kRateOff.second/1E5 * kRateOff.second/1E5 +
                                kRateOn.second/1E5 * kRateOn.second/1E5));
    }

    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> tsmap_on;
    std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> tsmap_off;
    std::vector<Float_t> tstamps;

    for(int j=0; j!=tInputData->GetEntries(); ++j) {
        tInputData->GetEvent(j);
        is_same = false;
        for(int i=0; i!=tstamps.size(); ++i) {
            if (tstamp_on == tstamps[i]){
                is_same = true;
            }
        }
        if (!is_same) {
            tstamps.push_back(tstamp_on);
        }
        tsmap_on[tstamp_on].push_back({(float) counts_on, period_on});
        tsmap_off[tstamp_on].push_back({(float) counts_off, period_off});
    }

    std::sort(tstamps.begin(), tstamps.end());
    for (auto kCurrentTime : tstamps) {
        auto kRateOff = uCalculateRate(tsmap_off[kCurrentTime]);
        auto kRateOn = uCalculateRate(tsmap_on[kCurrentTime]);

        auto npt = gTStampOn->GetN();
        gTStampOn->SetPoint(npt, kCurrentTime, kRateOn.first/1E5);
        gTStampOn->SetPointError(npt, kCurrentTime, kRateOn.second/1E5);
        gTStampOff->SetPoint(npt, kCurrentTime, kRateOff.first/1E5);
        gTStampOff->SetPointError(npt, kCurrentTime, kRateOff.second/1E5);
        gTStampDif->SetPoint(npt, kCurrentTime, kRateOn.first/1E5 - kRateOff.first/1E5);
        gTStampDif->SetPointError(npt, kCurrentTime,sqrt(kRateOff.second/1E5 * kRateOff.second/1E5 +
                                                       kRateOn.second/1E5 * kRateOn.second/1E5));
    }

    TFile* fOutputFile = new TFile(fFileName + TString(".out.root"), "RECREATE");
    gTempOn->Write();
    gTempOff->Write();
    gTempDif->Write();
    gTStampOn->Write();
    gTStampOff->Write();
    gTStampDif->Write();

    fOutputFile->Close();
    fInputFile->Close();
}