void analyse_A1(Float_t thres, TString fFName960a = "",TString fFName980a = "",TString fFName1000a = "",
             TString fFName960b = "",TString fFName980b = "",TString fFName1000b = "",
             TString var = "") {

    gStyle->SetOptStat(00000);

    if (fFName960a == "" || fFName980a == "" || fFName1000a == "" ||
        fFName960b == "" || fFName980b == "" || fFName1000b == "") {
        cout << "No filename" << endl;
    }

    TFile *fFile960a = new TFile(fFName960a);
    TFile *fFile980a = new TFile(fFName980a);
    TFile *fFile1000a = new TFile(fFName1000a);
    TFile *fFile960b = new TFile(fFName960b);
    TFile *fFile980b = new TFile(fFName980b);
    TFile *fFile1000b = new TFile(fFName1000b);

    TH1F *h960a = (TH1F * )(fFile960a->Get("hScan_Dif"));
    TH1F *h980a = (TH1F * )(fFile980a->Get("hScan_Dif"));
    TH1F *h1000a = (TH1F * )(fFile1000a->Get("hScan_Dif"));
    TH1F *h960b = (TH1F * )(fFile960b->Get("hScan_Dif"));
    TH1F *h980b = (TH1F * )(fFile980b->Get("hScan_Dif"));
    TH1F *h1000b = (TH1F * )(fFile1000b->Get("hScan_Dif"));

    h960a->SetLineColor(kRed);
    h980a->SetLineColor(kRed);
    h1000a->SetLineColor(kRed);
    h960b->SetLineColor(kBlue);
    h980b->SetLineColor(kBlue);
    h1000b->SetLineColor(kBlue);

    h960a->SetTitle("v_pulser = 960mV");
    h980a->SetTitle("v_pulser = 980mV");
    h1000a->SetTitle("v_pulser = 1000mV");

    TCanvas *cA1= new TCanvas("cA1_"+var, "cA1_"+var, 800, 2400);
    cA1->Divide(1, 3);
    cA1->cd(1);
    auto legend = new TLegend(0.70, 0.2, 0.90, 0.4);
    legend->SetFillColorAlpha(0.0, 0.0);
    legend->SetLineColorAlpha(0.0, 0.0);
    legend->AddEntry(h960a, "1-A1, prima misura");
    legend->AddEntry(h960b, "1-A1, seconda misura");
    gPad->SetLogy();
    //h960a->GetXaxis()->SetRangeUser(8,64);
    h960a->Draw();
    h960b->Draw("SAME");
    legend->Draw("SAME");
    cA1->cd(2);
    gPad->SetLogy();
    //h980a->GetXaxis()->SetRangeUser(8,64);
    h980a->Draw();
    h980b->Draw("SAME");
    legend->Draw("SAME");
    cA1->cd(3);
    gPad->SetLogy();
    //h1000a->GetXaxis()->SetRangeUser(8,64);
    h1000a->Draw();
    h1000b->Draw("SAME");
    legend->Draw("SAME");

    cA1->SaveAs("cA1_" + var + "_thres_" + thres + ".root");
    cA1->SaveAs("cA1_" + var + "_thres_" + thres + ".pdf");
    delete cA1;


    fFile1000b->Close();
    fFile980b->Close();
    fFile960b->Close();
    fFile1000a->Close();
    fFile980a->Close();
    fFile960a->Close();
}