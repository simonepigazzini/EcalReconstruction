conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_293765_293910.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_oct2017_rereco_calib.db -t EcalPulseShapes_data -b 293765
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_297113_297114.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_oct2017_rereco_calib.db -t EcalPulseShapes_data -b 297113
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_298809_298809.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_oct2017_rereco_calib.db -t EcalPulseShapes_data -b 298809
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_299481_299481.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_oct2017_rereco_calib.db -t EcalPulseShapes_data -b 299067
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_300079_300088.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_oct2017_rereco_calib.db -t EcalPulseShapes_data -b 300079
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_300157_300157.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_oct2017_rereco_calib.db -t EcalPulseShapes_data -b 300122
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_300574_300576.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_oct2017_rereco_calib.db -t EcalPulseShapes_data -b 300574
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_301141_301165.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_oct2017_rereco_calib.db -t EcalPulseShapes_data -b 301141
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_301472_301480.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_oct2017_rereco_calib.db -t EcalPulseShapes_data -b 301472

echo "===> Now checking the contents..."
conddb --db ecaltemplates_popcon_oct2017_rereco_calib.db list EcalPulseShapes_data
