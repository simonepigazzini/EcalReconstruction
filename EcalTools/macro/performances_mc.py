#! /usr/bin/env python
from array import array
from math import *
import numpy, re
import ROOT
ROOT.gROOT.SetBatch(True)

def getOneResolutionHisto(tree,absetamin,absetamax,etmin,etmax,xmin=0.6,xmax=1.2):
    phasespace = 'abs(eta)>{etamin} && abs(eta)<{etamax} && pt>{etmin} && pt<{etmax}'.format(etamin=absetamin,etamax=absetamax,etmin=etmin,etmax=etmax)
    unconv     = 'simconversion==0'
    cut = '{c1} && {c2}'.format(c1=phasespace,c2=unconv)
    variable   = 'e5x5/gene'
    
    nbins = 50 if etmax>20 else 26
    histo = ROOT.TH1D('historeso','',nbins,xmin,xmax)
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

    th1 = datahist.createHistogram('th1',var)
    mean,rms = th1.GetMean(), th1.GetRMS()

    # Crystal Ball parameters
    cbBias  = ROOT.RooRealVar("mean", "CB Bias", mean, 0.9, 1.02);
    cbSigma = ROOT.RooRealVar("sigma", "CB Width", rms, 0.008, 0.20);
    cbCut   = ROOT.RooRealVar("a","CB Cut", 1.0, 0.1, 3.0);
    cbPower = ROOT.RooRealVar("n","CB Order", 2.5);

    core = (mean-1.1*rms, mean+1.1*rms)
    print "*************************************** CORE = ",core
    # if 'weights' in name:
    #     core = (0.95,1.03) if xmin>0.8 else (0.8,1.15)
    # else:
    #     core = (0.94,1.03) if xmin>0.6 else ((0.88,1.1) if 'absetato1.5' in name else (0.85,1.1))
                                             
        
    var.setRange("core",core[0],core[1]) ;  
    #cball = ROOT.RooCBShape("cball", "Crystal Ball", var, cbBias, cbSigma, cbCut, cbPower);
    gauss = ROOT.RooGaussian("gaussian","Gaussian", var, cbBias, cbSigma)
    result = gauss.fitTo(datahist, ROOT.RooFit.Save(), ROOT.RooFit.Range('core'))
    
    frame = var.frame()
    frame.SetTitle(''); frame.SetName('')
    data.plotOn(frame, ROOT.RooFit.LineColor(histcolor), ROOT.RooFit.MarkerColor(histcolor))
    gauss.plotOn(frame, ROOT.RooFit.LineColor(funccolor), ROOT.RooFit.MoveToBack())
    frame.Draw()
    printCanvas(canv, name, [], [], options)
    return (result,frame)

def getRelReso(fitmandsigma):
    mean,sigma=fitmandsigma
    val = mean.getVal()
    err = sigma.getError()
    return (val,err)

def printPlot(frames, name, text, colors):
    canv = ROOT.TCanvas("canv","",1200,1200)
    canv.SetLeftMargin(0.15)
    canv.SetRightMargin(0.05)
    canv.SetBottomMargin(0.15)
    ymax = max([f.GetMaximum() for f in frames])
    for iframe,frame in enumerate(frames):
        frame.SetMaximum(ymax*(1.10))
        frame.GetXaxis().SetNdivisions(505)
        frame.GetXaxis().SetDecimals(1)    
        frame.Draw('' if iframe==0 else 'same')
    printCanvas(canv, name, text, colors, options)

def doSpam(text,x1,y1,x2,y2,align=12,fill=False,textSize=0.033,textColor=ROOT.kBlack,_noDelete={},debugMargins=False):
    cmsprel = ROOT.TPaveText(x1,y1,x2,y2,"NDC");
    cmsprel.SetTextSize(textSize);
    cmsprel.SetFillColor(0);
    cmsprel.SetFillStyle(1001 if fill else 0);
    if debugMargins:
        cmsprel.SetLineStyle(1);
        cmsprel.SetLineColor(ROOT.kRed);
    else:
        cmsprel.SetLineStyle(2);
        cmsprel.SetLineColor(0);
    cmsprel.SetTextAlign(align);
    cmsprel.SetTextFont(42);
    cmsprel.SetTextColor(textColor);
    cmsprel.AddText(text);
    cmsprel.Draw("same");
    _noDelete[text] = cmsprel; ## so it doesn't get deleted by PyROOT
    return cmsprel

