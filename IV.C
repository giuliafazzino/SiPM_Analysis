#include "style.C"
#include "graphutils.C"

// errors from Ferrara
double belowI[6] = {2.e-9, 20.e-9, 200.e-9, 2.e-6, 20.e-6, 200.e-6};
double percI[6] = {0.003, 0.002, 0.0015, 0.0015, 0.001, 0.001};
double fixedI[6] = {400.e-15, 1.e-12, 10.e-12, 100.e-12, 1.e-9, 10.e-9};

double percV[1] = {0.001};
double fixedV[1] = {4.e-3};

double fRadiation[4] = {1.e+8,1.e+9,1.e+10,1.e+11};
const char* fLetters[8] = {"A","B","C","D","E","F","G","H"};

//>>    Keithley 2450 Specifics
//https://download.tek.com/manual/SPEC-2450B_November_2017.pdf
double fVoltageRange    [5] =   { 20*1.e-3,     200*1.e-3,  2.0,        20.0,       200.0       };
double fVoltagePrcnt    [5] =   { 1.e-3,        .15*1.e-3,  .2*1.e-3,   .15*1.e-3,  .15*1.e-3   };
double fVoltageOffSt    [5] =   { 200*1.e-6,    200*1.e-6,  300*1.e-6,  2.4*1.e-3,  24*1.e-3    };
/*
 Temperature coefficient: ± (0.15 × accuracy specification)/°C
 0 °C to 18 °C and 28 °C to 50 °C
 */

double fCurrentRange    [9] =   { 10*1.e-9,     100*1.e-9,  1.e-6,      10.e-6,     100.e-6,    1.e-3,      10.e-3,     100.e-3,    1.,         };
double fCurrentPrcnt    [9] =   { 1.e-3,        .6*1.e-3,   .25*1.e-3,  .25*1.e-3,  .20*1.e-3,  .20*1.e-3,  .20*1.e-3,  .25*1.e-3,  .67*1.e-3   };
double fCurrentOffSt    [9] =   { 100*1.e-12,   150*1.e-12, 400*1.e-12, 1.5*1.e-9,  15*1.e-9,   150*1.e-9,  1.5*1.e-6,  15*1.e-6,   900*1.e-6   };
/*
 Temperature coefficient: ± (0.15 × accuracy specification)/°C
0 °C to 18 °C and 28 °C to 50 °C
*/

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


TGraphErrors * makemetgerrors ( TGraph* gTarget) {
    auto fResult = new TGraphErrors();
    for ( auto i = 0; i < gTarget->GetN(); i++ ) {
        fResult->SetPoint(i, gTarget->GetPointX(i), gTarget->GetPointY(i));
    }
    return fResult;
}

TGraphErrors *
makeiv(const std::string fnivscan, const std::string fnzero, bool invertX = true, bool invertY = true)
{

  /** compute zero volt level **/
  auto zero = 0.;
  auto ezero = 0.;
  if (!fnzero.empty()) {
    std::cout << " --- open zero volt level file: " << fnzero << std::endl;
    auto gzero = new TGraph(fnzero.c_str(), "%lg %*lg %lg");
    gzero = invertY ? graphutils::invertY(gzero) : gzero;
    auto pzero = new TProfile(Form("pzero_%s",fnivscan.c_str()), "", 1, 0., 1.);
    for (int i = 0; i < gzero->GetN(); ++i)
      pzero->Fill(0., gzero->GetY()[i]);
    zero = pzero->GetBinContent(1);
    ezero = pzero->GetBinError(1);
  }
  std::cout << " --- zero volt level: " << zero * 1.e12 << " +- " << ezero * 1.e12 << " pA" << std::endl;

  /** IV curve from average of all measurements **/
  std::cout << " --- open iv scan file: " << fnivscan << std::endl;
  auto givscan = new TGraph(fnivscan.c_str(), "%*lg %lg %lg");
  givscan = invertX ? graphutils::invertX(givscan) : givscan;
  givscan = invertY ? graphutils::invertY(givscan) : givscan;
  auto pivscan = new TProfile(Form("pivscan_%s",fnivscan.c_str()), "", 10001, -0.005, 100.005);
  for (int i = 0; i < givscan->GetN(); ++i)
    pivscan->Fill(givscan->GetX()[i], givscan->GetY()[i]);

  /** write as TH1 and TGraphErrors **/
  auto hivscan = new TH1F(Form("hivscan_%s",fnivscan.c_str()), "", 10001, -0.005, 100.005);
  auto gout = new TGraphErrors;
  auto npoints = 0;
  for (int i = 0; i < pivscan->GetNbinsX(); ++i) {
    if (pivscan->GetBinError(i + 1) == 0.) continue;
    auto cen = pivscan->GetBinCenter(i + 1);
    auto val = pivscan->GetBinContent(i + 1);
    auto eval = pivscan->GetBinError(i + 1);
    val = val - zero;
    eval = std::sqrt(eval * eval + ezero * ezero);
    gout->SetPoint(npoints, cen, val);
    gout->SetPointError(npoints, 0., eval);
    hivscan->SetBinContent(i + 1, val);
    hivscan->SetBinError(i + 1, eval);
    npoints++;
  }
  auto lastindex = fnivscan.find_last_of(".");
  auto fnout = fnivscan.substr(0, lastindex) + ".makeiv.root";
  std::cout << " --- writing output file: " << fnout << std::endl;
  auto fout = TFile::Open(fnout.c_str(), "RECREATE");
  gout->Write("ivscan");
  hivscan->Write("hivscan");
  
  fout->Close();
  return gout;
}
TGraphErrors *
makeiv ( TString fSensorName, Int_t fTemperature, TString fLetter, Int_t iTer )  {
    //
    // - - - - -
    //
    TString fSensorName_Actual;
    if ( fSensorName.Contains("_2") )   {
        fSensorName_Actual = fSensorName.Strip(TString::EStripType::kTrailing,'2');
        fSensorName_Actual = fSensorName_Actual.Strip(TString::EStripType::kTrailing,'_');
    } else if ( fSensorName.Contains("_3") )   {
        fSensorName_Actual = fSensorName.Strip(TString::EStripType::kTrailing,'3');
        fSensorName_Actual = fSensorName_Actual.Strip(TString::EStripType::kTrailing,'_');
    } else if ( fSensorName.Contains("_4") )   {
        fSensorName_Actual = fSensorName.Strip(TString::EStripType::kTrailing,'4');
        fSensorName_Actual = fSensorName_Actual.Strip(TString::EStripType::kTrailing,'_');
    } else  {
        fSensorName_Actual = fSensorName;
    }
    //
    // - - - - -
    //
    TFile  *fCheckAlreadyDone   =   new TFile(Form("./Data/%s/%iK/csv/ivscan/%s_%iK_%s%i.ivscan.makeiv.root",fSensorName.Data(),fTemperature,fSensorName_Actual.Data(),fTemperature,fLetter.Data(),iTer));
    if ( fCheckAlreadyDone->IsZombie() )   return makeiv(Form("./Data/%s/%iK/csv/ivscan/%s_%iK_%s%i.ivscan.csv",fSensorName.Data(),fTemperature,fSensorName_Actual.Data(),fTemperature,fLetter.Data(),iTer),Form("./Data/%s/%iK/csv/zero/%s_%iK_%s%i.zero.csv",fSensorName.Data(),fTemperature,fSensorName_Actual.Data(),fTemperature,fLetter.Data(),iTer));
    else                        return (TGraphErrors*)(fCheckAlreadyDone->Get("ivscan"));
}

TGraphErrors *IV(const char *fname, bool invert = false)
{
  auto gIV = new TGraphErrors(fname);

  // add errors
  for (int i = 0; i < gIV->GetN() ; ++i) {

    double errorI = 0., errorV = 0.;
    
    // error on I
    for (int j = 0; j < 6; ++j) {
      if (gIV->GetY()[i] < belowI[j]) {
    errorI = percI[j] * gIV->GetY()[i] + fixedI[j];
    break;
      }
    }
    // error on V
    errorV = percV[0] * gIV->GetX()[i] + fixedV[0];
    
    gIV->SetPointError(i, errorV, errorI);
    
  }

  if (invert) {
    for (int i = 0; i < gIV->GetN() ; ++i) {
      gIV->GetX()[i] = -gIV->GetX()[i];
      gIV->GetY()[i] = -gIV->GetY()[i];
    }
  }
  return gIV;
}

TGraphErrors*
fSetErrorsOnIV          ( TGraphErrors *fTarget,    Double_t fScale =   1. )            {
    if ( !fTarget ) return nullptr;
    //>>  Looping over points
    for ( Int_t iPnt = 0; iPnt < fTarget->GetN(); ++iPnt ) {
        double fCurrentError = 0., fVoltageError = 0.;
        //
        //>>    Current Error evaluation
        for ( Int_t iRng = 0; iRng < 6; ++iRng ) {
            if ( fTarget->GetY()[iPnt] < belowI[iRng]) {
                fCurrentError = percI[iRng] * fTarget->GetY()[iPnt] + fixedI[iRng];
                break;
            }
        }
        //
        //>>    Voltage Error evaluation
        fVoltageError = percV[0] * fTarget->GetX()[iPnt] + fixedV[0];
        //
        //>>    Setting Errors
        fTarget ->  SetPointError( iPnt, fVoltageError, fCurrentError );
        //
        //>>    Rescaling points
        fTarget ->  GetX()[iPnt] = fScale * fTarget->GetX()[iPnt];
        fTarget ->  GetY()[iPnt] = fScale * fTarget->GetY()[iPnt];
    }
    return fTarget;
}
TGraphErrors*
fSetErrorsOnIV          ( TString fTargetPath,    Double_t fScale =   1. )            {    return fSetErrorsOnIV(new TGraphErrors(fTargetPath.Data(),"%*lg %lg %lg"),fScale); }

TGraphErrors *sqrtIV(const TGraphErrors *gIV);
TGraphErrors *sqrtIV(const char *fname) { return sqrtIV(IV(fname)); }

TGraphErrors *D(const TGraphErrors *gIV);
TGraphErrors *D(const char *fname)  { return D(IV(fname)); }

TGraphErrors *LD(const TGraphErrors *gIV);
TGraphErrors *LD(const char *fname)  { return LD(IV(fname)); }

TGraphErrors *ILD(const TGraphErrors *gIV);
TGraphErrors *ILD(const char *fname) { return ILD(IV(fname)); }

double Vbd(const TGraphErrors *gIV, int method = 3, double vbd_ini = 24.5);
double Vbd(const char *fname, int method = 3, double vbd_ini = 24.5) { return Vbd(IV(fname), method, vbd_ini); }

std::pair<double,double> Rq(TGraphErrors *gIV, double fitmin, double fitmax);
std::pair<double,double> Rq(const char *fname, double fitmin = 2, double fitmax = 3) { return Rq(IV(fname, true), fitmin, fitmax); }

TGraphErrors *sqrtIV(const TGraphErrors *gIV) {
  auto gsqrtIV = new TGraphErrors;
  for (int i = 0; i < gIV->GetN() ; ++i) {
    auto x = gIV->GetX()[i];
    auto y = std::sqrt(gIV->GetY()[i]);
    gsqrtIV->SetPoint(i, x, y);
  }
  return gsqrtIV;  
}

std::pair<double,double> Rq(TGraphErrors *gIV, double fitmin, double fitmax)
{
  gIV->Draw("ap*");
  auto pol1 = (TF1 *)gROOT->GetFunction("pol1");
  gIV->Fit(pol1, "", "", fitmin, fitmax);
  auto R1 = pol1->GetParameter(1);
  auto eR1 = pol1->GetParError(1);
  auto R = 1. / R1;
  auto eR = 1. / R1 / R1 * eR1;
  return std::pair<double,double>(R, eR);
}

TGraphErrors *D(const TGraphErrors *gIV) {
  auto gD = new TGraphErrors;
  for (int i = 0; i < gIV->GetN() - 1; ++i) {
    auto xmin = gIV->GetX()[i];
    auto xmax = gIV->GetX()[i + 1];
    auto ymin = gIV->GetY()[i];
    auto ymax = gIV->GetY()[i + 1];
    auto x = 0.5 * (xmax + xmin);
    auto y = (ymax - ymin) / (xmax - xmin);
    gD->SetPoint(i, x, y);
    gD->SetPoint(i, x, y);
  }
  return gD;
}

TGraphErrors *LD(const TGraphErrors *gIV) {
  auto gLD = new TGraphErrors;
  for (int i = 0; i < gIV->GetN() - 1; ++i) {
    auto xmin = gIV->GetX()[i];
    auto xmax = gIV->GetX()[i + 1];
    auto exmin = gIV->GetEX()[i];
    auto exmax = gIV->GetEX()[i + 1];
    auto ymin = std::log(gIV->GetY()[i]);
    auto ymax = std::log(gIV->GetY()[i + 1]);
    auto eymin = gIV->GetEY()[i] / gIV->GetY()[i]; 
    auto eymax = gIV->GetEY()[i + 1] / gIV->GetY()[i + 1];
    auto x = 0.5 * (xmax + xmin);
    auto ex = 0.5 * std::sqrt(exmin * exmin + exmax * exmax);
    auto y = (ymax - ymin) / (xmax - xmin);
    auto ey = std::sqrt(eymax * eymax + eymin * eymin) / (xmax - xmin);
    gLD->SetPoint(i, x, y);
    gLD->SetPointError(i, ex, ey);
  }
  return gLD;
}

TGraphErrors *ILD(const TGraphErrors *gIV) {
  auto gLD = LD(gIV);
  auto gILD = new TGraphErrors;
  for (int i = 0; i < gLD->GetN(); ++i) {
    auto x = gLD->GetX()[i];
    auto y = 1. / gLD->GetY()[i];
    auto ey = y*y*gLD->GetEY()[i];
    auto ex = gLD->GetEX()[i];
      gILD->SetPoint(i, x, y);
      gILD->SetPointError(i, ex, ey);
  }
  return gILD;
}

