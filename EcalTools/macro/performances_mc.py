#! /usr/bin/env python
from array import array
from math import *
import numpy, re, sys
import ROOT
ROOT.gROOT.SetBatch(True)

def effSigma(histo):
    xaxis = histo.GetXaxis()
    nb = xaxis.GetNbins()
    xmin = xaxis.GetXmin()
    ave = histo.GetMean()
    rms = histo.GetRMS()
    total=histo.Integral()
    if total < 100:
        print "effsigma: Too few entries to compute it: ", total
        return 0.
    ierr=0
    ismin=999
    rlim=0.683*total
    bwid = xaxis.GetBinWidth(1)
    nrms=int(rms/bwid)
    if nrms > nb/10: nrms=int(nb/10) # Could be tuned...
    widmin=9999999.
    for iscan in xrange(-nrms,nrms+1): # // Scan window centre
        ibm=int((ave-xmin)/bwid)+1+iscan
        x=(ibm-0.5)*bwid+xmin
        xj=x; xk=x;
        jbm=ibm; kbm=ibm;
        bin=histo.GetBinContent(ibm)
        total=bin
        for j in xrange(1,nb):
            if jbm < nb:
                jbm += 1
                xj += bwid
                bin=histo.GetBinContent(jbm)
                total += bin
                if total > rlim: break
            else: ierr=1
            if kbm > 0:
                kbm -= 1
                xk -= bwid
                bin=histo.GetBinContent(kbm)
                total+=bin
            if total > rlim: break
            else: ierr=1
        dxf=(total-rlim)*bwid/bin
        wid=(xj-xk+bwid-dxf)*0.5
        if wid < widmin:
            widmin=wid
            ismin=iscan
    if ismin == nrms or ismin == -nrms: ierr=3
    if ierr != 0: print "effsigma: Error of type ", ierr
    return widmin

    
def getOneResolutionHisto(tree,absetamin,absetamax,etmin,etmax,xmin=0.6,xmax=1.2):
    phasespace = 'abs(geneta)>{etamin} && abs(geneta)<{etamax} && genpt>{etmin} && genpt<{etmax}'.format(etamin=absetamin,etamax=absetamax,etmin=etmin,etmax=etmax)
    unconv     = 'simconversion==0'
    cut = '{c1} && {c2}'.format(c1=phasespace,c2=unconv)
    variable   = 'e5x5/gene'
    
    nbins = 100 if etmax>20 else 50
    nicename = phasespace.replace(">","gt").replace("<","lt").replace("&&","AND").replace(".","p").replace(" ","_").replace(")","").replace("(","")
    
    histo = ROOT.TH1D('historeso_'+nicename,'',nbins,xmin,xmax)
    histo.Reset()
    print "====== drawing ",variable," with cut ",cut
    print "nbins,xmin,xmax = ",nbins," ",xmin,"  ",xmax
    print "Integral = ",histo.Integral()
    tree.Draw('{var}>>historeso_{nn}'.format(var=variable,nn=nicename),cut)

    histo.Sumw2()
    effsigma = effSigma(histo)

    histo.Scale(1./histo.Integral())
    
    return (histo,effsigma)
    
def fitOneReso(hist,name,xmin,histcolor,funccolor):
    canv = ROOT.TCanvas("canvreso","",1200,1200)
    canv.SetLeftMargin(0.15)
    canv.SetRightMargin(0.15)
    canv.SetBottomMargin(0.15)
    mean,rms = hist.GetMean(), hist.GetRMS()
    hist.SetLineColor(histcolor)
    hist.SetMarkerColor(histcolor)
    hist.SetMarkerSize(0.9)
    hist.SetMarkerStyle(ROOT.kOpenSquare)
    hist.Draw("hist pe1")
    result = hist.Fit('gaus','SR','',mean-1.1*rms, mean+1.1*rms)
    func = hist.GetFunction('gaus')    
    func.SetLineColor(funccolor)
    printCanvas(canv, name, [], [], options)
    return (result,func)
    
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
    ymax = max([f[0].GetMaximum() for f in frames])
    for iframe,frame in enumerate(frames):
        frame[0].SetMaximum(ymax*(1.10))
        frame[0].GetXaxis().SetNdivisions(505)
        frame[0].GetXaxis().SetDecimals(1)    
        frame[0].Draw('' if iframe==0 else 'same')
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

