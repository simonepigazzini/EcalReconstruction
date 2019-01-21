#! /usr/bin/env python
from array import array
from math import *
import numpy
import ROOT
ROOT.gROOT.SetBatch(True)

def getOneResolutionHisto(tree,absetamin,absetamax,etmin,etmax,xmin=0.6,xmax=1.2):
    phasespace = 'abs(eta)>{etamin} && abs(eta)<{etamax} && pt>{etmin} && pt<{etmax}'.format(etamin=absetamin,etamax=absetamax,etmin=etmin,etmax=etmax)
    unconv     = 'simconversion==0'
    cut = '{c1} && {c2}'.format(c1=phasespace,c2=unconv)
    variable   = 'e5x5/gene'
    
    histo = ROOT.TH1D('historeso','',50,xmin,xmax)
    print "drawing ",variable," with cut ",cut
    tree.Draw('{var}>>historeso'.format(var=variable),cut)

    histo.Sumw2()
    histo.Scale(1./histo.Integral())
    
    # convert histo to roofit
    x    = ROOT.RooRealVar('x','E_{5x5}/E_{true}',xmin,xmax)
    data = ROOT.RooDataHist('reso','reso',ROOT.RooArgList(x),histo)
    return (x,data)

def fitOneReso(var,datahist,name,xmin,histcolor,funccolor):
    canv = ROOT.TCanvas("canvreso","",1200,1200)
    #canv.SetGridx()
    #canv.SetGridy()
    canv.SetLeftMargin(0.15)
    canv.SetRightMargin(0.15)
    canv.SetBottomMargin(0.15)

    # Crystal Ball parameters
    cbBias  = ROOT.RooRealVar("mean", "CB Bias", 1.0, 0.9, 1.05);
    cbSigma = ROOT.RooRealVar("sigma", "CB Width", 0.01, 0.005, 0.15);
    cbCut   = ROOT.RooRealVar("a","CB Cut", 1.0, 0.1, 3.0);
    cbPower = ROOT.RooRealVar("n","CB Order", 2.5);

    if 'weights' in name:
        core = (0.95,1.03) if xmin>0.8 else (0.8,1.15)
    else:
        core = (0.94,1.03) if xmin>0.6 else ((0.88,1.1) if 'absetato1.5' in name else (0.8,1.1))
                                             
        
    x.setRange("core",core[0],core[1]) ;  
    cball = ROOT.RooCBShape("cball", "Crystal Ball", var, cbBias, cbSigma, cbCut, cbPower);
    result = cball.fitTo(data, ROOT.RooFit.Minos(), ROOT.RooFit.Save(), ROOT.RooFit.Range('core'))
    
    frame = x.frame()
    data.plotOn(frame, ROOT.RooFit.LineColor(histcolor), ROOT.RooFit.MarkerColor(histcolor))
    cball.plotOn(frame, ROOT.RooFit.LineColor(funccolor), ROOT.RooFit.MoveToBack())
    frame.Draw()
    canv.SaveAs('{label}.pdf'.format(label=name))
    return (result,frame)
    
if __name__ == "__main__":

    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] tag1.txt tag2.txt [tagTimeIC.txt]")
    parser.add_option(     "--noPU",  dest="noPileup",   action="store_true", help="do the plots for the no PU case")
    (options, args) = parser.parse_args()

    ROOT.gStyle.SetOptStat(0)

    maindir='/Users/emanuele/Work/data/cms/multifit/trees_josh_2014/'
    weights_dir = 'photontree_photongun_720p4_nopu_defreco' if options.noPileup else 'photontree_photongun_720p4_pu20_defreco'
    mf_dir = 'photontree_photongun72x_nopu_multifitSep4' if options.noPileup else 'photontree_photongun72x_pu20_multifitSep4'

    bins_et = [1,5,10,30,100]; ranges_etacoarse = [[(0.5,1.3),(0.2,1.4)], [(0.8,1.15),(0.6,1.2)], [(0.85,1.1),(0.7,1.1)], [(0.94,1.04),(0.85,1.1)]]
    bins_etafine   = [0,0.43,0.8,1.23,1.4442,1.566,1.8,2.0,2.5,3.0]
    bins_etacoarse = [0,1.5,3.0]; 

    bins_eta = bins_etacoarse
    
    dirs = [weights_dir,mf_dir]
    recolabels = ['weights','multifit']
    hist_colors = {'weights':ROOT.kBlack, 'multifit':ROOT.kRed}
    func_colors = {'weights':ROOT.kGray, 'multifit':ROOT.kOrange+1}
    chain = ROOT.TChain('photonanalyzer/PhotonClusterAnalyzerTree')

    fit_res = {}
    fit_plots = {}
    
    for idir,d in enumerate(dirs):
        chain.Add('{d1}/{d2}/photongun_*.root'.format(d1=maindir,d2=d))
        for ieb in xrange(len(bins_eta)-1):
            for iet in xrange(len(bins_et)-1):
                reco = recolabels[idir]
                subdet_idx = 0 if abs(bins_eta[ieb])<1.4442 else 1
                x_range = (ranges_etacoarse[iet])[subdet_idx]
                x,data = getOneResolutionHisto(chain,bins_eta[ieb],bins_eta[ieb+1],bins_et[iet],bins_et[iet+1],x_range[0],x_range[1])
                namereso = 'resol_{label}_abseta{eta1}to{eta2}_et{et1}to{et2}'.format(label=reco,eta1=bins_eta[ieb],eta2=bins_eta[ieb+1],
                                                                                      et1=bins_et[iet],et2=bins_et[iet+1])
                res,plot = fitOneReso(x,data,namereso,x_range[0],hist_colors[reco],func_colors[reco])
                fit_res[(reco,ieb,iet)] = (res.floatParsFinal().find('mean').getVal(), res.floatParsFinal().find('sigma').getVal())
                fit_plots[(reco,ieb,iet)] = plot
                
    print "Fitted results = ",fit_res
    print "Fit plots = ",fit_plots

    canv = ROOT.TCanvas("canv","",1200,1200)
    #canv.SetGridx()
    #canv.SetGridy()
    canv.SetLeftMargin(0.15)
    canv.SetRightMargin(0.15)
    canv.SetBottomMargin(0.15)

    # superimpose weights / MF
    for ieb in xrange(len(bins_eta)-1):
        for iet in xrange(len(bins_et)-1):
            wgplot = fit_plots[('weights',ieb,iet)]
            mfplot = fit_plots[('multifit',ieb,iet)]
            mfplot.Draw()
            wgplot.Draw('same')
            namereso = 'compreso_abseta{eta1}to{eta2}_et{et1}to{et2}.pdf'.format(eta1=bins_eta[ieb],eta2=bins_eta[ieb+1],
                                                                                 et1=bins_et[iet],et2=bins_et[iet+1])
            canv.SaveAs(namereso)
