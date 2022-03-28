void sensorsConsistency(TString iFileName = ""){
    
    gStyle->SetOptStat(0000);
    
    TFile *fInputFile = new TFile(iFileName);
    
    std::array<TH1F*,6> hInputHistos;
    char *diffName=new char[10];
    
    for ( int i = 0; i != 6; ++i){
        sprintf(diffName, "hDiff%d", i);
        hInputHistos[i] = (TH1F*)(fInputFile->Get(diffName));
    }
    
    TH2F *hMeans = new TH2F ("hMeans", "Medie", 3, 950, 1010, 2, 50.3, 54.3);
    TH2F *hRMS = new TH2F ("hRMS", "RMS", 3, 950, 1010, 2, 50.3, 54.3);
    
    hMeans->GetXaxis()->SetNdivisions(4,0,0);
    hRMS->GetXaxis()->SetNdivisions(4,0,0);
    
    
    hMeans->GetYaxis()->SetBinLabel(1, "+3");
    hMeans->GetYaxis()->SetBinLabel(2, "+5");
    hRMS->GetYaxis()->SetBinLabel(1, "+3");
    hRMS->GetYaxis()->SetBinLabel(2, "+5");
    hMeans->GetYaxis()->SetLabelSize(.05);
    hRMS->GetYaxis()->SetLabelSize(.05);
    
    hMeans->GetXaxis()->SetTitle("Pulser voltage (mV)");
    hMeans->GetYaxis()->SetTitle("V Threshold");
    hRMS->GetXaxis()->SetTitle("Pulser voltage (mV)");
    hRMS->GetYaxis()->SetTitle("V Threshold");
    
    double mean;
    double e_mean;
    double rms;
    double e_rms;
    int x ;
    int y;
    
    for (int i = 0; i != 6; ++i){
        mean = 0.;
        e_mean = 0.;
        rms = 0.;
        e_rms = 0.;
        x = 0;
        y = 0;
        mean = hInputHistos[i]->GetMean();
        e_mean = hInputHistos[i]->GetMeanError();
        rms = hInputHistos[i]->GetRMS();
        e_rms = hInputHistos[i]->GetRMSError();
        switch (i){
            case 0:
                x = 1;
                y = 1;
                break;
            case 1:
                x = 2;
                y = 1;
                break;
            case 2:
                x = 3;
                y = 1;
                break;
            case 3:
                x = 1;
                y = 2;
                break;
            case 4:
                x = 2;
                y = 2;
                break;
            case 5:
                x = 3;
                y = 2;
                break;
        }
        hMeans->SetBinContent(x, y, mean);
        hMeans->SetBinError(x, y, e_mean);
        hRMS->SetBinContent(x, y, rms);
        hRMS->SetBinError(x, y, e_rms);
    }
    
    gStyle->SetPaintTextFormat("4.3 g");
    TCanvas *cMean = new TCanvas("cMean", "cMean", 1600, 800);
    hMeans->DrawClone("COLZ TEXT ERROR");
    TCanvas *cRMS = new TCanvas("cRMS", "cRMS", 1600, 800);
    hRMS->DrawClone("COLZ TEXT ERROR");
    
    cMean->SaveAs(iFileName + ".means.png");
    cRMS->SaveAs(iFileName + ".RMS.png");
    
    fInputFile->Close();
}
