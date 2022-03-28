#include "listA1bias.h"
#include "listA1threshold.h"

void analyse_A1_vbias() {

    gStyle->SetOptStat(00000);
    
    const int N_Files_b = 30;
    const int N_Config_b = 6;
    
    auto fFileNamesb = listA1Bias();
    
    std::array<TFile*, N_Files_b> fInputFilesb;
    std::array<TH1F*, N_Files_b> hInputHistosb;
    
    for (int i = 0; i != fFileNamesb.size(); ++i){
        fInputFilesb[i] = new TFile (fFileNamesb[i]);
    }

    for (int i = 0; i != N_Files_b; ++i){
        hInputHistosb[i] = (TH1F*)(fInputFilesb[i]->Get("hScan_Dif"));
        hInputHistosb[i]->SetName(Form("h_%i",i));
    }
    
    for(int i = 0; i != N_Files_b; i += N_Config_b){
        for (int j = 0; j != N_Config_b; ++j) {
            if (i == 0) {
                hInputHistosb[i+j]->SetMarkerColor(2);
                hInputHistosb[i+j]->SetLineColor(2);
                hInputHistosb[i+j]->SetMarkerStyle(20);
            } else if (i == 6) {
                hInputHistosb[i+j]->SetMarkerColor(3);
                hInputHistosb[i+j]->SetLineColor(3);
                hInputHistosb[i+j]->SetMarkerStyle(21);
            } else if (i == 12) {
                hInputHistosb[i+j]->SetMarkerColor(4);
                hInputHistosb[i+j]->SetLineColor(4);
                hInputHistosb[i+j]->SetMarkerStyle(22);
            } else if (i == 18) {
                hInputHistosb[i+j]->SetMarkerColor(7);
                hInputHistosb[i+j]->SetLineColor(7);
                hInputHistosb[i+j]->SetMarkerStyle(29);
            } else {
                hInputHistosb[i+j]->SetMarkerColor(6);
                hInputHistosb[i+j]->SetLineColor(6);
                hInputHistosb[i+j]->SetMarkerStyle(33);
            }
        }
    }

    hInputHistosb[0]->SetTitle("pulser voltage : 960 mV, V threshold + 3");
    hInputHistosb[1]->SetTitle("pulser voltage : 980 mV, V threshold + 3");
    hInputHistosb[2]->SetTitle("pulser voltage : 1000 mV, V threshold + 3");
    hInputHistosb[3]->SetTitle("pulser voltage : 960 mV, V threshold + 5");
    hInputHistosb[4]->SetTitle("pulser voltage : 980 mV, V threshold + 5");
    hInputHistosb[5]->SetTitle("pulser voltage : 1000 mV, V threshold + 5");
    
    TCanvas *cA1_b= new TCanvas("cA1_b", "cA1_b", 2400, 2100);
    auto legend = new TLegend(0.1,0.1,0.9,0.3);
    legend->SetNColumns(5);
    legend->AddEntry(hInputHistosb[0], "25/02");
    legend->AddEntry(hInputHistosb[6], "26/02");
    legend->AddEntry(hInputHistosb[12], "27/02");
    legend->AddEntry(hInputHistosb[18], "01/03");
    legend->AddEntry(hInputHistosb[24], "03/03");
    
    cA1_b->Divide(3, 2);
    for(int i = 0; i != N_Config_b; ++i){
        cA1_b->cd(i+1);
        gPad->SetLogy();
        hInputHistosb[i]->Draw("P");
        for(int j = i; j < N_Files_b; j += N_Config_b){
            hInputHistosb[j]->Draw("SAME, P");
        }
        legend->Draw("SAME");
    }

    cA1_b->SaveAs("cA1_b.root");
    cA1_b->SaveAs("cA1_b.png");
    delete cA1_b;
    
    //istogrammi con x/media
    std::array<TH1F*, N_Files_b> hDivisionsb;
    char *histName=new char[10];

    for (int i = 0; i != N_Files_b; ++i){
        sprintf(histName,"h%d",i);
        hDivisionsb[i] = (TH1F*)(hInputHistosb[i]->Clone(histName));
    }
    

    for (int j = 0; j != N_Config_b; ++j) {
        TH1F *hMean = (TH1F*)(hInputHistosb[j]->Clone("hMean"));
        //hMean->Sumw2();
        for (int l = 0; l != hInputHistosb[j]->GetNbinsX(); ++l){
            float sum = 0.;
            float sqerr = 0.;
            int reps = 0.;
            for (int i = j; i < N_Files_b; i += N_Config_b){
                sum += hInputHistosb[i]->GetBinContent(l);
                sqerr += pow(hInputHistosb[i]->GetBinError(l), 2);
                if (hInputHistosb[i]->GetBinContent(l) != 0) {
                    ++reps;
                }
            }
            if (reps != 0){
                hMean->SetBinContent(l, sum/(float)reps);
                hMean->SetBinError(l, (sqrt(sqerr))/(float)reps);
            }
        }
        for (int i = j; i < N_Files_b; i += N_Config_b){
            hDivisionsb[i] -> Divide(hInputHistosb[i], hMean);
        }
    }

    TCanvas *cDivisions_b = new TCanvas("cDivisions_b", "cDivision_b", 2400, 2100);

    auto divlegendb = new TLegend(0.1,0.1,0.9,0.3);
    divlegendb->SetNColumns(5);
    divlegendb->AddEntry(hDivisionsb[0],  "25_02/mean");
    divlegendb->AddEntry(hDivisionsb[6], "26_02/mean");
    divlegendb->AddEntry(hDivisionsb[12], "27_02/mean");
    divlegendb->AddEntry(hDivisionsb[18],  "01_03/mean");
    divlegendb->AddEntry(hDivisionsb[24], "03_03/mean");
    
    cDivisions_b->Divide(3, 2);
    for(int i = 0; i != N_Config_b; ++i){
        cDivisions_b->cd(i+1);
        gPad->SetGridy();
        hDivisionsb[i]->GetYaxis()->SetRangeUser(0, 2);
        hDivisionsb[i]->GetYaxis()->SetTitle("(Rate on - off) / (Rate on - off (mean))");
        for(int j = i; j < N_Files_b; j += N_Config_b){
            hDivisionsb[j]->Draw("SAME, P");
        }
        divlegendb->Draw("SAME");
    }

    cDivisions_b->SaveAs("cDivisions_b.root");
    cDivisions_b->SaveAs("cDivisions_b.png");
    delete cDivisions_b;
    
    //istogrammi musura/media -1 cumulativi per punto
    std::array<std::vector<TH1F*>, N_Config_b> hDivsConfig;
    char *DivVName=new char[10];
    
    for (int kCurrentConfig = 0; kCurrentConfig != N_Config_b; ++kCurrentConfig){
        for (int i = 0; i != hDivisionsb[0]->GetNbinsX(); ++i){
            if (hDivisionsb[0]->GetBinContent(i) != 0 && hDivisionsb[0]->GetBinCenter(i) > 49.9){
                auto current_v = hDivisionsb[0]->GetBinCenter(i);
                sprintf(DivVName, "divV%.2f_%d", current_v, kCurrentConfig);
                TH1F* kHisto = new TH1F(DivVName, DivVName, 101, -0.505, 0.505);
                for (int j = kCurrentConfig; j < N_Files_b; j += N_Config_b){
                    kHisto->Fill(hDivisionsb[j]->GetBinContent(i) - 1);
                }
                hDivsConfig[kCurrentConfig].push_back((TH1F*)(kHisto->Clone(DivVName)));
                delete kHisto;
            }
        }
    }
    
    //istogrammi con errori di misura/media, cumulativi per punto
    std::array<std::vector<TH1F*>, N_Config_b> hDivsConfigErr;
    char *DivVerrName=new char[15];
    for (int kCurrentConfig = 0; kCurrentConfig != N_Config_b; ++kCurrentConfig){
        for (int i = 0; i != hDivisionsb[0]->GetNbinsX(); ++i){
            if (hDivisionsb[0]->GetBinContent(i) != 0 && hDivisionsb[0]->GetBinCenter(i) > 49.9 ){
                auto current_v = hDivisionsb[0]->GetBinCenter(i);
                sprintf(DivVerrName, "divV_err%.2f_%d", current_v, kCurrentConfig);
                TH1F* kHisto = new TH1F(DivVerrName, DivVerrName, 101, 0, 0.04);
                for (int j = kCurrentConfig; j < N_Files_b; j += N_Config_b){
                        kHisto->Fill(hDivisionsb[j]->GetBinError(i));
                }
                hDivsConfigErr[kCurrentConfig].push_back((TH1F*)(kHisto->Clone(DivVerrName)));
                delete kHisto;
            }
        }
    }
    //istogrammi con x/media, tutte le misure
    std::array<TH1F*, N_Config_b> hDiffsb;
    char *diffName=new char[10];
    
    for (int i = 0; i != N_Config_b; ++i){
        sprintf(diffName, "hDiff%d", i);
        hDiffsb[i] = new TH1F(diffName, diffName, 101, -0.505, 0.505);
        for (int j = i; j < N_Files_b; j += N_Config_b){
            for (int bin = 0; bin != (hDivisionsb[j]->GetNbinsX()); ++bin){
                if ((hDivisionsb[j]->GetBinCenter(bin)) > 49.9 && (hDivisionsb[j]->GetBinContent(bin) != 0)){
                    hDiffsb[i]->Fill(hDivisionsb[j]->GetBinContent(bin) - 1);
                }
            }
        }
    }
    
    //istogrammi con errore x/media, tutte le misure
    std::array<TH1F*, N_Config_b> hDiffs_err_b;
    char *diff_err_Name=new char[10];
    
    for (int i = 0; i != N_Config_b; ++i){
        sprintf(diff_err_Name, "hDiff_err%d", i);
        hDiffs_err_b[i] = new TH1F(diff_err_Name, diff_err_Name, 151, 0., 0.04);
        for (int j = i; j < N_Files_b; j += N_Config_b){
            for (int bin = 0; bin != (hDivisionsb[j]->GetNbinsX()); ++bin){
                if ((hDivisionsb[j]->GetBinCenter(bin)) > 49.9 && (hDivisionsb[j]->GetBinContent(bin) != 0)){
                    hDiffs_err_b[i]->Fill(hDivisionsb[j]->GetBinError(bin));
                }
            }
        }
    }
    
    TCanvas *ca1tot = new TCanvas("ca1tot", "ca1tot", 900, 900);
    
    hInputHistosb[2]->GetXaxis()->SetRangeUser(49.5, 56.5);
    for(int j = 2; j < N_Files_b; j += N_Config_b){
        hInputHistosb[j]->Draw("SAME, P");
    }
    legend->Draw("SAME");
    
    ca1tot->SaveAs("cA1tot.png");
    delete ca1tot;
    
    TCanvas *ca1div = new TCanvas("ca1div", "cadiv", 900, 900);
    
        
    hDivisionsb[2]->GetXaxis()->SetRangeUser(49.5, 56.5);
    hDivisionsb[2]->GetYaxis()->SetRangeUser(0.8, 1.2);
    for(int j = 2; j < N_Files_b; j += N_Config_b){
        hDivisionsb[j]->Draw("SAME, P");
    }
    divlegendb->Draw("SAME");
    
    ca1div->SaveAs("cA1div.png");
    delete ca1div;
    
    
    //scrittura su file output
    TFile *fOutputFile = new TFile ("A1_bias.root", "RECREATE");
    for (auto kCurrentHisto : hDiffsb){
        kCurrentHisto->Write();
    } for (auto kCurrentErrHisto : hDiffs_err_b){
        kCurrentErrHisto->Write();
    }
    for (int i = 0; i != N_Config_b; ++i){
        for (auto kCurrentHisto : hDivsConfig[i]){
            kCurrentHisto->Write();
        }
    }
    for (int i = 0; i != N_Config_b; ++i){
        for (auto kCurrentHisto : hDivsConfigErr[i]){
            kCurrentHisto->Write();
        }
    }
    fOutputFile->Close();
    
    for (auto kCurrentFile : fInputFilesb){
        kCurrentFile->Close();
    }
}

