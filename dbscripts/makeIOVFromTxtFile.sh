#! /bin/bash

ebinput=$1
eeinput=$2
output=$3

echo "merging EB file " $ebinput " with EE file " $eeinput " to make IOV file " $output
awk '$1==0 {print;}' $ebinput > $output
awk '$1==1 {print;}' $eeinput >> $output
echo "done. Check the output in " $output

