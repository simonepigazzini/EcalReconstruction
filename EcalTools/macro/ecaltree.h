//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Apr 29 15:40:17 2014 by ROOT version 5.34/10
// from TTree ntp0/ntp0
// found on file: root://eoscms//store/group/phys_egamma/emanuele/ecal/time/ntuples/timeweights-from710/rereco_9.root
//////////////////////////////////////////////////////////

#ifndef ecaltree_h
#define ecaltree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class ecaltree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           runNumber;
   ULong64_t       eventNumber;
   Int_t           lumiBlock;
   Int_t           bunchCrossing;
   Int_t           orbitNumber;
   Double_t        beamSpotX;
   Double_t        beamSpotY;
   Double_t        beamSpotZ;
   Int_t           nBX;
   Int_t           nObsPU[16];   //[nBX]
   Float_t         nTruePU[16];   //[nBX]
   Int_t           bxPU[16];   //[nBX]
   Int_t           nEBRecHits;
   Float_t         etaEBRecHits[10000];   //[nEBRecHits]
   Float_t         phiEBRecHits[10000];   //[nEBRecHits]
   Float_t         ixEBRecHits[10000];   //[nEBRecHits]
   Float_t         iyEBRecHits[10000];   //[nEBRecHits]
   Float_t         energyEBRecHits[10000];   //[nEBRecHits]
   Float_t         timeEBRecHits[10000];   //[nEBRecHits]
   Float_t         timeerrorEBRecHits[10000];   //[nEBRecHits]
   Float_t         chi2EBRecHits[10000];   //[nEBRecHits]
   Float_t         swissXEBRecHits[10000];   //[nEBRecHits]
   Float_t         r9EBRecHits[10000];   //[nEBRecHits]
   Float_t         ootenergyEBRecHits[10000];   //[nEBRecHits]
   Float_t         ootchi2EBRecHits[10000];   //[nEBRecHits]
   Int_t           nEERecHits;
   Float_t         etaEERecHits[10000];   //[nEERecHits]
   Float_t         phiEERecHits[10000];   //[nEERecHits]
   Float_t         ixEERecHits[10000];   //[nEERecHits]
   Float_t         iyEERecHits[10000];   //[nEERecHits]
   Float_t         energyEERecHits[10000];   //[nEERecHits]
   Float_t         timeEERecHits[10000];   //[nEERecHits]
   Float_t         timeerrorEERecHits[10000];   //[nEERecHits]
   Float_t         chi2EERecHits[10000];   //[nEERecHits]
   Float_t         swissXEERecHits[10000];   //[nEERecHits]
   Float_t         r9EERecHits[10000];   //[nEERecHits]
   Float_t         ootenergyEERecHits[10000];   //[nEERecHits]
   Float_t         ootchi2EERecHits[10000];   //[nEERecHits]
   Int_t           nEBRecHits2;
   Float_t         etaEBRecHits2[10000];   //[nEBRecHits2]
   Float_t         phiEBRecHits2[10000];   //[nEBRecHits2]
   Float_t         ixEBRecHits2[10000];   //[nEBRecHits2]
   Float_t         iyEBRecHits2[10000];   //[nEBRecHits2]
   Float_t         energyEBRecHits2[10000];   //[nEBRecHits2]
   Float_t         timeEBRecHits2[10000];   //[nEBRecHits2]
   Float_t         timeerrorEBRecHits2[10000];   //[nEBRecHits2]
   Float_t         chi2EBRecHits2[10000];   //[nEBRecHits2]
   Float_t         swissXEBRecHits2[10000];   //[nEBRecHits2]
   Float_t         r9EBRecHits2[10000];   //[nEBRecHits2]
   Float_t         ootenergyEBRecHits2[10000];   //[nEBRecHits2]
   Float_t         ootchi2EBRecHits2[10000];   //[nEBRecHits2]
   Int_t           nEERecHits2;
   Float_t         etaEERecHits2[10000];   //[nEERecHits2]
   Float_t         phiEERecHits2[10000];   //[nEERecHits2]
   Float_t         ixEERecHits2[10000];   //[nEERecHits2]
   Float_t         iyEERecHits2[10000];   //[nEERecHits2]
   Float_t         energyEERecHits2[10000];   //[nEERecHits2]
   Float_t         timeEERecHits2[10000];   //[nEERecHits2]
   Float_t         timeerrorEERecHits2[10000];   //[nEERecHits2]
   Float_t         chi2EERecHits2[10000];   //[nEERecHits2]
   Float_t         swissXEERecHits2[10000];   //[nEERecHits2]
   Float_t         r9EERecHits2[10000];   //[nEERecHits2]
   Float_t         ootenergyEERecHits2[10000];   //[nEERecHits2]
   Float_t         ootchi2EERecHits2[10000];   //[nEERecHits2]

   // List of branches
   TBranch        *b_runNumber;   //!
   TBranch        *b_eventNumber;   //!
   TBranch        *b_lumiBlock;   //!
   TBranch        *b_bunchCrossing;   //!
   TBranch        *b_orbitNumber;   //!
   TBranch        *b_beamSpotX;   //!
   TBranch        *b_beamSpotY;   //!
   TBranch        *b_beamSpotZ;   //!
   TBranch        *b_nBX;   //!
   TBranch        *b_nObsPU;   //!
   TBranch        *b_nTruePU;   //!
   TBranch        *b_bxPU;   //!
   TBranch        *b_nEBRecHits;   //!
   TBranch        *b_etaEBRecHits;   //!
   TBranch        *b_phiEBRecHits;   //!
   TBranch        *b_ixEBRecHits;   //!
   TBranch        *b_iyEBRecHits;   //!
   TBranch        *b_energyEBRecHits;   //!
   TBranch        *b_timeEBRecHits;   //!
   TBranch        *b_timeerrorEBRecHits;   //!
   TBranch        *b_chi2EBRecHits;   //!
   TBranch        *b_swissXEBRecHits;   //!
   TBranch        *b_r9EBRecHits;   //!
   TBranch        *b_ootenergyEBRecHits;   //!
   TBranch        *b_ootchi2EBRecHits;   //!
   TBranch        *b_nEERecHits;   //!
   TBranch        *b_etaEERecHits;   //!
   TBranch        *b_phiEERecHits;   //!
   TBranch        *b_ixEERecHits;   //!
   TBranch        *b_iyEERecHits;   //!
   TBranch        *b_energyEERecHits;   //!
   TBranch        *b_timeEERecHits;   //!
   TBranch        *b_timeerrorEERecHits;   //!
   TBranch        *b_chi2EERecHits;   //!
   TBranch        *b_swissXEERecHits;   //!
   TBranch        *b_r9EERecHits;   //!
   TBranch        *b_ootenergyEERecHits;   //!
   TBranch        *b_ootchi2EERecHits;   //!
   TBranch        *b_nEBRecHits2;   //!
   TBranch        *b_etaEBRecHits2;   //!
   TBranch        *b_phiEBRecHits2;   //!
   TBranch        *b_ixEBRecHits2;   //!
   TBranch        *b_iyEBRecHits2;   //!
   TBranch        *b_energyEBRecHits2;   //!
   TBranch        *b_timeEBRecHits2;   //!
   TBranch        *b_timeerrorEBRecHits2;   //!
   TBranch        *b_chi2EBRecHits2;   //!
   TBranch        *b_swissXEBRecHits2;   //!
   TBranch        *b_r9EBRecHits2;   //!
   TBranch        *b_ootenergyEBRecHits2;   //!
   TBranch        *b_ootchi2EBRecHits2;   //!
   TBranch        *b_nEERecHits2;   //!
   TBranch        *b_etaEERecHits2;   //!
   TBranch        *b_phiEERecHits2;   //!
   TBranch        *b_ixEERecHits2;   //!
   TBranch        *b_iyEERecHits2;   //!
   TBranch        *b_energyEERecHits2;   //!
   TBranch        *b_timeEERecHits2;   //!
   TBranch        *b_timeerrorEERecHits2;   //!
   TBranch        *b_chi2EERecHits2;   //!
   TBranch        *b_swissXEERecHits2;   //!
   TBranch        *b_r9EERecHits2;   //!
   TBranch        *b_ootenergyEERecHits2;   //!
   TBranch        *b_ootchi2EERecHits2;   //!

   ecaltree(TTree *tree=0);
   virtual ~ecaltree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(const char *outputfilename);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ecaltree_cxx
