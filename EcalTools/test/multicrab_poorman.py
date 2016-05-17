#!/usr/bin/env python
"""
This is a small script that does the equivalent of multicrab.
"""
import os
from optparse import OptionParser

def getOptions():
    """
    Parse and return the arguments provided by the user.
    """
    usage = ("Usage: %prog --crabCmd CMD [--workArea WAD --crabCmdOpts OPTS]"
             "\nThe multicrab command executes 'crab CMD OPTS' for each project directory contained in WAD"
             "\nUse multicrab -h for help")

    parser = OptionParser(usage=usage)

    parser.add_option('-c', '--crabCmd',
                      dest = 'crabCmd',
                      default = '',
                      help = "crab command",
                      metavar = 'CMD')

    parser.add_option('-o', '--crabCmdOpts',
                      dest = 'crabCmdOpts',
                      default = '',
                      help = "options for crab command CMD",
                      metavar = 'OPTS')

    (options, arguments) = parser.parse_args()
    
    if arguments:
        parser.error("Found positional argument(s): %s." % (arguments))
        if not options.crabCmd:
            parser.error("(-c CMD, --crabCmd=CMD) option not provided.")
            
    return options

def main():
    
    options = getOptions()

    submitfile = open('submitall.sh','w')

    #--------------------------------------------------------
    # This is the base config:
    #--------------------------------------------------------

    offset = -5.0
    while offset <= 5.0:
        suffix = "timeShifted_"+"{0:.6f}".format(offset)+"ns"
        suffix_sane = suffix.replace(".","p")
        print "running shapes for ",suffix_sane,"...\n"

        # cfg customisation
        icfgfilename = "rereco_"+suffix_sane+"_cfg.py"
        icfgfile = open(icfgfilename,'w')
        cfgfile=open('RECO_Run2015D.py','r')
        for line in cfgfile:
            icfgfile.write(line)
        icfgfile.write('#customise GT\n')
        icfgfile.write('process.GlobalTag.toGet = cms.VPSet(\n')
        icfgfile.write('cms.PSet(record = cms.string("EcalPulseShapesRcd"),\n')
        icfgfile.write('tag = cms.string("EcalPulseShapes_data"),\n')
        icfgfile.write('connect = cms.untracked.string("sqlite_file:ecaltemplates_popcon_'+suffix_sane+'.db'+'"\n')
        icfgfile.write(')))\n')

        # crab shit
        configfilename = "crab_rerecosyst_"+suffix_sane+".py"
        configfile = open(configfilename,'w')
        
        configfile.write('from CRABClient.UserUtilities import config, getUsernameFromSiteDB\n')
        configfile.write('config = config()\n\n')

        configfile.write('config.General.workArea = \'crab_rerecoes_timeshift\'\n')
        configfile.write('config.General.transferOutputs = True\n')
        configfile.write('config.General.transferLogs = False\n\n')
        
        configfile.write('config.JobType.pluginName = \'Analysis\'\n')
        
        configfile.write('config.Data.inputDataset = \'/DoubleEG/Run2015D-ZElectron-PromptReco-v3/RAW-RECO\'\n')
        configfile.write('config.Data.inputDBS = \'global\'\n')
        configfile.write('config.Data.splitting = \'LumiBased\'\n')
        configfile.write('config.Data.unitsPerJob = 20\n')
        configfile.write('config.Data.lumiMask = \'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-256869_13TeV_PromptReco_Collisions15_25ns_JSON.txt\'\n')
        configfile.write('config.Data.runRange = \'246908-256869\'\n')
        configfile.write('config.Data.outLFNDirBase = \'/store/user/%s/\' % (getUsernameFromSiteDB())\n')
        configfile.write('config.Data.publication = True\n')

        configfile.write('config.Site.storageSite = "T2_IT_Rome"\n')

        # crab customisation
        configfile.write('config.JobType.psetName = \''+icfgfilename+'\'\n')
        configfile.write('config.General.requestName = \'rereco_zskim_Run2015D_'+suffix_sane+'\'\n')
        configfile.write('config.Data.publishDataName = \'ZElectron_Run2015D_SystReReco_'+suffix_sane+'\'\n')
        configfile.write('config.JobType.inputFiles = [ \'/afs/cern.ch/work/e/emanuele/ecal/ecalreco/multifit/studies_25ns/CMSSW_7_4_12_patch4/src/crab/ecaltemplates_popcon_'+suffix_sane+'.db\' ]\n')


        
        # Submit.
        if options.crabCmd == 'submit':
            print "Submitting for shift %s" % (suffix_sane)
            submitfile.write("crab submit "+configfilename+"\n")
        elif options.crabCmd == 'dryrun':
             print "Making cfgs for shift %s" % (suffix_sane)
             config.write("")

        offset += 0.250

if __name__ == '__main__':
    main()
