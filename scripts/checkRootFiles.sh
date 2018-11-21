#!/bin/bash
if [[ "$1" == "" || "$1" == "-h" || "$1" == "--help" ]]; then
    echo "Usage: $0 [ dir ]"
    echo "Will look for chunks and check if there is some one missing. "
    echo "It only looks for holes in the sequence of chunks, " 
    echo "and the presence of zombies if option -z is given. " 
    exit 1;
fi

dir=$1
echo "# Testing for zombies or not correctly closed files";
FILES=$(ls ${dir}/*.root);
id=`shuf -i 1-100 -n 1`
for Z in $FILES; do
    if test -s $Z; then # empty files have already been found
        root -b -l -q $Z >& check_$id.log 
        result=$(grep -E "(nullptr|recover|Zombie)" check_$id.log | wc -l)
        if [ $result -ne 0 ]; then
            echo "$Z     # zombie";
        else
            echo "$Z     # OK";
        fi;
        rm check_$id.log
    fi;
done

