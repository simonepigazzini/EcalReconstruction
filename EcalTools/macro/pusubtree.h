//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Aug  1 10:46:55 2014 by ROOT version 5.34/10
// from TTree ntp1/ntp1
// found on file: root://eoscms//store/cmst3/user/emanuele/ecal/amplireco/ntuples/rereco_photongun_pu25_0.root
//////////////////////////////////////////////////////////

#ifndef pusubtree_h
#define pusubtree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TVector3.h>
#include <math.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class pusubtree {
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
   Int_t           nEBCaloClusters;
   Int_t           nCrystalsEBCaloClusters[1000];   //[nEBCaloClusters]
   Float_t         energyEBCaloClusters[1000];   //[nEBCaloClusters]
   Float_t         etaEBCaloClusters[1000];   //[nEBCaloClusters]
   Float_t         thetaEBCaloClusters[1000];   //[nEBCaloClusters]
   Float_t         phiEBCaloClusters[1000];   //[nEBCaloClusters]
   Int_t           nEECaloClusters;
   Int_t           nCrystalsEECaloClusters[1000];   //[nEECaloClusters]
   Float_t         energyEECaloClusters[1000];   //[nEECaloClusters]
   Float_t         etaEECaloClusters[1000];   //[nEECaloClusters]
   Float_t         thetaEECaloClusters[1000];   //[nEECaloClusters]
   Float_t         phiEECaloClusters[1000];   //[nEECaloClusters]
   Int_t           nEBCaloClusters2;
   Int_t           nCrystalsEBCaloClusters2[1000];   //[nEBCaloClusters2]
   Float_t         energyEBCaloClusters2[1000];   //[nEBCaloClusters2]
   Float_t         etaEBCaloClusters2[1000];   //[nEBCaloClusters2]
   Float_t         thetaEBCaloClusters2[1000];   //[nEBCaloClusters2]
   Float_t         phiEBCaloClusters2[1000];   //[nEBCaloClusters2]
   Int_t           nEECaloClusters2;
   Int_t           nCrystalsEECaloClusters2[1000];   //[nEECaloClusters2]
   Float_t         energyEECaloClusters2[1000];   //[nEECaloClusters2]
   Float_t         etaEECaloClusters2[1000];   //[nEECaloClusters2]
   Float_t         thetaEECaloClusters2[1000];   //[nEECaloClusters2]
   Float_t         phiEECaloClusters2[1000];   //[nEECaloClusters2]
   Int_t           nEBCaloClusters3;
   Int_t           nCrystalsEBCaloClusters3[1000];   //[nEBCaloClusters3]
   Float_t         energyEBCaloClusters3[1000];   //[nEBCaloClusters3]
   Float_t         etaEBCaloClusters3[1000];   //[nEBCaloClusters3]
   Float_t         thetaEBCaloClusters3[1000];   //[nEBCaloClusters3]
   Float_t         phiEBCaloClusters3[1000];   //[nEBCaloClusters3]
   Int_t           nEECaloClusters3;
   Int_t           nCrystalsEECaloClusters3[1000];   //[nEECaloClusters3]
   Float_t         energyEECaloClusters3[1000];   //[nEECaloClusters3]
   Float_t         etaEECaloClusters3[1000];   //[nEECaloClusters3]
   Float_t         thetaEECaloClusters3[1000];   //[nEECaloClusters3]
   Float_t         phiEECaloClusters3[1000];   //[nEECaloClusters3]
   Int_t           nMc;
   Float_t         pMc[1];   //[nMc]
   Float_t         thetaMc[1];   //[nMc]
   Float_t         etaMc[1];   //[nMc]
   Float_t         phiMc[1];   //[nMc]
   Float_t         energyMc[1];   //[nMc]
   Float_t         vxMc[1];   //[nMc]
   Float_t         vyMc[1];   //[nMc]
   Float_t         vzMc[1];   //[nMc]
   Int_t           idMc[1];   //[nMc]
   Int_t           mothMc[1];   //[nMc]
   Int_t           statusMc[1];   //[nMc]

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
   TBranch        *b_nEBCaloClusters;   //!
   TBranch        *b_nCrystalsEBCaloClusters;   //!
   TBranch        *b_energyEBCaloClusters;   //!
   TBranch        *b_etaEBCaloClusters;   //!
   TBranch        *b_thetaEBCaloClusters;   //!
   TBranch        *b_phiEBCaloClusters;   //!
   TBranch        *b_nEECaloClusters;   //!
   TBranch        *b_nCrystalsEECaloClusters;   //!
   TBranch        *b_energyEECaloClusters;   //!
   TBranch        *b_etaEECaloClusters;   //!
   TBranch        *b_thetaEECaloClusters;   //!
   TBranch        *b_phiEECaloClusters;   //!
   TBranch        *b_nEBCaloClusters2;   //!
   TBranch        *b_nCrystalsEBCaloClusters2;   //!
   TBranch        *b_energyEBCaloClusters2;   //!
   TBranch        *b_etaEBCaloClusters2;   //!
   TBranch        *b_thetaEBCaloClusters2;   //!
   TBranch        *b_phiEBCaloClusters2;   //!
   TBranch        *b_nEECaloClusters2;   //!
   TBranch        *b_nCrystalsEECaloClusters2;   //!
   TBranch        *b_energyEECaloClusters2;   //!
   TBranch        *b_etaEECaloClusters2;   //!
   TBranch        *b_thetaEECaloClusters2;   //!
   TBranch        *b_phiEECaloClusters2;   //!
   TBranch        *b_nEBCaloClusters3;   //!
   TBranch        *b_nCrystalsEBCaloClusters3;   //!
   TBranch        *b_energyEBCaloClusters3;   //!
   TBranch        *b_etaEBCaloClusters3;   //!
   TBranch        *b_thetaEBCaloClusters3;   //!
   TBranch        *b_phiEBCaloClusters3;   //!
   TBranch        *b_nEECaloClusters3;   //!
   TBranch        *b_nCrystalsEECaloClusters3;   //!
   TBranch        *b_energyEECaloClusters3;   //!
   TBranch        *b_etaEECaloClusters3;   //!
   TBranch        *b_thetaEECaloClusters3;   //!
   TBranch        *b_phiEECaloClusters3;   //!
   TBranch        *b_nMc;   //!
   TBranch        *b_pMc;   //!
   TBranch        *b_thetaMc;   //!
   TBranch        *b_etaMc;   //!
   TBranch        *b_phiMc;   //!
   TBranch        *b_energyMc;   //!
   TBranch        *b_vxMc;   //!
   TBranch        *b_vyMc;   //!
   TBranch        *b_vzMc;   //!
   TBranch        *b_idMc;   //!
   TBranch        *b_mothMc;   //!
   TBranch        *b_statusMc;   //!

   pusubtree(TTree *tree=0);
   virtual ~pusubtree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(const char *outputfilename);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   virtual bool     mcmatch(TVector3 scdir, TVector3 truep);
};

