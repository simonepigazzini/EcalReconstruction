#! /usr/bin/env python
import os
import sys
import re

offset = -5.0
while offset <= 5.0:

    suffix = "timeShifted_"+"{0:.6f}".format(offset)+"ns"
    
    suffix_sane = suffix.replace(".","p")
    print "running shapes for ",suffix_sane,"...\n"
    os.system("cmsRun Ecal_PulseShapes_argv_cfg.py "+suffix_sane+" "+suffix)
    
    offset += 0.250
