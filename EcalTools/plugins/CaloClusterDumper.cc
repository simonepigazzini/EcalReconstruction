#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "EcalReconstruction/EcalTools/interface/CmsCaloClusterFiller.h"
#include "EcalReconstruction/EcalTools/interface/CmsRunInfoFiller.h"
#include "EcalReconstruction/EcalTools/interface/CmsMcTruthTreeFiller.h"
#include "EcalReconstruction/EcalTools/plugins/CaloClusterDumper.h"

using namespace edm;
using namespace reco;

CaloClusterDumper::CaloClusterDumper(const edm::ParameterSet& iConfig)
{
  
  fileName_      = iConfig.getUntrackedParameter<std::string>("fileName", "RootOutput.root");
  nameTree_      = iConfig.getUntrackedParameter<std::string>("nameTree", "BaseTree");
  isMC_          = iConfig.getUntrackedParameter<bool>("isMC",false);
  //  superclusters collections
  ecalBarrelCaloClusters1_ = iConfig.getParameter<edm::InputTag>("EBCaloClusters1");
  ecalEndcapCaloClusters1_ = iConfig.getParameter<edm::InputTag>("EECaloClusters1");
  ecalBarrelCaloClusters2_ = iConfig.getParameter<edm::InputTag>("EBCaloClusters2");
  ecalEndcapCaloClusters2_ = iConfig.getParameter<edm::InputTag>("EECaloClusters2");
  ecalBarrelCaloClusters3_ = iConfig.getParameter<edm::InputTag>("EBCaloClusters3");
  ecalEndcapCaloClusters3_ = iConfig.getParameter<edm::InputTag>("EECaloClusters3");
  // rechits (used for the clusters shapes)
  ecalBarrelRecHits_       = iConfig.getParameter<edm::InputTag>("ecalBarrelRecHits");
  ecalEndcapRecHits_       = iConfig.getParameter<edm::InputTag>("ecalEndcapRecHits");
  // MC truth
  mcTruthCollection_       = iConfig.getParameter<edm::InputTag>("mcTruthCollection");

}

CaloClusterDumper::~CaloClusterDumper() { }

// ------------ method called to for each event  ------------
void CaloClusterDumper::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{ 

  CmsRunInfoFiller runFiller( tree_, isMC_ );
  runFiller.writeRunInfoToTree(iEvent,iSetup,false);

  CmsCaloClusterFiller ebFill(tree_);
  std::string prefix("");
  std::string suffix("EBCaloClusters");
  ebFill.setEcalHits(ecalBarrelRecHits_);
  ebFill.writeCollectionToTree(ecalBarrelCaloClusters1_, iEvent, iSetup, prefix, suffix, false);

  CmsCaloClusterFiller eeFill(tree_);
  suffix = std::string("EECaloClusters");
  eeFill.setEcalHits(ecalEndcapRecHits_);
  eeFill.writeCollectionToTree(ecalEndcapCaloClusters1_, iEvent, iSetup, prefix, suffix, false);

  if(ecalBarrelCaloClusters2_.label().size()>0) {
    CmsCaloClusterFiller ebFill2(tree_);
    suffix = std::string("EBCaloClusters2");
    ebFill2.setEcalHits(ecalBarrelRecHits_);
    ebFill2.writeCollectionToTree(ecalBarrelCaloClusters2_, iEvent, iSetup, prefix, suffix, false);
  }

  if(ecalEndcapCaloClusters2_.label().size()>0) {
    CmsCaloClusterFiller eeFill2(tree_);
    suffix = std::string("EECaloClusters2");
    eeFill2.setEcalHits(ecalEndcapRecHits_);
    eeFill2.writeCollectionToTree(ecalEndcapCaloClusters2_, iEvent, iSetup, prefix, suffix, false);
  }

  if(ecalBarrelCaloClusters3_.label().size()>0) {
    CmsCaloClusterFiller ebFill3(tree_);
    suffix = std::string("EBCaloClusters3");
    ebFill3.setEcalHits(ecalBarrelRecHits_);
    ebFill3.writeCollectionToTree(ecalBarrelCaloClusters3_, iEvent, iSetup, prefix, suffix, false);
  }

  if(ecalEndcapCaloClusters3_.label().size()>0) {
    CmsCaloClusterFiller eeFill3(tree_);
    suffix = std::string("EECaloClusters3");
    eeFill3.setEcalHits(ecalEndcapRecHits_);
    eeFill3.writeCollectionToTree(ecalEndcapCaloClusters3_, iEvent, iSetup, prefix, suffix, false);
  }

  // to compare with the MC truth
  // get MC truth
  CmsMcTruthTreeFiller treeFill(tree_);
  treeFill.writeCollectionToTree( mcTruthCollection_, iEvent, 100 );

  tree_->dumpData();
  

}

// ------------ method called once each job just before starting event loop  ------------
void CaloClusterDumper::beginJob() {
  
  fileOut_ = TFile::Open(fileName_.c_str(), "RECREATE");

  tree_  = new  CmsTree(nameTree_.c_str(),nameTree_.c_str());
}

void CaloClusterDumper::beginRun( const Run & iRun, const EventSetup & iSetup ) { }



// ------------ method called once each job just after ending the event loop  ------------
void  CaloClusterDumper::endJob() {

  fileOut_->cd();

  TTree* treeEventsOut = tree_->getTree();
  treeEventsOut->Write();

  fileOut_->Close();

}

DEFINE_FWK_MODULE (CaloClusterDumper);

