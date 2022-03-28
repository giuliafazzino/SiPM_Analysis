#include "AnalysisUtility.h"
#include "list_diretta.h"
#include "list_inversa.h"
#include "IV.C"

void prova(){

    TH1F*   hScan_On  = new TH1F( "hScan_On", "hScan_On", 201, -0.005, 2.005);
	
	for (int i = 0; i != hScan_On->GetNbinsX(); ++i){
		cout <<hScan_On->GetBinCenter(i+1) <<endl;
	}
}










































/*
void analyseIV_tempscan_dir(){
    gStyle->SetOptTitle(1);
    int const N_Temp = 9;
    float temps[N_Temp] = {26.6, 27.7, 30.3, 37.1, 46.5, 67.4, 89.2, 121, 142};
    
    //lettura file
    std::vector<TString> fInputNames = list_diretta();
    std::array<TFile*, N_Temp> fInputFiles;
    std::array<TGraph*, N_Temp> gIV;
    
    for(int i = 0; i != N_Temp; ++i){
        fInputFiles[i] = new TFile (fInputNames[i]);
        gIV[i] = (TGraph*)(fInputFiles[i]->Get("gIV"));
        if ( i != 4){
            gIV[i]->SetMarkerColor(i+1);
            gIV[i]->SetLineColor(i+1);
        } else {
            gIV[i]->SetMarkerColor(kOrange);
            gIV[i]->SetLineColor(kOrange);
        }
        gIV[i]->SetMarkerStyle(33);
    }
    

    //temperatura ambiente prima e dopo
    TFile *faDiretta = new TFile("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/18032022/debuffer4.txt.root");
    TFile *fbInversa = new TFile("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/16032022/debuffer1.txt.root");
    TFile *faInversa = new TFile("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/18032022/debuffer6.txt.root");
    
    TGraph *gaDiretta = (TGraph*)(faDiretta->Get("gIV"));
    TGraph *gbInversa = (TGraph*)(fbInversa->Get("gIV"));
    TGraph *gaInversa = (TGraph*)(faInversa->Get("gIV"));
    
    gaDiretta->SetMarkerStyle(kCircle);
    gaDiretta->SetMarkerColor(kRed);
    gaDiretta->SetLineColor(kRed);
    gbInversa->SetMarkerStyle(33);
    gbInversa->SetMarkerColor(kBlue);
    gbInversa->SetLineColor(kBlue);
    gaInversa->SetMarkerStyle(kCircle);
    gaInversa->SetMarkerColor(8);
    gaInversa->SetLineColor(8);
    
    //grafica temp ambiente
    TMultiGraph *gDiretta = new TMultiGraph();
    gDiretta->Add(gIV[0]);
    gDiretta->Add(gaDiretta);
    gDiretta->SetTitle("Temperatura ambiente, polarizzazione diretta; Voltaggio(V); Corrente(A)");
    
    TMultiGraph *gInversa = new TMultiGraph();
    gInversa->Add(gbInversa);
    gInversa->Add(gaInversa);
    gInversa->SetTitle("Temperatura ambiente, polarizzazione inversa; Voltaggio(V); Corrente(A)");
    
    TLegend *lDiretta = new TLegend(0.1, 0.572, 0.35, 0.9);
    lDiretta->AddEntry(gIV[0], "16/03/22");
    lDiretta->AddEntry(gaDiretta, "18/03/22");
    
    TLegend *lInversa = new TLegend(0.1, 0.572, 0.35, 0.9);
    lInversa->AddEntry(gbInversa, "17/03/22");
    lInversa->AddEntry(gaInversa, "18/03/22");
    
    TCanvas *cAmb = new TCanvas("cAmb", "cAmb", 2300, 900);
    cAmb->Divide(2,1);
    cAmb->cd(1);
    gDiretta->Draw("AP");
    lDiretta->Draw("SAME");
    cAmb->cd(2);
    gInversa->Draw("AP");
    lInversa->Draw("SAME");
    
    cAmb->SaveAs("cAmb.root");
    cAmb->SaveAs("cAmb.png");
    
    delete cAmb;
    
    //stessa misura (al buio) a confronto
    TH1F *hDiretta = new TH1F("hDiretta", "Polarizzazione diretta", 201, -0.0025, 1.0025);
    TH1F *hInversa = new TH1F("hInversa", "Polarizzazione inversa", 301, 24.995, 28.005);
    
    auto VBDb = fMeasureVbd(makemetgerrors(gbInversa), 26.6, 2, 0.3, 0.9, "BCOM_sn9_A1_299K_0_bef");
    auto VBDa = fMeasureVbd(makemetgerrors(gaInversa), 26.8, 2, 0.3, 0.9, "BCOM_sn9_A1_299K_0_aft");
    
    hDiretta->SetMarkerStyle(kFullCircle);
    hInversa->SetMarkerStyle(33);
    hInversa->SetMarkerColor(kBlue);
    hInversa->GetYaxis()->SetTitle("I dopo / I prima");
    hInversa->GetXaxis()->SetTitle("Voltaggio (V)");
    

    for(int i = 5; i != gaInversa->GetN(); ++i){
        double yb = gbInversa->GetPointY(i);
        double ya = gaInversa->GetPointY(i);
        hInversa->SetBinContent(2*i + 1, ya/yb);
    }
    
    double ratio_1 = gaInversa->Eval(VBDa.first + 1) / gbInversa->Eval(VBDb.first + 1);
    double ratio_5 = gaInversa->Eval(VBDa.first + 0.5) / gbInversa->Eval(VBDb.first + 0.5);
    cout <<endl;
    cout <<"ratio (overvg 0.5): " <<ratio_5 <<endl;
    cout <<"ratio (overvg 1): " <<ratio_1 <<endl;
}

void prova_3(){
    std::vector<TString> iFileNames = {"/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/24032022/inva1.txt.root",
                                       "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/24032022/inva2.txt.root",
                                        "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/24032022/inva3.txt.root",
                                        "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/24032022/inva4.txt.root",
                                        "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/24032022/invb1.txt.root",
                                        "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/24032022/invb2.txt.root",
                                        "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/24032022/invb3.txt.root",
                                        "/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/24032022/invb4.txt.root"};
    
    std::vector<TString> kNames = {"BCOM_sn9_A1_299K_0", "BCOM_sn9_A2_299K_0", "BCOM_sn9_A3_299K_0", "BCOM_sn9_A4_299K_0", "BCOM_sn9_B1_299K_0", "BCOM_sn9_B2_299K_0", "BCOM_sn9_B3_299K_0", "BCOM_sn9_B4_299K_0"};
    
    std::vector<TFile*> iFiles;
    std::vector<TGraphErrors*> gIV;
    for (int i = 0; i != iFileNames.size(); ++i){
        iFiles.push_back( new TFile(iFileNames[i]));
        gIV.push_back(makemetgerrors((TGraph*)(iFiles[i]->Get("gIV"))));
    }
    
    TGraphErrors *g1 = new TGraphErrors();

    for (int i = 0; i != gIV.size(); ++i){
        if( i != 2) {
        auto vbd = fMeasureVbd(gIV[i], 26.6, 2, 0.3, 0.9, kNames[i]);
        g1->SetPoint(i+1, i+1, vbd.first);
        g1->SetPointError(i+1, 0, vbd.second);
        }
    }
    
    TCanvas *c1 = new TCanvas("c1", "c1", 1100, 1100);
    
    g1->GetXaxis()->SetBinLabel(12, "A1");
    g1->GetXaxis()->SetBinLabel(23, "A2");
    //g1->GetXaxis()->SetBinLabel(1, "A1");
    g1->GetXaxis()->SetBinLabel(46, "A4");
    g1->GetXaxis()->SetBinLabel(57, "B1");
    g1->GetXaxis()->SetBinLabel(69, "B2");
    g1->GetXaxis()->SetBinLabel(80, "B3");
    g1->GetXaxis()->SetBinLabel(92, "B4");
    g1->SetMarkerStyle(kFullCircle);
    g1->SetMarkerColor(kBlue);
    g1->Fit("pol0");
    g1->GetYaxis()->SetRangeUser(26.8, 27.3);
    g1->GetYaxis()->SetTitle("VBD (V)");
    g1->Draw("AP");
    
    TF1 *dts = new TF1 ("dts",  "26.926", 0, 250);
    dts->SetLineColor(8);
    dts->Draw("SAME");
    
    
    TLatex *latexVBD = new TLatex();
    latexVBD->DrawLatexNDC(.13,.70,Form("VBD = %.3f V", (g1->GetFunction("pol0"))->GetParameter(0)));
    latexVBD->DrawLatexNDC(.13,.60,"DTS : 26.936 V");
    
    c1->SaveAs("CC1.png");
}

void prova_4(){
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(00000);
	TFile *fPre = new TFile("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/24032022/inve4.txt.root");
	TFile *fPos = new TFile("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/24032022/inve4post.txt.root");
	
	TGraphErrors *gPre = makemetgerrors((TGraph*)fPre->Get("gIV"));
	TGraphErrors *gPos = makemetgerrors((TGraph*)fPos->Get("gIV"));
	
	TLegend *lGraphs = new TLegend();
	lGraphs->AddEntry(gPre, "pre");
	lGraphs->AddEntry(gPos, "post");
	
	
	TMultiGraph *mg = new TMultiGraph();
	mg->Add(gPre);
	mg->Add(gPos);
	gPre->SetMarkerStyle(kCircle);
	gPre->SetMarkerSize(0.8);
	gPos->SetMarkerStyle(kCircle);
	gPos->SetMarkerSize(0.8);
	gPre->SetMarkerColor(kBlue);
	gPos->SetMarkerColor(kRed);
	mg->GetYaxis()->SetTitle("Current (A)");
	mg->GetXaxis()->SetTitle("Voltage (V)");
	
	TCanvas *cGraphs = new TCanvas("cGraphs", "cGraphs", 900, 900);
	//gPad->SetLogy();
	
	mg->Draw("AP");
	lGraphs->Draw("SAME");
	
	cGraphs->SaveAs("cGraphs.png");

	auto vbdPre = fMeasureVbd(gPre, 26.3, 2, 0.3, 0.8, "BCOM_sn9_E4_299K_0_PRE");
	auto vbdPos = fMeasureVbd(gPos, 26.1, 2, 0.3, 0.8, "BCOM_sn9_E4_299K_0_POS");
	
	
	cout <<"Vbd pre : " <<vbdPre.first <<endl;
	cout <<"Vbd post: " <<vbdPos.first <<endl;
	
	//I ratio at same overvoltage
	double overvg[4] = {0.5, 1.0, 1.5, 2.0};
	
	for (int i = 0; i != 4; ++i){
		if(i%2 == 0)
		{
			cout <<"Overvoltage: " <<overvg[i] <<", ratio: " <<gPos->Eval(26.92 + overvg[i])/gPre->Eval(26.86 + overvg[i]) <<endl;
		} else {
			cout <<"Overvoltage: " <<overvg[i] <<".0, ratio: " <<gPos->Eval(26.92 + overvg[i])/gPre->Eval(26.86 + overvg[i]) <<endl;	
		}
	}
	
	TH1F *hDivision = new TH1F ("hDivision", "hDivision", 201, -0.005, 2.005);
	for (int i = 1; i < 202; i += 2){
		hDivision->SetBinContent(i, gPos->Eval(26.92 + 0.01*(i-1))/gPre->Eval(26.86 + 0.01*(i-1)));
	}
	
	hDivision->SetMarkerStyle(kFullCircle);
	hDivision->SetMarkerSize(0.4);
	hDivision->SetMarkerColor(kBlue);
	hDivision->GetYaxis()->SetTitle("I post / I pre");
	hDivision->GetXaxis()->SetTitle("Overvoltage (V)");
	
	TCanvas *cRatio = new TCanvas();
	gPad->SetGridy();
	hDivision->Draw("P");
	cRatio->SaveAs("ratio.png");
	
}*/
