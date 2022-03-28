#include "list_diretta.h"
#include "list_inversa.h"
#include "IV.C"

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
    
    //grafica
    TMultiGraph *multiIV = new TMultiGraph();
    for (int i = 0; i != N_Temp; ++i){
        multiIV->Add(gIV[i]);
    }
    multiIV->SetTitle("Caratteristica IV - polarizzazione diretta; Voltaggio(V); Corrente(A)");
    
    TLegend *legend = new TLegend(0.1, 0.572, 0.35, 0.9);
    legend->SetNColumns(3);
    legend->AddEntry(gIV[0], "26.6 ^{o}C");
    legend->AddEntry(gIV[3], "37.1 ^{o}C");
    legend->AddEntry(gIV[6], "89.2 ^{o}C");
    legend->AddEntry(gIV[1], "27.7 ^{o}C");
    legend->AddEntry(gIV[4], "46.5 ^{o}C");
    legend->AddEntry(gIV[7], "121 ^{o}C");
    legend->AddEntry(gIV[2], "30.3 ^{o}C");
    legend->AddEntry(gIV[5], "67.4 ^{o}C");
    legend->AddEntry(gIV[8], "142 ^{o}C");
    
    TCanvas *cIV = new TCanvas("cIV", "cIV", 1800, 900);
    multiIV->Draw("AP");
    legend->Draw("SAME");
    
    cIV->SaveAs("IVdiretta.root");
    cIV->SaveAs("IVdiretta.png");
    delete cIV;
    
    //fit per trovare Vgamma e Rq
    float Vgamma[N_Temp];
    float Vgamma_e[N_Temp];
    float Rq[N_Temp];
    float Rq_e[N_Temp];
    
    for (int i = 0; i != N_Temp; ++i){
        gIV[i]->Fit("pol1", "RQ0", "", 0.70, 1);
        TF1 *fitfunc = gIV[i]->GetFunction("pol1");
        
        Vgamma[i] = (-1) * fitfunc->GetParameter(0)/fitfunc->GetParameter(1);
        Vgamma_e[i] = sqrt(pow(fitfunc->GetParError(0)/ fitfunc->GetParameter(1),2) + pow(Vgamma[i] * fitfunc->GetParError(1) / fitfunc->GetParameter(1), 2));
        
        Rq[i] = pow(fitfunc->GetParameter(1), -1);
        Rq_e[i] = (fitfunc->GetParError(1))/pow(fitfunc->GetParameter(1), 2);
        
        cout <<" R: " <<Rq[i] <<" +- " <<Rq_e[i] <<endl;
    }
    
    //V gamma vs T
    TGraphErrors *gVGamma = new TGraphErrors(N_Temp, temps, Vgamma, 0, Vgamma_e);
    gVGamma->SetName("gVGamma");
    gVGamma->SetTitle("");
    gVGamma->GetXaxis()->SetTitle("Temperature (^{o}C)");
    gVGamma->GetYaxis()->SetTitle("V_{#gamma} (V)");
    gVGamma->SetMarkerStyle(kCircle);
    gVGamma->SetMarkerColor(kBlue);
    gVGamma->SetMarkerSize(0.8);
    gVGamma->SetLineColor(kBlue);
    
    //Rq vs T
    TGraphErrors *gRq = new TGraphErrors(N_Temp, temps, Rq, 0, Rq_e);
    gRq->SetName("gRq");
    gRq->SetTitle("");
    gRq->GetXaxis()->SetTitle("Temperature (^{o}C)");
    gRq->GetYaxis()->SetTitle("R_{q} (Ohm)");
    gRq->SetMarkerStyle(kCircle);
    gRq->SetMarkerColor(kBlue);
    gRq->SetMarkerSize(0.8);
    gRq->SetLineColor(kBlue);
    
    TF1 *fitR = new TF1("fitR", "[0] + [1] * exp(-[2]*x)", 20, 150);
    fitR->SetParameter(0, 155);
    fitR->SetParameter(1, 90);
    fitR->SetParameter(2, 0.01);
    fitR->SetLineStyle(2);
    
    gRq->Fit(fitR,"MES");

    
    //grafica Vgamma vs T
    
    TCanvas *cGamma = new TCanvas("cGamma", "CGamma", 900, 900);
    gVGamma->Draw("APE");
    gVGamma->Fit("pol1", "Q");
    TF1 *fGamma = gVGamma->GetFunction("pol1");
    fGamma->SetLineColor(kRed);
    fGamma->SetLineStyle(2);
    fGamma->Draw("SAME");
    
    TLatex *latexV = new TLatex();
    latexV->DrawLatexNDC(.13,.2,Form("m = (%f +/- %f) V/ ^{o}C", fGamma->GetParameter(1), fGamma->GetParError(1)));
    
    
    cGamma->SaveAs("Vgamma.root");
    cGamma->SaveAs("Vgamma.png");
    delete cGamma;
    
    //grafica Rq vs T
    
    TCanvas *cRq = new TCanvas("cRq", "CRq", 900, 900);

    gRq->Draw("APE");
    
    
    TLatex *latex = new TLatex();
    latex->DrawLatexNDC(.4,.75,Form("R = %2.2f + %2.2f e^{-%.2f T}", fitR->GetParameter(0), fitR->GetParameter(1), fitR->GetParameter(2)));
    
    cRq->SaveAs("Resistance.root");
    cRq->SaveAs("Resistance.png");
    delete cRq;
    
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
	
	hDiretta->SetMarkerStyle(kFullCircle);
	hInversa->SetMarkerStyle(33);
	hInversa->SetMarkerColor(kBlue);
	hInversa->GetYaxis()->SetTitle("I dopo / I prima");
	hInversa->GetXaxis()->SetTitle("Voltaggio (V)");
    
	for(int i = 0; i != gaDiretta->GetN(); ++i){
		double yb = gIV[0]->GetPointY(i);
		double ya = gaDiretta->GetPointY(i);
		hDiretta->SetBinContent(2*i + 1, ya/yb);
	}
 
	for(int i = 0; i != gaInversa->GetN(); ++i){
		double yb = gbInversa->GetPointY(i);
		double ya = gaInversa->GetPointY(i);
		hInversa->SetBinContent(2*i + 1, ya/yb);
	}
	
	
	TCanvas *cDiretta = new TCanvas("cDiretta", "cDiretta", 1800, 900);
	hDiretta->Draw("P");
    cDiretta->SaveAs("cDiretta.root");
    delete cDiretta;
    
	TCanvas *cInversa = new TCanvas("cInversa", "Polarizzazione Inversa", 1800, 900);
	gPad->SetGridx();
	hInversa->Draw("P");
	cInversa->SaveAs("cInversa.png");
    delete cInversa;
	
    faInversa->Close();
    fbInversa->Close();
    faDiretta->Close();
    for (auto kCurrentFile : fInputFiles){
        kCurrentFile->Close();
    }
}


