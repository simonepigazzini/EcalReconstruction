#! /usr/bin/env python
import os
import sys
import re

ranges = ["306896_307082"]

for r in ranges:
    print "processing run range ",r
    
    label="_runs_"+r
    
    os.system("cmsRun Ecal_PulseShapes_argv_cfg.py "+label+" "+label)
