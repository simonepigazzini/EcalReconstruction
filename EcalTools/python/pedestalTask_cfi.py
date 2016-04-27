import FWCore.ParameterSet.Config as cms
pedestalTask = cms.EDAnalyzer("PedestalTask",
                              EBdigiCollection = cms.InputTag("ecalDigis","ebDigis"),
                              EEdigiCollection = cms.InputTag("ecalDigis","eeDigis"),
                              pedestalMeanFile = cms.string("pedestal_means_barrel.root"),
                              covariancesFile = cms.string("covariances.root"),
                              calcPedMean = cms.bool(False),
                              doBarrel = cms.bool(True),
                              feds = cms.vstring("EB+13"),
                              gain = cms.int32(12)
                              )
