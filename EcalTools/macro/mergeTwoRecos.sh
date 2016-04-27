#! /bin/sh

stdrecofile=$1
newrecofile=$2
merged=$3

echo "merging std reco: " $1 " and mod reco " $2 " in " $3

cat $stdrecofile $newrecofile > $merged\_merged
sort $merged\_merged > $merged\_sorted

mv $merged\_sorted $merged
rm -f $merged\_merged

echo "DONE. Final file is " $merged
