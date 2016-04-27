

//======================================================== //
// For ECAL with CMS Detector at LHC                       //
// Roofit Macro for Unbinned fit to Z peak                 //
//======================================================== //

// run:
// gInterpreter->AddIncludePath("../../");
// .L FitZMassScaleAndResolution.C+

#ifndef __CINT__
#include<stdio.h>
#include<string>
#include<sstream> 
#include<iostream>
#include<fstream>
#endif

#include "RooGlobalFunc.h"
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooArgList.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooExponential.h"
#include "RooFFTConvPdf.h"
#include "RooGaussian.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"

#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TInterpreter.h"
#include "TStopwatch.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TGraphAsymmErrors.h"
#include "TLine.h"
#include "TAxis.h"
#include "../../../Scripts/RooHZZStyle.C"

using namespace RooFit;

void makefit(int energytype, string inputFilename, string outFilename,  
	     Int_t ptBin, Int_t etaBin, Int_t vtxBin,
	     double minMass, double maxMass, 
	     double mean_bw, double gamma_bw, double cutoff_cb, double power_cb,
	     const char *plotOpt, const int nbins);

void FitZMassScaleAndResolution(int energytype, string inputFilename, string outFilename, Int_t ptBin, Int_t etaBin, Int_t vtxBin) {

  // Define Fit Inputs and Call Fit
  double minMass = 75;
  double maxMass = 105;
  double mean_bw = 91.1876;
  double gamma_bw = 2.4952;
  double cutoff_cb = 1.0;
//double power_cb = 1.40;		// Use to fix some fits
  double power_cb = 2.45;
  const char *plotOpt = "NEU";
  // int nbins = 40;
  int nbins = 20;

  // Call the fitting program and output a workspace with a root file
  // of the model and data as well as a pdf of the fit
  makefit(energytype, inputFilename, outFilename, ptBin, etaBin, vtxBin, minMass,  maxMass,  mean_bw,  gamma_bw,  cutoff_cb, power_cb, plotOpt, nbins);

}
//______________________________________________________________

enum p4Kind {kGlobal=0, kMf25, kMf0, kMf50};

enum Flags { 
  kGood=0,                   // channel ok, the energy and time measurement are reliable
  kPoorReco,                 // the energy is available from the UncalibRecHit, but approximate (bad shape, large chi2)
  kOutOfTime,                // the energy is available from the UncalibRecHit (sync reco), but the event is out of time
  kFaultyHardware,           // The energy is available from the UncalibRecHit, channel is faulty at some hardware level (e.g. noisy)
  kNoisy,                    // the channel is very noisy
  kPoorCalib,                // the energy is available from the UncalibRecHit, but the calibration of the channel is poor
  kSaturated,                // saturated channel (recovery not tried)
  kLeadingEdgeRecovered,     // saturated channel: energy estimated from the leading edge before saturation
  kNeighboursRecovered,      // saturated/isolated dead: energy estimated from neighbours
  kTowerRecovered,           // channel in TT with no data link, info retrieved from Trigger Primitive
  kDead,                     // channel is dead and any recovery fails
  kKilled,                   // MC only flag: the channel is killed in the real detector
  kTPSaturated,              // the channel is in a region with saturated TP
  kL1SpikeFlag,              // the channel is in a region with TP with sFGVB = 0
  kWeird,                    // the signal is believed to originate from an anomalous deposit (spike) 
  kDiWeird,                  // the signal is anomalous, and neighbors another anomalous signal  
  kHasSwitchToGain6,         // at least one data frame is in G6
  kHasSwitchToGain1,         // at least one data frame is in G1
  //
  kUnknown                   // to ease the interface with functions returning flags. 
};