TGraphErrors*
fEvaluateErrors     ( TGraphErrors *gIV )   {
    TGraphErrors   *fResult = new TGraphErrors();
    std::vector<Double_t> fBuffer;
    for ( Int_t iPnt = 0; iPnt < gIV->GetN(); iPnt++ )  {
        Double_t    fXpoint =   gIV->GetX()[iPnt];
        Double_t    fYpoint =   gIV->GetY()[iPnt];
        if ( fXpoint == gIV->GetX()[iPnt+1] ) {
            fBuffer.push_back(fYpoint);
        }   else    {
            Double_t    fMean_ = 0.;
            Double_t    fStDev = 0.;
            Int_t    nPoints =   0;
            for ( auto iVal : fBuffer )  {
                fMean_  +=   iVal;
                nPoints++;
            }
            fMean_ /= nPoints*1.;
            for ( auto iVal : fBuffer )  {
                fStDev  +=   (iVal - fMean_)*(iVal - fMean_);
            }
            fStDev  /= (nPoints);
            fBuffer.clear();
            auto fiPnt = fResult->GetN();
            fResult->SetPoint(fiPnt,fXpoint,fMean_);
            fResult->SetPointError(fiPnt,0,TMath::Sqrt(fStDev));
        }
    }
    return fResult;
}

TGraphErrors*
fEvaluateErrors     ( const char *fname )   {
    return fEvaluateErrors(new TGraphErrors(fname,"%*lg %lg %lg"));
}


TF1 * fMeasVbd = new TF1("fMeasVbd","[0]*(x-[1])");
TF1 * fMeasVb2 = new TF1("fMeasVb2","[0]*pow(x-[1],[2])");
TF1 * fMeasRqn = new TF1("fMeasRqn","[0]+x/[1]");
TF1 * fBaseline= new TF1("fBaseline","[0]*x+[1]");
TF1 * fMeasCurr= new TF1("fMeasCurr","[0]*pow(x,[1])");

std::pair<Double_t,Double_t>
fMeasureVbd ( TGraphErrors *gIV, Double_t fGuessVbd = 25, Int_t fMode = 0, Double_t fMnusRng = 1, Double_t fPlusRng = 4, TString fName = "" )  {
    
    TString fSensor = "UNK";
    if ( fName.Contains("FBK") ) fSensor = "FBK";
    TString fSerial = "_snX";
    for ( Int_t iSer = 0; iSer < 10; iSer++ )   {
        if ( fName.Contains(Form("_sn%i",iSer)) ) fSerial = Form("_sn%i",iSer);
    }
    TString fStep = "_0";
    for ( Int_t iStp = 0; iStp < 10; iStp++ )   {
        if ( fName.Contains(Form("_%i_",iStp)) ) fStep = Form("_%i",iStp);
    }
    
    TString fFolder = Form("./Data/plots/VBD/%s%s%s/",fSensor.Data(),fSerial.Data(),fStep.Data());
    gROOT->ProcessLine( Form(".! mkdir -p ./%s",fFolder.Data()) );
    
    
    TGraphErrors*   gILD;
    TCanvas*        c1;
    TF1*            fFitresult;
    auto fTolerance = 1.e-5;
    auto fIncr = -.0001;
    auto m = 0.;
    auto q = 0.;
    auto T0= 0.;
    m = 0.026;
    q = 26.9;
    T0= 298;
    if ( fName.Contains("BCOM") ) {
        m = 0.026;
        q = 26.9;
        T0= 298;
    }   if ( fName.Contains("FBK") ) {
        m = 0.035;
        q = 32;
        T0= 297;
    }
    auto temp = 0.;
    if ( fName.Contains("243K") ) temp = 243;
    if ( fName.Contains("263K") ) temp = 263;
    if ( fName.Contains("293K") ) temp = 293;
    auto TargetTemp = (temp-T0)*m+q;
    TLatex * ftext = new TLatex();
    TGraph * fTemperatureTarget = new TGraph();
    fTemperatureTarget->SetLineColor(kBlue);
    fTemperatureTarget->SetLineStyle(kDashed);
    fTemperatureTarget->SetLineWidth(3);
    for ( Int_t i = 0; i < 10; i++ )   {
        auto fYvalue = 0.1 + i*1.e-1;
        fTemperatureTarget->SetPoint(i,TargetTemp,fYvalue);
    }
    switch ( fMode )    {
            // Fit of IV with (x-x0)^n
        case 2: {
            gILD = (gIV);
            c1 = new TCanvas("","",800,800);
            c1->SetLogy();
            gILD->SetLineColor(kBlack);
            gILD->SetMarkerStyle(20);
            fBaseline->FixParameter(0,0);
            fBaseline->SetParameter(1,0);
            gILD->Fit("fBaseline","MRS","Q",fGuessVbd-fPlusRng,fGuessVbd-fMnusRng);
            fBaseline->ReleaseParameter(0);
            fBaseline->ReleaseParameter(1);
            gILD->Fit("fBaseline","MRS","Q",fGuessVbd-fPlusRng,fGuessVbd-fMnusRng);
            fMeasVb2->FixParameter(1,fGuessVbd);
            fMeasVb2->FixParameter(2,2);
            auto fpowerLaw =  gILD->Fit("fMeasVb2","MRS","Q",fGuessVbd+fMnusRng,fGuessVbd+fPlusRng);
            fMeasVb2->ReleaseParameter(1);
            fMeasVb2->ReleaseParameter(2);
            fMeasVb2->SetParLimits(2,0.,1000.);
            auto fBaseLine = gILD->Fit("fBaseline","MRS","Q",fGuessVbd-fPlusRng,fGuessVbd-fMnusRng);
            auto _CurrentX = fGuessVbd + 3;
            for ( Int_t iTer = 0; iTer < 100; iTer++ )  {
                _CurrentX = fGuessVbd + 3;
                for ( Int_t iTer = 0; iTer < 150000; iTer++ )  {
                    auto x1 = fabs(fBaseline ->Eval(_CurrentX));
                    auto x2 = fabs(fMeasVb2  ->Eval(_CurrentX));
                    if ( x2 < x1 ) break;
                    _CurrentX += fIncr;
                }
                if ( fabs(fGuessVbd - _CurrentX ) < fTolerance ) break;
                fGuessVbd = _CurrentX;
                fBaseline->ReleaseParameter(0);
                fBaseline->ReleaseParameter(1);
                fMeasVb2->ReleaseParameter(0);
                fMeasVb2->ReleaseParameter(1);
                fMeasVb2->ReleaseParameter(2);
                fpowerLaw = gILD->Fit("fMeasVb2",   "MRS",  "Q", fGuessVbd,fGuessVbd+fPlusRng);
                fBaseLine = gILD->Fit("fBaseline",  "MRS",  "Q", fGuessVbd-fPlusRng,fGuessVbd-fMnusRng);
            }
            // ERROR Evaluation
            
            Double_t fMeasuredVbdPnt[1]     =   {fGuessVbd};
            
            Double_t fEvaluatePowerLaw[1]   =   {fMeasVb2->Eval(fGuessVbd)};
            Double_t fErrorOnPowerLaw[1];
            fpowerLaw->GetConfidenceIntervals(1,1,1,fMeasuredVbdPnt,fErrorOnPowerLaw,0.683,false);
            
            Double_t fEvaluateBaseLine[1]   =   {fMeasVb2->Eval(fGuessVbd)};
            Double_t fErrorOnBaseLine[1];
            fBaseLine->GetConfidenceIntervals(1,1,1,fMeasuredVbdPnt,fErrorOnBaseLine,0.683,false);
            
            auto fPar0 = fBaseline->GetParameter(0);
            auto fPEr0 = fBaseline->GetParError(0);
            auto fPar1 = fBaseline->GetParameter(1);
            auto fPEr1 = fBaseline->GetParError(1);
            
            fBaseline->SetParameter(0,fPar0+fPEr0);
            fBaseline->SetParameter(1,fPar1-fPEr1);
            _CurrentX = fGuessVbd + 3;
            for ( Int_t iTer = 0; iTer < 150000; iTer++ )  {
                auto x1 = fabs(fBaseline ->Eval(_CurrentX));
                auto x2 = fabs(fMeasVb2  ->Eval(_CurrentX));
                if ( x2 < x1 ) break;
                _CurrentX += fIncr;
            }
            auto fHighError = _CurrentX;
            
            fBaseline->SetParameter(0,fPar0-fPEr0);
            fBaseline->SetParameter(1,fPar1+fPEr1);
            _CurrentX = fGuessVbd + 3;
            for ( Int_t iTer = 0; iTer < 150000; iTer++ )  {
                auto x1 = fabs(fBaseline ->Eval(_CurrentX));
                auto x2 = fabs(fMeasVb2  ->Eval(_CurrentX));
                if ( x2 < x1 ) break;
                _CurrentX += fIncr;
            }
            auto fLow_Error = _CurrentX;
            
            fBaseline->SetParameter(0,fPar0);
            fBaseline->SetParameter(1,fPar1);
            auto fError = 0.5*fabs(fHighError-fLow_Error);
            
            //
            gILD->GetXaxis()->SetTitle("V_{bias} (V)");
            gILD->GetYaxis()->SetTitle("I (A)");
            gILD->GetYaxis()->SetTitleOffset(1.);
            gILD->Draw("ape");
            fBaseline->SetRange(0,60);
            fBaseline->DrawCopy("SAME");
            fMeasVb2->SetRange(fMeasVb2->GetParameter(1),100);
            fMeasVb2->Draw("SAME");
            c1->SaveAs(Form("%s/CHFit_%s.pdf",fFolder.Data(),fName.Data()));
            // Close UP
            gILD->GetXaxis()->SetRangeUser(fGuessVbd-3,fGuessVbd+3);
            gILD->SetMaximum(fMeasVb2->Eval(fGuessVbd+3.5));
            ftext->DrawLatexNDC(0.2,0.8,fName.Data());
            ftext->DrawLatexNDC(0.2,0.7,Form("Vbd = %.3f +- %.3f",fGuessVbd,fError));
            ftext->DrawLatexNDC(0.2,0.6,Form("DTS = %.3f",TargetTemp));
            for ( Int_t i = 0; i < 1000000; i++ )   {
                auto fYvalue = i*1.e-12;
                if ( (fMeasVb2->Eval(fGuessVbd+1)) < fYvalue ) break;
                fTemperatureTarget->SetPoint(i,TargetTemp,fYvalue);
            }
            fTemperatureTarget->Draw("same L ");
            fBaseline->DrawCopy("SAME");
            fBaseline->SetParameter(0,fPar0+fPEr0);
            fBaseline->SetParameter(1,fPar1-fPEr1);
            fBaseline->DrawCopy("SAME");
            fBaseline->SetParameter(0,fPar0-fPEr0);
            fBaseline->SetParameter(1,fPar1+fPEr1);
            fBaseline->DrawCopy("SAME");
            c1->SaveAs(Form("%s/CHFit_%s_CloseUp.pdf",fFolder.Data(),fName.Data()));
            gILD->SetMaximum(fBaseline->Eval(fGuessVbd+3.5));
            c1->SetLogy(false);
            c1->SaveAs(Form("%s/CHFit_%s_CloseUpNoLog.pdf",fFolder.Data(),fName.Data()));
            delete c1;
            return std::pair<Double_t,Double_t> (fGuessVbd,fError);
        break;
        }
            // Linear fit of ILD(gIV)
        default: {
            gILD = ILD(gIV);
            c1 = new TCanvas("","",800,800);
            gILD->SetMinimum(0.);
            gILD->SetMaximum(5.);
            gILD->SetLineColor(kBlack);
            gILD->SetMarkerStyle(20);
            gILD->Fit("fMeasVbd","MRS","",fGuessVbd+fMnusRng,fGuessVbd+fPlusRng);
            fFitresult = gILD->GetFunction("fMeasVbd");
            for ( Int_t iTer = 0; iTer < 100; iTer++ )  {
                if ( fabs(fGuessVbd - fFitresult->GetParameter(1)) < fTolerance ) break;
                fGuessVbd = fFitresult->GetParameter(1);
                gILD->Fit("fMeasVbd","MRS","",fGuessVbd+fMnusRng,fGuessVbd+fPlusRng);
                fFitresult = gILD->GetFunction("fMeasVbd");
            }
            fFitresult->SetRange(25.,30.);
            fFitresult->Draw("same");
            gILD->GetXaxis()->SetTitle("V_{bias} (V)");
            gILD->GetYaxis()->SetTitle("Inverse Log Derivative");
            gILD->Draw("ape");
            ftext->DrawLatexNDC(0.2,0.8,fName.Data());
            ftext->DrawLatexNDC(0.2,0.7,Form("Vbd = %.3f",fGuessVbd));
            ftext->DrawLatexNDC(0.2,0.6,Form("DTS = %.3f",TargetTemp));
            fTemperatureTarget->Draw("same L ");
            c1->SaveAs(Form("./Data/plots/CHFit_1_%s.pdf",fName.Data()));
            //delete c1;
            return std::pair<Double_t,Double_t> (fMeasVbd->GetParameter(1),fMeasVbd->GetParError(1));
        break;
        }
    }
}
std::pair<Double_t,Double_t>
fMeasureVbd ( TString fFileName, Double_t fGuessVbd = 25, Int_t fMode = 0, Double_t fMnusRng = 1, Double_t fPlusRng = 4, TString fName = "" )  {
    auto target = makeiv((fFileName+TString(".ivscan.csv")).Data(),(fFileName+TString(".zero.csv")).Data()) ;
    return fMeasureVbd( target ,fGuessVbd,fMode,fMnusRng,fPlusRng,fName);
}
std::pair<Double_t,Double_t>
fMeasureVbd ( TString fSensorName, Int_t fTemperature, TString fLetter, Int_t iTer, Double_t fGuessVbd, Int_t fMode = 0, Double_t fMnusRng = 1, Double_t fPlusRng = 4 )  {
    TString fSensorName_Actual;
    if ( fSensorName.Contains("_2") )   {
        fSensorName_Actual = fSensorName.Strip(TString::EStripType::kTrailing,'2');
        fSensorName_Actual = fSensorName_Actual.Strip(TString::EStripType::kTrailing,'_');
    } else if ( fSensorName.Contains("_3") )   {
        fSensorName_Actual = fSensorName.Strip(TString::EStripType::kTrailing,'3');
        fSensorName_Actual = fSensorName_Actual.Strip(TString::EStripType::kTrailing,'_');
    } else {
        fSensorName_Actual = fSensorName;
    }
    return fMeasureVbd(makeiv(fSensorName.Data(),fTemperature,fLetter.Data(),iTer),fGuessVbd,fMode,fMnusRng,fPlusRng,Form("%s_%iK_%s%i",fSensorName.Data(),fTemperature,fLetter.Data(),iTer));
}
std::pair<TH1F*,TH1F*>
fMakeVbdDistribution    ( TString fSensor, Int_t fTemperature = 293, Int_t fMode = 0, Int_t fIfFBKType = 0, Int_t fColumnOnly = 0  )  {
    // Letter array
    const char* fLetters[8] = {"A","B","C","D","E","F","G","H"};
    //
    // Out histos
    TH1F * hVbd_measure         = new TH1F(Form("hVbd_measure_%i",fTemperature),Form("%s_%iK_*",fSensor.Data(),fTemperature),2000,20,40);
    TH1F * hVbd_measure_error   = new TH1F(Form("hVbd_measure_error_%i",fTemperature),Form("%s_%iK_err",fSensor.Data(),fTemperature),1000,0,1);
    
    for ( Int_t iLtr = 0; iLtr < 8; iLtr++ )    {
        if ( iLtr >= 6 && fSensor.Contains("FBK") ) break;
        for ( Int_t iTer = 1; iTer <= 4; iTer++ )    {
            auto fInitialGuess      =   26;
            if ( fSensor.Contains("FBK") ) fInitialGuess      =   31;
            if ( fSensor.Contains("FBK") && fIfFBKType == 1 )   {
                if ( iLtr == 0 || iLtr == 2 || iLtr == 4 ) continue;
            }
            if ( fSensor.Contains("FBK") && fIfFBKType == 0 )   {
                if ( iLtr == 1 || iLtr == 3 || iLtr == 5 ) continue;
            }
            if ( fColumnOnly > 0 )    {
                if ( iTer != fColumnOnly ) continue;
            }
            auto fVbd               =   fMeasureVbd(fSensor,fTemperature,fLetters[iLtr],iTer,fInitialGuess,fMode);
            hVbd_measure->Fill(         fVbd.first      );
            hVbd_measure_error->Fill(   fVbd.second     );
        }
    }
    return std::pair<TH1F*,TH1F*> (hVbd_measure,hVbd_measure_error);
}
std::pair<std::vector<float>,std::vector<float>>
fGetVbdDistribution    ( TString fSensor, Int_t fTemperature = 293, Int_t fMode = 0, Int_t fIfFBKType = 0, Int_t fColumnOnly = 0  )  {
    // Letter array
    const char* fLetters[8] = {"A","B","C","D","E","F","G","H"};
    //
    // Out histos
    std::vector<float> hVbd_measure        ;
    std::vector<float> hVbd_measure_error  ;
    
    for ( Int_t iLtr = 0; iLtr < 8; iLtr++ )    {
        if ( iLtr >= 6 && fSensor.Contains("FBK") ) break;
        for ( Int_t iTer = 1; iTer <= 4; iTer++ )    {
            auto fInitialGuess      =   26;
            if ( fSensor.Contains("FBK") ) fInitialGuess      =   32;
            if ( fSensor.Contains("FBK") && fIfFBKType == 1 )   {
                if ( iLtr == 0 || iLtr == 2 || iLtr == 4 ) continue;
            }
            if ( fSensor.Contains("FBK") && fIfFBKType == 0 )   {
                if ( iLtr == 1 || iLtr == 3 || iLtr == 5 ) continue;
            }
            if ( fColumnOnly > 0 )    {
                if ( iTer != fColumnOnly ) continue;
            }
            auto fVbd               =   fMeasureVbd(fSensor,fTemperature,fLetters[iLtr],iTer,fInitialGuess,fMode);
            hVbd_measure.push_back(         fVbd.first      );
            hVbd_measure_error.push_back(   fVbd.second     );
        }
    }
    return std::pair<std::vector<float>,std::vector<float>> (hVbd_measure,hVbd_measure_error);
}
void
fMakeVbdvsTemp          ( TString fSensor ) {
    std::vector<pair<TH1F*,TH1F*>> fAverageVbdAtT_m2;
    std::vector<pair<TH1F*,TH1F*>> fAverageVbdAtT_m1;
    Int_t Temperatures[3] = {243,263,293};
    
    gROOT->SetBatch(true);
    for ( Int_t iTer = 0; iTer < 3; iTer++ )  {
        fAverageVbdAtT_m1.push_back(fMakeVbdDistribution("FBK_sn3",Temperatures[iTer],2));
        fAverageVbdAtT_m2.push_back(fMakeVbdDistribution("FBK_sn3",Temperatures[iTer],2));
    }
    gROOT->SetBatch(false);
    
    TGraphErrors   *gVbd_vs_Temp_m1    =   new TGraphErrors();
    gVbd_vs_Temp_m1->SetMarkerStyle(22);
    gVbd_vs_Temp_m1->SetMarkerColor(kGreen-2);
    
    TGraphErrors   *gVbd_vs_Temp_m2    =   new TGraphErrors();
    gVbd_vs_Temp_m2->SetMarkerStyle(22);
    gVbd_vs_Temp_m2->SetMarkerColor(kBlue);

    TGraphErrors   *gVbd_vs_Temp_DTSheet    =   new TGraphErrors();
    gVbd_vs_Temp_DTSheet->SetLineStyle(3);
    gVbd_vs_Temp_DTSheet->SetLineWidth(3);
    gVbd_vs_Temp_DTSheet->SetLineColor(kBlue);
    
    for ( auto fAverageVbdAtTPlot : fAverageVbdAtT_m1 )    {
        auto iPnt = gVbd_vs_Temp_m1->GetN();
        auto fYvalue = fAverageVbdAtTPlot.first->GetMean();
        auto fYerror = fAverageVbdAtTPlot.first->GetMeanError();
        TCanvas *c1 = new TCanvas();
        fAverageVbdAtTPlot.first->Draw();
        c1->SaveAs(Form("%s_Vbd_temp_%i.pdf",fSensor.Data(),Temperatures[iPnt]));
        gVbd_vs_Temp_m1->SetPoint(iPnt,Temperatures[iPnt],fYvalue);
        gVbd_vs_Temp_m1->SetPointError(iPnt,0,fYerror);
        cout << "FBK_sn3 " << Temperatures[iPnt]<< "K " << Form("%.3f +- %.3f",fYvalue,fYerror) << endl;
    }
    
    for ( auto fAverageVbdAtTPlot : fAverageVbdAtT_m2 )    {
        auto iPnt = gVbd_vs_Temp_m2->GetN();
        auto fYvalue = fAverageVbdAtTPlot.first->GetMean();
        auto fYerror = fAverageVbdAtTPlot.first->GetMeanError();
        TCanvas *c1 = new TCanvas();
        fAverageVbdAtTPlot.first->Draw();
        c1->SaveAs(Form("%s_Vbd_temp_%i.pdf",fSensor.Data(),Temperatures[iPnt]));
        gVbd_vs_Temp_m2->SetPoint(iPnt,Temperatures[iPnt],fYvalue);
        gVbd_vs_Temp_m2->SetPointError(iPnt,0,fYerror);
        cout << "FBK_sn4 " << Temperatures[iPnt]<< "K " << Form("%.3f +- %.3f",fYvalue,fYerror) << endl;
        
        //--//--//--//
        
        auto m = 0.;
        auto q = 0.;
        auto T0= 0.;
        if ( fSensor.Contains("BCOM") ) {
            m = 0.026;
            q = 26.9;
            T0= 298;
        }   if ( fSensor.Contains("FBK") ) {
            m = 0.035;
            q = 32;
            T0= 297;
        }
        gVbd_vs_Temp_DTSheet->SetPoint(iPnt,Temperatures[iPnt],(Temperatures[iPnt]-T0)*m+q);
        cout << "DTS " << Temperatures[iPnt]<< "K " << Form("%.3f +- %.3f",(Temperatures[iPnt]-T0)*m+q,(Temperatures[iPnt]-T0)*m+q) << endl;

    }

    TCanvas*            cAll    =   new TCanvas();
    TMultiGraph * gAll  = new TMultiGraph();
    
    gAll->Add(gVbd_vs_Temp_DTSheet,"AL");
    gAll->Add(gVbd_vs_Temp_m1,"APE");
    gAll->Add(gVbd_vs_Temp_m2,"APE");

    gAll->Draw("APE");
    gAll->GetXaxis()->SetTitle("Temperature (K)");
    gAll->GetYaxis()->SetTitle("V_{breakdown} (V)");
    
    TLegend *   lAll    =   new TLegend(0.15,0.85,0.35,0.75);
    lAll->AddEntry(gVbd_vs_Temp_DTSheet,"DataSheet","L");
    lAll->AddEntry(gVbd_vs_Temp_m1,"Measured (FBK3-R)","EP");
    lAll->AddEntry(gVbd_vs_Temp_m2,"Measured (FBK4-R)","EP");
    lAll->Draw("same");
    
    /*
    TLatex * fText = new TLatex();
    fText->DrawLatexNDC(0.6,0.4,"Channels:");
    fText->DrawLatexNDC(0.6,0.32,"B-D-F");
     */
    gVbd_vs_Temp_m1->Fit("pol1");
    gVbd_vs_Temp_m2->Fit("pol1");

    cAll->SaveAs(Form("%s_VBD_Temp.pdf",fSensor.Data()));
    //delete cAll;
}
std::pair<TH1F*,TH1F*>
fMakeVbdDistributionSystematics    ( TString fSensor, Int_t fTemperature = 293, Int_t fMode = 1 )  {
    // Letter array
    const char* fLetters[8] = {"A","B","C","D","E","F","G","H"};
    //
    // Out histos
    TH1F * hVbd_measure         = new TH1F(Form("hVbd_measure_%i",fTemperature),Form("BCOM_sn2_%iK_*",fTemperature),2000,20,40);
    TH1F * hVbd_measure_error   = new TH1F(Form("hVbd_measure_error_%i",fTemperature),Form("error_BCOM_sn2_%iK_*",fTemperature),1000,-2000,2000);
    //
    std::vector<double> fitmin          =   {0.80, 0.90, 1.00, 1.10, 1.20};
    std::vector<double> fitwidth        =   {2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2, 3.3, 3.4, 3.5};
    std::vector<double> fInitialGuess   =   {24.5,25,25.5,26.5,27,27.5};
    //
    gROOT->SetBatch(true);
    for ( Int_t iLtr = 0; iLtr < 8; iLtr++ )    {
        if ( iLtr >= 6 && fSensor.Contains("FBK_sn3") ) break;
        if ( iLtr >= 6 && fSensor.Contains("FBK_sn4") ) break;
        for ( Int_t iTer = 1; iTer <= 4; iTer++ )    {
            auto fVbd               =   fMeasureVbd(fSensor,fTemperature,fLetters[iLtr],iTer,26,1,1,4);
            hVbd_measure->Fill(         fVbd.first      );
            for ( auto fInit : fInitialGuess) {
                for ( auto min : fitmin) {
                    for ( auto width : fitwidth) {
                        auto fVbdSys        =   fMeasureVbd(fSensor,fTemperature,fLetters[iLtr],iTer,fInit,1,min,min+width);
                        hVbd_measure_error->Fill(         (fVbdSys.first-fVbd.first)*1000      );
                        hVbd_measure->Fill(         fVbdSys.first      );
                    }
                }
            }
        }
    }
    gROOT->SetBatch(false);
    return std::pair<TH1F*,TH1F*> (hVbd_measure,hVbd_measure_error);
}

