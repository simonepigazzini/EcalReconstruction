#ifndef RecHitDumper_h
#define RecHitDumper_h

#include <TTree.h>
#include <TFile.h>

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "EcalReconstruction/EcalTools/interface/CmsTree.h"


class RecHitDumper : public edm::EDAnalyzer {
 public:
  explicit RecHitDumper(const edm::ParameterSet&);
  ~RecHitDumper();


 private:
  virtual void beginJob() ;
  virtual void beginRun(const edm::Run & iRun, const edm::EventSetup & iSetup );
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

 private:

  //! ECAL rechits
  edm::InputTag ecalBarrelRecHits1_, ecalEndcapRecHits1_;
  edm::InputTag ecalBarrelRecHits2_, ecalEndcapRecHits2_;
  //! output
  std::string fileName_, nameTree_;
  bool isMC_;
  //! ROOT file with the plain ROOT tree inside
  TFile *fileOut_;
  //! the tree with the events
  CmsTree *tree_;

};
#endif // RecHitDumper_h
