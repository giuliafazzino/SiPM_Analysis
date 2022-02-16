
void
readTree
( TString fFileName = "", Bool_t kIsX = true ) {
    if ( fFileName == "" ) {
        cout << " NO FILENAME AAAAHHHHH "  << endl;
    }
    //
    TFile*  fInputFile  =   new TFile( fFileName );
    //
    TTree*  tInputData  =   (TTree*)(fInputFile->Get("tree"));
    //
    Float_t position, counts_on, counts_off, period_on, period_off;
    if (kIsX) {
        tInputData->SetBranchAddress("position_x", &position);
    } else {
        tInputData->SetBranchAddress("position_y", &position);
    }
    tInputData->SetBranchAddress( "counts_on", &counts_on );
    tInputData->SetBranchAddress( "counts_off", &counts_off );
    tInputData->SetBranchAddress( "period_on", &period_on );
    tInputData->SetBranchAddress( "period_off", &period_off );
    //
    TH1F*   hScan_Off  = new TH1F( "hScan_Off", "hScan_Off", 51, -5, +5 );
    hScan_Off->GetXaxis()->SetTitle("position (mm)");
    hScan_Off->GetYaxis()->SetTitle("rate (Hz)");
    //
    TH1F*   hScan_On_  = new TH1F( "hScan_On_", "hScan_On_", 51, -5, +5 );
    hScan_On_->GetXaxis()->SetTitle("position (mm)");
    hScan_On_->GetYaxis()->SetTitle("rate (Hz)");
    //
    TH1F*   hScan_Dif  = new TH1F( "hScan_Dif", "hScan_Dif", 51, -5, +5 );
    hScan_Dif->GetXaxis()->SetTitle("position (mm)");
    hScan_Dif->GetYaxis()->SetTitle("rate (Hz)");
    //
    for ( Int_t iMes = 0; iMes < tInputData->GetEntries(); iMes++ )    {
        tInputData->GetEvent(iMes);
        auto kCurrent_Bin = hScan_Off->GetXaxis()->FindBin( position/800. );
        hScan_Off->SetBinContent   ( kCurrent_Bin, counts_off/period_off );
        hScan_Off->SetBinError     ( kCurrent_Bin, sqrt( counts_off )/period_off  );
        hScan_On_->SetBinContent   ( kCurrent_Bin, counts_on/period_on );
        hScan_On_->SetBinError     ( kCurrent_Bin, sqrt( counts_on )/period_on  );
        hScan_Dif->SetBinContent   ( kCurrent_Bin, (counts_on/period_on) - (counts_off/period_off) );
        hScan_Dif->SetBinError     ( kCurrent_Bin, sqrt( counts_off/(period_off*period_off) + counts_on/(period_on*period_on) )  );
    }
    /*PROVA
    TCanvas* c1=new TCanvas();
    hScan_Off->Draw();
    TCanvas* c2=new TCanvas();
    hScan_On_->Draw();
    TCanvas* c3=new TCanvas();
    hScan_Dif->Draw();
    */
    TFile*  fOutputFile  =   new TFile( fFileName + TString(".out.root"), "RECREATE" );
    //
    hScan_Dif->Write();
    //
    fOutputFile->Close();
    fInputFile->Close();
}
