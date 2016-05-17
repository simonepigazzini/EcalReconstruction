import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("Tree")

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.PAT_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:step4.root')
)

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:com10', '')


# selections
from EcalReconstruction.EcalTools.zee_selections_cff import *

## 1) DEFINE LOOSE LEPTONS 
process.looseEl = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag("boostedElectrons"),
    cut = cms.string(ELID_LOOSE),
)

## 3) DEFINE GOOD LEPTONS 
process.goodEl = cms.EDFilter("PATElectronSelector",
    src = cms.InputTag("looseEl"),
    cut = cms.string(ELID_GOOD),
)

process.countGoodEl = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("goodEl"),
    minNumber = cms.uint32(2),
)

## 4) MAKE Z CANDIDATES
process.zll = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string("goodEl@+ goodEl@-"),
    cut = cms.string(SEL_BEST_Z)
)

process.countZ = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("zll"),
    minNumber = cms.uint32(1)
)


### ========= INCLUSIVE MONITORING OF ALL Z'S =============
process.anyZllTree = cms.EDFilter("ProbeTreeProducer",
    src = cms.InputTag("zll"),
    sortDescendingBy = cms.string("-abs(mass - 91.188)"),
    cut = cms.string(""),
    variables   = cms.PSet(
       l1pt     = cms.string("daughter(0).pt"),
       l1eta    = cms.string("daughter(0).eta"),
       l1phi    = cms.string("daughter(0).phi"),
       l1pdgId  = cms.string("daughter(0).pdgId"),
       l2pt     = cms.string("daughter(1).pt"),
       l2eta    = cms.string("daughter(1).eta"),
       l2phi    = cms.string("daughter(1).phi"),
       l2pdgId  = cms.string("daughter(1).pdgId"),
       zmass    = cms.string("mass"),
       zpt      = cms.string("pt"),
       zeta     = cms.string("eta"),
       zphi     = cms.string("phi"),
       l1pfIsoChHad04  = cms.string("daughter(0).masterClone.pfIsolationVariables().sumChargedParticlePt"),
       l2pfIsoChHad04  = cms.string("daughter(1).masterClone.pfIsolationVariables().sumChargedParticlePt"),
       l1pfIsoNHad04  = cms.string("daughter(0).masterClone.pfIsolationVariables().sumNeutralHadronEtHighThreshold"),
       l2pfIsoNHad04  = cms.string("daughter(1).masterClone.pfIsolationVariables().sumNeutralHadronEtHighThreshold"),
       l1pfIsoPhoton04  = cms.string("daughter(0).masterClone.pfIsolationVariables().sumPhotonEtHighThreshold"),
       l2pfIsoPhoton04  = cms.string("daughter(1).masterClone.pfIsolationVariables().sumPhotonEtHighThreshold"),
       l1bdtID   = cms.string("daughter(0).masterClone.userFloat('bdttrig')"),
       l2bdtID   = cms.string("daughter(1).masterClone.userFloat('bdttrig')"),
       nvtx     = cms.string("daughter(0).masterClone.userFloat('vertices')"),
       ),
    flags = cms.PSet(),
    addRunLumiInfo = cms.bool(True),
)

process.zeetree = process.anyZllTree.clone(cut = cms.string("abs(daughter(1).pdgId) == 11"))
process.zeetree.variables.l1sceta     = cms.string("daughter(0).masterClone.superCluster.eta")
process.zeetree.variables.l1scphi     = cms.string("daughter(0).masterClone.superCluster.phi")
process.zeetree.variables.l1r9        = cms.string("daughter(0).masterClone.r9")
process.zeetree.variables.l1sietaieta = cms.string("daughter(0).masterClone.sigmaIetaIeta")
process.zeetree.variables.l1gsfp      = cms.string("daughter(0).masterClone.gsfTrack.p")
process.zeetree.variables.l1gsfpmode  = cms.string("daughter(0).masterClone.gsfTrack.momentumMode.R")
process.zeetree.variables.l1p         = cms.string("daughter(0).p")
process.zeetree.variables.l1classification = cms.string("daughter(0).masterClone.classification")
process.zeetree.variables.l2sceta     = cms.string("daughter(1).masterClone.superCluster.eta")
process.zeetree.variables.l2scphi     = cms.string("daughter(1).masterClone.superCluster.phi")
process.zeetree.variables.l2r9        = cms.string("daughter(1).masterClone.r9")
process.zeetree.variables.l2sietaieta = cms.string("daughter(1).masterClone.sigmaIetaIeta")
process.zeetree.variables.l2gsfp      = cms.string("daughter(1).masterClone.gsfTrack.p")
process.zeetree.variables.l2gsfpmode  = cms.string("daughter(1).masterClone.gsfTrack.momentumMode.R")
process.zeetree.variables.l2p         = cms.string("daughter(1).p")
process.zeetree.variables.l2classification = cms.string("daughter(1).masterClone.classification")

# here the alternative energies
process.zeetree.variables.l1r9global    = cms.string("daughter(0).masterClone.userFloat('r9global')") 
process.zeetree.variables.l1e5x5global  = cms.string("daughter(0).masterClone.userFloat('e5x5global')") 
process.zeetree.variables.l1e5x5mf50ns  = cms.string("daughter(0).masterClone.userFloat('e5x5mf50ns')") 
process.zeetree.variables.l1e5x5mf25ns  = cms.string("daughter(0).masterClone.userFloat('e5x5mf25ns')") 
process.zeetree.variables.l1e5x5mf0pu   = cms.string("daughter(0).masterClone.userFloat('e5x5mf0pu')") 
process.zeetree.variables.l2r9global    = cms.string("daughter(1).masterClone.userFloat('r9global')") 
process.zeetree.variables.l2e5x5global  = cms.string("daughter(1).masterClone.userFloat('e5x5global')") 
process.zeetree.variables.l2e5x5mf50ns  = cms.string("daughter(1).masterClone.userFloat('e5x5mf50ns')") 
process.zeetree.variables.l2e5x5mf25ns  = cms.string("daughter(1).masterClone.userFloat('e5x5mf25ns')") 
process.zeetree.variables.l2e5x5mf0pu   = cms.string("daughter(1).masterClone.userFloat('e5x5mf0pu')") 

# and few more hit variables
process.zeetree.variables.l1seedflag    = cms.string("daughter(0).masterClone.userFloat('seedflag')") 
process.zeetree.variables.l1seedflag    = cms.string("daughter(0).masterClone.userFloat('seedflag')") 
process.zeetree.variables.l1seedchi2    = cms.string("daughter(0).masterClone.userFloat('seedchi2')") 
process.zeetree.variables.l1seedeerr    = cms.string("daughter(0).masterClone.userFloat('seedeerr')") 
process.zeetree.variables.l2seedflag    = cms.string("daughter(1).masterClone.userFloat('seedflag')") 
process.zeetree.variables.l2seedflag    = cms.string("daughter(1).masterClone.userFloat('seedflag')") 
process.zeetree.variables.l2seedchi2    = cms.string("daughter(1).masterClone.userFloat('seedchi2')") 
process.zeetree.variables.l2seedeerr    = cms.string("daughter(1).masterClone.userFloat('seedeerr')") 

process.common = cms.Sequence(
    process.looseEl +
    process.goodEl +
    process.countGoodEl + 
    process.zll +
    process.countZ
)

process.zPath = cms.Path(
    process.common +   
    process.zll +
    process.countZ +
    process.zeetree 
)

process.schedule = cms.Schedule([process.zPath])

process.TFileService = cms.Service("TFileService", fileName = cms.string("zeeTree.root"))
