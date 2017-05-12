#! /usr/bin/env python
import os
import sys
import re

# ranges = [ "271983_272818",
#            "273301_273590",
#            "274080_274344",
#            "274958_275659",
#            "275757_275931",
#            "276315_276587",
#            "277932_277992",
#            "278969_278976",
#            "279115_279116",
#            "279653_279658",
#            "279841_279841",
#            "281616_282092",
#            "282408_283067",
#            "283171_283835",
#            "283863_284035" ]
ranges = [ "278167_278175" ]

for r in ranges:
    print "processing run range ",r
    
    label="_runs_"+r
    
    os.system("cmsRun Ecal_PulseShapes_argv_cfg.py "+label+" "+label)
