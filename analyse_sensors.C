void analyse_sensors(TString fFName960a = "",TString fFName980a = "",TString fFName1000a = "",
                 TString fFName960b = "",TString fFName980b = "",TString fFName1000b = "",
                 TString fFName960c = "",TString fFName980c = "",TString fFName1000c = "",
                 TString var = "") {

    gStyle->SetOptStat(00000);

    if (fFName960a == "" || fFName980a == "" || fFName1000a == "" ||
        fFName960b == "" || fFName980b == "" || fFName1000b == "" ||
        fFName960c == "" || fFName980c == "" || fFName1000c == "" ) {
        cout << "No filename" << endl;
    }

    TFile *fFile960a = new TFile(fFName960a);
    TFile *fFile980a = new TFile(fFName980a);
    TFile *fFile1000a = new TFile(fFName1000a);
    TFile *fFile960b = new TFile(fFName960b);
    TFile *fFile980b = new TFile(fFName980b);
    TFile *fFile1000b = new TFile(fFName1000b);
    TFile *fFile960c = new TFile(fFName960c);
    TFile *fFile980c = new TFile(fFName980c);
    TFile *fFile1000c = new TFile(fFName1000c);

    TH1F *h960a = (TH1F * )(fFile960a->Get("hScan_Dif"));
    TH1F *h980a = (TH1F * )(fFile980a->Get("hScan_Dif"));
    TH1F *h1000a = (TH1F * )(fFile1000a->Get("hScan_Dif"));
    TH1F *h960b = (TH1F * )(fFile960b->Get("hScan_Dif"));
    TH1F *h980b = (TH1F * )(fFile980b->Get("hScan_Dif"));
    TH1F *h1000b = (TH1F * )(fFile1000b->Get("hScan_Dif"));
    TH1F *h960c = (TH1F * )(fFile960c->Get("hScan_Dif"));
    TH1F *h980c = (TH1F * )(fFile980c->Get("hScan_Dif"));
    TH1F *h1000c = (TH1F * )(fFile1000c->Get("hScan_Dif"));

    h960a->SetLineColor(kRed);
    h980a->SetLineColor(kRed);
    h1000a->SetLineColor(kRed);
    h960b->SetLineColor(kBlue);
    h980b->SetLineColor(kBlue);
    h1000b->SetLineColor(kBlue);
    h960c->SetLineColor(8);
    h980c->SetLineColor(8);
    h1000c->SetLineColor(8);

    h960a->SetTitle("v_pulser = 960mV");
    h980a->SetTitle("v_pulser = 980mV");
    h1000a->SetTitle("v_pulser = 1000mV");

    TCanvas *cSens = new TCanvas("cSens_" + var, "cSens_"+var, 800, 2400);
    cSens->Divide(1, 3);

    cSens->cd(1);
    auto legend = new TLegend(0.70, 0.9, 0.90, 0.7);
    legend->SetFillColorAlpha(0.0, 0.0);
    legend->SetLineColorAlpha(0.0, 0.0);
    legend->AddEntry(h960a, "chip0-C4");
    legend->AddEntry(h960b, "chip1-A1");
    legend->AddEntry(h960c, "chip1-A2");
    gPad->SetLogy();
    h960a->Draw();
    h960b->Draw("SAME");
    h960c->Draw("SAME");
    legend->Draw("SAME");

    cSens->cd(2);
    gPad->SetLogy();
    h980a->Draw();
    h980b->Draw("SAME");
    h980c->Draw("SAME");
    legend->Draw("SAME");

    cSens->cd(3);
    gPad->SetLogy();
    h1000a->Draw();
    h1000b->Draw("SAME");
    h1000c->Draw("SAME");
    legend->Draw("SAME");

    cSens->SaveAs("cSens_" + var + ".root");
    cSens->SaveAs("cSens_" + var + ".pdf");
    delete cSens;

    fFile1000c->Close();
    fFile980c->Close();
    fFile960c->Close();
    fFile1000b->Close();
    fFile980b->Close();
    fFile960b->Close();
    fFile1000a->Close();
    fFile980a->Close();
    fFile960a->Close();
}