#! /usr/bin/env python
import os
import sys

currentPromptIOV = ('IOV3','/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016B_runs_274080_274344.txt')

toValidateIOVs = [('IOV4','/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_274958_275659.txt'),
                  ('IOV5','/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_275757_275931.txt'),
                  ('IOV6','/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_276315_276587.txt') ]

allIOVs = [currentPromptIOV] + toValidateIOVs

for iov,txtfile in allIOVs:
    if iov == currentPromptIOV[0]: continue
    print "Validating ", iov, " reading file ", txtfile, "..."
    reftxt = (allIOVs[allIOVs.index((iov,txtfile))-1])[1]
    print "---> Reference IOV = ", reftxt
    for partition in ['EB','EE']:
        for dimOption in ['--do1Ddiff','--do2Ddiff']:
            cmd = 'python TagValidation.py -p ' + partition + ' ' + dimOption + ' ' + txtfile + ' ' + reftxt + ' --pdir plots/' + iov + ' --print png,pdf'
            print "Command to run = ",cmd
            os.system(cmd)
            fcmd = open('plots/'+iov+'/command.txt','w')
            fcmd.write(cmd)
            
