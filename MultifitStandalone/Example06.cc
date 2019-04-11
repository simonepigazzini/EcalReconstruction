//
// MultiFit amplitude reconstruction vs deltaT shift
// To run:
// > g++ -o Example06 Example06.cc PulseChiSqSNNLS.cc -std=c++11 `root-config --cflags --glibs`
// > ./Example06
//

#include <iostream>
#include "PulseChiSqSNNLS.h"
#include "Pulse.h"

#include "TTree.h"
#include "TF1.h"
#include "TProfile.h"
#include "TH2.h"
#include "TFile.h"
 
using namespace std;

Pulse pSh;

FullSampleVector fullpulse(FullSampleVector::Zero());
FullSampleMatrix fullpulsecov(FullSampleMatrix::Zero());
SampleMatrix noisecor(SampleMatrix::Zero());
BXVector activeBX;
SampleVector amplitudes(SampleVector::Zero());

int NSTEPS=21;

TFile *fout;
TTree *treeout;
TH1D *hsteps;
double amplitude[21];
double amplitudeTruth;
// double steps[19] = {-10,-5,-4,-3,-2,-1,-0.5,-0.25,-0.1,
//                     0.0,
//                     0.1,0.25,0.5,1.,2.,3,4,5,10};
double steps[21] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,
                    0.0,
                    0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1};

void initHist()
{
  fout = new TFile("output.root","recreate");
  treeout = new TTree("amplitudes","tree with reco amplitudes");
  hsteps = new TH1D("hsteps","",NSTEPS,0,NSTEPS);
  for(int i=0;i<NSTEPS;++i) {
    hsteps->SetBinContent(i+1,steps[i]);
  }
  treeout->Branch("amplitude", amplitude, Form("amplitude[%i]/D",NSTEPS));
  treeout->Branch("amplitudeTruth", &amplitudeTruth, "amplitudeTruth/D");

  for (int i=0; i<NSAMPLES; ++i) {
    for (int j=0; j<NSAMPLES; ++j) {
      int vidx = std::abs(j-i);
      noisecor(i,j) = pSh.corr(vidx);
    }
  }

  int activeBXs[] = { -5, -4, -3, -2, -1,  0,  1,  2,  3,  4 };
  activeBX.resize(10);
  for (unsigned int ibx=0; ibx<10; ++ibx) {
    activeBX.coeffRef(ibx) = activeBXs[ibx];
  } 
  //  activeBX.resize(1);
  //  activeBX.coeffRef(0) = 0;

}

void init(int step)
{
  float deltat = steps[step];
  // intime sample is [2]
  double pulseShapeTemplate[NSAMPLES+2];
  for(int i=0; i<(NSAMPLES+2); i++){
    double x = double( IDSTART + NFREQ * (i + 3) - WFLENGTH / 2);
    pulseShapeTemplate[i] = pSh.fShape(x-deltat);
  }
  for (int i=0; i<(NSAMPLES+2); ++i) {
    //pulseShapeTemplate[i] /= pulseShapeTemplate[2];
    fullpulse(i+7) = pulseShapeTemplate[i];
  }
}



void run()
{

  // TFile *file2 = new TFile("data/samples_signal_10GeV_pu_0.root");
  TFile *file2 = TFile::Open("data/samples_signal_50GeV_eta_0.0_pu_40.root");
  // TFile *file2 = TFile::Open("data/samples_signal_10GeV_eta_0.0_pu_140.root");

  double samples[NSAMPLES];
  TTree *tree = (TTree*)file2->Get("Samples");
  tree->SetBranchAddress("amplitudeTruth",      &amplitudeTruth);
  tree->SetBranchAddress("samples",             samples);
  int nentries = tree->GetEntries();

  for(int ievt=0; ievt<nentries; ++ievt){
    tree->GetEntry(ievt);
    for(int i=0; i<NSAMPLES; i++){
      amplitudes[i] = samples[i];
    }

    std::cout << "PROCESSING EVENT " << ievt << std::endl;
    double pedval = 0.;
    double pedrms = 1.0;
    PulseChiSqSNNLS pulsefunc;

    pulsefunc.disableErrorCalculation();

    for(int step=0; step<NSTEPS; ++step) {
      init(step);
      bool status = pulsefunc.DoFit(amplitudes,noisecor,pedrms,activeBX,fullpulse,fullpulsecov);
      double chisq = pulsefunc.ChiSq();

      unsigned int ipulseintime = 0;
      for (unsigned int ipulse=0; ipulse<pulsefunc.BXs().rows(); ++ipulse) {
        if (pulsefunc.BXs().coeff(ipulse)==0) {
          ipulseintime = ipulse;
          break;
        }
      }
      double aMax = status ? pulsefunc.X()[ipulseintime] : 0.;
      //  double aErr = status ? pulsefunc.Errors()[ipulseintime] : 0.;
      
      amplitude[step] = aMax;

    }
    treeout->Fill();
  }
}

void saveHist()
{

  fout->cd();
  treeout->Write();
  hsteps->Write();
  fout->Close();
}



# ifndef __CINT__
int main()
{
  initHist();
  run();
  saveHist();
  return 0;
}
# endif
