import FWCore.ParameterSet.Config as cms
import re
process = cms.Process("PEDS")

######## configure here #######
calcPedestalMean = False
run = 195592
barrel = True
FEDused = "EB+13"
Gain = 1
###############################

covarianceOutputFile = str("covariances_run"+str(run)+"_"+FEDused+"_Gain"+str(Gain)+".root")
covarianceOutputFile = re.sub("\+","p", covarianceOutputFile)
covarianceOutputFile = re.sub("\-","m", covarianceOutputFile)

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

process.load('EcalReconstruction.EcalTools.pedestalTask_cfi')

# now configure the analysis module
import EcalReconstruction.EcalTools.pedestalTask_cfi
process.pedestalMeasurement = EcalReconstruction.EcalTools.pedestalTask_cfi.pedestalTask.clone()
process.pedestalMeasurement.calcPedMean = calcPedestalMean
process.pedestalMeasurement.doBarrel = barrel
if barrel: process.pedestalMeasurement.pedestalMeanFile = cms.string("pedestal_means_run_"+str(run)+"_barrel_Gain"+str(Gain)+".root")
else: process.pedestalMeasurement.pedestalMeanFile = cms.string("pedestal_means_run_"+str(run)+"_endcap_Gain"+str(Gain)+".root")
process.pedestalMeasurement.covariancesFile = covarianceOutputFile
process.pedestalMeasurement.feds = cms.vstring(FEDused)
process.pedestalMeasurement.gain = Gain

process.maxEvents = cms.untracked.PSet(  input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1

path='/store/cmst3/user/emanuele/ecal/pedestals/'
# run 172496 (3/8/2011)
#process.source = cms.Source("NewEventStreamFileReader",
#                            fileNames = cms.untracked.vstring(path+'ecal_local.00172496.0001.A.storageManager.00.0000.dat')
#                            )

#run 195592 (6/6/2012)
process.source = cms.Source("PoolSource",
                            duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
                            fileNames = cms.untracked.vstring(path+'ecal_local.00195592.0000.PEDESTAL-STD.root')
                            )

# process.source = cms.Source("PoolSource",
#                             duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
#                             fileNames = cms.untracked.vstring(path+'ecal_local.00203189.0000.PEDESTAL-STD.root')
#                             )

# start from RAW format for more flexibility
process.raw2digi_step = cms.Sequence(process.RawToDigi)

process.p = cms.Path(process.ecalDigis * 
                     process.pedestalMeasurement )