void
fMakeVbdHeatMap          ( TString fSensor, Int_t fIfFBKType = 0 ) {
    std::vector<Double_t> fAverageVbd;
    std::vector<Double_t> fAverageVbd_err;
    std::vector<Int_t> fTemperature = {243,263,293};
    std::vector<TString> fLetters   = {"A","B","C","D","E","F","G","H"};
    
    TH2F * hResult = new TH2F("hResult","hResult",32,0,32,3,230,300);
    
    gROOT->SetBatch(true);
    auto jTemp = 0;
    for ( auto iTemp : fTemperature )  {
        auto nPoints = 0;
        for ( auto iLtr : fLetters )  {
            for ( Int_t iTer = 1; iTer <= 4; iTer++ )    {
                auto fVbd = fMeasureVbd(fSensor.Data(),iTemp,iLtr.Data(),iTer,25);
                fAverageVbd     .push_back( fVbd.first  );
                fAverageVbd_err .push_back( fVbd.second );
                hResult->GetXaxis()->SetBinLabel(nPoints+1,Form("%s%i",iLtr.Data(),iTer));
                nPoints++;
            }
        }
        for ( Int_t iTer = 0; iTer < nPoints; iTer++ )  {
            hResult->SetBinContent  ( iTer + 1, jTemp + 1, fAverageVbd.at(iTer) );
            hResult->SetBinError    ( iTer + 1, jTemp + 1, fAverageVbd_err.at(iTer) );
        }
        fAverageVbd     .clear();
        fAverageVbd_err .clear();
        jTemp++;
    }
    gROOT->SetBatch(false);
    
    TCanvas * c1 = new TCanvas();
    gStyle->SetOptStat(0);
    hResult->Draw("colz TEXT89");
}

