#! /usr/bin/env python
import os
from math import *
from TagValidation import *


class etaRingMapping:
    def __init__(self):
        Endc_x_y_ring=("%s/src/EcalReconstruction/data/Endc_x_y_ring.txt" % os.environ['CMSSW_BASE'])
        self.eeringmap = {}
        for line in open(Endc_x_y_ring,'r'):
            item = line.split()
            key = (int(item[0]),int(item[1]))
            self.eeringmap[key] = int(item[3])

        self.ebringmap = {}
        for iz in range(-1,2,2):
            for ieta in range(1,86):
                for iphi in range(1,361):
                    key = (ieta,iphi,iz)
                    self.ebringmap[key] = iz*ieta
                    
    def getRing(self,doEB,x,y,z):
        if doEB: return self.ebringmap[(x,y,z)]
        else: return self.eeringmap[(x,y)] if (x,y) in self.eeringmap else -1

def asymmetryByRing(tv,data):

    customROOTstyle()
    rt.gStyle.SetOptStat(0)

    histos = {}
    histos['eeplus'] = []
    histos['eeminus'] = []
    histos['eediff'] = []
    for s in range(12): 
        hp = rt.TProfile(('endcap_plus_ring_sample%d' % s),"",39,0,39)
        hp.GetXaxis().SetTitle('#eta ring')
        hp.SetTitle('average sample %d' %s)
        histos['eeplus'].append(hp)
        hm = hp.Clone('endcap_minus_ring_sample%d' % s)
        histos['eeminus'].append(hm)
        hd = rt.TH1D(('endcap_diff_ring_sample%d' % s),"",39,0,39)
        hd.GetXaxis().SetTitle('#eta ring')
        hd.SetTitle('sample %d (EE^{+}-EE^{-})/(EE^{+}+EE^{-})' %s)
        hd.GetYaxis().SetLimits(-0.01,0.01)
        histos['eediff'].append(hd)
        
    print "Loading detIds mapping..."
    detids = EcalDetId('/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/detids_ECAL.txt')
    print "Loading EE rings..."
    ringmap = etaRingMapping()

    for (partition,detid),samples in data.iteritems():
        key = (partition,detid)
        (x,y,z) = detids.xyz(detid)

        if int(partition)==1: continue # skip barrel

        htofill = histos['eeplus'] if z>0 else histos['eeminus']
        ring = ringmap.getRing(False,x,y,z)
        for s in range(12):
            htofill[s].Fill(ring,float(data[key][s]))
            
    xsize = 1200
    ysize = int(xsize*0.9)
    canv = rt.TCanvas("c","",xsize,ysize)

    for s in range(12):
        for b in range(histos['eeplus'][s].GetNbinsX()+1):
            reldiff = (histos['eeplus'][s].GetBinContent(b)-histos['eeminus'][s].GetBinContent(b))/(histos['eeplus'][s].GetBinContent(b)+histos['eeminus'][s].GetBinContent(b))
            relerr = hypot(histos['eeplus'][s].GetBinError(b)/histos['eeplus'][s].GetBinContent(b),
                           histos['eeplus'][s].GetBinError(b)/histos['eeplus'][s].GetBinContent(b))
            histos['eediff'][s].SetBinContent(b,reldiff)
            histos['eediff'][s].SetBinError(b,relerr)
        tv.printOnePlot((histos['eediff'])[s],canv,'endcap_diff_ring_sample%d' % s )
        tv.printOnePlot((histos['eeplus'])[s],canv,'endcap_plus_ring_sample%d' % s )
        tv.printOnePlot((histos['eeminus'])[s],canv,'endcap_minus_ring_sample%d' % s )

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] tag.txt")
    parser.add_option("--pdir", "--print-dir", dest="printDir", type="string", default="iovX", help="print out plots in this directory");
    (options, args) = parser.parse_args()

    tv = TagValidation(args,options) 
    tv._options.printDir = '/afs/cern.ch/user/e/emanuele/www/ECAL/reco/PSTag/Legacy2016/'+options.printDir
    tv._options.printPlots =  'pdf,png'
    data = tv.parseDic(tv._allData["current"])
    asymmetryByRing(tv,data)
