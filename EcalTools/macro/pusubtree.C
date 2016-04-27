#define pusubtree_cxx
#include "pusubtree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void pusubtree::Loop(const char *outputfilename)
{
//   In a ROOT session, you can do:
//      Root > .L pusubtree.C
//      Root > pusubtree t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   TFile *fileo = TFile::Open(outputfilename,"recreate");

   TH1F *res = new TH1F("res","",1000,0.50,1.50);

   for(int clustertype=0; clustertype<3; ++clustertype) {
     TH1F* inclusive = (TH1F*)res->Clone(Form("cluster%d_res_inclusive",clustertype));
     TH1F* resolutions[2][4];
     float ebins[5] = {1,5,10,30,100};
     float etabins[3] = {0.,1.479,2.5};
     
     for(int e=0;e<2;++e) {
       for(int p=0;p<4;++p) {
         char namer[50];
         sprintf(namer,"cluster%d_res_eta%d_e%d",clustertype,e,p);
         TH1F* ires = (TH1F*)res->Clone(namer);
         resolutions[e][p] = ires;
       }
     }

     Long64_t nentries = fChain->GetEntries();
     
     std::cout << "Total entries = " << nentries << std::endl;
     
     Long64_t nbytes = 0, nb = 0;
     for (Long64_t jentry=0; jentry<nentries;jentry++) {
       Long64_t ientry = LoadTree(jentry);
       if (ientry < 0) break;
       nb = fChain->GetEntry(jentry);   nbytes += nb;
       // if (Cut(ientry) < 0) continue;
       
       if(jentry % 1000 == 0) std::cout << "Loop " << clustertype <<  ": processing entry " << jentry << std::endl;
       
       if(idMc[0] != 22) continue;
       TVector3 pGamma;
       pGamma.SetMagThetaPhi(pMc[0],thetaMc[0],phiMc[0]);
       bool matched = false;
      
       if(fabs(etaMc[0])<1.479) {
         int nClus=0;
         if(clustertype==0) nClus = nEBCaloClusters; else if(clustertype==1) nClus = nEBCaloClusters2 ; else nClus=nEBCaloClusters3;
         nClus = std::min(nClus,100); // the true one is the high energy one (and clustrs are sorted)
         for(int isc=0; isc<nClus && matched==false; ++isc) {
           TVector3 scdir;
           if(clustertype==0) scdir.SetMagThetaPhi(energyEBCaloClusters[isc],  thetaEBCaloClusters[isc],  phiEBCaloClusters[isc]);
           if(clustertype==1) scdir.SetMagThetaPhi(energyEBCaloClusters2[isc], thetaEBCaloClusters2[isc], phiEBCaloClusters2[isc]);
           if(clustertype==2) scdir.SetMagThetaPhi(energyEBCaloClusters3[isc], thetaEBCaloClusters3[isc], phiEBCaloClusters3[isc]);

           if(mcmatch(scdir,pGamma)) {
             matched=true;
             int ebin=0;
             for(int iene=0; iene<4; ++iene) {
               if(pGamma.Pt() >= ebins[iene] && pGamma.Pt() < ebins[iene+1]) {
                 ebin=iene; break;
               }
             }
             float energy=0;
             if(clustertype==0) energy = energyEBCaloClusters[isc]; else if(clustertype==1) energy = energyEBCaloClusters2[isc]; else energy = energyEBCaloClusters3[isc];
             resolutions[0][ebin]->Fill(energy/pMc[0]);
             inclusive->Fill(energy/pMc[0]);
           }
         }
       } else {
         int nClus=0;
         if(clustertype==0) nClus = nEECaloClusters; else if(clustertype==1) nClus = nEECaloClusters2 ; else nClus=nEECaloClusters3;
         nClus = std::min(nClus,100); // the true one is the high energy one (and clustrs are sorted)
         for(int isc=0; isc<nClus && matched==false; ++isc) {
          TVector3 scdir;
           if(clustertype==0) scdir.SetMagThetaPhi(energyEECaloClusters[isc],  thetaEECaloClusters[isc],  phiEECaloClusters[isc]);
           if(clustertype==1) scdir.SetMagThetaPhi(energyEECaloClusters2[isc], thetaEECaloClusters2[isc], phiEECaloClusters2[isc]);
           if(clustertype==2) scdir.SetMagThetaPhi(energyEECaloClusters3[isc], thetaEECaloClusters3[isc], phiEECaloClusters3[isc]);

          if(mcmatch(scdir,pGamma)) {
            matched=true;
            int ebin=0;
            for(int iene=0; iene<4; ++iene) {
              if(pGamma.Pt() >= ebins[iene] && pGamma.Pt() < ebins[iene+1]) {
                ebin=iene; break;
              }
            }
            float energy=0;
            if(clustertype==0) energy = energyEECaloClusters[isc]; else if(clustertype==1) energy = energyEECaloClusters2[isc]; else energy = energyEECaloClusters3[isc];
            resolutions[1][ebin]->Fill(energy/pMc[0]);
            inclusive->Fill(energy/pMc[0]);
          }
         }
       }
       
     } // loop over the events
     
     for(int i=0; i<2; ++i) for(int j=0; j<4; ++j) resolutions[i][j]->Write();
     inclusive->Write();

   } // loop over the cluster types

   fileo->Close();
   
}
