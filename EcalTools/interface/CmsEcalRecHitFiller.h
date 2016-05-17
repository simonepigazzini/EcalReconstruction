// -*- C++ -*-
//-----------------------------------------------------------------------
//
// Package:    
//      HiggsAnalysis/HiggsToWW2e
// Description:
//      Class CmsCaloTowerFiller
//      Simple class for dumping RECO (or AOD) contents to a ROOT tree
//      
// Original Author:  Emanuele Di Marco
//         Created:  Mon March  10 10:05:34 CEST 2014
//
//-----------------------------------------------------------------------

#ifndef CmsEcalRecHitFiller_h
#define CmsEcalRecHitFiller_h

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"

#include "EcalReconstruction/EcalTools/interface/CmsTree.h"
#include <TTree.h>

struct CmsEcalRecHitFillerData {

  
  std::vector<float> *energy, *time, *timeerror, *eta, *phi, *ix, *iy;
  std::vector<float> *swissX, *r9, *chi2;

public:
  void initialise();
  void clear();

};

class CmsEcalRecHitFiller {

 public:

  // Constructors

  // Dump everything
  CmsEcalRecHitFiller(CmsTree *, int maxHits=100000);

  
  // Destructor
  virtual ~CmsEcalRecHitFiller();

  void writeCollectionToTree(edm::InputTag,
			     const edm::Event&, const edm::EventSetup&,
			     const std::string &columnPrefix, const std::string &columnSuffix,
			     bool dumpData=false);

 private:
  
  void writeEcalRecHitInfo(const EcalRecHit *hit, const EcalRecHitCollection *collection, bool isEB, const CaloSubdetectorTopology*, const CaloSubdetectorGeometry*);
  void treeEcalRecHitInfo(const std::string colPrefix, const std::string colSuffix);
  float recHitEnergyECAL(DetId id, const EcalRecHitCollection *recHits);

  bool hitLimitsMeansNoOutput_;
  int maxHits_;

  std::string *trkIndexName_;

  CmsEcalRecHitFillerData *privateData_;
  edm::InputTag rechitLabel_;

  CmsTree *cmstree;

};

#endif // CmsEcalRecHitFiller_h
