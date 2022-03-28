#include "listThresholdFiles.h"

void analyse_threshold()
{
    gStyle->SetOptStat(00000);

    const int N_Files = 72;
    const int N_Config = 6;

    auto fFileNames = listThresholdFiles();

    std::array<TFile*, N_Files> fInputFiles;
    std::array<TH1F*, N_Files> hInputHistos;

    for(int i = 0; i != fFileNames.size(); ++i) {
        fInputFiles[i] = new TFile (fFileNames[i]);
    }

    for(int i = 0; i != N_Files; ++i){
        hInputHistos[i] = (TH1F*)(fInputFiles[i]->Get("hScan_Dif"));
        hInputHistos[i]->SetName(Form("h_%i",i));
    }

//Secondo te non si capisce, chiedi
    
    for (int i = 0; i != N_Files; i += 4 * N_Config){
        for (int j = 0; j != 4 * N_Config; ++j){
            hInputHistos[i+j]->SetMarkerStyle(20 + (j / N_Config));
            hInputHistos[i+j]->SetMarkerColor(2 + (i / (4 * N_Config)));
        }
    }
    

    hInputHistos[0]->SetTitle("pulser voltage : 960 mV, V bias : 51.3 V");
    hInputHistos[1]->SetTitle("pulser voltage : 980 mV, V bias : 51.3 V");
    hInputHistos[2]->SetTitle("pulser voltage : 1000 mV, V bias : 51.3 V");
    hInputHistos[3]->SetTitle("pulser voltage : 960 mV, V bias : 53.3 V");
    hInputHistos[4]->SetTitle("pulser voltage : 980 mV, V bias : 53.3 V");
    hInputHistos[5]->SetTitle("pulser voltage : 1000 mV, V bias : 53.3 V");

    TCanvas *cThreshold = new TCanvas("cThreshold", "cThreshold", 2400, 2100);

    auto legend = new TLegend();
    
    legend->SetNColumns(3);
    
    legend->AddEntry(hInputHistos[0],  "chip0-C1");
    legend->AddEntry(hInputHistos[24], "chip0-E1");
    legend->AddEntry(hInputHistos[48], "chip0-G1");
    legend->AddEntry(hInputHistos[6],  "chip0-C2");
    legend->AddEntry(hInputHistos[30], "chip0-E2");
    legend->AddEntry(hInputHistos[54], "chip0-G2");
    legend->AddEntry(hInputHistos[12], "chip0-C3");
    legend->AddEntry(hInputHistos[36], "chip0-E3");
    legend->AddEntry(hInputHistos[60], "chip0-G3");
    legend->AddEntry(hInputHistos[18], "chip0-C4");
    legend->AddEntry(hInputHistos[42], "chip0-E4");
    legend->AddEntry(hInputHistos[66], "chip0-G4");

    cThreshold->Divide(3, 2);
    for(int i = 0; i != N_Config; ++i){
        cThreshold->cd(i+1);
        gPad->SetLogy();
        hInputHistos[i]->Draw();
        for(int j = i; j < N_Files; j += N_Config){
            hInputHistos[j]->Draw("SAME");
        }
        legend->Draw("SAME");
    }

    cThreshold->SaveAs("cThreshold.root");
    cThreshold->SaveAs("cThreshold.png");
    delete cThreshold;
    
    for (auto kCurrentFile : fInputFiles){
        kCurrentFile->Close();
    }
}
