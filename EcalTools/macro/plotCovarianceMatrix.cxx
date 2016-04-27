#include <vector>
#include <string>
#include <fstream>

#include "TFile.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include "/Users/emanuele/Scripts/RooHZZStyle.C"

void plotCovarianceMatrix(int gain,int run, std::string option="recreate") {

  TFile *outputcorr = TFile::Open("average_correlation.root",option.c_str());

  std::vector<std::string> covs;
  for(int ieb=0; ieb<18; ++ieb) { 
    if(ieb<9) {
      covs.push_back(Form("covariance_files/covariances_run%d_EBp0%d_Gain%d.root",run,ieb+1,gain));
      covs.push_back(Form("covariance_files/covariances_run%d_EBm0%d_Gain%d.root",run,ieb+1,gain));
    } else {
      covs.push_back(Form("covariance_files/covariances_run%d_EBp%d_Gain%d.root",run,ieb+1,gain));
      covs.push_back(Form("covariance_files/covariances_run%d_EBm%d_Gain%d.root",run,ieb+1,gain));
    }
  }
  for(int iee=0; iee<9; ++iee) { 
    covs.push_back(Form("covariance_files/covariances_run%d_EEp0%d_Gain%d.root",run,iee+1,gain));
    covs.push_back(Form("covariance_files/covariances_run%d_EEm0%d_Gain%d.root",run,iee+1,gain));
  }

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();
  mystyle->SetPalette(1);

  float EBCorrNoiseMatrixG12[10] = {
    1.00000, 0.71073, 0.55721, 0.46089, 0.40449,
    0.35931, 0.33924, 0.32439, 0.31581, 0.30481 };
  float EECorrNoiseMatrixG12[10] = {
    1.00000, 0.71373, 0.44825, 0.30152, 0.21609,
    0.14786, 0.11772, 0.10165, 0.09465, 0.08098 };

  float EBCorrNoiseMatrixG06[10] = {
    1.00000, 0.70946, 0.58021, 0.49846, 0.45006,
    0.41366, 0.39699, 0.38478, 0.37847, 0.37055 };
  
  float EECorrNoiseMatrixG06[10] = {
    1.00000, 0.71217, 0.47464, 0.34056, 0.26282,
    0.20287, 0.17734, 0.16256, 0.15618, 0.14443 };
  
  float EBCorrNoiseMatrixG01[10] = {
    1.00000, 0.73354, 0.64442, 0.58851, 0.55425,
    0.53082, 0.51916, 0.51097, 0.50732, 0.50409 };
  
  float EECorrNoiseMatrixG01[10] = {
    1.00000, 0.72698, 0.62048, 0.55691, 0.51848,
    0.49147, 0.47813, 0.47007, 0.46621, 0.46265};

  TH1F *EBAutocorrSim = new TH1F("EBAutocorrSim","",10,0,10);
  EBAutocorrSim->SetMarkerStyle(kFullCircle);
  TH1F *EEAutocorrSim = (TH1F*)EBAutocorrSim->Clone("EEAutocorrSim");
  for(int s=0;s<10;++s) {
    switch (gain) {
    case 12:
      EBAutocorrSim->SetBinContent(s+1,EBCorrNoiseMatrixG12[s]);
      EEAutocorrSim->SetBinContent(s+1,EECorrNoiseMatrixG12[s]);
      break;
    case 6:
      EBAutocorrSim->SetBinContent(s+1,EBCorrNoiseMatrixG06[s]);
      EEAutocorrSim->SetBinContent(s+1,EECorrNoiseMatrixG06[s]);
      break;
    case 1:
      EBAutocorrSim->SetBinContent(s+1,EBCorrNoiseMatrixG01[s]);
      EEAutocorrSim->SetBinContent(s+1,EECorrNoiseMatrixG01[s]);
      break;
    }
  }

  TCanvas *c1 = new TCanvas("c1","",600,600);
  std::vector<std::string>::iterator it;

  std::vector<TH1F*> corr_EB, corr_EE;


  for(it=covs.begin(); it<covs.end(); ++it) {
    TFile *file = TFile::Open(it->c_str());
    if(!file) continue;
    TH2D *noisecov_average = (TH2D*)file->Get("noisecov_average");
    noisecov_average->GetXaxis()->SetTitle("time sample");
    noisecov_average->GetYaxis()->SetTitle("time sample");
    noisecov_average->DrawClone("colz");
    noisecov_average->DrawClone("text same");
    c1->SaveAs((TString(*it)).ReplaceAll(".root","_matrix.pdf").Data());

    outputcorr->cd();

    TH1F* autocorr = new TH1F("autocorr","",10,0,10);
    autocorr->SetMarkerStyle(kOpenCircle);
    autocorr->SetMarkerColor(kRed);
    autocorr->SetLineColor(kRed);
    for(int s=0;s<10; ++s)  autocorr->SetBinContent(s+1,noisecov_average->GetBinContent(s+1,1));
    autocorr->Draw("PL");
    file->Close();
    delete file;


    TLegend* legend = new TLegend(0.70, 0.70, 0.85, 0.85);
    
    legend->SetBorderSize(     0);
    legend->SetFillColor (  4000);
    legend->SetTextAlign (    12);
    legend->SetTextFont  (    42);
    legend->SetTextSize  (0.03);
    
    legend->AddEntry(EBAutocorrSim,"current sim.","pl");
    legend->AddEntry(autocorr,Form("run %d",run),"pl");

    autocorr->GetYaxis()->SetRangeUser(0.0,1.2);
    autocorr->GetYaxis()->SetTitle("Autocorrelation");
    autocorr->GetXaxis()->SetTitle("Time sample");
    if(TString(*it).Contains("EB")) {
      EBAutocorrSim->Draw("PL same");
      corr_EB.push_back(autocorr);
    }
    else {
      EEAutocorrSim->Draw("PL same");
      corr_EE.push_back(autocorr);
    }
    legend->Draw();

    c1->SaveAs((TString(*it)).ReplaceAll(".root","_1D.pdf").Data());
    //delete autocorr;
    delete legend;
  }

  std::map<std::string, std::vector<TH1F*> > corrs;
  corrs["EB"] = corr_EB;
  corrs["EE"] = corr_EE;

  std::map<std::string, TH1F*> average_corrs;
  average_corrs["EB"] = (TH1F*)corr_EB[0]->Clone(Form("average_correlation_EB_Gain%d_run%d",gain,run));
  average_corrs["EE"] = (TH1F*)corr_EE[0]->Clone(Form("average_correlation_EE_Gain%d_run%d",gain,run));

  for(std::map<std::string, std::vector<TH1F*> >::iterator idet=corrs.begin(); idet!=corrs.end(); ++idet) {
    std::vector<TH1F*> corr = idet->second;
    TH1F* average = average_corrs[idet->first]; 
    int i=0;
    for(std::vector<TH1F*>::iterator it=corr.begin(); it!=corr.end(); ++it) {
      TString option = (i==0) ? "PL" : "PL same";
      int col = i;
      (*it)->SetLineColor(col); (*it)->SetMarkerColor(col);
      (*it)->Draw(option.Data());
      c1->SaveAs(Form("corr_run_%d_%s_compare_Gain%d.pdf",run,idet->first.c_str(),gain));
      //      delete *it;
      if(!std::isnan((*it)->GetBinContent(2))) { 
	average->Add(*it); 
	i++;
      }
    }
    average->Scale(1./double(i+1));
  }

  // now average the autocorrelation in EB and EE, since they are very similar for each sector
  
  for(std::map<std::string, TH1F* >::iterator idet=average_corrs.begin(); idet!=average_corrs.end(); ++idet) {
    TH1F* average = idet->second;
    average->GetYaxis()->SetRangeUser(0.0,1.2);
    average->GetYaxis()->SetTitle("Autocorrelation");
    average->GetXaxis()->SetTitle("Time sample");
    average->Draw("PL");
    if(TString(idet->first).Contains("EB"))  EBAutocorrSim->Draw("PL same");
    else EEAutocorrSim->Draw("PL same");

    TLegend* legend = new TLegend(0.60, 0.70, 0.85, 0.85);
    
    legend->SetBorderSize(     0);
    legend->SetFillColor (  4000);
    legend->SetTextAlign (    12);
    legend->SetTextFont  (    42);
    legend->SetTextSize  (0.03);
    
    legend->AddEntry(EBAutocorrSim,"current sim.","pl");
    legend->AddEntry(average,Form("run %d, average",run),"pl");
    legend->Draw();

    c1->SaveAs(Form("%s.pdf",average->GetName()));
    average->Write();
  }
  
  outputcorr->Close();

}

