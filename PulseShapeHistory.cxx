#include <algorithm>
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

struct pulseshape {
  double time;
  double alpha;
  double beta;
  double chi2;
  int iecal;
  int ietaix;
  int iphiiy;
  int iz;
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
  fitF->FixParameter(1,alpha);
  fitF->FixParameter(2,beta);
  fitF->SetParameter(3,5.5);
  fitF->SetParLimits(3,4,7); // tmax
  fitF->FixParameter(5,pedestal);

  pulse->Fit("alphabeta","Q WW M","",0,10);

  ps.alpha = fitF->GetParameter(1);
  ps.beta = fitF->GetParameter(2);
  ps.time = fitF->GetParameter(3);
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
  tags[254987] = "template_histograms_EB_runs_1_999999.txt";
  tags[257400] = "template_histograms_EB_runs_257400_257400.txt";

  typedef std::map<int, pulseshape> shapemap;
  std::map<int, shapemap> shapetags;

  TFile *fileOut = TFile::Open("pulseshape_history.root", "recreate");

  TH1D *pulseshapeH = new TH1D("pulseshape","",15,0,15);

  // 2D example maps
  TH2D* ebmap = new TH2D("ebmap","",360,1,360,170,-85,85);
  TH2D* eemap = new TH2D("eemap","",100,1,100,100,1,100);

  int iov = 1;
  for(std::map<int, std::string>::iterator it = tags.begin(); it!=tags.end(); ++it) {

    shapemap reference_psmap = shapetags[tags.begin()->first];

    shapemap psmap;

    ifstream tempdump;
    tempdump.open(it->second,  std::ifstream::in);

    unsigned int rawId;
    int iecal, ietaix, iphiiy, iz;
    double normpulse[15];

    TH2D* time_eb, *alpha_eb, *beta_eb;
    TH2D* time_ee, *alpha_ee, *beta_ee;

    if(iov>1) {
      time_eb  = (TH2D*)ebmap->Clone(Form("time_eb_run_%d",it->first));
      alpha_eb = (TH2D*)ebmap->Clone(Form("alpha_eb_run_%d",it->first));
      beta_eb  = (TH2D*)ebmap->Clone(Form("beta_eb_run_%d",it->first));
      
      time_ee  = (TH2D*)eemap->Clone(Form("time_ee_run_%d",it->first));
      alpha_ee = (TH2D*)eemap->Clone(Form("alpha_ee_run_%d",it->first));
      beta_ee  = (TH2D*)eemap->Clone(Form("beta_ee_run_%d",it->first));
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
      ps.iecal = iecal;
      ps.ietaix = ietaix;
      ps.iphiiy = iphiiy;
      ps.iz = iz;
      
      psmap[rawId] = ps;

      if(iov > 1) {
	if(iecal==1) {
	  time_eb ->SetBinContent(iphiiy, ietaix, ps.time  / reference_psmap[rawId].time);
	  alpha_eb->SetBinContent(iphiiy, ietaix, ps.alpha / reference_psmap[rawId].alpha);
	  beta_eb ->SetBinContent(iphiiy, ietaix, ps.beta  / reference_psmap[rawId].beta);
	} else {
	  time_ee ->SetBinContent(iphiiy, ietaix, ps.time  / reference_psmap[rawId].time);
	  alpha_ee->SetBinContent(iphiiy, ietaix, ps.alpha / reference_psmap[rawId].alpha);
	  beta_ee ->SetBinContent(iphiiy, ietaix, ps.beta  / reference_psmap[rawId].beta);
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
