//
// MultiFit amplitude reconstruction vs deltaT shift
// To run:
// > g++ -o Example06_weights_vsPU Example06_weights_vsPU.cc -std=c++11 `root-config --cflags --glibs`
// > ./Example06_vsPU
//

#include <iostream>
#include "PulseChiSqSNNLS.h"
#include "Pulse.h"

#include "TTree.h"
#include "TF1.h"
#include "TProfile.h"
#include "TH2.h"
#include "TFile.h"
#include "TString.h"

using namespace std;

Pulse pSh;

TFile *fout;
TTree *treeout;
double amplitudeTruth;
double amplitude;

// https://cmssdt.cern.ch/lxr/source/CalibCalorimetry/EcalTrivialCondModules/data/ampWeights_CMS.txt?v=CMSSW_5_3_22
double weights[10] = {0,0,0, 0,   0.235699,  0.4228363,  0.3298652,  0.1575187, -0.002082776,          0 };

void initHist(int nPU)
{
  fout = new TFile(TString::Format("data/reco_wgt_signal_2GeV_eta_0.0_pu_%d.root",nPU),"recreate");
  treeout = new TTree("amplitudes","tree with reco amplitudes");
  treeout->Branch("amplitude", &amplitude, "amplitude/D");
  treeout->Branch("amplitudeTruth", &amplitudeTruth, "amplitudeTruth/D");

}


void run(int nPU)
{

  // TFile *file2 = new TFile("data/samples_signal_10GeV_pu_0.root");
  TFile *file2 = TFile::Open(TString::Format("data/samples_signal_2GeV_eta_0.0_pu_%d.root",nPU));
  // TFile *file2 = TFile::Open("data/samples_signal_10GeV_eta_2.5_pu_140.root");

  double samples[NSAMPLES];
  TTree *tree = (TTree*)file2->Get("Samples");
  tree->SetBranchAddress("amplitudeTruth",      &amplitudeTruth);
  tree->SetBranchAddress("samples",             samples);
  int nentries = tree->GetEntries();

  for(int ievt=0; ievt<nentries; ++ievt){
    tree->GetEntry(ievt);

    std::cout << "Processing event " << ievt << " ..." << std::endl;

    double amp =0;
    for (int is=0; is<NSAMPLES; ++is) {
      amp += weights[is] * samples[is];
    }
    amplitude = amp;
    
    treeout->Fill();
  }
}

void saveHist()
{

  fout->cd();
  treeout->Write();
  fout->Close();
}



# ifndef __CINT__
int main(int argc, char** argv)
{

  if (argc<2) {
    std::cout << "Give the number of PU interactions as argument" << endl;
    return 0;
  }

  int npu = atoi(argv[1]);
  std::cout << "RECONSTRUCT SAMPLE WITH PU = " << npu << std::endl;
  initHist(npu);
  run(npu);
  saveHist();

  return 0;
}
# endif