std::pair<Double_t,Double_t>
fMeasureRqn ( TGraphErrors *gIV, Double_t fNormal = 1., TString fName = "" )  {
    if ( !gIV || gIV->GetN() == 0 )        return std::pair<Double_t,Double_t> (-1.,-1.);
    TCanvas* c1 = new TCanvas("","",800,800);
    gIV->SetLineColor(kBlack);
    gIV->SetMarkerStyle(20);
    gIV->Fit("pol1","IMRSQ","",1.8,2.8);
    if ( fName.Contains("FBK_sn4") ) gIV->Fit("pol1","IMRSQ","",2.5,3.5);
    auto fFitresult = gIV->GetFunction("pol1");
    fMeasRqn->SetParameter(0,fFitresult->GetParameter(0));
    fMeasRqn->SetParameter(1,1./fFitresult->GetParameter(1));
    gIV->Fit("fMeasRqn","IMRS","",2,3);
    fFitresult = gIV->GetFunction("fMeasRqn");
    fFitresult->Draw("same");
    auto fResistance    =   100.;
    auto fResistErro    =   0.14;
    if ( fName.Contains("BCOM_sn2") )    {
        if ( fName.Contains("_A") ) {   fResistance = 99.73;   }
        if ( fName.Contains("_B") ) {   fResistance = 99.70;   }
        if ( fName.Contains("_C") ) {   fResistance = 99.77;   }
        if ( fName.Contains("_D") ) {   fResistance = 99.83;   }
        if ( fName.Contains("_E") ) {   fResistance = 99.60;   }
        if ( fName.Contains("_F") ) {   fResistance = 99.60;   }
        if ( fName.Contains("_G") ) {   fResistance = 99.67;   }
        if ( fName.Contains("_H") ) {   fResistance = 99.67;   }
    }
    if ( fName.Contains("FBK_sn3") )    {
        if ( fName.Contains("_A") ) {   fResistance = 99.80;   }
        if ( fName.Contains("_B") ) {   fResistance = 99.77;   }
        if ( fName.Contains("_C") ) {   fResistance = 99.67;   }
        if ( fName.Contains("_D") ) {   fResistance = 99.77;   }
        if ( fName.Contains("_E") ) {   fResistance = 99.50;   }
        if ( fName.Contains("_F") ) {   fResistance = 99.53;   }
    }
    if ( fName.Contains("FBK_sn4") )    {
        if ( fName.Contains("_A") ) {   fResistance = 99.77;   }
        if ( fName.Contains("_B") ) {   fResistance = 99.63;   }
        if ( fName.Contains("_C") ) {   fResistance = 99.70;   }
        if ( fName.Contains("_D") ) {   fResistance = 99.57;   }
        if ( fName.Contains("_E") ) {   fResistance = 99.50;   }
        if ( fName.Contains("_F") ) {   fResistance = 99.70;   }
    }
    auto fVal_TotResist =   fFitresult->GetParameter(1);
    auto fErr_TotResist =   fFitresult->GetParError(1);
    auto fVal_SiPResist =   fVal_TotResist - fResistance;
    auto fErr_SiPResist =   TMath::Sqrt( fErr_TotResist*fErr_TotResist + fResistErro*fResistErro );
    auto fVal_QncResist =   fVal_SiPResist/fNormal;
    auto fErr_QncResist =   fErr_SiPResist/fNormal;

    // -- // -- // -- // Graphics
    gIV->GetXaxis()->SetTitle("V_{bias} (V)");
    gIV->GetYaxis()->SetTitle("Inverse Log Derivative");
    gIV->Draw("ape");
    TLatex * ftext = new TLatex();
    ftext->DrawLatexNDC(0.2,0.8,fName.Data());
    ftext->DrawLatexNDC(0.2,0.7,Form("Rqn = %.3f",fVal_QncResist));
    c1->SaveAs(Form("./Data/plotsRqn/CHFit_%s.pdf",fName.Data()));
    delete c1;
    return std::pair<Double_t,Double_t> (fVal_QncResist,fErr_QncResist);
}
std::pair<Double_t,Double_t>
fMeasureRqn ( TString fSensorName, Int_t fTemperature, TString fLetter, Int_t iTer  )  {
    auto fNormal = 9815;
    if ( fSensorName.Contains("FBK") && (fLetter.Contains("A") || fLetter.Contains("C") || fLetter.Contains("E")) ) fNormal = 6240;
    if ( fSensorName.Contains("FBK") && (fLetter.Contains("B") || fLetter.Contains("D") || fLetter.Contains("F")) ) fNormal = 40986;
    return fMeasureRqn(fSetErrorsOnIV(Form("./Data/%s/%iK/csv/fwdscan/%s_%iK_%s%i.fwdscan.csv",fSensorName.Data(),fTemperature,fSensorName.Data(),fTemperature,fLetter.Data(),iTer)),fNormal,Form("%s_%iK_%s%i",fSensorName.Data(),fTemperature,fLetter.Data(),iTer));
}
std::pair<TH1F*,TH1F*>
fMakeRqnDistribution    ( TString fSensor, Int_t fTemperature = 293, Int_t fMode = 0, Int_t fIfFBKType = 0 )  {
    // Letter array
    const char* fLetters[8] = {"A","B","C","D","E","F","G","H"};
    //
    // Out histos
    TH1F * hRqn_measure         = new TH1F(Form("hRQN_%s_%i_%i",fSensor.Data(),fTemperature,fIfFBKType),   Form("hRQN_%s_%i_%i",fSensor.Data(),fTemperature,fIfFBKType),  10000,0,.1);
    TH1F * hRqn_measure_error   = new TH1F(Form("hRQN_E_%s_%i_%i",fSensor.Data(),fTemperature,fIfFBKType), Form("hRQN_E_%s_%i_%i",fSensor.Data(),fTemperature,fIfFBKType),1000,0,1);
    for ( Int_t iLtr = 0; iLtr < 8; iLtr++ )    {
        if ( iLtr >= 6 && fSensor.Contains("FBK") ) break;
        for ( Int_t iTer = 1; iTer <= 4; iTer++ )    {
            if ( fSensor.Contains("FBK") && fIfFBKType == 1 )   {
                if ( iLtr == 0 || iLtr == 2 || iLtr == 4 ) continue;
            }
            if ( fSensor.Contains("FBK") && fIfFBKType == 0 )   {
                if ( iLtr == 1 || iLtr == 3 || iLtr == 5 ) continue;
            }
            hRqn_measure->Fill(         fMeasureRqn(fSensor,fTemperature,fLetters[iLtr],iTer).first      );
            hRqn_measure_error->Fill(   fMeasureRqn(fSensor,fTemperature,fLetters[iLtr],iTer).second     );
        }
    }
    return std::pair<TH1F*,TH1F*> (hRqn_measure,hRqn_measure_error);
}
TGraphErrors*
fMakeRqnvsTemp          ( TString fSensor, Int_t fIfFBKType = 0 ) {
    std::vector<pair<TH1F*,TH1F*>> fAverageRqnAtT;
    Int_t Temperatures[3] = {243,263,293};
    
    gROOT->SetBatch(true);
    for ( Int_t iTer = 0; iTer < 3; iTer++ )  {
        fAverageRqnAtT.push_back(fMakeRqnDistribution(fSensor,Temperatures[iTer],fIfFBKType));
    }
    gROOT->SetBatch(false);
    
    TGraphErrors   *gVbd_vs_Temp    =   new TGraphErrors();
    gVbd_vs_Temp->SetMarkerStyle(22);
    gVbd_vs_Temp->SetMarkerColor(kGreen-2);
    gVbd_vs_Temp->GetXaxis()->SetTitle("Temperature (K)");
    gVbd_vs_Temp->GetYaxis()->SetTitle("R_{quenching} (#Omega)");
    
    for ( auto fAverageRqnAtTPlot : fAverageRqnAtT )    {
        auto iPnt = gVbd_vs_Temp->GetN();
        auto fYvalue = fAverageRqnAtTPlot.first->GetMean();
        auto fYerror = fAverageRqnAtTPlot.first->GetMeanError();
        TCanvas *c1 = new TCanvas();
        fAverageRqnAtTPlot.first->Draw();
        c1->SaveAs(Form("%s_Rqn_temp_%i.pdf",fSensor.Data(),Temperatures[iPnt]));
        gVbd_vs_Temp->SetPoint(iPnt,Temperatures[iPnt],fYvalue);
        gVbd_vs_Temp->SetPointError(iPnt,0,fYerror);
    }

    TCanvas*            cAll    =   new TCanvas();
    gVbd_vs_Temp->Draw("APE");
    gVbd_vs_Temp->Fit("pol1");
    
    cAll->SaveAs(Form("%s_RQN_Temp.pdf",fSensor.Data()));
    
    return gVbd_vs_Temp;
}

void
fMakeIVComparison () {
    TFile  *fBCOM_243    = new TFile("./Data/BCOM_sn2/243K/csv/ivscan/BCOM_sn2_243K_A1.ivscan.makeiv.root");
    TFile  *fBCOM_263    = new TFile("./Data/BCOM_sn2/263K/csv/ivscan/BCOM_sn2_263K_A1.ivscan.makeiv.root");
    TFile  *fBCOM_293    = new TFile("./Data/BCOM_sn2/293K/csv/ivscan/BCOM_sn2_293K_A1.ivscan.makeiv.root");
    TFile  *fFBK1_243    = new TFile("./Data/FBK_sn3/243K/csv/ivscan/FBK_sn3_243K_A1.ivscan.makeiv.root");
    TFile  *fFBK1_263    = new TFile("./Data/FBK_sn3/263K/csv/ivscan/FBK_sn3_263K_A1.ivscan.makeiv.root");
    TFile  *fFBK1_293    = new TFile("./Data/FBK_sn3/293K/csv/ivscan/FBK_sn3_293K_A1.ivscan.makeiv.root");
    TFile  *fFBK2_243    = new TFile("./Data/FBK_sn4/243K/csv/ivscan/FBK_sn4_243K_A1.ivscan.makeiv.root");
    TFile  *fFBK2_263    = new TFile("./Data/FBK_sn4/263K/csv/ivscan/FBK_sn4_263K_A1.ivscan.makeiv.root");
    TFile  *fFBK2_293    = new TFile("./Data/FBK_sn4/293K/csv/ivscan/FBK_sn4_293K_A1.ivscan.makeiv.root");
    TFile  *fFBK1B_243   = new TFile("./Data/FBK_sn3/243K/csv/ivscan/FBK_sn3_243K_B1.ivscan.makeiv.root");
    TFile  *fFBK1B_263   = new TFile("./Data/FBK_sn3/263K/csv/ivscan/FBK_sn3_263K_B1.ivscan.makeiv.root");
    TFile  *fFBK1B_293   = new TFile("./Data/FBK_sn3/293K/csv/ivscan/FBK_sn3_293K_B1.ivscan.makeiv.root");
    TFile  *fFBK2B_243   = new TFile("./Data/FBK_sn4/243K/csv/ivscan/FBK_sn4_243K_B1.ivscan.makeiv.root");
    TFile  *fFBK2B_263   = new TFile("./Data/FBK_sn4/263K/csv/ivscan/FBK_sn4_263K_B1.ivscan.makeiv.root");
    TFile  *fFBK2B_293   = new TFile("./Data/FBK_sn4/293K/csv/ivscan/FBK_sn4_293K_B1.ivscan.makeiv.root");
    TGraphErrors   *gBC243    =   (TGraphErrors*)fBCOM_243->Get("ivscan");
    gBC243->SetMarkerStyle(20);
    gBC243->SetMarkerColor(kRed);
    TGraphErrors   *gBC263    =   (TGraphErrors*)fBCOM_263->Get("ivscan");
    gBC263->SetMarkerStyle(21);
    gBC263->SetMarkerColor(kBlue);
    TGraphErrors   *gBC293    =   (TGraphErrors*)fBCOM_293->Get("ivscan");
    gBC293->SetMarkerStyle(22);
    gBC293->SetMarkerColor(kGreen-2);
    TGraphErrors   *gF1243    =   (TGraphErrors*)fFBK1_243->Get("ivscan");
    gF1243->SetMarkerStyle(20);
    gF1243->SetMarkerColor(kRed);
    TGraphErrors   *gF1263    =   (TGraphErrors*)fFBK1_263->Get("ivscan");
    gF1263->SetMarkerStyle(21);
    gF1263->SetMarkerColor(kBlue);
    TGraphErrors   *gF1293    =   (TGraphErrors*)fFBK1_293->Get("ivscan");
    gF1293->SetMarkerStyle(22);
    gF1293->SetMarkerColor(kGreen-2);
    TGraphErrors   *gF2243    =   (TGraphErrors*)fFBK2_243->Get("ivscan");
    gF2243->SetMarkerStyle(20);
    gF2243->SetMarkerColor(kRed);
    TGraphErrors   *gF2263    =   (TGraphErrors*)fFBK2_263->Get("ivscan");
    gF2263->SetMarkerStyle(21);
    gF2263->SetMarkerColor(kBlue);
    TGraphErrors   *gF2293    =   (TGraphErrors*)fFBK2_293->Get("ivscan");
    gF2293->SetMarkerStyle(22);
    gF2293->SetMarkerColor(kGreen-2);
    TGraphErrors   *gF1243B   =   (TGraphErrors*)fFBK1B_243->Get("ivscan");
    gF1243B->SetMarkerStyle(20);
    gF1243B->SetMarkerColor(kRed);
    TGraphErrors   *gF1263B   =   (TGraphErrors*)fFBK1B_263->Get("ivscan");
    gF1263B->SetMarkerStyle(21);
    gF1263B->SetMarkerColor(kBlue);
    TGraphErrors   *gF1293B   =   (TGraphErrors*)fFBK1B_293->Get("ivscan");
    gF1293B->SetMarkerStyle(22);
    gF1293B->SetMarkerColor(kGreen-2);
    TGraphErrors   *gF2243B   =   (TGraphErrors*)fFBK2B_243->Get("ivscan");
    gF2243->SetMarkerStyle(20);
    gF2243->SetMarkerColor(kRed);
    TGraphErrors   *gF2263B   =   (TGraphErrors*)fFBK2B_263->Get("ivscan");
    gF2263B->SetMarkerStyle(21);
    gF2263B->SetMarkerColor(kBlue);
    TGraphErrors   *gF2293B   =   (TGraphErrors*)fFBK2B_293->Get("ivscan");
    gF2293B->SetMarkerStyle(22);
    gF2293B->SetMarkerColor(kGreen-2);
    
    TCanvas        *cAll    =   new TCanvas();
    cAll->Divide(3,2);
    
    TLatex* ftext = new TLatex();
    
    cAll->cd(1);
    gPad->SetLogy();
    TMultiGraph    *gAll1    =   new TMultiGraph();
    gAll1->SetMaximum(1.e-04);
    gAll1->SetMinimum(1.e-12);
    gAll1->Add(gBC243,"EP");
    gAll1->Add(gBC263,"EP");
    gAll1->Add(gBC293,"EP");
    gAll1->Draw("AP");
    ftext->DrawLatexNDC(0.5,0.15,"BCOM A1");
    
    cAll->cd(2);
    gPad->SetLogy();
    TMultiGraph    *gAll2    =   new TMultiGraph();
    gAll2->SetMaximum(1.e-04);
    gAll2->SetMinimum(1.e-12);
    gAll2->Add(gF1243,"EP");
    gAll2->Add(gF1263,"EP");
    gAll2->Add(gF1293,"EP");
    gAll2->Draw("AP");
    ftext->DrawLatexNDC(0.5,0.15,"FBK_sn3 A1");
    
    cAll->cd(3);
    gPad->SetLogy();
    TMultiGraph    *gAll4    =   new TMultiGraph();
    gAll4->SetMaximum(1.e-04);
    gAll4->SetMinimum(1.e-12);
    gAll4->Add(gF1243B,"EP");
    gAll4->Add(gF1263B,"EP");
    gAll4->Add(gF1293B,"EP");
    gAll4->Draw("AP");
    ftext->DrawLatexNDC(0.5,0.15,"FBK_sn3 B1");
    
    cAll->cd(5);
    gPad->SetLogy();
    TMultiGraph    *gAll3    =   new TMultiGraph();
    gAll3->SetMaximum(1.e-04);
    gAll3->SetMinimum(1.e-12);
    gAll3->Add(gF2243,"EP");
    //gAll3->Add(gF2263,"EP");
    //gAll3->Add(gF2293,"EP");
    gAll3->Draw("AP");
    ftext->DrawLatexNDC(0.5,0.15,"FBK_sn4 A1");
    
    cAll->cd(6);
    gPad->SetLogy();
    TMultiGraph    *gAll5    =   new TMultiGraph();
    gAll5->SetMaximum(1.e-04);
    gAll5->SetMinimum(1.e-12);
    gAll5->Add(gF2243B,"EP");
    gAll5->Add(gF2263B,"EP");
    gAll5->Add(gF2293B,"EP");
    gAll5->Draw("AP");
    ftext->DrawLatexNDC(0.5,0.15,"FBK_sn4 B1");
    
    cAll->cd(4);
    TLegend        *lAll    =   new TLegend(0.9,0.9,0.1,0.1);
    lAll->AddEntry(gBC243, "243K", "EP");
    lAll->AddEntry(gBC263, "263K", "EP");
    lAll->AddEntry(gBC293, "293K", "EP");
    lAll->Draw("same");
    
    
    cAll->SaveAs("IVScan_E1_AllT.pdf");
    delete cAll;
    delete lAll;
    delete gAll1;
}

