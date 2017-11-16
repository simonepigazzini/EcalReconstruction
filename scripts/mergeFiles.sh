#! /bin/bash

range=$1
echo Merging EB and EE files for range $range
cat template_histograms_EB_runs_$range.txt template_histograms_EE_runs_$range.txt > template_histograms_ECAL_Run2017_runs_$range.txt
echo Merged.
