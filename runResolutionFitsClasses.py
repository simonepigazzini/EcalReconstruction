#!/usr/bin/python
import sys
import os
from optparse import OptionParser

parser = OptionParser()
(options, args) = parser.parse_args()

inputfdata="zeeheppytree_25nsruns_256584_256842.root"
outputpref="Run2015D_PromptReco"

#inputfdata="zeeheppytree_25nsruns_mc.root"
#outputpref="Spring15MC"

from ROOT import gROOT
gROOT.LoadMacro('FitZMassScaleAndResolutionHeppyTree.C+')
from ROOT import FitZMassScaleAndResolution

# do the fits in eta,pt bins of the electrons
for r9b in range(0,2):
    print "r9 bin = ",r9b
    for etab in range(0,5):
        print "   eta bin = ",etab
        for p4kind in range(1,2):
            print " local reco type = ",p4kind
            outputfdata="resNVertGt12"+outputpref+"R9Bin"+str(r9b)+"_EtaBin"+str(etab)+"_Reco"+str(p4kind)
            print "        fitting DATA..."
            FitZMassScaleAndResolution(p4kind,inputfdata,outputfdata,r9b,etab,-1)
            print "        output is in file "+outputfdata
