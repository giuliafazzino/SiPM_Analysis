#ifndef ANALYSIS_UTILITY_H
#define ANALYSIS_UTILITY_H

#include <iostream>
#include <numeric>
#include <string_view>
#include <vector>

std::pair<Float_t,Float_t>
calculatemean(std::vector<std::pair<Float_t,Float_t>> fInputCountsPeriods) {
    std::pair<Float_t,Float_t> fResult;
    Float_t counts=0.;
    Float_t period=0.;
    for (auto kCurrentPair : fInputCountsPeriods) {
        counts+=kCurrentPair.first;
        period+=kCurrentPair.second;
    }
    fResult.first=counts/period;
    fResult.second=sqrt(counts)/period;
    return fResult;
}

Float_t
calculaterms(std::vector<std::pair<Float_t,Float_t>> fInputCountsPeriods, Float_t mean = -999) {
    Float_t rate= mean == -999 ? calculatemean(fInputCountsPeriods).first : mean;
    Float_t rms=0.;
    Int_t nSkips = 0;
    for (auto kCurrentPair : fInputCountsPeriods) {
        if ( kCurrentPair.second == 0 ) { nSkips++; continue; }
        rms+=pow(((kCurrentPair.first/kCurrentPair.second)-rate),2);
    }
    rms /=((float)fInputCountsPeriods.size()-nSkips);
    rms = sqrt(rms);
    return rms;
}

std::pair<Float_t,Float_t>
uCalculateRate
(std::vector<std::pair<Float_t,Float_t>> fInputCountsPeriods) {
    std::pair<Float_t,Float_t> fResult;

    Float_t rate = calculatemean(fInputCountsPeriods).first;
    Float_t rms = calculaterms(fInputCountsPeriods, rate);

    for(auto it=fInputCountsPeriods.begin(); it!=fInputCountsPeriods.end(); ){
        if ( (*it).second == 0 ) {
            it = fInputCountsPeriods.erase(it);
        } else if(TMath::Abs((((*it).first)/((*it).second)-rate))>3*rms){
            it=fInputCountsPeriods.erase(it);
        } else {
            ++it;
        }
    }
    return calculatemean(fInputCountsPeriods);
}

TGraph* invert(TGraph* gTarget){
    auto gResult = (TGraph*)(gTarget->Clone());
    for (int i = 0; i != gTarget->GetN(); ++i){
        auto xVal = -1 * gTarget->GetPointX(i);
        auto yVal = -1 * gTarget->GetPointY(i);
        gResult->SetPoint(i, xVal, yVal);
    }
    return gResult;
}

#endif
