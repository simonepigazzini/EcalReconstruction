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

struct rechit {
  double time;
  double chi2;
  double amplitude;
};

double* noiseCorr(bool barrel, int gain) {
 static double corr_EB_G12[10] = { 1.00000, 0.71073, 0.55721, 0.46089, 0.40449, 0.35931, 0.33924, 0.32439, 0.31581, 0.30481};
 static double corr_EB_G6[10]  = { 1.00000, 0.70946, 0.58021, 0.49846, 0.45006, 0.41366, 0.39699, 0.38478, 0.37847, 0.37055};
 static double corr_EB_G1[10]  = { 1.00000, 0.73354, 0.64442, 0.58851, 0.55425, 0.53082, 0.51916, 0.51097, 0.50732, 0.50409};
 static double corr_EE_G12[10] = { 1.00000, 0.71373, 0.44825, 0.30152, 0.21609, 0.14786, 0.11772, 0.10165, 0.09465, 0.08098};
 static double corr_EE_G6[10]  = { 1.00000, 0.71217, 0.47464, 0.34056, 0.26282, 0.20287, 0.17734, 0.16256, 0.15618, 0.14443};
 static double corr_EE_G1[10]  = { 1.00000, 0.72698, 0.62048, 0.55691, 0.51848, 0.49147, 0.47813, 0.47007, 0.46621, 0.46265};
  
  if(barrel) {
    switch ( gain ) {
    case 12: 
      return corr_EB_G12;
      break;
    case 6: 
      return corr_EB_G6;
      break;
    case 1: 
      return corr_EB_G1;
      break;
    default:
      return corr_EB_G12;
      break;
    }
  } else {
    switch ( gain ) {
    case 12: 
      return corr_EE_G12;
      break;
    case 6: 
      return corr_EE_G6;
      break;
    case 1: 
      return corr_EE_G1;
      break;
    default:
      return corr_EE_G12;
      break;
    }
  }
}

double applyJacobian(double sample, double sample5, int i, int j) {
  if(i!=5) {
    if(i==j) return 1./sample5;
    if(j==5) return -sample/sample5/sample5;
  }
  return 0.;
}

TH2D *simPulseShapeCovariance(bool barrel) {

  float EBPulseShapeCovariance[144] = {
    3.001e-06,  1.233e-05,  0.000e+00, -4.416e-06, -4.571e-06, -3.614e-06, -2.636e-06, -1.286e-06, -8.410e-07, -5.296e-07,  0.000e+00,  0.000e+00, 
    1.233e-05,  6.154e-05,  0.000e+00, -2.200e-05, -2.309e-05, -1.838e-05, -1.373e-05, -7.334e-06, -5.088e-06, -3.745e-06, -2.428e-06,  0.000e+00, 
    0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00, 
    -4.416e-06, -2.200e-05,  0.000e+00,  8.319e-06,  8.545e-06,  6.792e-06,  5.059e-06,  2.678e-06,  1.816e-06,  1.223e-06,  8.245e-07,  5.589e-07, 
    -4.571e-06, -2.309e-05,  0.000e+00,  8.545e-06,  9.182e-06,  7.219e-06,  5.388e-06,  2.853e-06,  1.944e-06,  1.324e-06,  9.083e-07,  6.335e-07, 
    -3.614e-06, -1.838e-05,  0.000e+00,  6.792e-06,  7.219e-06,  6.016e-06,  4.437e-06,  2.385e-06,  1.636e-06,  1.118e-06,  7.754e-07,  5.556e-07, 
    -2.636e-06, -1.373e-05,  0.000e+00,  5.059e-06,  5.388e-06,  4.437e-06,  3.602e-06,  1.917e-06,  1.322e-06,  9.079e-07,  6.529e-07,  4.752e-07, 
    -1.286e-06, -7.334e-06,  0.000e+00,  2.678e-06,  2.853e-06,  2.385e-06,  1.917e-06,  1.375e-06,  9.100e-07,  6.455e-07,  4.693e-07,  3.657e-07, 
    -8.410e-07, -5.088e-06,  0.000e+00,  1.816e-06,  1.944e-06,  1.636e-06,  1.322e-06,  9.100e-07,  9.115e-07,  6.062e-07,  4.436e-07,  3.422e-07, 
    -5.296e-07, -3.745e-06,  0.000e+00,  1.223e-06,  1.324e-06,  1.118e-06,  9.079e-07,  6.455e-07,  6.062e-07,  7.217e-07,  4.862e-07,  3.768e-07, 
    0.000e+00, -2.428e-06,  0.000e+00,  8.245e-07,  9.083e-07,  7.754e-07,  6.529e-07,  4.693e-07,  4.436e-07,  4.862e-07,  6.509e-07,  4.418e-07, 
    0.000e+00,  0.000e+00,  0.000e+00,  5.589e-07,  6.335e-07,  5.556e-07,  4.752e-07,  3.657e-07,  3.422e-07,  3.768e-07,  4.418e-07,  6.142e-07
  };

  float EEPulseShapeCovariance[144] = {
    3.941e-05,  3.333e-05,  0.000e+00, -1.449e-05, -1.661e-05, -1.424e-05, -1.183e-05, -6.842e-06, -4.915e-06, -3.411e-06,  0.000e+00,  0.000e+00, 
    3.333e-05,  2.862e-05,  0.000e+00, -1.244e-05, -1.431e-05, -1.233e-05, -1.032e-05, -5.883e-06, -4.154e-06, -2.902e-06, -2.128e-06,  0.000e+00, 
    0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00, 
    -1.449e-05, -1.244e-05,  0.000e+00,  5.840e-06,  6.649e-06,  5.720e-06,  4.812e-06,  2.708e-06,  1.869e-06,  1.330e-06,  9.186e-07,  6.446e-07, 
    -1.661e-05, -1.431e-05,  0.000e+00,  6.649e-06,  7.966e-06,  6.898e-06,  5.794e-06,  3.157e-06,  2.184e-06,  1.567e-06,  1.084e-06,  7.575e-07, 
    -1.424e-05, -1.233e-05,  0.000e+00,  5.720e-06,  6.898e-06,  6.341e-06,  5.347e-06,  2.859e-06,  1.991e-06,  1.431e-06,  9.839e-07,  6.886e-07, 
    -1.183e-05, -1.032e-05,  0.000e+00,  4.812e-06,  5.794e-06,  5.347e-06,  4.854e-06,  2.628e-06,  1.809e-06,  1.289e-06,  9.020e-07,  6.146e-07, 
    -6.842e-06, -5.883e-06,  0.000e+00,  2.708e-06,  3.157e-06,  2.859e-06,  2.628e-06,  1.863e-06,  1.296e-06,  8.882e-07,  6.108e-07,  4.283e-07, 
    -4.915e-06, -4.154e-06,  0.000e+00,  1.869e-06,  2.184e-06,  1.991e-06,  1.809e-06,  1.296e-06,  1.217e-06,  8.669e-07,  5.751e-07,  3.882e-07, 
    -3.411e-06, -2.902e-06,  0.000e+00,  1.330e-06,  1.567e-06,  1.431e-06,  1.289e-06,  8.882e-07,  8.669e-07,  9.522e-07,  6.717e-07,  4.293e-07, 
    0.000e+00, -2.128e-06,  0.000e+00,  9.186e-07,  1.084e-06,  9.839e-07,  9.020e-07,  6.108e-07,  5.751e-07,  6.717e-07,  7.911e-07,  5.493e-07, 
    0.000e+00,  0.000e+00,  0.000e+00,  6.446e-07,  7.575e-07,  6.886e-07,  6.146e-07,  4.283e-07,  3.882e-07,  4.293e-07,  5.493e-07,  7.027e-07
  };

  int NZSAMPLES=12;
  TH2D *simPulseCov = new TH2D("simPulseCov","",NZSAMPLES,0,NZSAMPLES,NZSAMPLES,0,NZSAMPLES);

  for(int k=0; k<std::pow(NZSAMPLES,2); ++k) { 
    int i = k/NZSAMPLES;
    int j = k%NZSAMPLES;
    simPulseCov->SetBinContent(i+1,j+1,barrel ? EBPulseShapeCovariance[k] : EEPulseShapeCovariance[k]);
  }

  return simPulseCov;
}

