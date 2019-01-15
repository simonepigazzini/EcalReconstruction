rm -f ecaltemplates_popcon_UltraLegacy2018_calib.db

conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_314474_314530.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 314472
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_315255_315267.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 315255
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_315488_315512.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 315488
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_315721_316153.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 315721
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_316758_316758.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 316758
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_316876_316928.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 316876
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_316944_317089.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 316944
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_317683_317696.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 317683
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_318438_318464.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 318438
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_318816_318828.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 318816
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_319697_319840.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 319697
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_320673_320757.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 320394
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_321162_321218.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 321162
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_321973_322022.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 321973
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_323841_324022.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 323841
conddb_import -f sqlite_file:/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/ecaltemplates_popcon_runs_326635_326787.db -i EcalPulseShapes_data -c sqlite_file:ecaltemplates_popcon_UltraLegacy2018_calib.db -t EcalPulseShapes_data -b 326294

echo "===> Now checking the contents..."
conddb --db ecaltemplates_popcon_UltraLegacy2018_calib.db list EcalPulseShapes_data
