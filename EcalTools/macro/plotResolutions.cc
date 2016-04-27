// C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// ROOT includes
#include <TROOT.h>
#include <TStyle.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1D.h>
#include <TProfile2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveStats.h>
#include <TLatex.h>
#include <TString.h>
#include <TBinomialEfficiencyFitter.h>

#include "RooHZZStyle.C"

double effectiveSigma(TH1 *histo);

void plotResolutions(const char *file) {

  vector<TH1F*> resolutions_EB_1, resolutions_EE_1, resolutions_EB_2, resolutions_EE_2;
  //               [type][eta][pt]
  TH1F *resolutions[3][4][5];
  TFile *tfile = TFile::Open(file);

  // get the histograms from the files
  for(int clustertype=0;clustertype<3;++clustertype) {
    if(clustertype==1) continue;
    for(int e=0;e<4;++e) {
      for(int p=0;p<5;++p) {
        char namer[50];
        sprintf(namer,"cluster%d_res_eta%d_e%d",clustertype,e,p);
        resolutions[clustertype][e][p] = (TH1F*)tfile->Get(namer);
        if(p<2) resolutions[clustertype][e][p]->Rebin(20);
        else resolutions[clustertype][e][p]->Rebin(5);
      }
    }
  }

  // plot
  gStyle->SetOptStat(0);

  float effSigma[3][2][5], coreSigma[3][2][5];
  float mean[3][2][5];
  float xsigmaToFit=1.;

  // the sample is pT: [1-100] GeV
  int ptbins[7] = {1,5,10,30,50,100,300};

  TCanvas *c1 = new TCanvas("c1","",600,600);
  TLatex* CP = new TLatex(0.15,0.96, "CMS Gun Simulation                                               #sqrt{s} = 8 TeV");
  CP->SetNDC(kTRUE);
  CP->SetTextSize(0.030);
  
  for(int idet=0;idet<4;++idet) {
    for(int p=0;p<5;++p) {
      
      resolutions[0][idet][p]->SetLineColor(kBlack);
      // resolutions[1][idet][p]->SetLineColor(kRed+1);
      resolutions[2][idet][p]->SetLineColor(kGreen+1);
      
      //       float maxy = std::max(resolutions[0][idet][p]->GetMaximum(),std::max(resolutions[1][idet][p]->GetMaximum(),resolutions[2][idet][p]->GetMaximum()));
      float maxy = std::max(resolutions[0][idet][p]->GetMaximum(),resolutions[2][idet][p]->GetMaximum());
      maxy*=1.2;

      if(p>1) resolutions[0][idet][p]->GetXaxis()->SetRangeUser(-0.15,0.15);
      resolutions[0][idet][p]->GetXaxis()->SetTitle("E_{5x5}/E_{true}");

      resolutions[0][idet][p]->Draw();
      //resolutions[1][idet][p]->Draw("sames");
      resolutions[2][idet][p]->Draw("sames");

      char ptrange[50];
      sprintf(ptrange,"%d < E_{gen} < %d GeV", ptbins[p], ptbins[p+1]);
      TLatex *ptr = new TLatex(0.15,0.70,ptrange);
      ptr->SetNDC(kTRUE);
      ptr->SetTextSize(0.040);
      ptr->Draw();

      effSigma[0][idet][p] = effectiveSigma(resolutions[0][idet][p]);
      effSigma[2][idet][p] = effectiveSigma(resolutions[2][idet][p]);
      /*
      mean[0][idet][p] = resolutions[0][idet][p]->GetMean();
      mean[2][idet][p] = resolutions[2][idet][p]->GetMean();

      // first pass, rough
      resolutions[0][idet][p]->Fit("gaus","","QN",mean[0][idet][p]-3*effSigma[0][idet][p],mean[0][idet][p]+3*effSigma[0][idet][p]);
      mean[0][idet][p] = resolutions[0][idet][p]->GetFunction("gaus")->GetParameter(1);
      resolutions[2][idet][p]->Fit("gaus","","QN",mean[2][idet][p]-3*effSigma[2][idet][p],mean[2][idet][p]+3*effSigma[2][idet][p]);
      mean[2][idet][p] = resolutions[2][idet][p]->GetFunction("gaus")->GetParameter(1);
      
      // last pass, draw
      resolutions[0][idet][p]->Fit("gaus","","same",mean[0][idet][p]-xsigmaToFit*effSigma[0][idet][p],mean[0][idet][p]+xsigmaToFit*effSigma[0][idet][p]);
      resolutions[0][idet][p]->GetFunction("gaus")->SetLineColor(kBlack);
      resolutions[2][idet][p]->Fit("gaus","","same",mean[2][idet][p]-xsigmaToFit*effSigma[2][idet][p],mean[2][idet][p]+xsigmaToFit*effSigma[2][idet][p]);
      resolutions[2][idet][p]->GetFunction("gaus")->SetLineColor(kGreen+1);
      coreSigma[0][idet][p] = resolutions[0][idet][p]->GetFunction("gaus")->GetParameter(2);
      coreSigma[2][idet][p] = resolutions[2][idet][p]->GetFunction("gaus")->GetParameter(2);
      */

      resolutions[0][idet][p]->GetYaxis()->SetRangeUser(0,maxy);
      c1->Update();

      TPaveText *results = new TPaveText(.6,.7,.8,.9,"NDC");
      results->SetBorderSize(0);
      results->SetFillColor (0);
      results->SetTextAlign(12);
      results->SetTextFont(42);
      results->SetTextSize(0.035);
      results->AddText(Form("3+5: #sigma_{eff},=%.1f%%",100.0*effSigma[0][idet][p]));
      //results->AddText(Form("     #sigma_{core},=%.1f%%",100.0*coreSigma[0][idet][p]));
      //results->AddText(Form("DB+5: #sigma_{eff}=%.2f%%",100.0*effectiveSigma(resolutions[1][idet][p])));
      results->AddText(Form("DB+5 MPS: #sigma_{eff}=%.1f%%",100.0*effSigma[2][idet][p]));
      //results->AddText(Form("          #sigma_{core}=%.1f%%",100.0*coreSigma[2][idet][p]));

      results->Draw();

      c1->Update();

      TLegend *legend = new TLegend(0.15,0.75,0.40,0.85,NULL,"brNDC");
      legend->SetBorderSize(     0);
      legend->SetFillColor (     0);
      legend->SetTextAlign (    12);
      legend->SetTextFont  (    42);
  
      legend->AddEntry(resolutions[0][idet][p], "3+5");
      //legend->AddEntry(resolutions[1][idet][p], "DB+5");
      legend->AddEntry(resolutions[2][idet][p], "DB+5 MPS");

      legend->Draw();
      CP->Draw();

      c1->SaveAs(TString("figures/test3_")+resolutions[0][idet][p]->GetName()+TString(".pdf"));
      c1->SaveAs(TString("figures/test3_")+resolutions[0][idet][p]->GetName()+TString(".png"));

    }
  }
  
}

