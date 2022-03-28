#include "AnalysisUtility.h"


void makeIV(TString fFileName = ""){
    
    if (fFileName == ""){
        cout <<"No file name" <<endl;
    }
    TGraph *gIV = new TGraph(fFileName, "%lg%lg ");
    if(gIV->GetPointX(1) < 0){
        gIV = invert (gIV);
    }
    
    gIV->SetName("gIV");
    gIV->GetXaxis()->SetTitle("Voltage (V)");
    gIV->GetYaxis()->SetTitle("Current (A)");
    gIV->SetMarkerStyle(43);
    gIV->SetMarkerColor(kBlue);
    
    TFile* fOutputFile = new TFile (fFileName + ".root", "RECREATE");
    gIV->Write();
    fOutputFile->Close();
}
