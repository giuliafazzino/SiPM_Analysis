void dispersions(){
    
    gStyle->SetOptStat(0000);
    
    TFile *fA1  = new TFile("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/rootfiles/A1_bias.root");
    TFile *fAll = new TFile("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/rootfiles/vbias_output.root");
    
    char *histname = new char[10];
    float RMS = 0.;
    float e_RMS = 0.;
    float mean = 0.;
    float e_mean = 0.;
  
    //errore totale, A1
    TH1F *hA1_3_Tot = new TH1F("hA1_3_Tot", "Sensore A1", 610, 949.05, 1010.05);
    TH1F *hA1_5_Tot = new TH1F("hA1_5_Tot", "Sensore A1", 610, 949.05, 1010.05);
    TH1F *hA1_3_Tot_D = new TH1F("hA1_3_Tot_D", "V threshold +3 (A1)", 17, 946, 1014);
    TH1F *hA1_5_Tot_D = new TH1F("hA1_5_Tot_D", "V threshold +5 (A1)", 17, 946, 1014);

    for (int i = 0; i != 6; ++i){
        sprintf(histname, "hDiff%d", i);
        TH1F *histo = (TH1F*)fA1->Get(histname);
        RMS = histo->GetRMS();
        e_RMS = histo->GetRMSError();
        switch (i){
            case 0:
                hA1_3_Tot->SetBinContent(110, RMS);
                hA1_3_Tot->SetBinError(110, e_RMS);
                hA1_3_Tot_D->SetBinContent(4, RMS);
                break;
            case 1:
                hA1_3_Tot->SetBinContent(310, RMS);
                hA1_3_Tot->SetBinError(310, e_RMS);
                hA1_3_Tot_D->SetBinContent(9, RMS);
                break;
            case 2:
                hA1_3_Tot->SetBinContent(510, RMS);
                hA1_3_Tot->SetBinError(510, e_RMS);
                hA1_3_Tot_D->SetBinContent(14, RMS);
                break;
            case 3:
                hA1_5_Tot->SetBinContent(110, RMS);
                hA1_5_Tot->SetBinError(110, e_RMS);
                hA1_5_Tot_D->SetBinContent(4, RMS);
                break;
            case 4:
                hA1_5_Tot->SetBinContent(310, RMS);
                hA1_5_Tot->SetBinError(310, e_RMS);
                hA1_5_Tot_D->SetBinContent(9, RMS);
                break;
            case 5:
                hA1_5_Tot->SetBinContent(510, RMS);
                hA1_5_Tot->SetBinError(510, e_RMS);
                hA1_5_Tot_D->SetBinContent(14, RMS);
                break;
        }
    }
    
    hA1_3_Tot_D->SetFillColorAlpha(0,0);
    hA1_3_Tot_D->SetLineColor(1);
    hA1_3_Tot_D->SetLineWidth(3);
    hA1_5_Tot_D->SetFillColorAlpha(0,0);
    hA1_5_Tot_D->SetLineColor(1);
    hA1_5_Tot_D->SetLineWidth(3);
    
    //errore statistico, A1
    TH1F *hA1_3_Stat = new TH1F("hA1_3_Stat", "Sensore A1", 610, 949.05, 1010.05);
    TH1F *hA1_5_Stat = new TH1F("hA1_5_Stat", "Sensore A1", 610, 949.05, 1010.05);
    TH1F *hA1_3_Stat_D = new TH1F("hA1_3_Stat_D", "V threshold +3 (A1)", 17, 946, 1014);
    TH1F *hA1_5_Stat_D = new TH1F("hA1_5_Stat_D", "V threshold +5 (A1)", 17, 946, 1014);
    
    for (int i = 0; i != 6; ++i){
        sprintf(histname, "hDiff_err%d", i);
        TH1F *histo = (TH1F*)fA1->Get(histname);
        mean = histo->GetMean();
        e_mean = histo->GetMeanError();
        switch (i){
            case 0:
                hA1_3_Stat->SetBinContent(110, mean);
                hA1_3_Stat->SetBinError(110, e_mean);
                hA1_3_Stat_D->SetBinContent(4, mean);
                break;
            case 1:
                hA1_3_Stat->SetBinContent(310, mean);
                hA1_3_Stat->SetBinError(310, e_mean);
                hA1_3_Stat_D->SetBinContent(9, mean);
                break;
            case 2:
                hA1_3_Stat->SetBinContent(510, mean);
                hA1_3_Stat->SetBinError(510, e_mean);
                hA1_3_Stat_D->SetBinContent(14, mean);
                break;
            case 3:
                hA1_5_Stat->SetBinContent(110, mean);
                hA1_5_Stat->SetBinError(110, e_mean);
                hA1_5_Stat_D->SetBinContent(4, mean);
                break;
            case 4:
                hA1_5_Stat->SetBinContent(310, mean);
                hA1_5_Stat->SetBinError(310, e_mean);
                hA1_5_Stat_D->SetBinContent(9, mean);
                break;
            case 5:
                hA1_5_Stat->SetBinContent(510, mean);
                hA1_5_Stat->SetBinError(510, e_mean);
                hA1_5_Stat_D->SetBinContent(14, mean);
                break;
        }
    }
    
    hA1_3_Stat_D->SetFillColorAlpha(38, 0.35);
    hA1_3_Stat_D->SetLineColorAlpha(0, 0);
    hA1_5_Stat_D->SetFillColorAlpha(38, 0.35);
    hA1_5_Stat_D->SetLineColorAlpha(0, 0);
    
    //errore sistematico, A1
    TH1F *hA1_3_Sys = new TH1F("hA1_3_Sys", "Sensore A1", 610, 949.05, 1010.05);
    TH1F *hA1_5_Sys = new TH1F("hA1_5_Sys", "Sensore A1", 610, 949.05, 1010.05);
    TH1F *hA1_3_Sys_D = new TH1F("hA1_3_Stat_D", "V threshold +3 (A1)", 17, 946, 1014);
    TH1F *hA1_5_Sys_D = new TH1F("hA1_5_Stat_D", "V threshold +5 (A1)", 17, 946, 1014);
    
    for (int bin = 0; bin != hA1_3_Tot->GetNbinsX(); ++bin){
        float sys_3 = sqrt(pow(hA1_3_Tot->GetBinContent(bin),2)-pow(hA1_3_Stat->GetBinContent(bin),2));
        float sys_5 = sqrt(pow(hA1_5_Tot->GetBinContent(bin),2)-pow(hA1_5_Stat->GetBinContent(bin),2));
        if (sys_3 != 0 && sys_5 != 0){
            hA1_3_Sys->SetBinContent(bin, sys_3);
            hA1_3_Sys->SetBinError(bin, sqrt(pow(hA1_3_Tot->GetBinContent(bin)*hA1_3_Tot->GetBinError(bin)/sys_3,2)
                                        +pow(hA1_3_Stat->GetBinContent(bin)*hA1_3_Stat->GetBinError(bin)/sys_3,2)));
            hA1_5_Sys->SetBinContent(bin, sys_5);
            hA1_5_Sys->SetBinError(bin, sqrt(pow(hA1_5_Tot->GetBinContent(bin)*hA1_5_Tot->GetBinError(bin)/sys_5,2)
                                        +pow(hA1_5_Stat->GetBinContent(bin)*hA1_5_Stat->GetBinError(bin)/sys_5,2)));
            
        }
    }
    hA1_3_Sys_D->SetBinContent(4, hA1_3_Sys->GetBinContent(110));
    hA1_3_Sys_D->SetBinContent(9, hA1_3_Sys->GetBinContent(310));
    hA1_3_Sys_D->SetBinContent(14, hA1_3_Sys->GetBinContent(510));
    hA1_5_Sys_D->SetBinContent(4, hA1_5_Sys->GetBinContent(110));
    hA1_5_Sys_D->SetBinContent(9, hA1_5_Sys->GetBinContent(310));
    hA1_5_Sys_D->SetBinContent(14, hA1_5_Sys->GetBinContent(510));
    
    hA1_3_Sys_D->SetFillColorAlpha(0, 0);
    hA1_3_Sys_D->SetLineColor(kBlue);
    hA1_3_Sys_D->SetLineWidth(3);
    hA1_5_Sys_D->SetFillColorAlpha(0, 0);
    hA1_5_Sys_D->SetLineColor(kBlue);
    hA1_5_Sys_D->SetLineWidth(3);
    
    //errore totale, sensori
    TH1F *hAll_3_Tot = new TH1F("hAll_3_Tot", "Tutti i sensori", 610, 949.05, 1010.05);
    TH1F *hAll_5_Tot = new TH1F("hAll_5_Tot", "Tutti i sensori", 610, 949.05, 1010.05);
    TH1F *hAll_3_Tot_D = new TH1F("h_All_3_Tot_D", "V threshold +3 (C, E, G)", 17, 946, 1014);
    TH1F *hAll_5_Tot_D = new TH1F("h_All_5_Tot_D", "V threshold +5 (C, E, G)", 17, 946, 1014);

    for (int i = 0; i != 6; ++i){
        sprintf(histname, "hDiff%d", i);
        TH1F *histo = (TH1F*)fAll->Get(histname);
        RMS = histo->GetRMS();
        e_RMS = histo->GetRMSError();
        switch (i){
            case 0:
                hAll_3_Tot->SetBinContent(110, RMS);
                hAll_3_Tot->SetBinError(110, e_RMS);
                hAll_3_Tot_D->SetBinContent(4, RMS);
                break;
            case 1:
                hAll_3_Tot->SetBinContent(310, RMS);
                hAll_3_Tot->SetBinError(310, e_RMS);
                hAll_3_Tot_D->SetBinContent(9, RMS);
                break;
            case 2:
                hAll_3_Tot->SetBinContent(510, RMS);
                hAll_3_Tot->SetBinError(510, e_RMS);
                hAll_3_Tot_D->SetBinContent(14, RMS);
                break;
            case 3:
                hAll_5_Tot->SetBinContent(110, RMS);
                hAll_5_Tot->SetBinError(110, e_RMS);
                hAll_5_Tot_D->SetBinContent(4, RMS);
                break;
            case 4:
                hAll_5_Tot->SetBinContent(310, RMS);
                hAll_5_Tot->SetBinError(310, e_RMS);
                hAll_5_Tot_D->SetBinContent(9, RMS);
                break;
            case 5:
                hAll_5_Tot->SetBinContent(510, RMS);
                hAll_5_Tot->SetBinError(510, e_RMS);
                hAll_5_Tot_D->SetBinContent(14, RMS);
                break;
        }
    }
    
    hAll_3_Tot_D->SetFillColorAlpha(0,0);
    hAll_3_Tot_D->SetLineColor(kBlack);
    hAll_3_Tot_D->SetLineWidth(4);
    hAll_5_Tot_D->SetFillColorAlpha(0,0);
    hAll_5_Tot_D->SetLineColor(kBlack);
    hAll_5_Tot_D->SetLineWidth(4);
    
    //errore statistico sensori
    TH1F *hAll_3_Stat = new TH1F("hAll_3_Stat", "Tutti i sensori", 610, 949.05, 1010.05);
    TH1F *hAll_5_Stat = new TH1F("hAll_5_Stat", "Tutti i sensori", 610, 949.05, 1010.05);
    TH1F *hAll_3_Stat_D = new TH1F("hAll_3_Stat_D", "V threshold +3 (C, E, G)", 17, 946, 1014);
    TH1F *hAll_5_Stat_D = new TH1F("hAll_5_Stat_D", "V threshold +5 (C, E, G)", 17, 946, 1014);
    
    for (int i = 0; i != 6; ++i){
        sprintf(histname, "hDiff_err%d", i);
        TH1F *histo = (TH1F*)fAll->Get(histname);
        mean = histo->GetMean();
        e_mean = histo->GetMeanError();
        switch (i){
            case 0:
                hAll_3_Stat->SetBinContent(110, mean);
                hAll_3_Stat->SetBinError(110, e_mean);
                hAll_3_Stat_D->SetBinContent(4, mean);
                break;
            case 1:
                hAll_3_Stat->SetBinContent(310, mean);
                hAll_3_Stat->SetBinError(310, e_mean);
                hAll_3_Stat_D->SetBinContent(9, mean);
                break;
            case 2:
                hAll_3_Stat->SetBinContent(510, mean);
                hAll_3_Stat->SetBinError(510, e_mean);
                hAll_3_Stat_D->SetBinContent(14, mean);
                break;
            case 3:
                hAll_5_Stat->SetBinContent(110, mean);
                hAll_5_Stat->SetBinError(110, e_mean);
                hAll_5_Stat_D->SetBinContent(4, mean);
                break;
            case 4:
                hAll_5_Stat->SetBinContent(310, mean);
                hAll_5_Stat->SetBinError(310, e_mean);
                hAll_5_Stat_D->SetBinContent(9, mean);
                break;
            case 5:
                hAll_5_Stat->SetBinContent(510, mean);
                hAll_5_Stat->SetBinError(510, e_mean);
                hAll_5_Stat_D->SetBinContent(14, mean);
                break;
        }
    }
    
    hAll_3_Stat_D->SetFillColorAlpha(38, 0.35);
    hAll_3_Stat_D->SetLineColorAlpha(0, 0);
    hAll_3_Stat_D->SetLineWidth(3);
    hAll_5_Stat_D->SetFillColorAlpha(38, 0.35);
    hAll_5_Stat_D->SetLineColorAlpha(0, 0);
    hAll_5_Stat_D->SetLineWidth(3);
    
    
    //dispersione reale sensori
    TH1F *hAll_3_Disp = new TH1F("hAll_3_Disp", "Tutti i sensori", 610, 949.05, 1010.05);
    TH1F *hAll_5_Disp = new TH1F("hAll_5_Disp", "Tutti i sensori", 610, 949.05, 1010.05);
    TH1F *hAll_3_Disp_D = new TH1F("h_All_3_Disp_D", "V threshold +3 (C, E, G)", 17, 946, 1014);
    TH1F *hAll_5_Disp_D = new TH1F("h_All_5_Disp_D", "V threshold +5 (C, E, G)", 17, 946, 1014);

    for (int bin = 0; bin != hA1_3_Tot->GetNbinsX(); ++bin){
        float disp_3 = sqrt(pow(hAll_3_Tot->GetBinContent(bin),2)-pow(hAll_3_Stat->GetBinContent(bin),2));
        float disp_5 = sqrt(pow(hAll_5_Tot->GetBinContent(bin),2)-pow(hAll_5_Stat->GetBinContent(bin),2));
        if (disp_3 != 0 && disp_5 != 0){
            hAll_3_Disp->SetBinContent(bin, disp_3);
            hAll_3_Disp->SetBinError(bin, sqrt(pow(hAll_3_Tot->GetBinContent(bin)*hAll_3_Tot->GetBinError(bin)/disp_3,2)
                                               +pow(hAll_3_Stat->GetBinContent(bin)*hAll_3_Stat->GetBinError(bin)/disp_3,2)));
            hAll_5_Disp->SetBinContent(bin, disp_5);
            hAll_5_Disp->SetBinError(bin, sqrt(pow(hAll_5_Tot->GetBinContent(bin)*hAll_5_Tot->GetBinError(bin)/disp_5,2)
                                               +pow(hAll_5_Stat->GetBinContent(bin)*hAll_5_Stat->GetBinError(bin)/disp_5,2)));
            
        }
    }
    
    hAll_3_Disp_D->SetBinContent(4, hAll_3_Disp->GetBinContent(110));
    hAll_3_Disp_D->SetBinContent(9, hAll_3_Disp->GetBinContent(310));
    hAll_3_Disp_D->SetBinContent(14, hAll_3_Disp->GetBinContent(510));
    hAll_5_Disp_D->SetBinContent(4, hAll_5_Disp->GetBinContent(110));
    hAll_5_Disp_D->SetBinContent(9, hAll_5_Disp->GetBinContent(310));
    hAll_5_Disp_D->SetBinContent(14, hAll_5_Disp->GetBinContent(510));
    
    hAll_3_Disp_D->SetFillColorAlpha(0, 0);
    hAll_3_Disp_D->SetLineColor(kGreen);
    hAll_3_Disp_D->SetLineWidth(3);
    hAll_5_Disp_D->SetFillColorAlpha(0, 0);
    hAll_5_Disp_D->SetLineColor(kGreen);
    hAll_5_Disp_D->SetLineWidth(3);
    
    //errore sensori
    TH1F *hAll_3_Sens = new TH1F("hAll_3_Sens", "Tutti i sensori", 610, 949.05, 1010.05);
    TH1F *hAll_5_Sens = new TH1F("hAll_5_Sens", "Tutti i sensori", 610, 949.05, 1010.05);
    TH1F *hAll_3_Sens_D = new TH1F("h_All_3_Sens_D", "V threshold +3 (C, E, G)", 17, 946, 1014);
    TH1F *hAll_5_Sens_D = new TH1F("h_All_5_Sens_D", "V threshold +5 (C, E, G)", 17, 946, 1014);

    for (int bin = 0; bin != hA1_3_Tot->GetNbinsX(); ++bin){
        float sens_3 = sqrt(pow(hAll_3_Tot->GetBinContent(bin),2)-pow(hA1_3_Tot->GetBinContent(bin),2));
        float sens_5 = sqrt(pow(hAll_5_Tot->GetBinContent(bin),2)-pow(hA1_5_Tot->GetBinContent(bin),2));
        if (sens_3 != 0 && sens_5 != 0){
            hAll_3_Sens->SetBinContent(bin, sens_3);
            hAll_3_Sens->SetBinError(bin, sqrt(pow(hAll_3_Tot->GetBinContent(bin)*hAll_3_Tot->GetBinError(bin)/sens_3,2)
                                               +pow(hA1_3_Tot->GetBinContent(bin)*hA1_3_Tot->GetBinError(bin)/sens_3,2)));
            hAll_5_Sens->SetBinContent(bin, sens_5);
            hAll_5_Sens->SetBinError(bin, sqrt(pow(hAll_5_Tot->GetBinContent(bin)*hAll_5_Tot->GetBinError(bin)/sens_5,2)
                                               +pow(hA1_5_Tot->GetBinContent(bin)*hA1_5_Tot->GetBinError(bin)/sens_5,2)));
            
        }
    }
    hAll_3_Sens_D->SetBinContent(4, hAll_3_Sens->GetBinContent(110));
    hAll_3_Sens_D->SetBinContent(9, hAll_3_Sens->GetBinContent(310));
    hAll_3_Sens_D->SetBinContent(14, hAll_3_Sens->GetBinContent(510));
    hAll_5_Sens_D->SetBinContent(4, hAll_5_Sens->GetBinContent(110));
    hAll_5_Sens_D->SetBinContent(9, hAll_5_Sens->GetBinContent(310));
    hAll_5_Sens_D->SetBinContent(14, hAll_5_Sens->GetBinContent(510));
    
    hAll_3_Sens_D->SetFillColorAlpha(0, 0);
    hAll_3_Sens_D->SetLineColor(kRed);
    hAll_3_Sens_D->SetLineWidth(3);
    hAll_5_Sens_D->SetFillColorAlpha(0, 0);
    hAll_5_Sens_D->SetLineColor(kRed);
    hAll_5_Sens_D->SetLineWidth(3);
    
    //grafica A1
    TLegend *legendA1_3 = new TLegend(0.55, 0.75, 0.9, 0.9);
    legendA1_3->AddEntry(hA1_3_Tot_D,  "Total dispersion");
    legendA1_3->AddEntry(hA1_3_Sys_D, "Systematic dispersion");
    legendA1_3->AddEntry(hA1_3_Stat_D, "Statistical error");
    
    TLegend *legendA1_5 = new TLegend(0.55, 0.75, 0.9, 0.9);
    legendA1_5->AddEntry(hA1_5_Tot_D,  "Total dispersion");
    legendA1_5->AddEntry(hA1_5_Sys_D, "Systematic dispersion");
    legendA1_5->AddEntry(hA1_5_Stat_D, "Statistical error");

    TCanvas *cA1 = new TCanvas("cA1", "Sensore A1", 1800, 900);
    cA1->Divide(2,1);
    
    cA1->cd(1);
    gPad->SetGridy();
    hA1_3_Sys_D->GetYaxis()->SetRangeUser(0, 0.047);
    hA1_3_Sys_D->GetXaxis()->SetTitle("Pulse voltage (mV)");
    hA1_3_Sys_D->Draw("HIST");
    hA1_3_Stat_D->Draw("HIST, SAME");
    hA1_3_Tot_D->Draw("HIST, SAME");
    legendA1_3->Draw("SAME");
    
    cA1->cd(2);
    gPad->SetGridy();
    hA1_5_Sys_D->GetYaxis()->SetRangeUser(0, 0.047);
    hA1_5_Sys_D->GetXaxis()->SetTitle("Pulse voltage (mV)");
    hA1_5_Sys_D->Draw("HIST");
    hA1_5_Stat_D->Draw("HIST, SAME");
    hA1_5_Tot_D->Draw("HIST, SAME");
    legendA1_5->Draw("SAME");
    
    cA1->SaveAs("cA1.png");
    delete cA1;
    
    //grafica con tutti i sensori, totale e statistico
    TLegend *legendAll_3 = new TLegend(0.45, 0.70, 0.9, 0.9);
    legendAll_3->AddEntry(hAll_3_Tot_D, "Total dispersion");
    legendAll_3->AddEntry(hAll_3_Disp_D, "\"Real\" dispersion");
    legendAll_3->AddEntry(hAll_3_Stat_D, "Statistical error");
    
    TLegend *legendAll_5 = new TLegend(0.45, 0.70, 0.9, 0.9);
    legendAll_5->AddEntry(hAll_5_Tot_D, "Total dispersion");
    legendAll_5->AddEntry(hAll_5_Disp_D, "\"Real\" dispersion");
    legendAll_5->AddEntry(hAll_5_Stat_D, "Statistical error");
    
    TCanvas *cAll_1 = new TCanvas("cAll_1", "Tutti i sensori", 1800, 900);
    cAll_1->Divide(2,1);
    
    cAll_1->cd(1);
    gPad->SetGridy();
    hAll_3_Disp_D->GetYaxis()->SetRangeUser(0, 0.07);
    hAll_3_Disp_D->GetXaxis()->SetTitle("Pulse voltage (mV)");
    hAll_3_Disp_D->Draw("HIST");
    hAll_3_Stat_D->Draw("HIST, SAME");
    hAll_3_Tot_D->Draw("HIST, SAME");
    legendAll_3->Draw("SAME");
    
    cAll_1->cd(2);
    gPad->SetGridy();
    hAll_5_Disp_D->GetYaxis()->SetRangeUser(0, 0.07);
    hAll_5_Disp_D->GetXaxis()->SetTitle("Pulse voltage (mV)");
    hAll_5_Disp_D->Draw("HIST");
    hAll_5_Stat_D->Draw("HIST, SAME");
    hAll_5_Tot_D->Draw("HIST, SAME");
    legendAll_5->Draw("SAME");
    
    cAll_1->SaveAs("cAll_1.png");
    delete cAll_1;
    
    //grafica con tutti i sensori, spezzettata
    TLegend *legendAll_3_N = new TLegend(0.45, 0.70, 0.9, 0.9);
    legendAll_3_N->AddEntry(hAll_3_Tot_D,  "Total dispersion");
    legendAll_3_N->AddEntry(hA1_3_Sys_D, "Systematic dispersion(A1)");
    legendAll_3_N->AddEntry(hAll_3_Sens_D, "Sensors dispersion");
    legendAll_3_N->AddEntry(hAll_3_Stat_D, "Statistical error");
    
    TLegend *legendAll_5_N = new TLegend(0.45, 0.70, 0.9, 0.9);
    legendAll_5_N->AddEntry(hAll_5_Tot_D,  "Total dispersion");
    legendAll_5_N->AddEntry(hA1_5_Sys_D, "Systematic dispersion(A1)");
    legendAll_5_N->AddEntry(hAll_5_Sens_D, "Sensors dispersion");
    legendAll_5_N->AddEntry(hAll_5_Stat_D, "Statistical error");
    
    TCanvas *cAll_2 = new TCanvas("cAll", "Tutti i sensori", 1800, 900);
    cAll_2->Divide(2,1);
    
    cAll_2->cd(1);
    gPad->SetGridy();
    hAll_3_Stat_D->GetYaxis()->SetRangeUser(0, 0.07);
    hAll_3_Stat_D->GetXaxis()->SetTitle("Pulse voltage (mV)");
    hAll_3_Stat_D->Draw("HIST");
    hA1_3_Sys_D->Draw("HIST,SAME");
    hAll_3_Sens_D->Draw("SAME");
    hAll_3_Tot_D->Draw("HIST, SAME");
    legendAll_3_N->Draw("SAME");
    
    cAll_2->cd(2);
    gPad->SetGridy();
    hAll_5_Stat_D->GetYaxis()->SetRangeUser(0, 0.07);
    hAll_5_Stat_D->GetXaxis()->SetTitle("Pulse voltage (mV)");
    hAll_5_Stat_D->Draw("HIST");
    hA1_5_Sys_D->Draw("HIST, SAME");
    hAll_5_Sens_D->Draw("SAME");
    hAll_5_Tot_D->Draw("HIST, SAME");
    legendAll_5_N->Draw("SAME");
    
    cAll_2->SaveAs("cAll_2.png");
    delete cAll_2;
    
}

