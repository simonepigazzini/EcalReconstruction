import FWCore.ParameterSet.Config as cms
process = cms.Process("RECO2")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.Geometry.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')

process.XMLFromDBSource.label = cms.string("Extended2015ZeroMaterial")
process.GlobalTag.globaltag = 'POSTLS172_V7::All'

#### CONFIGURE IT HERE
isMC = True
doTimeProfiling = False
#####################
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# start from RAW format for more flexibility
process.raw2digi_step = cms.Sequence(process.RawToDigi)

# get the recovered digis
if isMC:
    process.ecalDetIdToBeRecovered.ebSrFlagCollection = 'simEcalDigis:ebSrFlags'
    process.ecalDetIdToBeRecovered.eeSrFlagCollection = 'simEcalDigis:eeSrFlags'
    process.ecalRecHit.recoverEBFE = False
    process.ecalRecHit.recoverEEFE = False
    process.ecalRecHit.killDeadChannels = False
    process.ecalRecHit.ebDetIdToBeRecovered = ''
    process.ecalRecHit.eeDetIdToBeRecovered = ''
    process.ecalRecHit.ebFEToBeRecovered = ''
    process.ecalRecHit.eeFEToBeRecovered = ''


####### reconstruction
process.reconstruction_step = cms.Sequence(process.offlineBeamSpot*process.ecalLocalRecoSequence*process.pfClusteringPS*process.pfClusteringECAL*process.ecalClusters)
process.particleFlowSuperClusterECAL.useRegression = False

process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(100) )
path = '/store/group/comm_ecal/localreco/cmssw_720p4_zeromaterial/photongun_pu25_ave40/'
process.source = cms.Source("PoolSource",
                            duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                            fileNames = cms.untracked.vstring(path+'photongun_pu25_ave40_0.root',
                                                              path+'photongun_pu25_ave40_1.root',
                                                              path+'photongun_pu25_ave40_10.root',
                                                              path+'photongun_pu25_ave40_100.root',
                                                              path+'photongun_pu25_ave40_1000.root',
                                                              path+'photongun_pu25_ave40_1001.root',
                                                              path+'photongun_pu25_ave40_1002.root',
                                                              path+'photongun_pu25_ave40_1003.root',
                                                              path+'photongun_pu25_ave40_1004.root',
                                                              path+'photongun_pu25_ave40_1005.root'
                                                              ))


process.out = cms.OutputModule("PoolOutputModule",
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_ecalUncalib*_*_*',
                                                                      'keep *_ecalRecHit*_*_*',
                                                                      'keep *_particleFlowSuperClusterECAL_*_*',
                                                                      'keep *_offlineBeamSpot_*_*',
                                                                      'keep *_addPileupInfo_*_*'
                                                                      ),
                               fileName = cms.untracked.string('reco2_pu40.root')
                               )



process.ecalTestRecoLocal = cms.Sequence( process.raw2digi_step * process.reconstruction_step )

from PhysicsTools.PatAlgos.tools.helpers import *

process.p = cms.Path(process.ecalTestRecoLocal)
process.outpath = cms.EndPath(process.out)


#########################
#    Time Profiling     #
#########################

#https://twiki.cern.ch/twiki/bin/viewauth/CMS/FastTimerService
if(doTimeProfiling):
    process.MessageLogger.categories.append('FastReport')
    process.MessageLogger.cerr.FastReport = cms.untracked.PSet( limit = cms.untracked.int32( 10000000 ) )
    
    # remove any instance of the FastTimerService
    if 'FastTimerService' in process.__dict__:
        del process.FastTimerService
        
    # instrument the menu with the FastTimerService
    process.load( "HLTrigger.Timer.FastTimerService_cfi" )
    
    # this is currently ignored in 7.x, and alway uses the real tim clock
    process.FastTimerService.useRealTimeClock         = True
    
    # enable specific features
    process.FastTimerService.enableTimingPaths        = True
    process.FastTimerService.enableTimingModules      = True
    process.FastTimerService.enableTimingExclusive    = True
    
    # print a text summary at the end of the job
    process.FastTimerService.enableTimingSummary      = True
    
    # skip the first path (useful for HLT timing studies to disregard the time spent loading event and conditions data)
    process.FastTimerService.skipFirstPath            = False
    
    # enable per-event DQM plots
    process.FastTimerService.enableDQM                = True
    
    # enable per-path DQM plots
    process.FastTimerService.enableDQMbyPathActive    = True
    process.FastTimerService.enableDQMbyPathTotal     = True
    process.FastTimerService.enableDQMbyPathOverhead  = True
    process.FastTimerService.enableDQMbyPathDetails   = True
    process.FastTimerService.enableDQMbyPathCounters  = True
    process.FastTimerService.enableDQMbyPathExclusive = True
    
    # enable per-module DQM plots
    process.FastTimerService.enableDQMbyModule        = True
    process.FastTimerService.enableDQMbyModuleType    = True
    
    # enable per-event DQM sumary plots
    process.FastTimerService.enableDQMSummary         = True
    
    # enable per-event DQM plots by lumisection
    process.FastTimerService.enableDQMbyLumiSection   = True
    process.FastTimerService.dqmLumiSectionsRange     = 2500    # lumisections (23.31 s)

    # set the time resolution of the DQM plots
    process.FastTimerService.dqmTimeRange             = 1000.   # ms
    process.FastTimerService.dqmTimeResolution        =    5.   # ms
    process.FastTimerService.dqmPathTimeRange         =  100.   # ms
    process.FastTimerService.dqmPathTimeResolution    =    0.5  # ms
    process.FastTimerService.dqmModuleTimeRange       = 1000.   # ms
    process.FastTimerService.dqmModuleTimeResolution  =    0.5  # ms
    
    # set the base DQM folder for the plots
    process.FastTimerService.dqmPath                  = "HLT/TimerService"
    process.FastTimerService.enableDQMbyProcesses     = True
    
    # save the DQM plots in the DQMIO format
    process.dqmOutput = cms.OutputModule("DQMRootOutputModule",
                                         fileName = cms.untracked.string("DQM_pu40.root")
                                         )
    process.FastTimerOutput = cms.EndPath( process.dqmOutput )

