//
// Created by Giulia Fazzino on 18/02/2022.
//

#ifndef HISTOGRAMS_H
#define HISTOGRAMS_H

#include "AnalysisUtility.h"

void fillHist
(TH1F* h_On, TH1F* h_Off, TH1F* h_Dif, std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> map_on,
 std::map<Float_t, std::vector <std::pair<Float_t, Float_t>>> map_off, std::vector<Float_t> positions)
{
    for(auto kCurrentPos : positions) {
        auto kRateOff = uCalculateRate(map_off[kCurrentPos]);
        auto kRateOn = uCalculateRate(map_on[kCurrentPos]);

        auto kCurrent_Bin = h_Off->GetXaxis()->FindBin(kCurrentPos);

        h_Off->SetBinContent(kCurrent_Bin, kRateOff.first);
        h_Off->SetBinError(kCurrent_Bin, kRateOff.second);
        h_On->SetBinContent(kCurrent_Bin, kRateOn.first);
        h_On->SetBinError(kCurrent_Bin, kRateOn.second);
        h_Dif->SetBinContent(kCurrent_Bin, kRateOn.first - kRateOff.first);
        h_Dif->SetBinError(kCurrent_Bin, sqrt(kRateOff.second * kRateOff.second + kRateOn.second * kRateOn.second));
    }
}


#endif
