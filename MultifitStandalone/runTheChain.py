#! /usr/bin/env python
import ROOT
import os

if __name__ == "__main__":

    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] ")
    parser.add_option("-d","--dryrun", dest="dryrun",  action="store_true", help="only print the commands, do not run")
    
    print "NOW SIMULATING THE WAVEFORMS WITH PU..."
    ROOT.gROOT.ProcessLine(".L Example02_vsPU.C+")    
    ROOT.gROOT.ProcessLine(".L Example04_vsPU.C+")    
    os.system("g++ -o Example06_weights_vsPU Example06_weights_vsPU.cc -std=c++11 `root-config --cflags --glibs`")
    os.system("g++ -o Example06_vsPU Example06_vsPU.cc PulseChiSqSNNLS.cc -std=c++11 `root-config --cflags --glibs`")
    for pu in range(40,41,5):
        example2 = ROOT.Example02(pu)
        example4 = ROOT.Example04(pu)
        #os.system("./Example06_vsPU {npu}".format(npu=pu))
        #os.system("./Example06_weights_vsPU {npu}".format(npu=pu))
