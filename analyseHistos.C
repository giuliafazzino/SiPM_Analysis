
void
analyseHistos
( TString fFileNameX = "", TString fFileNameY = "" ) {
    if ( fFileNameX == "" || fFileNameY == "" ) {
        cout << " NO FILENAME AAAAHHHHH "  << endl;
    }
    //
     gStyle->SetOptFit(1011);
    //
    TFile*  fInputFileX =   new TFile( fFileNameX );
    TFile*  fInputFileY =   new TFile( fFileNameY );
    //
    TH1F*   hXScan_Dif  =   (TH1F*)(fInputFileX->Get("hScan_Dif"));
    TH1F*   hYScan_Dif  =   (TH1F*)(fInputFileY->Get("hScan_Dif"));
    //
    TF1*    custom_fit  =   new TF1("custom_fit","[0]*0.5*(1.-TMath::Erf((TMath::Abs(x-[1])-[2])/[3]))");
    custom_fit->SetParameter(0, 2E4 );
    custom_fit->SetParameter(1, 0);
    custom_fit->SetParameter(2, 1);
    custom_fit->SetParameter(3, 1);
    //
    gROOT->SetBatch();
    TCanvas*    cDrawCompare = new TCanvas();
    hXScan_Dif->Draw();
    hYScan_Dif->SetLineColor(8);
    hYScan_Dif->Draw("same");
    cDrawCompare->SaveAs("cDrawCompare.pdf");
    cDrawCompare->SaveAs("cDrawCompare.root");
    delete cDrawCompare;
    //
    cDrawCompare = new TCanvas();
	//fit posizioni in X
    hXScan_Dif->Draw();
    hXScan_Dif->Fit(custom_fit);
    custom_fit->Draw("same");
    cDrawCompare->SaveAs("cDrawCompare_FitX.pdf");
    delete cDrawCompare;
    //
    cDrawCompare = new TCanvas();
    //fit posizioni in Y
    hYScan_Dif->Draw();
    hYScan_Dif->Fit(custom_fit);
    custom_fit->Draw("same");
    cDrawCompare->SaveAs("cDrawCompare_FitY.pdf");
    cDrawCompare->SaveAs("cDrawCompare_FitY.root");
    delete cDrawCompare;
    //
    gROOT->SetBatch(false);
    //
    TFile*  fOutputFile  =   new TFile( fFileNameX + TString(".out.analysis.root"), "RECREATE" );
    //
    hXScan_Dif->Write();
	hYScan_Dif->Write();
    //

    fOutputFile->Close();
    fInputFileY->Close();
    fInputFileX->Close();
}

