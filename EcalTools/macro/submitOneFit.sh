#! /bin/bash
# bash submitOneFit.sh template_histograms_ECAL_Run2016_runs_283863_284035.txt norm_template_histograms_ECAL_Run2016_runs_283863_284035.txt

inputFile=$1
outputFile=$2

# these settings are for release CMSSW_8_0_5
export SCRAM_ARCH=slc6_amd64_gcc530
afsdir=/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_8_0_8/src/EcalReconstruction/EcalTools/macro
cd $afsdir
eval `scramv1 runtime -sh`
echo "running the analysis from dir: " $afsdir

python fitPulseShapes.py $inputFile $outputFile
cp *txt $afsdir

echo "done"
