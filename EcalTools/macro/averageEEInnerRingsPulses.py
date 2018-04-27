#! /usr/bin/env python
import os,sys
import math
from TagValidation import TagValidation
from EndcapAsymmetry import etaRingMapping
from ecalDetId import EcalDetId

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] oldtag.txt newtag.txt")
    (options, args) = parser.parse_args()

    txt = args[0]
    outtxt = args[1]
    print "scanning ",txt

    maxIr = 20
    minCryFrac = 0.8

    tv = TagValidation([txt],options)
    data = tv.parseDic(tv._allData["current"])
    
    ebdata = {(partition,detid):samples for (partition,detid),samples in data.iteritems() if partition=='1'}
    eedata = {(partition,detid):samples for (partition,detid),samples in data.iteritems() if partition=='0'}
    
    print "Original has {neb} crystals in EB and {nee} in EE".format(neb=len(ebdata),nee=len(eedata))

    # some geometry
    detids = EcalDetId('/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/detids_ECAL.txt')
    ringmap = etaRingMapping()

    averages = {}
    filledCry = {}
    irCry = {}

    print "Starting the first loop..."
    for (partition,detid),samples in eedata.iteritems():
        (x,y,z) = detids.xyz(detid)
        x0=x-50; y0=y-50
        ir = int(math.sqrt(x0*x0+y0*y0))
        iring = ringmap.getRing(False,x,y,z)
        if iring not in averages: 
            averages[iring] = samples
            filledCry[iring] = 1
            irCry[iring] = ir
        else:
            averages[iring] = [float(averages[iring][s])+float(samples[s]) for s in xrange(len(samples))]
            filledCry[iring] += 1

    print "Starting the second loop..."
    eenewdata = {}
    partition = '0'
    total=0; unchanged=0; averaged=0; averagedtoneighbor=0;
    for detid,val in detids.geometry.iteritems():
        key=(partition,detid)
        (x,y,z) = detids.xyz(detid)
        if detids.subdet(detid)==1: continue # barrel
        x0=x-50; y0=y-50
        ir = int(math.sqrt(x0*x0+y0*y0))
        iring = ringmap.getRing(False,x,y,z)
        #if iring==-1: 
        #    continue # not valid x,y
        total+=1
        if ir>maxIr: 
            if key in eedata:
                eenewdata[key] = eedata[key]
                unchanged+=1
            else:
                pass # leave empty crystals that have no measurements in low eta EE
        else:
            if iring not in filledCry: 
                print "Warning! Completely empty ring: ",iring
                continue
            if filledCry[iring]>100: 
                eenewdata[key] = [s/float(filledCry[iring]) for s in averages[iring]]
                averaged+=1
            else: 
                rring = iring
                while filledCry[rring]<100: rring -= 1
                #print "ring ",iring, "has ",filledCry[iring]," filled crystals. The closest ring with 100 crystals is ",rring
                eenewdata[key] = [s/float(filledCry[rring]) for s in averages[rring]]
                averagedtoneighbor+=1
    
    newdata = eenewdata
    newdata.update(ebdata)

    print "EE: Total of ",total," channels. Unchanged wrt measure: ",unchanged," averaged to ring: ",averaged," averaged to closest neighbor ring: ",averagedtoneighbor

    print "writing the new txt file into ",outtxt
    newfile = open(outtxt,'w')
    for (partition,detid),samples in newdata.iteritems():
        samples_str = ["%.6f" % float(s) for s in samples]
        vals = [partition,detid]+samples_str
        newfile.write('\t'.join(vals)+'\n')