TH1D *simPulseShapeTemplate(bool barrel) {

  double EBPulseShapeTemplate[12] = { 1.13979e-02, 7.58151e-01, 1.00000e+00, 8.87744e-01, 6.73548e-01, 4.74332e-01, 3.19561e-01, 2.15144e-01, 1.47464e-01, 1.01087e-01, 6.93181e-02, 4.75044e-02 };
  double EEPulseShapeTemplate[12] = { 1.16442e-01, 7.56246e-01, 1.00000e+00, 8.97182e-01, 6.86831e-01, 4.91506e-01, 3.44111e-01, 2.45731e-01, 1.74115e-01, 1.23361e-01, 8.74288e-02, 6.19570e-02 };
  
  TH1D *simPulseShape = new TH1D("simPulseShape","",15,0,15);

  TH2D *pulseCov = simPulseShapeCovariance(barrel);
  
  int SAMPLES=15;
  int firstSignalSample=3;
  for(int i=0; i<SAMPLES; ++i) {
    if(i<firstSignalSample) {
      simPulseShape->SetBinContent(i+1, 1E-10);
      simPulseShape->SetBinError(i+1, 0.);      
    }
    else {
      simPulseShape->SetBinContent(i+1, (barrel ? EBPulseShapeTemplate[i-firstSignalSample] : EEPulseShapeTemplate[i-firstSignalSample]));
      simPulseShape->SetBinError(i+1, sqrt(pulseCov->GetBinContent(i-firstSignalSample,i-firstSignalSample)));
    }
  }

  delete pulseCov;
  return simPulseShape;

}

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

double ALPHABETAT[3];
double ERR_ALPHABETAT[3];
double val4, s4s5fit;
double valExtrap[5];

rechit makeRecHit(TH1D *pulse, bool doEB, double pedestal) {

  rechit rh;

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

  rh.amplitude = fitF->GetParameter(0);
  rh.time = fitF->GetParameter(3);
  rh.chi2 = fitF->GetChisquare();

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
  return rh;

}


TH1D* fitTemplate(TH1D *templateh, bool doEB, double pedestal=0, TH1D* simTemplate=0) {

  TH1D *shifted_temp = (TH1D*)templateh->Clone(Form("shifted_%s",templateh->GetName()));

  double alpha = doEB ? 1.250 : 1.283;
  double beta = doEB ? 1.600 : 1.674;

  TF1 *fitF = new TF1("alphabeta",alphabeta,0,10,5);
  fitF->SetParNames("norm","#alpha","#beta","tmax","pedestal","raiset");
  fitF->SetParLimits(0,0,10); // normalization
  fitF->SetParameter(1,alpha);
  fitF->SetParameter(2,beta);
  fitF->SetParameter(3,5.5);
  if(doEB) fitF->SetParLimits(1,0.8,2.5); // alpha
  else fitF->SetParLimits(1,0.5,2.5);
  fitF->SetParLimits(2,0.8,2.5); // beta
  fitF->SetParLimits(3,4,7); // tmax
  //  fitF->SetParLimits(5,-0.1,0.1); // pedestal
  fitF->FixParameter(5,pedestal);

  templateh->SetLineColor(kBlue+2);
  templateh->SetLineWidth(2);
  fitF->SetLineColor(kRed+1);

  templateh->GetXaxis()->SetTitle("Sample");
  templateh->GetYaxis()->SetTitle("Ped. Subtracted Amplitude (a.u.)");
  templateh->Draw("hist E2");
  templateh->Fit("alphabeta","Q WW M","same",0,10);
  fitF->Draw("same");

  TPaveText *pt = new TPaveText(0.40, 0.80, 0.50, 0.99);
  pt->SetBorderSize(     0);
  pt->SetFillColor (  4000);
  pt->SetTextAlign (    12);
  pt->SetTextFont  (    42);
  pt->SetTextSize  (0.03);
  pt->AddText(Form("#alpha = %1.2f #pm %1.2f",fitF->GetParameter(1), fitF->GetParError(1)));  
  pt->AddText(Form("#beta = %1.2f #pm %1.2f",fitF->GetParameter(2), fitF->GetParError(2)));  
  pt->AddText(Form("#Delta t = %1.2f #pm %1.2f",fitF->GetParameter(3)-5.5, fitF->GetParError(3)));  
  pt->Draw();

  for(int i=1;i<4;++i) {
    ALPHABETAT[i-1] = fitF->GetParameter(i);
    ERR_ALPHABETAT[i-1] = fitF->GetParError(i);
  }
  val4 = fitF->Eval(fitF->GetParameter(3) - 1.0);
  s4s5fit = fitF->Eval(4.5)/fitF->Eval(5.5);

  //  std::cout << "chi2 = " << fitF->GetChisquare() << std::endl;
  for(int iextra=0;iextra<5; ++iextra) {
    if(fitF->GetChisquare()<1e-1) valExtrap[iextra] = fitF->Eval(10.5+iextra);
    else valExtrap[iextra] = simTemplate ? simTemplate->GetBinContent(11+iextra) : 0.0;
  }

  for(int i=0;i<10;++i) {
    double val = 0.0; 
    if(i>3) { // do not use the fit until the 5th sample (even if for EE is not strictly 0)
      double dt = fitF->GetParameter(3)-5.5;
      val = fitF->Eval(i+0.5+dt)/fitF->Eval(5.5+dt);
    }
    shifted_temp->SetBinContent(i+1, val); 
  }

  delete pt;
  delete fitF;
  return shifted_temp;
}

