#! /bin/bash
# bash submitPulseShapeAnalysisOneRange.sh 271983 272818

firstRun=$1
lastRun=$2
filelist=$3

# these settings are for release CMSSW_9_0_3
export SCRAM_ARCH=$SCRAM_ARCH
afsdir=$PWD
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
