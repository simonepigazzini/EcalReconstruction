import FWCore.ParameterSet.Config as cms
import re
process = cms.Process("PEDS")

######## configure here #######
calcPedestalMean = False
run = 211831
barrel = True
FEDused = "EB+13"
Gain = 12
LoneBunch = False
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

process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(5000) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v5'

process.load('EcalReconstruction.EcalTools.pulseDump_cfi')

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('/store/data/Run2016A/AlCaPhiSym/RAW/v1/000/270/988/00000/245EF7D0-8808-E611-9B68-02163E0143E1.root')
                            )

process.TFileService = cms.Service("TFileService", fileName = cms.string("templates.root") )

# start from RAW format for more flexibility
#process.raw2digi_step = cms.Sequence(process.RawToDigi)

# reco
process.load('RecoLocalCalo.Configuration.ecalLocalRecoSequence_cff')
process.ecalLocalRecoSequenceAlCaStream = cms.Sequence (process.ecalMultiFitUncalibRecHit * process.ecalRecHit)
process.ecalMultiFitUncalibRecHit.EBdigiCollection = cms.InputTag("hltEcalPhiSymFilter","phiSymEcalDigisEB")
process.ecalMultiFitUncalibRecHit.EEdigiCollection = cms.InputTag("hltEcalPhiSymFilter","phiSymEcalDigisEE")
process.ecalRecHit.killDeadChannels = False
process.ecalRecHit.recoverEBFE = False
process.ecalRecHit.recoverEEFE = False


# TRIGGER RESULTS FILTER
process.triggerSelectionLoneBunch = cms.EDFilter( "TriggerResultsFilter",
                                                  triggerConditions = cms.vstring('L1_AlwaysTrue'),
                                                  hltResults = cms.InputTag( "TriggerResults", "", "HLT" ),
                                                  l1tResults = cms.InputTag( "hltGtDigis" ),
                                                  l1tIgnoreMask = cms.bool( False ),
                                                  l1techIgnorePrescales = cms.bool( False ),
                                                  daqPartitions = cms.uint32( 1 ),
                                                  throw = cms.bool( True )
                                                  )
if LoneBunch:
    process.p = cms.Path(process.triggerSelectionLoneBunch * 
                         process.bunchSpacingProducer *
                         process.ecalLocalRecoSequenceAlCaStream *
                         process.pulseDump )
else:
    process.p = cms.Path(process.bunchSpacingProducer *
                         process.ecalLocalRecoSequenceAlCaStream *
                         process.pulseDump )
