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

process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.suppressWarning = cms.untracked.vstring( "triggerSelectionLoneBunch" )

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag as customiseGlobalTag
#process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = 'auto:run2_hlt_GRun')
process.GlobalTag.globaltag = "80X_dataRun2_HLT_v12"

process.load('EcalReconstruction.EcalTools.pulseDump_cfi')

process.source = cms.Source("PoolSource",
                              fileNames = cms.untracked.vstring('root://xrootd-cms.infn.it//store/data/Commissioning2017/AlCaPhiSym/RAW/v1/000/293/765/00000/007A9FAE-8B36-E711-B50D-02163E0144FE.root')
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
process.StableParametersRcdSource = cms.ESSource( "EmptyESSource",
    iovIsRunNotTime = cms.bool( True ),
    recordName = cms.string( "L1TGlobalStableParametersRcd" ),
    firstValid = cms.vuint32( 1 )
)

process.GlobalParametersRcdSource = cms.ESSource( "EmptyESSource",
    iovIsRunNotTime = cms.bool( True ),
    recordName = cms.string( "L1TGlobalParametersRcd" ),
    firstValid = cms.vuint32( 1 )
)


process.StableParameters = cms.ESProducer( "StableParametersTrivialProducer",
   NumberL1IsoEG = cms.uint32( 4 ),
   NumberL1JetCounts = cms.uint32( 12 ),
   NumberPhysTriggersExtended = cms.uint32( 64 ),
   NumberTechnicalTriggers = cms.uint32( 64 ),
   NumberL1NoIsoEG = cms.uint32( 4 ),
   IfCaloEtaNumberBits = cms.uint32( 4 ),
   NumberL1CenJet = cms.uint32( 4 ),
   NumberL1TauJet = cms.uint32( 4 ),
   NumberL1Mu = cms.uint32( 4 ),
   NumberConditionChips = cms.uint32( 1 ),
   IfMuEtaNumberBits = cms.uint32( 6 ),
   NumberPsbBoards = cms.int32( 7 ),
   NumberPhysTriggers = cms.uint32( 512 ),
   PinsOnConditionChip = cms.uint32( 512 ),
   UnitLength = cms.int32( 8 ),
   NumberL1ForJet = cms.uint32( 4 ),
   WordLength = cms.int32( 64 ),
   OrderConditionChip = cms.vint32( 1 )
)

process.hltGtStage2ObjectMap = cms.EDProducer( "L1TGlobalProducer",
    L1DataBxInEvent = cms.int32( 5 ),
    JetInputTag = cms.InputTag( 'hltCaloStage2Digis','Jet' ),
    AlgorithmTriggersUnmasked = cms.bool( True ),
    EmulateBxInEvent = cms.int32( 1 ),
    ExtInputTag = cms.InputTag( "hltGtStage2Digis" ),
    AlgorithmTriggersUnprescaled = cms.bool( True ),
    Verbosity = cms.untracked.int32( 0 ),
    EtSumInputTag = cms.InputTag( 'hltCaloStage2Digis','EtSum' ),
    ProduceL1GtDaqRecord = cms.bool( True ),
    PrescaleSet = cms.uint32( 1 ),
    EGammaInputTag = cms.InputTag( 'hltCaloStage2Digis','EGamma' ),
    TriggerMenuLuminosity = cms.string( "startup" ),
    ProduceL1GtObjectMapRecord = cms.bool( True ),
    AlternativeNrBxBoardDaq = cms.uint32( 0 ),
    PrescaleCSVFile = cms.string( "prescale_L1TGlobal.csv" ),
    TauInputTag = cms.InputTag( 'hltCaloStage2Digis','Tau' ),
    BstLengthBytes = cms.int32( -1 ),
    MuonInputTag = cms.InputTag( 'hltGmtStage2Digis','Muon' )
)

process.triggerSelectionLoneBunch = cms.EDFilter( "HLTL1TSeed",
    L1SeedsLogicalExpression = cms.string( "L1_IsolatedBunch" ),
    L1EGammaInputTag = cms.InputTag( 'hltCaloStage2Digis','EGamma' ),
    L1JetInputTag = cms.InputTag( 'hltCaloStage2Digis','Jet' ),
    saveTags = cms.bool( True ),
    L1ObjectMapInputTag = cms.InputTag( "hltGtStage2ObjectMap" ),
    L1EtSumInputTag = cms.InputTag( 'hltCaloStage2Digis','EtSum' ),
    L1TauInputTag = cms.InputTag( 'hltCaloStage2Digis','Tau' ),
    L1MuonInputTag = cms.InputTag( 'hltGmtStage2Digis','Muon' ),
    L1GlobalInputTag = cms.InputTag( "hltGtStage2Digis" )
)



if LoneBunch:
    process.p = cms.Path(process.hltGtStage2ObjectMap *
                         process.triggerSelectionLoneBunch * 
                         process.bunchSpacingProducer *
                         process.ecalLocalRecoSequenceAlCaStream *
                         process.pulseDump )
else:
    process.p = cms.Path(process.bunchSpacingProducer *
                         process.ecalLocalRecoSequenceAlCaStream *
                         process.pulseDump )
