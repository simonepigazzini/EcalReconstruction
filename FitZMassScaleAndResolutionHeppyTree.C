

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
#include "Math/GenVector/LorentzVector.h"
#include "Math/GenVector/PtEtaPhiM4D.h"
#include "../../../Scripts/RooHZZStyle.C"

using namespace RooFit;

float mass_2_ene(float ene1, float eta1, float phi1, float m1, float ene2, float eta2, float phi2, float m2) {
    typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > PtEtaPhiMVector;
    PtEtaPhiMVector unitp41(1.0,eta1,phi1,m1);
    PtEtaPhiMVector unitp42(1.0,eta2,phi2,m2);
    double theta1 = unitp41.Theta();
    double theta2 = unitp42.Theta();
    double pt1 = ene1*fabs(sin(theta1));
    double pt2 = ene2*fabs(sin(theta2));
    PtEtaPhiMVector p41(pt1,eta1,phi1,m1);
    PtEtaPhiMVector p42(pt2,eta2,phi2,m2);
    return (p41+p42).M();
}

void makefit(int energytype, string inputFilename, string outFilename,  
	     Int_t r9Bin, Int_t etaBin, Int_t vtxBin,
	     double minMass, double maxMass, 
	     double mean_bw, double gamma_bw, double cutoff_cb, double power_cb,
	     const char *plotOpt, const int nbins);

void FitZMassScaleAndResolution(int energytype, string inputFilename, string outFilename, Int_t r9Bin, Int_t etaBin, Int_t vtxBin) {

  // Define Fit Inputs and Call Fit
  double minMass = 65;
  double maxMass = 105;
  double mean_bw = 91.1876;
  double gamma_bw = 2.4952;
  double cutoff_cb = 1.0;
//double power_cb = 1.40;		// Use to fix some fits
  double power_cb = 2.45;
  const char *plotOpt = "NEU";
  int nbins = 40;
  //int nbins = 20;

  // Call the fitting program and output a workspace with a root file
  // of the model and data as well as a pdf of the fit
  makefit(energytype, inputFilename, outFilename, r9Bin, etaBin, vtxBin, minMass,  maxMass,  mean_bw,  gamma_bw,  cutoff_cb, power_cb, plotOpt, nbins);

}
//______________________________________________________________

