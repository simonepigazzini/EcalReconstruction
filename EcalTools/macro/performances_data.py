#! /usr/bin/env python
from array import array
from math import *
import numpy, re, sys, os
import itertools as it
from datetime import datetime
import ROOT
ROOT.gROOT.SetBatch(True)
from PlotUtils import doLegend

from performances_mc import effSigma,printCanvas,printPlot

def getOneMassHisto(tree,absetamin,absetamax,variable='invMass_5x5SC',xmin=60,xmax=110,timeMin=-1,timeMax=-1,name='mass'):
    canv = ROOT.TCanvas("canvreso","",1200,1200)
    canv.SetLeftMargin(0.15)
    canv.SetRightMargin(0.15)
    canv.SetBottomMargin(0.15)
    #if absetamax>1.5:
    #phasespace = '((abs(etaEle[0])>{etamin} && abs(etaEle[0])<{etamax}) || (abs(etaEle[1])>{etamin} && abs(etaEle[1])<{etamax}))'.format(etamin=absetamin,etamax=absetamax)
    phasespace = '(TMath::Max(abs(etaEle[0]),abs(etaEle[1]))>{etamin} && TMath::Max(abs(etaEle[0]),abs(etaEle[1]))<{etamax})'.format(etamin=absetamin,etamax=absetamax)
    #else:
    #phasespace = '(abs(etaEle[0])>{etamin} && abs(etaEle[0])<{etamax} && abs(etaEle[1])>{etamin} && abs(etaEle[1])<{etamax})'.format(etamin=absetamin,etamax=absetamax)        
    if timeMin>0 and timeMax>0:
        time = 'eventTime>{timemin} && eventTime<{timemax}'.format(timemin=timeMin,timemax=timeMax)
        nbins = 100
    else:
        time = '1'
        nbins = 200 if absetamax<1.3 else 100
    unconv     = 'fbremEle[0]>0 && fbremEle[0]<{fbremcut} && fbremEle[1]>0 && fbremEle[1]<{fbremcut}'.format(fbremcut=0.1 if absetamax<1.5 else 0.1)
    cut = '{c1} && {c2} && {c3}'.format(c1=phasespace,c2=unconv,c3=time)
    variable   = 'invMass_5x5SC'
    #print "Now plotting ",variable,"  with cuts: ",cut
    nicename = cut.replace(">","gt").replace("<","lt").replace("&&","AND").replace(".","p").replace(" ","_").replace(")","").replace("(","").replace("[",'').replace("]",'').replace("!","NOT").replace('||','OR')
    histoname = '{name}_{cuts}'.format(name=name,cuts=nicename)
    histo = ROOT.TH1D(histoname,'',nbins,xmin,xmax)
    histo.GetXaxis().SetTitle("m_{ee}^{5x5} (GeV)")
    histo.GetYaxis().SetTitle("Entries")
    tree.Draw('{var}>>{hn}'.format(var=variable,hn=histoname),cut)
    return histo

def getMedian(histo):
    probSums = array('d',[0.5])
    quantiles = array('d',[0])
    histo.GetQuantiles(len(probSums),quantiles,probSums)
    return quantiles[0]