def printCanvas(c1, name, text, colors, options,textSize=0.03):

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
    bins_et = [1,5,10,30,100]; ranges_etacoarse = [[(0.5,1.3),(0.2,1.7)], [(0.8,1.15),(0.4,1.3)], [(0.85,1.1),(0.6,1.2)], [(0.94,1.04),(0.75,1.15)]]
    bins_etafine   = [0,0.43,0.8,1.23,1.4442,1.566,1.8,2.0,2.5,3.0]
    bins_etacoarse = [0,1.5,2.5]; 

    bins_eta = bins_etacoarse
    
    dirs = [weights_dir,mf_dir]
    recolabels = ['weights','multifit']
    hist_colors = {'weights':ROOT.kBlack, 'multifit':ROOT.kRed}
    func_colors = {'weights':ROOT.kGray, 'multifit':ROOT.kRed-7}
    chains = {'weights': ROOT.TChain('photonanalyzer/PhotonClusterAnalyzerTree'),
              'multifit': ROOT.TChain('photonanalyzer/PhotonClusterAnalyzerTree') }

    fit_res = {}
    fit_plots = {}
    hist_res = {}
    
    for idir,d in enumerate(dirs):
        reco = recolabels[idir]
        chains[reco].Add('{d1}/{d2}/photongun_*.root'.format(d1=maindir,d2=d))
        for ieb in xrange(len(bins_eta)-1):
            for iet in xrange(len(bins_et)-1):
                subdet_idx = 0 if abs(bins_eta[ieb])<1.4442 else 1
                x_range = (ranges_etacoarse[iet])[subdet_idx]
                hist,effsigma = getOneResolutionHisto(chains[reco],bins_eta[ieb],bins_eta[ieb+1],bins_et[iet],bins_et[iet+1],x_range[0],x_range[1])
                namereso = 'plots/resol_{label}_abseta{eta1}to{eta2}_et{et1}to{et2}'.format(label=reco,eta1=bins_eta[ieb],eta2=bins_eta[ieb+1],
                                                                                      et1=bins_et[iet],et2=bins_et[iet+1])
                res,func = fitOneReso(hist,namereso,x_range[0],hist_colors[reco],func_colors[reco])
                #fit_res[(reco,ieb,iet)] = (res.floatParsFinal().find('mean'), res.floatParsFinal().find('sigma'))
                fit_res[(reco,ieb,iet)] = (res.Parameter(1), res.Parameter(2))
                fit_plots[(reco,ieb,iet)] = (hist,func)
                hist_res[(reco,ieb,iet)] = effsigma
                
    # superimpose weights / MF
    for ieb in xrange(len(bins_eta)-1):
        for iet in xrange(len(bins_et)-1):
            frames = []
            text = ['Resolution 5x5']; colors = [ROOT.kBlack]
            for lbl in recolabels:
                key = (lbl,ieb,iet)
                frames.append(fit_plots[key])
                resol = fit_res[key][1]
                text.append("#sigma_{{ eff (core) }} = {eff:.1f} ({gauss:.1f}) %".format(eff=100*hist_res[key], gauss=100*resol))
                colors.append(hist_colors[lbl])
            namereso = 'plots/compreso_abseta{eta1}to{eta2}_et{et1}to{et2}'.format(eta1=bins_eta[ieb],eta2=bins_eta[ieb+1],
                                                                             et1=bins_et[iet],et2=bins_et[iet+1])
            printPlot(frames, namereso, text, colors)