#endif

#ifdef pusubtree_cxx
pusubtree::pusubtree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://eoscms//store/cmst3/user/emanuele/ecal/amplireco/ntuples/rereco_photongun_pu25_0.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("root://eoscms//store/cmst3/user/emanuele/ecal/amplireco/ntuples/rereco_photongun_pu25_0.root");
      }
      f->GetObject("ntp1",tree);

   }
   Init(tree);
}

pusubtree::~pusubtree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t pusubtree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t pusubtree::LoadTree(Long64_t entry)
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

void pusubtree::Init(TTree *tree)
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
   fChain->SetBranchAddress("nEBCaloClusters", &nEBCaloClusters, &b_nEBCaloClusters);
   fChain->SetBranchAddress("nCrystalsEBCaloClusters", nCrystalsEBCaloClusters, &b_nCrystalsEBCaloClusters);
   fChain->SetBranchAddress("energyEBCaloClusters", energyEBCaloClusters, &b_energyEBCaloClusters);
   fChain->SetBranchAddress("etaEBCaloClusters", etaEBCaloClusters, &b_etaEBCaloClusters);
   fChain->SetBranchAddress("thetaEBCaloClusters", thetaEBCaloClusters, &b_thetaEBCaloClusters);
   fChain->SetBranchAddress("phiEBCaloClusters", phiEBCaloClusters, &b_phiEBCaloClusters);
   fChain->SetBranchAddress("nEECaloClusters", &nEECaloClusters, &b_nEECaloClusters);
   fChain->SetBranchAddress("nCrystalsEECaloClusters", nCrystalsEECaloClusters, &b_nCrystalsEECaloClusters);
   fChain->SetBranchAddress("energyEECaloClusters", energyEECaloClusters, &b_energyEECaloClusters);
   fChain->SetBranchAddress("etaEECaloClusters", etaEECaloClusters, &b_etaEECaloClusters);
   fChain->SetBranchAddress("thetaEECaloClusters", thetaEECaloClusters, &b_thetaEECaloClusters);
   fChain->SetBranchAddress("phiEECaloClusters", phiEECaloClusters, &b_phiEECaloClusters);
   fChain->SetBranchAddress("nEBCaloClusters2", &nEBCaloClusters2, &b_nEBCaloClusters2);
   fChain->SetBranchAddress("nCrystalsEBCaloClusters2", nCrystalsEBCaloClusters2, &b_nCrystalsEBCaloClusters2);
   fChain->SetBranchAddress("energyEBCaloClusters2", energyEBCaloClusters2, &b_energyEBCaloClusters2);
   fChain->SetBranchAddress("etaEBCaloClusters2", etaEBCaloClusters2, &b_etaEBCaloClusters2);
   fChain->SetBranchAddress("thetaEBCaloClusters2", thetaEBCaloClusters2, &b_thetaEBCaloClusters2);
   fChain->SetBranchAddress("phiEBCaloClusters2", phiEBCaloClusters2, &b_phiEBCaloClusters2);
   fChain->SetBranchAddress("nEECaloClusters2", &nEECaloClusters2, &b_nEECaloClusters2);
   fChain->SetBranchAddress("nCrystalsEECaloClusters2", nCrystalsEECaloClusters2, &b_nCrystalsEECaloClusters2);
   fChain->SetBranchAddress("energyEECaloClusters2", energyEECaloClusters2, &b_energyEECaloClusters2);
   fChain->SetBranchAddress("etaEECaloClusters2", etaEECaloClusters2, &b_etaEECaloClusters2);
   fChain->SetBranchAddress("thetaEECaloClusters2", thetaEECaloClusters2, &b_thetaEECaloClusters2);
   fChain->SetBranchAddress("phiEECaloClusters2", phiEECaloClusters2, &b_phiEECaloClusters2);
   fChain->SetBranchAddress("nEBCaloClusters3", &nEBCaloClusters3, &b_nEBCaloClusters3);
   fChain->SetBranchAddress("nCrystalsEBCaloClusters3", nCrystalsEBCaloClusters3, &b_nCrystalsEBCaloClusters3);
   fChain->SetBranchAddress("energyEBCaloClusters3", energyEBCaloClusters3, &b_energyEBCaloClusters3);
   fChain->SetBranchAddress("etaEBCaloClusters3", etaEBCaloClusters3, &b_etaEBCaloClusters3);
   fChain->SetBranchAddress("thetaEBCaloClusters3", thetaEBCaloClusters3, &b_thetaEBCaloClusters3);
   fChain->SetBranchAddress("phiEBCaloClusters3", phiEBCaloClusters3, &b_phiEBCaloClusters3);
   fChain->SetBranchAddress("nEECaloClusters3", &nEECaloClusters3, &b_nEECaloClusters3);
   fChain->SetBranchAddress("nCrystalsEECaloClusters3", nCrystalsEECaloClusters3, &b_nCrystalsEECaloClusters3);
   fChain->SetBranchAddress("energyEECaloClusters3", energyEECaloClusters3, &b_energyEECaloClusters3);
   fChain->SetBranchAddress("etaEECaloClusters3", etaEECaloClusters3, &b_etaEECaloClusters3);
   fChain->SetBranchAddress("thetaEECaloClusters3", thetaEECaloClusters3, &b_thetaEECaloClusters3);
   fChain->SetBranchAddress("phiEECaloClusters3", phiEECaloClusters3, &b_phiEECaloClusters3);
   fChain->SetBranchAddress("nMc", &nMc, &b_nMc);
   fChain->SetBranchAddress("pMc", pMc, &b_pMc);
   fChain->SetBranchAddress("thetaMc", thetaMc, &b_thetaMc);
   fChain->SetBranchAddress("etaMc", etaMc, &b_etaMc);
   fChain->SetBranchAddress("phiMc", phiMc, &b_phiMc);
   fChain->SetBranchAddress("energyMc", energyMc, &b_energyMc);
   fChain->SetBranchAddress("vxMc", vxMc, &b_vxMc);
   fChain->SetBranchAddress("vyMc", vyMc, &b_vyMc);
   fChain->SetBranchAddress("vzMc", vzMc, &b_vzMc);
   fChain->SetBranchAddress("idMc", idMc, &b_idMc);
   fChain->SetBranchAddress("mothMc", mothMc, &b_mothMc);
   fChain->SetBranchAddress("statusMc", statusMc, &b_statusMc);
   Notify();
}

Bool_t pusubtree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void pusubtree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t pusubtree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
bool pusubtree::mcmatch(TVector3 scdir, TVector3 truep) {
  /* for samples with material and electrons. 
  // for the EB, back-propagate with the magnetic field, since it is simple.
  // for the EE, only match in deltaEta
  
  float abseta = fabs(scdir.Eta());
  float phicurv = atan2(1.24*3.8*charge,4.*scdir.Pt());
  
  float deltaeta = scdir.Eta() - truep.Eta();  
  float deltaphi = scdir.DeltaPhi(truep) - phicurv;
  if(abseta<1.479) {
    return sqrt(deltaeta*deltaeta + deltaphi*deltaphi) < 0.03;
  } else {
    //return fabs(deltaeta) < 0.03;
    return sqrt(deltaeta*deltaeta + deltaphi*deltaphi) < 0.03;
  }
  */
  
  // for photon gun samples with no material in front of ECAL
  float deltaeta = scdir.Eta() - truep.Eta();  
  float deltaphi = scdir.Phi() - truep.Phi();  
  return sqrt(deltaeta*deltaeta + deltaphi*deltaphi) < 0.03;

}
#endif // #ifdef pusubtree_cxx
