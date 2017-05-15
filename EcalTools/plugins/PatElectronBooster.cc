// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <DataFormats/PatCandidates/interface/Electron.h>
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

//BDT ElectronID
//#include "RecoEgamma/ElectronIdentification/interface/EGammaMvaEleEstimator.h"

// cluster shapes noZS
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"

#include <vector>
#include <string>

#include "Math/VectorUtil.h"
#include "DataFormats/Math/interface/deltaPhi.h"


//
// class declaration
//

class PatElectronBooster : public edm::EDProducer {
    public:
        explicit PatElectronBooster(const edm::ParameterSet&);
        ~PatElectronBooster();

    private:
        virtual void beginJob() ;
        virtual void produce(edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;

        // ----------member data ---------------------------
        edm::InputTag electronTag_;
        edm::InputTag vertices_;
        edm::InputTag ecalRHGlobalEBTag_,ecalRHGlobalEETag_;
        edm::InputTag ecalRHMultifit50nsEBTag_,ecalRHMultifit50nsEETag_;
        edm::InputTag ecalRHMultifit25nsEBTag_,ecalRHMultifit25nsEETag_;
        edm::InputTag ecalRHMultifitNoOOTPUEBTag_,ecalRHMultifitNoOOTPUEETag_;
        // EGammaMvaEleEstimator* eleMVATrig;
        // EGammaMvaEleEstimator* eleMVANonTrig;
        std::vector<std::string> manualCatTrigWeigths;
        std::vector<std::string> manualCatNonTrigWeigths;
        std::string postfix_;
        bool doTrig_, doNonTrig_;
        bool debug_;

        // ------- tools -------------------
        noZS::EcalClusterLazyTools *lazyToolnoZS_Global, *lazyToolnoZS_Multifit50ns, *lazyToolnoZS_Multifit25ns, *lazyToolnoZS_MultifitNoOOTPU;
        edm::EDGetTokenT<EcalRecHitCollection> ebGlobalRecHitCollection_;
        edm::EDGetTokenT<EcalRecHitCollection> eeGlobalRecHitCollection_;
        edm::EDGetTokenT<EcalRecHitCollection> ebMultifit50nsRecHitCollection_;
        edm::EDGetTokenT<EcalRecHitCollection> eeMultifit50nsRecHitCollection_;
        edm::EDGetTokenT<EcalRecHitCollection> ebMultifit25nsRecHitCollection_;
        edm::EDGetTokenT<EcalRecHitCollection> eeMultifit25nsRecHitCollection_;
        edm::EDGetTokenT<EcalRecHitCollection> ebMultifitNoOOTPURecHitCollection_;
        edm::EDGetTokenT<EcalRecHitCollection> eeMultifitNoOOTPURecHitCollection_;
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
PatElectronBooster::PatElectronBooster(const edm::ParameterSet& iConfig) :
        electronTag_(iConfig.getParameter<edm::InputTag>("src")),
        vertices_(iConfig.getParameter<edm::InputTag>("vertices")),
        postfix_(iConfig.existsAs<std::string>("postfix") ? iConfig.getParameter<std::string>("postfix") : ""),
        doTrig_(iConfig.existsAs<bool>("doTrigMVA") ? iConfig.getParameter<bool>("doTrigMVA") : true),
        doNonTrig_(iConfig.existsAs<bool>("doNonTrigMVA") ? iConfig.getParameter<bool>("doNonTrigMVA") : true),
        debug_(iConfig.getUntrackedParameter<bool>("verbose",false))
{
  produces<pat::ElectronCollection>();  

  ebGlobalRecHitCollection_ = consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("ebGlobalRecHitCollection"));
  eeGlobalRecHitCollection_ = consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("eeGlobalRecHitCollection"));
  ebMultifit50nsRecHitCollection_ = consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("ebMultifit50nsRecHitCollection"));
  eeMultifit50nsRecHitCollection_ = consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("eeMultifit50nsRecHitCollection"));
  ebMultifit25nsRecHitCollection_ = consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("ebMultifit25nsRecHitCollection"));
  eeMultifit25nsRecHitCollection_ = consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("eeMultifit25nsRecHitCollection"));
  ebMultifitNoOOTPURecHitCollection_ = consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("ebMultifitNoOOTPURecHitCollection"));
  eeMultifitNoOOTPURecHitCollection_ = consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("eeMultifitNoOOTPURecHitCollection"));

  // ---- Here I initialize the BDT for the mva ElectronID

  std::string baseFolder("/afs/cern.ch/cms/data/CMSSW/RecoEgamma/ElectronIdentification/data/");
  manualCatTrigWeigths.push_back(baseFolder+"/Electrons_BDTG_TrigV0_Cat1.weights.xml");
  manualCatTrigWeigths.push_back(baseFolder+"/Electrons_BDTG_TrigV0_Cat2.weights.xml");
  manualCatTrigWeigths.push_back(baseFolder+"/Electrons_BDTG_TrigV0_Cat3.weights.xml");
  manualCatTrigWeigths.push_back(baseFolder+"/Electrons_BDTG_TrigV0_Cat4.weights.xml");
  manualCatTrigWeigths.push_back(baseFolder+"/Electrons_BDTG_TrigV0_Cat5.weights.xml");
  manualCatTrigWeigths.push_back(baseFolder+"/Electrons_BDTG_TrigV0_Cat6.weights.xml");  

  manualCatNonTrigWeigths.push_back(baseFolder+"/Electrons_BDTG_NonTrigV0_Cat1.weights.xml");
  manualCatNonTrigWeigths.push_back(baseFolder+"/Electrons_BDTG_NonTrigV0_Cat2.weights.xml");
  manualCatNonTrigWeigths.push_back(baseFolder+"/Electrons_BDTG_NonTrigV0_Cat3.weights.xml");
  manualCatNonTrigWeigths.push_back(baseFolder+"/Electrons_BDTG_NonTrigV0_Cat4.weights.xml");
  manualCatNonTrigWeigths.push_back(baseFolder+"/Electrons_BDTG_NonTrigV0_Cat5.weights.xml");
  manualCatNonTrigWeigths.push_back(baseFolder+"/Electrons_BDTG_NonTrigV0_Cat6.weights.xml");  

  // if (doTrig_) {
  //     eleMVATrig  = new EGammaMvaEleEstimator();
  //     eleMVATrig->initialize("BDT",EGammaMvaEleEstimator::kTrig,true,manualCatTrigWeigths);
  // }
  // if (doNonTrig_) {
  //     eleMVANonTrig  = new EGammaMvaEleEstimator();
  //     eleMVANonTrig->initialize("BDT",EGammaMvaEleEstimator::kNonTrig,true,manualCatNonTrigWeigths);
  // }
  
  //  eleMVATrig->SetPrintMVADebug(kTRUE);
