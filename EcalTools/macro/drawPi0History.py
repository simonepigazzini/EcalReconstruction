#! /usr/bin/env python
from array import array
from math import *
import ROOT
ROOT.gROOT.SetBatch(True)

if __name__ == "__main__":

    files = {"weights":   "Weights_pi0_no_proj.root",
             "multifit":  "Multifit_pi0_no_proj.root" }
    canvases = {"weights":  "pi0_EB_minus_1",
                "multifit": "pi0_EB_1"}

    graphs = {}
    
    for k,f in files.items():
        print ("processing reco ",k)
        tf = ROOT.TFile(f)
        canv = tf.Get(canvases[k])
        for obj in canv.GetListOfPrimitives():
            if obj.InheritsFrom("TMultiGraph"):
                print ("this is a MG: ",obj.GetName())
                for gr in obj.GetListOfGraphs():
                    print ("\t single gr name = ",gr.GetName())
                    if "_cor_" in gr.GetName():
                        graphs[k] = gr.Clone("pi0_"+k)
                        graphs[k].SetMarkerSize(1.3)
                        graphs[k].SetTitle("")

    print ("Graphs = ",graphs)

    ROOT.gStyle.SetPaintTextFormat('.2f')
    canv = ROOT.TCanvas("canv","",2400,1200)
    canv.SetGridx()
    canv.SetGridy()
    canv.SetLeftMargin(0.10)
    canv.SetRightMargin(0.05)
    canv.SetBottomMargin(0.15)

    graphs['multifit'].SetMarkerColor(ROOT.kAzure+7)
    graphs['multifit'].SetLineColor(ROOT.kAzure+7)
    graphs['multifit'].SetMarkerStyle(ROOT.kFullSquare)
    graphs['weights'].SetMarkerColor(ROOT.kMagenta+2)
    graphs['weights'].SetLineColor(ROOT.kMagenta+2)
    graphs['weights'].SetMarkerStyle(ROOT.kFullCircle)

    leg = ROOT.TLegend(0.60,0.70,0.90,0.85,"","brNDC")
    leg.SetBorderSize(0)

    igr = 0
    for k,gr in graphs.items():
        if igr==0:
            gr.GetYaxis().SetRangeUser(0.985,1.02)
            gr.GetXaxis().SetTimeFormat("%d/%H:%M");
            gr.GetXaxis().LabelsOption("v");
            gr.GetXaxis().SetTimeOffset(0,"gmt");
            gr.GetXaxis().SetTimeDisplay(1);
            gr.GetXaxis().SetNdivisions(406);
            gr.GetXaxis().SetLabelSize(0.04);
            gr.GetYaxis().SetLabelSize(0.04);
            gr.GetXaxis().SetTitleSize(0.06);
            gr.GetYaxis().SetTitleSize(0.06);
            gr.GetYaxis().SetTitleOffset(0.8);
            gr.GetYaxis().SetNdivisions(405);
            gr.GetXaxis().SetTitle("Date in Oct. 2017 (day/hr:min)");
            gr.GetYaxis().SetTitle("m_{#gamma#gamma}/m_{#pi^{0}}")
            gr.Draw("Ape")
        else:
            gr.Draw("pe")
        leg.AddEntry(gr,k,"pe")                       
        igr += 1

    leg.Draw()
    
    lat = ROOT.TLatex()
    lat.SetNDC(); lat.SetTextFont(42)
    lat.DrawLatex(0.12, 0.92, '#bf{CMS}')
    lat.DrawLatex(0.77, 0.92, '0.5 fb^{-1} (13 TeV)')
    canv.SaveAs('pi0hist.pdf')
