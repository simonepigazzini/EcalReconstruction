#! /usr/bin/env python
import os
import sys

path='/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/'
currentPromptIOV = ('IOV15',path+'template_histograms_ECAL_Run2017_runs_306121_306126.txt')
toValidateIOVs = [('IOV16',path+'template_histograms_ECAL_Run2017_runs_306896_307082.txt')]

allIOVs = [currentPromptIOV] + toValidateIOVs

for iov,txtfile in allIOVs:
    if iov == currentPromptIOV[0]: continue
    print "Validating ", iov, " reading file ", txtfile, "..."
    reftxt = (allIOVs[allIOVs.index((iov,txtfile))-1])[1]
    print "---> Reference IOV = ", reftxt
    for dimOption in ['--do1Ddiff','--do2Ddiff','--do2DShapeDiff']:
        for partition in ['EB','EE']:
            cmd = 'python TagValidation.py -p ' + partition + ' ' + dimOption + ' ' + txtfile + ' ' + reftxt + ' --pdir plots/' + iov + ' --print png,pdf'
            print "Command to run = ",cmd
            os.system(cmd)
            fcmd = open('plots/'+iov+'/command.txt','w')
            fcmd.write(cmd)