void
fMakeRqnComparison () {
    auto g1 = fMakeRqnvsTemp("BCOM_sn2");
    auto g2 = fMakeRqnvsTemp("FBK_sn3",0);
    auto g3 = fMakeRqnvsTemp("FBK_sn3",1);
    
    TCanvas        *cAll    =   new TCanvas();
    cAll->Divide(3,1);
    
    TLatex* ftext = new TLatex();

    cAll->cd(1);
    g1->Draw("ap*");
    
    cAll->cd(2);
    g2->Draw("ap*");
    
    cAll->cd(3);
    g3->Draw("ap*");
    
}

void
fMakeHAMAvsT()  {
    TCanvas* c1 = new TCanvas();
    
    TGraphErrors* cPlo1 = new TGraphErrors();
    TGraphErrors* cPlo2 = new TGraphErrors();
    TGraphErrors* cData = new TGraphErrors();
    TGraphErrors* cDTEP = new TGraphErrors();
    TGraphErrors* cDTEM = new TGraphErrors();
    
    cPlo1->SetPoint(0,243,48.10);
    cPlo1->SetPoint(1,263,49.35);
    cPlo1->SetPoint(2,293,51.00);
    cPlo1->SetMarkerStyle(20);
    cPlo1->SetMarkerColor(kRed);
    
    cPlo2->SetPoint(0,243,49.10);
    cPlo2->SetPoint(1,263,50.15);
    cPlo2->SetPoint(2,293,51.80);
    cPlo2->SetMarkerStyle(21);
    cPlo2->SetMarkerColor(kGreen-2);
    
    cData->SetPoint(0,243,50.03);
    cData->SetPoint(1,263,51.11);
    cData->SetPoint(2,293,52.73);
    cData->SetLineStyle(4);
    cData->SetLineColor(kBlue);
    cData->SetLineWidth(5);
    
    cDTEP->SetPoint(0,243,53.03);
    cDTEP->SetPoint(1,263,54.11);
    cDTEP->SetPoint(2,293,55.73);
    cDTEP->SetLineStyle(2);
    cDTEP->SetLineColor(kBlue-3);
    cDTEP->SetLineWidth(3);
    
    cDTEM->SetPoint(0,243,47.03);
    cDTEM->SetPoint(1,263,48.11);
    cDTEM->SetPoint(2,293,49.73);
    cDTEM->SetLineStyle(2);
    cDTEM->SetLineColor(kBlue-3);
    cDTEM->SetLineWidth(3);
    
    TMultiGraph * cAll = new TMultiGraph();
    cAll->Add(cPlo1,"EP");
    cAll->Add(cPlo2,"EP");
    cAll->Add(cData,"L");
    cAll->Add(cDTEP,"L");
    cAll->Add(cDTEM,"L");
    
    TLegend *lAll = new TLegend();
    lAll->AddEntry(cPlo1,"HAM1-A","EP");
    lAll->AddEntry(cPlo2,"HAM1-B","EP");
    lAll->AddEntry(cData,"DataSheet (DTS)","EP");
    lAll->AddEntry(cDTEM,"DTS #pm 1 #sigma","EP");
    
    cPlo1->Fit("pol1");
    cPlo2->Fit("pol1");
    
    cAll->Draw("AP");
    cAll->GetXaxis()->SetTitle("Temperature (K)");
    cAll->GetYaxis()->SetTitle("V_{breakdown} (V)");
    lAll->Draw("same");
    
    c1->SaveAs("./c1.pdf");
    delete c1;
}

void
drawA1()
{

  style();

  auto xmin = 23.;
  auto xmax = 27.;
  
  auto c = new TCanvas("c", "c", 1500, 500);
  c->Divide(3, 1);
  
  c->cd(1)->DrawFrame(xmin, 1.e-10, xmax, 5.e-8, ";V_{bias} (V); I (A)");
  c->cd(1)->SetLogy();

  auto gIV_22 = IV("SensL_light-A1_22C.csv");
  gIV_22->SetMarkerStyle(20);
  gIV_22->SetMarkerSize(0.8);
  gIV_22->SetMarkerColor(kRed+1);
  gIV_22->SetLineColor(kRed+1);
  gIV_22->Draw("pc,same");

  auto gIV_22r = IV("SensL_light-A1_22C_repeated.csv");
  gIV_22r->SetMarkerStyle(25);
  gIV_22r->SetMarkerSize(0.8);
  gIV_22r->SetMarkerColor(kRed+1);
  gIV_22r->SetLineColor(kRed+1);
  gIV_22r->Draw("pc,same");

  auto gIV_0 = IV("SensL_light-A1_0C.csv");
  gIV_0->SetMarkerStyle(20);
  gIV_0->SetMarkerSize(0.8);
  gIV_0->SetMarkerColor(kGreen+2);
  gIV_0->SetLineColor(kGreen+2);
  gIV_0->Draw("pc,same");

  auto gIV_0r = IV("SensL_light-A1_0C_repeated.csv");
  gIV_0r->SetMarkerStyle(25);
  gIV_0r->SetMarkerSize(0.8);
  gIV_0r->SetMarkerColor(kGreen+2);
  gIV_0r->SetLineColor(kGreen+2);
  gIV_0r->Draw("pc,same");

  auto gIV_m10 = IV("SensL_light-A1_-10C.csv");
  gIV_m10->SetMarkerStyle(20);
  gIV_m10->SetMarkerSize(0.8);
  gIV_m10->SetMarkerColor(kAzure-3);
  gIV_m10->SetLineColor(kAzure-3);
  //  gIV_m10->Draw("pc,same");

  c->cd(2)->DrawFrame(xmin, -1., xmax, 9., ";V_{bias} (V);LD");

  auto gLD_22 = LD("SensL_light-A1_22C.csv");
  gLD_22->SetMarkerStyle(20);
  gLD_22->SetMarkerSize(0.8);
  gLD_22->SetMarkerColor(kRed+1);
  gLD_22->SetLineColor(kRed+1);
  gLD_22->Draw("pc,same");

  auto gLD_22r = LD("SensL_light-A1_22C_repeated.csv");
  gLD_22r->SetMarkerStyle(25);
  gLD_22r->SetMarkerSize(0.8);
  gLD_22r->SetMarkerColor(kRed+1);
  gLD_22r->SetLineColor(kRed+1);
  gLD_22r->Draw("pc,same");

  auto gLD_0 = LD("SensL_light-A1_0C.csv");
  gLD_0->SetMarkerStyle(20);
  gLD_0->SetMarkerSize(0.8);
  gLD_0->SetMarkerColor(kGreen+2);
  gLD_0->SetLineColor(kGreen+2);
  gLD_0->Draw("pc,same");
  
  auto gLD_0r = LD("SensL_light-A1_0C_repeated.csv");
  gLD_0r->SetMarkerStyle(25);
  gLD_0r->SetMarkerSize(0.8);
  gLD_0r->SetMarkerColor(kGreen+2);
  gLD_0r->SetLineColor(kGreen+2);
  gLD_0r->Draw("pc,same");
  
  auto gLD_m10 = LD("SensL_light-A1_-10C.csv");
  gLD_m10->SetMarkerStyle(20);
  gLD_m10->SetMarkerSize(0.8);
  gLD_m10->SetMarkerColor(kAzure-3);
  gLD_m10->SetLineColor(kAzure-3);
  gLD_m10->Draw("pc,same");
  
  c->cd(3)->DrawFrame(xmin, -1., xmax, 9., ";V_{bias} (V);ILD");

  auto gILD_22 = ILD("SensL_light-A1_22C.csv");
  gILD_22->SetMarkerStyle(20);
  gILD_22->SetMarkerSize(0.8);
  gILD_22->SetMarkerColor(kRed+1);
  gILD_22->SetLineColor(kRed+1);
  gILD_22->Draw("pc,same");

  auto gILD_22r = ILD("SensL_light-A1_22C_repeated.csv");
  gILD_22r->SetMarkerStyle(25);
  gILD_22r->SetMarkerSize(0.8);
  gILD_22r->SetMarkerColor(kRed+1);
  gILD_22r->SetLineColor(kRed+1);
  gILD_22r->Draw("pc,same");

  auto gILD_0 = ILD("SensL_light-A1_0C.csv");
  gILD_0->SetMarkerStyle(20);
  gILD_0->SetMarkerSize(0.8);
  gILD_0->SetMarkerColor(kGreen+2);
  gILD_0->SetLineColor(kGreen+2);
  gILD_0->Draw("pc,same");
  
  auto gILD_0r = ILD("SensL_light-A1_0C_repeated.csv");
  gILD_0r->SetMarkerStyle(25);
  gILD_0r->SetMarkerSize(0.8);
  gILD_0r->SetMarkerColor(kGreen+2);
  gILD_0r->SetLineColor(kGreen+2);
  gILD_0r->Draw("pc,same");
  
  auto gILD_m10 = ILD("SensL_light-A1_-10C.csv");
  gILD_m10->SetMarkerStyle(20);
  gILD_m10->SetMarkerSize(0.8);
  gILD_m10->SetMarkerColor(kAzure-3);
  gILD_m10->SetLineColor(kAzure-3);
  gILD_m10->Draw("pc,same");
  
}

