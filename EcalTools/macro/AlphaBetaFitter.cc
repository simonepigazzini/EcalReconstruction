#ifndef AlphaBetaFitter_H
#define AlphaBetaFitter_H

#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <TH1F.h>
#include <TMath.h>
#include <TF1.h>
#include <TCanvas.h>
#include "tdrstyle.C"
#include "CMS_lumi.h"

double alphabeta(double *x, double *par) {
  // par[0] = normalization
  // par[1] = alpha
  // par[2] = beta
  // par[3] = tmax
  // par[4] = pedestal
  
  double alphabeta = par[1]*par[2];
  double deltat = x[0]-par[3];
  double power_term = TMath::Power(1+deltat/par[1]/par[2], par[1]);
  double decay_term = TMath::Exp(-deltat/par[2]);
  double fcn=0;
  if (deltat > -alphabeta) fcn = par[0] * power_term * decay_term + par[4];
  else fcn = par[4];
  return fcn;
}

class AlphaBetaFitter {

public:

  enum AlphaBetaParameter { alpha, beta, T0 };

  // Constructor, destructor
  AlphaBetaFitter();
  ~AlphaBetaFitter() {}

  void fit(TH1F *histo,bool doEB,float pedestal=0,std::string canvasName="");
  double* getPars() {return _pars;}
  double* getErrs() {return _errs;}
  TF1*    getFcn() {return _fitF;}
  

protected:
  void setParameters(bool doEB, double pedestal);
  
  TF1 *_fitF;
  double _pars[4], _errs[4];
};

AlphaBetaFitter::AlphaBetaFitter() {
  _fitF = new TF1("alphabeta",alphabeta,0,10,5);
  _fitF->SetParNames("norm","#alpha","#beta","tmax","pedestal");
  _fitF->SetLineColor(kRed+1);
}

void AlphaBetaFitter::setParameters(bool doEB, double pedestal) {
  float alpha = doEB ? 1.250 : 1.283;
  float beta = doEB ? 1.600 : 1.674;
  _fitF->FixParameter(1,alpha);
  _fitF->FixParameter(2,beta);
  _fitF->FixParameter(0,1); // normalization
  _fitF->SetParameter(3,5.5);
  if(doEB) _fitF->SetParLimits(1,0.8,2.5);  // alpha
  else _fitF->SetParLimits(1,0.5,2.5);
  _fitF->SetParLimits(2,0.8,2.5);  // beta
  _fitF->SetParLimits(3,4,7); // tmax 
  _fitF->FixParameter(4,pedestal);
}

void AlphaBetaFitter::fit(TH1F *histo,bool doEB,float pedestal,std::string canvasName) {
  
  setParameters(doEB,pedestal);
  
  TCanvas *canv = 0;
  if(canvasName!="") {
    canv = new TCanvas("fitc","",600,600);
    histo->GetXaxis()->SetTitle("Time sample");
    histo->GetYaxis()->SetTitle("Normalized Amplitude");
    histo->GetYaxis()->SetRangeUser(-0.1,1.2);
    histo->Draw("p 2");
  }

  histo->Fit("alphabeta","QM","same",0,10);
 
  for(int p=1; p<4; ++p) {
    _pars[p-1] = _fitF->GetParameter(p);
    _errs[p-1] = _fitF->GetParError(p);
  }

  if(canvasName!="") {
    gROOT->LoadMacro("tdrstyle.C");
    setTDRStyle();
    gStyle->SetOptFit(0);
    _fitF->Draw("same");
    gROOT->LoadMacro("CMS_lumi.h");
    CMS_lumi(canv,-1);
    canv->SaveAs(canvasName.c_str());
    delete canv;
  }
}
#endif

