#! /usr/bin/env python
import ROOT
import root_numpy as rtnp

def getGraph(filename):
    tf = ROOT.TFile.Open(filename)
    tree = tf.Get("amplitudes")
    hsteps = tf.Get("hsteps")

    steps = rtnp.hist2array(hsteps)
    print steps

    gr = ROOT.TGraphErrors(len(steps))
    gr.SetTitle("")
    for i,s in enumerate(steps):
        hist = ROOT.TH1D("hist","",1000,0.95,1.05)
        tree.Draw("amplitude[{i}]/amplitudeTruth>>hist".format(i=i))
        bias = hist.GetMean()
        err = hist.GetMeanError()
        print "deltat = {dt} => bias = {bias}".format(dt=s,bias=bias)
        gr.SetPoint(i,s,bias)
        gr.SetPointError(i,0,err)
    gr.SetMarkerStyle(ROOT.kFullCircle)
    gr.SetMarkerSize(1.5)
    gr.Draw("ACPE")
    xax = gr.GetXaxis(); yax = gr.GetYaxis()
    xax.SetRangeUser(-4.,4.)
    yax.SetRangeUser(0.97,1.05)
    yax.SetDecimals()
    xax.SetTitleOffset(1.1); xax.SetTitleSize(0.05)
    yax.SetTitleOffset(1.5); yax.SetTitleSize(0.05)
    xax.SetTitle('#Delta T (ns)')
    yax.SetTitle('A/A_{true}')
    return gr
    
if __name__ == "__main__":

    grfull = getGraph("data/reco_signal_50GeV_eta_0.0_pu_40_shiftdt_pm5ns.root")
    grzoom = getGraph("data/reco_signal_50GeV_eta_0.0_pu_40_shiftdt_pm1ns.root")
    
    ROOT.gStyle.SetOptStat(0)

    canv = ROOT.TCanvas("canv","",1200,1200)
    canv.SetGridx()
    canv.SetGridy()
    canv.SetLeftMargin(0.17)
    canv.SetRightMargin(0.1)
    canv.SetBottomMargin(0.15)

    grfull.Draw("ALPE")

    lat = ROOT.TLatex()
    lat.SetNDC(); lat.SetTextFont(42)
    lat.DrawLatex(0.19, 0.92, '#bf{CMS} Simulation')
    lat.DrawLatex(0.73, 0.92, '(13 TeV)')
    lat.DrawLatex(0.60,0.20, 'E=50 GeV')
    
    subpad = ROOT.TPad("subpad","",0.22,0.5,0.55,0.88); subpad.Draw(); subpad.cd();
    subpad.SetGridx(); subpad.SetGridy()
    subpad.SetLeftMargin(0.17)
    subpad.SetBottomMargin(0.15)
    grzoom.GetYaxis().SetRangeUser(0.985,1.015)
    grzoom.GetYaxis().SetRangeUser(0.985,1.015)
    grzoom.Draw("ALPE")

    canv.SaveAs("bias_vs_dt.pdf")
    
