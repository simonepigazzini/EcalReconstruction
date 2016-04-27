import FWCore.ParameterSet.Config as cms

# configure here #
isMC = True
##################

process = cms.Process("ECALRecHitAnalysis")

process.load('Configuration.StandardSequences.Services_cff')
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load('Configuration.Geometry.GeometryExtended2017Reco_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

if isMC:
    process.GlobalTag.globaltag = 'START71_V1::All'

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.source = cms.Source("PoolSource",
                            noEventSort = cms.untracked.bool(True),
                            duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
                            fileNames = cms.untracked.vstring('file:testEcalLocalRecoA.root')
                            )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load("EcalReconstruction.EcalTools.rechit_dumper_cfi")
process.rechitDumper.EBRecHits1 = "ecalRecHitGlobal:EcalRecHitsGlobalEB"
process.rechitDumper.EERecHits1 = "ecalRecHitGlobal:EcalRecHitsGlobalEE"
process.rechitDumper.EBRecHits2 = "ecalRecHitWeights:EcalRecHitsWeightsEB"
process.rechitDumper.EERecHits2 = "ecalRecHitWeights:EcalRecHitsWeightsEE"
process.rechitDumper.fileName = cms.untracked.string('tree_ecalhits.root')

process.mypath = cms.Path(process.rechitDumper)
