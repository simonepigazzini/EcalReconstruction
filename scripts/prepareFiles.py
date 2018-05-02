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

    dbdir="/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/"

    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] range1 range2...")
    parser.add_option("-p","--prefix", dest="prefix", type="string", default="template_histograms_ECAL_Run2018", help="Prefix for the file")
    parser.add_option("-b","--backup", dest="backup", action="store_true", default=False, help="Backup the file in the dir: {dbdir}".format(dbdir=dbdir))
    parser.add_option(     "--doNotAverageEE", dest="doNotAverageEE", action="store_true", default=False, help="don't average the inner endcap rings dynamically (default is average)")
    (options, args) = parser.parse_args()
    if len(args)<1: raise RuntimeError, 'Expecting at least one run range in the form runmin_runmax'

    ranges=args[:]
    prefix=options.prefix

    print bcolors.OKBLUE + "Now preparing the basic txt files..." + bcolors.ENDC
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

        if options.doNotAverageEE==False:
            print bcolors.OKBLUE + "AVERAGING THE INNER ENDCAP RINGS WHICH HAVE NOT MUCH STATISTICS..." + bcolors.ENDC
            os.system("{cmsswbase}/src/EcalReconstruction/EcalTools/macro/averageEEInnerRingsPulses.py {txtfile}".format(cmsswbase=os.environ['CMSSW_BASE'],txtfile=txtfile))
            os.system("mv {base}.txt {base}_ee_original.txt; mv {base}_ee_averaged.txt {base}.txt".format(base= txtfile.replace('.txt','')))

        print bcolors.OKBLUE + "Now preparing the sqlite tag..." + bcolors.ENDC
        dbfile = "ecaltemplates_popcon_runs_{runs}.db".format(runs=rrange)
        if os.path.isfile(dbfile):
            print "The sqlite file ",dbfile," is already existing here. Removing it and replacing not to append an IOV!"
            os.system("rm -f {dbfile}".format(dbfile=dbfile))
        os.system("cmsRun dbscripts/Ecal_PulseShapes_argv_cfg.py "+rrange+" "+prefix)
        os.system("conddb --db ecaltemplates_popcon_runs_{runs}.db list EcalPulseShapes_data".format(runs=rrange))

        if options.backup:
            os.system("mv -i {txt} {dbdir}".format(txt=txtfile,dbdir=dbdir))
            os.system("mv -i {dbfile} {dbdir}".format(dbfile=dbfile,dbdir=dbdir))
            print bcolors.OKGREEN + "sqlite file ecaltemplates_popcon_runs_{runs}.db is in {dbdir}".format(runs=rrange,dbdir=dbdir) + bcolors.ENDC