void makefit(int energytype, string inputFilename, string outFilename, 
	     Int_t ptBin, Int_t etaBin,  Int_t vtxBin,
	     double minMass, double maxMass, 
	     double mean_bw, double gamma_bw, double cutoff_cb, double power_cb, 
	     const char* plotOpt, const int nbins) {

//   gROOT->ProcessLine(".L tdrstyle.C");
//   setTDRStyle();
//   gStyle->SetPadRightMargin(0.05);

  //Create Data Set
  RooRealVar mass("zmass","m(e^{+}e^{-})",minMass,maxMass,"GeV");
  RooRealVar puw("puW","pileup weight",0.,2.);
  //  mass.setRange(80,100);

  // Reading everything from root tree instead
  TFile *tfile = TFile::Open(inputFilename.c_str());
  TTree *ttree = (TTree*)tfile->Get("probe_tree");
  float l1pt,l2pt,l1eta,l2eta,mass_weights,mass_mf50,mass_mf25,mass_mf0,nvtx, l1flag,l2flag;
  
  ttree->SetBranchAddress("l1pt",&l1pt);
  ttree->SetBranchAddress("l2pt",&l2pt);
  ttree->SetBranchAddress("l1eta",&l1eta);
  ttree->SetBranchAddress("l2eta",&l2eta);
  ttree->SetBranchAddress("l1flag",&l1flag);
  ttree->SetBranchAddress("l2flag",&l2flag);
  ttree->SetBranchAddress("mass_weights",&mass_weights);
  ttree->SetBranchAddress("mass_mf50",&mass_mf50);
  ttree->SetBranchAddress("mass_mf25",&mass_mf25);
  ttree->SetBranchAddress("mass_mf0",&mass_mf0);
  ttree->SetBranchAddress("nvtx",&nvtx);

  float puW=1.0;
  
  RooArgSet zMassArgSet(mass,puw);
  RooDataSet* data = new RooDataSet("data", "ntuple parameters", zMassArgSet, RooFit::WeightVar("puW"));

  for (int i = 0; i < ttree->GetEntries(); i++) {
    if(i%100000==0) cout << "Processing Event " << i << endl;
    ttree->GetEntry(i);

    //*************************************************************************
    //Electron Selection
    //*************************************************************************
    // already passed for this tree

    //*************************************************************************
    //Compute electron four vector;
    //*************************************************************************
    double ele1pt = l1pt;
    double ele2pt = l2pt;

    //*************************************************************************
    //pt and eta cuts on electron
    //*************************************************************************
    if (! (ele1pt > 7 && ele2pt > 7
           && fabs( l1eta) < 2.5 
           && fabs( l2eta) < 2.5 )) continue;

    //*************************************************************************
    //reconstruction rechit flags of the seed of the cluster
    //*************************************************************************
    // if ( l1flag < kHasSwitchToGain6 && l2flag < kHasSwitchToGain6) continue;

    //*************************************************************************
    //pt bins and eta bins
    //*************************************************************************
    Int_t Ele1PtBin = -1;
    Int_t Ele1EtaBin = -1;
    Int_t Ele2PtBin = -1;
    Int_t Ele2EtaBin = -1;
    Int_t NvtxBin = -1;
    if (ele1pt > 20 && ele1pt < 40) Ele1PtBin = 0;
    else Ele1PtBin = 1;
    if (ele2pt > 20 && ele2pt < 40) Ele2PtBin = 0;
    else Ele2PtBin = 1;
    if (fabs(l1eta) < 1.479) Ele1EtaBin = 0;
    else Ele1EtaBin = 1;
    if (fabs(l2eta) < 1.479) Ele2EtaBin = 0;
    else Ele2EtaBin = 1;
    if(nvtx < 10) NvtxBin = 0;
    else if(nvtx < 15) NvtxBin = 1;
    else if(nvtx < 25) NvtxBin = 2;
    else NvtxBin = 3;

    if (ptBin  > -1 &&  !(Ele1PtBin == ptBin || Ele2PtBin == ptBin)) continue; 
    if (etaBin > -1 && !(Ele1EtaBin == etaBin && Ele2EtaBin == etaBin)) continue; 
    if (vtxBin > -1 && NvtxBin != vtxBin) continue;
    

    //*************************************************************************
    // restrict range of mass
    //*************************************************************************
    double zMass;
    switch (energytype) {
    case kGlobal:
      zMass = mass_weights;
      break;
    case kMf50:
      zMass = mass_mf50;
      break;
    case kMf25:
      zMass = mass_mf25;
      break;
    case kMf0:
      zMass = mass_mf0;
      break;
    default:
      zMass = 0.0;
      break;
    }
    
    if (zMass < minMass || zMass > maxMass) continue;

    //*************************************************************************
    //set mass variable
    //*************************************************************************
    zMassArgSet.setRealValue("zmass", zMass);    
    
    data->add(zMassArgSet,puW);
    }

  cout << "data->isWeighted() = " << data->isWeighted() << endl;

  // do binned fit to gain time...
  mass.setBins(nbins);
  RooDataHist *bdata = new RooDataHist("data_binned","data_binned", zMassArgSet, *data);
  cout << "bdata->isWeighted() = " << bdata->isWeighted() << endl;

  cout << "dataset size: " << data->numEntries() << endl;

//   // Closing file
//   treeFile->Close();
  //====================== Parameters===========================

  //Crystal Ball parameters
  RooRealVar cbBias ("#Deltam_{CB}", "CB Bias", -.01, -10, 10, "GeV");
  RooRealVar cbSigma("#sigma_{CB}", "CB Width", 1.5, 0.8, 5.0, "GeV");
  RooRealVar cbCut  ("a_{CB}","CB Cut", 1.0, 1.0, 3.0);
  RooRealVar cbPower("n_{CB}","CB Order", 2.5, 0.1, 20.0);
  cbCut.setVal(cutoff_cb);
  cbPower.setVal(power_cb);

  // Just checking
  //cbCut.Print();
  //cbPower.Print();

  //Breit_Wigner parameters
  RooRealVar bwMean("m_{Z}","BW Mean", 91.1876, "GeV");
  bwMean.setVal(mean_bw);
  RooRealVar bwWidth("#Gamma_{Z}", "BW Width", 2.4952, "GeV");
  bwWidth.setVal(gamma_bw);

  // Fix the Breit-Wigner parameters to PDG values
  bwMean.setConstant(kTRUE);
  bwWidth.setConstant(kTRUE);

  // Exponential Background parameters
  RooRealVar expRate("#lambda_{exp}", "Exponential Rate", -0.064, -1, 1);
  RooRealVar c0("c_{0}", "c0", 1., 0., 50.);

  //Number of Signal and Background events
  RooRealVar nsig("N_{S}", "# signal events", 524, 0.1, 10000000000.);
  RooRealVar nbkg("N_{B}", "# background events", 43, 1., 10000000.);

  //============================ P.D.F.s=============================

  // Mass signal for two decay electrons p.d.f.
  RooBreitWigner bw("bw", "bw", mass, bwMean, bwWidth);
  RooCBShape  cball("cball", "Crystal Ball", mass, cbBias, cbSigma, cbCut, cbPower);
  RooFFTConvPdf BWxCB("BWxCB", "bw X crystal ball", mass, bw, cball);

  // Mass background p.d.f.
  RooExponential bg("bg", "exp. background", mass, expRate);

  // Mass model for signal electrons p.d.f.
  RooAddPdf model("model", "signal", RooArgList(BWxCB), RooArgList(nsig));

  TStopwatch t ;
  t.Start() ;
  RooFitResult *fitres = model.fitTo(*bdata,Hesse(1),Minos(1),Timer(1),Save(1),Strategy(2));
  fitres->SetName("fitres");
  t.Print() ;

  TCanvas* c = new TCanvas("c","Unbinned Invariant Mass Fit", 0,0,800,600);

  //========================== Plotting  ============================
  //Create a frame
  RooPlot* plot = mass.frame(Range(minMass,maxMass),Bins(nbins));
       // Add data and model to canvas
       int linecolors[4] = {kRed,kBlack,kViolet,kOrange};
  data->plotOn(plot);
  model.plotOn(plot,LineColor(linecolors[energytype]));
  data->plotOn(plot);
       //  model.paramOn(plot, Format(plotOpt, AutoPrecision(1)), Parameters(RooArgSet(cbBias, cbSigma, cbCut, cbPower, bwMean, bwWidth, expRate, nsig, nbkg)), Layout(0.15,0.45,0.80));
       model.paramOn(plot, Format(plotOpt, AutoPrecision(2)), Parameters(RooArgSet(cbBias, cbSigma)), Layout(0.13,0.45,0.85));
  plot->getAttText()->SetTextSize(.03);
  plot->SetTitle("");
  plot->Draw();

  // Print Fit Values
  TLatex *tex = new TLatex();
  tex->SetNDC();
  tex->SetTextSize(.1);
  tex->SetTextFont(132);
  //  tex->Draw();
  tex->SetTextSize(0.057);
  tex->DrawLatex(0.65, 0.75, "Z #rightarrow e^{+}e^{-} data");
  tex->SetTextSize(0.030);
  tex->DrawLatex(0.645, 0.65, Form("BW Mean = %.2f GeV", bwMean.getVal()));
  tex->DrawLatex(0.645, 0.60, Form("BW #sigma = %.2f GeV", bwWidth.getVal()));
  c->Update();
  c->SaveAs((outFilename + ".pdf").c_str());
  c->SaveAs((outFilename + ".png").c_str());

  // tablefile << Form(Outfile + "& $ %f $ & $ %f $ & $ %f $\\ \hline",cbBias.getVal(), cbSigma.getVal(), cbCut.getVal());
  // Output workspace with model and data

  RooWorkspace *w = new RooWorkspace("ZeeMassScaleAndResolutionFit");
  w->import(model);
  w->import(*bdata);
  w->writeToFile((outFilename + ".root").c_str());  

  TFile *tfileo = TFile::Open((outFilename + ".root").c_str(),"update");
  fitres->Write();
  tfileo->Close();

}




