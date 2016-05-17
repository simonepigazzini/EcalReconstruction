#! /usr/bin/env python
import sys
import ROOT as rt  
rt.gROOT.SetBatch(True) 
from PlotUtils import customROOTstyle, customPalette

def plotTimingMaps(filename,iz):
    f = rt.TFile.Open(filename)
    pref = {0:'EB',-1:'EEminus',1:'EEplus'}
    histname = '%s_time' % pref[iz]
    histo = f.Get(histname)
    histo.SetDirectory(None)

    if iz==0: histo.SetAxisRange(-1, 1, "Z")
    else: histo.SetAxisRange(-3, 3, "Z")

    doEB = True if iz==0 else False
    xsize = 1200
    ysize = int(xsize*170/360+0.1*xsize) if doEB else int(xsize*0.9)

    canv = rt.TCanvas("c","",xsize,ysize)
    histo.Draw("colz")
    canv.SaveAs(histo.GetName()+'.pdf')

    return histo

def plotProjection(prof2D):

    for k,p in prof2D.iteritems():
        rt.gStyle.SetOptFit(1111)
        pfx = p.ProfileX()
        axtitle = 'i#phi' if k==0 else 'ix'
        pfx.GetXaxis().SetTitle(axtitle)
        pfx.GetYaxis().SetTitle("Time (ns)")
        #pfx.GetYaxis().SetRangeUser(0,0.5 if k==0 else )
        pfx.Draw()
        rt.gPad.SaveAs("time_vsix_iz%d.pdf" % k)
        xrange1d = [-.5,1] if k==0 else [-3,2]
        h1d = rt.TH1D("time1d","",100,xrange1d[0],xrange1d[1])
        if k!=0: h1d.Rebin(2)
        for b in range(p.GetNbinsX()*p.GetNbinsY()):
                if p.GetBinEntries(b) > 0: h1d.Fill(p.GetBinContent(b))
        h1d.GetXaxis().SetTitle("Time (ns)")
        h1d.Draw()
        if k==0: h1d.Fit("gaus","Q WW M")
        else: h1d.Fit("gaus","Q WW M","same",-2.0,0.5)

        rt.gPad.SaveAs("time1d_iz%d.pdf" % k)

        pfy = p.ProfileY()
        axtitle = 'i#eta' if k==0 else 'iy'
        pfy.GetXaxis().SetTitle(axtitle)
        pfx.GetYaxis().SetTitle("Time (ns)")
        #pfx.GetYaxis().SetRangeUser(0,0.5)
        pfy.Draw()
        rt.gPad.SaveAs("time_vsiy_iz%d.pdf" % k)

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] filewithhists.root")
    parser.add_option("--iz", dest="iz",  type="string", default="", help="iz to plot (0,-1,1)")
    (options, args) = parser.parse_args()

    customROOTstyle()
    rt.gROOT.SetBatch(True)
    filename = args[0]

    prof2D = {}

    if options.iz:
        iz=int(options.iz)
        prof2D[iz] = plotTimingMaps(filename,iz)
    else:
        for iz in range(-1,2): prof2D[iz] = plotTimingMaps(filename,iz)

    plotProjection(prof2D)
