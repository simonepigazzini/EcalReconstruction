#! /usr/bin/env python
import os
import sys

# N-1 update
currentPromptIOV = ('IOV11','/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_279841_279841.txt')

toValidateIOVs = [('IOV12','/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_281616_282092.txt'),
                  ('IOV13','/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_282408_283067.txt'),
                  ('IOV14','/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_283171_283835.txt'),
                  ('IOV15','/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_283863_284035.txt')]

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
            