void analyseIV_tempscan_inv(){

    
    gStyle->SetOptTitle(1);
    int const N_Temp = 9;
    float temps[N_Temp] = {26.6, 27.7, 30.3, 37.1, 46.5, 67.4, 89.2, 125, 146};
    
    //lettura file
    std::vector<TString> fInputNames = list_inversa();
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
        gIV[i]->SetMarkerStyle(kCircle);
    }
    
 /*   //grafica
    TMultiGraph *multiIV = new TMultiGraph();
    for (int i = 0; i != N_Temp; ++i){
        multiIV->Add(gIV[i]);
    }
    multiIV->SetTitle("Caratteristica IV - polarizzazione inversa; Voltaggio(V); Corrente(A)");
    
    TLegend *legend = new TLegend(0.1, 0.572, 0.35, 0.9);
    legend->SetNColumns(3);
    legend->AddEntry(gIV[0], "26.6 ^{o}C");
    legend->AddEntry(gIV[3], "37.1 ^{o}C");
    legend->AddEntry(gIV[6], "89.2 ^{o}C");
    legend->AddEntry(gIV[1], "27.7 ^{o}C");
    legend->AddEntry(gIV[4], "46.5 ^{o}C");
    legend->AddEntry(gIV[7], "121 ^{o}C");
    legend->AddEntry(gIV[2], "30.3 ^{o}C");
    legend->AddEntry(gIV[5], "67.4 ^{o}C");
    legend->AddEntry(gIV[8], "142 ^{o}C");
    
    TCanvas *cIV = new TCanvas("cIV_inv", "cIV_inv", 1800, 900);
    gPad->SetLogy();
    multiIV->Draw("AP");
    legend->Draw("SAME");
    
    cIV->SaveAs("IVinversa.root");
    cIV->SaveAs("IVinversa.png");
    delete cIV;
*/
    //find VBD for each temperature
    auto klow = 0.5;
    auto khig = 0.7;
    double kgss[N_Temp] = { 26.6, 26.67, 26.7, 26.9, 26.8, 27.5, 28.1, 29.3, 29.3};
    TGraphErrors *g1 = new TGraphErrors();
    for (int i = 0; i != gIV.size()-1 ; ++i){
        auto vbd = fMeasureVbd(makemetgerrors(gIV[i]), kgss[i], 2, klow, khig, Form("BCOM_sn9_C2_%fK_0", temps[i] + 273));
        g1->SetPoint(i, temps[i], vbd.first);
        g1->SetPointError(i, 0, 0);
    }
    

    TCanvas *cVBDg = new TCanvas("cVBDg", "cVBDg", 900, 900);
    g1->SetMarkerStyle(kFullCircle);
    g1->SetMarkerColor(kBlue);
    g1->GetXaxis()->SetTitle("Temperature ^{o}C");
    g1->GetYaxis()->SetTitle("V_{BD} (V)");
    g1->Draw("APE");
    g1->Fit("pol1");
    TF1 *fitpol = g1->GetFunction("pol1");
    fitpol->SetLineStyle(7);
    fitpol->SetLineColor(kRed);
    fitpol->Draw("SAME");
    TLatex *latexVBD = new TLatex();
    latexVBD->DrawLatexNDC(.43,.29,Form("m = %.1f +/- %.1f (mV/ ^{o}C)", 1000 * fitpol->GetParameter(1), 1000 * fitpol->GetParError(1)) );
    latexVBD->DrawLatexNDC(.43,.19,"#chi^{2} = 0.019");
    
    cVBDg->SaveAs("cVBDg.png");
   
    
    for (auto kCurrentFile : fInputFiles){
        kCurrentFile->Close();
    }
}