void saveTemplates(bool dobarrel) {

  TFile *file = TFile::Open("/Users/emanuele/Work/data/ecalreco/multifit/templates_dynped_rawid.root"); // 2013 low PU runs
  // TFile *file = TFile::Open("/Users/emanuele/Work/data/ecalreco/multifit/templates_tree_pi0_2015C_lowPU.root"); // 2015 low PU runs
  TTree *tree = (TTree*)file->Get("pulseDump/pulse_tree");

  Long64_t nentries = tree->GetEntries();
  std::cout << "The tree has " << nentries << " entries" << std::endl;
  
  Bool_t          barrel;
  UInt_t          gain;
  Double_t        pedrms;
  Double_t        pedval;
  Int_t           ietaix;
  Int_t           iphiiy;
  Int_t           iz;
  Double_t        pulse[10];
  UInt_t          rawid;
  
  tree->SetBranchAddress("barrel", &barrel);
  tree->SetBranchAddress("gain", &gain);
  tree->SetBranchAddress("pedrms", &pedrms);
  tree->SetBranchAddress("pedval", &pedval);
  tree->SetBranchAddress("ietaix", &ietaix);
  tree->SetBranchAddress("iphiiy", &iphiiy);
  tree->SetBranchAddress("iz", &iz);
  tree->SetBranchAddress("pulse", pulse);
  tree->SetBranchAddress("rawid", &rawid); 

  std::map<int, std::vector<double> > templates;
  std::map<int, std::vector<double> > templates_weight;
  std::map<int, double> norm_average;
  std::map<int, double> norm_counts;
  std::map<int, unsigned int> rawIds;

  // to reject noise" better in ADC not to make eta-dependent cuts
  // ~10 sigma from the 2012 plots: https://twiki.cern.ch/twiki/bin/view/CMSPublic/EcalDPGResultsCMSDP2013007
  float minAmplitude = dobarrel ? 14 : 26; 
  // to reject spikes
  float maxTimeShift = 8.0/25.; // ns
  float maxChi2 = 25.188; 
  int minNhits = 2;

  // float adcToGeV = dobarrel ? 0.035 : 0.06;
  // float minAmplitude = minEnergy / adcToGeV;

  TH1D *htempl = new TH1D("htempl","",10,0,10);

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = tree->LoadTree(jentry);
    if (ientry < 0) break;
    nb = tree->GetEntry(jentry);   nbytes += nb;

    if(jentry%100000==0) std::cout << "Processing entry " << jentry << std::endl;

    if((dobarrel && (!barrel)) || (!dobarrel && barrel)) continue;

    int offset;
    if(barrel) offset = (ietaix > 0) ? 1000 * ietaix : 1000 * (abs(ietaix)+85);
    else offset = (iz > 0) ? 1000 * ietaix : 1000 * (ietaix+100);
    int ic = offset + iphiiy;
    
    //    std::cout << "ietaix = " << ietaix << "\toffset = " << offset << "\tic = " << ic << std::endl;

    TH1D *digis = (TH1D*)htempl->Clone(Form("rh_%d",ic));
    for(int iSample(0); iSample < 10; iSample++) digis->SetBinContent(iSample+1,pulse[iSample]);
    rechit rh = makeRecHit(digis,barrel,pedval);

    // std::cout << "\t Rechit has A = " << rh.amplitude << "   t = " << rh.time << "   chi2 = " << rh.chi2 
    // 	      << "   A_maxsample = " << pulse[5] << std::endl;

    delete digis;


    // calc the max-sample
    int maxsample=5;
    double maxval=0;
    for(int iSample=2;iSample<10;++iSample) {
      double val = pulse[iSample];
      if(val>maxval) {
	maxval=val;
	maxsample=iSample;
      }
    }

    // double weight = 1.0;
    double weight = pow(rh.amplitude,2);

    if(rh.amplitude<minAmplitude || fabs(rh.time-5.5)>maxTimeShift || rh.chi2>maxChi2) continue;

    if(templates.count(ic)==0) {
      std::vector<double> templ;
      templ.resize(10);
      for(int iSample(0); iSample < 10; iSample++) templ[iSample] = pulse[iSample]/maxval * weight;
      templates[ic] = templ;
      norm_average[ic] = weight;
      norm_counts[ic] = 1;
      // std::cout << "inserting new ped for DetId = " << ic << std::endl;
      rawIds[ic] = rawid;
    } else {
      std::vector<double> &templ = templates[ic];
      for(int iSample(0); iSample < 10; iSample++) templ[iSample] += pulse[iSample]/maxval * weight;
      norm_average[ic] += weight;
      norm_counts[ic] ++;
      //        std::cout << "updating ped for DetId = " << ic << std::endl;
    }
  } // crystals x events
  
  TH1D *simTemplate = simPulseShapeTemplate(dobarrel);

  /// output
  TString nameoutput = dobarrel ? "template_histograms_EB.root" : "template_histograms_EE.root";
  TFile *outfile = TFile::Open(nameoutput.Data(),"recreate");
  TH1D *htemplAverage =  (TH1D*)htempl->Clone("templates_average");
  int nCry(0);

  // text file to fill the DB objects with templates
  ofstream txtdumpfile;  
  TString nametxtoutput = dobarrel ? "template_histograms_EB.txt" : "template_histograms_EE.txt";
  txtdumpfile.open (nametxtoutput.Data(), ios::out | ios::trunc);

  for(std::map<int, std::vector<double> >::iterator it=templates.begin(); it!=templates.end(); ++it) {
    unsigned int rawId = rawIds[it->first];
    txtdumpfile.unsetf ( std::ios::floatfield ); 
    txtdumpfile << ( dobarrel ? 1 : 0 ) << "\t";
    txtdumpfile << rawId << "\t";
    txtdumpfile.precision(6);
    txtdumpfile.setf( std::ios::fixed, std:: ios::floatfield ); // floatfield set to fixed
    
    int ix = it->first / 1000;
    if(dobarrel && ix >= 85) ix = -1*(ix-85);
    if((!dobarrel) && ix >= 100) ix = -1*(ix-100);
    
    int iy = it->first % 1000;
    std::cout << "Writing templates histogram. ix = " << ix << "  iy = " << iy << " got " << norm_counts[it->first] << " events" << std::endl;
    TH1D *th = (TH1D*)htempl->Clone(Form("template_%d_%d",ix,iy));

    float pdfval[12];
    
    for(int iSample(3); iSample < 10; iSample++) {
      pdfval[iSample-3] = (it->second)[iSample] / norm_average[it->first];
      th->SetBinContent(iSample+1,pdfval[iSample-3]);   
      //      th->SetBinError(iSample+1,1./norm_average[it->first]);   
      if(norm_counts[it->first]<minNhits) pdfval[iSample-3] = simTemplate->GetBinContent(iSample+1); // PROTECTION !!!
      //      txtdumpfile << pdfval << "\t";
    }

    if(nCry%1000==0) std::cout << "Fitting the crystal # " << nCry << " with rawId = " << rawId << std::endl;
    TH1D *fittedh = fitTemplate(th,dobarrel,0,simTemplate);
    for(int iExtraSample(0); iExtraSample < 5; iExtraSample++) {
       // if(norm_average[it->first]>2) txtdumpfile << valExtrap[iExtraSample] << "\t";
       // else txtdumpfile << simTemplate->GetBinContent(iExtraSample+11) << "\t";
      if(norm_counts[it->first]>=minNhits) pdfval[7+iExtraSample] = valExtrap[iExtraSample];
      else pdfval[7+iExtraSample] = simTemplate->GetBinContent(iExtraSample+11);
    }

    /*
    // PROTECTION AGAINST STRANGE SHAPES
    if(fabs(pdfval[4]-simTemplate->GetBinContent(8))>0.1 || pdfval[0]==0) {
      //      std::cout << pdfval[4] << "   " << simTemplate->GetBinContent(8) << " ==> replacing..." << std::endl;
      for(int s=0; s<12; ++s) { 
	pdfval[s] = simTemplate->GetBinContent(s+4);
      }
    }
    */

    for(int s=0;s<12;++s) txtdumpfile << pdfval[s] << "\t";

    delete fittedh;

    txtdumpfile << std::endl;

    th->Write();
    
    if(norm_counts[it->first]>0) *htemplAverage = (*htemplAverage) + (*th);
    nCry += 1;
    delete th;
  }
  htemplAverage->Scale(1./float(nCry));
  //  std::cout << "Writing average template histogram (averaged over " << nCry << " crystals" << std::endl;
  //  htemplAverage->Write();
  
  outfile->Close();
  txtdumpfile.close();

}

