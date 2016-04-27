import FWCore.ParameterSet.Config as cms
process = cms.Process("HitsToClusters")

process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("RecoEgamma.EgammaElectronProducers.gsfElectronSequence_cff")
process.load("Configuration.StandardSequences.DigiToRaw_cff")
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("EventFilter.EcalRawToDigi.EcalUnpackerMapping_cfi")
process.load("EventFilter.EcalRawToDigi.EcalUnpackerData_cfi")

#### CONFIGURE IT HERE
isMC = True
#####################


# get uncalib rechits from global method (weights for ampli, time from ratio, etc)
import RecoLocalCalo.EcalRecProducers.ecalGlobalUncalibRecHit_cfi
# modify the rechits sequence activating the PU sub
process.ecalGlobalUncalibRecHitNoPU = process.ecalGlobalUncalibRecHit.clone()
process.ecalGlobalUncalibRecHitNoPU.subtractPU = cms.bool(True)

# get calibrated rechits
process.load("CalibCalorimetry.EcalLaserCorrection.ecalLaserCorrectionService_cfi")
process.load("RecoLocalCalo.EcalRecProducers.ecalRecHit_cfi")

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


# and now the clusters and superclusters
process.load("RecoEcal.Configuration.RecoEcal_cff")
process.load("RecoParticleFlow.PFClusterProducer.particleFlowCluster_cff")
process.particleFlowSuperClusterECAL.useRegression = False

process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(100) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.source = cms.Source("PoolSource",
              #fileNames = cms.untracked.vstring('/store/cmst3/user/bendavid/photongun_pu25/photongun_pu25_988_1_AV6.root')
              fileNames = cms.untracked.vstring('/store/cmst3/user/bendavid/photongun_nopu/photongun_nopu_9_2_QGL.root')
                ) 


process.ecalRecHitNoPU = process.ecalRecHit.clone()
process.ecalRecHitNoPU.EBuncalibRecHitCollection = cms.InputTag("ecalGlobalUncalibRecHitNoPU","EcalUncalibRecHitsEB")
process.ecalRecHitNoPU.EEuncalibRecHitCollection = cms.InputTag("ecalGlobalUncalibRecHitNoPU","EcalUncalibRecHitsEE")

process.ecalLocalReco = cms.Sequence( process.ecalGlobalUncalibRecHit * process.ecalRecHit
                                      * process.ecalGlobalUncalibRecHitNoPU * process.ecalRecHitNoPU )


# modify the clustering sequence to use the PU-subtracted rechits
# do just the 5x5 clusters
# with the RECO rechits
process.load("RecoEcal.EgammaClusterProducers.multi5x5BasicClusters_cfi")
process.fixed5x5ClustersReco = process.multi5x5BasicClustersCleaned.clone()
process.fixed5x5ClustersReco.doBarrel = True
process.fixed5x5ClustersReco.barrelHitTag = cms.InputTag('ecalRecHit','EcalRecHitsEB','RECO')
process.fixed5x5ClustersReco.endcapHitTag = cms.InputTag('ecalRecHit','EcalRecHitsEE','RECO')
process.fixed5x5ClustersReco.barrelClusterCollection = 'Barrel5x5Clusters'
process.fixed5x5ClustersReco.endcapClusterCollection = 'Endcap5x5Clusters'

# with the new rechits 
process.fixed5x5ClustersFit = process.multi5x5BasicClustersCleaned.clone()
process.fixed5x5ClustersFit.doBarrel = True
process.fixed5x5ClustersFit.barrelHitTag = cms.InputTag('ecalRecHit','EcalRecHitsEB','HitsToClusters')
process.fixed5x5ClustersFit.endcapHitTag = cms.InputTag('ecalRecHit','EcalRecHitsEE','HitsToClusters')
process.fixed5x5ClustersFit.barrelClusterCollection = 'Barrel5x5Clusters'
process.fixed5x5ClustersFit.endcapClusterCollection = 'Endcap5x5Clusters'

# with the new rechits 
process.fixed5x5ClustersFitNoPU = process.multi5x5BasicClustersCleaned.clone()
process.fixed5x5ClustersFitNoPU.doBarrel = True
process.fixed5x5ClustersFitNoPU.barrelHitTag = cms.InputTag('ecalRecHitNoPU','EcalRecHitsEB','HitsToClusters')
process.fixed5x5ClustersFitNoPU.endcapHitTag = cms.InputTag('ecalRecHitNoPU','EcalRecHitsEE','HitsToClusters')
process.fixed5x5ClustersFitNoPU.barrelClusterCollection = 'Barrel5x5Clusters'
process.fixed5x5ClustersFitNoPU.endcapClusterCollection = 'Endcap5x5Clusters'


process.ClusterECALSequence = cms.Sequence(process.fixed5x5ClustersReco
                                           * process.fixed5x5ClustersFit
                                           * process.fixed5x5ClustersFitNoPU )

process.p = cms.Path( process.ecalLocalReco
                      * process.ClusterECALSequence )

process.out = cms.OutputModule("PoolOutputModule",
                               outputCommands = cms.untracked.vstring('drop *',
#                                                                      'keep *_ecalRecHit*_*_*',
#                                                                      'keep *_ecalGlobalUncalibRecHit*_*_*',
                                                                      'keep *_fixed5x5Clusters*_*_*',
                                                                      'keep *_offlineBeamSpot_*_*',
                                                                      'keep *_addPileupInfo_*_*',
                                                                      'keep *_genParticles_*_*'
                                                                      ),
                               fileName = cms.untracked.string('testEcalFullReco.root')
                               )

process.outpath = cms.EndPath(process.out)



process.GlobalTag.globaltag = 'START71_V1::All'

process.GlobalTag.toGet = cms.VPSet(
    cms.PSet(record = cms.string("EcalTBWeightsRcd"),
             tag = cms.string("EcalTBWeights_3p5_time_mc"),
             connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_ECAL")
             )
    )

