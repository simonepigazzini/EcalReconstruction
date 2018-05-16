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
#    314474:314530 # first stable beams 2018 (TR shift in EB-05)
#    315255:315267 # after the delays changes in the EE
#    315488:315512 # after 1/fb of irradiation
     315721:316153 # plateau of timing, finally
    }

pwd = os.getcwd()

for start,stop in ranges.iteritems():
    comm = 'bsub -q cmscaf1nw -J '+str(start)+'_'+str(stop)+' -o psana_runs_'+str(start)+'_'+str(stop)+'.log '+pwd+'/submitPulseShapeAnalysisOneRange.sh '+str(start)+' '+str(stop)+' '+args[0]
    if options.create: print comm
    else: os.system(comm)

print 'DONE.'

