#! /bin/bash

ranges="306896_307082"
for r in $ranges ; do
    echo "considering range = " $r
    bash scripts/mergeFiles.sh $r
    bash scripts/stripTxtFile.sh template_histograms_ECAL_Run2017_runs_$r.txt
done
