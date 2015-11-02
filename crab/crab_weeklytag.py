from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'rereco_zskim_weeklytag_RecHitCuts'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'RECO_Run2015D_weeklytag.py'
config.JobType.inputFiles = [ '/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_7_4_15_patch1/src/EcalReconstruction/crab/ecaltemplates_popcon_weekly.db' ]

config.Data.inputDataset = '/DoubleEG/Run2015D-ZElectron-PromptReco-v3/RAW-RECO'
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 40
config.Data.lumiMask = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Cert_246908-258714_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
config.Data.runRange = '246908-258714'
config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = True
config.Data.publishDataName = 'ZElectron_Run2015D_WeeklyPS_RecHitCuts'

config.Site.storageSite = "T2_IT_Rome"