void plotTemplates (int ixmin, int ixmax, int iymin, int iymax, bool doEB=true, bool shifted=false) {

  int maxCryToBePlotted = 10;
  
  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();
  mystyle->SetPalette(1);

  TFile *tfile;
  if(shifted) tfile = TFile::Open((doEB ? "shifted_template_histograms_EB.root" : "shifted_template_histograms_EE.root"));
  else tfile = TFile::Open((doEB ? "template_histograms_EB.root" : "template_histograms_EE.root"));
  if(!tfile) {
    std::cout << "File template_histograms.root not present" << std::endl;
    return;
  }

  THashList *listOfHists = (THashList*)gDirectory->GetListOfKeys();
  
  std::vector<TH1D*> templates;
  for(int i=0; i<listOfHists->GetSize(); ++i) {
    TString name = listOfHists->At(i)->GetName();
    TObjArray *tokens = name.Tokenize("_");
    int offset_tokens = shifted ? 1 : 0;
    TObjString *sieta = (TObjString*)tokens->At(1+offset_tokens);
    TObjString *siphi = (TObjString*)tokens->At(2+offset_tokens);
    int ieta = atoi(sieta->GetString().Data());
    int iphi = atoi(siphi->GetString().Data());
    delete tokens;
    if(doEB && (ieta<ixmin || ieta>ixmax || iphi<iymin || iphi>iymax)) continue;
    if(!doEB && (sqrt(pow(ieta-50,2)+pow(iphi-50,2))<ixmin || sqrt(pow(ieta-50,2)+pow(iphi-50,2))>ixmax)) continue;
    if(doEB) std::cout << "Getting template for " << name.Data() << "\tieta, iphi = " << ieta << ",\t" << iphi << std::endl;
    else std::cout << "Getting template for " << name.Data() << "\tir = " << sqrt(pow(ieta-50,2)+pow(iphi-50,2)) << std::endl;
    TH1D *hist = (TH1D*)gDirectory->Get(name.Data());
    templates.push_back(hist);
  }

  std::cout << "Templates taken. Now taking the sim. template..." << std::endl;

  TH1D *simPulseShape = simPulseShapeTemplate(doEB);

  std::cout << "Sim. template taken. Now Plotting..." << std::endl;
  
  TCanvas *c1 = new TCanvas("c1","",1200,1200);
  simPulseShape->GetXaxis()->SetTitle("Sample");
  simPulseShape->GetYaxis()->SetTitle("Ped. Subtracted Amplitude (a.u.)");
  simPulseShape->SetMinimum(-0.05);
  simPulseShape->SetMarkerStyle(kFullCircle);
  simPulseShape->SetMarkerSize(1);
  simPulseShape->SetFillColor(kAzure+1);
  simPulseShape->Draw("pe2");

  for(int i=0; i<(int)templates.size(); ++i) {
    templates[i]->SetLineColor(i+kOrange);
    templates[i]->Draw("same hist");
    if(i>maxCryToBePlotted) break;
  }  

  TString subdet = doEB ? "EB" : "EE";
  TString range = doEB ? Form("%d < |i #eta| < %d; %d < i #phi < %d",ixmin,ixmax,iymin,iymax) : 
    Form("%d < ir < %d",ixmin,ixmax);

  TPaveText *pt = new TPaveText(0.40, 0.90, 0.50, 0.99);
  pt->SetBorderSize(     0);
  pt->SetFillColor (  4000);
  pt->SetTextAlign (    12);
  pt->SetTextFont  (    42);
  pt->SetTextSize  (0.03);
  pt->AddText(subdet);  
  pt->AddText(range);  
  pt->Draw();

  TLegend* legend = new TLegend(0.20, 0.50, 0.40, 0.70);
  
  legend->SetBorderSize(     0);
  legend->SetFillColor (  4000);
  legend->SetTextAlign (    12);
  legend->SetTextFont  (    42);
  legend->SetTextSize  (0.03);
  
  legend->AddEntry(simPulseShape,"Sim. pulse shape","pf");
  legend->AddEntry(templates[0],"pp pulse shapes","l");
  legend->Draw();

  TString rangelbl = doEB ? Form("%d_ieta_%d_%d_iphi_%d",ixmin,ixmax,iymin,iymax) : 
    Form("%d_ir_%d",ixmin,ixmax);

  if(shifted) {
    c1->SaveAs(Form("plot_shifted_template_%s_%s.pdf",subdet.Data(),rangelbl.Data()));
    c1->SaveAs(Form("plot_shifted_template_%s_%s.png",subdet.Data(),rangelbl.Data()));
  } else {
    c1->SaveAs(Form("plot_template_%s_%s.pdf",subdet.Data(),rangelbl.Data()));
    c1->SaveAs(Form("plot_template_%s_%s.png",subdet.Data(),rangelbl.Data()));
  }

  tfile->Close();

}


void plotAllTemplates(bool shifted=false) {

  plotTemplates( 0,20,0,1, true, shifted);
  plotTemplates(20,40,0,1, true, shifted);
  plotTemplates(40,60,0,1, true, shifted);
  plotTemplates(60,85,0,1, true, shifted);

  plotTemplates(10,15,0,0, false, shifted);
  plotTemplates(15,20,0,0, false, shifted);
  plotTemplates(30,35,0,0, false, shifted);
  plotTemplates(45,50,0,0, false, shifted);
}



TProfile *alpha_prof, *beta_prof, *deltat_prof;
TH1D *alpha_hist, *beta_hist, *deltat_hist;
TH1D *s4s5_raw, *s4s5_fit, *val4val5_fit;

void fitTemplates (int ixmin, int ixmax, int iymin, int iymax, bool doEB=true) {

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();
  mystyle->SetPalette(1);

  float xlow = 0;
  float xup = doEB ? 85 : 50;
  int nbins = doEB ? 85 : 50;

  alpha_prof  = new TProfile("alpha_prof", "",nbins,xlow,xup,"s");
  beta_prof   = new TProfile("beta_prof",  "",nbins,xlow,xup,"s");
  deltat_prof = new TProfile("deltat_prof","",nbins,xlow,xup,"s");

  float alphaup = 1.4;
  float alphadown = doEB ? 0.8 : 0.5;
  alpha_hist  = new TH1D("alpha_hist", "",500,alphadown,alphaup);
  beta_hist   = new TH1D("beta_hist",  "",500,1.3,2.0);
  deltat_hist = new TH1D("deltat_hist","",500,-8.0,8.0);

  s4s5_raw = new TH1D("s4s5_raw", "", 500, 0.6, 0.9);
  s4s5_fit = new TH1D("s4s5_fit", "", 500, 0.6, 0.9);
  val4val5_fit = new TH1D("val4val5_fit", "", 500, 0.6, 0.9);

  TFile *tfile = TFile::Open((doEB ? "template_histograms_EB.root" : "template_histograms_EE.root"));
  if(!tfile) {
    std::cout << "File template_histograms.root not present" << std::endl;
    return;
  }

  THashList *listOfHists = (THashList*)gDirectory->GetListOfKeys();
  
  std::vector<TH1D*> templates;
  std::vector< std::pair<int,int> > coords;

  std::cout << "Getting templates, please wait..." << std::endl;
  for(int i=0; i<listOfHists->GetSize(); ++i) {
    TString name = listOfHists->At(i)->GetName();
    TObjArray *tokens = name.Tokenize("_");
    TObjString *sieta = (TObjString*)tokens->At(1);
    TObjString *siphi = (TObjString*)tokens->At(2);
    int ieta = atoi(sieta->GetString().Data());
    int iphi = atoi(siphi->GetString().Data());
    delete tokens;
    if(ieta<ixmin || ieta>ixmax || iphi<iymin || iphi>iymax) continue;
    // if(doEB) std::cout << "Getting template for " << name.Data() << "\tieta, iphi = " << ieta << ",\t" << iphi << std::endl;
    // else std::cout << "Getting template for " << name.Data() << "\tix, iy = " << ieta << ",\t" << iphi << std::endl;
    TH1D *hist = (TH1D*)gDirectory->Get(name.Data());
    s4s5_raw->Fill(hist->GetBinContent(5));
    templates.push_back(hist);
    coords.push_back(std::make_pair(ieta,iphi));
  }

  std::cout << "Templates taken. Now fit..." << std::endl;


  TString nameoutput = doEB ? "shifted_template_histograms_EB.root" : "shifted_template_histograms_EE.root";
  TFile *filet = TFile::Open(nameoutput.Data(),"recreate");

  TCanvas *c1 = new TCanvas("c1","",1200,1200);

  for(int i=0; i<(int)templates.size(); ++i) {
    filet->cd();
    TH1D *shifted_temp = fitTemplate(templates[i],doEB,0.0);
    shifted_temp->Write();
    delete shifted_temp;
    if(i%360==0) c1->SaveAs(Form("fits/fit%s_alphabeta_%s.pdf",(doEB ? "EB" : "EE"), templates[i]->GetName()) );
    float ix = doEB ? coords[i].first : sqrt(std::pow(coords[i].first - 50,2) + std::pow(coords[i].second - 50,2));
    // profile plots
    alpha_prof->Fill(fabs(ix),ALPHABETAT[0]);
    beta_prof->Fill(fabs(ix),ALPHABETAT[1]);
    deltat_prof->Fill(fabs(ix),25.*(ALPHABETAT[2]-5.5));
    // histograms plots
    alpha_hist->Fill(ALPHABETAT[0]);
    beta_hist->Fill(ALPHABETAT[1]);
    deltat_hist->Fill(25*(ALPHABETAT[2]-5.5));
    val4val5_fit->Fill(val4);
    s4s5_fit->Fill(s4s5fit);
  }

  tfile->Close();
  filet->Close();

}

