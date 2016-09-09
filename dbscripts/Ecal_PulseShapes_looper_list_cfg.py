#! /usr/bin/env python
import os
import sys
import re

ranges = [ "278969_278976", "279115_279116", "279653_279658", "279841_279841" ]

for r in ranges:
    print "processing run range ",r
    
    label="_runs_"+r
    
    os.system("cmsRun Ecal_PulseShapes_argv_cfg.py "+label+" "+label)
