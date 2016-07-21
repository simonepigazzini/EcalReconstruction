# ---- templates ----
# === Run 2015 === 
#conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_data_Run2015C_lowPU.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly_best.db -t EcalPulseShapes_data -b 248036
#conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_7_4_15_patch1/src/EcalReconstruction/dbscripts/ecaltemplates_popcon_runs_257616_258600.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly_best.db -t EcalPulseShapes_data -b 257616
#conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_7_4_15_patch1/src/EcalReconstruction/dbscripts/ecaltemplates_popcon_runs_258601_999999.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly_best.db -t EcalPulseShapes_data -b 258601 

# === Run 2016 ===
#conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_8_0_8/src/EcalReconstruction/dbscripts/ecaltemplates_popcon_runs_271983_272818.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly_best.db -t EcalPulseShapes_data -b 271983
#conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_8_0_8/src/EcalReconstruction/dbscripts/ecaltemplates_popcon_runs_273301_273590.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly_best.db -t EcalPulseShapes_data -b 272818
#conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_8_0_8/src/EcalReconstruction/dbscripts/ecaltemplates_popcon_runs_274080_274344.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly_best.db -t EcalPulseShapes_data -b 274080

conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_8_0_8/src/EcalReconstruction/dbscripts/ecaltemplates_popcon_runs_274958_275659.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly_best.db -t EcalPulseShapes_data -b 274958
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_8_0_8/src/EcalReconstruction/dbscripts/ecaltemplates_popcon_runs_275757_275931.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly_best.db -t EcalPulseShapes_data -b 275757
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_8_0_8/src/EcalReconstruction/dbscripts/ecaltemplates_popcon_runs_276315_276587.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly_best.db -t EcalPulseShapes_data -b 276315

echo "===> Now checking the contents..."
conddb --db ecaltemplates_popcon_weekly_best.db list EcalPulseShapes_data


# ---- covariances ----
#conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_7_4_15_patch1/src/EcalReconstruction/dbscripts/ecalcovariances_popcon_runs_248036_257615.db -i EcalPulseCovariances_data -c sqlite_file:ecalcovariances_popcon_weekly_best.db -t EcalPulseCovariances_data -b 248036
#conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_7_4_15_patch1/src/EcalReconstruction/dbscripts/ecalcovariances_popcon_runs_257616_258600.db -i EcalPulseCovariances_data -c sqlite_file:ecalcovariances_popcon_weekly_best.db -t EcalPulseCovariances_data -b 257616
#conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_7_4_15_patch1/src/EcalReconstruction/dbscripts/ecalcovariances_popcon_runs_258601_999999.db -i EcalPulseCovariances_data -c sqlite_file:ecalcovariances_popcon_weekly_best.db -t EcalPulseCovariances_data -b 258601
