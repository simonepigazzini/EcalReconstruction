#! /bin/bash

input=$1
output=$2
echo converting $input
awk '{ if($3==0){printf "1\t"} else{printf "0\t"} ; {printf "%s\t",$16} ; for(i=4;i<NF;++i){printf "%s\t",$i} ; {printf "\n"} }' $input > $output
echo converted into $output
