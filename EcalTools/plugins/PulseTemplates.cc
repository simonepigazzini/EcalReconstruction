#include "PulseTemplates.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "Geometry/Records/interface/CaloTopologyRecord.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CommonTools/Utils/interface/StringToEnumValue.h"

#include "DataFormats/Common/interface/Handle.h"

#include <iostream>
#include <fstream>

PulseTemplates::PulseTemplates(const edm::ParameterSet& conf)
{
  
  // for AlCaP0 stream
  // ebDigiCollectionToken_ = consumes<EBDigiCollection>(edm::InputTag("dummyHits","dummyBarrelDigis"));
  // eeDigiCollectionToken_ = consumes<EEDigiCollection>(edm::InputTag("dummyHits","dummyEndcapDigis"));

  // for phi symmetry stream
  // ebDigiCollectionToken_ = consumes<EBDigiCollection>(edm::InputTag("hltEcalPhiSymFilter","phiSymEcalDigisEB"));
  // eeDigiCollectionToken_ = consumes<EEDigiCollection>(edm::InputTag("hltEcalPhiSymFilter","phiSymEcalDigisEE"));

  ebDigiCollectionToken_ = consumes<EBDigiCollection>(conf.getParameter<edm::InputTag>("barrelDigis"));
  eeDigiCollectionToken_ = consumes<EEDigiCollection>(conf.getParameter<edm::InputTag>("endcapDigis"));

  ebhitcollToken_  = consumes<EBRecHitCollection>(conf.getParameter<edm::InputTag> ("EBRecHitCollection"));
  eehitcollToken_  = consumes<EERecHitCollection>(conf.getParameter<edm::InputTag> ("EERecHitCollection"));
  
  minEnergyBarrel_ = conf.getParameter<double>("minEnergyBarrel");
  minEnergyEndcap_ = conf.getParameter<double>("minEnergyEndcap");
  maxEnergyBarrel_ = conf.getParameter<double>("maxEnergyBarrel");
  maxEnergyEndcap_ = conf.getParameter<double>("maxEnergyEndcap");

  minNHits_ = conf.getParameter<double>("minNHits");  
  maxChi2_ = conf.getParameter<double>("maxChi2");

  ebSimPulseShape_ = conf.getParameter<std::vector<double>>("ebSimPulseShape");
  eeSimPulseShape_ = conf.getParameter<std::vector<double>>("eeSimPulseShape");

  pedestalAnalysis_ = conf.getParameter<bool>("pedestalAnalysis");
  
  // Traslate string representation of flagsMapDBReco into enum values 
  const edm::ParameterSet & p=conf.getParameter< edm::ParameterSet >("flagsMapDBReco");
  std::vector<std::string> recoflagbitsStrings = p.getParameterNames();
  v_DB_reco_flags_.resize(32); 

  for (unsigned int i=0;i!=recoflagbitsStrings.size();++i){
    EcalRecHit::Flags recoflagbit = (EcalRecHit::Flags)
      StringToEnumValue<EcalRecHit::Flags>(recoflagbitsStrings[i]);
    std::vector<std::string> dbstatus_s =  
      p.getParameter<std::vector<std::string> >(recoflagbitsStrings[i]);
    std::vector<uint32_t> dbstatuses;
    for (unsigned int j=0; j!= dbstatus_s.size(); ++j){
      EcalChannelStatusCode::Code  dbstatus  = (EcalChannelStatusCode::Code)
        StringToEnumValue<EcalChannelStatusCode::Code>(dbstatus_s[j]);
      dbstatuses.push_back(dbstatus);
    }

    v_DB_reco_flags_[recoflagbit]=dbstatuses;
  }  


  flagmask_=0;
  flagmask_|=    0x1<<EcalRecHit::kNeighboursRecovered;
  flagmask_|=    0x1<<EcalRecHit::kTowerRecovered;
  flagmask_|=    0x1<<EcalRecHit::kDead;
  flagmask_|=    0x1<<EcalRecHit::kKilled;
  flagmask_|=    0x1<<EcalRecHit::kTPSaturated;
  flagmask_|=    0x1<<EcalRecHit::kL1SpikeFlag;   
  flagmask_|=    0x1<<EcalRecHit::kNoisy;
  flagmask_|=    0x1<<EcalRecHit::kFaultyHardware;
  

}

