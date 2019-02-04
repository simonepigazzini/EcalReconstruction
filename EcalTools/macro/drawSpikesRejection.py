#! /usr/bin/env python
from array import array
from math import *
import ROOT
ROOT.gROOT.SetBatch(True)

from performances_mc import printPlot
from PlotUtils import doLegend

if __name__ == "__main__":

    lat = ROOT.TLatex()
    lat.SetNDC(); lat.SetTextFont(42)

    files = {'weights':  'spikes_wgts.root',
             'multifit': 'spikes_multifit.root'}
    cuthistos = ['all','Topology']
    
    histos = {}
    for label,fname in files.iteritems():
        tf = ROOT.TFile(fname)
        for h in cuthistos:
            histos[(label,h)] = tf.Get(h)
            #histos[(label,h)].SetDirectory(None)

    canv = ROOT.TCanvas("canv","",1200,1200)
    canv.SetGridx()
    canv.SetGridy()
    canv.SetLeftMargin(0.15)
    canv.SetRightMargin(0.15)
    canv.SetBottomMargin(0.15)

    frames = []; labels = []; styles = []
    for k,h in histos.iteritems():
        reco,cuts = k
        labels.append('{cutlbl}, {reco}'.format(cutlbl='Topology + timing' if cuts=='all' else 'Topology',
                                                reco=reco))
        styles.append('pe')
        if cuts=='all':
            h.SetMarkerStyle(ROOT.kFullCircle if reco=='multifit' else ROOT.kOpenCircle)
            h.SetMarkerColor(ROOT.kAzure-3); h.SetLineColor(ROOT.kAzure-3); 
        else:
            h.SetMarkerStyle(ROOT.kFullSquare if reco=='multifit' else ROOT.kOpenSquare)
            h.SetMarkerColor(ROOT.kOrange-3); h.SetLineColor(ROOT.kOrange-3); 
        h.SetMinimum(0.5)
        frames.append(h)

    leg = doLegend(frames,labels,styles,legBorder=False,corner='BC')
    printPlot(frames, 'spikes_rejection', legend=leg)

        
