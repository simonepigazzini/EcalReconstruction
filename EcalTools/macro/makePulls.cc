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
#include <TBinomialEfficiencyFitter.h>

#include "RooHZZStyle.C"

void makePulls(const char* filein, const char *fileout) {

  int ev0=0;
  int ix0, iy0, npu0;
  float a0,t0;

  int ev1=-1;
  int ix1, iy1, npu1;
  float a1,t1;

  std::cout << "Reading file " << filein << ", will put the output in " << fileout << std::endl;

  TFile *fileo = TFile::Open(fileout,"recreate");
  TTree *tree = new TTree("tree","");
  TTree *treeFake = new TTree("tree_fake","");
  
  tree->Branch("ix",&ix0,"ix/I");
  tree->Branch("iy",&iy0,"iy/I");
  tree->Branch("a0",&a0,"a0/F");
  tree->Branch("a1",&a1,"a1/F");
  tree->Branch("t0",&t0,"t0/F");
  tree->Branch("t1",&t1,"t1/F");

  treeFake->Branch("ix",&ix0,"ix/I");
  treeFake->Branch("iy",&iy0,"iy/I");
  treeFake->Branch("a1",&a1,"a1/F");
  treeFake->Branch("t1",&t1,"t1/F");

  ifstream myfile(filein);
  int ih=0;
  if (myfile.is_open()) {
    while ( !myfile.eof() ) {
      myfile >> ev1 >> ix1 >> iy1 >> npu1 >> a1 >> t1;
      if(ih%1000000==0) std::cout << "Hit # " << ih << std::endl;

      if(npu1==0) { npu0=npu1; ev0=ev1; ix0=ix1; iy0=iy1; a0=a1; t0=t1; continue;}
      else if(ev1==ev0 && ix1==ix0 && iy1==iy0) {
	tree->Fill();
      } else {
	treeFake->Fill();
      }
      ih++;
    }
  }
  myfile.close();

  fileo->cd();
  tree->Write();
  treeFake->Write();
  fileo->Close();

}



void plotEEAmpliResolution(const char *filein) {

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();

  TFile *file = TFile::Open(filein);
  TTree *tree = (TTree*)file->Get("tree");

  int ix,iy;
  float a0,a1,t0,t1;
  float minA=5.0; // ADC 

  tree->SetBranchAddress("ix",&ix);
  tree->SetBranchAddress("iy",&iy);
  tree->SetBranchAddress("a0",&a0);
  tree->SetBranchAddress("a1",&a1);
  tree->SetBranchAddress("t0",&t0);
  tree->SetBranchAddress("t1",&t1);

  TProfile2D *ampli0 = new TProfile2D("ampli0","",100,0,100,100,0,100,"s");
  TProfile2D *ampliD = new TProfile2D("ampliD","",100,0,100,100,0,100,"s");
  TH2D *ampliReso = new TH2D("ampliReso","",100,0,100,100,0,100);

  TProfile2D *timeD = new TProfile2D("timeD","",100,0,100,100,0,100,"s");

  for(int i=0;i<tree->GetEntries(); ++i) {
    tree->GetEntry(i);
    if(i%1000==0) cout << "Processing hit = " << i << endl;
    if(fabs(a0-a1)<100) { // remove outliers
      ampliD->Fill(ix,iy,a1-a0);
      ampli0->Fill(ix,iy,a0);
      if(fabs(t0-t1)<100) timeD->Fill(ix,iy,t1-t0);      
    }
  }

  for(int x=0;x<100;++x) {
    for(int y=0;y<100;++y) {
      float r=ampliD->GetBinError(x,y)/ampli0->GetBinContent(x,y);
      if(ampli0->GetBinEntries(ampli0->GetBin(x,y))>0) ampliReso->SetBinContent(x,y,r);
    }
  }
  ampliReso->SetMinimum(0.0000001);

  ampliD->GetZaxis()->SetRangeUser(-3,3);
  ampliD->GetXaxis()->SetTitle("ix");
  ampliD->GetYaxis()->SetTitle("iy");
  ampliD->GetZaxis()->SetTitle("A_{140PU}-A_{0PU} (adc)");

  ampliReso->GetXaxis()->SetTitle("ix");
  ampliReso->GetYaxis()->SetTitle("iy");
  ampliReso->GetZaxis()->SetTitleOffset(1.2);
  ampliReso->GetZaxis()->SetRangeUser(0,0.4);
  ampliReso->GetZaxis()->SetTitle("#sigma_{A} / A");

  timeD->GetZaxis()->SetRangeUser(-3,3);
  timeD->GetXaxis()->SetTitle("ix");
  timeD->GetYaxis()->SetTitle("iy");
  timeD->GetZaxis()->SetTitle("T_{140PU}-T_{0PU} (clocks)");

  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
  TCanvas *c0 = new TCanvas("c0","deltas",600,600);
  c0->SetLeftMargin(0.20);
  c0->SetRightMargin(0.20);
  c0->SetTopMargin(0.20);
  c0->SetBottomMargin(0.20); 

  ampliD->Draw("colz");
  c0->SaveAs("deltaA.pdf");
  c0->SaveAs("deltaA.png");

  timeD->Draw("colz");
  c0->SaveAs("deltaT.pdf");
  c0->SaveAs("deltaT.png");
  
  ampliReso->Draw("colz");
  c0->SaveAs("sigmaA.pdf");
  c0->SaveAs("sigmaA.png");
  
}

