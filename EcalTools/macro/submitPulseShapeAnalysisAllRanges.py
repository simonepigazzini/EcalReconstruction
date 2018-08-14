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
#    274958:275281, # 2016B IOV 4.1
#    275282:275376 # 2016B IOV 4.2
#    275656:275847 # 2016C IOV 5.1
#    275886:276097 # 2016C IOV 5.2
#    276282:276502, # Run2016D IOV 6.1
#    276525:276659, # Run2016D IOV 6.2
#    276775:276811, # Run2016D IOV 6.3
#    279715:279716, # Run2016G IOV 10.2
#    279993:280024, # Run2016G IOV 11.2
#    280327:280364 # Run2016G IOV 11.3
#    282408:282712, # Run2016H 13.1
#    282730:282814, # Run2016H 13.2
#    282842:283067, # Run2016H 13.3
#    314093:314094 # first unstable beams 2018
#    314474:314530 # first stable beams 2018 (TR shift in EB-05): IOV1
#    315255:315267 # after the delays changes in the EE: IOV2
#    315488:315512 # after 1/fb of irradiation: IOV3
#    315721:316153 # plateau of timing, finally: IOV4
#    316758:316758 # after the change of channel TokenRing8 in EE+06 22/05/2018: IOV5
#    316876:316928 # after the tuning of new TR8 in EE+06: IOV6
#    316944:317089 # after that EB+17 has returned in the original position
#    318438:318464 # unstable collisions of Run2018B after delays change (again, EB+EE)
#    318622:318622 # stable collisions after TS1 (but still small dataset) --- not used --- 
#    318816:318828 # stable collisions after TS1 (~70/pb)
#    319697:319840 # collisions after some integrated lumi, fill with 2 isolated bunches
#
### 2017
#
# 2017B # remaking the ntuples on LSF
#    297046:297101,
#    297113:297316,
#    297359:297505,
#    297557:297723,
#    298809:299067,
#    299096:299329,
# 2017C
# 
#    299368:299450, #OK
#    299477:299650, #OK
#    300079:300124, # OK
#    300155:300401, # OK
#     300459:300742, #OK
#     300777:301165, #OK
#     301330:301480, #OK
#     301519:301914, # manca EE
#     301941:302044, #OK
# 2017D
#    302131:302280, #OK
#    302322:302479, # manca tutto
#    302484:302555, #OK
#    302563:302678, #OK
# 2017E
#    303817:303832,
#    303838:304062,
#    304119:304354,
#    304366:304655,
#    304661:304797,
# Run2017F
#    305040:305114,
#    305178:305252,
#    305310:305518,
#    305586:305842,
     305902:306126,
     306134:306462,

# 2018D (for prompt reco)
#    321162:321218
    }

pwd = os.getcwd()

for start,stop in ranges.iteritems():
    for iecal in xrange(2):
        comm = 'bsub -q 1nw -J '+str(start)+'_'+str(stop)+' -o psana_runs_'+str(start)+'_'+str(stop)+'.log '+pwd+'/submitPulseShapeAnalysisOneRange.sh '+str(start)+' '+str(stop)+' '+args[0]+' '+str(iecal)
        if options.create: print comm
        else: os.system(comm)

print 'DONE.'

