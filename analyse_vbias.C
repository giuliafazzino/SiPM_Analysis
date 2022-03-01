#include "listVBiasFiles.h"

//What's going on to A1?????


void analyse_vbias()
{
    gStyle->SetOptStat(00000);

    const int N_Files = 54;
    const int N_Config = 6;

    auto fFileNames = listVBiasFiles();

    std::array<TFile*, N_Files> fInputFiles;
    std::array<TH1F*, N_Files> hInputHistos;

    for(int i = 0; i != fFileNames.size(); ++i) {
        fInputFiles[i] = new TFile (fFileNames[i]);
    }

    for(int i = 0; i != N_Files; ++i){
        hInputHistos[i] = (TH1F*)(fInputFiles[i]->Get("hScan_Dif"));
        hInputHistos[i]->SetName( Form("h_%i",i) );
    }

    for(int i = 0; i != N_Files; i += N_Config){
        for (int j = 0; j != N_Config; ++j) {
            if (i == 0) {
                hInputHistos[i+j]->SetLineColor(2);
            } else if (i == 6) {
                hInputHistos[i+j]->SetLineColor(3);
            } else if (i == 12) {
                hInputHistos[i+j]->SetLineColor(4);
            } else if (i == 18) {
                hInputHistos[i+j]->SetLineColor(6);
            } else if (i == 24) {
                hInputHistos[i+j]->SetLineColor(7);
            } else if (i == 30) {
                hInputHistos[i+j]->SetLineColor(209);
            } else if (i == 36) {
                hInputHistos[i+j]->SetLineColor(45);
            } else if (i == 42) {
                hInputHistos[i+j]->SetLineColor(93);
            } else if (i == 48) {
                hInputHistos[i+j]->SetLineColor(219);
            }
        }
    }

    hInputHistos[0]->SetTitle("pulser voltage : 960 mV, V threshold + 3");
    hInputHistos[1]->SetTitle("pulser voltage : 980 mV, V threshold + 3");
    hInputHistos[2]->SetTitle("pulser voltage : 1000 mV, V threshold + 3");
    hInputHistos[3]->SetTitle("pulser voltage : 960 mV, V threshold + 5");
    hInputHistos[4]->SetTitle("pulser voltage : 980 mV, V threshold + 5");
    hInputHistos[5]->SetTitle("pulser voltage : 1000 mV, V threshold + 5");

    TCanvas *cBias = new TCanvas("cBias", "cBias", 2400, 2100);

    auto legend = new TLegend(0.5,0.1,0.9,0.5);
    legend->AddEntry(hInputHistos[0], "chip1-A1");
    legend->AddEntry(hInputHistos[6], "chip0-C1");
    legend->AddEntry(hInputHistos[12], "chip0-C2");
    legend->AddEntry(hInputHistos[18], "chip0-C3");
    legend->AddEntry(hInputHistos[24], "chip0-C4");
    legend->AddEntry(hInputHistos[30], "chip0-E1");
    legend->AddEntry(hInputHistos[36], "chip0-E2");
    legend->AddEntry(hInputHistos[42], "chip0-E3");
    legend->AddEntry(hInputHistos[48], "chip0-E4");

    cBias->Divide(3, 2);
    for(int i = 0; i != N_Config; ++i){
        cBias->cd(i+1);
        gPad->SetLogy();
        hInputHistos[i]->Draw();
        for(int j = i; j < N_Files; j += N_Config){
            hInputHistos[j]->Draw("SAME");
        }
        legend->Draw("SAME");
    }

    cBias->SaveAs("cBias.root");
    cBias->SaveAs("cBias.png");
    delete cBias;

    std::array<TH1F*, N_Files> hDivisions;
    char *histName=new char[10];

    for (int i = 0; i != N_Files; ++i){
        sprintf(histName,"h%d",i);
        hDivisions[i] = ( TH1F* )( hInputHistos[i]->Clone(histName) );
    }

    for (int j = 0; j < N_Config; ++j) {
        for (int i = j; i < N_Files; i += N_Config) {
            hDivisions[i]->Divide(hInputHistos[i], hInputHistos[j]);
            cout <<"i : " <<i <<" /j: " <<j <<endl;
        }
    }

    TCanvas *cDivisions = new TCanvas("cDivisions", "cDivisions", 2400, 2100);

    auto divlegend = new TLegend(0.7,0.1,0.9,0.5);
    divlegend->AddEntry(hDivisions[0], "chip1-A1/chip1-A1");
    divlegend->AddEntry(hDivisions[6], "chip0-C1/chip1-A1");
    divlegend->AddEntry(hDivisions[12], "chip0-C2/chip1-A1");
    divlegend->AddEntry(hDivisions[18], "chip0-C3/chip1-A1");
    divlegend->AddEntry(hDivisions[24], "chip0-C4/chip1-A1");
    divlegend->AddEntry(hDivisions[30], "chip0-E1/chip1-A1");
    divlegend->AddEntry(hDivisions[36], "chip0-E2/chip1-A1");
    divlegend->AddEntry(hDivisions[42], "chip0-E3/chip1-A1");
    divlegend->AddEntry(hDivisions[48], "chip0-E4/chip1-A1");


    cDivisions->Divide(3, 2);
    for(int i = 0; i != N_Config; ++i){
        cDivisions->cd(i+1);
        hDivisions[i]->GetYaxis()->SetRangeUser(0, 2);
        hDivisions[i]->GetYaxis()->SetTitle("Rate on - off / (Rate on - off (A1))");
        for(int j = i; j < N_Files; j += N_Config){
            hDivisions[j]->Draw("SAME");
        }
        divlegend->Draw("SAME");
    }

    cDivisions->SaveAs("cDivisions.root");
    delete cDivisions;

    for (auto kCurrentFile : fInputFiles){
        kCurrentFile->Close();
    }
}