// ------------ method called once each job just before starting event loop  ------------
void PulseTemplates::beginJob() {

  // this is the reference template file. It is used only to put constraints on single hit timing information in a robust way
  // based on the ratio S_max / S_max-1
  int iseb;
  unsigned int detid;
  
  std::ifstream myfile;
  myfile.open("/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2015C_lowPU.txt");
  if (myfile.is_open()) {
    while ( !myfile.eof() ) {
      myfile >> iseb;
      myfile >> detid;
      std::vector<float> samples;
      samples.resize(12);
      for(int s=0; s<12; ++s) myfile >> samples[s];
      templates_ref_[detid] = samples;
    }
    myfile.close();
  }
  else std::cout << "Unable to open file";   

  // build the simTemplate (used only in case of problems)
  ebSimPulseShapeH_ = new TH1D("ebSimPulseShapeH","",15,0,15);
  eeSimPulseShapeH_ = new TH1D("eeSimPulseShapeH","",15,0,15);
  
  int SAMPLES= EcalDataFrame::MAXSAMPLES + 5; // readout samples + 5 OOT 
  int firstSignalSample=3;
  for(int i=0; i<SAMPLES; ++i) {
    if(i<firstSignalSample) {
      ebSimPulseShapeH_->SetBinContent(i+1, 0.);
      ebSimPulseShapeH_->SetBinError(i+1, 0.);
      ebSimPulseShapeH_->SetBinContent(i+1, 0.);
      ebSimPulseShapeH_->SetBinError(i+1, 0.);
    }
    else {
      ebSimPulseShapeH_->SetBinContent(i+1, ebSimPulseShape_[i-firstSignalSample]);
      ebSimPulseShapeH_->SetBinError(i+1, 0.);
      eeSimPulseShapeH_->SetBinContent(i+1, eeSimPulseShape_[i-firstSignalSample]);
      eeSimPulseShapeH_->SetBinError(i+1, 0.);
    }
  }

}

void PulseTemplates::beginRun( const edm::Run & iRun, const edm::EventSetup & iSetup ) { }

