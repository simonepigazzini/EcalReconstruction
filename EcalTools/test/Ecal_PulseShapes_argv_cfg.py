import sys
import os.path
import FWCore.ParameterSet.Config as cms

POPULATE_MC = False
FIRST_RUN_DATA = '2'

if POPULATE_MC: suffix = "mc-_timeShifted_1.000000ns"
else: 
    suffix = sys.argv[2]
    suffixorig = sys.argv[3]


print "reading txt file with suffix ",suffixorig
print "writing into ",suffix

process = cms.Process("ProcessOne")
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.CondDBCommon.connect = 'sqlite_file:ecaltemplates_popcon_'+suffix+'.db'
process.CondDBCommon.DBParameters.authenticationPath = '.'
process.CondDBCommon.DBParameters.messageLevel=cms.untracked.int32(1)

process.MessageLogger = cms.Service("MessageLogger",
                                    debugModules = cms.untracked.vstring('*'),
                                    destinations = cms.untracked.vstring('cout')
                                    )

process.source = cms.Source("EmptyIOVSource",
                            firstValue = cms.uint64(1),
                            lastValue = cms.uint64(1),
                            timetype = cms.string('runnumber'),
                            interval = cms.uint64(1)
                            )

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDBCommon,
    logconnect = cms.untracked.string('sqlite_file:logecaltemplates_popcon_'+suffix+'.db'),
    timetype = cms.untracked.string('runnumber'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('EcalPulseShapesRcd'),
        tag = cms.string('EcalPulseShapes_data')
    ))
)

txtfile = "atemplate_histograms_ECAL_"+suffixorig+".txt"
if os.path.isfile(txtfile)==False:
    print "WARNING: file ",txtfile," does not exist. Exiting... "
    exit

process.Test1 = cms.EDAnalyzer("ExTestEcalPulseShapesAnalyzer",
    SinceAppendMode = cms.bool(True),
    record = cms.string('EcalPulseShapesRcd'),
    loggingOn = cms.untracked.bool(True),
    Source = cms.PSet(
        firstRun = cms.string('1' if POPULATE_MC else FIRST_RUN_DATA),
        inputFileName = cms.string(txtfile),
        EBPulseShapeTemplate = cms.vdouble (
            # 1.13979e-02, 7.58151e-01, 1.00000e+00, 8.87744e-01, 6.73548e-01, 4.74332e-01, 3.19561e-01, 2.15144e-01, 1.47464e-01, 1.01087e-01, 6.93181e-02, 4.75044e-02  # nominal
            # 1.13979e-02, 0.766431, 1, 0.881932, 0.675448, 0.48065, 0.326914, 0.21564, 0.139126, 0.0882683, 0.0552715, 0.0342461 # -0.25 ns
            # 0, 0.754766, 1, 0.885617, 0.67959, 0.484141, 0.32953, 0.217478, 0.140365, 0.089081, 0.0557935, 0.0345761 # 0.25 ns
            # 1.15969e-02, 0.772164, 1, 0.880122, 0.673413, 0.478934, 0.325628, 0.214737, 0.138516, 0.0878689, 0.055015, 0.0340839 # -0.50 ns
            # 0, 0.748832, 1, 0.887491, 0.681696, 0.485917, 0.330861, 0.218413, 0.140995, 0.0894943, 0.056059, 0.034744 # 0.50 ns
            # 1.16223e-02, 0.783438, 1, 0.87656, 0.66941, 0.47556, 0.323099, 0.212961, 0.137319, 0.0870835, 0.0545105, 0.033765 # -1.0 ns
            0, 0.736756, 1, 0.891306, 0.685984, 0.489531, 0.333569, 0.220315, 0.142279, 0.0903356, 0.0565993, 0.0350856 # 1.0 ns
            ) ,
        EEPulseShapeTemplate = cms.vdouble (
            # 1.16442e-01, 7.56246e-01, 1.00000e+00, 8.97182e-01, 6.86831e-01, 4.91506e-01, 3.44111e-01, 2.45731e-01, 1.74115e-01, 1.23361e-01, 8.74288e-02, 6.19570e-02  # nominal
            # 1.16442e-01, 0.79583, 1, 0.906977, 0.712123, 0.515228, 0.353617, 0.233912, 0.150572, 0.0949155, 0.0588461, 0.0359946 # -0.25 ns
            # 0.0996929, 0.78725, 1, 0.910632, 0.716495, 0.519059, 0.356557, 0.236005, 0.151991, 0.0958459, 0.0594407, 0.0363672 # 0.25 ns
            # 0.12265, 0.800071, 1, 0.905175, 0.70997, 0.513341, 0.35217, 0.232882, 0.149873, 0.0944576, 0.0585535, 0.0358113 # -0.50 ns
            # 0.0922125, 0.78291, 1, 0.912487, 0.718714, 0.521004, 0.35805, 0.237068, 0.152712, 0.0963185, 0.0597427, 0.0365565 # 0.50 ns
            # 0.138311, 0.808457, 1, 0.901624, 0.705726, 0.509625, 0.349319, 0.230853, 0.148497, 0.093556, 0.0579774, 0.0354503 # -1.0 ns
            0.077564, 0.774131, 1, 0.916252, 0.723221, 0.524956, 0.361084, 0.239229, 0.154177, 0.097279, 0.0603566, 0.0369412 # 1.0 ns
            )
        )
)


process.p = cms.Path(process.Test1)
