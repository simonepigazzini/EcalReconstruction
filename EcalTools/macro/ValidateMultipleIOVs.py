#! /usr/bin/env python
import os
import sys

path='/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/'
# Year 2016 Ultra-Legacy
# currentPromptIOV = ('IOV0',path+'template_histograms_ECAL_Run2015C_lowPU.txt')
# toValidateIOVs = [
#     ('IOV1',path+'template_histograms_ECAL_Run2016B_runs_271983_272818.txt'),
#     ('IOV2',path+'template_histograms_ECAL_Run2016B_runs_273301_273590.txt'),
#     ('IOV3',path+'template_histograms_ECAL_Run2016B_runs_274080_274344.txt'),
#     ('IOV4',path+'template_histograms_ECAL_Run2018_runs_274958_275281.txt'),
#     ('IOV5',path+'template_histograms_ECAL_Run2018_runs_275282_275376.txt'),
#     ('IOV6',path+'template_histograms_ECAL_Run2018_runs_275656_275847.txt'),
#     ('IOV7',path+'template_histograms_ECAL_Run2018_runs_275886_276097.txt'),
#     ('IOV8',path+'template_histograms_ECAL_Run2018_runs_276282_276502.txt'),
#     ('IOV9',path+'template_histograms_ECAL_Run2018_runs_276525_276659.txt'),
#     ('IOV10',path+'template_histograms_ECAL_Run2018_runs_276775_276811.txt'),
#     ('IOV11',path+'template_histograms_ECAL_Run2016_runs_278167_278175.txt'),
#     ('IOV12',path+'template_histograms_ECAL_Run2016_runs_278969_278976.txt'),
#     ('IOV13',path+'template_histograms_ECAL_Run2016_runs_279115_279116.txt'),
#     ('IOV14',path+'template_histograms_ECAL_Run2016_runs_279653_279658.txt'),
#     ('IOV15',path+'template_histograms_ECAL_Run2018_runs_279715_279716.txt'),
#     ('IOV16',path+'template_histograms_ECAL_Run2016_runs_279841_279841.txt'),
#     ('IOV17',path+'template_histograms_ECAL_Run2018_runs_279993_280024.txt'),
#     ('IOV18',path+'template_histograms_ECAL_Run2018_runs_280327_280364.txt'),
#     ('IOV19',path+'template_histograms_ECAL_Run2016_runs_281616_282092.txt'),
#     ('IOV20',path+'template_histograms_ECAL_Run2018_runs_282408_282712.txt'),
#     ('IOV21',path+'template_histograms_ECAL_Run2018_runs_282730_282814.txt'),
#     ('IOV22',path+'template_histograms_ECAL_Run2016_runs_283171_283835.txt'),
#     ('IOV23',path+'template_histograms_ECAL_Run2016_runs_283863_284035.txt'),
# ]
# Year 2017 Ultra-Legacy
# currentPromptIOV = ('IOV0',path+'template_histograms_ECAL_Run2016_runs_283863_284035.txt')
# toValidateIOVs = [
#     ('IOV1',path+'template_histograms_ECAL_Run2018_runs_297046_297101.txt'),
#     ('IOV2',path+'template_histograms_ECAL_Run2018_runs_297113_297316.txt'),
#     ('IOV3',path+'template_histograms_ECAL_Run2018_runs_297359_297505.txt'),
#     ('IOV4',path+'template_histograms_ECAL_Run2018_runs_297557_297723.txt'),
#     ('IOV5',path+'template_histograms_ECAL_Run2018_runs_298809_299067.txt'),
#     ('IOV6',path+'template_histograms_ECAL_Run2018_runs_299096_299329.txt'),
#     ('IOV7',path+'template_histograms_ECAL_Run2018_runs_299368_299450.txt'),
#     ('IOV8',path+'template_histograms_ECAL_Run2018_runs_299477_299650.txt'),
#     ('IOV9',path+'template_histograms_ECAL_Run2018_runs_300079_300124.txt'),
#     ('IOV10',path+'template_histograms_ECAL_Run2018_runs_300155_300401.txt'),
#     ('IOV11',path+'template_histograms_ECAL_Run2018_runs_300459_300742.txt'),
#     ('IOV12',path+'template_histograms_ECAL_Run2018_runs_300777_301165.txt'),
#     ('IOV13',path+'template_histograms_ECAL_Run2018_runs_301330_301480.txt'),
#     ('IOV14',path+'template_histograms_ECAL_Run2018_runs_301519_301914.txt'),
#     ('IOV15',path+'template_histograms_ECAL_Run2018_runs_301941_302044.txt'),
#     ('IOV16',path+'template_histograms_ECAL_Run2018_runs_302131_302280.txt'),
#     ('IOV17',path+'template_histograms_ECAL_Run2018_runs_302322_302479.txt'),
#     ('IOV18',path+'template_histograms_ECAL_Run2018_runs_302484_302555.txt'),
#     ('IOV19',path+'template_histograms_ECAL_Run2018_runs_302563_302678.txt'),
#     ('IOV20',path+'template_histograms_ECAL_Run2018_runs_303817_303832.txt'),
#     ('IOV21',path+'template_histograms_ECAL_Run2018_runs_303838_304062.txt'),
#     ('IOV22',path+'template_histograms_ECAL_Run2018_runs_304119_304354.txt'),
#     ('IOV23',path+'template_histograms_ECAL_Run2018_runs_304366_304655.txt'),
#     ('IOV24',path+'template_histograms_ECAL_Run2018_runs_304661_304797.txt'),
#     ('IOV25',path+'template_histograms_ECAL_Run2018_runs_305040_305114.txt'),
#     ('IOV26',path+'template_histograms_ECAL_Run2018_runs_305178_305252.txt'),
#     ('IOV27',path+'template_histograms_ECAL_Run2018_runs_305310_305518.txt'),
#     ('IOV28',path+'template_histograms_ECAL_Run2018_runs_305586_305842.txt'),
#     ('IOV29',path+'template_histograms_ECAL_Run2018_runs_305902_306126.txt'),
#     ('IOV30',path+'template_histograms_ECAL_Run2018_runs_306134_306462.txt'),
#     ]
currentPromptIOV = ('IOV13',path+'template_histograms_ECAL_Run2018_runs_323413_323470.txt')
toValidateIOVs = [ 
    ('IOV14bis',path+'template_histograms_ECAL_Run2018_runs_323841_324022.txt'),
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
