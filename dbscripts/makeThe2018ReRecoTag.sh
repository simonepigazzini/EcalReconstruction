conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_314474_314530.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2018_101X_rereco.db -t EcalPulseShapes_data -b 314093
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_315255_315267.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2018_101X_rereco.db -t EcalPulseShapes_data -b 315255
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_315488_315512.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2018_101X_rereco.db -t EcalPulseShapes_data -b 315488
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_315721_316153.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2018_101X_rereco.db -t EcalPulseShapes_data -b 315721
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_316758_316758.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2018_101X_rereco.db -t EcalPulseShapes_data -b 316758
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_316876_316928.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2018_101X_rereco.db -t EcalPulseShapes_data -b 316876
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_316944_317089.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2018_101X_rereco.db -t EcalPulseShapes_data -b 316944
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_318438_318464.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2018_101X_rereco.db -t EcalPulseShapes_data -b 318438
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_318816_318828.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2018_101X_rereco.db -t EcalPulseShapes_data -b 318816
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_319697_319840.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_2018_101X_rereco.db -t EcalPulseShapes_data -b 319697

echo "===> Now checking the contents..."
conddb --db ecaltemplates_popcon_2018_101X_rereco.db list EcalPulseShapes_data