void FitAllTemplatesSubdet(bool doEB) {

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();
  mystyle->SetPalette(1);

  TString subdet = doEB ? "EB" : "EE";
  TFile *resultfile = TFile::Open(Form("shape_results_%s.root",subdet.Data()), "recreate");
  if(doEB) fitTemplates(-85,85,0,360, true);
  else fitTemplates(-100,100,0,100, false);

  TCanvas *c1 = new TCanvas("c1","",1200,1200);
  
  std::vector<std::string> labels;
  labels.push_back("#alpha");
  labels.push_back("#beta");
  labels.push_back("#Delta t (ns)");
  labels.push_back("Sample_{4}/Sample_{5} (raw)");
  labels.push_back("Sample_{4}/Sample_{5} (fit)");
  labels.push_back("Val_{t_{max}-25 ns}/Val_{t_{max}} (fit)");
  
  std::vector<TProfile*> results_prof;
  results_prof.push_back(alpha_prof);
  results_prof.push_back(beta_prof);
  results_prof.push_back(deltat_prof);

  std::vector<TH1D*> results_hist;
  results_hist.push_back(alpha_hist);
  results_hist.push_back(beta_hist);
  results_hist.push_back(deltat_hist);
  results_hist.push_back(s4s5_raw);
  results_hist.push_back(s4s5_fit);
  results_hist.push_back(val4val5_fit);

  for(int i=0; i<(int)results_prof.size(); ++i) {
    mystyle->SetOptStat(1111111);
    resultfile->cd();
    TProfile *prof = results_prof[i];
    if(doEB) prof->GetXaxis()->SetTitle("i #eta");
    else  prof->GetXaxis()->SetTitle("iR");
    prof->GetYaxis()->SetTitle(labels[i].c_str());
    prof->Draw();
    prof->Write();
    c1->SaveAs(Form("shape_parameter_%s_%s.pdf",subdet.Data(),prof->GetName()) );
  }

  for(int i=0; i<(int)results_hist.size(); ++i) {
    TH1D *hist = results_hist[i];
    hist->GetXaxis()->SetTitle(labels[i].c_str());
    hist->GetYaxis()->SetTitle("# of crystals");
    if(!doEB) hist->Rebin(4);
    hist->Draw();
    TPaveText *pt = new TPaveText(0.20, 0.80, 0.40, 0.90,"brNDC");
    pt->SetBorderSize(     0);
    pt->SetFillColor (  4000);
    pt->SetTextAlign (    12);
    pt->SetTextFont  (    42);
    pt->SetTextSize  (0.03);
    pt->AddText(Form("mean = %1.3f",hist->GetMean()));
    pt->AddText(Form("rms/mean = %1.3f",hist->GetRMS()/hist->GetMean()));
    pt->Draw();
    hist->Write();
    c1->SaveAs(Form("shape_parameter_%s_%s.pdf",subdet.Data(),hist->GetName()) );
    delete pt;
  }

  resultfile->Close();
}

void FitAllTemplates() {
  FitAllTemplatesSubdet(true);
  FitAllTemplatesSubdet(false);
}




// ===========================================
// COVARIANCE MATRIX 
// ===========================================


