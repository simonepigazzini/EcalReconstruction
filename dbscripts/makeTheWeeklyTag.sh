# ---- templates ----
#conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_data_Run2015C_lowPU.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly.db -t EcalPulseShapes_data -b 248036
#conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_7_4_15_patch1/src/EcalReconstruction/dbscripts/ecaltemplates_popcon_runs_257615_258600.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly.db -t EcalPulseShapes_data -b 257615 
#conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_7_4_15_patch1/src/EcalReconstruction/dbscripts/ecaltemplates_popcon_runs_258601_259896.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly.db -t EcalPulseShapes_data -b 258601 

# ---- covariances ----
# the since of the first IOV is the beginning of Run2, since we don't have enough pi0 low PU data
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_7_4_15_patch1/src/EcalReconstruction/dbscripts/ecalcovariances_popcon_runs_257615_258600.db -i EcalPulseCovariances_data -c sqlite_file:ecalcovariances_popcon_weekly.db -t EcalPulseCovariances_data -b 256514 
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_7_4_15_patch1/src/EcalReconstruction/dbscripts/ecalcovariances_popcon_runs_258601_259896.db -i EcalPulseCovariances_data -c sqlite_file:ecalcovariances_popcon_weekly.db -t EcalPulseCovariances_data -b 258601 