void plotResolution(const char* bintype="etapt") {

  if(std::string(bintype).compare("etapt")!=0 && 
     std::string(bintype).compare("etavtx")!=0) {
    std::cout << "Wrong bintype. Allowed ones are: etapt, etavtx" << std::endl;
    return;
  }

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();

  double ptbinedgesZ[3] = {20,40,80};
  double vtxbinedgesZ[5] = {0,10,15,25,50};
  double binedgesZ[5];
  for(int i=0;i<5;++i) binedgesZ[i] = (std::string(bintype).compare("etapt")==0) ? ptbinedgesZ[i] : vtxbinedgesZ[i];
  TGraphAsymmErrors gScaleZ[2][3];
  TGraphAsymmErrors gResoZ[2][3];

  double MZ0 = 91.1876;

  // Z->ee
  for(int ipt=0; ipt<2; ++ipt) {
    for(int ieta=0; ieta<2; ++ieta) {
      for(int iene=0; iene<2; ++iene) {
	
	stringstream datafile;
	if(std::string(bintype).compare("etapt")==0) {
	  cout << "Analyzing pt bin = " << ipt << ", eta bin: " << ieta << "  and local reco: " << iene << endl;
	  datafile << "dataZ2015_MCShapesPtBin" << ipt << "_EtaBin" << ieta << "_Reco" << iene << ".root";
	} else if(std::string(bintype).compare("etavtx")==0) {
	  cout << "Analyzing vtxt bin = " << ipt << ", eta bin: " << ieta << "  and local reco: " << iene << endl;
	  datafile << "dataZ2015_MCShapesVtxBin" << ipt << "_EtaBin" << ieta << "_Reco" << iene << ".root";
	} else continue;
    
	TFile *tdatafile = TFile::Open(datafile.str().c_str());
	RooFitResult *datafr = (RooFitResult*)tdatafile->Get("fitres");
	float dataDM = ((RooRealVar*)(datafr->floatParsFinal().find("#Deltam_{CB}")))->getVal();
	float dataDM_err = ((RooRealVar*)(datafr->floatParsFinal().find("#Deltam_{CB}")))->getError();
	float dataS = ((RooRealVar*)(datafr->floatParsFinal().find("#sigma_{CB}")))->getVal();
	float dataS_err = ((RooRealVar*)(datafr->floatParsFinal().find("#sigma_{CB}")))->getError();

	float bincenter=(binedgesZ[ipt+1]+binedgesZ[ipt])/2.;
	// add some offset not to overlap points
	bincenter += (ieta==0) ? 2. : -2.;
	bincenter += (iene==0) ? 1. : -1.;
	
	float binerrup=binedgesZ[ipt+1]-bincenter;
	float binerrdn=bincenter-binedgesZ[ipt];

	gScaleZ[ieta][iene].SetPoint(ipt,bincenter,dataDM/MZ0 * 100);
	gScaleZ[ieta][iene].SetPointError(ipt,binerrdn,binerrup,dataDM_err/MZ0*100,dataDM_err/MZ0*100);
	gResoZ[ieta][iene].SetPoint(ipt,bincenter,dataS);
	gResoZ[ieta][iene].SetPointError(ipt,binerrdn,binerrup,dataS_err,dataS_err);
      }
    }
  }

  for(int ipt=0; ipt<2; ++ipt) {
    for(int ieta=0; ieta<2; ++ieta) {
      for(int iene=0; iene<2; ++iene) {

	// rescale for the change in the mass scale
	double bincenter, y, y0, yerr, yerr0;
	int p = gScaleZ[ieta][iene].GetPoint(ipt,bincenter,y);
	yerr =  gScaleZ[ieta][iene].GetErrorYhigh(ipt);
	p = gScaleZ[ieta][0].GetPoint(ipt,bincenter,y0);
	yerr0 =  gScaleZ[ieta][0].GetErrorYhigh(ipt);

	double reso;
	p =  gResoZ[ieta][iene].GetPoint(ipt,bincenter,reso);

	//	double massscalef = (MZ0 + y) / (MZ0 + y0);
	double massscalef = MZ0 + y;
	std::cout << "ipt = " << ipt << "\tieta = " << ieta << "\tiene = " << iene << "\tf = " << massscalef << std::endl;
	double massreso_scaled = reso / massscalef;
	double massresoerr_scaled = gResoZ[ieta][iene].GetErrorYhigh(ipt) / massscalef;

	float binerrup=binedgesZ[ipt+1]-bincenter;
	float binerrdn=bincenter-binedgesZ[ipt];

	gResoZ[ieta][iene].SetPoint(ipt,bincenter,massreso_scaled);
	gResoZ[ieta][iene].SetPointError(ipt,binerrdn,binerrup,massresoerr_scaled,massresoerr_scaled);
      }}}  
  
  int markerstyles[3] = {kFullCircle,kOpenTriangleUp,kOpenSquare};
  int markercolors[3] = {kRed,kBlack,kViolet};
  std::string etalabels[2] = {"|#eta|<1.5","|#eta|>1.5"};
  std::string enelabels[2] = {"weights","10-pulse fit"};

  TPaveText *pt2 = new TPaveText(0.20,0.94,0.93,0.98,"brNDC");
  pt2->SetBorderSize(0);
  pt2->SetFillStyle(0);
  pt2->SetTextAlign(12);
  pt2->SetTextFont(42);
  pt2->SetTextSize(0.04);
  pt2->AddText("CMS Preliminary, #sqrt{s} = 13 TeV,     L = xxx pb  ^{-1}");
  
  TCanvas *c1 = new TCanvas("c1","",600,600);
  c1->Range(-22.61122,-0.006062015,75.00967,0.004744186);
  c1->SetLeftMargin(0.2316227);
  c1->SetRightMargin(0.05131761);
  c1->SetTopMargin(0.06886657);
  c1->SetBottomMargin(0.1908178);
  
  for(int ieta=0; ieta<2; ++ieta) {
    TLegend* leg2 = new TLegend(0.3,0.80,0.90,0.90);
    leg2->SetFillStyle(0); leg2->SetBorderSize(0); leg2->SetTextSize(0.03);
    leg2->SetTextFont(42);
    leg2->SetFillColor(0);
    for(int iene=0; iene<2; ++iene) {

      if(std::string(bintype).compare("etapt")==0) {
	gScaleZ[ieta][iene].GetXaxis()->SetLimits(0,80);
	gScaleZ[ieta][iene].GetXaxis()->SetTitle("electron p_{T} (GeV)");
      } else if(std::string(bintype).compare("etavtx")==0) {
	gScaleZ[ieta][iene].GetXaxis()->SetLimits(-5,60);
	gScaleZ[ieta][iene].GetXaxis()->SetTitle("vertices");
      }
      gScaleZ[ieta][iene].GetYaxis()->SetTitle("#Delta M/M_{ Z^{0}} [%]");
      gScaleZ[ieta][iene].GetYaxis()->SetTitleOffset(1.8);
      gScaleZ[ieta][iene].GetXaxis()->SetTitleOffset(1.5);

      gScaleZ[ieta][iene].SetMarkerSize(1.);
      if(ieta==0) gScaleZ[ieta][iene].GetYaxis()->SetRangeUser(-3.0,3.0);
      else gScaleZ[ieta][iene].GetYaxis()->SetRangeUser(-15.0,5.0);
      
      gScaleZ[ieta][iene].SetMarkerColor(markercolors[iene]);
      gScaleZ[ieta][iene].SetLineColor(markercolors[iene]);
      gScaleZ[ieta][iene].SetMarkerStyle(markerstyles[iene]);

      if(iene==0) gScaleZ[ieta][0].Draw("AP");
      else gScaleZ[ieta][iene].Draw("P");
      leg2->AddEntry(&gScaleZ[ieta][iene],Form("Z,    %s, %s",etalabels[ieta].c_str(),enelabels[iene].c_str()),"pl");
    }
    
    leg2->Draw();
    pt2->Draw();

    TLine *zero2 = new TLine(0,0,80,0);
    zero2->SetLineColor(kGray+2);
    zero2->SetLineWidth(1);
    zero2->Draw();

    c1->SaveAs(Form("scale-%s-EtaBin%d_mcPS.pdf",bintype,ieta));
    c1->SaveAs(Form("scale-%s-EtaBin%d_mcPS.png",bintype,ieta));
    delete leg2;
    delete zero2;
  }


  TCanvas *c2 = new TCanvas("c2","",600,600);
  c2->Range(-22.61122,-0.006062015,75.00967,0.004744186);
  c2->SetLeftMargin(0.2316227);
  c2->SetRightMargin(0.05131761);
  c2->SetTopMargin(0.06886657);
  c2->SetBottomMargin(0.1908178);

  for(int ieta=0; ieta<2; ++ieta) {
    TLegend* leg2 = new TLegend(0.50,0.20,0.90,0.40);
    leg2->SetFillStyle(0); leg2->SetBorderSize(0); leg2->SetTextSize(0.03);
    leg2->SetTextFont(42);
    leg2->SetFillColor(0);
    for(int iene=0; iene<2; ++iene) {

      if(std::string(bintype).compare("etapt")==0) {
	gResoZ[ieta][iene].GetXaxis()->SetLimits(0,80);
	gResoZ[ieta][iene].GetXaxis()->SetTitle("electron p_{T} (GeV)");
      } else if(std::string(bintype).compare("etavtx")==0) {
	gResoZ[ieta][iene].GetXaxis()->SetLimits(-5,60);
	gResoZ[ieta][iene].GetXaxis()->SetTitle("vertices");
      }

      gResoZ[ieta][iene].GetYaxis()->SetTitle("#sigma_{CB} (Z ^{0}-width subtracted) / m_{CB}");
      gResoZ[ieta][iene].GetYaxis()->SetTitleOffset(1.8);
      gResoZ[ieta][iene].GetXaxis()->SetTitleOffset(1.5);

      gResoZ[ieta][iene].SetMarkerSize(1.);
      if(ieta==0) gResoZ[ieta][iene].GetYaxis()->SetRangeUser(0.005,0.03);
      else gResoZ[ieta][iene].GetYaxis()->SetRangeUser(0.02,0.06);
      // if(ieta==0) gResoZ[ieta][iene].GetYaxis()->SetRangeUser(0.1/MZ0,3.5/MZ0);
      // else gResoZ[ieta][iene].GetYaxis()->SetRangeUser(1.0/MZ0,10./MZ0);

      
      gResoZ[ieta][iene].SetMarkerColor(markercolors[iene]);
      gResoZ[ieta][iene].SetLineColor(markercolors[iene]);
      gResoZ[ieta][iene].SetMarkerStyle(markerstyles[iene]);

      if(iene==0) gResoZ[ieta][0].Draw("AP");
      else gResoZ[ieta][iene].Draw("P");
      leg2->AddEntry(&gResoZ[ieta][iene],Form("Z,    %s, %s",etalabels[ieta].c_str(),enelabels[iene].c_str()),"pl");
    }
    
    leg2->Draw();
    pt2->Draw();

    c2->SaveAs(Form("resolution-rescaled-%s-EtaBin%d_mcPS.pdf",bintype,ieta));
    c2->SaveAs(Form("resolution-rescaled-%s-EtaBin%d_mcPS.png",bintype,ieta));
    delete leg2;
  }



}

