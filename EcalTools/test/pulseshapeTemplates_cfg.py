import FWCore.ParameterSet.Config as cms
import re
process = cms.Process("PEDS")

######## configure here #######
calcPedestalMean = False
run = 211831
barrel = True
FEDused = "EB+13"
Gain = 12
###############################

templateOutputFile = str("templates_run"+str(run)+"_"+FEDused+"_Gain"+str(Gain)+".root")
templateOutputFile = re.sub("\+","p", templateOutputFile)
templateOutputFile = re.sub("\-","m", templateOutputFile)

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.Geometry.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.GlobalTag.globaltag = '74X_dataRun2_Prompt_v1'

process.load('EcalReconstruction.EcalTools.pulseDump_cfi')

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('/store/data/Run2015C/AlCaP0/RAW/v1/000/255/029/00000/1E2825A6-934B-E511-AB43-02163E01348B.root')
                            )

process.TFileService = cms.Service("TFileService", fileName = cms.string("templates.root") )

# start from RAW format for more flexibility
#process.raw2digi_step = cms.Sequence(process.RawToDigi)

#DUMMY RECHIT
process.dummyHits = cms.EDProducer('DummyRechitDigis',
                                     doDigi = cms.untracked.bool(True),
                                     # rechits
                                     barrelHitProducer      = cms.InputTag('hltAlCaPi0EBUncalibrator','pi0EcalRecHitsEB' ,'HLT'),
                                     endcapHitProducer      = cms.InputTag('hltAlCaPi0EEUncalibrator','pi0EcalRecHitsEE' ,'HLT'),
                                     barrelRecHitCollection = cms.untracked.string('dummyBarrelRechits'),
                                     endcapRecHitCollection = cms.untracked.string('dummyEndcapRechits'),
                                     # digis
                                     barrelDigis            = cms.InputTag("hltAlCaPi0EBRechitsToDigis","pi0EBDigis","HLT"),
                                     endcapDigis            = cms.InputTag("hltAlCaPi0EERechitsToDigisLowPU","pi0EEDigis","HLT"),
                                     barrelDigiCollection   = cms.untracked.string('dummyBarrelDigis'),
                                     endcapDigiCollection   = cms.untracked.string('dummyEndcapDigis'))


process.p = cms.Path(process.dummyHits * 
                     process.pulseDump )
