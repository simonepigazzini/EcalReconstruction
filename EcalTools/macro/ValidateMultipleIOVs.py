#! /usr/bin/env python
import os
import sys

path='/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/'
currentPromptIOV = ('IOV0',path+'template_histograms_ECAL_Run2015C_lowPU.txt')
toValidateIOVs = [
('IOV1',path+'template_histograms_ECAL_Run2016B_runs_271983_272818.txt'),
('IOV2',path+'template_histograms_ECAL_Run2016B_runs_273301_273590.txt'),
('IOV3',path+'template_histograms_ECAL_Run2016B_runs_274080_274344.txt'),
('IOV4',path+'template_histograms_ECAL_Run2018_runs_274958_275281.txt'),
('IOV5',path+'template_histograms_ECAL_Run2018_runs_275282_275376.txt'),
('IOV6',path+'template_histograms_ECAL_Run2018_runs_275656_275847.txt'),
('IOV7',path+'template_histograms_ECAL_Run2018_runs_275886_276097.txt'),
('IOV8',path+'template_histograms_ECAL_Run2018_runs_276282_276502.txt'),
('IOV9',path+'template_histograms_ECAL_Run2018_runs_276525_276659.txt'),
('IOV10',path+'template_histograms_ECAL_Run2018_runs_276775_276811.txt'),
('IOV11',path+'template_histograms_ECAL_Run2016_runs_278167_278175.txt'),
('IOV12',path+'template_histograms_ECAL_Run2016_runs_278969_278976.txt'),
('IOV13',path+'template_histograms_ECAL_Run2016_runs_279115_279116.txt'),
('IOV14',path+'template_histograms_ECAL_Run2016_runs_279653_279658.txt'),
('IOV15',path+'template_histograms_ECAL_Run2018_runs_279715_279716.txt'),
('IOV16',path+'template_histograms_ECAL_Run2016_runs_279841_279841.txt'),
('IOV17',path+'template_histograms_ECAL_Run2018_runs_279993_280024.txt'),
('IOV18',path+'template_histograms_ECAL_Run2018_runs_280327_280364.txt'),
('IOV19',path+'template_histograms_ECAL_Run2016_runs_281616_282092.txt'),
('IOV20',path+'template_histograms_ECAL_Run2018_runs_282408_282712.txt'),
('IOV21',path+'template_histograms_ECAL_Run2018_runs_282730_282814.txt'),
('IOV22',path+'template_histograms_ECAL_Run2016_runs_283171_283835.txt'),
('IOV23',path+'template_histograms_ECAL_Run2016_runs_283863_284035.txt'),
]

allIOVs = [currentPromptIOV] + toValidateIOVs

for iov,txtfile in allIOVs:
    if iov == currentPromptIOV[0]: continue
    print "Validating ", iov, " reading file ", txtfile, "..."
    reftxt = (allIOVs[allIOVs.index((iov,txtfile))-1])[1]
    print "---> Reference IOV = ", reftxt
    for dimOption in ['--do1Ddiff','--do2Ddiff','--do2DShapeDiff','--do2DShape']:
        for partition in ['EB','EE']:
            cmd = 'python TagValidation.py -p ' + partition + ' ' + dimOption + ' ' + txtfile + ' ' + reftxt + ' --pdir plots/' + iov + ' --print png,pdf'
            print "Command to run = ",cmd
            os.system(cmd)
            fcmd = open('plots/'+iov+'/command.txt','w')
            fcmd.write(cmd)
