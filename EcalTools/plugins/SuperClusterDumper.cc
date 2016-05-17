#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "EcalReconstruction/EcalTools/interface/CmsSuperClusterFiller.h"
#include "EcalReconstruction/EcalTools/interface/CmsRunInfoFiller.h"
#include "EcalReconstruction/EcalTools/interface/CmsMcTruthTreeFiller.h"
#include "EcalReconstruction/EcalTools/plugins/SuperClusterDumper.h"

using namespace edm;
using namespace reco;

SuperClusterDumper::SuperClusterDumper(const edm::ParameterSet& iConfig)
{
  
  fileName_      = iConfig.getUntrackedParameter<std::string>("fileName", "RootOutput.root");
  nameTree_      = iConfig.getUntrackedParameter<std::string>("nameTree", "BaseTree");
  isMC_          = iConfig.getUntrackedParameter<bool>("isMC",false);
  //  superclusters collections
  ecalBarrelSuperClusters1_ = iConfig.getParameter<edm::InputTag>("EBSuperClusters1");
  ecalEndcapSuperClusters1_ = iConfig.getParameter<edm::InputTag>("EESuperClusters1");
  ecalBarrelSuperClusters2_ = iConfig.getParameter<edm::InputTag>("EBSuperClusters2");
  ecalEndcapSuperClusters2_ = iConfig.getParameter<edm::InputTag>("EESuperClusters2");
  // rechits (used for the clusters shapes)
  ecalBarrelRecHits_       = iConfig.getParameter<edm::InputTag>("ecalBarrelRecHits");
  ecalEndcapRecHits_       = iConfig.getParameter<edm::InputTag>("ecalEndcapRecHits");
  // MC truth
  mcTruthCollection_       = iConfig.getParameter<edm::InputTag>("mcTruthCollection");

}

SuperClusterDumper::~SuperClusterDumper() { }

// ------------ method called to for each event  ------------
void SuperClusterDumper::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{ 

  CmsRunInfoFiller runFiller( tree_, isMC_ );
  runFiller.writeRunInfoToTree(iEvent,iSetup,false);

  CmsSuperClusterFiller ebFill(tree_);
  std::string prefix("");
  std::string suffix("EBSuperClusters");
  ebFill.setEcalHits(ecalBarrelRecHits_);
  ebFill.writeCollectionToTree(ecalBarrelSuperClusters1_, iEvent, iSetup, prefix, suffix, false);

  CmsSuperClusterFiller eeFill(tree_);
  suffix = std::string("EESuperClusters");
  eeFill.setEcalHits(ecalEndcapRecHits_);
  eeFill.writeCollectionToTree(ecalEndcapSuperClusters1_, iEvent, iSetup, prefix, suffix, false);

  if(ecalBarrelSuperClusters2_.instance().compare("")!=0) {
    CmsSuperClusterFiller ebFill2(tree_);
    std::string suffix("EBSuperClusters2");
    ebFill2.setEcalHits(ecalBarrelRecHits_);
    ebFill2.writeCollectionToTree(ecalBarrelSuperClusters2_, iEvent, iSetup, prefix, suffix, false);
  }

  if(ecalEndcapSuperClusters2_.instance().compare("")!=0) {
    CmsSuperClusterFiller eeFill2(tree_);
    std::string suffix("EESuperClusters2");
    eeFill2.setEcalHits(ecalEndcapRecHits_);
    eeFill2.writeCollectionToTree(ecalEndcapSuperClusters2_, iEvent, iSetup, prefix, suffix, false);
  }

  // to compare with the MC truth
  // get MC truth
  CmsMcTruthTreeFiller treeFill(tree_);
  treeFill.writeCollectionToTree( mcTruthCollection_, iEvent, 100 );

  tree_->dumpData();
  

}

// ------------ method called once each job just before starting event loop  ------------
void SuperClusterDumper::beginJob() {
  
  fileOut_ = TFile::Open(fileName_.c_str(), "RECREATE");

  tree_  = new  CmsTree(nameTree_.c_str(),nameTree_.c_str());
}

void SuperClusterDumper::beginRun( const Run & iRun, const EventSetup & iSetup ) { }



// ------------ method called once each job just after ending the event loop  ------------
void  SuperClusterDumper::endJob() {

  fileOut_->cd();

  TTree* treeEventsOut = tree_->getTree();
  treeEventsOut->Write();

  fileOut_->Close();

}

DEFINE_FWK_MODULE (SuperClusterDumper);

