#! /usr/bin/env python
import os
import sys
from TagValidation import *

from optparse import OptionParser
parser = OptionParser(usage="%prog [options] detid")
(options, args) = parser.parse_args()
options.printDir = "plots/history"
options.printPlots = "png,pdf"

tempdir = "/afs/cern.ch/user/e/emanuele/w/public/ecal/pulseshapes_db/"
iovs = []
iovs.append(tempdir+"template_histograms_ECAL_Run2016B_runs_271983_272818.txt")
iovs.append(tempdir+"template_histograms_ECAL_Run2016B_runs_273301_273590.txt")
iovs.append(tempdir+"template_histograms_ECAL_Run2016B_runs_274080_274344.txt")
iovs.append(tempdir+"template_histograms_ECAL_Run2016_runs_274958_275659.txt")
iovs.append(tempdir+"template_histograms_ECAL_Run2016_runs_275757_275931.txt")
iovs.append(tempdir+"template_histograms_ECAL_Run2016_runs_276315_276587.txt")
iovs.append(tempdir+"template_histograms_ECAL_Run2016_runs_277932_277992.txt")
iovs.append(tempdir+"template_histograms_ECAL_Run2016_runs_278969_278976.txt")
iovs.append(tempdir+"template_histograms_ECAL_Run2016_runs_279653_279658.txt")
iovs.append(tempdir+"template_histograms_ECAL_Run2016_runs_279115_279116.txt")
iovs.append(tempdir+"template_histograms_ECAL_Run2016_runs_279841_279841.txt")
iovs.append(tempdir+"template_histograms_ECAL_Run2016_runs_281616_282092.txt")
iovs.append(tempdir+"template_histograms_ECAL_Run2016_runs_282408_283067.txt")
iovs.append(tempdir+"template_histograms_ECAL_Run2016_runs_283171_283835.txt")
iovs.append(tempdir+"template_histograms_ECAL_Run2016_runs_283863_284035.txt")

tv = TagValidation([],options)
tv.historyPlot(args[0],iovs)
            
