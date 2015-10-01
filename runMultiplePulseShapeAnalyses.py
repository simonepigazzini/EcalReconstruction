#!/usr/bin/python
import sys
import os
import json
from pprint import pprint
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-p", "--partition", dest="partition", default="barrel",
                  help="specify if barrel (default) or endcap")
parser.add_option("-i", "--input", dest="input",
                  help="input file with the run list")
(options, args) = parser.parse_args()


from ROOT import gROOT
gROOT.LoadMacro('PulseShapeAnalysis.cxx+')
from ROOT import saveTemplates

with open(options.input) as data_file:    
    data = json.load(data_file)

for chunk in data:
    begin_run = data[chunk][0]
    end_run = data[chunk][1]
    dobarrel = True if option.partition == "barrel" else False
    print "Running ",option.partition," for run range [",begin_run,",",end_run,"]\n"
    saveTemplates(dobarrel,begin_run,end_run)
