#define MultifitValidation_cxx
#include "MultifitValidation.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void MultifitValidation::Loop(const char *outputfilename)
{
//   In a ROOT session, you can do:
//      Root > .L MultifitValidation.C
//      Root > MultifitValidation t
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

   Long64_t nentries = fChain->GetEntries();

   std::cout << "Total entries = " << nentries << std::endl;
   
   TFile *tfileout = TFile::Open(outputfilename,"recreate");
   TTree *tree = new TTree("probe_tree","selected Z skim");
   
   float mass_mf0,mass_mf50,mass_mf25,mass_weights;

   tree->Branch("run",&run);
   tree->Branch("nvtx",&nvtx);
   tree->Branch("l1pt",&l1pt);
   tree->Branch("l1eta",&l1eta);
   tree->Branch("l2pt",&l2pt);
   tree->Branch("l2eta",&l2eta);
   tree->Branch("l1flag",&l1seedflag);
   tree->Branch("l2flag",&l2seedflag);
   tree->Branch("l1energy_mf0",&l1e5x5mf0pu);
   tree->Branch("l1energy_mf50",&l1e5x5mf50ns);
   tree->Branch("l1energy_mf25",&l1e5x5mf25ns);
   tree->Branch("l1energy_weights",&l1e5x5global);
   tree->Branch("l2energy_mf0",&l1e5x5mf0pu);
   tree->Branch("l2energy_mf50",&l1e5x5mf50ns);
   tree->Branch("l2energy_mf25",&l1e5x5mf25ns);
   tree->Branch("l2energy_weights",&l1e5x5global);
   tree->Branch("mass_mf0",&mass_mf0);
   tree->Branch("mass_mf50",&mass_mf50);
   tree->Branch("mass_mf25",&mass_mf25);
   tree->Branch("mass_weights",&mass_weights);

   BookHistograms();

   float r9cut = 0.95;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if(jentry % 100000 == 0) std::cout << "Processing entry " << jentry << std::endl;

      std::string detkey;
      std::string l1ptkey, l2ptkey;

      if(l1pt<25) l1ptkey="5to25"; else l1ptkey="25to100";
      if(l2pt<25) l2ptkey="5to25"; else l2ptkey="25to100";

      // single electron plots
      if(fabs(l1eta)<1.479) h1ds[Form("h_r9_bb",detkey.c_str())]->Fill(l1r9); else h1ds[Form("h_r9_ee",detkey.c_str())]->Fill(l1r9); 
      if(fabs(l2eta)<1.479) h1ds[Form("h_r9_bb",detkey.c_str())]->Fill(l2r9); else h1ds[Form("h_r9_ee",detkey.c_str())]->Fill(l2r9); 

      if(l1r9>r9cut) {        
        detkey = (fabs(l1eta)<1.479) ? "bb" : "ee";
        h1ds[Form("h_see_%s",detkey.c_str())]->Fill(l1sietaieta); 
        h1ds[Form("h_eop_global_%s",detkey.c_str())]->Fill(l1e5x5global/l1gsfp); 
        h1ds[Form("h_eop_mf50_%s",detkey.c_str())]->Fill(l1e5x5mf50ns/l1gsfp); 
        h1ds[Form("h_eop_mf25_%s",detkey.c_str())]->Fill(l1e5x5mf25ns/l1gsfp); 
        h1ds[Form("h_eop_mf0_%s",detkey.c_str())]->Fill(l1e5x5mf0pu/l1gsfp);   

        h1ds[Form("h_eop_global_%s_%s",l1ptkey.c_str(),detkey.c_str())]->Fill(l1e5x5global/l1gsfp); 
        h1ds[Form("h_eop_mf50_%s_%s",l1ptkey.c_str(),detkey.c_str())]->Fill(l1e5x5mf50ns/l1gsfp); 
        h1ds[Form("h_eop_mf25_%s_%s",l1ptkey.c_str(),detkey.c_str())]->Fill(l1e5x5mf25ns/l1gsfp); 
        h1ds[Form("h_eop_mf0_%s_%s",l1ptkey.c_str(),detkey.c_str())]->Fill(l1e5x5mf0pu/l1gsfp);   
        
        h2ds[Form("h2_glob_mf50_%s",detkey.c_str())]->Fill(l1e5x5global,l1e5x5mf50ns);
        h2ds[Form("h2_glob_mf25_%s",detkey.c_str())]->Fill(l1e5x5global,l1e5x5mf25ns);
        h2ds[Form("h2_glob_mf0_%s",detkey.c_str())]->Fill(l1e5x5global,l1e5x5mf0pu);
      }
      if(l2r9>r9cut) {
        detkey = (fabs(l2eta)<1.479) ? "bb" : "ee";
        h1ds[Form("h_see_%s",detkey.c_str())]->Fill(l2sietaieta); 
        h1ds[Form("h_eop_global_%s",detkey.c_str())]->Fill(l2e5x5global/l2gsfp); 
        h1ds[Form("h_eop_mf50_%s",detkey.c_str())]->Fill(l2e5x5mf50ns/l2gsfp); 
        h1ds[Form("h_eop_mf25_%s",detkey.c_str())]->Fill(l2e5x5mf25ns/l2gsfp); 
        h1ds[Form("h_eop_mf0_%s",detkey.c_str())]->Fill(l2e5x5mf0pu/l2gsfp);   

        h1ds[Form("h_eop_global_%s_%s",l2ptkey.c_str(),detkey.c_str())]->Fill(l2e5x5global/l2gsfp); 
        h1ds[Form("h_eop_mf50_%s_%s",l2ptkey.c_str(),detkey.c_str())]->Fill(l2e5x5mf50ns/l2gsfp); 
        h1ds[Form("h_eop_mf25_%s_%s",l2ptkey.c_str(),detkey.c_str())]->Fill(l2e5x5mf25ns/l2gsfp); 
        h1ds[Form("h_eop_mf0_%s_%s",l2ptkey.c_str(),detkey.c_str())]->Fill(l2e5x5mf0pu/l2gsfp);   

        h2ds[Form("h2_glob_mf50_%s",detkey.c_str())]->Fill(l2e5x5global,l2e5x5mf50ns);
        h2ds[Form("h2_glob_mf25_%s",detkey.c_str())]->Fill(l2e5x5global,l2e5x5mf25ns);
        h2ds[Form("h2_glob_mf0_%s",detkey.c_str())]->Fill(l2e5x5global,l2e5x5mf0pu);
      }

      // high r9 pairs
      //      if (run<198022) continue;
      if(l1r9<r9cut || l2r9<r9cut) continue;
      //if(l1classification != 0 || l2classification != 0) continue;

      if(fabs(l1eta)<1.479 && fabs(l2eta)<1.479) detkey = "bb";
      else if(fabs(l1eta)>1.479 && fabs(l2eta)>1.479) detkey = "ee";
      else detkey = "be";

      mass_weights = (l1p4(kGlobal)+l2p4(kGlobal)).M();
      mass_mf50 = (l1p4(kMf50)+l2p4(kMf50)).M();
      mass_mf25 = (l1p4(kMf25)+l2p4(kMf25)).M();
      mass_mf0 = (l1p4(kMf0)+l2p4(kMf0)).M();
      
      h1ds[Form("h_mass_global_%s",detkey.c_str())]->Fill(mass_weights);
      h1ds[Form("h_mass_mf50_%s",detkey.c_str())]->Fill(mass_mf50);
      h1ds[Form("h_mass_mf25_%s",detkey.c_str())]->Fill(mass_mf25);
      h1ds[Form("h_mass_mf0_%s",detkey.c_str())]->Fill(mass_mf0);
      
      std::string atleastoneptkey;
      if(l1ptkey.compare("5to25")==0 || l2ptkey.compare("5to25")==0) atleastoneptkey = "5to25";
      else atleastoneptkey = "25to100";
      
      h1ds[Form("h_mass_global_%s_%s",atleastoneptkey.c_str(),detkey.c_str())]->Fill(mass_weights);
      h1ds[Form("h_mass_mf50_%s_%s",atleastoneptkey.c_str(),detkey.c_str())]->Fill(mass_mf50);
      h1ds[Form("h_mass_mf25_%s_%s",atleastoneptkey.c_str(),detkey.c_str())]->Fill(mass_mf25);
      h1ds[Form("h_mass_mf0_%s_%s",atleastoneptkey.c_str(),detkey.c_str())]->Fill(mass_mf0);

      tree->Fill();

   }
   
   tfileout->cd();
   std::map<std::string,TH1D*>::iterator it;
   for(it=h1ds.begin(); it!=h1ds.end(); ++it) {
     it->second->Write();
   }
   std::map<std::string,TH2D*>::iterator it2;
   for(it2=h2ds.begin(); it2!=h2ds.end(); ++it2) {
     it2->second->Write();
   }
   tree->Write();
   tfileout->Close();

}