void saveCovariances(bool dobarrel) {

  TFile *file = TFile::Open("/Users/emanuele/Work/data/ecalreco/multifit/templates_tree_pi0_2015C_lowPU.root");
  TTree *tree = (TTree*)file->Get("pulseDump/pulse_tree");

  Long64_t nentries = tree->GetEntries();
  std::cout << "The tree has " << nentries << " entries" << std::endl;
  
  Bool_t          barrel;
  UInt_t          gain;
  Double_t        pedrms;
  Double_t        pedval;
  Int_t           ietaix;
  Int_t           iphiiy;
  Int_t           iz;
  Double_t        pulse[10];
  UInt_t          rawid;

  tree->SetBranchAddress("barrel", &barrel);
  tree->SetBranchAddress("gain", &gain);
  tree->SetBranchAddress("pedrms", &pedrms);
  tree->SetBranchAddress("pedval", &pedval);
  tree->SetBranchAddress("ietaix", &ietaix);
  tree->SetBranchAddress("iphiiy", &iphiiy);
  tree->SetBranchAddress("iz", &iz);
  tree->SetBranchAddress("pulse", pulse);
  tree->SetBranchAddress("rawid", &rawid); 
  
  std::map<int, std::vector<double> > xy, x, xx;
  std::map<int, std::vector<double> > noise_xy, noise_x, noise_xx;
  std::map<int, std::vector<double> > templates_weight;
  std::map<int, double> norm_average;
  std::map<int, double> norm_counts;
  std::map<int, unsigned int> rawIds;

  // to reject noise" better in ADC not to make eta-dependent cuts
  // ~10 sigma from the 2012 plots: https://twiki.cern.ch/twiki/bin/view/CMSPublic/EcalDPGResultsCMSDP2013007
  float minAmplitude = dobarrel ? 14 : 26; 
  // to reject spikes
  float maxTimeShift = 8.0/25.; // ns
  float maxChi2 = 25.188; 
  int minNhits = 2;

  TH1D *htempl = new TH1D("htempl","",10,0,10);

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = tree->LoadTree(jentry);
    if (ientry < 0) break;
    nb = tree->GetEntry(jentry);   nbytes += nb;

    if(jentry%100000==0) std::cout << "Processing entry " << jentry << std::endl;

    if((dobarrel && (!barrel)) || (!dobarrel && barrel)) continue;

    int offset;
    if(barrel) offset = (ietaix > 0) ? 1000 * ietaix : 1000 * (abs(ietaix)+85);
    else offset = (iz > 0) ? 1000 * ietaix : 1000 * (ietaix+100);
    int ic = offset + iphiiy;
    
    //    std::cout << "ietaix = " << ietaix << "\toffset = " << offset << "\tic = " << ic << std::endl;

    TH1D *digis = (TH1D*)htempl->Clone(Form("rh_%d",ic));
    for(int iSample(0); iSample < 10; iSample++) digis->SetBinContent(iSample+1,pulse[iSample]);
    rechit rh = makeRecHit(digis,barrel,pedval);

    delete digis;

    // calc the max-sample
    int maxsample=5;
    double maxval=0;
    for(int iSample=2;iSample<10;++iSample) {
      double val = pulse[iSample];
      if(val>maxval) {
	maxval=val;
	maxsample=iSample;
      }
    }

    double weight = 1.0;
    //double weight = pow(rh.amplitude,2);

    if(rh.amplitude<minAmplitude || fabs(rh.time-5.5)>maxTimeShift || rh.chi2>maxChi2) continue;
    
    if(xy.count(ic)==0) {
      std::vector<double> this_xy, this_x, this_xx;
      this_xy.resize(100);
      this_x.resize(10);
      this_xx.resize(10);
      std::vector<double> this_noise_xy, this_noise_x, this_noise_xx;
      this_noise_xy.resize(100);
      this_noise_x.resize(10);
      this_noise_xx.resize(10);
      for(int ix(0); ix < 10; ix++) {
    	this_x[ix] = pulse[ix]/maxval * weight;
	this_xx[ix] = std::pow(pulse[ix]/maxval,2) * weight;
    	this_noise_x[ix] = noiseCorr(barrel,gain)[ix]*pedrms/maxval * weight;
	this_noise_xx[ix] = std::pow(noiseCorr(barrel,gain)[ix]*pedrms/maxval,2) * weight;
	//	std::cout << "ix = " << ix << "pedrms = " << pedrms << "   noisecorr = " << noiseCorr(barrel,gain)[ix] << " maxval = " << maxval << "  this_x[ix] = " << this_x[ix] << "   noise = " << this_noise_x[ix] << std::endl;
	for(int iy(0); iy < 10; iy++) {
	  int index = ix + 10*iy;
	  this_xy[index] = pulse[ix]/maxval * pulse[iy]/maxval * weight;
	  this_noise_xy[index] = noiseCorr(barrel,gain)[ix]*pedrms/maxval * noiseCorr(barrel,gain)[iy]*pedrms/maxval * weight;
	  // std::cout << "\t   iy = " << iy << "  this_xy[index] = " << this_xy[index] << "   noise = " << this_noise_xy[index] << std::endl;
	}
      }
      xy[ic] = this_xy;
      x[ic] = this_x;
      xx[ic] = this_xx;
      noise_xy[ic] = this_xy;
      noise_x[ic] = this_x;
      noise_xx[ic] = this_xx;
      norm_average[ic] = weight;
      norm_counts[ic] = 1;
      rawIds[ic] = rawid;
      // std::cout << "inserting new ped for DetId = " << ic << std::endl;
    } else {
      std::vector<double> &this_xy  = xy[ic];
      std::vector<double> &this_x   =  x[ic];
      std::vector<double> &this_xx  = xx[ic];
      std::vector<double> &this_noise_xy  = noise_xy[ic];
      std::vector<double> &this_noise_x   = noise_x[ic];
      std::vector<double> &this_noise_xx  = noise_xx[ic];
      for(int ix(0); ix < 10; ix++) {
	this_x[ix] += pulse[ix]/maxval * weight;
	this_xx[ix] += std::pow(pulse[ix]/maxval,2) * weight;
    	this_noise_x[ix] = noiseCorr(barrel,gain)[ix]*pedrms/maxval * weight;
	this_noise_xx[ix] = std::pow(noiseCorr(barrel,gain)[ix]*pedrms/maxval,2) * weight;
	for(int iy(0); iy < 10; iy++) {
	  int index = ix + 10*iy;
	  this_xy[index] += pulse[ix]/maxval * pulse[iy]/maxval * weight;
	  this_noise_xy[index] = noiseCorr(barrel,gain)[ix]*pedrms/maxval * noiseCorr(barrel,gain)[iy]*pedrms/maxval * weight;
	}
      }
      norm_average[ic] += weight;
      norm_counts[ic] ++;
      //        std::cout << "updating ped for DetId = " << ic << std::endl;
    }
  } // crystals x events

  TH2D *simCovariance = simPulseShapeCovariance(dobarrel);
  
  /// output
  TString nameoutput = dobarrel ? "templatecov_histograms_EB.root" : "templatecov_histograms_EE.root";
  TFile *outfile = TFile::Open(nameoutput.Data(),"recreate");
  TH2D *hcov = new TH2D("htempl","",10,0,10,10,0,10);
  TH2D *hcovAverage =  (TH2D*)hcov->Clone("covariance_average");
  int nCry(0);

  // text file to fill the DB objects with templates
  ofstream txtdumpfile;  
  TString nametxtoutput = dobarrel ? "template_covariances_EB.txt" : "template_covariances_EE.txt";
  txtdumpfile.open (nametxtoutput.Data(), ios::out | ios::trunc);

  int minHits = 50;
  int numLowstatCry = 0;
  bool verbose = true;
  for(std::map<int, std::vector<double> >::iterator it=xy.begin(); it!=xy.end(); ++it) {
    unsigned int rawId = rawIds[it->first];
    txtdumpfile.unsetf ( std::ios::floatfield ); 
    txtdumpfile << ( dobarrel ? 1 : 0 ) << "\t";
    txtdumpfile << rawId << "\t";
    txtdumpfile.precision(3);
    txtdumpfile.setf( std::ios::scientific ); // floatfield set to fixed

    int ix = it->first / 1000;
    if(dobarrel && ix >= 85) ix = -1*(ix-85);
    if((!dobarrel) && ix >= 100) ix = -1*(ix-100);
    int iy = it->first % 1000;
    // std::cout << "Writing templates histogram. ix = " << ix << "  iy = " << iy << " got " << nevts[it->first] << " events" << std::endl;
    TH2D *ch = (TH2D*)hcov->Clone(Form("templatecov_%d_%d",ix,iy));
    TH2D *chNotNorm = (TH2D*)hcov->Clone(Form("templatecovNotNorm_%d_%d",ix,iy));
    for(int index=0; index<100; ++index) {
      int ind_x = index % 10;
      int ind_y = index / 10;
      double E_xy = (it->second)[index] / norm_average[it->first];
      double E_x = (x[it->first])[ind_x] / norm_average[it->first];
      double E_y = (x[it->first])[ind_y] / norm_average[it->first];
      double E_xx = (xx[it->first])[ind_x] / norm_average[it->first];
      double E_yy = (xx[it->first])[ind_y] / norm_average[it->first];

      double E_noise_xy = (noise_xy[it->first])[index] / norm_average[it->first];
      double E_noise_x  = (noise_x[it->first])[ind_x]  / norm_average[it->first];
      double E_noise_y  = (noise_x[it->first])[ind_y]  / norm_average[it->first];
      double E_noise_xx = (noise_xx[it->first])[ind_x] / norm_average[it->first];
      double E_noise_yy = (noise_xx[it->first])[ind_y] / norm_average[it->first];
      
      double cov_tot = E_xy - E_x*E_y;
      double var_tot = std::sqrt(fabs(E_xx - E_x*E_x)) * std::sqrt(fabs(E_yy - E_y*E_y));

      double cov_noise = E_noise_xy - E_noise_x*E_noise_y;
      double var_noise = std::sqrt(fabs(E_noise_xx - E_noise_x*E_noise_x)) * std::sqrt(fabs(E_noise_yy - E_noise_y*E_noise_y));

      // double cov_temp = std::sqrt(pow(cov_tot,2) - pow(cov_noise,2));
      // double var_temp = std::sqrt(pow(var_tot,2) - pow(var_noise,2));

      double cov_temp = cov_tot;
      double var_temp = var_tot;

      // std::cout << "ind_x = " << ind_x << "   ind_y = " << ind_x << std::endl;
      // std::cout << "\tTot var = " << var_tot << std::endl;
      // std::cout << "\tNoise var = " << var_noise << std::endl;
      // std::cout << "\tTemp var = " << var_temp << std::endl;
      ch->SetBinContent(ind_x+1, ind_y+1, (var_temp==0 ? 0.0 : cov_temp/var_temp));
      chNotNorm->SetBinContent(ind_x+1, ind_y+1, cov_temp);
    }

    int numped=3;
    for(int index=0; index<225; ++index) {
      int i = index % 15;
      int j = index / 15;
      if(i<numped || j<numped) continue; // skip the 3 pedestal samples
      if(i<10 && j<10 && norm_counts[it->first]>=minHits) txtdumpfile << chNotNorm->GetBinContent(i+1,j+1) << "\t";
      else             txtdumpfile << simCovariance->GetBinContent(i-numped+1,j-numped+1) << "\t";
    }
    
    if(norm_counts[it->first]<minHits) numLowstatCry++;
    else ch->Write();

    // if(norm_counts[it->first]<10) std::cout << "WARNING: Measured pulse covariance for rawID = " << rawId 
    // 					     << " with limited statistics estimate: " << norm_average[it->first] << " pulses above " << minAmplitude << " ADCs. Replace with the simulation value" << std::endl;

    txtdumpfile << std::endl;
    if(norm_average[it->first]>0) *hcovAverage = (*hcovAverage) + (*ch);
    nCry += 1;
    delete ch;
    delete chNotNorm;
    verbose = false;
  }

  std::cout << "NOTE: " << numLowstatCry << " crystals have been filled with sim values because have less than " 
	    << minHits << " hits above " << minAmplitude << " ADC " << std::endl;

  hcovAverage->Scale(1./nCry);
  //  std::cout << "Writing average template histogram (averaged over " << nCry << " crystals" << std::endl;
  //  htemplAverage->Write();
  
  outfile->Close();

}



