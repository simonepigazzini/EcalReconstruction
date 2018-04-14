#! /bin/bash

#ranges="304797_304797 305814_305814 305832_305842 306121_306126"
ranges="314093_314094"
for r in $ranges ; do
    echo "considering range = " $r
    bash scripts/mergeFiles.sh $r
    bash scripts/stripTxtFile.sh template_histograms_ECAL_Run2018_runs_$r.txt
done
