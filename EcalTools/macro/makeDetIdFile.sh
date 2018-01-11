#! /bin/bash

input=$1
output=$2
echo converting $input
awk '{ {printf "%s\t",$16} ; if($3==0){printf "1\t"} else{printf "0\t"} ; {printf $1"\t"}; {printf $2"\t"} ; {printf $3"\t"}; {printf "\n"} }' $input > $output
echo converted into $output
