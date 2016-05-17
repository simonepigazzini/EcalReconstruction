//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Dec 12 19:21:11 2014 by ROOT version 5.34/22
// from TTree probe_tree/probe_tree
// found on file: output_0.root
//////////////////////////////////////////////////////////

#ifndef MultifitValidation_h
#define MultifitValidation_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLorentzVector.h>
#include <string>
#include <map>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class MultifitValidation {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         l1bdtID;
   Float_t         l1classification;
   Float_t         l1e5x5global;
   Float_t         l1e5x5mf0pu;
   Float_t         l1e5x5mf25ns;
   Float_t         l1e5x5mf50ns;
   Float_t         l1eta;
   Float_t         l1gsfp;
   Float_t         l1gsfpmode;
   Float_t         l1p;
   Float_t         l1pdgId;
   Float_t         l1pfIsoChHad04;
   Float_t         l1pfIsoNHad04;
   Float_t         l1pfIsoPhoton04;
   Float_t         l1phi;
   Float_t         l1pt;
   Float_t         l1r9;
   Float_t         l1r9global;
   Float_t         l1sceta;
   Float_t         l1scphi;
   Float_t         l1seedchi2; 
   Float_t         l1seedeerr;
   Float_t         l1seedflag; 
   Float_t         l1sietaieta;
   Float_t         l2bdtID;
   Float_t         l2classification;
   Float_t         l2e5x5global;
   Float_t         l2e5x5mf0pu;
   Float_t         l2e5x5mf25ns;
   Float_t         l2e5x5mf50ns;
   Float_t         l2eta;
   Float_t         l2gsfp;
   Float_t         l2gsfpmode;
   Float_t         l2p;
   Float_t         l2pdgId;
   Float_t         l2pfIsoChHad04;
   Float_t         l2pfIsoNHad04;
   Float_t         l2pfIsoPhoton04;
   Float_t         l2phi;
   Float_t         l2pt;
   Float_t         l2r9;
   Float_t         l2r9global;
   Float_t         l2sceta;
   Float_t         l2scphi;
   Float_t         l2seedchi2; 
   Float_t         l2seedeerr;
   Float_t         l2seedflag; 
   Float_t         l2sietaieta;
   Float_t         nvtx;
   Float_t         zeta;
   Float_t         zmass;
   Float_t         zphi;
   Float_t         zpt;
   UInt_t          run;
   UInt_t          lumi;
   ULong64_t       event;

   // List of branches
   TBranch        *b_l1bdtID;   //!
   TBranch        *b_l1classification;   //!
   TBranch        *b_l1e5x5global;   //!
   TBranch        *b_l1e5x5mf0pu;   //!
   TBranch        *b_l1e5x5mf25ns;   //!
   TBranch        *b_l1e5x5mf50ns;   //!
   TBranch        *b_l1eta;   //!
   TBranch        *b_l1gsfp;   //!
   TBranch        *b_l1gsfpmode;   //!
   TBranch        *b_l1p;   //!
   TBranch        *b_l1pdgId;   //!
   TBranch        *b_l1pfIsoChHad04;   //!
   TBranch        *b_l1pfIsoNHad04;   //!
   TBranch        *b_l1pfIsoPhoton04;   //!
   TBranch        *b_l1phi;   //!
   TBranch        *b_l1pt;   //!
   TBranch        *b_l1r9;   //!
   TBranch        *b_l1r9global;   //!
   TBranch        *b_l1sceta;   //!
   TBranch        *b_l1scphi;   //!
   TBranch        *b_l1seedchi2;   //!
   TBranch        *b_l1seedeerr;   //!
   TBranch        *b_l1seedflag;   //!
   TBranch        *b_l1sietaieta;   //!
   TBranch        *b_l2bdtID;   //!
   TBranch        *b_l2classification;   //!
   TBranch        *b_l2e5x5global;   //!
   TBranch        *b_l2e5x5mf0pu;   //!
   TBranch        *b_l2e5x5mf25ns;   //!
   TBranch        *b_l2e5x5mf50ns;   //!
   TBranch        *b_l2eta;   //!
   TBranch        *b_l2gsfp;   //!
   TBranch        *b_l2gsfpmode;   //!
   TBranch        *b_l2p;   //!
   TBranch        *b_l2pdgId;   //!
   TBranch        *b_l2pfIsoChHad04;   //!
   TBranch        *b_l2pfIsoNHad04;   //!
   TBranch        *b_l2pfIsoPhoton04;   //!
   TBranch        *b_l2phi;   //!
   TBranch        *b_l2pt;   //!
   TBranch        *b_l2r9;   //!
   TBranch        *b_l2r9global;   //!
   TBranch        *b_l2sceta;   //!
   TBranch        *b_l2scphi;   //!
   TBranch        *b_l2seedchi2;   //!
   TBranch        *b_l2seedeerr;   //!
   TBranch        *b_l2seedflag;   //!
   TBranch        *b_l2sietaieta;   //!
   TBranch        *b_nvtx;   //!
   TBranch        *b_zeta;   //!
   TBranch        *b_zmass;   //!
   TBranch        *b_zphi;   //!
   TBranch        *b_zpt;   //!
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_event;   //!

   MultifitValidation(TTree *tree=0);
   virtual ~MultifitValidation();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(const char *outputfilename);
   virtual void     BookHistograms();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

 private:

   enum p4Kind {kGlobal=0, kMf50, kMf25, kMf0};

   TLorentzVector l1p4(int pestimate);
   TLorentzVector l2p4(int pestimate);

   std::map<std::string,TH1D*> h1ds;
   std::map<std::string,TH2D*> h2ds;
};