void dispersion_vbias(){
    gStyle->SetOptStat(0000);
    
    TFile *fA1  = new TFile("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/A1_bias.root");
    TFile *fAll = new TFile("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/vbias_output.root");
    
    int const N_Config = 6;
    char *histname = new char[15];
    float RMS = 0.;
    float mean = 0.;
    
    double v_bias[13] = {50.28, 50.82, 51.30, 51.78, 52.32, 52.80, 53.28, 53.82, 54.30, 54.78, 55.32, 55.80, 56.28};
    
    //errore totale, A1
    std::array<TH1F*, N_Config> hA1_Tot;
    for (int i = 0; i != N_Config; ++i){
        hA1_Tot[i] = new TH1F (Form("hA1tot%d", i), "hA1tot", 135, 48.27, 56.37);
        for (int j = 0; j != 13; ++j){
            sprintf(histname, "divV%.2f_%d", v_bias[j], i);
            TH1F *histo = (TH1F*)(fA1->Get(histname));
            RMS  = histo->GetRMS();
            auto kCurrenBin = hA1_Tot[i]->FindBin(v_bias[j]);
            hA1_Tot[i]->SetBinContent(kCurrenBin, RMS);
            delete histo;
        }
    }
    
    for (auto kCurrentHist : hA1_Tot){
        kCurrentHist->SetMarkerColor(kBlack);
        kCurrentHist->SetLineColor(kBlack);
        kCurrentHist->SetMarkerStyle(kFullCircle);
    }
    
    
    //errore statistico, A1
    std::array<TH1F*, N_Config> hA1_Stat;
    for (int i = 0; i != N_Config; ++i){
        hA1_Stat[i] = new TH1F (Form("hA1stat%d", i), "hA1stat", 135, 48.27, 56.37);
        for (int j = 0; j != 13; ++j){
            sprintf(histname, "divV_err%.2f_%d", v_bias[j], i);
            TH1F *histo = (TH1F*)(fA1->Get(histname));
            mean = histo->GetMean();
            auto kCurrenBin = hA1_Stat[i]->FindBin(v_bias[j]);
            hA1_Stat[i]->SetBinContent(kCurrenBin, mean);
            delete histo;
        }
    }
    
    for (auto kCurrentHist : hA1_Stat){
        kCurrentHist->SetMarkerColor(kBlue);
        kCurrentHist->SetLineColor(kBlue);
        kCurrentHist->SetMarkerStyle(kCircle);
    }
    
     
    //errore sistematico, A1
    std::array<TH1F*, N_Config> hA1_Sys;
    for (int i = 0; i != N_Config; ++i){
        hA1_Sys[i] = new TH1F (Form("hA1sys%d", i), "hA1sys", 135, 48.27, 56.37);
        for (int bin = 0; bin != hA1_Tot[i]->GetNbinsX(); ++bin){
            float sys = 0;
            if (hA1_Tot[i]->GetBinContent(bin) > hA1_Stat[i]->GetBinContent(bin)){
                sys = sqrt(pow(hA1_Tot[i]->GetBinContent(bin),2)-pow(hA1_Stat[i]->GetBinContent(bin),2));
            }
            if (sys != 0) {
                hA1_Sys[i]->SetBinContent(bin, sys);
            }
        }
    }
    
    for (auto kCurrentHist : hA1_Sys){
        kCurrentHist->SetMarkerColor(kRed);
        kCurrentHist->SetLineColor(kRed);
        kCurrentHist->SetMarkerStyle(kOpenSquare);
    }
    
    TLegend *legend = new TLegend(0.1, 0.9, 0.4, 0.75);
    legend->SetFillColorAlpha(0,0);
    legend->AddEntry(hA1_Tot[0], "Total", "P");
    legend->AddEntry(hA1_Stat[0], "Statistical", "P");
    legend->AddEntry(hA1_Sys[0], "Systematic", "P");
    
    
    hA1_Sys[0]->SetTitle("pvol = 960, Vthre = +3");
    hA1_Sys[1]->SetTitle("pvol = 980, Vthre = +3");
    hA1_Sys[2]->SetTitle("pvol = 1000, Vthre = +3");
    hA1_Sys[3]->SetTitle("pvol = 960, Vthre = +5");
    hA1_Sys[4]->SetTitle("pvol = 980, Vthre = +5");
    hA1_Sys[5]->SetTitle("pvol = 1000, Vthre = +5");
    
    std::array<TCanvas*, N_Config> cA1;
    
    for (int i = 0; i != N_Config; ++i){
        cA1[i] = new TCanvas(Form("cA1_%d", i), "C", 900, 900);
        hA1_Sys[i]->GetXaxis()->SetRangeUser(50, 57);
        hA1_Sys[i]->GetYaxis()->SetRangeUser(0, 0.08);
        hA1_Sys[i]->GetXaxis()->SetTitle("V bias (V)");
        hA1_Sys[i]->Draw("P");
        hA1_Tot[i]->Draw("P, SAME");
        hA1_Stat[i]->Draw("P, SAME");
        legend->Draw("SAME");
        cA1[i]->SaveAs(Form("cA1_%d.png", i));
        delete cA1[i];
    }
    
    
    //errore totale, sensori
    std::array<TH1F*, N_Config> hSens_Tot;
    for (int i = 0; i != N_Config; ++i){
        hSens_Tot[i] = new TH1F (Form("hSenstot%d", i), "hSenstot", 135, 48.27, 56.37);
        for (int j = 0; j != 13; ++j){
            sprintf(histname, "divV%.2f_%d", v_bias[j], i);
            TH1F *histo = (TH1F*)(fAll->Get(histname));
            RMS  = histo->GetRMS();
            auto kCurrenBin = hSens_Tot[i]->FindBin(v_bias[j]);
            hSens_Tot[i]->SetBinContent(kCurrenBin, RMS);
            delete histo;
        }
    }
    
    for (auto kCurrentHist : hSens_Tot){
        kCurrentHist->SetLineColor(1);
        kCurrentHist->SetMarkerColor(1);
        kCurrentHist->SetMarkerStyle(kFullCircle);
    }
    
    
    //errore statistico, sensori
    std::array<TH1F*, N_Config> hSens_Stat;
    for (int i = 0; i != N_Config; ++i){
        hSens_Stat[i] = new TH1F (Form("hSensstat%d", i), "hSensstat", 135, 48.27, 56.37);
        for (int j = 0; j != 13; ++j){
            sprintf(histname, "divV_err%.2f_%d", v_bias[j], i);
            TH1F *histo = (TH1F*)(fAll->Get(histname));
            mean = histo->GetMean();
            auto kCurrenBin = hSens_Stat[i]->FindBin(v_bias[j]);
            hSens_Stat[i]->SetBinContent(kCurrenBin, mean);
            delete histo;
        }
    }
    
    for (auto kCurrentHist : hSens_Stat){
        kCurrentHist->SetLineColor(kBlue);
        kCurrentHist->SetMarkerColor(kBlue);
        kCurrentHist->SetMarkerStyle(kCircle);
    }
    
     
    //errore reale, sensori
    std::array<TH1F*, N_Config> hSens_Rea;
    for (int i = 0; i != N_Config; ++i){
        hSens_Rea[i] = new TH1F (Form("hSensRe%d", i), "hSensRe", 135, 48.27, 56.37);
        for (int bin = 0; bin != hSens_Tot[i]->GetNbinsX(); ++bin){
            float disp = sqrt(pow(hSens_Tot[i]->GetBinContent(bin),2)-pow(hSens_Stat[i]->GetBinContent(bin),2));
            if (disp != 0) {
                hSens_Rea[i]->SetBinContent(bin, disp);
            }
        }
    }
    
    for (auto kCurrentHist : hSens_Rea){
        kCurrentHist->SetLineColor(kGreen);
        kCurrentHist->SetMarkerColor(kGreen);
        kCurrentHist->SetMarkerStyle(kOpenTriangleUp);
    }
    
    TLegend *legendSens = new TLegend(0.6, 0.9, 0.9, 0.7);
    legendSens->SetFillColorAlpha(0,0);
    legendSens->AddEntry(hSens_Tot[0], "Total", "P");
    legendSens->AddEntry(hSens_Stat[0], "Statistical", "P");
    legendSens->AddEntry(hSens_Rea[0], "Real", "P");
    
    
    hSens_Rea[0]->SetTitle("pvol = 960, Vthre = +3");
    hSens_Rea[1]->SetTitle("pvol = 980, Vthre = +3");
    hSens_Rea[2]->SetTitle("pvol = 1000, Vthre = +3");
    hSens_Rea[3]->SetTitle("pvol = 960, Vthre = +5");
    hSens_Rea[4]->SetTitle("pvol = 980, Vthre = +5");
    hSens_Rea[5]->SetTitle("pvol = 1000, Vthre = +5");
    
    std::array<TCanvas*, N_Config> cSens;
    
    for (int i = 0; i != N_Config; ++i){
        cSens[i] = new TCanvas(Form("cSens_%d", i), "C", 900, 900);
        hSens_Rea[i]->GetYaxis()->SetRangeUser(0, 0.09);
        hSens_Rea[i]->GetXaxis()->SetRangeUser(50, 57);
        hSens_Rea[i]->GetXaxis()->SetTitle("V bias (V)");
        hSens_Rea[i]->Draw("P");
        hSens_Tot[i]->Draw("P, SAME");
        hSens_Stat[i]->Draw("P, SAME");
        legendSens->Draw("SAME");
        cSens[i]->SaveAs(Form("cSens_%d.png", i));
        delete cSens[i];
    }
    fA1->Close();
    fAll->Close();
}
