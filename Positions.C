void positions(){
	TH1F *hPositions = new TH1F("hPosition", "hPositions", 10, 0, 10);
	
	hPositions->GetXaxis()->SetBinLabel(1, "A1_X");
	hPositions->SetBinContent(1, - 0.067);
	hPositions->SetBinError(1, 0.011);
	
	hPositions->GetXaxis()->SetBinLabel(2, "A1_Y");
	hPositions->SetBinContent(2, -0.120);
	hPositions->SetBinError(2, 0.012);
	
	hPositions->GetXaxis()->SetBinLabel(3, "C1_X");
	hPositions->SetBinContent(3, 0.009);
	hPositions->SetBinError(3, 0.011);
	
	hPositions->GetXaxis()->SetBinLabel(4, "C1_Y");
	hPositions->SetBinContent(4,-0.082);
	hPositions->SetBinError(4, 0.011);
	
	hPositions->GetXaxis()->SetBinLabel(5, "C4_X");
	hPositions->SetBinContent(5, 0.009);
	hPositions->SetBinError(5, 0.012);
	
	hPositions->GetXaxis()->SetBinLabel(6, "C4_Y");
	hPositions->SetBinContent(6, -0.110);
	hPositions->SetBinError(6, 0.012);
	
	hPositions->GetXaxis()->SetBinLabel(7, "G1_X");
	hPositions->SetBinContent(7,-0.046);
	hPositions->SetBinError(7,0.012);
	
	hPositions->GetXaxis()->SetBinLabel(8, "G1_Y");
	hPositions->SetBinContent(8, -0.052);
	hPositions->SetBinError(8, 0.011);
	
	hPositions->GetXaxis()->SetBinLabel(9, "G4_X");
	hPositions->SetBinContent(9, -0.056);
	hPositions->SetBinError(9, 0.011);
	
	hPositions->GetXaxis()->SetBinLabel(10, "G4_Y");
	hPositions->SetBinContent(10, -0.103);
	hPositions->SetBinError(10, 0.011);

	TCanvas *c1 = new TCanvas();
	hPositions->GetYaxis()->SetTitle("Media (mm)");
	hPositions->Draw("E");
	c1->Print("medie.pdf");
	
	
}