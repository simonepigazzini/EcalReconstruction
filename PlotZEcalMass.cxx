#include <vector>
#include <map>
#include <algorithm> 
#include <iostream>

#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLegend.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TPaveText.h>
#include <TGAxis.h>
#include <TObjArray.h>

using namespace std;

TLegend* doLegend(std::map<TString, TH1F*> pmap, std::vector<TString> style, float textSize=0.035, float legWidth=0.18) {

  float x1 = 0.93-legWidth;
  float y1 = 0.75 - textSize*std::max(float(pmap.size())-3.,0.);
  float x2 = .93;
  float y2 = .93;

  TLegend *leg = new TLegend(x1,y1,x2,y2);
  leg->SetFillColor(0);
  leg->SetShadowColor(0);
  leg->SetLineColor(0);
  leg->SetTextFont(42);
  leg->SetTextSize(textSize);

  std::map<TString, TH1F*>::const_iterator it;
  int i=0;
  for(it=pmap.begin();it!=pmap.end();++it) {
    TString lbl=it->first;
    TH1F *plot=it->second;
    leg->AddEntry(plot,lbl,style[i]);
    ++i;
  }
  return leg;
}

TPaveText* doSpam(TString text,float x1,float y1,float x2,float y2,int align=12,bool fill=false,
		  float textSize=0.033) {
    TPaveText *cmsprel = new TPaveText(x1,y1,x2,y2,"NDC");
    cmsprel->SetTextSize(textSize);
    cmsprel->SetFillColor(0);
    cmsprel->SetFillStyle(fill ? 1001 : 0);
    cmsprel->SetLineStyle(2);
    cmsprel->SetLineColor(0);
    cmsprel->SetTextAlign(align);
    cmsprel->SetTextFont(42);
    cmsprel->AddText(text);
    cmsprel->Draw("same");
    return cmsprel;
}

void doTinyCmsPrelim(TString textLeft,TString textRight,float textSize=0.033,float lumi=2.11,float xoffs=0) {
  doSpam(textLeft, .30+xoffs, .955, .60+xoffs, .995, 12, textSize);
  doSpam(textRight,.66+xoffs, .955, .97+xoffs, .995, 32, textSize);
}

void plotMasses(const char* file, bool iseb, const char *plotpref) {

  gROOT->ProcessLine(".x tdrstyle.cc");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TGaxis::SetMaxDigits(3);

  TFile *tfile = TFile::Open(file);
  
  TH1F *mass_e5x5_data = (TH1F*)tfile->Get("mass_e5x5_data");
  TH1F *mass_ScRaw_data = (TH1F*)tfile->Get("mass_ScRaw_data");
  TH1F *mass_ScRawES_data = ( iseb ? 0 : (TH1F*)tfile->Get("mass_ScRawES_data") );
  TH1F *mass_corrEcEnergy_data = (TH1F*)tfile->Get("mass_corrEcEnergy_data");

  mass_corrEcEnergy_data->SetLineColor(kBlack);
  mass_corrEcEnergy_data->SetLineWidth(2);
  mass_corrEcEnergy_data->GetXaxis()->SetTitle("m(e^{+}e^{-}) [GeV]");
  mass_corrEcEnergy_data->GetYaxis()->SetTitle("Events / GeV");
  mass_corrEcEnergy_data->GetYaxis()->SetTitleOffset(1.3);

  mass_ScRaw_data->SetLineColor(kBlue+2);
  mass_ScRaw_data->SetFillColor(kBlue+2);
  mass_ScRaw_data->SetFillStyle(3002);
  if (!iseb) {
    mass_ScRawES_data->SetLineColor(kMagenta+1);
    mass_ScRawES_data->SetLineWidth(2);
  }
  mass_e5x5_data->SetLineColor(kOrange-3);
  mass_e5x5_data->SetFillColor(kOrange-3);
  mass_e5x5_data->SetFillStyle(3002);

  std::map<TString, TH1F*> pmap;  
  std::vector<TString> style;
  
  pmap.insert(std::pair<TString,TH1F*>("E^{SuperCluster}_{corrected}",mass_corrEcEnergy_data));
  if(!iseb)  pmap.insert(std::pair<TString,TH1F*>("E^{SuperCluster}_{raw+ES}",mass_ScRawES_data));
  pmap.insert(std::pair<TString,TH1F*>("E^{SuperCluster}_{raw}",mass_ScRaw_data));
  pmap.insert(std::pair<TString,TH1F*>("E_{5x5 crystals}",mass_e5x5_data));

  style.push_back("l");
  if(!iseb) style.push_back("l");
  style.push_back("fl");
  style.push_back("fl");

  TCanvas *c1 = new TCanvas("c1","c1");
  mass_corrEcEnergy_data->Draw("hist");
  mass_e5x5_data->Draw("same hist");
  mass_ScRaw_data->Draw("same hist");
  if(!iseb) mass_ScRawES_data->Draw("same hist");

  TLegend *leg = doLegend(pmap,style);
  leg->Draw();

  if(iseb) doSpam("Barrel-Barrel", .25, .80, .50, .90, 12);
  else doSpam("Endcap-Endcap", .25, .80, .50, .90, 12);
  

  doTinyCmsPrelim("CMS Preliminary","2.11 fb^{-1} (13 TeV)");

  c1->SaveAs(Form("%s.png",plotpref));
  c1->SaveAs(Form("%s.pdf",plotpref));
  

}

void plotAllMassEstimates() {
  plotMasses("canvases/zee_incl_plots_bb.root",true,"zee_mass_bb");
  plotMasses("canvases/zee_incl_plots_ee.root",false,"zee_mass_ee");
}


void plotDataMC(const char* file, bool iseb) {
  //  gROOT->ProcessLine(".x tdrstyle.cc");
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TGaxis::SetMaxDigits(3);

  TFile *tfile = TFile::Open(file);
  TCanvas *cheppy = (TCanvas*)tfile->Get("mass_corrEcEnergy_calib_canvas");

  cheppy->Draw();
  if(iseb) doSpam("Barrel-Barrel", .25, .80, .50, .90, 12);
  else doSpam("Not Barrel-Barrel", .25, .80, .50, .90, 12);
  
}