//   eleMVANonTrig->SetPrintMVADebug(kTRUE);


  // ---------
}


PatElectronBooster::~PatElectronBooster() {
  // delete eleMVATrig;
  // delete eleMVANonTrig;
}


void PatElectronBooster::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    using namespace edm;
    using namespace std;

    edm::Handle<edm::View<reco::Candidate> > electrons;
    iEvent.getByLabel(electronTag_,electrons);

    edm::Handle<reco::VertexCollection> vertices; 
    iEvent.getByLabel(vertices_, vertices);

    edm::Handle< EcalRecHitCollection > pEBRecHits;
    iEvent.getByToken( ebMultifit50nsRecHitCollection_, pEBRecHits );
    const EcalRecHitCollection *ebRecHits_ = pEBRecHits.product();

    edm::Handle< EcalRecHitCollection > pEERecHits;
    iEvent.getByToken( eeMultifit50nsRecHitCollection_, pEERecHits );
    const EcalRecHitCollection *eeRecHits_ = pEERecHits.product();

    std::unique_ptr<pat::ElectronCollection> pOut(new pat::ElectronCollection);

    lazyToolnoZS_Global = new noZS::EcalClusterLazyTools(iEvent, iSetup, ebGlobalRecHitCollection_, eeGlobalRecHitCollection_);
    lazyToolnoZS_Multifit50ns = new noZS::EcalClusterLazyTools(iEvent, iSetup, ebMultifit50nsRecHitCollection_, eeMultifit50nsRecHitCollection_);
    lazyToolnoZS_Multifit25ns = new noZS::EcalClusterLazyTools(iEvent, iSetup, ebMultifit25nsRecHitCollection_, eeMultifit25nsRecHitCollection_);
    lazyToolnoZS_MultifitNoOOTPU = new noZS::EcalClusterLazyTools(iEvent, iSetup, ebMultifitNoOOTPURecHitCollection_, eeMultifitNoOOTPURecHitCollection_);

    // ----- here is the real loop over the electrons ----
    for(edm::View<reco::Candidate>::const_iterator ele=electrons->begin(); ele!=electrons->end(); ++ele){    
      const pat::ElectronRef elecsRef = edm::RefToBase<reco::Candidate>(electrons,ele-electrons->begin()).castTo<pat::ElectronRef>();
      pat::Electron clone = *edm::RefToBase<reco::Candidate>(electrons,ele-electrons->begin()).castTo<pat::ElectronRef>();

      // ------ HERE I ADD THE BDT ELE ID VALUE TO THE ELECTRONS
      //      double xieSign  = ( (-clone.userFloat("dxyPV")) >=0 )  ? 1: -1;
      //      float ctfChi2 = clone.userFloat("ctfChi2"); if (ctfChi2 == -1) ctfChi2 = 0; // fix a posteriori problem in booster

      if (doTrig_) {
        double mvaValueTrig = -999; // disable to compile the package in 80X
        // eleMVATrig->mvaValue(
        //              clone.fbrem(),
        //              ctfChi2,
        //              clone.userInt("ctfHits"),   
        //              clone.gsfTrack()->normalizedChi2(),
	// 				 clone.deltaEtaSuperClusterTrackAtVtx(),
	// 				 clone.deltaPhiSuperClusterTrackAtVtx(),
	// 				 clone.deltaEtaSeedClusterTrackAtCalo(),
	// 				 clone.sigmaIetaIeta(), 
	// 				 clone.userFloat("sigmaIphiIphi"),
        //              clone.userFloat("etaWidth"), 
        //              clone.userFloat("phiWidth"), 
	// 				 (clone.e5x5()) !=0. ? 1.-(clone.e1x5()/clone.e5x5()) : -1.,
        //              clone.userFloat("e3x3")/(clone.superCluster()->rawEnergy()),
	// 				 clone.hcalOverEcal(),
	// 				 clone.eSuperClusterOverP(), 
        //              (1.0 / clone.ecalEnergy()) - (1.0 / clone.p()),   
        //              clone.eEleClusterOverPout(),
        //              clone.superCluster()->preshowerEnergy()/clone.superCluster()->rawEnergy(),          
	// 				 -clone.userFloat("dxyPV"),
	// 				 xieSign*clone.userFloat("ip"),
        //              clone.superCluster()->eta(), 
        //              clone.pt(), 
        //              debug_);	
      clone.addUserFloat(std::string("bdttrig")+postfix_,mvaValueTrig);
      }

      if (doNonTrig_) {
        double mvaValueNonTrig = -999;
        // eleMVANonTrig->mvaValue(
        //              clone.fbrem(),
        //              ctfChi2,
        //              clone.userInt("ctfHits"),   
        //              clone.gsfTrack()->normalizedChi2(),
        //              clone.deltaEtaSuperClusterTrackAtVtx(),
        //              clone.deltaPhiSuperClusterTrackAtVtx(),
        //              clone.deltaEtaSeedClusterTrackAtCalo(),
        //              clone.sigmaIetaIeta(),
        //              clone.userFloat("sigmaIphiIphi"),
        //              clone.userFloat("etaWidth"),
        //              clone.userFloat("phiWidth"),
	// 				 (clone.e5x5()) !=0. ? 1.-(clone.e1x5()/clone.e5x5()) : -1.,
        //              clone.userFloat("e3x3")/(clone.superCluster()->rawEnergy()),
        //              clone.hcalOverEcal(),
        //              clone.eSuperClusterOverP(), 
        //              (1.0 / clone.ecalEnergy()) - (1.0 / clone.p()),
        //              clone.eEleClusterOverPout(),
        //              clone.superCluster()->preshowerEnergy()/clone.superCluster()->rawEnergy(),
        //              clone.superCluster()->eta(),
        //              clone.pt(),
        //              debug_);
      clone.addUserFloat(std::string("bdtnontrig"+postfix_),mvaValueNonTrig);
      }

      // -----------------------------

      // ------ HERE I ADD THE OTHER 5x5 CLUSTER ENERGIES TO THE ELECTRONS
      const auto& theseed = *(clone.superCluster()->seed());
      float r9_global = lazyToolnoZS_Global->e3x3( theseed ) / clone.superCluster()->rawEnergy();
      float e5x5_global = lazyToolnoZS_Global->e5x5( theseed );
      float e5x5_multifit50ns = lazyToolnoZS_Multifit50ns->e5x5( theseed );
      float e5x5_multifit25ns = lazyToolnoZS_Multifit25ns->e5x5( theseed );
      float e5x5_multifitNoOOTPU = lazyToolnoZS_MultifitNoOOTPU->e5x5( theseed );      

      clone.addUserFloat(std::string("r9global"+postfix_),r9_global);
      clone.addUserFloat(std::string("e5x5global"+postfix_),e5x5_global);
      clone.addUserFloat(std::string("e5x5mf50ns"+postfix_),e5x5_multifit50ns);
      clone.addUserFloat(std::string("e5x5mf25ns"+postfix_),e5x5_multifit25ns);
      clone.addUserFloat(std::string("e5x5mf0pu"+postfix_),e5x5_multifitNoOOTPU);

      DetId seed_id = (theseed.hitsAndFractions()[0]).first;
      const EcalRecHitCollection *recHits = 0;
      if ( seed_id.subdetId() == EcalBarrel ) {
        recHits = ebRecHits_;
      } else if ( seed_id.subdetId() == EcalEndcap ) {
        recHits = eeRecHits_;
      } else {
        throw cms::Exception("InvalidSubdetector") << "The subdetId() " << seed_id.subdetId() << " does not correspond to EcalBarrel neither EcalEndcap";
      }

      EcalRecHitCollection::const_iterator theSeedHit = recHits->find (seed_id);
      clone.addUserFloat(std::string("seedflag"+postfix_),theSeedHit->recoFlag());
      clone.addUserFloat(std::string("seedchi2"+postfix_),theSeedHit->chi2());
      clone.addUserFloat(std::string("seedeerr"+postfix_),theSeedHit->energyError());
      // -----------------------------

      // ------ HERE I ADD SOME EVENT VARIABLE (needed here because I use the ProbeTreeProducer) -------
      clone.addUserFloat(std::string("vertices"+postfix_),float(vertices->size()));
      // -----------------------------      

      pOut->push_back(clone);
	
    }
    iEvent.put(std::move(pOut));
    delete lazyToolnoZS_Global;
    delete lazyToolnoZS_Multifit50ns;
    delete lazyToolnoZS_Multifit25ns;
    delete lazyToolnoZS_MultifitNoOOTPU;
}

// ------------ method called once each job just before starting event loop  ------------
void PatElectronBooster::beginJob() { }

// ------------ method called once each job just after ending the event loop  ------------
void PatElectronBooster::endJob() { }


//define this as a plug-in
DEFINE_FWK_MODULE(PatElectronBooster);