if __name__ == "__main__":

    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] tag1.txt tag2.txt [tagTimeIC.txt]")
    parser.add_option('-s',  "--step",    dest="step", type='string', default='history', help="step (can be 'histograms','history')")
    parser.add_option('-o',  "--outfile", dest="outfile", default='masses.root', help="name of the output file for the step 1 (default = masses.root)")    
    parser.add_option('-i',  "--infile", dest="infile",   default='masses.root', help="name of the input file with histograms")    
    parser.add_option(       "--deltat", dest="deltat",   default=5, type=int, help="time interval in days where average the hiostory (dafault=5 days)")    
    (options, args) = parser.parse_args()
    
    ROOT.gStyle.SetOptStat(0)

    maindir = '/Users/emanuele/Work/data/cms/multifit/trees_ecalelf'
    weights_dir = 'DoubleEG-Run2016B-ZSkim-23Sep2016-weightsReco/'
    mf_dir = 'DoubleEG-Run2016B-ZSkim-23Sep2016'
    #mf_dir = 'Cal_Mar2017_ref_v2_etaScale'
    
    dirs = [weights_dir,mf_dir]
    recolabels = ['weights','multifit']
    hist_colors = {'weights':ROOT.kBlack, 'multifit':ROOT.kRed}
    chains = {'weights': ROOT.TChain('selected'),
              'multifit': ROOT.TChain('selected') }

    bins_etafine   = [0,0.42,0.79,1.12,1.4442,1.566,1.8,2.0,2.5]
    bins_etacoarse = [0,1.5,2.5]; 
    bins_eta = bins_etacoarse

    if options.step=='histograms':
        oneday = 24*60*60
        deltat = options.deltat * oneday
        beginTime = 1462950000
        endTime = 1473000000
        #endTime = beginTime+10*oneday
        
        outfname = options.outfile
        outfile = ROOT.TFile(outfname,'recreate')
        
        for idir,d in enumerate(dirs):
            reco = recolabels[idir]
            print "Resolution vs time: analyzing reconstruction algo: ",reco
            chains[reco].Add('{d1}/{d2}/DoubleEG*Run2016*.root'.format(d1=maindir,d2=d))
            time = beginTime
            while time<endTime:
                print "Date: ",datetime.utcfromtimestamp(time).strftime('%Y-%m-%d %H:%M:%S')
                for ieb in xrange(len(bins_eta)-1):
                    subdet_idx = 0 if abs(bins_eta[ieb])<1.4442 else 1
                    start = time; stop = time+oneday
                    hist = getOneMassHisto(chains[reco],bins_eta[ieb],bins_eta[ieb+1],timeMin=start,timeMax=stop,name='mass_'+reco)
                    outfile.cd()
                    hist.Write()
                time += deltat

        outfile.Close()

    if options.step == 'history':
        if not os.path.isfile(options.infile):
            print "ERROR! The input file ",options.infile," not found. Exiting."
            sys.exit(1)

        histograms = {}
        hist_res = {}
        infile = ROOT.TFile(options.infile,"read")

        nTimePoints = 0
        for k in infile.GetListOfKeys():
            hname = k.GetName()
            histo = infile.Get(hname)
            tokens = hname.split('_')
            variable = tokens[0]; reco = tokens[1]
            startTime = tokens[-3].replace('eventTimegt',''); stopTime = tokens[-1].replace('eventTimelt','');
            #print "var = ",variable," reco = ",reco," start = ",startTime,"   stop = ",stopTime
            median,effsigma = (getMedian(histo),effSigma(histo))
            #print "integral = ",histo.Integral(),"  median = ",median,"    effsigma = ",effsigma
            subdet = 'EE' if 'NOT' in hname else 'EB'
            if subdet=='EB' and reco =='multifit': nTimePoints+=1
            histograms[(variable,reco,subdet,startTime,stopTime)] = histo
            hist_res[(variable,reco,subdet,startTime,stopTime)] = (median,effsigma)
        infile.Close()

        print "estimators = ",hist_res

        history = ROOT.TGraphErrors(nTimePoints)
        history.GetXaxis().SetTimeDisplay(1)
        history.GetXaxis().SetTimeFormat("%m\/%y%F2016-05-01 13:00:01");
        
        graph = {}; graph_npoints = {}
        hist_colors = {'weights':ROOT.kBlack, 'multifit':ROOT.kRed}
        for subdet in ['EB','EE']:
            for reco in ['weights','multifit']:
                for est in ['median','sigma']:
                    graph[(reco,subdet,est)] = history.Clone('history_{reco}_{sub}_{est}'.format(reco=reco,sub=subdet,est=est))
                    graph_npoints[(reco,subdet,est)] = 0
                    graph[(reco,subdet,est)].SetTitle("")
                    graph[(reco,subdet,est)].GetXaxis().SetTitle()
                    graph[(reco,subdet,est)].SetMarkerStyle(ROOT.kOpenSquare)
                    graph[(reco,subdet,est)].SetMarkerSize(0.9)
                    graph[(reco,subdet,est)].SetMarkerColor(hist_colors[reco])
                    graph[(reco,subdet,est)].SetLineColor(hist_colors[reco])
                    if est=='median':
                        graph[(reco,subdet,est)].SetMaximum(84)
                        graph[(reco,subdet,est)].SetMinimum(82)
                    else:
                        graph[(reco,subdet,est)].SetMaximum(5)
                        graph[(reco,subdet,est)].SetMinimum(4)
                        

                        
        for key,val in hist_res.iteritems():
            (variable,reco,subdet,start,stop) = key
            (median,effsigma) = val
            time = (float(stop)+float(start))/2.
            graph[(reco,subdet,'median')].SetPoint(graph_npoints[(reco,subdet,'median')],time,median); graph_npoints[(reco,subdet,'median')]+=1
            graph[(reco,subdet,'sigma')].SetPoint(graph_npoints[(reco,subdet,'sigma')],time,effsigma); graph_npoints[(reco,subdet,'sigma')]+=1
            print key, "has values:", val
            print graph_npoints[(reco,subdet,'median')]
            
        for subdet in ['EB','EE']:
            for est in ['median','sigma']:
                plots = [ graph[(reco,subdet,est)] for reco in ['weights','multifit'] ]
                nameplot = 'history_mass_{sub}_{est}'.format(sub=subdet,est=est)
                printPlot(plots, nameplot)



    if options.step=='etaresol':

        bins_eta = bins_etafine
        mass_etafine = {}
        for idir,d in enumerate(dirs):
            reco = recolabels[idir]
            chains[reco].Add('{d1}/{d2}/DoubleEG*Run2016B*.root'.format(d1=maindir,d2=d))
            print "Resolution vs Eta: analyzing reconstruction algo: ",reco
            for ieb in xrange(len(bins_eta)-1):
                hist = getOneMassHisto(chains[reco],bins_eta[ieb],bins_eta[ieb+1],timeMin=0,timeMax=1E+10,name='mass_'+reco)
                hist.SetLineColor(hist_colors[reco])
                hist.SetMarkerColor(hist_colors[reco])
                hist.SetMarkerSize(0.9)
                hist.SetMarkerStyle(ROOT.kOpenSquare)
                mass_etafine[(reco,ieb)] = hist
                
        # superimpose weights / MF
        resolVsEta = ROOT.TH1F('resolVsEta','',len(bins_eta)-1,array('f',bins_eta))
        resolVsEta.GetXaxis().SetTitle('#eta')
        resolVsEta.GetYaxis().SetTitle('Effective resolution (GeV)')
        resolVsEta.SetMarkerStyle(ROOT.kFullSquare)
        resolVsEta.SetMarkerSize(1)

        gapBand = resolVsEta.Clone('gapBand')
        gapBand.SetBinContent(5,100)
        gapBand.SetFillColor(ROOT.kGray+1)
        
        resolutionsEta = {}
        for lbl in recolabels:
            resolutionsEta[lbl] = resolVsEta.Clone('resolEta_'+lbl)
            resolutionsEta[lbl].SetMarkerSize(3)
            resolutionsEta[lbl].SetMarkerStyle(ROOT.kFullCircle if lbl=='multifit' else ROOT.kOpenCircle)
            resolutionsEta[lbl].SetMarkerColor(hist_colors[lbl])
            resolutionsEta[lbl].SetLineColor(hist_colors[lbl])

        resolQuadDiffEta = resolVsEta.Clone('resolQuadDiffEta_')
        resolQuadDiffEta.SetMarkerSize(3)
        resolQuadDiffEta.GetYaxis().SetTitle('#sigma_{eff}^{weights} (#minus) #sigma_{eff}^{multifit} (GeV)')
        resolQuadDiffEta.SetMarkerStyle(ROOT.kFullCircle)
        resolQuadDiffEta.SetMarkerColor(hist_colors['multifit'])
        resolQuadDiffEta.SetLineColor(hist_colors['multifit'])
        
        for ieb in xrange(len(bins_eta)-1):
            frames = []
            text = ['Resolution 5x5']; colors = [ROOT.kBlack]
            for lbl in recolabels:
                key = (lbl,ieb)
                frames.append(mass_etafine[key])
                resol = effSigma(mass_etafine[key])
                median = getMedian(mass_etafine[key])
                text.append("#sigma_{{ eff }}^{{ {label} }} = {resol:.1f} (GeV)".format(resol=resol, label=lbl))
                colors.append(hist_colors[lbl])
                # fill the resolution vs ET histo
                resolutionsEta[lbl].SetBinContent(ieb+1,0 if ieb==4 else resol) # don't show the gap
                resolutionsEta[lbl].SetBinError(ieb+1,0.001) # this is just to show the error on the X
                resolutionsEta[lbl].GetXaxis().SetTitle('max(|#eta_{1}|,|#eta_{2}|)')
                resolutionsEta[lbl].GetYaxis().SetTitle("#sigma_{eff}^{m5x5} (GeV)")
            namereso = 'plots/compZ_abseta{eta1}to{eta2}'.format(eta1=bins_eta[ieb],eta2=bins_eta[ieb+1])
            printPlot(frames, namereso, text, colors, sim=False, histopt='pe')
            
        # resolution vs ETA
        plots = []; labels = []; styles = []
        for k,h in resolutionsEta.iteritems():
            labels.append(k)
            plots.append(h)
            styles.append('pe')
        leg = doLegend(plots,labels,styles,legBorder=False,corner='TL')
        printPlot(plots,"plots/resolutionZdataEta",histopt='pe',legend=leg,sim=False,yaxMin=3.9,yaxMax=7.0,gridy=True,bandPlot=gapBand)
