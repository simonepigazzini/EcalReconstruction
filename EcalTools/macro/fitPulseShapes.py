#! /usr/bin/env python
import os
import sys
import math
import numpy as np
import ROOT as rt
rt.gROOT.SetBatch(True)

from ecalDetId import EcalDetId
from AlphaBetaFitter import *

class TagFitter:

    def loadTemplates(self,txtfile):
        data = []
        for line in open(txtfile,'r'):
            data.append(line.split())
        return data
    
    def __init__(self,filename):
        self._data = self.loadTemplates(filename)
        self._badFits = 0

    def parseDic(self,rawdata):
        mydata = {}
        for item in rawdata:
            key = (item[0],item[1])
            val = item[-12:]
            mydata[key] = val
        return mydata

    def getPeakNormalisedPulseShape(self,pulseshape,doEB):
        
        histo = rt.TH1F("histo","",15,0,15)        
        # fill the template and fit it
        for s in range(3):  histo.SetBinContent(s+1,0)
        for s in range(12): histo.SetBinContent(s+4,float(pulseshape[s]))

        fitter = AlphaBetaFitter( rt.TF1("alphabeta",alphabeta,0,10,5), doEB)
        results = fitter.fit(histo,doEB)
        fcn = fitter.getFcn()
        chi2 = fcn.GetChisquare()
        dt = fcn.GetParameter(3)-5.5
        valAtMax = fcn.Eval(5.5+dt)

        ret = []
        for s in range(4,16):
            if chi2 < 5:
                ret.append(histo.GetBinContent(s)/valAtMax)
            else:
                ret.append(histo.GetBinContent(s))
                self._badFits += 1
                           
        return ret

    def writeNormalisedTag(self,fileout):
        fout = open(fileout,"w")
        myData = self.parseDic(self._data)
        ncry=0
        for (partition,detid),pulseshape in myData.iteritems():
            if ncry % 1000 == 0: print "fitted ",ncry," pulse shapes"
            key = (partition,detid)
            normshape = self.getPeakNormalisedPulseShape(pulseshape,partition)
            fout.write(partition+"  "+detid+"  "+"  ".join("%.6f" % x for x in normshape)+"\n")
            ncry += 1
        print "Bad Fits = ",self._badFits
        

if __name__ == "__main__":
    from sys import argv
    
    tf = TagFitter(argv[1])
    tf.writeNormalisedTag(argv[2])

    
