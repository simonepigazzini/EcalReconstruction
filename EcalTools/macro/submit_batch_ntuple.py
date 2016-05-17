#! /usr/bin/env python
# example: submit_batch.py -p test0 -d pccmsrm DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball 1

import os
import sys
import time
import commands
import optparse
import datetime

def main():
#######################################
### usage  
#######################################
    usage = '''usage: %prog [opts] dataset'''
    parser = optparse.OptionParser(usage=usage)
    now = datetime.datetime.now()
    defaultoutputdir='vecbosjob_'+str(now.year)+str(now.month)+str(now.day)+"_"+str(now.hour)+str(now.minute)+str(now.second)
        
    parser.add_option('-q', '--queue',       action='store',     dest='queue',       help='run in batch in queue specified as option (default -q 8nh)', default='8nh')
    parser.add_option('-n', '--nfileperjob', action='store',     dest='nfileperjob', help='split the jobs with n files read/batch job'                , default=10,   type='int')
    parser.add_option('-p', '--prefix',      action='store',     dest='prefix',      help='the prefix to be added to the output'                      , default=defaultoutputdir)
    parser.add_option('-a', '--application', action='store',     dest='application', help='the executable to be run'                                  , default='ecalhitana')
    parser.add_option('-d', '--download',    action='store',     dest='download',    help='download the output on a local computer'                   , default='')
    parser.add_option('-c', '--create',      action='store_true',dest='create',      help='create only the jobs, do not submit them'                  , default=False)
    parser.add_option('-t', '--testnjobs',   action='store',     dest='testnjobs',   help='submit only the first n jobs'                              , default=1000000, type='int')
    (opt, args) = parser.parse_args()

    if len(args) != 1:
        print usage
        sys.exit(1)
    dataset = args[0]

    inputlist = dataset+".list"
    output = dataset

    print "the outputs will be in the directory: "+opt.prefix

    #look for the current directory
    #######################################
    pwd = os.environ['PWD']
    scramarch = os.environ['SCRAM_ARCH']
    #######################################

    if opt.download=='pccmsrm':
        diskoutputdir = "/cmsrm/pc24_2/emanuele/data/EcalReco7.1.X/"
    elif opt.download=='cmsphys06':
        diskoutputdir = "/data1/emanuele/EcalRecoPu7.1.0/"
    else:
        diskoutputdir = pwd
    diskoutputmain = diskoutputdir+"/"+opt.prefix+"/"+output

    os.system("mkdir -p "+opt.prefix+"/"+output)
    os.system("mkdir -p "+opt.prefix+"/"+output+"/log/")
    os.system("mkdir -p "+opt.prefix+"/"+output+"/input/")
    os.system("mkdir -p "+opt.prefix+"/"+output+"/src/")

    if (diskoutputdir != "none" and opt.download!=''): 
        os.system("ssh -o BatchMode=yes -o StrictHostKeyChecking=no pccmsrm24 mkdir -p "+diskoutputmain)

    inputListfile=open(inputlist)
    inputfiles = inputListfile.readlines()
    ijob=0

    while (len(inputfiles) > 0):
        inputfilename = pwd+"/"+opt.prefix+"/"+output+"/input/input_"+str(ijob)+".list"
        inputfile = open(inputfilename,'w')
        for line in range(min(opt.nfileperjob,len(inputfiles))):
            ntpfile = inputfiles.pop()
            if ntpfile != '':
                inputfile.write(ntpfile)

        inputfile.close()

        # prepare the script to run
        outputname = opt.prefix+"/"+output+"/src/submit_"+str(ijob)+".src"
        outputfile = open(outputname,'w')
        outputfile.write('#!/bin/bash\n')
        outputfile.write('export SCRAM_ARCH='+scramarch+'\n')
        outputfile.write('cd '+pwd+'\n')
        outputfile.write('eval `scramv1 runtime -sh`\n')
        outputfile.write('cd $WORKDIR\n')
        outputfile.write(pwd+'/'+opt.application+' '+inputfilename+" "+output+"_"+str(ijob)+".root \n")
        if(opt.download=='pccmsrm'): outputfile.write('ls *.root *.txt | xargs -i scp -o BatchMode=yes -o StrictHostKeyChecking=no {} pccmsrm24:'+diskoutputmain+'/{}\n') 
        else: outputfile.write('ls *.root | xargs -i cp {} '+diskoutputmain+'\n')
        outputfile.close
        logfile = opt.prefix+"/"+output+"/log/"+output+"_"+str(ijob)+".log"
        os.system("echo bsub -G CMS_CERN01_YODA -q "+opt.queue+" -o "+logfile+" source "+pwd+"/"+outputname)
        if(opt.create==False):
            os.system("bsub -G CMS_CERN01_YODA -q "+opt.queue+" -o "+logfile+" source "+pwd+"/"+outputname)
        ijob = ijob+1
        if(ijob==opt.testnjobs): break

if __name__ == "__main__":
        main()