void plotCovarianceElements(bool doEB, bool diagonal=false) {

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();
  mystyle->SetPalette(1);

  int ixmin = doEB ? -85 : -100;
  int ixmax = doEB ?  85 :  100;
  int iymin = doEB ?   0 :  0;
  int iymax = doEB ? 360 :  100;
  
  TFile *file = 0;
  if(diagonal) file = TFile::Open(doEB ? "templatecov_unnormalized_histograms_EB.root" : "templatecov_unnormalized_histograms_EE.root");
  else file = TFile::Open(doEB ? "templatecov_histograms_EB.root" : "templatecov_histograms_EE.root");
  THashList *listOfHists = (THashList*)gDirectory->GetListOfKeys();
  
  std::cout << "Getting the templates..." << std::endl;

  std::vector<TH2D*> covariances;
  std::vector<double> ietair;

  for(int i=0; i<listOfHists->GetSize(); ++i) {
    TString name = listOfHists->At(i)->GetName();
    TObjArray *tokens = name.Tokenize("_");
    TObjString *sieta = (TObjString*)tokens->At(1);
    TObjString *siphi = (TObjString*)tokens->At(2);
    int ieta = atoi(sieta->GetString().Data());
    int iphi = atoi(siphi->GetString().Data());
    delete tokens;
    if(doEB && (ieta<ixmin || ieta>ixmax || iphi<iymin || iphi>iymax)) continue;
    if(!doEB && (sqrt(pow(ieta-50,2)+pow(iphi-50,2))<ixmin || sqrt(pow(ieta-50,2)+pow(iphi-50,2))>ixmax)) continue;
    //    if(doEB) std::cout << "Getting template for " << name.Data() << "\tieta, iphi = " << ieta << ",\t" << iphi << std::endl;
    //    else std::cout << "Getting template for " << name.Data() << "\tir = " << sqrt(pow(ieta-50,2)+pow(iphi-50,2)) << std::endl;
    TH2D *hist = (TH2D*)gDirectory->Get(name.Data());
    covariances.push_back(hist);
    
    if(doEB) ietair.push_back(ieta);
    else     ietair.push_back(sqrt(pow(ieta-50,2)+pow(iphi-50,2)));
  }

  std::cout << "Templates taken, now plotting 1D." << std::endl;

  TString subdet = doEB ? "EB" : "EE";
  TFile *output = TFile::Open(Form("covariance_elements_distributions_%s.root",subdet.Data()) ,"recreate");

  TH1D *correlation = new TH1D("correlation","",100,-1,1);
  TH1D *sigma = new TH1D("sigma","",100,0,0.01);
  TH1D *sigmaRaisingEdge = new TH1D("sigma","",100,0,0.05);

  float xlow = 0;
  float xup = doEB ? 85 : 50;
  int nbins = doEB ? 85 : 50;

  TProfile *profile = new TProfile("profile","",nbins, xlow, xup, "s");

  TCanvas *c1 = new TCanvas("c1","",1200,1200);

  if(diagonal) {
    for(int ix=0; ix<10; ++ix) {
      if(ix==5) continue;
      TH1D *corr1D = 0;
      if(ix==4) corr1D = (TH1D*)sigmaRaisingEdge->Clone(Form("sigma1D_%d_%d",ix,ix));
      else corr1D = (TH1D*)sigma->Clone(Form("sigma1D_%d_%d",ix,ix));
      TProfile *prof1D = (TProfile*)profile->Clone(Form("prof1D_%d_%d",ix,ix));
      for(int ichan=0; ichan<(int)covariances.size(); ++ichan) {
	double corrval = covariances[ichan]->GetBinContent(ix+1,ix+1);
	if(std::isnan(corrval) || std::isinf(corrval)) continue;
	corr1D->Fill(corrval);
	prof1D->Fill(fabs(ietair[ichan]),corrval);
      }
      
      corr1D->GetXaxis()->SetTitle("#sigma");
      corr1D->GetYaxis()->SetTitle("n. of crystals");
      corr1D->Draw();
      c1->SaveAs(Form("%s_%s.pdf",corr1D->GetName(),subdet.Data()));
      c1->SaveAs(Form("%s_%s.png",corr1D->GetName(),subdet.Data()));
      
      prof1D->GetXaxis()->SetTitle(doEB ? "i#eta" : "iR");
      prof1D->GetYaxis()->SetTitle("#sigma");
      prof1D->Draw();
      c1->SaveAs(Form("%s_%s.pdf",prof1D->GetName(),subdet.Data()));
      c1->SaveAs(Form("%s_%s.png",prof1D->GetName(),subdet.Data()));
      
      corr1D->Write();
      prof1D->Write();
    }    
  } else {
    for(int ix=0; ix<10; ++ix) for(int iy=0; iy<ix; ++iy) {
	if(ix==5 || iy==5) continue;
	TH1D *corr1D = (TH1D*)correlation->Clone(Form("corr1D_%d_%d",ix,iy));
	TProfile *prof1D = (TProfile*)profile->Clone(Form("prof1D_%d_%d",ix,iy));
	for(int ichan=0; ichan<(int)covariances.size(); ++ichan) {
	  double corrval = covariances[ichan]->GetBinContent(ix+1,iy+1);
	  if(std::isnan(corrval) || std::isinf(corrval)) continue;
	  corr1D->Fill(corrval);
	  prof1D->Fill(fabs(ietair[ichan]),corrval);
	}
	
	corr1D->GetXaxis()->SetTitle("correlation");
	corr1D->GetYaxis()->SetTitle("n. of crystals");
	corr1D->Draw();
	c1->SaveAs(Form("%s_%s.pdf",corr1D->GetName(),subdet.Data()));
	c1->SaveAs(Form("%s_%s.png",corr1D->GetName(),subdet.Data()));
	
	prof1D->GetXaxis()->SetTitle(doEB ? "i#eta" : "iR");
	prof1D->GetYaxis()->SetTitle("correlation");
	prof1D->Draw();
	c1->SaveAs(Form("%s_%s.pdf",prof1D->GetName(),subdet.Data()));
	c1->SaveAs(Form("%s_%s.png",prof1D->GetName(),subdet.Data()));
	
	corr1D->Write();
	prof1D->Write();
      }
  }
  
  output->Close();
  file->Close();

}