ecaltree::ecaltree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://eoscms//store/group/phys_egamma/emanuele/ecal/time/ntuples/timeweights-from710/rereco_9.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("root://eoscms//store/group/phys_egamma/emanuele/ecal/time/ntuples/timeweights-from710/rereco_9.root");
      }
      f->GetObject("ntp0",tree);

   }
   Init(tree);
}

ecaltree::~ecaltree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ecaltree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ecaltree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void ecaltree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("eventNumber", &eventNumber, &b_eventNumber);
   fChain->SetBranchAddress("lumiBlock", &lumiBlock, &b_lumiBlock);
   fChain->SetBranchAddress("bunchCrossing", &bunchCrossing, &b_bunchCrossing);
   fChain->SetBranchAddress("orbitNumber", &orbitNumber, &b_orbitNumber);
   fChain->SetBranchAddress("beamSpotX", &beamSpotX, &b_beamSpotX);
   fChain->SetBranchAddress("beamSpotY", &beamSpotY, &b_beamSpotY);
   fChain->SetBranchAddress("beamSpotZ", &beamSpotZ, &b_beamSpotZ);
   fChain->SetBranchAddress("nBX", &nBX, &b_nBX);
   fChain->SetBranchAddress("nObsPU", nObsPU, &b_nObsPU);
   fChain->SetBranchAddress("nTruePU", nTruePU, &b_nTruePU);
   fChain->SetBranchAddress("bxPU", bxPU, &b_bxPU);
   fChain->SetBranchAddress("nEBRecHits", &nEBRecHits, &b_nEBRecHits);
   fChain->SetBranchAddress("etaEBRecHits", etaEBRecHits, &b_etaEBRecHits);
   fChain->SetBranchAddress("phiEBRecHits", phiEBRecHits, &b_phiEBRecHits);
   fChain->SetBranchAddress("ixEBRecHits", ixEBRecHits, &b_ixEBRecHits);
   fChain->SetBranchAddress("iyEBRecHits", iyEBRecHits, &b_iyEBRecHits);
   fChain->SetBranchAddress("energyEBRecHits", energyEBRecHits, &b_energyEBRecHits);
   fChain->SetBranchAddress("timeEBRecHits", timeEBRecHits, &b_timeEBRecHits);
   fChain->SetBranchAddress("timeerrorEBRecHits", timeerrorEBRecHits, &b_timeerrorEBRecHits);
   fChain->SetBranchAddress("chi2EBRecHits", chi2EBRecHits, &b_chi2EBRecHits);
   fChain->SetBranchAddress("swissXEBRecHits", swissXEBRecHits, &b_swissXEBRecHits);
   fChain->SetBranchAddress("r9EBRecHits", r9EBRecHits, &b_r9EBRecHits);
   fChain->SetBranchAddress("ootenergyEBRecHits", ootenergyEBRecHits, &b_ootenergyEBRecHits);
   fChain->SetBranchAddress("ootchi2EBRecHits", ootchi2EBRecHits, &b_ootchi2EBRecHits);
   fChain->SetBranchAddress("nEERecHits", &nEERecHits, &b_nEERecHits);
   fChain->SetBranchAddress("etaEERecHits", etaEERecHits, &b_etaEERecHits);
   fChain->SetBranchAddress("phiEERecHits", phiEERecHits, &b_phiEERecHits);
   fChain->SetBranchAddress("ixEERecHits", ixEERecHits, &b_ixEERecHits);
   fChain->SetBranchAddress("iyEERecHits", iyEERecHits, &b_iyEERecHits);
   fChain->SetBranchAddress("energyEERecHits", energyEERecHits, &b_energyEERecHits);
   fChain->SetBranchAddress("timeEERecHits", timeEERecHits, &b_timeEERecHits);
   fChain->SetBranchAddress("timeerrorEERecHits", timeerrorEERecHits, &b_timeerrorEERecHits);
   fChain->SetBranchAddress("chi2EERecHits", chi2EERecHits, &b_chi2EERecHits);
   fChain->SetBranchAddress("swissXEERecHits", swissXEERecHits, &b_swissXEERecHits);
   fChain->SetBranchAddress("r9EERecHits", r9EERecHits, &b_r9EERecHits);
   fChain->SetBranchAddress("ootenergyEERecHits", ootenergyEERecHits, &b_ootenergyEERecHits);
   fChain->SetBranchAddress("ootchi2EERecHits", ootchi2EERecHits, &b_ootchi2EERecHits);
   fChain->SetBranchAddress("nEBRecHits2", &nEBRecHits2, &b_nEBRecHits2);
   fChain->SetBranchAddress("etaEBRecHits2", etaEBRecHits2, &b_etaEBRecHits2);
   fChain->SetBranchAddress("phiEBRecHits2", phiEBRecHits2, &b_phiEBRecHits2);
   fChain->SetBranchAddress("ixEBRecHits2", ixEBRecHits2, &b_ixEBRecHits2);
   fChain->SetBranchAddress("iyEBRecHits2", iyEBRecHits2, &b_iyEBRecHits2);
   fChain->SetBranchAddress("energyEBRecHits2", energyEBRecHits2, &b_energyEBRecHits2);
   fChain->SetBranchAddress("timeEBRecHits2", timeEBRecHits2, &b_timeEBRecHits2);
   fChain->SetBranchAddress("timeerrorEBRecHits2", timeerrorEBRecHits2, &b_timeerrorEBRecHits2);
   fChain->SetBranchAddress("chi2EBRecHits2", chi2EBRecHits2, &b_chi2EBRecHits2);
   fChain->SetBranchAddress("swissXEBRecHits2", swissXEBRecHits2, &b_swissXEBRecHits2);
   fChain->SetBranchAddress("r9EBRecHits2", r9EBRecHits2, &b_r9EBRecHits2);
   fChain->SetBranchAddress("ootenergyEBRecHits2", ootenergyEBRecHits2, &b_ootenergyEBRecHits2);
   fChain->SetBranchAddress("ootchi2EBRecHits2", ootchi2EBRecHits2, &b_ootchi2EBRecHits2);
   fChain->SetBranchAddress("nEERecHits2", &nEERecHits2, &b_nEERecHits2);
   fChain->SetBranchAddress("etaEERecHits2", etaEERecHits2, &b_etaEERecHits2);
   fChain->SetBranchAddress("phiEERecHits2", phiEERecHits2, &b_phiEERecHits2);
   fChain->SetBranchAddress("ixEERecHits2", ixEERecHits2, &b_ixEERecHits2);
   fChain->SetBranchAddress("iyEERecHits2", iyEERecHits2, &b_iyEERecHits2);
   fChain->SetBranchAddress("energyEERecHits2", energyEERecHits2, &b_energyEERecHits2);
   fChain->SetBranchAddress("timeEERecHits2", timeEERecHits2, &b_timeEERecHits2);
   fChain->SetBranchAddress("timeerrorEERecHits2", timeerrorEERecHits2, &b_timeerrorEERecHits2);
   fChain->SetBranchAddress("chi2EERecHits2", chi2EERecHits2, &b_chi2EERecHits2);
   fChain->SetBranchAddress("swissXEERecHits2", swissXEERecHits2, &b_swissXEERecHits2);
   fChain->SetBranchAddress("r9EERecHits2", r9EERecHits2, &b_r9EERecHits2);
   fChain->SetBranchAddress("ootenergyEERecHits2", ootenergyEERecHits2, &b_ootenergyEERecHits2);
   fChain->SetBranchAddress("ootchi2EERecHits2", ootchi2EERecHits2, &b_ootchi2EERecHits2);
   Notify();
}

Bool_t ecaltree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ecaltree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ecaltree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ecaltree_cxx
