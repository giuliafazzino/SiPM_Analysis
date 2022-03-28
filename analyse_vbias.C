#include "listVBiasFiles.h"

void analyse_vbias()
{
    gStyle->SetOptStat(0000);
    const int N_Files = 72;
    const int N_Config = 6;

    auto fFileNames = listVBiasFiles();

    std::array<TFile*, N_Files> fInputFiles;
    std::array<TH1F*, N_Files> hInputHistos;

    for(int i = 0; i != fFileNames.size(); ++i) {
        fInputFiles[i] = new TFile (fFileNames[i]);
    }

    for(int i = 0; i != N_Files; ++i){
        hInputHistos[i] = (TH1F*)(fInputFiles[i]->Get("hScan_Dif"));
        hInputHistos[i]->SetName(Form("h_%i",i));
        hInputHistos[i]->SetMarkerStyle(kFullCircle);
    }
    
//colori
    for (int i = 0; i != N_Files; ++i){
        switch(i/N_Config){
            case 0:
                hInputHistos[i]->SetMarkerColor(2);
                hInputHistos[i]->SetLineColor(2);
                break;
            case 1:
                hInputHistos[i]->SetMarkerColor(3);
                hInputHistos[i]->SetLineColor(3);
                break;
            case 2:
                hInputHistos[i]->SetMarkerColor(4);
                hInputHistos[i]->SetLineColor(4);
                break;
            case 3:
                hInputHistos[i]->SetMarkerColor(6);
                hInputHistos[i]->SetLineColor(6);
                break;
            case 4:
                hInputHistos[i]->SetMarkerColor(7);
                hInputHistos[i]->SetLineColor(7);
                break;
            case 5:
                hInputHistos[i]->SetMarkerColor(8);
                hInputHistos[i]->SetLineColor(8);
                break;
            case 6:
                hInputHistos[i]->SetMarkerColor(90);
                hInputHistos[i]->SetLineColor(90);
                break;
            case 7:
                hInputHistos[i]->SetMarkerColor(40);
                hInputHistos[i]->SetLineColor(40);
                break;
            case 8:
                hInputHistos[i]->SetMarkerColor(28);
                hInputHistos[i]->SetLineColor(28);
                break;
            case 9:
                hInputHistos[i]->SetMarkerColor(205);
                hInputHistos[i]->SetLineColor(205);
                break;
            case 10:
                hInputHistos[i]->SetMarkerColor(221);
                hInputHistos[i]->SetLineColor(221);
                break;
            case 11:
                hInputHistos[i]->SetMarkerColor(1);
                hInputHistos[i]->SetLineColor(1);
                break;
        }
    }

    hInputHistos[0]->SetTitle("pulser voltage : 960 mV, V threshold + 3");
    hInputHistos[1]->SetTitle("pulser voltage : 980 mV, V threshold + 3");
    hInputHistos[2]->SetTitle("pulser voltage : 1000 mV, V threshold + 3");
    hInputHistos[3]->SetTitle("pulser voltage : 960 mV, V threshold + 5");
    hInputHistos[4]->SetTitle("pulser voltage : 980 mV, V threshold + 5");
    hInputHistos[5]->SetTitle("pulser voltage : 1000 mV, V threshold + 5");

    TCanvas *cBias = new TCanvas("cBias", "cBias", 2400, 2100);

    auto legend = new TLegend(0.1,0.1,0.9,0.3);
    
    legend->SetNColumns(6);
    
    legend->AddEntry(hInputHistos[0],  "chip0-C1");
    legend->AddEntry(hInputHistos[12], "chip0-C3");
    legend->AddEntry(hInputHistos[24], "chip0-E1");
    legend->AddEntry(hInputHistos[36], "chip0-E3");
    legend->AddEntry(hInputHistos[48], "chip0-G1");
    legend->AddEntry(hInputHistos[60], "chip0-G3");
    legend->AddEntry(hInputHistos[6],  "chip0-C2");
    legend->AddEntry(hInputHistos[18], "chip0-C4");
    legend->AddEntry(hInputHistos[30], "chip0-E2");
    legend->AddEntry(hInputHistos[42], "chip0-E4");
    legend->AddEntry(hInputHistos[54], "chip0-G2");
    legend->AddEntry(hInputHistos[66], "chip0-G4");

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
    
    //istogrammi misure/media
    std::array<TH1F*, N_Files> hDivisions;
    char *histName=new char[10];

    for (int i = 0; i != N_Files; ++i){
        sprintf(histName,"h%d",i);
        hDivisions[i] = (TH1F*)(hInputHistos[i]->Clone(histName));
    }

    for (int j = 0; j != N_Config; ++j) {
        TH1F *hMean = (TH1F*)(hInputHistos[j]->Clone("hMean"));
        hMean->Sumw2();
            for (int l = 0; l != hInputHistos[j]->GetNbinsX(); ++l){
                float sum = 0.;
                float sqerr = 0.;
                int reps = 0.;
                for (int i = j; i < N_Files; i += N_Config){
                    sum += hInputHistos[i]->GetBinContent(l);
                    sqerr += pow(hInputHistos[i]->GetBinError(l), 2);
                    if (hInputHistos[i]->GetBinContent(l) != 0) {
                        ++reps;
                    }
                }
                if (reps != 0){
                    hMean->SetBinContent(l, sum/(float)reps);
                    hMean->SetBinError(l, (sqrt(sqerr))/(float)reps);
                }
            }
        for (int i = j; i < N_Files; i += N_Config){
            hDivisions[i] -> Divide(hInputHistos[i], hMean);
        }
    }

    TCanvas *cDivisions = new TCanvas("cDivisions", "cDivisions", 2400, 2100);

    auto divlegend = new TLegend(0.1,0.1,0.9,0.3);
    
    divlegend->SetNColumns(6);
    
    divlegend->AddEntry(hDivisions[0],  "chip0-C1/mean");
    divlegend->AddEntry(hDivisions[12], "chip0-C3/mean");
    divlegend->AddEntry(hDivisions[24], "chip0-E1/mean");
    divlegend->AddEntry(hDivisions[36], "chip0-E3/mean");
    divlegend->AddEntry(hDivisions[48], "chip0-G1/mean");
    divlegend->AddEntry(hDivisions[60], "chip0-G3/mean");
    divlegend->AddEntry(hDivisions[6],  "chip0-C2/mean");
    divlegend->AddEntry(hDivisions[18], "chip0-C4/mean");
    divlegend->AddEntry(hDivisions[30], "chip0-E2/mean");
    divlegend->AddEntry(hDivisions[42], "chip0-E4/mean");
    divlegend->AddEntry(hDivisions[54], "chip0-G2/mean");
    divlegend->AddEntry(hDivisions[66], "chip0-G4/mean");

    cDivisions->Divide(3, 2);
    for(int i = 0; i != N_Config; ++i){
        cDivisions->cd(i+1);
        gPad->SetGridy();
        hDivisions[i]->GetYaxis()->SetRangeUser(0, 2);
        hDivisions[i]->GetYaxis()->SetTitle("(Rate on - off) / (Rate on - off (mean))");
        for(int j = i; j < N_Files; j += N_Config){
            hDivisions[j]->Draw("SAME, P");
        }
        divlegend->Draw("SAME");
    }

    cDivisions->SaveAs("cDivisions.root");
    cDivisions->SaveAs("cDivisions.png");
    delete cDivisions;
    
    //istogrammi musura/media -1 cumulativi per punto
    std::array<std::vector<TH1F*>, N_Config> hDivsConfig;
    char *DivVName=new char[10];
    
    for (int kCurrentConfig = 0; kCurrentConfig != N_Config; ++kCurrentConfig){
        for (int i = 0; i != hDivisions[0]->GetNbinsX(); ++i){
            if (hDivisions[0]->GetBinContent(i) != 0 && hDivisions[0]->GetBinCenter(i) > 49.9){
                auto current_v = hDivisions[0]->GetBinCenter(i);
                sprintf(DivVName, "divV%.2f_%d", current_v, kCurrentConfig);
                TH1F* kHisto = new TH1F(DivVName, DivVName, 101, -0.505, 0.505);
                for (int j = kCurrentConfig; j < N_Files; j += N_Config){
                    kHisto->Fill(hDivisions[j]->GetBinContent(i) - 1);
                }
                hDivsConfig[kCurrentConfig].push_back((TH1F*)(kHisto->Clone(DivVName)));
                delete kHisto;
            }
        }
    }
    
    //istogrammi con errori di misura/media, cumulativi per punto
    std::array<std::vector<TH1F*>, N_Config> hDivsConfigErr;
    char *DivVerrName=new char[15];
    for (int kCurrentConfig = 0; kCurrentConfig != N_Config; ++kCurrentConfig){
        for (int i = 0; i != hDivisions[0]->GetNbinsX(); ++i){
            if (hDivisions[0]->GetBinContent(i) != 0 && hDivisions[0]->GetBinCenter(i) > 49.9 ){
                auto current_v = hDivisions[0]->GetBinCenter(i);
                sprintf(DivVerrName, "divV_err%.2f_%d", current_v, kCurrentConfig);
                TH1F* kHisto = new TH1F(DivVerrName, DivVerrName, 101, 0, 0.04);
                for (int j = kCurrentConfig; j < N_Files; j += N_Config){
                        kHisto->Fill(hDivisions[j]->GetBinError(i));
                }
                hDivsConfigErr[kCurrentConfig].push_back((TH1F*)(kHisto->Clone(DivVerrName)));
                delete kHisto;
            }
        }
    }
    
    //istogrammi misura/media - 1, cumulativi
    std::array<TH1F*, N_Config> hDiffs;
    char *diffName=new char[10];
    
    for (int i = 0; i != N_Config; ++i){
        sprintf(diffName, "hDiff%d", i);
        hDiffs[i] = new TH1F(diffName, diffName, 101, -0.505, 0.505);
        for (int j = i; j < N_Files; j += N_Config){
            for (int bin = 0; bin != (hDivisions[j]->GetNbinsX()); ++bin){
                if ((hDivisions[j]->GetBinCenter(bin)) > 49.9 && (hDivisions[j]->GetBinContent(bin) != 0)){
                    hDiffs[i]->Fill(hDivisions[j]->GetBinContent(bin) - 1);
                }
            }
        }
    }
    
    
    //istogrammi con errori di misura/media
    std::array<TH1F*, N_Config> hDiffs_err;
    char *diff_err_Name=new char[10];
    
    for (int i = 0; i != N_Config; ++i){
        sprintf(diff_err_Name, "hDiff_err%d", i);
        hDiffs_err[i] = new TH1F(diff_err_Name, diff_err_Name, 101, 0., 0.04);
        for (int j = i; j < N_Files; j += N_Config){
            for (int bin = 0; bin != (hDivisions[j]->GetNbinsX()); ++bin){
                if ((hDivisions[j]->GetBinCenter(bin)) > 49.9 && (hDivisions[j]->GetBinContent(bin) != 0)){
                    hDiffs_err[i]->Fill(hDivisions[j]->GetBinError(bin));
                }
            }
        }
    }
    
    
    //scrittura su file output
    TFile *fOutputFile = new TFile ("vbias_output.root", "RECREATE");
    for (auto kCurrentHisto : hDiffs){
        kCurrentHisto->Write();
    }
    for (auto kCurrentHisto : hDiffs_err){
        kCurrentHisto->Write();
    }
    for (int i = 0; i != N_Config; ++i){
        for (auto kCurrentHisto : hDivsConfig[i]){
            kCurrentHisto->Write();
        }
    }
    for (int i = 0; i != N_Config; ++i){
        for (auto kCurrentHisto : hDivsConfigErr[i]){
            kCurrentHisto->Write();
        }
    }
    
    fOutputFile->Close();
    for (auto kCurrentFile : fInputFiles){
        kCurrentFile->Close();
    }
}