float sigmat(float ampli, bool isEB=false) {
  // only EB numbers at the moment
  float N=33.2 / 25.;
  float C=0.154 / 25.;
  float sigmaN=(isEB) ? 1.2 : 2.2;
  return sqrt(pow(N/(ampli/sigmaN),2) + pow(C,2));
}

void plotEfficiency(const char *filein, bool dofakes=false) {

  float NsigmaCut = 5.0;
  float corr=1.5;

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();

  TFile *file = TFile::Open(filein);
  TTree *tree;
  if(dofakes) tree = (TTree*)file->Get("tree_fake");
  else tree = (TTree*)file->Get("tree");

  int ix,iy;
  float a1,t1;

  tree->SetBranchAddress("ix",&ix);
  tree->SetBranchAddress("iy",&iy);
  tree->SetBranchAddress("a1",&a1);
  tree->SetBranchAddress("t1",&t1);

  TH2D *allFakeHits = new TH2D("allFakeHits","",100,0,100,100,0,100);
  TH2D *passFakeHits = new TH2D("passFakeHits","",100,0,100,100,0,100);

  for(int i=0;i<tree->GetEntries(); ++i) {
    tree->GetEntry(i);
    if(i%1000000==0) cout << "Processing hit = " << i << endl;
    allFakeHits->Fill(ix,iy);
    //    float ir=sqrt(pow(ix-50,2)+pow(iy-50,2));
    if(fabs(t1) < NsigmaCut*corr*sigmat(a1,false)) passFakeHits->Fill(ix,iy);
  }

  cout << passFakeHits->GetEntries() << endl;
  cout << allFakeHits->GetEntries() << endl;
  
  
  passFakeHits->Sumw2();
  TH2D *fakerate = dynamic_cast<TH2D*>(passFakeHits->Clone("fakerate"));
  fakerate->Divide(fakerate, allFakeHits, 1.0, 1.0, "B");

  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
  TCanvas *c0 = new TCanvas("c0","fakerate",600,600);
  c0->SetLeftMargin(0.20);
  c0->SetRightMargin(0.20);
  c0->SetTopMargin(0.20);
  c0->SetBottomMargin(0.20); 

  fakerate->GetXaxis()->SetTitle("ix");
  fakerate->GetYaxis()->SetTitle("iy");
  fakerate->GetZaxis()->SetRangeUser(0,(dofakes) ? 0.2 : 1.0);
  fakerate->GetZaxis()->SetTitleOffset(1.2);
  fakerate->GetZaxis()->SetTitle("efficiency");

  fakerate->Draw("colz");
  if(dofakes) {
    c0->SaveAs("fakerate.pdf");
    c0->SaveAs("fakerate.png");
  } else {
    c0->SaveAs("efficiency.pdf");
    c0->SaveAs("efficiency.png");
  }

}
