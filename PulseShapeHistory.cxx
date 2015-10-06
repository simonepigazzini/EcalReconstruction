#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "THashList.h"
#include "TH2D.h"
#include "TProfile2D.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "/Users/emanuele/Scripts/RooHZZStyle.C"

struct pulseshape {
  double time;
  double alpha;
  double beta;
  double chi2;
  double time_err;
  double alpha_err;
  double beta_err;
};

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

pulseshape getPulseShape(TH1D *pulse, int doEB, double pedestal=0) {

  pulseshape ps;

  double alpha = doEB ? 1.250 : 1.283;
  double beta = doEB ? 1.600 : 1.674;

  TF1 *fitF = new TF1("alphabeta",alphabeta,0,10,5);
  fitF->SetParNames("norm","#alpha","#beta","tmax","pedestal");
  fitF->SetParLimits(0,0,10000); // normalization
  fitF->SetParameter(1,alpha);
  fitF->SetParLimits(1,1.2,1.3);
  fitF->SetParameter(2,beta);
  fitF->SetParLimits(2,1.58,1.8);
  fitF->SetParameter(3,5.5);
  fitF->SetParLimits(3,4,7); // tmax
  fitF->FixParameter(5,pedestal);

  pulse->Fit("alphabeta","Q WW M","",0,10);

  ps.alpha = fitF->GetParameter(1);
  ps.beta = fitF->GetParameter(2);
  ps.time = 25. * fitF->GetParameter(3);

  ps.alpha_err = fitF->GetParError(1);
  ps.beta_err = fitF->GetParError(2);
  ps.time_err = 25. * fitF->GetParError(3);

  ps.chi2 = fitF->GetChisquare();

  // plotting (for debug only: 1 plot / rechit
  /*  
  TCanvas c1("c1","");
  pulse->SetLineColor(kBlue+2);
  pulse->SetLineWidth(2);
  fitF->SetLineColor(kRed+1);

  pulse->GetXaxis()->SetTitle("Sample");
  pulse->GetYaxis()->SetTitle("Ped. Subtracted Amplitude (a.u.)");
  pulse->Draw("hist E2");
  pulse->Fit("alphabeta","Q WW M","same",0,10);
  fitF->Draw("same");
  c1.SaveAs(Form("pulse_a%f_t%f_chisq%f.pdf",rh.amplitude,rh.time,rh.chi2));
  */

  delete fitF;
  return ps;

}

void historyPlots() {

  std::map<int, string> tags;
  tags[254987] = "template_histograms_ECAL_runs_1_999999.txt";
  tags[257645] = "template_histograms_ECAL_runs_257645_257645.txt";
  tags[257682] = "template_histograms_ECAL_runs_257682_257682.txt";
  tags[257721] = "template_histograms_ECAL_runs_257721_257743.txt";
  tags[257751] = "template_histograms_ECAL_runs_257751_257751.txt";

  typedef std::map<int, pulseshape> shapemap;
  std::map<int, shapemap> shapetags;

  TFile *fileOut = TFile::Open("pulseshape_history.root", "recreate");

  TH1D *pulseshapeH = new TH1D("pulseshape","",15,0,15);

  // 2D example maps
  TProfile2D* ebmap = new TProfile2D("ebmap","",360,1,360,170,-85,85);
  TProfile2D* eemap = new TProfile2D("eemap","",100,1,100,100,1,100);

  int iov = 1;
  for(std::map<int, std::string>::iterator it = tags.begin(); it!=tags.end(); ++it) {

    shapemap reference_psmap = shapetags[tags.begin()->first];

    shapemap psmap;

    ifstream tempdump;
    tempdump.open(it->second,  std::ifstream::in);

    unsigned int rawId;
    int iecal, ietaix, iphiiy, iz;
    double normpulse[15];

    TProfile2D* time_eb, *alpha_eb, *beta_eb;
    TProfile2D* time_ee, *alpha_ee, *beta_ee;

    if(iov>1) {
      time_eb  = (TProfile2D*)ebmap->Clone(Form("time_eb_run_%d",it->first));
      alpha_eb = (TProfile2D*)ebmap->Clone(Form("alpha_eb_run_%d",it->first));
      beta_eb  = (TProfile2D*)ebmap->Clone(Form("beta_eb_run_%d",it->first));
      
      time_ee  = (TProfile2D*)eemap->Clone(Form("time_ee_run_%d",it->first));
      alpha_ee = (TProfile2D*)eemap->Clone(Form("alpha_ee_run_%d",it->first));
      beta_ee  = (TProfile2D*)eemap->Clone(Form("beta_ee_run_%d",it->first));
    }
    
    // now read the tag
    int icry = 0;
    while (tempdump.good()) {
      if(icry%1000==0) std::cout << "IOV = " << iov << "\tAnalyzing crystal " << icry << std::endl;  
      tempdump >> iecal >> ietaix >> iphiiy >> iz >> rawId ;
      for(int i=0;i<3;++i) normpulse[i] = 0.0;
      for(int i=3;i<15;++i) tempdump >> normpulse[i];
      
      for(int i=0;i<15;++i) {
	pulseshapeH->SetBinContent(i+1,normpulse[i]);
	pulseshapeH->SetBinError(i+1, pulseshapeH->GetBinContent(i+1)*0.01);
      }

      pulseshape ps = getPulseShape(pulseshapeH, iecal);
      
      psmap[rawId] = ps;

      if(iov > 1) {
	if(ietaix>0) ietaix = ietaix-1;
	if(iecal==1) {
	  time_eb ->Fill(iphiiy, ietaix+0.5, ps.time  - reference_psmap[rawId].time);
	  alpha_eb->Fill(iphiiy, ietaix+0.5, (ps.alpha - reference_psmap[rawId].alpha)/reference_psmap[rawId].alpha_err);
	  beta_eb ->Fill(iphiiy, ietaix+0.5, (ps.beta  - reference_psmap[rawId].beta)/reference_psmap[rawId].beta_err);
	} else {
	  time_ee ->Fill(ietaix, iphiiy, ps.time  - reference_psmap[rawId].time);
	  alpha_ee->Fill(ietaix, iphiiy, (ps.alpha - reference_psmap[rawId].alpha)/reference_psmap[rawId].alpha_err * 100.);
	  beta_ee ->Fill(ietaix, iphiiy, (ps.beta  - reference_psmap[rawId].beta)/reference_psmap[rawId].beta_err * 100.);
	}
      }

      icry++;
    }

    if(iov > 1) {
      time_eb->Write();
      alpha_eb->Write();
      beta_eb->Write();
      
      time_ee->Write();
      alpha_ee->Write();
      beta_ee->Write();
    }

    shapetags[it->first] = psmap;
    iov++;
  }


  fileOut->Close();  

}

