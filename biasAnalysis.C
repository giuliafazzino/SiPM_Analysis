
void
biasAnalysis ( TString fFileNamea = "", TString fFileNameb = "",  TString fFileNamec = ""  ) {
    if ( fFileNamea == "" || fFileNameb == "" || fFileNamec== "" ) {
        cout << " NO FILENAME AAAAHHHHH "  << endl;
    }
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    //
    TFile*  fInputFilea =   new TFile( fFileNamea );
    TFile*  fInputFileb =   new TFile( fFileNameb );
    TFile*  fInputFilec =   new TFile( fFileNamec );
    //
    TH1F*   hScan_On960  =   (TH1F*)(fInputFilea->Get("hScan_On"));
    TH1F*   hScan_On980  =   (TH1F*)(fInputFileb->Get("hScan_On"));
    TH1F*   hScan_On1000  =   (TH1F*)(fInputFilec->Get("hScan_On"));
    //
    TH1F*   hScan_Off960  =   (TH1F*)(fInputFilea->Get("hScan_Off"));
    TH1F*   hScan_Off980  =   (TH1F*)(fInputFileb->Get("hScan_Off"));
    TH1F*   hScan_Off1000  =   (TH1F*)(fInputFilec->Get("hScan_Off"));
    //
    TH1F*   hScan_Dif960  =   (TH1F*)(fInputFilea->Get("hScan_Dif"));
    TH1F*   hScan_Dif980  =   (TH1F*)(fInputFileb->Get("hScan_Dif"));
    TH1F*   hScan_Dif1000  =   (TH1F*)(fInputFilec->Get("hScan_Dif"));
    //
    TH1F*   hScan_Div960 = (TH1F*)hScan_Dif960->Clone("hScan_Div960");
    hScan_Div960->Divide(hScan_Dif960, hScan_Off960);
    hScan_Div960->GetXaxis()->SetTitle("Bias Voltage (mV)");
    hScan_Div960->GetYaxis()->SetTitle("Rate (on-off)/off (Hz)");

    TH1F*   hScan_Div980 = (TH1F*)hScan_Dif980->Clone("hScan_Div980");
    hScan_Div980->Divide(hScan_Dif980, hScan_Off980);
    hScan_Div980->GetXaxis()->SetTitle("Bias Voltage (mV)");
    hScan_Div980->GetYaxis()->SetTitle("Rate (on-off)/off (Hz)");

    TH1F*   hScan_Div1000 = (TH1F*)hScan_Dif1000->Clone("hScan_Div1000");
    hScan_Div1000->Divide(hScan_Dif1000, hScan_Off1000);
    hScan_Div1000->GetXaxis()->SetTitle("Bias Voltage (mV)");
    hScan_Div1000->GetYaxis()->SetTitle("Rate (on-off)/off (Hz)");

    //
    gROOT->SetBatch();
    TCanvas*    cDrawCompare = new TCanvas();
    cDrawCompare->Divide(2,2);

    cDrawCompare->cd(1);
    gPad->SetLogy();
    hScan_On960->Draw();
    hScan_On980->SetLineColor(kRed);
    hScan_On980->Draw("SAME");
    hScan_On1000->SetLineColor(kGreen);
    hScan_On1000->Draw("same");

    auto legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->AddEntry(hScan_On960,"960mV");
    legend->AddEntry(hScan_On980,"980mV");
    legend->AddEntry(hScan_On1000,"1000mV");
    legend->Draw("Same");

    cDrawCompare->cd(2);
    gPad->SetLogy();
    hScan_Off960->Draw();
    hScan_Off980->SetLineColor(kRed);
    hScan_Off980->Draw("SAME");
    hScan_Off1000->SetLineColor(kGreen);
    hScan_Off1000->Draw("same");

    cDrawCompare->cd(3);
    gPad->SetLogy();
    hScan_Dif960->Draw();
    hScan_Dif980->SetLineColor(kRed);
    hScan_Dif980->Draw("SAME");
    hScan_Dif1000->SetLineColor(kGreen);
    hScan_Dif1000->Draw("same");

    cDrawCompare->cd(4);
    hScan_Div960->Draw();
    hScan_Div980->SetLineColor(kRed);
    hScan_Div980->Draw("SAME");
    hScan_Div1000->SetLineColor(kGreen);
    hScan_Div1000->Draw("same");

    cDrawCompare->SaveAs("cDrawCompare.root");
    delete cDrawCompare;
    //

    TH1F*   hNorm980 = (TH1F*)hScan_Dif960->Clone("hNorm980");
    hNorm980->Divide(hScan_Dif980, hScan_Dif960);
    hNorm980->GetXaxis()->SetTitle("Bias Voltage (mV)");
    hNorm980->GetYaxis()->SetTitle("Rate (on-off) 980mV / Rate (on-off) 960mV");
    hNorm980->SetLineColor(kRed);

    TH1F*   hNorm1000 = (TH1F*)hScan_Dif960->Clone("hNorm1000");
    hNorm1000->Divide(hScan_Dif1000, hScan_Dif960);
    hNorm1000->GetXaxis()->SetTitle("Bias Voltage (mV)");
    hNorm1000->GetYaxis()->SetTitle("Rate (on-off) 1000mV / Rate (on-off) 960mV");
    hNorm1000->SetLineColor(kGreen);

    TH1F*   hNorm960 = (TH1F*)hScan_Dif960->Clone("hNorm1000");
    hNorm960->Divide(hScan_Dif1000, hScan_Dif980);
    hNorm960->GetXaxis()->SetTitle("Bias Voltage (mV)");
    hNorm960->GetYaxis()->SetTitle("Rate (on-off) 1000mV / Rate (on-off) 980mV");
    hNorm960->SetLineColor(kBlue);

    TLatex* fLatex  =   new TLatex();

    TCanvas* cNormalize = new TCanvas();
    cNormalize->Divide(2,2);
    cNormalize->cd(1);
    hNorm960->SetMinimum(0);
    hNorm960->SetMaximum(10);
    hNorm960->Draw("SAME");
    hNorm960->Fit("pol0","IMRES","",50,56);
    auto fNorm960 = hNorm960->GetFunction("pol0");
    fLatex->DrawLatexNDC(0.3,0.8,Form("pol0: %.4f +- %.4f", fNorm960->GetParameter(0), fNorm960->GetParError(0) ));
    cNormalize->cd(2);
    hNorm980->SetMinimum(0);
    hNorm980->SetMaximum(10);
    hNorm980->Draw("SAME");
    hNorm980->Fit("pol0","IMRES","",50,56);
    auto fNorm980 = hNorm980->GetFunction("pol0");
    fLatex->DrawLatexNDC(0.3,0.8,Form("pol0: %.4f +- %.4f", fNorm980->GetParameter(0), fNorm980->GetParError(0) ));
    cNormalize->cd(3);
    hNorm1000->SetMinimum(0);
    hNorm1000->SetMaximum(10);
    hNorm1000->Draw("SAME");
    hNorm1000->Fit("pol0","IMRES","",50,56);
    auto fNorm100 = hNorm1000->GetFunction("pol0");
    fLatex->DrawLatexNDC(0.3,0.8,Form("pol0: %.4f +- %.4f", fNorm100->GetParameter(0), fNorm100->GetParError(0) ));
    cNormalize->cd(4);
    hNorm960->Draw("SAME");
    hNorm980->Draw("SAME");
    hNorm1000->Draw("SAME");
    cNormalize->SaveAs("cNormalize.root");
    delete cNormalize;

    gROOT->SetBatch(false);
    fInputFilec->Close();
    fInputFileb->Close();
    fInputFilea->Close();
}