void MultifitValidation::BookHistograms() {

  TH1D* h_mass = new TH1D("h_mass","",100,40.,120.);
  TH1D* h_r9 = new TH1D("h_r9","",100,0.,1.);
  TH1D* h_see = new TH1D("h_see","",100,0.,.05);
  TH1D* h_eop = new TH1D("h_eop","",100,0.,5.0);

  std::string ptbins[2] = {"5to25","25to100"};

  std::vector<TH1D*> base_h1d;

  std::string enetype[4] = {"global","mf50","mf25","mf0"};
  for(int iene=0; iene<4; iene++) {
    TH1D* mass_iene_h1d = (TH1D*)h_mass->Clone(Form("%s_%s",h_mass->GetName(),enetype[iene].c_str()));
    base_h1d.push_back(mass_iene_h1d);
    TH1D* eop_iene_h1d = (TH1D*)h_eop->Clone(Form("%s_%s",h_eop->GetName(),enetype[iene].c_str()));
    base_h1d.push_back(eop_iene_h1d);
    for(int ipt=0;ipt<2;ipt++) {
      TH1D* mass_iene_ipt_h1d = (TH1D*)h_mass->Clone(Form("%s_%s_%s",h_mass->GetName(),enetype[iene].c_str(),ptbins[ipt].c_str()));
      base_h1d.push_back(mass_iene_ipt_h1d);
      TH1D* eop_iene_ipt_h1d = (TH1D*)h_eop->Clone(Form("%s_%s_%s",h_eop->GetName(),enetype[iene].c_str(),ptbins[ipt].c_str()));
      base_h1d.push_back(eop_iene_ipt_h1d);
    }
  }
  base_h1d.push_back(h_r9);
  base_h1d.push_back(h_see);
  base_h1d.push_back(h_eop);

  TH2D* h2_glob_mf50 = new TH2D("h2_glob_mf50","",495,5,500,495,5,500);
  TH2D* h2_glob_mf25 = new TH2D("h2_glob_mf25","",495,5,500,495,5,500);
  TH2D* h2_glob_mf0 = new TH2D("h2_glob_mf0","",495,5,500,495,5,500);

  std::vector<TH2D*> base_h2d;
  base_h2d.push_back(h2_glob_mf50);
  base_h2d.push_back(h2_glob_mf25);
  base_h2d.push_back(h2_glob_mf0);

  std::string suffix[3] = {"bb","be","ee"};

  for(int idet=0;idet<3;++idet) {
    for(int i=0; i<(int)base_h1d.size(); ++i) {
      std::string h1d_name = Form("%s_%s",base_h1d[i]->GetName(),suffix[idet].c_str());
      TH1D* final_h1d = (TH1D*)base_h1d[i]->Clone(h1d_name.c_str());
      h1ds[h1d_name] = final_h1d;
      //      std::cout << "created 1D histogram " << h1d_name << std::endl;
    }
    for(int i=0; i<(int)base_h2d.size(); ++i) {
      std::string h2d_name = Form("%s_%s",base_h2d[i]->GetName(),suffix[idet].c_str());
      TH2D* final_h2d = (TH2D*)base_h2d[i]->Clone(h2d_name.c_str());      
      h2ds[h2d_name] = final_h2d;
      //a      std::cout << "created 2D histogram " << h2d_name << std::endl;
    }
  }
  std::cout << "Histograms created." << std::endl;
}

