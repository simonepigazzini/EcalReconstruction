#ifndef SuperClusterDumper_h
#define SuperClusterDumper_h

#include <TTree.h>
#include <TFile.h>

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "EcalReconstruction/EcalTools/interface/CmsTree.h"


class SuperClusterDumper : public edm::EDAnalyzer {
 public:
  explicit SuperClusterDumper(const edm::ParameterSet&);
  ~SuperClusterDumper();


 private:
  virtual void beginJob() ;
  virtual void beginRun(const edm::Run & iRun, const edm::EventSetup & iSetup );
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

 private:

  //! ECAL superclusters
  edm::InputTag ecalBarrelSuperClusters1_, ecalEndcapSuperClusters1_;
  edm::InputTag ecalBarrelSuperClusters2_, ecalEndcapSuperClusters2_;
  //! ECAL rechits to compute the cluster shapes on the fly
  edm::InputTag ecalBarrelRecHits_, ecalEndcapRecHits_;
  //! generator-level particle collection in input
  edm::InputTag mcTruthCollection_;
  //! output
  std::string fileName_, nameTree_;
  bool isMC_;
  //! ROOT file with the plain ROOT tree inside
  TFile *fileOut_;
  //! the tree with the events
  CmsTree *tree_;

};
#endif // SuperClusterDumper_h