void PulseTemplates::endRun( const edm::Run & iRun, const edm::EventSetup & iSetup ) {



  /// output
  TString nameoutput = Form("template_histograms_ECAL_run_%d.root",iRun.run());
  TFile *outfile = TFile::Open(nameoutput.Data(),"recreate");
  //TH1D *htempl = new TH1D("templates","",10,0,10);

  // text file to fill the DB objects with templates
  std::ofstream txtdumpfile;  
  txtdumpfile.open (nameoutput.Data(), std::ios::out | std::ios::trunc);

  for(std::map<int, std::vector<double> >::iterator it=templates_.begin(); it!=templates_.end(); ++it) {
    unsigned int rawId = rawIds_[it->first];
    DetId detid(rawId);
    barrel_ = detid.subdetId()==EcalBarrel;  

    txtdumpfile.unsetf ( std::ios::floatfield ); 
    txtdumpfile << ( barrel_ ? 1 : 0 ) << "\t";
    txtdumpfile << iXs_[it->first] << "\t" << iYs_[it->first] << "\t" << iZs_[it->first] << "\t" << rawId << "\t";
    txtdumpfile.precision(6);
    txtdumpfile.setf( std::ios::fixed, std::ios::floatfield ); // floatfield set to fixed
    
    int ix = it->first / 1000;
    if(barrel_ && ix >= 85) ix = -1*(ix-85);
    if((!barrel_) && ix >= 100) ix = -1*(ix-100);
    
    int iy = it->first % 1000;
    std::cout << "Writing templates histogram. ix = " << ix << "  iy = " << iy << " got " << norm_counts_[it->first] << " events" << std::endl;

    float pdfval[12];

    TH1D *simTemplate = barrel_ ? ebSimPulseShapeH_ : eeSimPulseShapeH_;
    for(int iSample(3); iSample < 10; iSample++) {
      pdfval[iSample-3] = (it->second)[iSample] / norm_average_[it->first];
      if(norm_counts_[it->first]<minNHits_) pdfval[iSample-3] = simTemplate->GetBinContent(iSample+1); // PROTECTION !!!
      //      txtdumpfile << pdfval << "\t";
    }

    // if(nCry%1000==0) std::cout << "Fitting the crystal # " << nCry << " with rawId = " << rawId << std::endl;
    // TH1D *fittedh = fitTemplate(th,dobarrel,0,simTemplate);
    // for(int iExtraSample(0); iExtraSample < 5; iExtraSample++) {
    //    // if(norm_average[it->first]>2) txtdumpfile << valExtrap[iExtraSample] << "\t";
    //    // else txtdumpfile << simTemplate->GetBinContent(iExtraSample+11) << "\t";
    //   if(norm_counts[it->first]>=minNHits) pdfval[7+iExtraSample] = valExtrap[iExtraSample];
    //   else pdfval[7+iExtraSample] = simTemplate->GetBinContent(iExtraSample+11);
    // }

    /*
    // PROTECTION AGAINST STRANGE SHAPES
    if(fabs(pdfval[4]-simTemplate->GetBinContent(8))>0.1 || pdfval[0]==0) {
      //      std::cout << pdfval[4] << "   " << simTemplate->GetBinContent(8) << " ==> replacing..." << std::endl;
      for(int s=0; s<12; ++s) { 
	pdfval[s] = simTemplate->GetBinContent(s+4);
      }
    }
    */

    for(int s=0;s<12;++s) txtdumpfile << pdfval[s] << "\t";

    //delete fittedh;

    txtdumpfile << std::endl;

    //th->Write();
    
  }
  //  std::cout << "Writing average template histogram (averaged over " << nCry << " crystals" << std::endl;
  //  htemplAverage->Write();
  
  outfile->Close();
  txtdumpfile.close();

}



// ------------ method called once each job just after ending the event loop  ------------
void  PulseTemplates::endJob() {

  delete ebSimPulseShapeH_;
  delete eeSimPulseShapeH_;

}


void PulseTemplates::analyze(const edm::Event& e, const edm::EventSetup& es) {
  
  run_ = e.id().run();
  lumi_ = e.luminosityBlock();

  es.get<EcalGainRatiosRcd>().get(gains);
  es.get<EcalPedestalsRcd>().get(peds); 
  es.get<EcalChannelStatusRcd>().get(chStatus);
  
  edm::Handle< EBDigiCollection > pEBDigis;
  edm::Handle< EEDigiCollection > pEEDigis;  
  
  e.getByToken( ebDigiCollectionToken_, pEBDigis);
  e.getByToken( eeDigiCollectionToken_, pEEDigis);
  
  edm::Handle<EcalRecHitCollection> EBHits;
  edm::Handle<EcalRecHitCollection> EEHits;

  e.getByToken(ebhitcollToken_,EBHits);
  e.getByToken(eehitcollToken_,EEHits);

  for(EBDigiCollection::const_iterator itdg = pEBDigis->begin(); itdg != pEBDigis->end(); ++itdg) {
    EcalRecHitCollection::const_iterator rhItrEB= EBHits->begin();
    while(rhItrEB != EBHits->end() && rhItrEB->id() != itdg->id()) rhItrEB++;
    if(rhItrEB != EBHits->end())    
      FillRecHit(*itdg,*rhItrEB);
  }

  for(EEDigiCollection::const_iterator itdg = pEEDigis->begin(); itdg != pEEDigis->end(); ++itdg) {
    EcalRecHitCollection::const_iterator rhItrEE= EEHits->begin();
    while(rhItrEE != EEHits->end() && rhItrEE->id() != itdg->id()) rhItrEE++;
    if(rhItrEE != EEHits->end())    
      FillRecHit(*itdg,*rhItrEE);
  }

  
}

