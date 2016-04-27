import FWCore.ParameterSet.Config as cms
superclusterDumper = cms.EDAnalyzer("SuperClusterDumper",
                                    EBSuperClusters1 = cms.InputTag("correctedHybridSuperClusters"),
                                    EESuperClusters1 = cms.InputTag("correctedMulti5x5SuperClustersWithPreshower"),
                                    EBSuperClusters2 = cms.InputTag("particleFlowSuperClusterECAL:particleFlowSuperClusterECALBarrel"),
                                    EESuperClusters2 = cms.InputTag("particleFlowSuperClusterECAL:particleFlowSuperClusterECALEndcapWithPreshower"),
                                    ecalBarrelRecHits = cms.InputTag("ecalRecHit:EcalRecHitsEB"),
                                    ecalEndcapRecHits = cms.InputTag("ecalRecHit:EcalRecHitsEE"),
                                    mcTruthCollection = cms.InputTag("genParticles"),
                                    isMC = cms.untracked.bool(True),
                                    fileName = cms.untracked.string('tree.root'),
                                    nameTree = cms.untracked.string('ntp1')
                                    )