#endif

#ifdef MultifitValidation_cxx
MultifitValidation::MultifitValidation(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("output_0.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("output_0.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("output_0.root:/zeetree");
      dir->GetObject("probe_tree",tree);

   }
   Init(tree);
}

MultifitValidation::~MultifitValidation()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MultifitValidation::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MultifitValidation::LoadTree(Long64_t entry)
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

void MultifitValidation::Init(TTree *tree)
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

   fChain->SetBranchAddress("l1bdtID", &l1bdtID, &b_l1bdtID);
   fChain->SetBranchAddress("l1classification", &l1classification, &b_l1classification);
   fChain->SetBranchAddress("l1e5x5global", &l1e5x5global, &b_l1e5x5global);
   fChain->SetBranchAddress("l1e5x5mf0pu", &l1e5x5mf0pu, &b_l1e5x5mf0pu);
   fChain->SetBranchAddress("l1e5x5mf25ns", &l1e5x5mf25ns, &b_l1e5x5mf25ns);
   fChain->SetBranchAddress("l1e5x5mf50ns", &l1e5x5mf50ns, &b_l1e5x5mf50ns);
   fChain->SetBranchAddress("l1eta", &l1eta, &b_l1eta);
   fChain->SetBranchAddress("l1gsfp", &l1gsfp, &b_l1gsfp);
   fChain->SetBranchAddress("l1gsfpmode", &l1gsfpmode, &b_l1gsfpmode);
   fChain->SetBranchAddress("l1p", &l1p, &b_l1p);
   fChain->SetBranchAddress("l1pdgId", &l1pdgId, &b_l1pdgId);
   fChain->SetBranchAddress("l1pfIsoChHad04", &l1pfIsoChHad04, &b_l1pfIsoChHad04);
   fChain->SetBranchAddress("l1pfIsoNHad04", &l1pfIsoNHad04, &b_l1pfIsoNHad04);
   fChain->SetBranchAddress("l1pfIsoPhoton04", &l1pfIsoPhoton04, &b_l1pfIsoPhoton04);
   fChain->SetBranchAddress("l1phi", &l1phi, &b_l1phi);
   fChain->SetBranchAddress("l1pt", &l1pt, &b_l1pt);
   fChain->SetBranchAddress("l1r9", &l1r9, &b_l1r9);
   fChain->SetBranchAddress("l1r9global", &l1r9global, &b_l1r9global);
   fChain->SetBranchAddress("l1sceta", &l1sceta, &b_l1sceta);
   fChain->SetBranchAddress("l1scphi", &l1scphi, &b_l1scphi);
   fChain->SetBranchAddress("l1seedchi2", &l1seedchi2, &b_l1seedchi2);
   fChain->SetBranchAddress("l1seedeerr", &l1seedeerr, &b_l1seedeerr);
   fChain->SetBranchAddress("l1seedflag", &l1seedflag, &b_l1seedflag);
   fChain->SetBranchAddress("l1sietaieta", &l1sietaieta, &b_l1sietaieta);
   fChain->SetBranchAddress("l2bdtID", &l2bdtID, &b_l2bdtID);
   fChain->SetBranchAddress("l2classification", &l2classification, &b_l2classification);
   fChain->SetBranchAddress("l2e5x5global", &l2e5x5global, &b_l2e5x5global);
   fChain->SetBranchAddress("l2e5x5mf0pu", &l2e5x5mf0pu, &b_l2e5x5mf0pu);
   fChain->SetBranchAddress("l2e5x5mf25ns", &l2e5x5mf25ns, &b_l2e5x5mf25ns);
   fChain->SetBranchAddress("l2e5x5mf50ns", &l2e5x5mf50ns, &b_l2e5x5mf50ns);
   fChain->SetBranchAddress("l2eta", &l2eta, &b_l2eta);
   fChain->SetBranchAddress("l2gsfp", &l2gsfp, &b_l2gsfp);
   fChain->SetBranchAddress("l2gsfpmode", &l2gsfpmode, &b_l2gsfpmode);
   fChain->SetBranchAddress("l2p", &l2p, &b_l2p);
   fChain->SetBranchAddress("l2pdgId", &l2pdgId, &b_l2pdgId);
   fChain->SetBranchAddress("l2pfIsoChHad04", &l2pfIsoChHad04, &b_l2pfIsoChHad04);
   fChain->SetBranchAddress("l2pfIsoNHad04", &l2pfIsoNHad04, &b_l2pfIsoNHad04);
   fChain->SetBranchAddress("l2pfIsoPhoton04", &l2pfIsoPhoton04, &b_l2pfIsoPhoton04);
   fChain->SetBranchAddress("l2phi", &l2phi, &b_l2phi);
   fChain->SetBranchAddress("l2pt", &l2pt, &b_l2pt);
   fChain->SetBranchAddress("l2r9", &l2r9, &b_l2r9);
   fChain->SetBranchAddress("l2r9global", &l2r9global, &b_l2r9global);
   fChain->SetBranchAddress("l2sceta", &l2sceta, &b_l2sceta);
   fChain->SetBranchAddress("l2scphi", &l2scphi, &b_l2scphi);
   fChain->SetBranchAddress("l2seedchi2", &l2seedchi2, &b_l2seedchi2);
   fChain->SetBranchAddress("l2seedeerr", &l2seedeerr, &b_l2seedeerr);
   fChain->SetBranchAddress("l2seedflag", &l2seedflag, &b_l2seedflag);
   fChain->SetBranchAddress("l2sietaieta", &l2sietaieta, &b_l2sietaieta);
   fChain->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   fChain->SetBranchAddress("zeta", &zeta, &b_zeta);
   fChain->SetBranchAddress("zmass", &zmass, &b_zmass);
   fChain->SetBranchAddress("zphi", &zphi, &b_zphi);
   fChain->SetBranchAddress("zpt", &zpt, &b_zpt);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("event", &event, &b_event);
   Notify();
}

Bool_t MultifitValidation::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MultifitValidation::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MultifitValidation::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MultifitValidation_cxx
