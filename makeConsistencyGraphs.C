
void makeGraphs(TString iFileName = ""){
    
    gStyle->SetOptStat(0000);
    
    TFile *fInputFile = new TFile(iFileName);
    
    std::array<TH1F*,6> hInputHistos;
    char *diffName=new char[10];
    
    for ( int i = 0; i != 6; ++i){
        sprintf(diffName, "hDiff_err%d", i);
        hInputHistos[i] = (TH1F*)(fInputFile->Get(diffName));
    }
    
    TGraphErrors *gMeans3 = new TGraphErrors();
    TGraphErrors *gMeans5 = new TGraphErrors();
    
    gMeans3->SetName("gMeans_3");
    gMeans5->SetName("gMeans_5");
    
    double mean;
    double e_mean;
    int x ;
    int y;
    
    for (int i = 0; i != 6; ++i){
        mean = 0.;
        e_mean = 0.;
        x = 0;
        y = 0;
        mean = hInputHistos[i]->GetMean();
        e_mean = hInputHistos[i]->GetMeanError();
        int current_point;
        switch (i){
            case 0:
                current_point = gMeans3->GetN();
                gMeans3->SetPoint(current_point, 960, mean);
                gMeans3->SetPointError(current_point, 0, e_mean);
                break;
            case 1:
                current_point = gMeans3->GetN();
                gMeans3->SetPoint(current_point, 980, mean);
                gMeans3->SetPointError(current_point, 0, e_mean);
                break;
            case 2:
                current_point = gMeans3->GetN();
                gMeans3->SetPoint(current_point, 1000, mean);
                gMeans3->SetPointError(current_point, 0, e_mean);
                break;
            case 3:
                current_point = gMeans5->GetN();
                gMeans5->SetPoint(current_point, 960, mean);
                gMeans5->SetPointError(current_point, 0, e_mean);
                break;
            case 4:
                current_point = gMeans5->GetN();
                gMeans5->SetPoint(current_point, 980, mean);
                gMeans5->SetPointError(current_point, 0, e_mean);
                break;
            case 5:
                current_point = gMeans5->GetN();
                gMeans5->SetPoint(current_point, 1000, mean);
                gMeans5->SetPointError(current_point, 0, e_mean);
                break;
        }
    }
    
    gMeans3->SetMarkerStyle(kFullCircle);
    gMeans5->SetMarkerStyle(21);
    
    gMeans3->SetMarkerColor(kBlue);
    gMeans5->SetMarkerColor(kRed);
    gMeans3->SetLineColor(kBlue);
    gMeans5->SetLineColor(kRed);
    
    TMultiGraph *gMeans = new TMultiGraph();
    gMeans->Add(gMeans3);
    gMeans->Add(gMeans5);
    gMeans->SetTitle("Valore medio (errori biN)");
 
    TLegend *legend = new TLegend();
    legend->AddEntry(gMeans3, "V threshold +3");
    legend->AddEntry(gMeans5, "V threshold +5");
    
    TCanvas *cMean = new TCanvas("cMean", "cMean", 1600, 800);
    gPad->SetGridy();
    gMeans->Draw("AP");
    legend->Draw("SAME");
    
    cMean->SaveAs(iFileName + ".provameans.png");
    delete cMean;
    
    TFile *fOutputFile = new TFile (iFileName + ".out.root", "RECREATE");
    gMeans->Write();

    fOutputFile->Close();
    fInputFile->Close();
}
