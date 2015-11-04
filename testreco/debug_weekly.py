# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: reRECO --conditions 74X_dataRun2_Prompt_v4 -s RAW2DIGI,L1Reco,RECO --runUnscheduled --data --eventcontent RECO --scenario pp --datatier RECO --customise Configuration/DataProcessing/RecoTLR.customiseDataRun2Common_25ns --filein /store/data/Run2015C/SingleElectron/RAW/v1/000/254/879/00000/8E51CA98-7349-E511-B9AE-02163E01427B.root -n 100 --no_exec --python_filename=debug.py
import FWCore.ParameterSet.Config as cms

process = cms.Process('reRECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/data1/emanuele/ecal/localreco/1A438326-636D-E511-9BE0-02163E01442D.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('reRECO nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RECO'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('reco_weekly.root'),
    outputCommands = process.RECOEventContent.outputCommands + cms.untracked.vstring('keep *_ecalDigis_*_*',
                                                                                     'keep *_*_*_RECO',
                                                                                     'keep *_*_EcalUncalibRecHitsEB_*',
                                                                                     'keep *_*_EcalUncalibRecHitsEE_*'),
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '74X_dataRun2_Prompt_v4', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)

# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.RecoTLR
from Configuration.DataProcessing.RecoTLR import customiseDataRun2Common_25ns 

#call to customisation function customiseDataRun2Common_25ns imported from Configuration.DataProcessing.RecoTLR
process = customiseDataRun2Common_25ns(process)

# End of customisation functions
#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)

# process.GlobalTag.toGet = cms.VPSet(
# cms.PSet(record = cms.string("EcalPulseShapesRcd"),
# tag = cms.string("EcalPulseShapes_data"),
# connect = cms.untracked.string("sqlite_file:ecaltemplates_popcon_weekly.db"
# )))

process.GlobalTag.toGet = cms.VPSet(
cms.PSet(record = cms.string("EcalPulseCovariancesRcd"),
tag = cms.string("EcalPulseCovariances_data"),
connect = cms.untracked.string("sqlite_file:ecalcovariances_popcon_weekly.db"
)))
