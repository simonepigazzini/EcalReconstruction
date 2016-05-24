#! /usr/bin/env python
import os
import sys
import re

ranges = [ "271983_272818", "273301_273590" ]

for r in ranges:
    print "processing run range ",r
    
    label="_runs_"+r
    
    os.system("cmsRun Ecal_PulseShapes_argv_cfg.py "+label+" "+label)
