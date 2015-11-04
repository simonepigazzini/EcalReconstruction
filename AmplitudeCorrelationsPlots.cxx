#include "TFile.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TH2D.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TString.h"

void plotITvsOOT(const char *file, const char *prefix, bool dobarrel) {

  gStyle->SetOptStat(0);

  TFile *tfile = TFile::Open(file);
  TTree *tree;
  if(dobarrel) tree = (TTree*)tfile->Get("dataTree");
  else tree = (TTree*)tfile->Get("dataTree2");

  TString detstr = (dobarrel ? "eb" : "ee");

  TString pfx(prefix);
  pfx += "_" + detstr;


  // correlations of IT amplitude with OOT ones
  TCanvas c0("c0","",600,500);
  TH2D *ene_it_vs_oott = new TH2D("ene_it_vs_oott","",100,0,1000,100,0,30);
  ene_it_vs_oott->GetXaxis()->SetTitle("BX=0 amplitude (ADC)");
  for(int i=0; i<10; ++i) {
    if(i==5) continue;
    int bxn = i-5;
    ene_it_vs_oott->GetYaxis()->SetTitle(Form("BX=%d amplitude (ADC)",bxn));
    ene_it_vs_oott->Reset();
    TString var2d = Form("uncalibrechit_multi10_ootampl_%s[%d]:uncalibrechit_multi10_ampl_%s",detstr.Data(),i,detstr.Data());
    TCut ootcut = Form("uncalibrechit_multi10_ootampl_%s[%d]<100",detstr.Data(),i);
    TCut itcut = Form("uncalibrechit_multi10_ampl_%s<1000",detstr.Data());
    tree->Project(ene_it_vs_oott->GetName(), var2d, ootcut && itcut);
    ene_it_vs_oott->Draw("colz");
    c0.SetLogz();
    TString name = Form("ampli_it_vs_oot_%s_%d.pdf",pfx.Data(),i);
    c0.SaveAs(name);
  }

}

void plotDeltaEvsOOT(const char *file, const char *prefix, bool dobarrel) {

  gStyle->SetOptStat(0);

  TFile *tfile = TFile::Open(file);
  TTree *tree;
  if(dobarrel) tree = (TTree*)tfile->Get("dataTree");
  else tree = (TTree*)tfile->Get("dataTree2");

  TString detstr = (dobarrel ? "eb" : "ee");

  TString pfx(prefix);
  pfx += "_" + detstr;


  // correlations of IT amplitude with OOT ones
  TCanvas c0("c0","",600,500);
  TH2D *deltae_vs_oott = new TH2D("deltae_vs_oott","",100,0,30,100,-0.5,0.5);
  deltae_vs_oott->GetYaxis()->SetTitle("hit energy (new - std) (GeV)");
  for(int i=0; i<10; ++i) {
    if(i==5) continue;
    int bxn = i-5;
    deltae_vs_oott->GetXaxis()->SetTitle(Form("BX=%d amplitude (ADC)",bxn));
    deltae_vs_oott->Reset();
    TString var2d = Form("rechit_ene_multi10-rechit_ene_multi10_old:uncalibrechit_multi10_ootampl_%s[%d]",detstr.Data(),i);
    TCut ootcut = Form("uncalibrechit_multi10_ootampl_%s[%d]<100",detstr.Data(),i);
    tree->Project(deltae_vs_oott->GetName(), var2d, ootcut);
    deltae_vs_oott->Draw("colz");
    c0.SetLogz();
    TString name = Form("deltae_vs_oot_%s_%d.pdf",pfx.Data(),i);
    c0.SaveAs(name);
  }

}