enum p4Kind {kFullCorr=0,kRaw};

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
	     Int_t r9Bin, Int_t etaBin,  Int_t vtxBin,
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
  TTree *ttree = (TTree*)tfile->Get("tree");
  float lpt[10],leta[10],lphi[10],lrawenergy[10],mass_Z1,lr9[10];
  Int_t nvtx;
  UInt_t run;

  ttree->SetBranchAddress("LepGood_pt",lpt);
  ttree->SetBranchAddress("LepGood_eta",leta);
  ttree->SetBranchAddress("LepGood_phi",lphi);
  ttree->SetBranchAddress("LepGood_r9",lr9);
  ttree->SetBranchAddress("mZ1",&mass_Z1);
  ttree->SetBranchAddress("LepGood_superCluster_rawEnergy",lrawenergy);
  ttree->SetBranchAddress("nVert",&nvtx);
  ttree->SetBranchAddress("run",&run);

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
    double ele1pt = lpt[0];
    double ele2pt = lpt[1];
    double l1eta = leta[0];
    double l2eta = leta[1];
    double l1phi = lphi[0];
    double l2phi = lphi[1];

    // if (run > 256700) continue; // Fill 4381 (76.5/pb)
    // if (run < 256728 || run > 256734) continue; // Fill 4384 (66.8/pb)
    // if (run != 256801) continue; // Fill 4386 (8.8/pb)

    // if(nvtx>7) continue;
    // if(nvtx<8 || nvtx>12) continue;
    // if(nvtx<13) continue;

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
    Int_t Ele1r9Bin = 0;
    Int_t Ele1EtaBin = 0;
    Int_t Ele2r9Bin = -1;
    Int_t Ele2EtaBin = -1;
    Int_t NvtxBin = -1;
    if (lr9[0]>=0.94) Ele1r9Bin = 0;
    else Ele1r9Bin = 1;
    if (lr9[1]>=0.94) Ele2r9Bin = 0;
    else Ele2r9Bin = 1;
    if (fabs(l1eta) < 0.5) Ele1EtaBin = 0;
    else if (fabs(l1eta) < 0.8) Ele1EtaBin = 1;
    else if (fabs(l1eta) < 1.1) Ele1EtaBin = 2;
    else if (fabs(l1eta) < 1.479) Ele1EtaBin = 3;
    else Ele1EtaBin = 4;
    if (fabs(l2eta) < 0.5) Ele2EtaBin = 0;
    else if (fabs(l2eta) < 0.8) Ele2EtaBin = 1;
    else if (fabs(l2eta) < 1.1) Ele2EtaBin = 2;
    else if (fabs(l2eta) < 1.479) Ele2EtaBin = 3;
    else Ele2EtaBin = 4;
    if(nvtx < 10) NvtxBin = 0;
    else if(nvtx < 15) NvtxBin = 1;
    else if(nvtx < 25) NvtxBin = 2;
    else NvtxBin = 3;

    if (r9Bin  > -1 &&  !(Ele1r9Bin == r9Bin || Ele2r9Bin == r9Bin)) continue; 
    if (etaBin > -1 && !(Ele1EtaBin == etaBin && Ele2EtaBin == etaBin)) continue; 
    if (vtxBin > -1 && NvtxBin != vtxBin) continue;
    

    //*************************************************************************
    // restrict range of mass
    //*************************************************************************
    double zMass;
    switch (energytype) {
    case kFullCorr:
      zMass = mass_Z1;
      break;
    case kRaw:
      zMass = mass_2_ene(lrawenergy[0],l1eta,l1phi,0., lrawenergy[1],l2eta,l2phi,0.);
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
  RooRealVar cbBias ("#Deltam_{CB}", "CB Bias", -.01, -20, 20, "GeV");
  RooRealVar cbSigma("#sigma_{CB}", "CB Width", 1.5, 0.5, 10.0, "GeV");
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




void plotResolution() {

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();

  double etabinedgesZ[6] = {0,0.5,0.8,1.1,1.479,2.5};
  double binedgesZ[6];
  for(int i=0;i<6;++i) binedgesZ[i] = etabinedgesZ[i];
  TGraphAsymmErrors gScaleZ[2][3];
  TGraphAsymmErrors gResoZ[2][3];

  double MZ0 = 91.1876;

  int fills[3] = {4381,4384,4386};
  string nvert[3] = {"Lt8","8To12","Gt12"};

  // Z->ee
  for(int idata=0; idata<3; ++idata) {
    for(int ir9=0; ir9<2; ++ir9) {
      for(int ieta=0; ieta<5; ++ieta) {
	
	stringstream datafile, mcfile;
	cout << "Analyzing isdata = " << idata << " R9 bin = " << ir9 << " eta bin = " << ieta << endl;
	// datafile << (idata==1 ? "resRun2015D_PromptReco" : "resSpring15MC") << "R9Bin" << ir9 << "_EtaBin" << ieta << "_Reco1.root";
	// datafile << "resFill" << fills[idata] << "Run2015D_PromptRecoR9Bin" << ir9 << "_EtaBin" << ieta << "_Reco1.root";
	datafile << "resNVert" << nvert[idata] << "Run2015D_PromptRecoR9Bin" << ir9 << "_EtaBin" << ieta << "_Reco1.root";
      
	TFile *tdatafile = TFile::Open(datafile.str().c_str());
	RooFitResult *datafr = (RooFitResult*)tdatafile->Get("fitres");
	float dataDM = ((RooRealVar*)(datafr->floatParsFinal().find("#Deltam_{CB}")))->getVal();
	float dataDM_err = ((RooRealVar*)(datafr->floatParsFinal().find("#Deltam_{CB}")))->getError();
	float dataS = ((RooRealVar*)(datafr->floatParsFinal().find("#sigma_{CB}")))->getVal();
	float dataS_err = ((RooRealVar*)(datafr->floatParsFinal().find("#sigma_{CB}")))->getError();
	tdatafile->Close();

	float bincenter=(binedgesZ[ieta+1]+binedgesZ[ieta])/2.;	
	float binerrup=binedgesZ[ieta+1]-bincenter;
	float binerrdn=bincenter-binedgesZ[ieta];

	gScaleZ[ir9][idata].SetPoint(ieta,bincenter,dataDM/MZ0 * 100);
	gScaleZ[ir9][idata].SetPointError(ieta,binerrdn,binerrup,dataDM_err/MZ0*100,dataDM_err/MZ0*100);
	gResoZ[ir9][idata].SetPoint(ieta,bincenter,dataS);
	gResoZ[ir9][idata].SetPointError(ieta,binerrdn,binerrup,dataS_err,dataS_err);
      }
    }
  }

  for(int ir9=0; ir9<2; ++ir9) {
    for(int idata=0; idata<3; ++idata) {
      for(int ieta=0; ieta<5; ++ieta) {

	// rescale for the change in the mass scale
	double bincenter, y, y0, yerr, yerr0;
	int p = gScaleZ[ir9][idata].GetPoint(ieta,bincenter,y);
	yerr =  gScaleZ[ir9][idata].GetErrorYhigh(ieta);
	p = gScaleZ[ir9][0].GetPoint(ieta,bincenter,y0);
	yerr0 =  gScaleZ[ir9][0].GetErrorYhigh(ieta);

	double reso;
	p =  gResoZ[ir9][idata].GetPoint(ieta,bincenter,reso);

	//	double massscalef = (MZ0 + y) / (MZ0 + y0);
	double massscalef = MZ0 + y;
	double massreso_scaled = reso / massscalef / sqrt(2.0);
	double massresoerr_scaled = gResoZ[ir9][idata].GetErrorYhigh(ieta) / massscalef / sqrt(2.0);

	float binerrup=binedgesZ[ieta+1]-bincenter;
	float binerrdn=bincenter-binedgesZ[ieta];

	gResoZ[ir9][idata].SetPoint(ieta,bincenter,massreso_scaled);
	gResoZ[ir9][idata].SetPointError(ieta,binerrdn,binerrup,massresoerr_scaled,massresoerr_scaled);
      }}}  

  int markerstyles[3] = {kOpenSquare,kFullCircle,kFullTriangleUp};
  int markercolors[3] = {kRed,kBlack,kBlue+2};
  std::string r9labels[2] = {"R_{9}>0.94","R_{9}<0.94"};
  // std::string datalabels[3] = {"MC", "Run2015D prompt reco"};
  // std::string datalabels[3] = {"Fill 4381","Fill 4384","Fill 4386"};
  std::string datalabels[3] = {"nVert#leq7","8#leq nVert #leq12","nVert#geq13"};

  TPaveText *pt2 = new TPaveText(0.20,0.94,0.93,0.98,"brNDC");
  pt2->SetBorderSize(0);
  pt2->SetFillStyle(0);
  pt2->SetTextAlign(12);
  pt2->SetTextFont(42);
  pt2->SetTextSize(0.04);
  pt2->AddText("CMS Preliminary, #sqrt{s} = 13 TeV,     L = 152 pb^{-1}");
  
  TCanvas *c1 = new TCanvas("c1","",600,600);
  c1->Range(-22.61122,-0.006062015,75.00967,0.004744186);
  c1->SetLeftMargin(0.2316227);
  c1->SetRightMargin(0.05131761);
  c1->SetTopMargin(0.06886657);
  c1->SetBottomMargin(0.1908178);
  
  for(int ir9=0; ir9<2; ++ir9) {
    TLegend* leg2 = new TLegend(0.3,0.80,0.90,0.90);
    leg2->SetFillStyle(0); leg2->SetBorderSize(0); leg2->SetTextSize(0.03);
    leg2->SetTextFont(42);
    leg2->SetFillColor(0);
    for(int idata=0; idata<3; ++idata) {

      gScaleZ[ir9][idata].GetXaxis()->SetLimits(0,2.5);
      gScaleZ[ir9][idata].GetXaxis()->SetTitle("electron #eta");

      gScaleZ[ir9][idata].GetYaxis()->SetTitle("#Delta M/M_{ Z^{0}} [%]");
      gScaleZ[ir9][idata].GetYaxis()->SetTitleOffset(1.8);
      gScaleZ[ir9][idata].GetXaxis()->SetTitleOffset(1.5);

      gScaleZ[ir9][idata].SetMarkerSize(1.);
      if(ir9==0) gScaleZ[ir9][idata].GetYaxis()->SetRangeUser(-15.0,5.0);
      else gScaleZ[ir9][idata].GetYaxis()->SetRangeUser(-15.0,5.0);
      
      gScaleZ[ir9][idata].SetMarkerColor(markercolors[idata]);
      gScaleZ[ir9][idata].SetLineColor(markercolors[idata]);
      gScaleZ[ir9][idata].SetMarkerStyle(markerstyles[idata]);

      if(idata==0) gScaleZ[ir9][0].Draw("AP");
      else gScaleZ[ir9][idata].Draw("P");
      leg2->AddEntry(&gScaleZ[ir9][idata],Form("Z,    %s, %s",r9labels[ir9].c_str(),datalabels[idata].c_str()),"pl");
    }
    
    leg2->Draw();
    pt2->Draw();

    TLine *zero2 = new TLine(0,0,2.5,0);
    zero2->SetLineColor(kGray+2);
    zero2->SetLineWidth(1);
    zero2->Draw();

    c1->SaveAs(Form("scale-R9Bin%d.pdf",ir9));
    c1->SaveAs(Form("scale-R9Bin%d.png",ir9));
    delete leg2;
    delete zero2;
  }


  TCanvas *c2 = new TCanvas("c2","",600,600);
  c2->Range(-22.61122,-0.006062015,75.00967,0.004744186);
  c2->SetLeftMargin(0.2316227);
  c2->SetRightMargin(0.05131761);
  c2->SetTopMargin(0.06886657);
  c2->SetBottomMargin(0.1908178);

  for(int ir9=0; ir9<2; ++ir9) {
    TLegend* leg2 = new TLegend(0.3,0.80,0.90,0.90);
    leg2->SetFillStyle(0); leg2->SetBorderSize(0); leg2->SetTextSize(0.03);
    leg2->SetTextFont(42);
    leg2->SetFillColor(0);
    for(int idata=0; idata<3; ++idata) {

      gResoZ[ir9][idata].GetXaxis()->SetLimits(0,2.5);
      gResoZ[ir9][idata].GetXaxis()->SetTitle("electron #eta");
      gResoZ[ir9][idata].GetYaxis()->SetTitle("#sigma_{CB} (Z ^{0}-width subtracted) / #sqrt{2} m_{CB}");
      gResoZ[ir9][idata].GetYaxis()->SetTitleOffset(1.8);
      gResoZ[ir9][idata].GetXaxis()->SetTitleOffset(1.5);

      gResoZ[ir9][idata].SetMarkerSize(1.);
      if(ir9==0) gResoZ[ir9][idata].GetYaxis()->SetRangeUser(0.00,0.06);
      else gResoZ[ir9][idata].GetYaxis()->SetRangeUser(0.00,0.06);
      // if(ieta==0) gResoZ[ir9][idata].GetYaxis()->SetRangeUser(0.1/MZ0,3.5/MZ0);
      // else gResoZ[ir9][idata].GetYaxis()->SetRangeUser(1.0/MZ0,10./MZ0);

      
      gResoZ[ir9][idata].SetMarkerColor(markercolors[idata]);
      gResoZ[ir9][idata].SetLineColor(markercolors[idata]);
      gResoZ[ir9][idata].SetMarkerStyle(markerstyles[idata]);

      if(idata==0) gResoZ[ir9][0].Draw("AP");
      else gResoZ[ir9][idata].Draw("P");
      leg2->AddEntry(&gResoZ[ir9][idata],Form("Z,    %s, %s",r9labels[ir9].c_str(),datalabels[idata].c_str()),"pl");
    }
    
    leg2->Draw();
    pt2->Draw();

    c2->SaveAs(Form("resolution-R9Bin%d.pdf",ir9));
    c2->SaveAs(Form("resolution-R9Bin%d.png",ir9));
    delete leg2;
  }



}

