#! /bin/bash

#ranges="281616_282092 282408_283067 283171_283835 283863_284035"
ranges="300574_300576 301141_301165 301472_301480 "
for r in $ranges ; do
    echo "considering range = " $r
    bash scripts/mergeFiles.sh $r
    bash scripts/stripTxtFile.sh template_histograms_ECAL_Run2017_runs_$r.txt
done