void PulseTemplates::FillRecHit(const EcalDataFrame& dataFrame, const EcalRecHit& rechit) {
   
  const unsigned int nsample = EcalDataFrame::MAXSAMPLES;
  
  double maxamplitude = -std::numeric_limits<double>::max();

  double pedval = 0.;
  double pedrms = 0.;
  
  unsigned int gain = 12;
  
  
  const EcalPedestals::Item * aped = 0;
  const EcalMGPAGainRatio * aGain = 0;
  
  DetId detid = dataFrame.id();
  barrel_ = detid.subdetId()==EcalBarrel;  
  
  EcalChannelStatusMap::const_iterator chit = chStatus->find(detid);
  EcalChannelStatusCode::Code  dbstatus = chit->getStatusCode();  
  
  // check for channels to be excluded from reconstruction
  if ( v_chstatus_.size() > 0) {
      
      std::vector<int>::const_iterator res = 
                  std::find( v_chstatus_.begin(), v_chstatus_.end(), dbstatus );
      if ( res != v_chstatus_.end() ) return;
      
  }
  
  uint32_t flagBits = setFlagBits(v_DB_reco_flags_, dbstatus);  
  if (flagmask_ & flagBits) return;
  
  if (!barrel_) {
          unsigned int hashedIndex = EEDetId(detid).hashedIndex();
          aped  = &peds->endcap(hashedIndex);
          aGain = &gains->endcap(hashedIndex);
  } else {
          unsigned int hashedIndex = EBDetId(detid).hashedIndex();
          aped  = &peds->barrel(hashedIndex);
          aGain = &gains->barrel(hashedIndex);
  }

  // dynamic pedestal
  double dyn_pedestal = 0.;
  for(unsigned int iSample = 0; iSample < 3; iSample++) {
    const EcalMGPASample &sample = dataFrame.sample(iSample);
    dyn_pedestal += (double)(sample.adc())/3.;
  }

  for(unsigned int iSample = 0; iSample < nsample; iSample++) {
    
    const EcalMGPASample &sample = dataFrame.sample(iSample);
    
    double amplitude = 0.;
    int gainId = sample.gainId();
    
    double pedestal = 0.;
    double pederr = 0.;
    double gainratio = 1.;
        
    if (gainId==0 || gainId==3) {
      pedestal = aped->mean_x1;
      pederr = aped->rms_x1;
      gainratio = aGain->gain6Over1()*aGain->gain12Over6();
      gain = 1;
    }
    else if (gainId==1) {
      pedestal = aped->mean_x12;
      pederr = aped->rms_x12;
      gainratio = 1.;
      gain = 12;
    }
    else if (gainId==2) {
      pedestal = aped->mean_x6;
      pederr = aped->rms_x6;
      gainratio = aGain->gain12Over6();
      gain = 6;
    }

    // overwrite the pedestal from DB with the pedestal from pre-samples
    //    std::cout << "db ped = " << pedestal << "\tdyn pedestal = " << dyn_pedestal << std::endl;
    pedestal = dyn_pedestal;
    if(!pedestalAnalysis_) amplitude = ((double)(sample.adc()) - pedestal) * gainratio;
    else amplitude = (double)(sample.adc());

    if (gainId == 0) {
      //saturation
      gain = 0;
      if(!pedestalAnalysis_) amplitude = (4095. - pedestal) * gainratio;
    }
        
    pulse_[iSample] = amplitude;
    
    if (amplitude>maxamplitude) {
    //if (iSample==5) {
      maxamplitude = amplitude;
      pedval = pedestal;
      pedrms = pederr*gainratio;
    }
        
  }
  
  
  gain_ = gain;
  pedrms_ = pedrms;
  pedval_ = pedval;
  
  double peaksig = pulse_[5]/pedrms_;
  
  if (!pedestalAnalysis_ && peaksig<10.) return;

  double minEnergy = barrel_ ? minEnergyBarrel_ : minEnergyEndcap_;
  double maxEnergy = barrel_ ? maxEnergyBarrel_ : maxEnergyEndcap_;
  if(!pedestalAnalysis_ && pulse_[5] < minEnergy) return;
  
  if (barrel_) {
    EBDetId ebid(detid);
    ietaix_ = ebid.ieta();
    iphiiy_ = ebid.iphi();
    iz_ = 0;
    rawid_ = ebid.rawId();
  }
  else {
    EEDetId eeid(detid);
    ietaix_ = eeid.ix();
    iphiiy_ = eeid.iy();
    iz_ = eeid.zside();  
    rawid_ = eeid.rawId();
  }
  
  // associated rechit quantities
  ene_     = rechit.energy();
  time_    = rechit.time();
  chi2_    = rechit.chi2();
  
  flag_kweird_=0;
  flag_kdiweird_=0;
  if (rechit.checkFlag(EcalRecHit::kWeird)) flag_kweird_=1;
  if (rechit.checkFlag(EcalRecHit::kDiWeird)) flag_kdiweird_=1;


  // check the max deviation wrt the reference tag (exclude the last ones which can fluctuate)
  double max_dev_ref=0;
  for(unsigned int iSample = 4; iSample < nsample-2; ++iSample) {
    if(templates_ref_.count(rawid_)>0) {
      double templVal = (templates_ref_[rawid_])[iSample-3];
      double dev = fabs(pulse_[iSample]/maxamplitude - templVal)/templVal;
      if(dev > max_dev_ref) max_dev_ref = dev;
    } else break;
  }


  double weight = pow(ene_,2);

  int offset;
  if(barrel_) offset = (ietaix_ > 0) ? 1000 * ietaix_ : 1000 * (abs(ietaix_)+85);
  else offset = (iz_ > 0) ? 1000 * ietaix_ : 1000 * (ietaix_+100);
  int ic = offset + iphiiy_;

  if (ene_ > minEnergy && ene_ < maxEnergy && (!flag_kweird_) && (!flag_kdiweird_)) {
  
    if(templates_.count(ic)==0) {
      std::vector<double> templ;
      templ.resize(10);
      for(int iSample(0); iSample < 10; iSample++) templ[iSample] = pulse_[iSample]/maxamplitude * weight;
      templates_[ic] = templ;
      norm_average_[ic] = weight;
      norm_counts_[ic] = 1;
      // std::cout << "inserting new ped for DetId = " << ic << std::endl;
      rawIds_[ic] = rawid_;
      iXs_[ic] = ietaix_;       iYs_[ic] = iphiiy_;        iZs_[ic] = iz_;
    } else {
      std::vector<double> &templ = templates_[ic];
      for(int iSample(0); iSample < 10; iSample++) templ[iSample] += pulse_[iSample]/maxamplitude * weight;
      norm_average_[ic] += weight;
      norm_counts_[ic] ++;
      //        std::cout << "updating ped for DetId = " << ic << std::endl;
    }
  }  
  
}

// Take our association map of dbstatuses-> recHit flagbits and return the apporpriate flagbit word
uint32_t PulseTemplates::setFlagBits(const std::vector<std::vector<uint32_t> >& map, 
                                             const uint32_t& status  ){
  
  for (unsigned int i = 0; i!=map.size(); ++i){
    if (std::find(map[i].begin(), map[i].end(),status)!= map[i].end()) 
      return 0x1 << i;
  }

  return 0;
}
