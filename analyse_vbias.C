void analyse_vbias(
        TString fFName960t1 = "", TString fFName980t1 = "", TString fFName1000t1 = "",
        TString fFName960t2 = "", TString fFName980t2 = "", TString fFName1000t2 = "",
        TString sens = "", TString t1 = "", TString t2 = "")
{
    gStyle->SetOptStat(00000);

    if (fFName960t1 == "" || fFName980t1 == "" || fFName1000t1 == "" ||
        fFName960t2 == "" || fFName980t2 == "" || fFName1000t2 == "") {
        cout << "No filename" << endl;
    }

    TFile *fFile960t1 = new TFile(fFName960t1);
    TFile *fFile980t1 = new TFile(fFName980t1);
    TFile *fFile1000t1 = new TFile(fFName1000t1);
    TFile *fFile960t2 = new TFile(fFName960t2);
    TFile *fFile980t2 = new TFile(fFName980t2);
    TFile *fFile1000t2= new TFile(fFName1000t2);

    TH1F *h960t1= (TH1F * )(fFile960t1->Get("hScan_Dif"));
    TH1F *h980t1 = (TH1F * )(fFile980t1->Get("hScan_Dif"));
    TH1F *h1000t1 = (TH1F * )(fFile1000t1->Get("hScan_Dif"));
    TH1F *h960t2 = (TH1F * )(fFile960t2->Get("hScan_Dif"));
    TH1F *h980t2 = (TH1F * )(fFile980t2->Get("hScan_Dif"));
    TH1F *h1000t2 = (TH1F * )(fFile1000t2->Get("hScan_Dif"));

    h960t1->SetTitle("v_pulser = 960mV, threshold = " + t1 + ", " + sens);
    h980t1->SetTitle("v_pulser = 980mV, threshold = " + t1 + ", " + sens);
    h1000t1->SetTitle("v_pulser = 1000mV, threshold = " + t1 + ", " + sens);
    h960t2->SetTitle("v_pulser = 960mV, threshold = " + t2 + ", " + sens);
    h980t2->SetTitle("v_pulser = 980mV, threshold = " + t2 + ", " + sens);
    h1000t2->SetTitle("v_pulser = 1000mV, threshold = " + t2 + ", " + sens);

    TCanvas *cBias= new TCanvas("cBias_" + sens, "cBias_" + sens, 800, 2400);
    cBias->Divide(3, 2);
    cBias->cd(1);
    gPad->SetLogy();
    h960t1->Draw("SAME");

    cBias->cd(2);
    gPad->SetLogy();
    h980t1->Draw();

    cBias->cd(3);
    gPad->SetLogy();
    h1000t1->Draw();

    cBias->cd(4);
    gPad->SetLogy();
    h960t2->Draw();

    cBias->cd(5);
    gPad->SetLogy();
    h980t2->Draw();

    cBias->cd(6);
    gPad->SetLogy();
    h1000t2->Draw();

    cBias->SaveAs("cBias_" + sens + ".root");
    cBias->SaveAs("cBias_" + sens + ".png");
    delete cBias;


    fFile1000t2->Close();
    fFile980t2->Close();
    fFile960t2->Close();
    fFile1000t1->Close();
    fFile980t1->Close();
    fFile960t1->Close();

}
