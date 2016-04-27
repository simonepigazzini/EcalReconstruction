#ifndef PedestalTask_H
#define PedestalTask_H

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TMatrixD.h"
#include <map>
#include <vector>
#include <string>

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"


class PedestalTask : public edm::EDAnalyzer {
 public:
  explicit PedestalTask(const edm::ParameterSet&);
  ~PedestalTask() {}
  
  private:

  virtual void beginJob() ;
  virtual void beginRun(const edm::Run & iRun, const edm::EventSetup & iSetup );
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  std::map<DetId, std::vector<double> > pedmeans;
  std::map<DetId, TMatrixD > covariances;
  std::map<DetId,int> nevts_;

  edm::EDGetTokenT<EBDigiCollection>  ebDigiCollectionToken_; 
  edm::EDGetTokenT<EEDigiCollection>  eeDigiCollectionToken_; 

  std::string pedestalMeanFile_, covariancesFile_;
  bool doBarrel_, calcPedMean_;
  TFile *file_;
  int32_t gain_;

  std::vector<std::string> feds_;

};

#endif
