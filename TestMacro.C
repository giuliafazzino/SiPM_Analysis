#include "ReadData.h"
void
TestMacro
( TString fFileName = "", Bool_t kIsX = true ) {
    InputData fData;
    TTree* fTree;
    std::pair InputData(fTree, fData);

    if ( fFileName == "" ) {
        cout << " NO FILENAME " << endl;
    }
    TFile*  fInputFile = new TFile( fFileName );

    assign(InputData, fInputFile, kIsX);

    TTree* DecodedData = merge(InputData);
    Float_t position;
    std::pair<Float_t, Float_t> Rate_on={};
    std::pair<Float_t, Float_t> Rate_off={};
    DecodedData->SetBranchAddress("Position", &position);
    DecodedData->SetBranchAddress("RateOff", &Rate_off);
    DecodedData->SetBranchAddress("RateOn", &Rate_on);


    TH1F*   hScan_Off  = new TH1F( "hScan_Off", "hScan_Off", 51, -5, +5 );
    hScan_Off->GetXaxis()->SetTitle("position (mm)");
    hScan_Off->GetYaxis()->SetTitle("rate (Hz)");
    //
    TH1F*   hScan_On_  = new TH1F( "hScan_On_", "hScan_On_", 51, -5, +5 );
    hScan_On_->GetXaxis()->SetTitle("position (mm)");
    hScan_On_->GetYaxis()->SetTitle("rate (Hz)");
    /*
    TH1F*   hScan_Dif  = new TH1F( "hScan_Dif", "hScan_Dif", 51, -5, +5 );
    hScan_Dif->GetXaxis()->SetTitle("position (mm)");
    hScan_Dif->GetYaxis()->SetTitle("rate (Hz)");
    */


    for ( Int_t i = 0; i < DecodedData->GetEntries(); i++ )    {
        DecodedData->GetEvent(i);

        auto kCurrent_Bin = hScan_Off->GetXaxis()->FindBin( position );
        hScan_Off->SetBinContent   ( kCurrent_Bin, Rate_off.first);
        hScan_Off->SetBinError     ( kCurrent_Bin, Rate_off.second);
        hScan_On_->SetBinContent   ( kCurrent_Bin, Rate_on.first);
        hScan_On_->SetBinError     ( kCurrent_Bin, Rate_on.second);
        //hScan_Dif->SetBinContent   ( kCurrent_Bin, (counts_on/period_on) - (counts_off/period_off) );
        //hScan_Dif->SetBinError     ( kCurrent_Bin, sqrt( counts_off/(period_off*period_off) + counts_on/(period_on*period_on) )  );
    }

    TCanvas *c1=new TCanvas();
    hScan_Off->Draw();
    c1->SaveAs("c1.pdf");
    TCanvas *c2=new TCanvas();
    hScan_On_->Draw();
    c2->SaveAs("c2.pdf");

    fInputFile->Close();
}