void analyse_A1_vthreshold(){
    
    const int N_Files_t = 24;
    const int N_Config_t = 6;
    
    auto fFileNamest = listA1Threshold();
    
    std::array<TFile*, N_Files_t> fInputFilest;
    std::array<TH1F*, N_Files_t> hInputHistost;
    
    for (int i = 0; i != fFileNamest.size(); ++i){
        fInputFilest[i] = new TFile (fFileNamest[i]);
    }

    for (int i = 0; i != N_Files_t; ++i){
        hInputHistost[i] = (TH1F*)(fInputFilest[i]->Get("hScan_Dif"));
        hInputHistost[i]->SetName(Form("h_%i",i));
    }
    
    for(int i = 0; i != N_Files_t; i += N_Config_t){
        for (int j = 0; j != N_Config_t; ++j) {
            if (i == 0) {
                hInputHistost[i+j]->SetMarkerColor(2);
                hInputHistost[i+j]->SetLineColor(2);
                hInputHistost[i+j]->SetMarkerStyle(20);
            } else if (i == 6) {
                hInputHistost[i+j]->SetMarkerColor(3);
                hInputHistost[i+j]->SetLineColor(3);
                hInputHistost[i+j]->SetMarkerStyle(21);
            } else if (i == 12) {
                hInputHistost[i+j]->SetMarkerColor(4);
                hInputHistost[i+j]->SetLineColor(4);
                hInputHistost[i+j]->SetMarkerStyle(22);
            } else {
                hInputHistost[i+j]->SetMarkerColor(7);
                hInputHistost[i+j]->SetLineColor(7);
                hInputHistost[i+j]->SetMarkerStyle(33);
            }
        }
    }

    hInputHistost[0]->SetTitle("pulser voltage : 960 mV, V bias : 51.3 V");
    hInputHistost[1]->SetTitle("pulser voltage : 980 mV, V bias : 51.3 V");
    hInputHistost[2]->SetTitle("pulser voltage : 1000 mV, V bias : 51.3 V");
    hInputHistost[3]->SetTitle("pulser voltage : 960 mV, V bias : 53.3 V");
    hInputHistost[4]->SetTitle("pulser voltage : 980 mV, V bias : 53.3 V");
    hInputHistost[5]->SetTitle("pulser voltage : 1000 mV, V bias : 53.3 V");
    
    TCanvas *cA1_t= new TCanvas("cA1_t", "cA1_t", 2400, 2100);
    auto legend_t = new TLegend();
    legend_t->AddEntry(hInputHistost[0], "25/02");
    legend_t->AddEntry(hInputHistost[6], "26/02");
    legend_t->AddEntry(hInputHistost[12], "27/02");
    legend_t->AddEntry(hInputHistost[18], "01/03");
    
    cA1_t->Divide(3, 2);
    for(int i = 0; i != N_Config_t; ++i){
        cA1_t->cd(i+1);
        gPad->SetLogy();
        hInputHistost[i]->Draw("P");
        for(int j = i; j < N_Files_t; j += N_Config_t){
            hInputHistost[j]->Draw("SAME, P");
        }
        legend_t->Draw("SAME");
    }

    cA1_t->SaveAs("cA1_t.root");
    cA1_t->SaveAs("cA1_t.png");
    delete cA1_t;
    
    for (auto kCurrentFile : fInputFilest){
        kCurrentFile->Close();
    }
}
