#include <iostream>
#include <string>
#include "TBits.h"
#include "TH1F.h"
#include "TFile.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TPad.h"
#include "TChain.h"
#include "TStyle.h"
#include "TMath.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TPaveText.h"
#include "TGraphErrors.h"
#include "TLorentzVector.h"

using namespace std;
using namespace RooFit ; ////#define EE_PLUS

void plotPi0vsBx() {

  double bx_num[8]={1,2,3,4,5,6,7,8} ;
  double bx_num_err[8]={0.001,0.001, 0.001,0.001, 0.001,0.0001,0.0001,0.0001} ;
  
  double masse[8]={132.46,134.67,136.62,136.86,136.22,134.71,133.22,131.19} ;
  double masse_err[8]={0.17,0.16,0.17,0.17,0.17,0.16,0.18,0.19} ;
  double masseM[8]={127.81,128.25,128.36,128.35,128.42,128.42,128.38,128.32} ;
  double masseM_err[8]={0.08,0.07,0.08,0.07,0.08,0.06,0.08,0.09} ;
  //  RUN E
  /*
    double masse[8]={132.698,135.576,136.887,137.612,135.991,134.837,133.529,130.688} ;
    double masse_err[8]={0.21,0.19,0.20,0.21,0.20,0.25,0.22,0.25} ;
    double masseM[8]={132.504,134.539,136.425,136.614,136.316,135.048,133.351,131.395} ;
    double masseM_err[8]={0.19,0.18,0.19,0.20,0.19,0.22,0.21,0.21} ;
  */
  //RUNF Multifit  Fill 6325
  //  double masseM[8]={129.9,130.,129.53,130.13,130.80,129.13,130.07,129.23} ;
  //  double masse_err[8]={0.43,0.43,0.43,0.43,0.43,0.45,0.53,0.60} ;
  //   double masse[8]={128.50,128.98,129.05,129.67,128.89,128.71,129.24,129.46} ;
  //  double masseM_err[8]={0.30,0.27,0.28,0.29,0.31,0.29,0.35,0.40} ;
  //RUNF weights fill 6325
  //  double masse[8]={132.19,134.82,136.28,136.98,136.46,134.90,131.95,129.62} ;
  //  double masse_err[8]={0.063,0.063,0.063,0.063,0.073,0.065,0.053,0.050} ;
  //  double masseM[8]={131.0,135.28,135.2,138.87,139.20,137.2,132.5,130.2} ;
  //  double masseM_err[8]={0.011,0.009,0.01,0.01,0.01,0.011,0.01,0.01} ;
  //

  double fact= masse[0] ;
  double factM = masseM[0] ;
  
  for(int i=0 ; i<8 ; i++) {
           
    masse[i]=masse[i]/fact ;
           
    masseM[i]=masseM[i]/factM ;
    masse_err[i]=masse_err[i]/fact ;
    masseM_err[i]=masseM_err[i]/factM ;
    printf(" i %d masse[i] %f masse_err %f \n",i,masse[i],masse_err[i]) ;
  }
  TGraphErrors *graph_mass ;
  graph_mass = new TGraphErrors(8,bx_num,masse,bx_num_err,masse_err) ;
  graph_mass->SetMarkerStyle(kFullCircle) ;
  graph_mass->SetMarkerColor(kMagenta+2) ;
  graph_mass->SetLineColor(kMagenta+2) ;
  graph_mass->SetMarkerSize(1.0) ;
  graph_mass->SetTitle("Weights") ;

  TGraphErrors *graph_massM ;
  graph_massM = new TGraphErrors(8,bx_num,masseM,bx_num_err,masseM_err) ;
  graph_massM->SetMarkerStyle(kFullSquare) ;
  graph_massM->SetMarkerColor(kAzure+7) ;
  graph_massM->SetLineColor(kAzure+7) ;
  graph_massM->SetMarkerSize(1.0) ;
  graph_massM->SetTitle("Multi") ;

  TCanvas *cmasse = new TCanvas("mass_vs_#Bx","mass_vs_#Bx", 0,0,600,600);
  cmasse->SetLeftMargin(0.2);
  cmasse->SetRightMargin(0.05);
  cmasse->SetBottomMargin(0.15);
  cmasse->SetTopMargin(0.10);
  cmasse->cd() ;

  TMultiGraph *mg_masse = new TMultiGraph("","") ;
  mg_masse->Add(graph_mass) ;
  mg_masse->Add(graph_massM) ;
  mg_masse->Draw("ap") ;
  mg_masse->GetYaxis()->SetTitle("m_{#gamma#gamma}/m_{#gamma#gamma}^{BX=1}") ;
  mg_masse->GetYaxis()->SetTitleOffset(1.3);
  mg_masse->GetYaxis()->SetDecimals(2);

  mg_masse->GetXaxis()->SetTitle("bunch crossing number") ;
  mg_masse->SetMaximum(1.05) ;
  mg_masse->SetMinimum(0.98) ;
  mg_masse->Draw("ap") ;

  TLegend *leg = new TLegend(0.60,0.70,0.90,0.85,"","brNDC");
  leg->SetBorderSize(0);
  leg->AddEntry(graph_mass,"weights","pe");
  leg->AddEntry(graph_massM,"multifit","pe");                       
  
  TLatex lat;
  lat.SetNDC(); lat.SetTextFont(42); lat.SetTextSize(0.05);
  lat.DrawLatex(0.20, 0.92, "#bf{CMS}");
  lat.DrawLatex(0.62, 0.92, "0.5 fb^{-1} (13 TeV)");
  lat.DrawLatex(0.35, 0.25, "ECAL Barrel");


  leg->Draw();
  lat.Draw();
  cmasse->Update() ;
  cmasse->SaveAs("pi0vsBx.pdf") ;
}
