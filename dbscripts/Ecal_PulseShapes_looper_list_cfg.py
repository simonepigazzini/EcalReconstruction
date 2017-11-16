#! /usr/bin/env python
import os
import sys
import re

ranges = ["304797_304797",
          "305814_305814",
          "305832_305842",
          "306121_306126"]

for r in ranges:
    print "processing run range ",r
    
    label="_runs_"+r
    
    os.system("cmsRun Ecal_PulseShapes_argv_cfg.py "+label+" "+label)
