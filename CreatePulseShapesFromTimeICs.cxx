#include <vector>
#include <string>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "THashList.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "/Users/emanuele/Scripts/RooHZZStyle.C"

Double_t alphabeta( Double_t *x, Double_t * par)
{

  // par[0] = normalization
  // par[1] = alpha
  // par[2] = beta
  // par[3] = tmax
  // par[4] = pedestal
  
  double alphabeta = par[1]*par[2];

  double deltat = x[0]-par[3];
  double power_term = TMath::Power(1+deltat/par[1]/par[2],par[1]);
  double decay_term = TMath::Exp(-deltat/par[2]);
  double fcn;
  if(deltat>-alphabeta) fcn = par[0] * power_term * decay_term + par[4];
  else fcn = par[4];
  return fcn;
}

TH1D *simPulseShapeTemplate(bool barrel) {

  double EBPulseShapeTemplate[12] = { 1.13979e-02, 7.58151e-01, 1.00000e+00, 8.87744e-01, 6.73548e-01, 4.74332e-01, 3.19561e-01, 2.15144e-01, 1.47464e-01, 1.01087e-01, 6.93181e-02, 4.75044e-02 };
  double EEPulseShapeTemplate[12] = { 1.16442e-01, 7.56246e-01, 1.00000e+00, 8.97182e-01, 6.86831e-01, 4.91506e-01, 3.44111e-01, 2.45731e-01, 1.74115e-01, 1.23361e-01, 8.74288e-02, 6.19570e-02 };
  
  TH1D *simPulseShape = new TH1D("simPulseShape","",15,0,15);

  int SAMPLES=15;
  int firstSignalSample=3;
  for(int i=0; i<SAMPLES; ++i) {
    if(i<firstSignalSample) simPulseShape->SetBinContent(i+1, 1E-10);
    else {
      simPulseShape->SetBinContent(i+1, (barrel ? EBPulseShapeTemplate[i-firstSignalSample] : EEPulseShapeTemplate[i-firstSignalSample]));
      simPulseShape->SetBinError(i+1, simPulseShape->GetBinContent(i+1)*0.01);
    }
  }

  return simPulseShape;

}


TH1D* makeSingleTemplate(float dt, bool EB) {

  TH1D *tempH = new TH1D("tempH","",15,0,15);
  TF1 *fitF = new TF1("alphabeta",alphabeta,0,15,5);

  // those are the new alpha-beta parameters found by fitting the sim pulse shape
  double alpha = EB ? 1.250 : 1.283;
  double beta = EB ? 1.600 : 1.674;

  // time for alphabeta function is in units of 25 ns
  double unitdt = dt/25.;

  fitF->SetParNames("norm","#alpha","#beta","tmax","pedestal","raiset");
  fitF->FixParameter(0,1); // normalization
  fitF->FixParameter(1,alpha); // alpha
  fitF->FixParameter(2,beta); // beta
  fitF->FixParameter(3,5.5 + unitdt); // tmax
  fitF->FixParameter(4,0); // pedestal

  for(int i=0;i<15;++i) {
    double val = 0.0; 
    int firstNonPed = EB ? 2 : 2;
    if(i>firstNonPed) { 
      val = fitF->Eval(i+0.5)/fitF->Eval(5.5);
    } else  tempH->SetBinContent(i+1, 0.0);
    tempH->SetBinContent(i+1, val); 
  }

  //  tempH->Draw();
  return tempH;
}


void testIT(bool EB) {

  TH1D *analTemp = makeSingleTemplate(0,EB);
  analTemp->SetLineColor(kRed);

  TH1D *simTemp = simPulseShapeTemplate(EB);
  simTemp->SetLineColor(kBlack);
  
  TCanvas *c1 = new TCanvas("c1","",1200,1200);
  analTemp->Draw();
  simTemp->Draw("same");
  c1->SaveAs(Form("testIT_%s.pdf",(EB ? "EB":"EE")));

}

void makeTimeCalibratedTemplates(const char* timeICDump="EcalTimeCalibConstants_newIOV.dat") {

  ifstream icdump;
  icdump.open(timeICDump, std::ifstream::in);

  ofstream outtxt("template_histograms_timeIC.txt", ios::out | ios::trunc);

  unsigned int rawId;
  int ix,iy,iz;
  double time, boh;
    
  
  while (icdump.good()) {
    icdump.get();
    icdump >> ix >> iy >> iz >> time >> boh >> rawId ;
    string subdet;
    if(iz==0) subdet="EB";
    else if(iz>0) subdet="EE+";
    else subdet="EE-";

    outtxt.unsetf ( std::ios::floatfield ); 
    outtxt << ( (iz==0) ? 1 : 0 ) << "\t";
    outtxt << rawId << "\t";
    outtxt.precision(6);
    outtxt.setf( std::ios::fixed, std:: ios::floatfield ); // floatfield set to fixed
    
    float pdfval[12];
    TH1D *shiftedTemp = makeSingleTemplate(time,(iz==0));
    for(int iSample=3; iSample<15; iSample++) {
      pdfval[iSample-3] = shiftedTemp->GetBinContent(iSample+1);
      outtxt << pdfval[iSample-3] << "\t";
    }
    delete shiftedTemp;
    outtxt << std::endl;
  }

  outtxt.close();

}
