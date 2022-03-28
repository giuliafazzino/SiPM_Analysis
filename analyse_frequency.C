#include "listFrequencyFiles.h"


int const N_Config = 12;
int const N_Files = 144;
int const N_Sens = N_Files/N_Config;

void analyse_frequency(){
        
    gStyle->SetOptStat(0000);
    
    std::vector <TString> iFileNames = listFrequencyFiles();
    std::array <TFile*, N_Files> iFiles;
    
    for (int i = 0; i != iFileNames.size(); ++i){
        iFiles[i] = new TFile (iFileNames[i]);
    }
    
    std::array <TH1F*, N_Files> iHistos;
    for (int i = 0; i != N_Files; ++i){
        iHistos[i] = (TH1F*)(iFiles[i]->Get("hScan_Dif"));
        switch ( i/N_Config ){
            case 0:
                iHistos[i]->SetMarkerStyle(20);
                iHistos[i]->SetMarkerColor(2);
                iHistos[i]->SetLineColor(2);
                break;
            case 1:
                iHistos[i]->SetMarkerStyle(21);
                iHistos[i]->SetMarkerColor(3);
                iHistos[i]->SetLineColor(3);
                break;
            case 2:
                iHistos[i]->SetMarkerStyle(39);
                iHistos[i]->SetMarkerColor(95);
                iHistos[i]->SetLineColor(95);
                break;
            case 3:
                iHistos[i]->SetMarkerStyle(22);
                iHistos[i]->SetMarkerColor(4);
                iHistos[i]->SetLineColor(4);
                break;
            case 4:
                iHistos[i]->SetMarkerStyle(23);
                iHistos[i]->SetMarkerColor(6);
                iHistos[i]->SetLineColor(6);
                break;
            case 5:
                iHistos[i]->SetMarkerStyle(29);
                iHistos[i]->SetMarkerColor(7);
                iHistos[i]->SetLineColor(7);
                break;
            case 6:
                iHistos[i]->SetMarkerStyle(33);
                iHistos[i]->SetMarkerColor(8);
                iHistos[i]->SetLineColor(8);
                break;
            case 7:
                iHistos[i]->SetMarkerStyle(34);
                iHistos[i]->SetMarkerColor(9);
                iHistos[i]->SetLineColor(9);
                break;
            case 8:
                iHistos[i]->SetMarkerStyle(43);
                iHistos[i]->SetMarkerColor(28);
                iHistos[i]->SetLineColor(28);
                break;
            case 9:
                iHistos[i]->SetMarkerStyle(45);
                iHistos[i]->SetMarkerColor(221);
                iHistos[i]->SetLineColor(221);
                break;
            case 10:
                iHistos[i]->SetMarkerStyle(47);
                iHistos[i]->SetMarkerColor(30);
                iHistos[i]->SetLineColor(30);
                break;
            case 11:
                iHistos[i]->SetMarkerStyle(41);
                iHistos[i]->SetMarkerColor(40);
                iHistos[i]->SetLineColor(40);
                break;
        }
        iHistos[i]->GetYaxis()->SetRangeUser(-0.05, 0.12);
    }
    
    TLegend  *legend = new TLegend (0.1, 0.1, 0.9, 0.3);
    
    legend->SetNColumns(6);
    
    legend->AddEntry(iHistos[0], "chip0-C1");
    legend->AddEntry(iHistos[24], "chip0-C3");
    legend->AddEntry(iHistos[48], "chip0-E1");
    legend->AddEntry(iHistos[72], "chip0-E3");
    legend->AddEntry(iHistos[96], "chip0-G1");
    legend->AddEntry(iHistos[120], "chip0-G3");
    legend->AddEntry(iHistos[12], "chip0-C2");
    legend->AddEntry(iHistos[36], "chip0-C4");
    legend->AddEntry(iHistos[60], "chip0-E2");
    legend->AddEntry(iHistos[84], "chip0-E4");
    legend->AddEntry(iHistos[108], "chip0-G2");
    legend->AddEntry(iHistos[132], "chip0-G4");
    
    iHistos[0]->SetTitle("p_vol = 960 mV, V_bias = 51.3 mV, V threshold + 3");
    iHistos[1]->SetTitle("p_vol = 980 mV, V_bias = 51.3 mV, V threshold + 3");
    iHistos[2]->SetTitle("p_vol = 1000 mV, V_bias = 51.3 mV, V threshold + 3");
    iHistos[3]->SetTitle("p_vol = 960 mV, V_bias = 51.3 mV, V threshold + 5");
    iHistos[4]->SetTitle("p_vol = 980 mV, V_bias = 51.3 mV, V threshold + 5");
    iHistos[5]->SetTitle("p_vol = 1000 mV, V_bias = 51.3 mV, V threshold + 5");
    iHistos[6]->SetTitle("p_vol = 960 mV, V_bias = 53.3 mV, V threshold + 3");
    iHistos[7]->SetTitle("p_vol = 980 mV, V_bias = 53.3 mV, V threshold + 3");
    iHistos[8]->SetTitle("p_vol = 1000 mV, V_bias = 53.3 mV, V threshold + 3");
    iHistos[9]->SetTitle("p_vol = 960 mV, V_bias = 53.3 mV, V threshold + 5");
    iHistos[10]->SetTitle("p_vol = 980 mV, V_bias = 53.3 mV, V threshold + 5");
    iHistos[11]->SetTitle("p_vol = 1000 mV, V_bias = 53.3 mV, V threshold + 5");
    
    TCanvas *cAll = new TCanvas("cAll", "cAll", 1500, 2000);
    cAll->Divide(3, 4);
    for (int i = 0; i != N_Config; ++i){
        cAll->cd(i+1);
        gPad->SetGridy();
        for (int j = i; j < N_Files; j += N_Config){
            iHistos[j]->Draw("SAME");
        }
        legend->Draw("SAME");
    }
    
    cAll->SaveAs("cAll.root");
    cAll->SaveAs("cAll.png");
    
    delete cAll;
    
    for (auto kCurrentFile : iFiles){
        kCurrentFile->Close();
    }
}
