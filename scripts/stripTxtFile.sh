#! /bin/bash

input=$1

echo stripping $input ...
awk '{ {printf "%s\t",$1} ; for(i=5;i<=NF;++i){printf "%s\t",$i} ; {printf "\n"} }' $input > tmp
mv $input $input.stripped
mv tmp $input
echo stripped.
