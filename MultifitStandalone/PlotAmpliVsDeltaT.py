#! /usr/bin/env python
import ROOT
import root_numpy as rtnp

def getGraph(filename,inset=False):
    tf = ROOT.TFile.Open(filename)
    tree = tf.Get("amplitudes")
    hsteps = tf.Get("hsteps")

    steps = rtnp.hist2array(hsteps)
    print (steps)

    gr = ROOT.TGraphErrors(len(steps))
    gr.SetTitle("")
    for i,s in enumerate(steps):
        hist = ROOT.TH1D("hist","",1000,0.95,1.05)
        tree.Draw("amplitude[{i}]/amplitudeTruth>>hist".format(i=i))
        bias = hist.GetMean()
        err = hist.GetMeanError()
        print ("deltat = {dt} => bias = {bias}".format(dt=s,bias=bias))
        gr.SetPoint(i,s,bias)
        gr.SetPointError(i,0,err)
    gr.SetMarkerStyle(ROOT.kFullCircle)
    gr.SetMarkerSize(1.5)
    gr.Draw("APE")
    xax = gr.GetXaxis(); yax = gr.GetYaxis()
    xax.SetRangeUser(-4.,4.)
    yax.SetRangeUser(0.97,1.05)
    yax.SetDecimals()
    if not inset:
        xax.SetTitleOffset(1.1); xax.SetTitleSize(0.05)
        yax.SetTitleOffset(1.5); yax.SetTitleSize(0.05)
        xax.SetTitle('#DeltaT_{max} (ns)')
        yax.SetTitle('A / A_{true}')
    return gr

def plotOnePartition(eta):

    grfull = getGraph("data/reco_signal_50GeV_eta_%.1f_pu_40_shiftdt_pm5ns.root" % eta)
    grzoom = getGraph("data/reco_signal_50GeV_eta_%.1f_pu_40_shiftdt_pm1ns.root" % eta)
    
    ROOT.gStyle.SetOptStat(0)

    canv = ROOT.TCanvas("canv","",1200,1200)
    canv.SetGridx()
    canv.SetGridy()
    canv.SetLeftMargin(0.17)
    canv.SetRightMargin(0.1)
    canv.SetBottomMargin(0.15)
    canv.SetTicks(1,1)
    
    grfull.Draw("APE")

    lat = ROOT.TLatex()
    lat.SetNDC(); lat.SetTextFont(42)
    lat.DrawLatex(0.18, 0.92, '#bf{CMS}')
    lat.DrawLatex(0.73, 0.92, '(13 TeV)')
    lat.DrawLatex(0.60,0.30, 'ECAL {part}'.format(part="Barrel" if eta<1.5 else "Endcap"))
    lat.DrawLatex(0.60,0.25, 'E = 50 GeV')
    lat.DrawLatex(0.60,0.20, '<PU> = 40')

    ## another tlatex to make the font smaller
    lat2 = ROOT.TLatex()
    lat2.SetNDC(); lat2.SetTextFont(42); lat2.SetTextSize(0.04)
    lat2.DrawLatex(0.30, 0.92, '#it{Standalone simulation}')
    
    subpad = ROOT.TPad("subpad","",0.18,0.47,0.55,0.89); subpad.Draw(); subpad.cd();
    subpad.SetGridx(); subpad.SetGridy()
    subpad.SetLeftMargin(0.17)
    subpad.SetRightMargin(0.01)
    subpad.SetBottomMargin(0.1)
    subpad.SetTopMargin(0.03)
    subpad.SetTicks(1,1)
    grzoom.GetYaxis().SetRangeUser(0.985,1.015)
    grzoom.GetYaxis().SetRangeUser(0.985,1.015)

    grzoom.GetXaxis().SetLabelSize(0.07)
    grzoom.GetYaxis().SetLabelSize(0.07)
    grzoom.GetXaxis().SetTitleSize(0)
    grzoom.GetYaxis().SetTitleSize(0)

    grzoom.Draw("APE")

    canv.SaveAs("bias_{part}_vs_dt.pdf".format(part='eb' if eta<1.5 else 'ee'))
    canv.SaveAs("bias_{part}_vs_dt.png".format(part='eb' if eta<1.5 else 'ee'))


if __name__ == "__main__":

    etas = [0.0,2.5]
    for eta in etas:
        plotOnePartition(eta)        
    
    