void drawAll() {

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1111);

  const int nvars=3;
  const int niovs=4;
  int iovs[niovs] = {257645,257682,257721,257751};
  string titles[nvars] = {"t-t_{0} (ns)", "#alpha variation (n#sigma_{#alpha})", "#beta variation (n#sigma_{#beta})"};
  string vars[nvars] = {"time","alpha","beta"};
  string partitions[2] = {"eb","ee"};

  std::vector<TGraphErrors*> graphs;
  for(int var=0; var<nvars; ++var) {
    graphs.push_back(new TGraphErrors(niovs));
    graphs[var]->SetName(Form("graph_%s",vars[var].c_str()));
  }

  TFile *tfile = TFile::Open("pulseshape_history.root");

  int plotscale=600;
  TCanvas *ceb = new TCanvas("ceb","",plotscale*360./170,plotscale);
  TCanvas *cee = new TCanvas("cee","",plotscale,plotscale);

  for(int iov=0; iov<niovs; ++iov) {
    for(int var=0; var<nvars; ++var) {
      double min = var==0 ? -1.5 : -2.;
      double max = var==0 ?  1.5 : 2.;
      for(int partition=0; partition<2; ++partition) {
	TProfile2D *p2d = (TProfile2D*)tfile->Get(Form("%s_%s_run_%d",vars[var].c_str(),partitions[partition].c_str(),iovs[iov]));
	p2d->GetZaxis()->SetRangeUser(min,max);
	p2d->SetTitle(titles[var].c_str());
	if(partition==0) {
	  ceb->cd(); 
	  p2d->GetXaxis()->SetTitle("i#phi");
	  p2d->GetYaxis()->SetTitle("i#eta");
	}
	else {
	  cee->cd();
	  p2d->GetXaxis()->SetTitle("ix");
	  p2d->GetYaxis()->SetTitle("iy");
	}
	p2d->Draw("colz");
	gPad->SaveAs(Form("map_%s.pdf",p2d->GetName()));
	gPad->SaveAs(Form("map_%s.png",p2d->GetName()));


	// plot 1D
	int nbins1D = (partition==0 ? 400 : 150);
	TH1D *plot1D = new TH1D(Form("%s_1D",p2d->GetName()),"",nbins1D,min,max);

	for(int bin=0; bin<p2d->GetNbinsX() * p2d->GetNbinsY(); ++bin) {
	  if(p2d->GetBinEntries(bin+1)>0) plot1D->Fill(p2d->GetBinContent(bin+1));
	}
	if(var==0) {
	  if(partition==0) plot1D->Fit("gaus","","",-0.9,0.15);
	  else plot1D->Fit("gaus","","",-1.5,0.5);
	}
	cee->cd();
	plot1D->GetXaxis()->SetTitle(titles[var].c_str());
	plot1D->Draw();

	// fill averages
	graphs[var]->SetPoint(iov,iov,plot1D->GetMean());
	graphs[var]->SetPointError(iov,iov,plot1D->GetRMS());
	
	graphs[var]->GetXaxis()->SetBinLabel(iov+1,Form("%d",iovs[iov]));

	cee->SaveAs(Form("%s.pdf",plot1D->GetName()));
	cee->SaveAs(Form("%s.png",plot1D->GetName()));
      }
    }
  }

  TCanvas *c1 = new TCanvas("c1","",plotscale,plotscale);
  for(int var=0; var<nvars; ++var) {
    graphs[var]->Draw("AP");
    c1->SaveAs(Form("historyplot_%s.pdf",graphs[var]->GetName()));
    c1->SaveAs(Form("historyplot_%s.png",graphs[var]->GetName()));
  }
  
}
