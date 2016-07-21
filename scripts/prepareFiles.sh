#! /bin/bash

ranges="274958_275659 275757_275931 276315_276587"
for r in $ranges ; do
    echo "considering range = " $r
    bash scripts/mergeFiles.sh $r
    bash scripts/stripTxtFile.sh template_histograms_ECAL_runs_$r.txt
done
