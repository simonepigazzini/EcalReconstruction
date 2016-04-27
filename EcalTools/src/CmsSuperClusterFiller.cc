//---------------------------------------------------------------------------
//
// Description:
//       Package:   HtoWWTreeDumper
//       Class:     CmsSuperClusterFiller
//
// Original Author:  Emanuele Di Marco
//         Created:  Mon Apr  9 11:01:00 CEST 2007
//-----------------------------------------------------------------------

// system include files
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"

#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "EcalReconstruction/EcalTools/interface/CmsSuperClusterFiller.h"

#include <TTree.h>

#include <string>
#include <iostream>

using namespace edm;
using namespace reco;
using namespace std;



//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------


CmsSuperClusterFiller::CmsSuperClusterFiller(CmsTree *cmsTree, int maxSC):  privateData_(new CmsSuperClusterFillerData) 
{
  cmstree=cmsTree;
  trkIndexName_ = new std::string("n");
  maxSC_=maxSC;
  privateData_->initialiseCandidate();
}

//--------------
// Destructor --
//--------------

CmsSuperClusterFiller::~CmsSuperClusterFiller() 
{
  // delete here the vector ptr's
  delete privateData_->nBC;
  delete privateData_->nCrystals;
  delete privateData_->rawEnergy;
  delete privateData_->energy;
  delete privateData_->seedClusterEnergy;
  delete privateData_->seedCrystalEnergy;
  delete privateData_->seedCrystalTime;
  delete privateData_->eta;
  delete privateData_->theta;
  delete privateData_->phi;
  delete privateData_;
}


//-------------
// Methods   --
//-------------

// Set boolean control options for quantities that are written out


void CmsSuperClusterFiller::writeCollectionToTree(edm::InputTag collectionTag,
						  const edm::Event& iEvent, const edm::EventSetup& iSetup,
						  const std::string &columnPrefix, const std::string &columnSuffix,
						  bool dumpData)
{
  
  Handle<SuperClusterCollection> collectionHandle;
  try { iEvent.getByLabel(collectionTag, collectionHandle); }
  catch ( cms::Exception& ex ) { edm::LogWarning("CmsSuperClusterFiller") << "Can't get SC Collection: " << collectionTag; }
  const SuperClusterCollection *collection = collectionHandle.product();

  privateData_->clear();
  
  if(collection) 
    {
      if((int)collection->size() > maxSC_)
	{
	  edm::LogError("CmsSuperClusterFiller") << "Track length " << collection->size() 
						 << " is too long for declared max length for tree "
						 << maxSC_ 
						 << ". Collection will be truncated ";
	}
      
      *(privateData_->nSC) = collection->size();
  
      Handle< EcalRecHitCollection > EBEERecHits;
      try { iEvent.getByLabel(EcalRecHits_, EBEERecHits); }
      catch ( cms::Exception& ex ) { edm::LogWarning("CmsSuperClusterFiller") << "Can't get ECAL rec hits Collection" << EcalRecHits_; }
      const EcalRecHitCollection *EcalRecHits = EBEERecHits.product();
      
      SuperClusterCollection::const_iterator cand;
      for(cand=collection->begin(); cand!=collection->end(); cand++) {
        // fill basic kinematics
        writeSCInfo(&(*cand),iEvent,iSetup,EcalRecHits);
      }
    }
  else {
    *(privateData_->nSC) = 0;
  }
  
  // The class member vectors containing the relevant quantities 
  // have all been filled. Now transfer those we want into the 
  // tree 
  
  int blockSize = (collection) ? collection->size() : 0;
    
  std::string nCandString = columnPrefix+(*trkIndexName_)+columnSuffix; 
  cmstree->column(nCandString.c_str(),blockSize,0,"Reco");
  
  treeSCInfo(columnPrefix,columnSuffix);

  if(dumpData) cmstree->dumpData();

  delete trkIndexName_;

}






