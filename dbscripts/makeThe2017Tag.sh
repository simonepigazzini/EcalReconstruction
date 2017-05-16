conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_293765_293910.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2017_calib.db -t EcalPulseShapes_data -b 292723

echo "===> Now checking the contents..."
conddb --db ecaltemplates_popcon_2017_calib.db list EcalPulseShapes_data