void analyseIV_static(){
    
    gStyle->SetOptStat(00000);
    gStyle->SetOptTitle(0);
    //find VBD for each sensor
    std::vector<TString> kColumns = {"C","D","E","F","G","H" };
    std::vector<TFile*> kInputFiles;
    std::vector<TString> kNames;
    std::vector<TGraphErrors*> gIV;
    std::vector<std::pair<double, double>> VBD;
    
    for ( auto kCurrent_Column : kColumns ) {
        for ( int iRow = 1; iRow != 5; ++iRow ) {
            kInputFiles.push_back( new TFile( Form("/Users/giuliafazzino/Desktop/Tesi/SiPM_Analysis/21032022/inv%s%d.txt.root",kCurrent_Column.Data(), iRow) ) );
            kNames.push_back(Form("BCOM_sn9_%s%i_293K_0", kCurrent_Column.Data(), iRow));
        }
    }
    
    for (auto kCurrentFile : kInputFiles){
        gIV.push_back(makemetgerrors((TGraph*)(kCurrentFile->Get("gIV"))));
    }
    
    auto klow = 0.3;
    auto khig = 1.5;
    auto kgss = 26.4;
    
    for (int i = 0; i != gIV.size(); ++i){
        VBD.push_back(fMeasureVbd(gIV[i], kgss, 2, klow, khig, kNames[i]));
    }
    
    TH1F *hVBD = new TH1F("hVBD", "hVBD", 24, 0, 24);
    TH1F *hVBDtemp = new TH1F("hVBDtemp", "hVBDtemp", 201, 49.5, 250.5);
    std::array<TH1F*, 4> hRows;
    for (int i = 0; i != 4; ++i){
        hRows[i] = new TH1F(Form("hRow%d", i), "hRow", 201, 49.5, 250.5);
        hRows[i]->SetMarkerColorAlpha(13, 0.75);
        hRows[i]->SetLineColor(13);
        hRows[i]->SetMarkerStyle(20 + i);
        hRows[i]->SetMarkerSize(1.3);
    }
    
    for (int i = 0; i != kColumns.size(); ++i){
        TH1F *hTemp = new TH1F ("hTemp", "hTemp", 300, 26.7, 27);
        for (int j = 1; j != 5; ++j){
            hVBD->GetXaxis()->SetBinLabel(4*i+j, Form("%s%d", kColumns[i].Data(), j));
            hVBD->SetBinContent(4*i+j, VBD[4*i+j-1].first);
            hVBD->SetBinError(4*i+j, VBD[4*i+j-1].second);
            hTemp->Fill(VBD[4*i+j-1].first);
            hRows[j-1]->SetBinContent(201 - 50 - 20 * i, VBD[4*i+j-1].first);
            hRows[j-1]->SetBinError  (201 - 50 - 20 * i, VBD[4*i+j-1].second);
        }
        hVBDtemp->SetBinContent(201 - 50 - 20 * i, hTemp->GetMean());
        hVBDtemp->SetBinError(201 - 50 - 20 * i, hTemp->GetRMS());
        delete hTemp;
    }
    
    hVBD->GetXaxis()->SetTitle("Sensor");
    hVBD->GetYaxis()->SetTitle("V breakdown (V)");
    hVBD->GetYaxis()->SetRangeUser(26.7, 27.1);
    hVBD->SetMarkerStyle(kFullCircle);
    hVBD->SetLineColor(0);
    
    TF1 *dts_25 = new TF1("dts_25", "26.900", 0, 250);
    dts_25->SetLineColor(8);
    dts_25->SetLineStyle(7);
    
    TF1 *dts_26 = new TF1("dts_26", "26.926", 0, 250);
    dts_26->SetLineColor(8);
    dts_26->SetLineWidth(3);
    
    TF1 *dts_27 = new TF1("dts_27", "26.952", 0, 250);
    dts_27->SetLineColor(8);
    dts_27->SetLineStyle(7);

    
    TCanvas *cVBD = new TCanvas("CVBD", "CVBD", 1800, 900);
    hVBD->Draw();
    dts_26->Draw("SAME");
    cVBD->SaveAs("Vbreakdown.png");
    cVBD->SaveAs("Vbreakdown.root");
    delete cVBD;
    
    TLegend *legendTemp = new TLegend(0.1, 0.1, 0.9, 0.25);
    legendTemp->SetNColumns(5);
    legendTemp->AddEntry(hRows[0],"Column 1");
    legendTemp->AddEntry(hRows[1],"Column 2");
    legendTemp->AddEntry(hRows[2],"Column 3");
    legendTemp->AddEntry(hRows[3],"Column 4");
    legendTemp->AddEntry(hVBDtemp,"Mean");
    
    hVBDtemp->GetXaxis()->SetTitle("T max (^{o}C)");
    hVBDtemp->GetYaxis()->SetTitle("V breakdown (V)");
    hVBDtemp->SetMarkerStyle(kFullCircle);
    hVBDtemp->SetMarkerSize(1.3);
    hVBDtemp->SetMarkerColor(kBlue);
    hVBDtemp->SetLineColor(kBlue);
    hVBDtemp->GetXaxis()->SetRangeUser(95, 205);
    hVBDtemp->GetYaxis()->SetRangeUser(26.7, 27);
    
    TCanvas *cVBDtemp = new TCanvas("cVBDtemp", "cVBDtemp", 1800, 900);
    hVBDtemp->Draw();
    hVBDtemp->Fit("pol0", "Q");
    TF1 *fVBDtemp = hVBDtemp->GetFunction("pol0");
    fVBDtemp->SetLineColor(kRed);
    fVBDtemp->SetLineStyle(2);
    fVBDtemp->Draw("SAME");
    for (int i = 0; i != 4; ++i){
        hRows[i]->Draw("SAME");
    }
    dts_25->Draw("SAME, L");
    dts_26->Draw("SAME, L");
    dts_27->Draw("SAME, L");
    legendTemp->Draw("SAME");
    
    TLatex *latexVBD = new TLatex();
    latexVBD->DrawLatexNDC(.13,.29,Form("V_{BD} = %.2f", fVBDtemp->GetParameter(0)) );
    
    latexVBD->DrawLatexNDC(.73,.64,"DTS, T = 25 ^{o}C");
    
    latexVBD->DrawLatexNDC(.73,.71,"DTS, T = 26 ^{o}C");
    
    latexVBD->DrawLatexNDC(.73,.78,"DTS, T = 27 ^{o}C");
    
    cVBDtemp->SaveAs("VbdVST.png");
    
    delete cVBDtemp;
    
}