void CmsSuperClusterFiller::writeSCInfo(const SuperCluster *cand, 
                                        const edm::Event& iEvent, const edm::EventSetup& iSetup,
                                        const EcalRecHitCollection *EcalRecHits) {

  // fill the SC infos
  privateData_->nBC->push_back((int)cand->clustersSize());
  privateData_->nCrystals->push_back((int)cand->hitsAndFractions().size());
  privateData_->rawEnergy->push_back((float)cand->rawEnergy());
  privateData_->seedClusterEnergy->push_back((float)cand->seed()->energy());
  privateData_->energy->push_back((float)cand->energy());
  privateData_->eta->push_back((float)cand->position().eta());
  privateData_->theta->push_back((float)cand->position().theta());
  privateData_->phi->push_back((float)cand->position().phi());

  // seed crystal properties
  const Ptr<CaloCluster> theSeed = cand->seed();
  std::pair<DetId, float> maxRH = EcalClusterTools::getMaximum( *theSeed, &(*EcalRecHits) );
  DetId seedCrystalId = maxRH.first;

  EcalRecHitCollection::const_iterator seedRH = EcalRecHits->find(seedCrystalId);
  privateData_->seedCrystalEnergy->push_back(seedRH->energy());
  privateData_->seedCrystalTime->push_back(seedRH->time());

  // fill the seed basic cluster shapes
//   edm::ESHandle<CaloTopology> pTopology;
//   iSetup.get<CaloTopologyRecord>().get(pTopology);
  
//   edm::ESHandle<CaloGeometry> pGeometry;
//   iSetup.get<CaloGeometryRecord>().get(pGeometry);


}

void CmsSuperClusterFiller::treeSCInfo(const std::string colPrefix, const std::string colSuffix) 
{
  std::string nCandString = colPrefix+(*trkIndexName_)+colSuffix;
  cmstree->column((colPrefix+"nBC"+colSuffix).c_str(), *privateData_->nBC, nCandString.c_str(), 0, "Reco");
  cmstree->column((colPrefix+"nCrystals"+colSuffix).c_str(), *privateData_->nCrystals, nCandString.c_str(), 0, "Reco");
  cmstree->column((colPrefix+"rawEnergy"+colSuffix).c_str(), *privateData_->rawEnergy, nCandString.c_str(), 0, "Reco");
  cmstree->column((colPrefix+"energy"+colSuffix).c_str(), *privateData_->energy, nCandString.c_str(), 0, "Reco");
  cmstree->column((colPrefix+"seedClusterEnergy"+colSuffix).c_str(), *privateData_->seedClusterEnergy, nCandString.c_str(), 0, "Reco");
  cmstree->column((colPrefix+"eta"+colSuffix).c_str(), *privateData_->eta, nCandString.c_str(), 0, "Reco");
  cmstree->column((colPrefix+"theta"+colSuffix).c_str(), *privateData_->theta, nCandString.c_str(), 0, "Reco");
  cmstree->column((colPrefix+"phi"+colSuffix).c_str(), *privateData_->phi, nCandString.c_str(), 0, "Reco");
  cmstree->column((colPrefix+"seedCrystalEnergy"+colSuffix).c_str(), *privateData_->seedCrystalEnergy, nCandString.c_str(), 0, "Reco");
  cmstree->column((colPrefix+"seedCrystalTime"+colSuffix).c_str(), *privateData_->seedCrystalTime, nCandString.c_str(), 0, "Reco");
}


void CmsSuperClusterFillerData::initialiseCandidate() 
{
  nBC = new vector<int>;
  nCrystals = new vector<int>; 
  rawEnergy = new vector<float>; 
  energy = new vector<float>; 
  seedClusterEnergy = new vector<float>;
  eta = new vector<float>; 
  theta = new vector<float>; 
  phi = new vector<float>;
  seedCrystalEnergy = new vector<float>;
  seedCrystalTime = new vector<float>;
  nSC =  new int;
}

void CmsSuperClusterFillerData::clear() 
{
  nBC->clear();
  nCrystals->clear();
  rawEnergy->clear();
  energy->clear();
  seedClusterEnergy->clear();
  eta->clear(); 
  theta->clear();
  phi->clear();
  seedClusterEnergy->clear();
  seedCrystalEnergy->clear();
  seedCrystalTime->clear();
}