void saveAutocorr() {
  
  int gain[3] = {1,6,12};
  int run[2] = {195592, 203189};
  bool recreate = true;
  for(int irun=0; irun<2;++irun) for(int igain=0; igain<3; igain++) {
      plotCovarianceMatrix(gain[igain],run[irun],(recreate ? "recreate" : "update"));
      recreate = false;
    }
  
}

void plotHistory() {

  int gain[3] = {1,6,12};
  int run[2] = {195592, 203189};
  
  TFile *fileres = TFile::Open("average_correlation.root");
  
  TCanvas *c1 = new TCanvas("c1","",600,600);
  for(int igain=0; igain<3; igain++) {
    TLegend* legend = new TLegend(0.60, 0.70, 0.85, 0.85);
    for(int irun=0; irun<2;++irun) {
      TH1F *corr_eb = (TH1F*)fileres->Get(Form("average_correlation_EB_Gain%d_run%d",gain[igain],run[irun]));
      TH1F *corr_ee = (TH1F*)fileres->Get(Form("average_correlation_EE_Gain%d_run%d",gain[igain],run[irun]));
      corr_eb->SetMarkerStyle(kOpenCircle+irun);
      corr_eb->SetMarkerColor(kBlack);
      corr_eb->SetLineColor(kBlack);
      corr_ee->SetMarkerStyle(kOpenTriangleDown+irun);
      corr_eb->Draw( (irun==0 ? "PL" : "PL same"));
      corr_ee->Draw("PL same");

      legend->SetBorderSize(     0);
      legend->SetFillColor (  4000);
      legend->SetTextAlign (    12);
      legend->SetTextFont  (    42);
      legend->SetTextSize  (0.03);
      
      legend->AddEntry(corr_eb,Form("barrel, run %d",run[irun]),"pl");
      legend->AddEntry(corr_ee,Form("endcap, run %d",run[irun]),"pl");

    }
    legend->Draw();
    c1->SaveAs(Form("corr_history_Gain%d.pdf",gain[igain]));
  }

}

