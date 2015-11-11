#! /usr/bin/env python
import os
import sys
import re

ranges = [ "248036_257615", "257616_258600", "258601_999999" ]

for r in ranges:
    print "processing run range ",r
    
    label="runs_"+r
    
    os.system("cmsRun Ecal_PulseCovariances_argv_cfg.py "+label+" "+label)
