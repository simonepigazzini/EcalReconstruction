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

#include "/Users/emanuele/Scripts/RooHZZStyle.C"

double effectiveSigma(TH1 *histo);

void plotMass(const char *file, const char *postfix="") {

   gStyle->SetOptStat(0);

  std::string detkey[3] = {"bb","be","ee"};
  std::string ekind[3] = {"global","mf50","mf0"};
  int color[3] = {kRed,kBlack,kViolet};
  float scaleratio[3];

  TH1D* h_mass[3][3];

  TFile *tfile = TFile::Open(file);

  TCanvas *c1 = new TCanvas("c1","",600,300);
  for(int idet=0; idet<3; ++idet) {

    for(int ie=0; ie<3; ++ie) {
      h_mass[idet][ie] = (TH1D*)tfile->Get(Form("h_mass_%s_%s",ekind[ie].c_str(),detkey[idet].c_str()));
      h_mass[idet][ie]->SetLineColor(color[ie]);
      h_mass[idet][ie]->SetMarkerColor(color[ie]);
      // h_mass[idet][ie]->Sumw2();
      // h_mass[idet][ie]->Scale(1./h_mass[idet][ie]->Integral());
      if(idet==0) h_mass[idet][ie]->SetAxisRange(60,110);
      else {
	h_mass[idet][ie]->SetAxisRange(60,110);
	//	h_mass[idet][ie]->Rebin(2);
      }

      h_mass[idet][ie]->GetXaxis()->SetTitle("m_{ ee} (GeV)");

      scaleratio[ie] = h_mass[idet][ie]->GetMean()/h_mass[idet][0]->GetMean();
      

      if(ie==0) {
	h_mass[idet][ie]->Draw("hist");
	h_mass[idet][ie]->Draw("pe same");
      }	
      else {
	h_mass[idet][ie]->Draw("hist same");
	h_mass[idet][ie]->Draw("pe same");
      }
    }
    

    TLegend *legend = new TLegend(0.13,0.70,0.50,0.85,NULL,"brNDC");
    legend->SetBorderSize(     0);
    legend->SetFillColor (     0);
    legend->SetTextAlign (    12);
    legend->SetTextFont  (    42);
    legend->AddEntry     (    h_mass[idet][0], Form("weights: #sigma_{eff} = %.2f GeV",effectiveSigma( h_mass[idet][0])/scaleratio[0]));
    legend->AddEntry     (    h_mass[idet][1], Form("5-pulse fit: #sigma_{eff} = %.2f GeV",effectiveSigma( h_mass[idet][1])/scaleratio[1]));
    legend->AddEntry     (    h_mass[idet][2], Form("1-pulse fit: #sigma_{eff} = %.2f GeV",effectiveSigma( h_mass[idet][2])/scaleratio[2]));
    legend->Draw();

    c1->SaveAs(Form("mass_%s_%s.pdf",detkey[idet].c_str(),postfix));
    delete legend;
  }


}

