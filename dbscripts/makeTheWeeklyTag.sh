conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_data_Run2015C_lowPU.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly.db -t EcalPulseShapes_data -b 248036
conddb_import -f sqlite_file:/cmshome/dimarcoe/releases/ecalreco/CMSSW_7_4_13/src/EcalReconstruction/dbscripts/ecaltemplates_popcon_runs_257615_258600.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly.db -t EcalPulseShapes_data -b 257615 
conddb_import -f sqlite_file:/cmshome/dimarcoe/releases/ecalreco/CMSSW_7_4_13/src/EcalReconstruction/dbscripts/ecaltemplates_popcon_runs_258600_259700.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_weekly.db -t EcalPulseShapes_data -b 258600


