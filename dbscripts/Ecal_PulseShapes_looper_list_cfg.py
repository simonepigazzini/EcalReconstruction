#! /usr/bin/env python
import os
import sys
import re

#ranges = ["257394_258215", "258287_258714", "258741_258750"]
ranges = [ "257615_258600", "258600_259700" ]

for r in ranges:
    print "processing run range ",r
    
    label="runs_"+r
    
    os.system("cmsRun Ecal_PulseShapes_argv_cfg.py "+label+" "+label)
