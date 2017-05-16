#!/usr/bin/env python
import sys, commands

fileList = sys.argv[1]

with open(fileList) as file:
    for line in file:
        line = line.strip()
        #output = subprocess.check_output("edmLumisInFiles.py "+line, shell=True)
        status, output = commands.getstatusoutput("edmLumisInFiles.py "+line)
        print "File "+line+"  "+output.strip()

