//---------------------------------------------------------------------------
//
// Description:
//       Package:   HtoWWTreeDumper
//       Class:     CmsCaloClusterFiller
//
// Original Author:  Emanuele Di Marco
//         Created:  Mon Apr  9 11:01:00 CEST 2007
//-----------------------------------------------------------------------

// system include files
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
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

#include "EcalReconstruction/EcalTools/interface/CmsCaloClusterFiller.h"

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


CmsCaloClusterFiller::CmsCaloClusterFiller(CmsTree *cmsTree, int maxCluster):  privateData_(new CmsCaloClusterFillerData) 
{
  cmstree=cmsTree;
  trkIndexName_ = new std::string("n");
  maxCluster_=maxCluster;
  privateData_->initialiseCandidate();
  EcalRecHits_ = std::string();
  dumpSeed_ = false;
}

//--------------
// Destructor --
//--------------

CmsCaloClusterFiller::~CmsCaloClusterFiller() 
{
  // delete here the vector ptr's
  delete privateData_->nCrystals;
  delete privateData_->energy;
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


void CmsCaloClusterFiller::writeCollectionToTree(edm::InputTag collectionTag,
						  const edm::Event& iEvent, const edm::EventSetup& iSetup,
						  const std::string &columnPrefix, const std::string &columnSuffix,
						  bool dumpData)
{
  
  Handle<CaloClusterCollection> collectionHandle;
  try { iEvent.getByLabel(collectionTag, collectionHandle); }
  catch ( cms::Exception& ex ) { edm::LogWarning("CmsCaloClusterFiller") << "Can't get Cluster Collection: " << collectionTag; }
  const CaloClusterCollection *collection = collectionHandle.product();

  privateData_->clear();
  
  if(collection) 
    {
      if((int)collection->size() > maxCluster_)
	{
	  edm::LogError("CmsCaloClusterFiller") << "Track length " << collection->size() 
						 << " is too long for declared max length for tree "
						 << maxCluster_ 
						 << ". Collection will be truncated ";
	}
      
      *(privateData_->nCluster) = collection->size();
  
      EcalRecHitCollection *EcalRecHits = 0;
      if(EcalRecHits_.label().size()>0) {
        Handle< EcalRecHitCollection > EBEERecHits;
        iEvent.getByLabel(EcalRecHits_, EBEERecHits);
        EcalRecHits = (EcalRecHitCollection*)EBEERecHits.product();
        dumpSeed_=true;
      }

      CaloClusterCollection::const_iterator cand;
      for(cand=collection->begin(); cand!=collection->end(); cand++) {
        // fill basic kinematics
        writeClusterInfo(&(*cand),iEvent,iSetup,EcalRecHits);
      }
    }
  else {
    *(privateData_->nCluster) = 0;
  }
  
  // The class member vectors containing the relevant quantities 
  // have all been filled. Now transfer those we want into the 
  // tree 
  
  int blockSize = (collection) ? collection->size() : 0;
    
  std::string nCandString = columnPrefix+(*trkIndexName_)+columnSuffix; 
  cmstree->column(nCandString.c_str(),blockSize,0,"Reco");
  
  treeClusterInfo(columnPrefix,columnSuffix);

  if(dumpData) cmstree->dumpData();

  delete trkIndexName_;

}






void CmsCaloClusterFiller::writeClusterInfo(const CaloCluster *cand, 
                                            const edm::Event& iEvent, const edm::EventSetup& iSetup,
                                            const EcalRecHitCollection *EcalRecHits) {

  // fill the Cluster infos
  privateData_->nCrystals->push_back((int)cand->hitsAndFractions().size());
  privateData_->energy->push_back((float)cand->energy());
  privateData_->eta->push_back((float)cand->position().eta());
  privateData_->theta->push_back((float)cand->position().theta());
  privateData_->phi->push_back((float)cand->position().phi());

  if(dumpSeed_) {
    // seed crystal properties
    std::pair<DetId, float> maxRH = EcalClusterTools::getMaximum( *cand, &(*EcalRecHits) );
    DetId seedCrystalId = maxRH.first;
    
    EcalRecHitCollection::const_iterator seedRH = EcalRecHits->find(seedCrystalId);
    privateData_->seedCrystalEnergy->push_back(seedRH->energy());
    privateData_->seedCrystalTime->push_back(seedRH->time());
  }

}

void CmsCaloClusterFiller::treeClusterInfo(const std::string colPrefix, const std::string colSuffix) 
{
  std::string nCandString = colPrefix+(*trkIndexName_)+colSuffix;
  cmstree->column((colPrefix+"nCrystals"+colSuffix).c_str(), *privateData_->nCrystals, nCandString.c_str(), 0, "Reco");
  cmstree->column((colPrefix+"energy"+colSuffix).c_str(), *privateData_->energy, nCandString.c_str(), 0, "Reco");
  cmstree->column((colPrefix+"eta"+colSuffix).c_str(), *privateData_->eta, nCandString.c_str(), 0, "Reco");
  cmstree->column((colPrefix+"theta"+colSuffix).c_str(), *privateData_->theta, nCandString.c_str(), 0, "Reco");
  cmstree->column((colPrefix+"phi"+colSuffix).c_str(), *privateData_->phi, nCandString.c_str(), 0, "Reco");
  if(dumpSeed_) {
    cmstree->column((colPrefix+"seedCrystalEnergy"+colSuffix).c_str(), *privateData_->seedCrystalEnergy, nCandString.c_str(), 0, "Reco");
    cmstree->column((colPrefix+"seedCrystalTime"+colSuffix).c_str(), *privateData_->seedCrystalTime, nCandString.c_str(), 0, "Reco");
  }
}


void CmsCaloClusterFillerData::initialiseCandidate() 
{
  nCrystals = new vector<int>; 
  energy = new vector<float>; 
  eta = new vector<float>; 
  theta = new vector<float>; 
  phi = new vector<float>;
  seedCrystalEnergy = new vector<float>;
  seedCrystalTime = new vector<float>;
  nCluster =  new int;
}

void CmsCaloClusterFillerData::clear() 
{
  nCrystals->clear();
  energy->clear();
  eta->clear(); 
  theta->clear();
  phi->clear();
  seedCrystalEnergy->clear();
  seedCrystalTime->clear();
}
