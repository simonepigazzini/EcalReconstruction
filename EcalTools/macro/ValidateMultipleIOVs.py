#! /usr/bin/env python
import os
import sys

# N-1 update
#currentPromptIOV = ('IOV6','/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_276315_276587.txt')

#toValidateIOVs = [('IOV7','/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_277932_277992.txt')]

currentPromptIOV = ('IOV7','/afs/cern.ch/work/e/emanuele/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_277932_277992.txt')

toValidateIOVs = [('IOV8','/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_278969_278976.txt'),
                  ('IOV9','/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_279115_279116.txt'),
                  ('IOV10','/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_279653_279658.txt'),
                  ('IOV11','/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/template_histograms_ECAL_Run2016_runs_279841_279841.txt')]

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
            
