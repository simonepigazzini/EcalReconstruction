import FWCore.ParameterSet.Config as cms

boostedElectrons = cms.EDProducer("PatElectronBooster", 
                                  src = cms.InputTag("patElectrons"), 
                                  vertices = cms.InputTag("offlinePrimaryVertices"),
                                  doTrigMVA = cms.bool(True),
                                  ebGlobalRecHitCollection = cms.InputTag("ecalRecHitGlobal","EcalRecHitsGlobalEB","reRECO2"),
                                  eeGlobalRecHitCollection = cms.InputTag("ecalRecHitGlobal","EcalRecHitsGlobalEE","reRECO2"),
                                  ebMultifit50nsRecHitCollection = cms.InputTag("ecalRecHitMultiFit50ns","EcalRecHitsMultiFit50nsEB","reRECO2"),
                                  eeMultifit50nsRecHitCollection = cms.InputTag("ecalRecHitMultiFit50ns","EcalRecHitsMultiFit50nsEE","reRECO2"),
                                  ebMultifit25nsRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB","reRECO2"),
                                  eeMultifit25nsRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE","reRECO2"),
                                  ebMultifitNoOOTPURecHitCollection = cms.InputTag("ecalRecHitMultiFitNoOOTPU","EcalRecHitsMultiFitNoOOTPUEB","reRECO2"),
                                  eeMultifitNoOOTPURecHitCollection = cms.InputTag("ecalRecHitMultiFitNoOOTPU","EcalRecHitsMultiFitNoOOTPUEE","reRECO2")
                                  )