void scatterplot(const char *file) {
   gStyle->SetOptStat(0);
   
   std::string detkey[2] = {"bb","ee"};
   std::string ekind[3] = {"mf25","mf50","mf0"};
   std::string axlabel[3] = {"E_{5x5} (10-pulse fit) (GeV)","E_{5x5} (5-pulse fit) (GeV)","E_{5x5} (1-pulse fit) (GeV)"};

   TH2D* h_energy[2][3];

  TFile *tfile = TFile::Open(file);

  TCanvas *c1 = new TCanvas("c1","",600,600);
  c1->SetLogz();
  for(int idet=0; idet<2; ++idet) {
    for(int ie=0; ie<3; ++ie) {
      h_energy[idet][ie] = (TH2D*)tfile->Get(Form("h2_glob_%s_%s",ekind[ie].c_str(),detkey[idet].c_str()));
      h_energy[idet][ie]->GetXaxis()->SetTitle("E_{5x5} (weights) (GeV)");
      h_energy[idet][ie]->GetYaxis()->SetTitle(axlabel[ie].c_str());
      h_energy[idet][ie]->Draw("colz");
      c1->SaveAs(Form("%s.pdf",h_energy[idet][ie]->GetName()));
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


void drawTH2(TH2* plot, const char* xtitle, const char* ytitle) {

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();

  TCanvas *c1 = new TCanvas("c1","",600,600);
  plot->GetXaxis()->SetTitle(xtitle);
  plot->GetYaxis()->SetTitle(ytitle);

  plot->Draw("colz");
  c1->SaveAs(Form("%s_mcinputs.pdf",plot->GetName()));
  c1->SaveAs(Form("%s_mcinputs.png",plot->GetName()));

}

void plotResiduals(const char *file) {

  TFile *tfile = TFile::Open(file);
  TTree *tree = (TTree*)tfile->Get("probe_tree");
  
  TH2D *deltaE_mf50 = new TH2D("deltaE_mf50","",500,0,500,500,-20,20);
  TH2D *deltaE_mf25 = new TH2D("deltaE_mf25","",500,0,500,500,-20,20);
  TH2D *deltaE_mf0 = new TH2D("deltaE_mf0","",500,0,500,500,-20,20);

  std::vector<TH2D*> h2d;
  h2d.push_back(deltaE_mf50);
  h2d.push_back(deltaE_mf25);
  h2d.push_back(deltaE_mf0);
   
  std::string variables[3] = {"energy_mf50","energy_mf25","energy_mf0"};
  std::string titles[3] = {"(E_{MF 50ns}-E_{3+5})/E_{3+5} [%]","(E_{MF 25ns}-E_{3+5})/E_{3+5} [%]","(E_{MF 0PU}-E_{3+5})/E_{3+5} [%]"};

  std::string subdet[2] = {"b","e"};
  std::string l1subdetcut[2] = {"abs(l1eta)<1.479","abs(l1eta)>1.479"};
  std::string l2subdetcut[2] = {"abs(l2eta)<1.479","abs(l2eta)>1.479"};

  for(int h=0; h<(int)h2d.size();++h) {
    for(int idet=0;idet<2;++idet)  {
      TH2D *scatter = (TH2D*)h2d[h]->Clone(Form("%s_%s",h2d[h]->GetName(),subdet[idet].c_str()));
      tree->Project(scatter->GetName(),Form("100.*(l1%s-l1e nergy_weights)/l1energy_weights:l1energy_weights",variables[h].c_str()),l1subdetcut[idet].c_str());
      tree->Project(scatter->GetName(),Form("100.*(l2%s-l2energy_weights)/l2energy_weights:l2energy_weights",variables[h].c_str()),l2subdetcut[idet].c_str());
      drawTH2(scatter,"E_{3+5} (GeV)",titles[h].c_str());
    }
  }

}



void plotInclusiveMass(const char *file, const char *postfix="") {

   gStyle->SetOptStat(0);

  std::string ekind[3] = {"weights","mf25","mf50"};
  int color[3] = {kRed,kBlack,kViolet};
  float scaleratio[3];

  TH1D* spectrum = new TH1D("spectrum","",20,40,120);
  TH1D* h_mass[3];
  

  TFile *tfile = TFile::Open(file);
  TTree *ttree = (TTree*)tfile->Get("probe_tree");

  TCanvas *c1 = new TCanvas("c1","",600,300);
  for(int ie=0; ie<2; ++ie) {
    TString nameh = Form("h_mass_%s",ekind[ie].c_str());
    h_mass[ie] = (TH1D*)spectrum->Clone(nameh);
    ttree->Project(nameh,Form("mass_%s",ekind[ie].c_str()));
    h_mass[ie]->SetLineColor(color[ie]);
    h_mass[ie]->SetMarkerColor(color[ie]);
    
    h_mass[ie]->GetXaxis()->SetTitle("m_{ ee} (GeV)");
    h_mass[ie]->GetYaxis()->SetRangeUser(0,40);
    
    if(ie==0) {
      h_mass[ie]->Draw("hist");
      //      h_mass[ie]->Draw("pe same");
    }	
    else {
      h_mass[ie]->Draw("hist same");
      //      h_mass[ie]->Draw("pe same");
    }
  }
  
  
  TLegend *legend = new TLegend(0.13,0.70,0.50,0.85,NULL,"brNDC");
  legend->SetBorderSize(     0);
  legend->SetFillColor (     0);
  legend->SetTextAlign (    12);
  legend->SetTextFont  (    42);
  legend->AddEntry     (    h_mass[0], "weights");
  legend->AddEntry     (    h_mass[1], "10-pulse fit");
  //  legend->AddEntry     (    h_mass[2], "5-pulse fit");
  legend->Draw();
  
  c1->SaveAs(Form("mass_%s.pdf",postfix));
  delete legend;


}
