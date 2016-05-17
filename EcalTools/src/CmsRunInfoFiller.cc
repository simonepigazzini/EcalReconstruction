#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "EcalReconstruction/EcalTools/interface/CmsRunInfoFiller.h"


using namespace edm;
using namespace std;

CmsRunInfoFiller::CmsRunInfoFiller(CmsTree *cmsTree, bool isMC): 
  privateData_(new CmsRunInfoFillerData) {

  cmstree=cmsTree;
  privateData_->setMC(isMC);
  privateData_->initialise();
  isMC_=isMC;
}

CmsRunInfoFiller::~CmsRunInfoFiller() {

  // delete here the vector ptr's
  delete privateData_->run;
  delete privateData_->event;
  delete privateData_->lumisection;
  delete privateData_->bx;
  delete privateData_->orbit;
  if(isMC_) {
    delete privateData_->nBX;
    delete privateData_->nObsInteractions;
    delete privateData_->nTrueInteractions;
    delete privateData_->bxPU;
  }
  delete privateData_->beamSpotX;
  delete privateData_->beamSpotY;
  delete privateData_->beamSpotZ;
  delete privateData_;
}

void CmsRunInfoFiller::writeRunInfoToTree(const edm::Event& iEvent, const edm::EventSetup& iSetup,
                                          bool dumpData) {

  privateData_->clearVectors();

  *(privateData_->run) = iEvent.id().run();
  *(privateData_->event) = iEvent.id().event();

  if( iEvent.isRealData() ) {
    *(privateData_->lumisection) = iEvent.luminosityBlock();
    *(privateData_->bx) = iEvent.bunchCrossing();
    *(privateData_->orbit) = iEvent.orbitNumber();
  } else {
    *(privateData_->lumisection) = -1;
    *(privateData_->bx) = -1;
    *(privateData_->orbit) = -1;
  }

  Handle<reco::BeamSpot> beamSpotHandle;
  iEvent.getByLabel( "offlineBeamSpot", beamSpotHandle);  
  if(beamSpotHandle.isValid()){
    *(privateData_->beamSpotX) = beamSpotHandle->position().X();
    *(privateData_->beamSpotY) = beamSpotHandle->position().Y();
    *(privateData_->beamSpotZ) = beamSpotHandle->position().Z();
  }else{
    *(privateData_->beamSpotX) = 0.;
    *(privateData_->beamSpotY) = 0.;
    *(privateData_->beamSpotZ) = 0.;
  }
  
  if(isMC_) {

    // Pile Up informations
    Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    iEvent.getByLabel("addPileupInfo", PupInfo);

    *(privateData_->nBX) = PupInfo->size();

    std::vector<PileupSummaryInfo>::const_iterator PVI;
    for(PVI = PupInfo->begin(); PVI < PupInfo->end(); ++PVI) {
      privateData_->nObsInteractions->push_back(PVI->getPU_NumInteractions());
      privateData_->nTrueInteractions->push_back(PVI->getTrueNumInteractions());
      privateData_->bxPU->push_back(PVI->getBunchCrossing());
    }

  }

  treeRunInfo();

  if(dumpData) cmstree->dumpData();

}

void CmsRunInfoFiller::treeRunInfo() {

  cmstree->column("runNumber", *privateData_->run, 0, "L1T");
  cmstree->column("eventNumber", *privateData_->event, uint64_t(0), "L1T");
  cmstree->column("lumiBlock", *privateData_->lumisection, 0, "L1T");
  cmstree->column("bunchCrossing", *privateData_->bx, 0, "L1T");
  cmstree->column("orbitNumber", *privateData_->orbit, 0, "L1T");

  cmstree->column("beamSpotX",*privateData_->beamSpotX, 0., "L1T");
  cmstree->column("beamSpotY",*privateData_->beamSpotY, 0., "L1T");
  cmstree->column("beamSpotZ",*privateData_->beamSpotZ, 0., "L1T");

  if(isMC_) {
    cmstree->column("nBX", *(privateData_->nBX), 0, "Sim");
    cmstree->column("nObsPU", *privateData_->nObsInteractions, "nBX", 0, "Sim");
    cmstree->column("nTruePU", *privateData_->nTrueInteractions, "nBX", 0, "Sim");
    cmstree->column("bxPU", *privateData_->bxPU, "nBX", 0, "Sim");
  }

}

void CmsRunInfoFillerData::initialise() {

  run = new int;
  event = new uint64_t;
  lumisection = new int;
  bx = new int;
  orbit = new int;
  
  if(isMC_) {
    nBX = new int;
    nObsInteractions = new vector<int>;
    nTrueInteractions = new vector<float>;
    bxPU = new vector<int>;
  }

  beamSpotX = new double;
  beamSpotY = new double;
  beamSpotZ = new double;

}

void CmsRunInfoFillerData::clearVectors() {
  if(isMC_) {
    nObsInteractions->clear();
    nTrueInteractions->clear();
    bxPU->clear();
  }
}
