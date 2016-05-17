#ifndef CaloClusterDumper_h
#define CaloClusterDumper_h

#include <TTree.h>
#include <TFile.h>

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "EcalReconstruction/EcalTools/interface/CmsTree.h"


class CaloClusterDumper : public edm::EDAnalyzer {
 public:
  explicit CaloClusterDumper(const edm::ParameterSet&);
  ~CaloClusterDumper();


 private:
  virtual void beginJob() ;
  virtual void beginRun(const edm::Run & iRun, const edm::EventSetup & iSetup );
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

 private:

  //! ECAL superclusters
  edm::InputTag ecalBarrelCaloClusters1_, ecalEndcapCaloClusters1_;
  edm::InputTag ecalBarrelCaloClusters2_, ecalEndcapCaloClusters2_;
  edm::InputTag ecalBarrelCaloClusters3_, ecalEndcapCaloClusters3_;
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
#endif // CaloClusterDumper_h