void dumpTxtFile(const char* rootfile="average_correlation.root") {

  ofstream txtdumpfile;  
  txtdumpfile.open ("samples_noise_correlation.txt", ios::out | ios::trunc);

  txtdumpfile.unsetf ( std::ios::floatfield ); 
  txtdumpfile.precision(6);
  txtdumpfile.setf( std::ios::fixed, std:: ios::floatfield ); // floatfield set to fixed

  TFile *fileres = TFile::Open(rootfile);

  int gain[3] = {12,6,1};
  const int nruns = 1;
  int run[nruns] = {195592};

  for(int idet=0; idet<2; ++idet) {
    for(int igain=0; igain<3; igain++) {
      TLegend* legend = new TLegend(0.60, 0.70, 0.85, 0.85);
      for(int irun=0; irun<nruns;++irun) {
	TH1F *corr = idet==0 ? (TH1F*)fileres->Get(Form("average_correlation_EB_Gain%d_run%d",gain[igain],run[irun])) :
	  (TH1F*)fileres->Get(Form("average_correlation_EE_Gain%d_run%d",gain[igain],run[irun]));

	for(int b=1; b<=corr->GetNbinsX(); ++b) txtdumpfile << corr->GetBinContent(b) << "\t";

	txtdumpfile << std::endl;
      }
    }
  }

}
