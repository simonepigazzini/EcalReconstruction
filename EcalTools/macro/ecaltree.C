#define ecaltree_cxx
#include "ecaltree.h"
#include <TH2D.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>

#define ANALYSIS 1


void ecaltree::Loop(const char* outputfilename)
{
//   In a ROOT session, you can do:
//      Root > .L ecaltree.C
//      Root > ecaltree t
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

   std::cout << "Output file is " << outputfilename << std::endl;

#if ANALYSIS == 1

   TFile *fileo = TFile::Open(outputfilename,"recreate");

   // correlation plots
  TH2D *timesCorr_EB = new TH2D("timesCorr_EB","",1000,-10,10,1000,-10,10);
  TH2D *timesCorr_EE = new TH2D("timesCorr_EE","",1000,-25,25,1000,-25,25);

  TH2D *time1EnergyCorr_EB = new TH2D("time1EnergyCorr_EB","",1000,-10,10,1000,0,200);
  TH2D *time1EnergyCorr_EE = new TH2D("time1EnergyCorr_EE","",1000,-50,50,1000,0,200);
  TH2D *time2EnergyCorr_EB = new TH2D("time2EnergyCorr_EB","",1000,-10,10,1000,0,200);
  TH2D *time2EnergyCorr_EE = new TH2D("time2EnergyCorr_EE","",1000,-50,50,1000,0,200);

  // time ratio vs eta, ET, chi2
  TProfile *timeRatioVsEta = new TProfile("timeRatioVsEta","",100,0,3.0); 
  TProfile *timeRatioVsE_EB = new TProfile("timeRatioVsE_EB","",20,0.0,200.); 
  TProfile *timeRatioVsE_EE = (TProfile*)timeRatioVsE_EB->Clone("timeRatioVsE_EE");
  TProfile *timeRatioVsChi2_EB = new TProfile("timeRatioVsChi2_EB","",50,0.0,30.); 
  TProfile *timeRatioVsChi2_EE = (TProfile*)timeRatioVsChi2_EB->Clone("timeRatioVsChi2_EE");

  // time with weights for different slices of times with ratio
  double t_window=5.0; // ns
  TH1F *time_0_EB = new TH1F("time_0_EB","",1000,-75,75);
  TH1F *time_p1_EB = new TH1F("time_p1_EB","",1000,-50,100);
  TH1F *time_p2_EB = new TH1F("time_p2_EB","",1000,-25,125);
  TH1F *time_m1_EB = new TH1F("time_m1_EB","",1000,-100,50);
  TH1F *time_m2_EB = new TH1F("time_m2_EB","",1000,-125,25);
  TH1F *time_0_EE = new TH1F("time_0_EE","",1000,-75,75);
  TH1F *time_p1_EE = new TH1F("time_p1_EE","",1000,-50,100);
  TH1F *time_p2_EE = new TH1F("time_p2_EE","",1000,-25,125);
  TH1F *time_m1_EE = new TH1F("time_m1_EE","",1000,-100,50);
  TH1F *time_m2_EE = new TH1F("time_m2_EE","",1000,-125,25);

  std::vector<TH1F*> time1Dplots;
  time1Dplots.push_back(time_0_EB);
  time1Dplots.push_back(time_p1_EB);
  time1Dplots.push_back(time_p2_EB);
  time1Dplots.push_back(time_m1_EB);
  time1Dplots.push_back(time_m2_EB);
  time1Dplots.push_back(time_0_EE);
  time1Dplots.push_back(time_p1_EE);
  time1Dplots.push_back(time_p2_EE);
  time1Dplots.push_back(time_m1_EE);
  time1Dplots.push_back(time_m2_EE);

   Long64_t nentries = fChain->GetEntries();

   std::cout << "Total entries = " << nentries << std::endl;
   
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if(jentry % 1000 == 0) std::cout << "Processing entry " << jentry << std::endl;

      // barrel
      for(int h=0;h<std::min(nEBRecHits,10000);++h) {
        double r=timeEBRecHits2[h]-timeEBRecHits[h];
          if(energyEBRecHits[h]>10) {
            timesCorr_EB->Fill(timeEBRecHits[h],timeEBRecHits2[h]);
            timeRatioVsEta->Fill(etaEBRecHits[h],r);
            timeRatioVsChi2_EB->Fill(chi2EBRecHits[h],r);
            if(abs(timeEBRecHits[h])<t_window) time_0_EB->Fill(timeEBRecHits2[h]);
            if(abs(timeEBRecHits[h]-25.)<t_window) time_p1_EB->Fill(timeEBRecHits2[h]);
            if(abs(timeEBRecHits[h]-50.)<t_window) time_p2_EB->Fill(timeEBRecHits2[h]);
            if(abs(timeEBRecHits[h]+25.)<t_window) time_m1_EB->Fill(timeEBRecHits2[h]);
            if(abs(timeEBRecHits[h]+50.)<t_window) time_m2_EB->Fill(timeEBRecHits2[h]);
          }
          time1EnergyCorr_EB->Fill(timeEBRecHits[h],energyEBRecHits[h]);
          time2EnergyCorr_EB->Fill(timeEBRecHits2[h],energyEBRecHits[h]);
          timeRatioVsE_EB->Fill(energyEBRecHits[h],r);
      }
      // endcap
      for(int h=0;h<std::min(nEERecHits,10000);++h) {
        double r=timeEERecHits2[h]-timeEERecHits[h];
        if(energyEERecHits[h]>10) {
          timesCorr_EE->Fill(timeEERecHits[h],timeEERecHits2[h]);
          timeRatioVsEta->Fill(etaEERecHits[h],r);
          timeRatioVsChi2_EE->Fill(chi2EERecHits[h],r);
          if(abs(timeEERecHits[h])<t_window) time_0_EE->Fill(timeEERecHits2[h]);
          if(abs(timeEERecHits[h]-25.)<t_window) time_p1_EE->Fill(timeEERecHits2[h]);
          if(abs(timeEERecHits[h]-50.)<t_window) time_p2_EE->Fill(timeEERecHits2[h]);
          if(abs(timeEERecHits[h]+25.)<t_window) time_m1_EE->Fill(timeEERecHits2[h]);
          if(abs(timeEERecHits[h]+50.)<t_window) time_m2_EE->Fill(timeEERecHits2[h]);
        }
        time1EnergyCorr_EE->Fill(timeEERecHits[h],energyEERecHits[h]);
        time2EnergyCorr_EE->Fill(timeEERecHits2[h],energyEERecHits[h]);
        timeRatioVsE_EE->Fill(energyEERecHits[h],r);
      }

   } // event loop

   timeRatioVsEta->Write();

   timesCorr_EB->Write();
   time1EnergyCorr_EB->Write();
   time2EnergyCorr_EB->Write();
   timeRatioVsChi2_EB->Write();
   timeRatioVsE_EB->Write();

   timesCorr_EE->Write();
   time1EnergyCorr_EE->Write();
   time2EnergyCorr_EE->Write();
   timeRatioVsChi2_EE->Write();
   timeRatioVsE_EE->Write();
   
   for(int i=0; i<(int) time1Dplots.size(); ++i)  time1Dplots[i]->Write();

   fileo->Close();

#endif

#if ANALYSIS == 2

   ofstream txtfile;
   txtfile.open(outputfilename,std::ios::trunc);

   Long64_t nentries = fChain->GetEntries();

   std::cout << "Total entries = " << nentries << std::endl;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     if(jentry%1000==0) std::cout << "Processing entry " << jentry << "..." << std::endl;
     for(int h=0; h<std::min(nEERecHits,10000); ++h) {
       if(energyEERecHits[h]>5.0)
         txtfile << eventNumber << "\t"
                 << ixEERecHits[h] << "\t"
                 << iyEERecHits[h] << "\t"
                 << nTruePU[12] << "\t" // the bx = 0
                 << energyEERecHits[h] << "\t"
                 << timeEERecHits[h] << "\t"
                 << chi2EERecHits[h] << "\t"
                 << ootenergyEERecHits[h] << std::endl;
     }
     // if (Cut(ientry) < 0) continue;
   }
   txtfile.close();

#endif


}
