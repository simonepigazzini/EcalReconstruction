import FWCore.ParameterSet.Config as cms

VERTEX_SEL=("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2")

firstVertexIsGood = cms.EDFilter(
     "PATSingleVertexSelector",
     mode      = cms.string("firstVertex"),
     vertices              = cms.InputTag("offlinePrimaryVertices"),
     vertexPreselection    = cms.vstring(VERTEX_SEL),
#     candidates            = cms.VInputTag(cms.InputTag('gsfElectrons'), cms.InputTag('muons')),
#     candidatePreselection = cms.string("pt > 5"),
#     beamSpot              = cms.InputTag('offlineBeamSpot'),
     filter = cms.bool(True),
)

goodPrimaryVertices = cms.EDFilter("VertexSelector",
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string(VERTEX_SEL),
  filter = cms.bool(True),
)
