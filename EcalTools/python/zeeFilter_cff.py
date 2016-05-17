import FWCore.ParameterSet.Config as cms

zeeFilterElectrons = cms.EDFilter("GsfElectronRefSelector",
   src = cms.InputTag("gsfElectrons"),
   cut = cms.string("pt > 0"),
)

zeeFilterDiLep = cms.EDProducer("CandViewShallowCloneCombiner",
    decay = cms.string('zeeFilterElectrons zeeFilterElectrons'),
    cut = cms.string('deltaR(daughter(0).eta,daughter(0).phi,daughter(1).eta,daughter(1).phi) > 0.05'),
    checkCharge = cms.bool(False)
)

zeeFilterDiLep178Filter = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("zeeFilterDiLep"),
    cut = cms.string('min(daughter(0).pt,daughter(1).pt) >  8 && ' +
                     'max(daughter(0).pt,daughter(1).pt) > 17'),
    filter = cms.bool(True),
)

zeeFilterMll40Filter = cms.EDFilter("CandViewSelector",
    src = cms.InputTag("zeeFilterDiLep"),
    cut = cms.string('mass > 40 && daughter(0).pdgId = -daughter(1).pdgId'), ## and SF/OS
    filter = cms.bool(True),
)

zeeFilterMll40FilterSeq = cms.Sequence(
    zeeFilterElectrons *
    zeeFilterDiLep * 
    zeeFilterDiLep178Filter *
    zeeFilterMll40Filter
)

