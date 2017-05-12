#! /usr/bin/env python
import os
import sys

if __name__ == "__main__":
    from sys import argv

    pwd = os.getcwd()
    files = []
    for line in open(argv[1],'r'):
        strippedline = line.rstrip()
        print "submitting fits for file: ",strippedline
        filen = line.split("/")
        txtfile = filen[-1].rstrip()
        normtxtfile = "norm_"+txtfile
        logfile = "log_"+txtfile
        base = (txtfile.split("_",4)[-1]).split(".")[0]
        comm = "bsub -q cmscaf1nd -J "+base+" -o "+logfile+" "+pwd+"/submitOneFit.sh"+" "+strippedline+" "+normtxtfile
        print comm
        os.system(comm)
