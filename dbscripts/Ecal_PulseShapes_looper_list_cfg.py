#! /usr/bin/env python
import os
import sys
import re

ranges = [ "274958_275659", "275757_275931", "276315_276587" ]

for r in ranges:
    print "processing run range ",r
    
    label="_runs_"+r
    
    os.system("cmsRun Ecal_PulseShapes_argv_cfg.py "+label+" "+label)
