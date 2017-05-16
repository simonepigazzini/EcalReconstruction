#! /bin/bash
# bash submitPulseShapeAnalysisOneRange.sh 271983 272818

firstRun=$1
lastRun=$2
filelist=$3

# these settings are for release CMSSW_9_0_3
export SCRAM_ARCH=slc6_amd64_gcc530
afsdir=/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_9_0_3/src/EcalReconstruction/EcalTools/macro
cd $afsdir
eval `scramv1 runtime -sh`
echo "running the analysis from dir: " $afsdir

root -b <<EOF

.L PulseShapeAnalysis.cxx+
saveAllTemplatesOneRange("$filelist",$firstRun,$lastRun)
.q

EOF

cp *root *txt $afsdir

echo "done."
