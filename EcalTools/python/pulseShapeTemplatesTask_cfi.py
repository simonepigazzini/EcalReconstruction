import FWCore.ParameterSet.Config as cms
pulseShapeTemplatesTask = cms.EDAnalyzer("PulseShapeTemplatesTask",
                              EBdigiCollection = cms.InputTag("ecalDigis","ebDigis"),
                              EEdigiCollection = cms.InputTag("ecalDigis","eeDigis"),
                              pedestalMeanFile = cms.string("pedestal_means_barrel.root"),
                              templatesFile = cms.string("templates.root"),
                              calcPedMean = cms.bool(False),
                              doBarrel = cms.bool(True),
                              feds = cms.vstring("EB+13"),
                              gain = cms.int32(12)
                              )