def printCanvas(c1, name, text, colors, options,textSize=0.04):

    doSpam("#bf{CMS} #it{Simulation}", 0.12, 0.91, 0.55, 0.98, 12, textSize=0.05)
    doSpam("(13 TeV)", .55, .91, .95, .98, 32, textSize=0.05)
    
    y0 = 0.85 - textSize*1.8
    for il,line in enumerate(text):
        niceline = re.sub(r"(\s+)-(\d+)",r"\1#minus\2", line)
        doSpam(niceline, 0.20, y0, 0.40, y0 + textSize*1.2, 11, textSize=textSize, textColor=colors[il])
        y0 -= textSize * 1.8
    for ext in ['pdf','png']:
        c1.Print('{name}.{ext}'.format(name=name,ext=ext))

    
    
if __name__ == "__main__":

    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] tag1.txt tag2.txt [tagTimeIC.txt]")
    parser.add_option(     "--noPU",  dest="noPileup",   action="store_true", help="do the plots for the no PU case")
    (options, args) = parser.parse_args()

    ROOT.gStyle.SetOptStat(0)

    maindir='/Users/emanuele/Work/data/cms/multifit/trees_josh_2014/'
    weights_dir = 'photontree_photongun_720p4_nopu_defreco' if options.noPileup else 'photontree_photongun_720p4_pu20_defreco'
    mf_dir = 'photontree_photongun72x_nopu_multifitSep4' if options.noPileup else 'photontree_photongun72x_pu20_multifitSep4'

#    bins_et = [1,5,10,30,100]; ranges_etacoarse = [[(0.5,1.3),(0.2,1.4)], [(0.8,1.15),(0.6,1.2)], [(0.85,1.1),(0.7,1.1)], [(0.94,1.04),(0.85,1.1)]]
    bins_et = [1,5,10,30,100]; ranges_etacoarse = [[(0.5,1.3),(0.2,1.4)], [(0.8,1.15),(0.4,1.3)], [(0.85,1.1),(0.6,1.2)], [(0.94,1.04),(0.75,1.15)]]
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
                namereso = 'plots/resol_{label}_abseta{eta1}to{eta2}_et{et1}to{et2}'.format(label=reco,eta1=bins_eta[ieb],eta2=bins_eta[ieb+1],
                                                                                      et1=bins_et[iet],et2=bins_et[iet+1])
                res,plot = fitOneReso(x,data,namereso,x_range[0],hist_colors[reco],func_colors[reco])
                fit_res[(reco,ieb,iet)] = (res.floatParsFinal().find('mean'), res.floatParsFinal().find('sigma'))
                fit_plots[(reco,ieb,iet)] = plot
                
    # superimpose weights / MF
    for ieb in xrange(len(bins_eta)-1):
        for iet in xrange(len(bins_et)-1):
            frames = []
            text = ['Resolution 5x5']; colors = [ROOT.kBlack]
            frames = []
            for lbl in recolabels:
                frames.append(fit_plots[(lbl,ieb,iet)])
                resol,resol_unc = (fit_res[(lbl,ieb,iet)])[1].getVal(),  (fit_res[(lbl,ieb,iet)])[1].getError()
                text.append("#sigma(E)/E = {val:.1f} %".format(val=100*resol))
                colors.append(hist_colors[lbl])
            namereso = 'plots/compreso_abseta{eta1}to{eta2}_et{et1}to{et2}'.format(eta1=bins_eta[ieb],eta2=bins_eta[ieb+1],
                                                                             et1=bins_et[iet],et2=bins_et[iet+1])
            printPlot(frames, namereso, text, colors)

