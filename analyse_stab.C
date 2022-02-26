void analyse_stab
(TString fFName1 = "", TString fFName2 = "", TString fFName3 = "", TString fFName4 = "",TString fFName5 = "",
 TString fFName6 = "", TString fFName7 = "", TString fFName8 = "", TString fFName9 = "")
{
   if (fFName1 == "" || fFName2 == "" || fFName3 == "" || fFName4 == "" || fFName5 =="" ||
        fFName6 == "" || fFName7 == "" || fFName8 == "" || fFName9 == ""){
       cout <<"No file name" <<endl;
   }

   TFile* fIFile1 = new TFile(fFName1);
   TFile* fIFile2 = new TFile(fFName2);
   TFile* fIFile3 = new TFile(fFName3);
   TFile* fIFile4 = new TFile(fFName4);
   TFile* fIFile5 = new TFile(fFName5);
   TFile* fIFile6 = new TFile(fFName6);
   TFile* fIFile7 = new TFile(fFName7);
   TFile* fIFile8 = new TFile(fFName8);
   TFile* fIFile9 = new TFile(fFName9);

   TGraphErrors* gStab1 = (TGraphErrors*)(fIFile1->Get("gTempDif"));
   TGraphErrors* gStab2 = (TGraphErrors*)(fIFile2->Get("gTempDif"));
   TGraphErrors* gStab3 = (TGraphErrors*)(fIFile3->Get("gTempDif"));
   TGraphErrors* gStab4 = (TGraphErrors*)(fIFile4->Get("gTempDif"));
   TGraphErrors* gStab5 = (TGraphErrors*)(fIFile5->Get("gTempDif"));
   TGraphErrors* gStab6 = (TGraphErrors*)(fIFile6->Get("gTempDif"));
   TGraphErrors* gStab7 = (TGraphErrors*)(fIFile7->Get("gTempDif"));
   TGraphErrors* gStab8 = (TGraphErrors*)(fIFile8->Get("gTempDif"));
   TGraphErrors* gStab9 = (TGraphErrors*)(fIFile9->Get("gTempDif"));


   TGraphErrors* gStab2Norm = new TGraphErrors();
   TGraphErrors* gStab3Norm = new TGraphErrors();
   TGraphErrors* gStab5Norm = new TGraphErrors();
   TGraphErrors* gStab6Norm = new TGraphErrors();
   TGraphErrors* gStab8Norm = new TGraphErrors();
   TGraphErrors* gStab9Norm = new TGraphErrors();
   gStab2->SetLineColor(3);
   gStab3->SetLineColor(4);
   gStab4->SetLineColor(40);
   gStab5->SetLineColor(6);
   gStab6->SetLineColor(7);
   gStab7->SetLineColor(8);
   gStab8->SetLineColor(9);
   gStab9->SetLineColor(30);

   TGraphErrors* gStabR2 = new TGraphErrors();
   TGraphErrors* gStabR3 = new TGraphErrors();
   TGraphErrors* gStabR4 = new TGraphErrors();
   TGraphErrors* gStabR5 = new TGraphErrors();
   TGraphErrors* gStabR6 = new TGraphErrors();
   TGraphErrors* gStabR7 = new TGraphErrors();
   TGraphErrors* gStabR8 = new TGraphErrors();
   TGraphErrors* gStabR9 = new TGraphErrors();


   TCanvas* cStab = new TCanvas();
   cStab->Divide(5,2);
   cStab->cd(1);
   gStab1->Draw();
   cStab->cd(2);
   gStab2->Draw();
   cStab->cd(3);
   gStab3->Draw();
   cStab->cd(4);
   gStab4->Draw();
   cStab->cd(5);
   gStab5->Draw();
   cStab->cd(6);
   gStab6->Draw();
   cStab->cd(7);
   gStab7->Draw();
   cStab->cd(8);
   gStab8->Draw();
   cStab->cd(9);
   gStab9->Draw();

   cStab->cd(10);
   auto legend = new TLegend(0.1,0.7,0.48,0.9);
   legend->AddEntry(gStab1, "v_pulser = 960mV, v_bias = 51.3 V");
   legend->AddEntry(gStab2, "v_pulser = 960mV, v_bias = 52.3 V");
   legend->AddEntry(gStab3, "v_pulser = 960mV, v_bias = 53.3 V");
   legend->AddEntry(gStab4, "v_pulser = 980mV, v_bias = 51.3 V");
   legend->AddEntry(gStab5, "v_pulser = 980mV, v_bias = 52.3 V");
   legend->AddEntry(gStab6, "v_pulser = 980mV, v_bias = 53.3 V");
   legend->AddEntry(gStab7, "v_pulser = 1000mV, v_bias = 51.3 V");
   legend->AddEntry(gStab8, "v_pulser = 1000mV, v_bias = 52.3 V");
   legend->AddEntry(gStab9, "v_pulser = 1000mV, v_bias = 53.3 V");
   legend->Draw();

   cStab->SaveAs("cStab.root");
   delete cStab;

   fIFile9->Close();
   fIFile8->Close();
   fIFile7->Close();
   fIFile6->Close();
   fIFile5->Close();
   fIFile4->Close();
   fIFile3->Close();
   fIFile2->Close();
   fIFile1->Close();
};