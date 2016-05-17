import FWCore.ParameterSet.Config as cms
rechitDumper = cms.EDAnalyzer("RecHitDumper",
                              EBRecHits1 = cms.InputTag("ecalRecHit:EcalRecHitsEB"),
                              EERecHits1 = cms.InputTag("ecalRecHit:EcalRecHitsEE"),
                              EBRecHits2 = cms.InputTag(""),
                              EERecHits2 = cms.InputTag(""),
                              isMC = cms.untracked.bool(True),
                              fileName = cms.untracked.string('tree.root'),
                              nameTree = cms.untracked.string('ntp0')
                              )