TLorentzVector MultifitValidation::l1p4(int pestimate) {
  float energy = 0;
  switch (pestimate) {
    case kGlobal:
      energy = l1e5x5global;
      break;
    case kMf50:
      energy = l1e5x5mf50ns;
      break;
    case kMf25:
      energy = l1e5x5mf25ns; 
      break;
    case kMf0:
      energy = l1e5x5mf0pu;
      break;
    default:
      energy = 0.;
      break;
    }

  float theta = 2.*atan(std::exp(-1.*l1eta));
  float pt = energy*fabs(sin(theta));

  TLorentzVector l1p4;
  l1p4.SetPtEtaPhiM(pt,l1eta,l1phi,0.51e-3);
  return l1p4;
}

TLorentzVector MultifitValidation::l2p4(int pestimate) {
  float energy = 0;
  switch (pestimate) {
    case kGlobal:
      energy = l2e5x5global;
      break;
    case kMf50:
      energy = l2e5x5mf50ns;
      break;
    case kMf25:
      energy = l2e5x5mf25ns; 
      break;
    case kMf0:
      energy = l2e5x5mf0pu;
      break;
    default:
      energy = 0.;
      break;
    }

  float theta = 2.*atan(std::exp(-1.*l2eta));
  float pt = energy*fabs(sin(theta));

  TLorentzVector l2p4;
  l2p4.SetPtEtaPhiM(pt,l2eta,l2phi,0.51e-3);
  return l2p4;
}