void
drawA1B1()
{

  style();
  
  auto c = new TCanvas("c", "c", 1500, 500);
  c->Divide(3, 1);
  
  c->cd(1)->DrawFrame(23, 1.e-10, 27., 5.e-8, ";V_{bias} (V); I (A)");
  c->cd(1)->SetLogy();

  auto gIV_22_A1 = IV("SensL_light-A1_22C.csv");
  gIV_22_A1->SetMarkerStyle(20);
  gIV_22_A1->SetMarkerSize(0.8);
  gIV_22_A1->SetMarkerColor(kRed+1);
  gIV_22_A1->SetLineColor(kRed+1);
  gIV_22_A1->Draw("pc,same");

  auto gIV_22_B1 = IV("SensL_light-B1_22C.csv");
  gIV_22_B1->SetMarkerStyle(25);
  gIV_22_B1->SetMarkerSize(0.8);
  gIV_22_B1->SetMarkerColor(kRed+1);
  gIV_22_B1->SetLineColor(kRed+1);
  gIV_22_B1->Draw("pc,same");

  auto gIV_0_A1 = IV("SensL_light-A1_0C.csv");
  gIV_0_A1->SetMarkerStyle(20);
  gIV_0_A1->SetMarkerSize(0.8);
  gIV_0_A1->SetMarkerColor(kGreen+2);
  gIV_0_A1->SetLineColor(kGreen+2);
  gIV_0_A1->Draw("pc,same");

  auto gIV_0_B1 = IV("SensL_light-B1_0C.csv");
  gIV_0_B1->SetMarkerStyle(25);
  gIV_0_B1->SetMarkerSize(0.8);
  gIV_0_B1->SetMarkerColor(kGreen+2);
  gIV_0_B1->SetLineColor(kGreen+2);
  gIV_0_B1->Draw("pc,same");

  auto gIV_m10_A1 = IV("SensL_light-A1_-10C.csv");
  gIV_m10_A1->SetMarkerStyle(20);
  gIV_m10_A1->SetMarkerSize(0.8);
  gIV_m10_A1->SetMarkerColor(kAzure-3);
  gIV_m10_A1->SetLineColor(kAzure-3);
  //  gIV_m10_A1->Draw("pc,same");

  auto gIV_m10_B1 = IV("SensL_light-B1_-10C.csv");
  gIV_m10_B1->SetMarkerStyle(25);
  gIV_m10_B1->SetMarkerSize(0.8);
  gIV_m10_B1->SetMarkerColor(kAzure-3);
  gIV_m10_B1->SetLineColor(kAzure-3);
//  gIV_m10_B1->Draw("pc,same");

  c->cd(2)->DrawFrame(23, -1., 27., 9., ";V_{bias} (V);LD");

  auto gLD_22_A1 = LD("SensL_light-A1_22C.csv");
  gLD_22_A1->SetMarkerStyle(20);
  gLD_22_A1->SetMarkerSize(0.8);
  gLD_22_A1->SetMarkerColor(kRed+1);
  gLD_22_A1->SetLineColor(kRed+1);
  gLD_22_A1->Draw("pc,same");

  auto gLD_22_B1 = LD("SensL_light-B1_22C.csv");
  gLD_22_B1->SetMarkerStyle(25);
  gLD_22_B1->SetMarkerSize(0.8);
  gLD_22_B1->SetMarkerColor(kRed+1);
  gLD_22_B1->SetLineColor(kRed+1);
  gLD_22_B1->Draw("pc,same");

  auto gLD_0_A1 = LD("SensL_light-A1_0C.csv");
  gLD_0_A1->SetMarkerStyle(20);
  gLD_0_A1->SetMarkerSize(0.8);
  gLD_0_A1->SetMarkerColor(kGreen+2);
  gLD_0_A1->SetLineColor(kGreen+2);
  gLD_0_A1->Draw("pc,same");
  
  auto gLD_0_B1 = LD("SensL_light-B1_0C.csv");
  gLD_0_B1->SetMarkerStyle(25);
  gLD_0_B1->SetMarkerSize(0.8);
  gLD_0_B1->SetMarkerColor(kGreen+2);
  gLD_0_B1->SetLineColor(kGreen+2);
  gLD_0_B1->Draw("pc,same");
  
  auto gLD_m10_A1 = LD("SensL_light-A1_-10C.csv");
  gLD_m10_A1->SetMarkerStyle(20);
  gLD_m10_A1->SetMarkerSize(0.8);
  gLD_m10_A1->SetMarkerColor(kAzure-3);
  gLD_m10_A1->SetLineColor(kAzure-3);
  //  gLD_m10_A1->Draw("pc,same");
  
  auto gLD_m10_B1 = LD("SensL_light-B1_-10C.csv");
  gLD_m10_B1->SetMarkerStyle(20);
  gLD_m10_B1->SetMarkerSize(0.8);
  gLD_m10_B1->SetMarkerColor(kAzure-3);
  gLD_m10_B1->SetLineColor(kAzure-3);
  //  gLD_m10_B1->Draw("pc,same");
  
  c->cd(3)->DrawFrame(23, -1., 27., 9., ";V_{bias} (V);ILD");

  auto gILD_22_A1 = ILD("SensL_light-A1_22C.csv");
  gILD_22_A1->SetMarkerStyle(20);
  gILD_22_A1->SetMarkerSize(0.8);
  gILD_22_A1->SetMarkerColor(kRed+1);
  gILD_22_A1->SetLineColor(kRed+1);
  gILD_22_A1->Draw("pc,same");

  auto gILD_22_B1 = ILD("SensL_light-B1_22C.csv");
  gILD_22_B1->SetMarkerStyle(25);
  gILD_22_B1->SetMarkerSize(0.8);
  gILD_22_B1->SetMarkerColor(kRed+1);
  gILD_22_B1->SetLineColor(kRed+1);
  gILD_22_B1->Draw("pc,same");

  auto gILD_0_A1 = ILD("SensL_light-A1_0C.csv");
  gILD_0_A1->SetMarkerStyle(20);
  gILD_0_A1->SetMarkerSize(0.8);
  gILD_0_A1->SetMarkerColor(kGreen+2);
  gILD_0_A1->SetLineColor(kGreen+2);
  gILD_0_A1->Draw("pc,same");
  
  auto gILD_0_B1 = ILD("SensL_light-B1_0C.csv");
  gILD_0_B1->SetMarkerStyle(25);
  gILD_0_B1->SetMarkerSize(0.8);
  gILD_0_B1->SetMarkerColor(kGreen+2);
  gILD_0_B1->SetLineColor(kGreen+2);
  gILD_0_B1->Draw("pc,same");
  
  auto gILD_m10_A1 = ILD("SensL_light-A1_-10C.csv");
  gILD_m10_A1->SetMarkerStyle(20);
  gILD_m10_A1->SetMarkerSize(0.8);
  gILD_m10_A1->SetMarkerColor(kAzure-3);
  gILD_m10_A1->SetLineColor(kAzure-3);
  //  gILD_m10_A1->Draw("pc,same");
  
  auto gILD_m10_B1 = ILD("SensL_light-B1_-10C.csv");
  gILD_m10_B1->SetMarkerStyle(25);
  gILD_m10_B1->SetMarkerSize(0.8);
  gILD_m10_B1->SetMarkerColor(kAzure-3);
  gILD_m10_B1->SetLineColor(kAzure-3);
  //  gILD_m10_B1->Draw("pc,same");
  
}

TGraphErrors *
moving_average(TGraphErrors *gin, int n = 2)
{
  auto gout = new TGraphErrors;
  for (int i = 0; i < gin->GetN() - n; ++i) {
    double x = 0.;
    double y = 0.;
    for (int j = i; j < i + n; ++j) {
      x += gin->GetX()[j];
      y += gin->GetY()[j];
    }
    x /= (double)n;
    y /= (double)n;
    gout->SetPoint(i, x, y);
  }
  return gout;
}


TGraphErrors *
average(TGraphErrors *gin, int n = 2)
{
  auto gout = new TGraphErrors;
  for (int i = 0; i < gin->GetN() - n; i += n) {
    double x = 0.;
    double y = 0.;
    for (int j = i; j < i + n; ++j) {
      x += gin->GetX()[j];
      y += gin->GetY()[j];
    }
    x /= (double)n;
    y /= (double)n;
    gout->SetPoint(gout->GetN(), x, y);
  }
  return gout;
}


double
Vbd(const TGraphErrors *gIV, int method, double vbd_ini)
{

  std::cout << "using method" << method << std::endl;

  switch (method) {

  case 0: { // the SensL method
    auto vbd = vbd_ini;
    auto gsqrtIV = sqrtIV(gIV);
    // fit sqrt(IV) for V > Vbd with pol1 until we reach stable value
    auto pol1 = (TF1 *)gROOT->GetFunction("pol1");
    while (true) {
      gsqrtIV->Fit(pol1, "q", "", vbd + 0.5, vbd + 1.5);
      //      auto vbd_new = pol1->GetX(0.);   // -pol1->GetParameter(0) / pol1->GetParameter(1);
      auto vbd_new = -pol1->GetParameter(0) / pol1->GetParameter(1);
      std::cout << "vbd: " << vbd << "--> vdb_new: " << vbd_new << std::endl;
      if (fabs(vbd_new - vbd) > 0.001) {
	vbd = vbd_new;
	continue;
      }
      gsqrtIV->Draw("ap*");
      pol1->SetLineStyle(kDashed);
      pol1->SetRange(24., 27.);
      pol1->Draw("same");
      return vbd_new;
    }
  }
    
  case 1: {
    // LD has its maximum
    auto gLD = LD(gIV);
    gLD = moving_average(gLD, 10);
    double ymax = 0.;
    double vbd = -1.;
    for (int i = 0; i < gLD->GetN(); ++i) {
      if (gLD->GetY()[i] < ymax) continue;
      if (gLD->GetX()[i] > 27) continue;
      ymax = gLD->GetY()[i];
      vbd = gLD->GetX()[i];
    }
    return vbd;
  }
    
  case 3: {
    // estimate Vbd with method 1
    auto vbd = vbd_ini; // Vbd(gIV, 1) - 0.5;
    auto gILD = ILD(gIV);
    // fit ILD for V > Vbd with pol1 until we reach stable value
    auto pol1 = (TF1 *)gROOT->GetFunction("pol1");
    while (true) {
      gILD->Fit(pol1, "q", "", vbd + 0.5, vbd + 3.0);
      gILD->Draw("ap*");
      auto vbd_new = -pol1->GetParameter(0) / pol1->GetParameter(1);
      std::cout << "vbd: " << vbd << "--> vdb_new: " << vbd_new << std::endl;
      if (fabs(vbd_new - vbd) > 0.001) {
	vbd = vbd_new;
	continue;
      }
      return vbd_new;
    } 
  }
    
  }

  return 0.;
}

Double_t
fGetClosestY(TGraphErrors* gTarget, Double_t fValue)  {
    if ( fValue <= gTarget->GetX()[0] ) return gTarget->GetY()[0];
    if ( fValue >= gTarget->GetX()[gTarget->GetN()-1] ) return gTarget->GetY()[gTarget->GetN()-1];
    for ( Int_t iTer = 0; iTer < gTarget->GetN()-1; iTer++ )  {
        auto fBefore = gTarget->GetX()[iTer];
        auto fAfter_ = gTarget->GetX()[iTer+1];
        if ( fValue <= fAfter_  && fValue >= fBefore )  {
            if ( fabs( fAfter_ - fValue ) > fabs( fBefore - fValue )  ) return gTarget->GetY()[iTer];
            else return gTarget->GetY()[iTer+1];
        }
    }
    return -1.;
}

