#! /usr/bin/env python
import os
import sys
import math
import ROOT as rt
rt.gROOT.SetBatch(True)

class TagValidation:

    def loadTemplates(self,txtfile):
        data = []
        for line in open(txtfile,'r'):
            data.append(line.split())
        return data
    
    def __init__(self,files,options):
        self._options = options
        keys = ["current","ref"]
        tags = dict(zip(keys,files))
        self._allData = {}
        for k,v in tags.iteritems():
            self._allData[k] = self.loadTemplates(v)
            
    def parseDic(self,rawdata):
        mydata = {}
        for item in rawdata:
            key = (item[0],item[1])
            val = item[-12:]
            mydata[key] = val
        return mydata

    def do1dDiff(self,doEB):
        part = 'eb' if doEB else 'ee'
        rt.gStyle.SetOptStat(111111)
        refData = self.parseDic(self._allData["ref"])
        newData = self.parseDic(self._allData["current"])
        histos = []
        for s in range(12): 
            h = rt.TH1D(('%s_diff_1d_sample%d' % (part,s)),"",1000,-0.2,0.2)
            h.GetXaxis().SetTitle('sample_{%d}^{new}-sample_{%d}^{ref}' % (s,s))
            histos.append(h)
        for (partition,detid),samples in refData.iteritems():
            key = (partition,detid)
            if key not in newData: continue
            if ((doEB and partition==0) or (not doEB and partition==1)): continue
            for s in range(12):
                histos[s].Fill(float((newData[key])[s])-float((refData[key])[s]))
        canv = rt.TCanvas("c","",1200,1200)
        canv.SetLogy()
        for s in range(12):
            histos[s].Draw()
            canv.SaveAs('%s_diff_1d_sample%d.pdf' % (part,s))

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] tag1.txt tag2.txt")
    parser.add_option("-p","--partition",  dest="partition",  type="string", default="ALL", help="partitions to be analysed: EB,EE,ALL")
    parser.add_option(     "--do1Ddiff",   dest="do1Ddiff",   action="store_true", help="make the 1D differences of the samples in the two tags")

    (options, args) = parser.parse_args()
    if len(args) < 2: raise RuntimeError, 'Expecting at least two arguments'
        
    tv = TagValidation(args,options)
    
    if options.do1Ddiff:
        if options.partition=='ALL': 
             tv.do1dDiff(True)
             tv.do1dDiff(False)
        else:
            doEB = True if options.partition=='EB' else False
            tv.do1dDiff(doEB)
