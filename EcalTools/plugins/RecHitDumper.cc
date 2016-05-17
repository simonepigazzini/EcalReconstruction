#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "EcalReconstruction/EcalTools/interface/CmsEcalRecHitFiller.h"
#include "EcalReconstruction/EcalTools/interface/CmsRunInfoFiller.h"
#include "EcalReconstruction/EcalTools/plugins/RecHitDumper.h"

using namespace edm;
using namespace reco;

RecHitDumper::RecHitDumper(const edm::ParameterSet& iConfig)
{
  
  fileName_      = iConfig.getUntrackedParameter<std::string>("fileName", "RootOutput.root");
  nameTree_      = iConfig.getUntrackedParameter<std::string>("nameTree", "BaseTree");
  isMC_          = iConfig.getUntrackedParameter<bool>("isMC",false);
  // ECAL rechits collections
  ecalBarrelRecHits1_ = iConfig.getParameter<edm::InputTag>("EBRecHits1");
  ecalEndcapRecHits1_ = iConfig.getParameter<edm::InputTag>("EERecHits1");
  ecalBarrelRecHits2_ = iConfig.getParameter<edm::InputTag>("EBRecHits2");
  ecalEndcapRecHits2_ = iConfig.getParameter<edm::InputTag>("EERecHits2");
}

RecHitDumper::~RecHitDumper() { }

// ------------ method called to for each event  ------------
void RecHitDumper::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{ 

  CmsRunInfoFiller runFiller( tree_, isMC_ );
  runFiller.writeRunInfoToTree(iEvent,iSetup,false);

  CmsEcalRecHitFiller ebFill(tree_);
  std::string prefix("");
  std::string suffix("EBRecHits");
  ebFill.writeCollectionToTree(ecalBarrelRecHits1_, iEvent, iSetup, prefix, suffix, false);

  CmsEcalRecHitFiller eeFill(tree_);
  suffix = std::string("EERecHits");
  ebFill.writeCollectionToTree(ecalEndcapRecHits1_, iEvent, iSetup, prefix, suffix, false);

  if(ecalBarrelRecHits2_.instance().compare("")!=0) {
    CmsEcalRecHitFiller ebFill2(tree_);
    std::string suffix("EBRecHits2");
    ebFill2.writeCollectionToTree(ecalBarrelRecHits2_, iEvent, iSetup, prefix, suffix, false);
  }

  if(ecalEndcapRecHits2_.instance().compare("")!=0) {
    CmsEcalRecHitFiller eeFill2(tree_);
    std::string suffix("EERecHits2");
    eeFill2.writeCollectionToTree(ecalEndcapRecHits2_, iEvent, iSetup, prefix, suffix, false);
  }

  tree_->dumpData();
  

}

// ------------ method called once each job just before starting event loop  ------------
void RecHitDumper::beginJob() {
  
  fileOut_ = TFile::Open(fileName_.c_str(), "RECREATE");

  tree_  = new  CmsTree(nameTree_.c_str(),nameTree_.c_str());
}

void RecHitDumper::beginRun( const Run & iRun, const EventSetup & iSetup ) { }



// ------------ method called once each job just after ending the event loop  ------------
void  RecHitDumper::endJob() {

  fileOut_->cd();

  TTree* treeEventsOut = tree_->getTree();
  treeEventsOut->Write();

  fileOut_->Close();

}

DEFINE_FWK_MODULE (RecHitDumper);