double effectiveSigma(TH1 *histo) {
  
  TAxis *xaxis = histo->GetXaxis();
  Int_t nb = xaxis->GetNbins();
  if(nb < 10) {
    cout << "effsigma: Not a valid histo. nbins = " << nb << endl;
    return 0.;
  }
  
  Double_t bwid = xaxis->GetBinWidth(1);
  if(bwid == 0) {
    cout << "effsigma: Not a valid histo. bwid = " << bwid << endl;
    return 0.;
  }
  // Double_t xmax = xaxis->GetXmax();
  Double_t xmin = xaxis->GetXmin();
  Double_t ave = histo->GetMean();
  Double_t rms = histo->GetRMS();

  Double_t total=0.;
  for(Int_t i=0; i<nb+2; i++) {
    total+=histo->GetBinContent(i);
  }
  if(total < 100.) {
    cout << "effsigma: Too few entries " << total << endl;
    return 0.;
  }
  Int_t ierr=0;
  Int_t ismin=999;
  
  Double_t rlim=0.683*total;
  Int_t nrms=rms/(bwid);    // Set scan size to +/- rms
  if(nrms > nb/10) nrms=nb/10; // Could be tuned...

  Double_t widmin=9999999.;
  for(Int_t iscan=-nrms;iscan<nrms+1;iscan++) { // Scan window centre
    Int_t ibm=(ave-xmin)/bwid+1+iscan;
    Double_t x=(ibm-0.5)*bwid+xmin;
    Double_t xj=x;
    Double_t xk=x;
    Int_t jbm=ibm;
    Int_t kbm=ibm;
    Double_t bin=histo->GetBinContent(ibm);
    total=bin;
    for(Int_t j=1;j<nb;j++){
      if(jbm < nb) {
        jbm++;
        xj+=bwid;
        bin=histo->GetBinContent(jbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
      if(kbm > 0) {
        kbm--;
        xk-=bwid;
        bin=histo->GetBinContent(kbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
    }
    Double_t dxf=(total-rlim)*bwid/bin;
    Double_t wid=(xj-xk+bwid-dxf)*0.5;
    if(wid < widmin) {
      widmin=wid;
      ismin=iscan;
    }   
  }
  if(ismin == nrms || ismin == -nrms) ierr=3;
  if(ierr != 0) cout << "effsigma: Error of type " << ierr << endl;
  
  return widmin;

}
