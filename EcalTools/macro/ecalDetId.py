#! /usr/bin/env python

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

if __name__ == "__main__":
    edi = EcalDetId('/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/detids_ECAL_all.txt')

