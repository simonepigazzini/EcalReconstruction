conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_data_Run2016B_since_271983.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2016_calib.db -t EcalPulseShapes_data -b 271983
conddb_import -f sqlite_file:/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/ecaltemplates_popcon_data_Run2016BC_since_274958.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2016_calib.db -t EcalPulseShapes_data -b 274958
conddb_import -f sqlite_file:/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/ecaltemplates_popcon_data_Run2016F_since_277932.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2016_calib.db -t EcalPulseShapes_data -b 277932
conddb_import -f sqlite_file:/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_278969_278976.db  -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2016_calib.db -t EcalPulseShapes_data -b 278346
conddb_import -f sqlite_file:/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_279115_279116.db  -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2016_calib.db -t EcalPulseShapes_data -b 278977
conddb_import -f sqlite_file:/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_279653_279658.db  -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2016_calib.db -t EcalPulseShapes_data -b 279474
conddb_import -f sqlite_file:/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_279841_279841.db  -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2016_calib.db -t EcalPulseShapes_data -b 279717 



echo "===> Now checking the contents..."
conddb --db ecaltemplates_popcon_2016_calib.db list EcalPulseShapes_data
