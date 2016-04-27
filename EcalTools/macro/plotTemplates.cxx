#include <vector>
#include <string>

#include "TFile.h"
#include "THashList.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "/Users/emanuele/Scripts/RooHZZStyle.C"

void plotTemplates (const char *file, const char *label, bool doEB=true) {

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();
  mystyle->SetPalette(1);

  int firstCry=0;
  int nCry = 10;

  TFile *tfile = TFile::Open(file);
  if(!tfile) {
    std::cout << "File for FED: " << label << " not present" << std::endl;
    return;
  }

  THashList *listOfHists = (THashList*)gDirectory->GetListOfKeys();
  
  std::vector<TH1D*> templates;
  for(int i=0; i<std::min(nCry,listOfHists->GetSize()); ++i) {
    TString name = listOfHists->At(firstCry+i)->GetName();
    TH1D *hist = (TH1D*)gDirectory->Get(name.Data());
    templates.push_back(hist);
  }
    

  double EBPulseShapeTemplate[12] = { 1.13979e-02, 7.58151e-01, 1.00000e+00, 8.87744e-01, 6.73548e-01, 4.74332e-01, 3.19561e-01, 2.15144e-01, 1.47464e-01, 1.01087e-01, 6.93181e-02, 4.75044e-02 };
  double EEPulseShapeTemplate[12] = { 1.16442e-01, 7.56246e-01, 1.00000e+00, 8.97182e-01, 6.86831e-01, 4.91506e-01, 3.44111e-01, 2.45731e-01, 1.74115e-01, 1.23361e-01, 8.74288e-02, 6.19570e-02 };

  double EBPulseShapeCovariance[144] = {
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
        0.000e+00,  0.000e+00,  0.000e+00,  5.589e-07,  6.335e-07,  5.556e-07,  4.752e-07,  3.657e-07,  3.422e-07,  3.768e-07,  4.418e-07,  6.142e-07,
  };

  double EEPulseShapeCovariance[144] = {
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
        0.000e+00,  0.000e+00,  0.000e+00,  6.446e-07,  7.575e-07,  6.886e-07,  6.146e-07,  4.283e-07,  3.882e-07,  4.293e-07,  5.493e-07,  7.027e-07,
  };

  TH1F *simPulseShape = new TH1F("simPulseShape","",10,0,10);
  
  int SAMPLES=10;
  int firstSignalSample=3;
  for(int i=0; i<SAMPLES; ++i) {
    if(i<firstSignalSample) {
      simPulseShape->SetBinContent(i+1, 1E-10);
      simPulseShape->SetBinError(i+1, 0.);      
    }
    else {
      simPulseShape->SetBinContent(i+1, (doEB ? EBPulseShapeTemplate[i-firstSignalSample] : EEPulseShapeTemplate[i-firstSignalSample]));
      int errorIndex = 12*(i-firstSignalSample) + i-firstSignalSample;
      simPulseShape->SetBinError(i+1,doEB ? sqrt(EBPulseShapeCovariance[errorIndex]) : sqrt(EEPulseShapeCovariance[errorIndex]));
    }
  }

  TCanvas *c1 = new TCanvas("c1","",1200,1200);
  simPulseShape->GetXaxis()->SetTitle("Sample");
  simPulseShape->GetYaxis()->SetTitle("Ped. Subtracted Amplitude (a.u.)");
  simPulseShape->SetMinimum(-0.05);
  simPulseShape->SetMarkerStyle(kFullCircle);
  simPulseShape->SetMarkerSize(1);
  simPulseShape->SetFillColor(kAzure+1);
  simPulseShape->Draw("pe2");


  for(int i=0; i<nCry; ++i) {
    templates[i]->SetLineColor(i+2);
    templates[i]->Draw("same");
  }  

  TPaveText *pt = new TPaveText(0.40, 0.90, 0.50, 0.99);
  pt->SetBorderSize(     0);
  pt->SetFillColor (  4000);
  pt->SetTextAlign (    12);
  pt->SetTextFont  (    42);
  pt->SetTextSize  (0.03);
  pt->AddText(label);  
  pt->Draw();

  TLegend* legend = new TLegend(0.20, 0.70, 0.40, 0.85);
  
  legend->SetBorderSize(     0);
  legend->SetFillColor (  4000);
  legend->SetTextAlign (    12);
  legend->SetTextFont  (    42);
  legend->SetTextSize  (0.03);
  
  legend->AddEntry(simPulseShape,"Sim. pulse shape","pf");
  legend->AddEntry(templates[0],"pp pulse shapes","l");
  legend->Draw();

  c1->SaveAs(Form("plot_template_%s.pdf",label));
  c1->SaveAs(Form("plot_template_%s.png",label));

  tfile->Close();

}

void drawAll(int run=211831, int gain=12) {

  std::vector<std::string> templates_files;
  std::vector<std::string> fed_labels;
  for(int ieb=0; ieb<18; ++ieb) { 
    if(ieb<9) {
      templates_files.push_back(Form("templates_run%d_EBp0%d_Gain%d.root",run,ieb+1,gain));
      fed_labels.push_back(Form("EB+0%d",ieb+1));
      templates_files.push_back(Form("templates_run%d_EBm0%d_Gain%d.root",run,ieb+1,gain));
      fed_labels.push_back(Form("EB-0%d",ieb+1));
    } else {
      templates_files.push_back(Form("templates_run%d_EBp%d_Gain%d.root",run,ieb+1,gain));
      fed_labels.push_back(Form("EB+%d",ieb+1));
      templates_files.push_back(Form("templates_run%d_EBm%d_Gain%d.root",run,ieb+1,gain));
      fed_labels.push_back(Form("EB-%d",ieb+1));
    }
  }
  for(int iee=0; iee<9; ++iee) { 
    templates_files.push_back(Form("templates_run%d_EEp0%d_Gain%d.root",run,iee+1,gain));
    fed_labels.push_back(Form("EE+0%d",iee+1));
    templates_files.push_back(Form("templates_run%d_EEm0%d_Gain%d.root",run,iee+1,gain));
    fed_labels.push_back(Form("EE-0%d",iee+1));
  }

  for(int i=0; i<(int)templates_files.size(); ++i) {
    bool doEB = TString(fed_labels[i]).Contains("EB");
    std::cout << "===> DOING TEMPLATES FOR FED = " << fed_labels[i] << std::endl;
    plotTemplates(templates_files[i].c_str(), fed_labels[i].c_str(), doEB); 
  }

}

