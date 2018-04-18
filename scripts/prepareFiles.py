#! /usr/bin/env python
# USAGE: ./scripts/prepareFiles.py 314474_314530
import os
import sys

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] range1 range2...")
    parser.add_option("-p","--prefix", dest="prefix",  type="string", default="template_histograms_ECAL_Run2018", help="Prefix for the file")
    (options, args) = parser.parse_args()
    if len(args)<1: raise RuntimeError, 'Expecting at least one run range in the form runmin_runmax'

    ranges=args[:]
    prefix=options.prefix
    dbdir="/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/"

    for rrange in ranges:
        runs=[int(r) for r in rrange.split('_')]
        if len(runs)!=2: 
            print "Malformed run range: %s. Skipping." % rrange
            continue
        print bcolors.BOLD + ("Preparing txt files for run range: [%d,%d]" % (runs[0],runs[1])) + bcolors.ENDC
        os.system("scripts/mergeFiles.sh %s" %rrange)
        txtfile="%s_runs_%s.txt" % (prefix,rrange)
        os.system("scripts/stripTxtFile.sh {txt}".format(txt=txtfile))
        num_lines = sum(1 for line in open(txtfile))
        print bcolors.OKGREEN + "TXT file {txt} has {ncry} crystals".format(txt=txtfile,ncry=num_lines) + bcolors.ENDC
        os.system("mv -i {txt} {dbdir}".format(txt=txtfile,dbdir=dbdir))

        print "Now preparing the sqlite tag..."
        os.system("cmsRun dbscripts/Ecal_PulseShapes_argv_cfg.py "+rrange+" "+prefix)
        os.system("mv -i ecaltemplates_popcon_runs_{runs}.db {dbdir}".format(runs=rrange,dbdir=dbdir))
        os.system("conddb --db {dbdir}/ecaltemplates_popcon_runs_{runs}.db list EcalPulseShapes_data".format(runs=rrange,dbdir=dbdir))
        print bcolors.OKGREEN + "sqlite file ecaltemplates_popcon_runs_{runs}.db is in {dbdir}".format(runs=rrange,dbdir=dbdir) + bcolors.ENDC