TGraphErrors*
fCompareIrradiationVbd  ( TString fSensor, Int_t fTemperature = 293, Int_t fMode = 2, Int_t fIfFBKType = 0, Float_t fOverVoltage = 3)  {
    TGraphErrors    *gBefore=   new TGraphErrors();
    TGraphErrors    *gAfter_=   new TGraphErrors();
    TGraphErrors    *gDelta_=   new TGraphErrors();
    TGraphErrors    *gCurrnt=   new TGraphErrors();
    
    TGraphErrors    **  gSensors = new TGraphErrors* [3];
    for ( Int_t iTer = 1; iTer <= 3; iTer++ )   {
        gSensors[iTer-1] = new TGraphErrors();
    }
    
    TLatex* ftext = new TLatex();
    TLegend*L2 = new TLegend(0.1,0.1,0.5,0.2);
    L2->SetNColumns(4);
    L2->AddEntry(gDelta_,"Mean","EP");
    
    const char* fLetters[8] = {"A","B","C","D","E","F","G","H"};
    for ( Int_t iTer = 1; iTer <= 4; iTer++ )   {
        
        TH1F * fNotIrradiated   = new TH1F(Form("hVbd_measure_%i",fTemperature),Form("BCOM_sn2_%iK_*",fTemperature),2000,20,40);
        TH1F * fIrradiated      = new TH1F(Form("hVbd_measure_error_%i",fTemperature),Form("error_BCOM_sn2_%iK_*",fTemperature),2000,20,40);
        
        auto fNotIrradiatedArray    =   fGetVbdDistribution(fSensor,               fTemperature,   fMode,  fIfFBKType, iTer);
        auto fIrradiated___Array    =   fGetVbdDistribution(fSensor+TString("_2"), fTemperature,   fMode,  fIfFBKType, iTer);
        
        for ( Int_t jTer = 1; jTer <= 3; jTer++ )   {
            gSensors[jTer-1] -> SetPoint        (iTer-1,    fRadiation[iTer-1], fIrradiated___Array.first.at(jTer-1)-fNotIrradiatedArray.first.at(jTer-1));
            gSensors[jTer-1] -> SetPointError   (iTer-1,    0,                  sqrt(fIrradiated___Array.second.at(jTer-1)*fIrradiated___Array.second.at(jTer-1)+fNotIrradiatedArray.second.at(jTer-1)*fNotIrradiatedArray.second.at(jTer-1)));
            if ( iTer != 1 ) continue;
            if ( fIfFBKType == 1 )L2->AddEntry(gSensors[jTer-1],fLetters[-1+2*jTer],"EP");
            if ( fIfFBKType == 0 )L2->AddEntry(gSensors[jTer-1],fLetters[-2+2*jTer],"EP");
            gSensors[jTer-1]->SetMarkerStyle(20+jTer);
            gSensors[jTer-1]->SetMarkerColor(kRed);
        }
        
        for ( auto fVal : fNotIrradiatedArray.first ) {
            fNotIrradiated->Fill(fVal);
        }
        for ( auto fVal : fIrradiated___Array.first ) {
            fIrradiated->Fill(fVal);
        }
        auto fMeanNoIrr = fNotIrradiated->GetMean();
        auto fMErrNoIrr = fNotIrradiated->GetMeanError();
        auto fMeanIrrdt = fIrradiated->GetMean();
        auto fMErrIrrdt = fIrradiated->GetMeanError();
        
        TCanvas * cTest = new TCanvas("","",1000,500);
        cTest->Divide(2,1);
        gStyle->SetOptStat(0000000);
        cTest->cd(1);
        fNotIrradiated->SetTitle("No Irradiation");
        fNotIrradiated->GetXaxis()->SetRangeUser(fMeanNoIrr-1,fMeanNoIrr+1);
        fNotIrradiated->Draw();
        if ( fIfFBKType == 1 )  ftext->DrawLatexNDC(0.15,0.85,Form("%s NUV-HD-RH",fSensor.Data()));
        if ( fIfFBKType == 0 ) ftext->DrawLatexNDC(0.15,0.85,Form("%s NUV-HD-CHK",fSensor.Data()));
        ftext->DrawLatexNDC(0.15,0.80,Form("Irradiation @%.0E @%iK",fRadiation[iTer-1],fTemperature));
        cTest->cd(2);
        fIrradiated->SetTitle("Post Irradiation");
        fIrradiated->GetXaxis()->SetRangeUser(fMeanNoIrr-1,fMeanNoIrr+1);
        fIrradiated->Draw();
        cTest->SaveAs(Form("HVBD_%s_%iK_%i_%i.pdf",fSensor.Data(), fTemperature, fIfFBKType, iTer));
        cTest->SaveAs(Form("HVBD_%s_%iK_%i_%i.png",fSensor.Data(), fTemperature, fIfFBKType, iTer));
        delete cTest;
        
        gBefore->SetPoint       (iTer-1,iTer,fMeanNoIrr);
        gBefore->SetPointError  (iTer-1,0,fMErrNoIrr);
        
        gAfter_->SetPoint       (iTer-1,iTer,fMeanIrrdt);
        gAfter_->SetPointError  (iTer-1,0,fMErrIrrdt);
        
        gDelta_->SetPoint       (iTer-1,fRadiation[iTer-1],(fMeanIrrdt-fMeanNoIrr));
        gDelta_->SetPointError  (iTer-1,0,sqrt(fMErrIrrdt*fMErrIrrdt+fMErrNoIrr*fMErrNoIrr));
    
        auto fColumnOnly = iTer;
        //
        Double_t fGain = 0;
        for ( Int_t iLtr = 0; iLtr < 8; iLtr++ )    {
            if ( iLtr >= 6 && fSensor.Contains("FBK") ) break;
            for ( Int_t iTer = 1; iTer <= 4; iTer++ )    {
                auto fInitialGuess      =   26;
                if ( fSensor.Contains("FBK") ) fInitialGuess      =   32;
                if ( fSensor.Contains("FBK") && fIfFBKType == 1 )   {
                    if ( iLtr == 0 || iLtr == 2 || iLtr == 4 ) continue;
                }
                if ( fSensor.Contains("FBK") && fIfFBKType == 0 )   {
                    if ( iLtr == 1 || iLtr == 3 || iLtr == 5 ) continue;
                }
                if ( fColumnOnly > 0 )    {
                    if ( iTer != fColumnOnly ) continue;
                }
                auto gNotIrr = makeiv(fSensor.Data(),fTemperature,fLetters[iLtr],iTer);
                auto gIrradt = makeiv(fSensor.Data()+TString("_2"),fTemperature,fLetters[iLtr],iTer);
                
                auto fCurrent = 1.;
                auto fCurren2 = 1.;
                if ( fOverVoltage <=10 )   fCurrent = fGetClosestY(gNotIrr,fMeanNoIrr+fOverVoltage);
                if ( fOverVoltage <=10 )   fCurren2 = fGetClosestY(gIrradt,fMeanIrrdt+fOverVoltage);
                if ( fOverVoltage > 10 )   fCurrent = fGetClosestY(gNotIrr,fOverVoltage);
                if ( fOverVoltage > 10 )   fCurren2 = fGetClosestY(gIrradt,fOverVoltage);
                
                fGain += fCurren2/(fCurrent*1.);
                
                TCanvas *fCheck2 = new TCanvas("","",500,500);
                gPad->SetLogy();
                gNotIrr->SetMarkerColor(kRed);
                gIrradt->SetMarkerColor(kBlue);
                gNotIrr->SetMarkerStyle(22);
                gIrradt->SetMarkerStyle(23);
                TMultiGraph*g2 = new TMultiGraph();
                g2->Add(gNotIrr,"EP");
                g2->Add(gIrradt,"EP");
                TLegend*L2 = new TLegend(0.1,0.9,0.5,0.8);
                L2->AddEntry(gNotIrr,"Before Irradiation","EP");
                L2->AddEntry(gIrradt,"After Irradiation","EP");
                g2->Draw("ALP");
                L2->Draw("SAME");
                fCheck2->SaveAs(Form("./Data/plots/FBK_sn3_%iK_%s%i_BA.pdf",fTemperature,fLetters[iLtr],iTer));
                delete fCheck2;
            }
        }
        fGain /= 1.;
        gCurrnt->SetPoint(iTer-1,fRadiation[iTer-1],fGain);
    }
    TCanvas *fCheck = new TCanvas("","",1000,500);
    fCheck->Divide(2,1);
    fCheck->cd(1);
    gBefore->SetMarkerColor(kRed);
    gAfter_->SetMarkerColor(kBlue);
    gDelta_->SetMarkerColor(kBlue);
    gBefore->SetMarkerStyle(22);
    gAfter_->SetMarkerStyle(23);
    gDelta_->SetMarkerStyle(20);
    TMultiGraph*g1 = new TMultiGraph();
    g1->Add(gBefore,"EP");
    g1->Add(gAfter_,"EP");
    TLegend*L1 = new TLegend(0.1,0.1,0.5,0.2);
    L1->AddEntry(gBefore,"Before Irradiation","EP");
    L1->AddEntry(gAfter_,"After Irradiation","EP");
    g1->Draw("ALP");
    g1->GetXaxis()->SetTitle("Column");
    g1->GetYaxis()->SetTitle("V_{bd}");
    L1->Draw("SAME");
    fCheck->cd(2);
    gPad->SetLogx();
    TMultiGraph*g2= new TMultiGraph();
    g2->Add(gDelta_,"EP");
    g2->Add(gSensors[0],"EP");
    g2->Add(gSensors[1],"EP");
    g2->Add(gSensors[2],"EP");
    g2->Draw("ALP");
    if ( fIfFBKType == 1 )  ftext->DrawLatexNDC(0.15,0.85,Form("%s NUV-HD-RH",fSensor.Data()));
    if ( fIfFBKType == 0 ) ftext->DrawLatexNDC(0.15,0.85,Form("%s NUV-HD-CHK",fSensor.Data()));
    ftext->DrawLatexNDC(0.15,0.80,Form("After - Before @%iK",fTemperature));
    L2->Draw("SAME");
    fCheck->SaveAs(Form("%s_%iK_%i_BA.pdf",fSensor.Data(),fTemperature,fIfFBKType));
    fCheck->SaveAs(Form("%s__%iK_%i_BA.png",fSensor.Data(),fTemperature,fIfFBKType));
    
    TCanvas *fCheck2 = new TCanvas("","",500,500);
    gPad->SetLogy();
    gPad->SetLogx();
    gCurrnt->SetMarkerColor(kGreen-1);
    gCurrnt->SetMarkerStyle(21);
    fMeasCurr->SetParameter(0,1.e-8);
    fMeasCurr->SetParameter(1,1);
    gCurrnt->SetMaximum(1.e6);
    gCurrnt->SetMinimum(1.);
    gCurrnt->GetXaxis()->SetLimits(1.e7,1.e12);
    gCurrnt->GetXaxis()->SetTitle("1-MeV n_{eq}/cm^{2}");
    gCurrnt->GetYaxis()->SetTitle("Current Increase");
    gCurrnt->Draw("AP");
    if ( fIfFBKType == 1 )  ftext->DrawLatexNDC(0.15,0.85,Form("%s NUV-HD-RH",fSensor.Data()));
    if ( fIfFBKType == 0 ) ftext->DrawLatexNDC(0.15,0.85,Form("%s NUV-HD-CHK",fSensor.Data()));
    ftext->DrawLatexNDC(0.15,0.80,Form("Current Ratio: "));
    ftext->DrawLatexNDC(0.15,0.75,Form("@%2.0fV overvoltage",fOverVoltage));
    ftext->DrawLatexNDC(0.15,0.70,Form("@%iK",fTemperature));
    gCurrnt->Fit(fMeasCurr,"MES");
    fMeasCurr->Draw("same");
    fCheck2->SaveAs(Form("./%s_%iK_%i_CG.pdf",fSensor.Data(),fTemperature,fIfFBKType));
    fCheck2->SaveAs(Form("./%s_%iK_%i_CG.png",fSensor.Data(),fTemperature,fIfFBKType));
    
    if ( fIfFBKType == 1 )   cout << fSensor.Data() << "  NUV-HD-RH  @" << fTemperature << "K :" << endl;
    if ( fIfFBKType == 0 )   cout << fSensor.Data() << "  NUV-HD-CHK @" << fTemperature << "K :" << endl;
    
    for ( Int_t iTer = 0; iTer < 4; iTer++ ) {
        auto fVal = gAfter_->GetY()[iTer];
        auto fErr = gAfter_->GetEY()[iTer];
        cout << "Irradiation " << fRadiation[iTer] << " (column " << iTer+1 << " ): " << fVal << " +-" << fErr << endl;
    }
    for ( Int_t iTer = 0; iTer < 4; iTer++ ) {
        auto fVal = gBefore->GetY()[iTer];
        auto fErr = gBefore->GetEY()[iTer];
        cout << "Irradiation " << 0. << " (column " << iTer+1 << " ): " << fVal << " +-" << fErr << endl;
    }
    
    return gCurrnt;
}

void
fCompareIrradiationCurrentGain  ( TString fSensor, Int_t fMode = 2, Int_t fIfFBKType = 0, Float_t fOverVoltage = 3 )  {
    TGraphErrors**  gCurr = new TGraphErrors*[3];
    int fTemperature[3] = {293,263,243};
    for ( Int_t iTer = 1; iTer <= 3; iTer++ )   {
        gCurr[iTer-1] = new TGraphErrors(*fCompareIrradiationVbd(fSensor,fTemperature[iTer-1],fMode,fIfFBKType,fOverVoltage));
    }
    
    TLatex* ftext = new TLatex();
    TCanvas *fCheck = new TCanvas("","",700,700);
    gPad->SetLogx();
    gPad->SetLogy();
    gCurr[0]->SetMarkerColor(kRed);
    gCurr[1]->SetMarkerColor(kBlue);
    gCurr[2]->SetMarkerColor(kGreen-2);
    gCurr[0]->SetMarkerStyle(22);
    gCurr[1]->SetMarkerStyle(23);
    gCurr[2]->SetMarkerStyle(21);
    TMultiGraph*g1 = new TMultiGraph();
    g1->Add(gCurr[0],"EP");
    g1->Add(gCurr[1],"EP");
    g1->Add(gCurr[2],"EP");
    TLegend*L1 = new TLegend(0.9,0.1,0.5,0.2);
    L1->AddEntry(gCurr[0],"293K","EP");
    L1->AddEntry(gCurr[1],"263K","EP");
    L1->AddEntry(gCurr[2],"243K","EP");
    g1->Draw("ALP");
    g1->GetXaxis()->SetTitle("1-MeV n_{eq}/cm^{2}");
    g1->GetYaxis()->SetTitle("Current Increase");
    L1->Draw("SAME");
    if ( fIfFBKType == 1 )  ftext->DrawLatexNDC(0.15,0.85,Form("%s NUV-HD-RH",fSensor.Data()));
    if ( fIfFBKType == 0 ) ftext->DrawLatexNDC(0.15,0.85,Form("%s NUV-HD-CHK",fSensor.Data()));
    ftext->DrawLatexNDC(0.15,0.80,Form("@%2.0fV overvoltage",fOverVoltage));
    fCheck->SaveAs(Form("%s_%i_CG.pdf",fSensor.Data(),fIfFBKType));
    fCheck->SaveAs(Form("%s_%i_CG.png",fSensor.Data(),fIfFBKType));
}
void
fCompareIrradiationCurrentGainAll  ( TString fSensor, Float_t fOverVoltage = 3 )  {
    fCompareIrradiationCurrentGain  ( fSensor, 2, 0,fOverVoltage );
    fCompareIrradiationCurrentGain  ( fSensor, 2, 1,fOverVoltage );
}


