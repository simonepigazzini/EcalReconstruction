#ifndef __Analyzer_PulseTemplates_H__
#define __Analyzer_PulseTemplates_H__

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"

#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"

#include "TTree.h"
#include "TH1D.h"

#include "DataFormats/EcalDigi/interface/EEDataFrame.h"
#include "DataFormats/EcalDigi/interface/EBDataFrame.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"


#include "CondFormats/EcalObjects/interface/EcalPedestals.h"
#include "CondFormats/EcalObjects/interface/EcalGainRatios.h"
#include "DataFormats/EcalDigi/interface/EEDataFrame.h"
#include "DataFormats/EcalDigi/interface/EBDataFrame.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/DataRecord/interface/EcalGainRatiosRcd.h"
#include "CondFormats/DataRecord/interface/EcalPedestalsRcd.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"

#include <memory>




class PulseTemplates : public edm::one::EDAnalyzer<> {
 public:    
  PulseTemplates(const edm::ParameterSet&);
  ~PulseTemplates() { }
  
  virtual void beginJob() ;
  virtual void beginRun(const edm::Run & iRun, const edm::EventSetup & iSetup );
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endRun(const edm::Run & iRun, const edm::EventSetup & iSetup ) ;
  virtual void endJob() ;

 private:
  void FillRecHit(const EcalDataFrame& dataFrame,const EcalRecHit& rechit);

  uint32_t setFlagBits(const std::vector<std::vector<uint32_t> >& map, 
                        const uint32_t& status  );  
  
  edm::ESHandle<EcalPedestals> peds;
  edm::ESHandle<EcalGainRatios>  gains;    
   
  edm::ESHandle<EcalChannelStatus> chStatus;
  std::vector<int> v_chstatus_;  
  uint32_t flagmask_; // do not propagate channels with these flags on
  // Associate reco flagbit ( outer vector) to many db status flags (inner vector)
  std::vector<std::vector<uint32_t> > v_DB_reco_flags_;  
  
  
  edm::EDGetTokenT<EBDigiCollection>  ebDigiCollectionToken_; 
  edm::EDGetTokenT<EEDigiCollection>  eeDigiCollectionToken_; 

  edm::EDGetTokenT<EBRecHitCollection> ebhitcollToken_;
  edm::EDGetTokenT<EERecHitCollection> eehitcollToken_;

  double minEnergyBarrel_, minEnergyEndcap_;
  double maxEnergyBarrel_, maxEnergyEndcap_;
  double maxChi2_;
  std::vector<double> ebSimPulseShape_, eeSimPulseShape_;
  int minNHits_;
  bool pedestalAnalysis_;

  int run_;
  int lumi_;
  unsigned int gain_;
  bool barrel_;
  int ietaix_;
  int iphiiy_;
  int iz_;
  double pedrms_;
  double pedval_;
  double pulse_[10];
  unsigned int rawid_;

  double ene_;
  double time_;
  double chi2_;
  int flag_kweird_;
  int flag_kdiweird_;

  typedef std::map<unsigned int, std::vector<float> > tempmap;
  tempmap templates_ref_;
  std::map<int, std::vector<double> > templates_;
  std::map<int, std::vector<double> > templates_weight_;
  std::map<int, double> norm_average_;
  std::map<int, double> norm_counts_;
  std::map<int, unsigned int> rawIds_;
  std::map<int, int> iXs_;
  std::map<int, int> iYs_;
  std::map<int, int> iZs_;

  TH1D *ebSimPulseShapeH_, *eeSimPulseShapeH_;
  
};

DEFINE_FWK_MODULE(PulseTemplates);

#endif