void saveNoiseCovariances(bool dobarrel) {

  TFile *file = TFile::Open("/Users/emanuele/Work/data/ecalreco/multifit/pedestal_templates_run195592.root");
  TTree *tree = (TTree*)file->Get("pulseDump/pulse_tree");

  Long64_t nentries = tree->GetEntries();
  std::cout << "The tree has " << nentries << " entries" << std::endl;
  
  Bool_t          barrel;
  UInt_t          gain;
  Double_t        pedrms;
  Double_t        pedval;
  Int_t           ietaix;
  Int_t           iphiiy;
  Int_t           iz;
  Double_t        pulse[10];
  UInt_t          rawid;

  tree->SetBranchAddress("barrel", &barrel);
  tree->SetBranchAddress("gain", &gain);
  tree->SetBranchAddress("pedrms", &pedrms);
  tree->SetBranchAddress("pedval", &pedval);
  tree->SetBranchAddress("ietaix", &ietaix);
  tree->SetBranchAddress("iphiiy", &iphiiy);
  tree->SetBranchAddress("iz", &iz);
  tree->SetBranchAddress("pulse", pulse);
  tree->SetBranchAddress("rawid", &rawid); 
  
  std::map<int, std::vector<double> > xy, x, xx;
  std::map<int, std::vector<double> > templates_weight;
  std::map<int, double> norm_average;
  std::map<int, unsigned int> rawIds;

  float minEnergy = 15;

  float adcToGeV = dobarrel ? 0.035 : 0.06;
  float minAmplitude = minEnergy / adcToGeV;

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = tree->LoadTree(jentry);
    if (ientry < 0) break;
    nb = tree->GetEntry(jentry);   nbytes += nb;

    if(jentry%1000000==0) std::cout << "Processing entry " << jentry << std::endl;
    
    if((dobarrel && (!barrel)) || (!dobarrel && barrel)) continue;

    int offset;
    if(barrel) offset = (ietaix > 0) ? 1000 * ietaix : 1000 * (abs(ietaix)+85);
    else offset = (iz > 0) ? 1000 * ietaix : 1000 * (ietaix+100);
    int ic = offset + iphiiy;
    
    //    std::cout << "ietaix = " << ietaix << "\toffset = " << offset << "\tic = " << ic << std::endl;

    double norm = 1.0;
    double weight = 1.0;
    
    if(xy.count(ic)==0) {
      std::vector<double> this_xy, this_x, this_xx;
      this_xy.resize(100);
      this_x.resize(10);
      this_xx.resize(10);
      for(int ix(0); ix < 10; ix++) {
    	this_x[ix] = pulse[ix]/norm * weight;
	this_xx[ix] = std::pow(pulse[ix]/norm,2) * weight;
	for(int iy(0); iy < 10; iy++) {
	  int index = ix + 10*iy;
	  this_xy[index] = pulse[ix]/norm * pulse[iy]/norm * weight;
	}
      }
      xy[ic] = this_xy;
      x[ic] = this_x;
      xx[ic] = this_xx;
      norm_average[ic] = weight;
      rawIds[ic] = rawid;
      // std::cout << "inserting new ped for DetId = " << ic << std::endl;
    } else {
      std::vector<double> &this_xy  = xy[ic];
      std::vector<double> &this_x   =  x[ic];
      std::vector<double> &this_xx  = xx[ic];
      for(int ix(0); ix < 10; ix++) {
	this_x[ix] += pulse[ix]/norm * weight;
	this_xx[ix] += std::pow(pulse[ix]/norm,2) * weight;
	for(int iy(0); iy < 10; iy++) {
	  int index = ix + 10*iy;
	  this_xy[index] += pulse[ix]/norm * pulse[iy]/norm * weight;
	}
      }
      norm_average[ic] += weight;
      //        std::cout << "updating ped for DetId = " << ic << std::endl;
    }
  } // crystals x events

  TH2D *simCovariance = simPulseShapeCovariance(dobarrel);
  
  /// output
  TString nameoutput = dobarrel ? "noisecov_histograms_EB.root" : "noisecov_histograms_EE.root";
  TFile *outfile = TFile::Open(nameoutput.Data(),"recreate");
  TH2D *hcov = new TH2D("htempl","",10,0,10,10,0,10);
  TH2D *hcovAverage =  (TH2D*)hcov->Clone("covariance_average");
  int nCry(0);

  bool verbose = true;
  for(std::map<int, std::vector<double> >::iterator it=xy.begin(); it!=xy.end(); ++it) {
    unsigned int rawId = rawIds[it->first];

    int ix = it->first / 1000;
    if(dobarrel && ix >= 85) ix = -1*(ix-85);
    if((!dobarrel) && ix >= 100) ix = -1*(ix-100);
    int iy = it->first % 1000;
    // std::cout << "Writing templates histogram. ix = " << ix << "  iy = " << iy << " got " << nevts[it->first] << " events" << std::endl;
    TH2D *ch = (TH2D*)hcov->Clone(Form("templatecov_%d_%d",ix,iy));
    TH2D *chNotNorm = (TH2D*)hcov->Clone(Form("templatecovNotNorm_%d_%d",ix,iy));
    for(int index=0; index<100; ++index) {
      int ind_x = index % 10;
      int ind_y = index / 10;
      double E_xy = (it->second)[index] / norm_average[it->first];
      double E_x = (x[it->first])[ind_x] / norm_average[it->first];
      double E_y = (x[it->first])[ind_y] / norm_average[it->first];
      double E_xx = (xx[it->first])[ind_x] / norm_average[it->first];
      double E_yy = (xx[it->first])[ind_y] / norm_average[it->first];

      double cov = E_xy - E_x*E_y;
      double var = std::sqrt(fabs(E_xx - E_x*E_x)) * std::sqrt(fabs(E_yy - E_y*E_y));
      ch->SetBinContent(ind_x+1, ind_y+1, (var==0 ? 0.0 : cov/var));
      chNotNorm->SetBinContent(ind_x+1, ind_y+1, sqrt(cov));
    } 


    ch->Write();
    if(norm_average[it->first]>0) *hcovAverage = (*hcovAverage) + (*ch);
    nCry += 1;
    delete ch;
    delete chNotNorm;
    verbose = false;
  }
  hcovAverage->Scale(1./nCry);
  hcovAverage->Write();
  //  std::cout << "Writing average template histogram (averaged over " << nCry << " crystals" << std::endl;
  //  htemplAverage->Write();
  
  outfile->Close();

}


void draw1DSamples(bool dobarrel, const char *txtdumpfile = "template_histograms_ECAL.txt") {

  ifstream myfile;
  myfile.open(txtdumpfile);
  float samples[12];
  int iseb, detid;
  TH1F *h_1d = new TH1F("h_1d","",1000,0.,1.);
  TH1F *h_samples[12];
  for(int s=0; s<12; ++s) h_samples[s] = (TH1F*)h_1d->Clone(Form("h_1d_sample%d",s));

 if (myfile.is_open()) {
   while ( !myfile.eof() ) {
     myfile >> iseb;
     myfile >> detid;
     for(int s=0; s<12; ++s) {
       myfile >> samples[s];
       if((dobarrel && iseb==1) || (!dobarrel && iseb==0)) h_samples[s]->Fill(samples[s]);
     }
   }
    myfile.close();
  }
  else cout << "Unable to open file"; 

 TCanvas *c1 = new TCanvas("c1","",1200,1200);
 c1->SetLogy();
 for(int s=0; s<12; ++s) {
   h_samples[s]->Draw();
   c1->SaveAs(Form("h_1d_sample%d.pdf",s));
   c1->SaveAs(Form("h_1d_sample%d.png",s));
 }

}