//
//::::::::::::::::::::::::://
// - - CURRENT COMPARE - - //
//::::::::::::::::::::::::://
//
void
fCompare_I
 ( TString fSensor, Int_t fTemperature, TString fChannel )                  {
    TLatex* ftext = new TLatex();
    TLegend*L1 = new TLegend(0.75,0.25,1.0,0.65);
    TCanvas *fCheck = new TCanvas("fCheck","",1600,400);
    fCheck->Divide(4,1);
    for ( Int_t i(1); i<=4; i++ )    {
        fCheck->cd(i);
        gPad->SetLogy();
        TMultiGraph*g1 = new TMultiGraph();
        
        auto fNormal    = makeiv( fSensor,fTemperature,fChannel,i);
        fNormal->SetMarkerStyle(22);
        fNormal->SetMarkerColor(kBlue);
        
        auto fIrradiat  = makeiv( fSensor + TString(Form("_%i",2)) ,fTemperature,fChannel,i);
        fIrradiat->SetMarkerStyle(24);
        fIrradiat->SetMarkerColor(kRed);
        
        auto fAnneal1  = makeiv( fSensor + TString(Form("_%i",3)) ,fTemperature,fChannel,i);
        fAnneal1->SetMarkerStyle(26);
        fAnneal1->SetMarkerColor(kGreen-2);
        
        auto fAnneal2  = makeiv( fSensor + TString(Form("_%i",4)) ,fTemperature,fChannel,i);
        fAnneal2->SetMarkerStyle(26);
        fAnneal2->SetMarkerColor(kGreen-6);
        
        //  Legend
        if ( i==1 ) L1->AddEntry(fNormal,"Standard");
        if ( i==1 ) L1->AddEntry(fIrradiat,"Irradiated");
        if ( i==1 ) L1->AddEntry(fAnneal1,"1^ Anneal");
        if ( i==1 ) L1->AddEntry(fAnneal2,"2^ Anneal");
        
        // Multigraph
        g1->Add(fNormal,    "EP");
        g1->Add(fIrradiat,  "EP");
        g1->Add(fAnneal1,   "EP");
        g1->Add(fAnneal2,   "EP");
        g1->Draw("ALP");
        ftext->DrawLatexNDC(0.35,0.15,Form("%2.0E 1-MeV n_{eq}/cm^{2}",fRadiation[i-1]));
    }
    fCheck->cd(1);
    ftext->DrawLatexNDC(0.15,0.80,Form("%s %s",fSensor.Data(),fChannel.Data()));
    L1->Draw("SAME");
    gROOT->ProcessLine(Form(".! mkdir -p Data/plots/CurrentCompare/%s/%i/",(fSensor).Data(),fTemperature));
    fCheck->SaveAs(Form("Data/plots/CurrentCompare/%s/%i/%s_Channels.pdf",(fSensor).Data(),fTemperature,fChannel.Data()));
    fCheck->SaveAs(Form("Data/plots/CurrentCompare/%s/%i/%s_Channels.png",(fSensor).Data(),fTemperature,fChannel.Data()));
    delete fCheck;
}
//
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
void
fCompare_I
 ( TString fSensor, Int_t fTemperature )                                    {
    for ( Int_t iLtr = 0; iLtr < 8; iLtr++ )    {
        if ( iLtr >= 6 && fSensor.Contains("FBK") ) break;
        fCompare_I( fSensor, fTemperature, fLetters[iLtr]  );
    }
}
//
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
void
fCompare_I
 ( TString fSensor )                                                        {
    fCompare_I( fSensor, 243 );
    fCompare_I( fSensor, 263 );
    fCompare_I( fSensor, 293 );
}
//
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//
//::::::::::::::::::::::::://
// - - VBD COMPARE - - - - //
//::::::::::::::::::::::::://
//
void
fCompare_Vbd
 ( TString fSensor, Int_t fTemperature, TString fChannel )                  {
    TLatex* ftext = new TLatex();
    
    TLegend*L1 = new TLegend(1.0,1.0,0.6,0.8);
    
    TCanvas *fCheck = new TCanvas("fCheck","",1000,1000);
    
    TGraphErrors   *fNormal =   new TGraphErrors();
    fNormal->SetMarkerStyle(22);
    fNormal->SetMarkerColor(kBlue);
    
    TGraphErrors   *fIrradiat =   new TGraphErrors();
    fIrradiat->SetMarkerStyle(24);
    fIrradiat->SetMarkerColor(kRed);
    
    TGraphErrors   *fAnneal1 =   new TGraphErrors();
    fAnneal1->SetMarkerStyle(26);
    fAnneal1->SetMarkerColor(kGreen-2);
    
    TGraphErrors   *fAnneal2 =   new TGraphErrors();
    fAnneal2->SetMarkerStyle(26);
    fAnneal2->SetMarkerColor(kGreen-6);
    
    //  Legend
    L1->AddEntry(fNormal,"Standard");
    L1->AddEntry(fIrradiat,"Irradiated");
    L1->AddEntry(fAnneal1,"1^ Anneal");
    L1->AddEntry(fAnneal2,"2^ Anneal");
    L1->Draw("same");
    
    TMultiGraph*g1 = new TMultiGraph();
    g1->Add(fNormal,    "EP");
    g1->Add(fIrradiat,  "EP");
    g1->Add(fAnneal1,   "EP");
    g1->Add(fAnneal2,   "EP");
    g1->Draw("ALP");
    for ( Int_t i(1); i<=4; i++ )    {
        fCheck->cd(i);
        
        auto fNormal_   = fMeasureVbd( fSensor,fTemperature,fChannel,i,31,2);
        fNormal->SetPoint       (i-1,i,fNormal_.first);
        fNormal->SetPointError  (i-1,0,fNormal_.second);
        
        auto fIrradiat_ = fMeasureVbd( fSensor + TString(Form("_%i",2)) ,fTemperature,fChannel,i,31,2);
        fIrradiat->SetPoint       (i-1,i,fIrradiat_.first);
        fIrradiat->SetPointError  (i-1,0,fIrradiat_.second);
        
        auto fAnneal1_  = fMeasureVbd( fSensor + TString(Form("_%i",3)) ,fTemperature,fChannel,i,31,2);
        fAnneal1->SetPoint       (i-1,i,fAnneal1_.first);
        fAnneal1->SetPointError  (i-1,0,fAnneal1_.second);
        
        auto fAnneal2_  = fMeasureVbd( fSensor + TString(Form("_%i",4)) ,fTemperature,fChannel,i,31,2);
        fAnneal2->SetPoint       (i-1,i,fAnneal2_.first);
        fAnneal2->SetPointError  (i-1,0,fAnneal2_.second);
    }
    ftext->DrawLatexNDC(0.15,0.80,Form("%s %s",fSensor.Data(),fChannel.Data()));
    L1->Draw("SAME");
    gROOT->ProcessLine(Form(".! mkdir -p Data/plots/Compare_Vbd/%s/%i/",(fSensor).Data(),fTemperature));
    fCheck->SaveAs(Form("Data/plots/Compare_Vbd/%s/%i/%s_Channels.pdf",(fSensor).Data(),fTemperature,fChannel.Data()));
    fCheck->SaveAs(Form("Data/plots/Compare_Vbd/%s/%i/%s_Channels.png",(fSensor).Data(),fTemperature,fChannel.Data()));
    delete fCheck;
}
//
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
void
fCompare_Vbd
 ( TString fSensor, Int_t fTemperature, Int_t fIfFBKType )                  {
    TLatex* ftext = new TLatex();
    
    TLegend*L1 = new TLegend(1.0,1.0,0.6,0.8);
    
    TCanvas *fCheck = new TCanvas("fCheck","",1000,1000);
    
    TGraphErrors   *fNormal =   new TGraphErrors();
    fNormal->SetMarkerStyle(22);
    fNormal->SetMarkerColor(kBlue);
    
    TGraphErrors   *fIrradiat =   new TGraphErrors();
    fIrradiat->SetMarkerStyle(24);
    fIrradiat->SetMarkerColor(kRed);
    
    TGraphErrors   *fAnneal1 =   new TGraphErrors();
    fAnneal1->SetMarkerStyle(26);
    fAnneal1->SetMarkerColor(kGreen-2);
    
    TGraphErrors   *fAnneal2 =   new TGraphErrors();
    fAnneal2->SetMarkerStyle(26);
    fAnneal2->SetMarkerColor(kGreen-6);
    
    //  Legend
    L1->AddEntry(fNormal,"Standard");
    L1->AddEntry(fIrradiat,"Irradiated");
    L1->AddEntry(fAnneal1,"1^ Anneal");
    L1->AddEntry(fAnneal2,"2^ Anneal");
    L1->Draw("same");
    
    //  Graphs
    TMultiGraph*g1 = new TMultiGraph();
    g1->Add(fNormal,    "EP");
    g1->Add(fIrradiat,  "EP");
    g1->Add(fAnneal1,   "EP");
    g1->Add(fAnneal2,   "EP");
    g1->Draw("ALP");
    
    for ( Int_t i(1); i<=4; i++ )    {
        
        auto fNormal_   = fMakeVbdDistribution( fSensor,fTemperature,2,fIfFBKType,i);
        fNormal->SetPoint       (i-1,i,fNormal_.first->GetMean());
        fNormal->SetPointError  (i-1,0,fNormal_.first->GetMeanError());
        
        auto fIrradiat_ = fMakeVbdDistribution( fSensor + TString(Form("_%i",2)) ,fTemperature,2,fIfFBKType,i);
        fIrradiat->SetPoint       (i-1,i,fIrradiat_.first->GetMean());
        fIrradiat->SetPointError  (i-1,0,fIrradiat_.first->GetMeanError());
        
        auto fAnneal1_  = fMakeVbdDistribution( fSensor + TString(Form("_%i",3)) ,fTemperature,2,fIfFBKType,i);
        fAnneal1->SetPoint       (i-1,i,fAnneal1_.first->GetMean());
        fAnneal1->SetPointError  (i-1,0,fAnneal1_.first->GetMeanError());
        
        auto fAnneal2_  = fMakeVbdDistribution( fSensor + TString(Form("_%i",4)) ,fTemperature,2,fIfFBKType,i);
        fAnneal2->SetPoint       (i-1,i,fAnneal2_.first->GetMean());
        fAnneal2->SetPointError  (i-1,0,fAnneal2_.first->GetMeanError());
    }
    ftext->DrawLatexNDC(0.15,0.80,Form("%s",fSensor.Data()));
    L1->Draw("SAME");
    gROOT->ProcessLine(Form(".! mkdir -p Data/plots/Compare_Vbd/%s/%i/",(fSensor).Data(),fTemperature));
    fCheck->SaveAs(Form("Data/plots/Compare_Vbd/%s/%i/%i_Channels.pdf",(fSensor).Data(),fTemperature,fIfFBKType));
    fCheck->SaveAs(Form("Data/plots/Compare_Vbd/%s/%i/%i_Channels.png",(fSensor).Data(),fTemperature,fIfFBKType));
    delete fCheck;
}
//
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
void
fCompare_Vbd
 ( TString fSensor, Int_t fIfFBKType )                                      {
    fCompare_Vbd( fSensor, 243, fIfFBKType );
    fCompare_Vbd( fSensor, 263, fIfFBKType );
    fCompare_Vbd( fSensor, 293, fIfFBKType );
}
//
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
void
fCompare_Vbd
 ( TString fSensor )                                                        {
    fCompare_Vbd( fSensor, 0 );
    fCompare_Vbd( fSensor, 1 );
}
//
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//
//::::::::::::::::::::::::://
// - - UTILITY - - - - - - //
//::::::::::::::::::::::::://
//
void
fDownload
 ( TString fDirectory = "/nfshome/sipm4eic/20210503.BCOM.sn2/", TString fSensor = "BCOM_sn2",  TString fSensorName = "BCOM_sn2", Int_t fInTemp = -1., TString fColumn = "ABCDEFGH" )  {
    fDirectory += TString(fSensor) + TString("_");
    const char* fLetters[8] = {"A","B","C","D","E","F","G","H"};
    Int_t fTemperature[3] = {243,263,293};
    for ( Int_t iTmp = 0; iTmp < 3; iTmp++ )    {
        if ( fInTemp != -1 && fTemperature[iTmp] != fInTemp ) return;
        gROOT->ProcessLine(Form(".! mkdir -p ./Data/%s/%iK/png/zero",fSensorName.Data(),fTemperature[iTmp]));
        gROOT->ProcessLine(Form(".! mkdir -p ./Data/%s/%iK/png/ivscan",fSensorName.Data(),fTemperature[iTmp]));
        gROOT->ProcessLine(Form(".! mkdir -p ./Data/%s/%iK/png/fwdscan",fSensorName.Data(),fTemperature[iTmp]));
        gROOT->ProcessLine(Form(".! mkdir -p ./Data/%s/%iK/csv/zero",fSensorName.Data(),fTemperature[iTmp]));
        gROOT->ProcessLine(Form(".! mkdir -p ./Data/%s/%iK/csv/ivscan",fSensorName.Data(),fTemperature[iTmp]));
        gROOT->ProcessLine(Form(".! mkdir -p ./Data/%s/%iK/csv/fwdscan",fSensorName.Data(),fTemperature[iTmp]));
        for ( Int_t iLtr = 0; iLtr < 8; iLtr++ )    {
            if ( !fColumn.Contains(fLetters[iLtr]) ) continue;
            for ( Int_t iTer = 1; iTer <= 4; iTer++ )    {
                if ( !fDirectory.IsNull() )   {
                    // /nfshome/sipm4eic/20210503.BCOM.sn2
                    gROOT->ProcessLine(Form(".! scp sipm4eic@131.154.97.95:%s%iK_%s%i/%s_%iK_%s%i.zero.png   ./Data/%s/%iK/png/zero/",fDirectory.Data(),fTemperature[iTmp],fLetters[iLtr],iTer,fSensor.Data(),fTemperature[iTmp],fLetters[iLtr],iTer,fSensorName.Data(),fTemperature[iTmp]));
                    gROOT->ProcessLine(Form(".! scp sipm4eic@131.154.97.95:%s%iK_%s%i/%s_%iK_%s%i.ivscan.png ./Data/%s/%iK/png/ivscan/",fDirectory.Data(),fTemperature[iTmp],fLetters[iLtr],iTer,fSensor.Data(),fTemperature[iTmp],fLetters[iLtr],iTer,fSensorName.Data(),fTemperature[iTmp]));
                    gROOT->ProcessLine(Form(".! scp sipm4eic@131.154.97.95:%s%iK_%s%i/%s_%iK_%s%i.fwdscan.png ./Data/%s/%iK/png/fwdscan/",fDirectory.Data(),fTemperature[iTmp],fLetters[iLtr],iTer,fSensor.Data(),fTemperature[iTmp],fLetters[iLtr],iTer,fSensorName.Data(),fTemperature[iTmp]));
                    gROOT->ProcessLine(Form(".! scp sipm4eic@131.154.97.95:%s%iK_%s%i/%s_%iK_%s%i.zero.csv   ./Data/%s/%iK/csv/zero/",fDirectory.Data(),fTemperature[iTmp],fLetters[iLtr],iTer,fSensor.Data(),fTemperature[iTmp],fLetters[iLtr],iTer,fSensorName.Data(),fTemperature[iTmp]));
                    gROOT->ProcessLine(Form(".! scp sipm4eic@131.154.97.95:%s%iK_%s%i/%s_%iK_%s%i.ivscan.csv ./Data/%s/%iK/csv/ivscan/",fDirectory.Data(),fTemperature[iTmp],fLetters[iLtr],iTer,fSensor.Data(),fTemperature[iTmp],fLetters[iLtr],iTer,fSensorName.Data(),fTemperature[iTmp]));
                    gROOT->ProcessLine(Form(".! scp sipm4eic@131.154.97.95:%s%iK_%s%i/%s_%iK_%s%i.fwdscan.csv ./Data/%s/%iK/csv/fwdscan/",fDirectory.Data(),fTemperature[iTmp],fLetters[iLtr],iTer,fSensor.Data(),fTemperature[iTmp],fLetters[iLtr],iTer,fSensorName.Data(),fTemperature[iTmp]));
                }
            }
        }
    }
}
