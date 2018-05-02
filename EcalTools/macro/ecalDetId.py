#! /usr/bin/env python
import os

class EcalDetId:

    def __init__(self,txtfile):
        self.geometry = {}
        for line in open(txtfile,'r'):
            item = line.split()
            (key,val) = (item[0],item[1:])
            self.geometry[key] = val

    def subdet(self,detid):
        if detid not in self.geometry: return -1
        return int((self.geometry[detid])[0])

    def xyz(self,detid):
        if detid not in self.geometry: 
            print "WARNING! DetId ",detid," not in the geometry file. Not used in validation. Please check inputs."
            return(-999,-999,-999)
        coordinates = (self.geometry[detid])[-3:]
        return [int(i) for i in coordinates]

class etaRingMapping:
    def __init__(self):
        Endc_x_y_ring=("%s/src/EcalReconstruction/data/Endc_x_y_ring.txt" % os.environ['CMSSW_BASE'])
        self.eeringmap = {}
        self.eeringcrystals = {}
        for line in open(Endc_x_y_ring,'r'):
            item = line.split()
            key = (int(item[0]),int(item[1]))
            # map x,y -> ring
            thering = int(item[3])
            self.eeringmap[key] = thering
            # map ring -> vector of crystals
            if not thering in self.eeringcrystals:
                self.eeringcrystals[thering] = [(int(item[0]),int(item[1]))]
            else:
                self.eeringcrystals[thering].append((int(item[0]),int(item[1])))

        self.ebringmap = {}
        for iz in range(-1,2,2):
            for ieta in range(1,86):
                for iphi in range(1,361):
                    key = (ieta,iphi,iz)
                    self.ebringmap[key] = iz*ieta
                    
    def getRing(self,doEB,x,y,z):
        if doEB: return self.ebringmap[(x,y,z)]
        else: return self.eeringmap[(x,y)] if (x,y) in self.eeringmap else -1

    def getNCrystals(self,doEB,ring):
        if doEB: return 360
        else: 
            if ring==-1: return 0
            if ring not in self.eeringcrystals: 
                print "WARNING! Ring ",ring," not valid. Returning 0 crystals."
                return 0
            else: return len(self.eeringcrystals[ring])


if __name__ == "__main__":
    edi = EcalDetId('/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/detids_ECAL.txt')

