#! /usr/bin/env python
import os

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] filelist.txt")
    parser.add_option("-c","--create", dest="create",  action="store_true", default=False, help="do not submit jobs, only show the submission command")
    (options, args) = parser.parse_args()
    if len(args) < 1: raise RuntimeError, 'Expecting at least the filelist as argument'

# CUSTOMIZE THE RUN RANGES HERE
ranges = {
#    314093:314094 # first unstable beams 2018
#    314474:314530 # first stable beams 2018 (TR shift in EB-05): IOV1
#    315255:315267 # after the delays changes in the EE: IOV2
#    315488:315512 # after 1/fb of irradiation: IOV3
#    315721:316153 # plateau of timing, finally: IOV4
#    316758:316758 # after the change of channel TokenRing8 in EE+06 22/05/2018: IOV5
#    316876:316928 # after the tuning of new TR8 in EE+06: IOV6
    316944:317089 # after that EB+17 has returned in the original position
    }

pwd = os.getcwd()

for start,stop in ranges.iteritems():
    comm = 'bsub -q cmscaf1nw -J '+str(start)+'_'+str(stop)+' -o psana_runs_'+str(start)+'_'+str(stop)+'.log '+pwd+'/submitPulseShapeAnalysisOneRange.sh '+str(start)+' '+str(stop)+' '+args[0]
    if options.create: print comm
    else: os.system(comm)

print 'DONE.'

