#! /bin/bash

ranges="257826_258600 258601_259896"
for r in $ranges ; do
    echo "considering range = " $r
    bash scripts/mergeFiles.sh $r
    bash scripts/stripTxtFile.sh template_histograms_ECAL_runs_$r.txt
done
