import ROOT
ROOT.gROOT.SetBatch(True)

from PlotUtils import doLegend

def getCanvas():
    c = ROOT.TCanvas('c','',1200,1200)
    lMargin = 0.14
    rMargin = 0.10
    bMargin = 0.15
    tMargin = 0.10
    c.SetLeftMargin(lMargin)
    c.SetRightMargin(rMargin)
    c.SetTopMargin(tMargin)
    c.SetBottomMargin(bMargin)
    c.SetFrameBorderMode(0);
    c.SetBorderMode(0);
    c.SetBorderSize(0);
    return c

def makePads(canv):
    ROOT.gStyle.SetOptStat(0)
    
    canv.cd()
    lMargin = 0.12
    rMargin = 0.05
    bMargin = 0.30
    tMargin = 0.07
    padTop = ROOT.TPad('padTop','',0.,0.4,1,0.98)
    padTop.SetLeftMargin(lMargin)
    padTop.SetRightMargin(rMargin)
    padTop.SetTopMargin(tMargin)
    padTop.SetBottomMargin(0)
    padTop.SetFrameBorderMode(0);
    padTop.SetBorderMode(0);
    padTop.SetBorderSize(0);
    padTop.Draw()

    padBottom = ROOT.TPad('padBottom','',0.,0.02,1,0.4)
    padBottom.SetLeftMargin(lMargin)
    padBottom.SetRightMargin(rMargin)
    padBottom.SetTopMargin(0)
    padBottom.SetBottomMargin(bMargin)
    padBottom.SetFrameBorderMode(0);
    padBottom.SetBorderMode(0);
    padBottom.SetBorderSize(0);
    padBottom.Draw()

    return (padTop,padBottom)

if __name__ == "__main__":

    
    tfile = ROOT.TFile.Open('history_mass_EB_median_graphs.root')
    gr_w = tfile.Get('history_weights_EB_median')
    gr_m = tfile.Get('history_multifit_EB_median')

    onehour = 60*60
    deltat = 5*onehour
    ## these comes from running getAverageLumi.py
    lumi_averages = [10.497468590590163, 8.224083927348499, 6.6569549762386195, 5.815147175546296, 4.875020212957668]
    x = [deltat*(i+0.5) for i in range(len(lumi_averages))]

    gr_lumi = ROOT.TGraph(len(lumi_averages))
    for i in range(len(lumi_averages)):
        gr_lumi.SetPoint(i,x[i],lumi_averages[i])
    
    c1 = getCanvas()
    padTop,padBottom = makePads(c1)


    ## print the graphs here
    padTop.cd()
    padTop.SetGridy()
    padTop.SetGridx()
    gr_w.SetMarkerSize(3)
    gr_m.SetMarkerSize(3)
    gr_w.Draw('APE')
    gr_m.Draw('PE')

    gr_w.GetYaxis().SetRangeUser(0.878,0.92)
    gr_w.GetYaxis().CenterTitle()
    gr_w.GetYaxis().SetTitleFont(42)
    gr_w.GetYaxis().SetTitleOffset(1.0)
    gr_w.GetXaxis().SetTickSize(0)
    gr_w.GetXaxis().SetNdivisions(10)
    
    ## legend
    plots = [gr_w,gr_m]
    labels = ['weights','multifit']
    styles = ['p','p']
    leg = doLegend(plots,labels,styles,legBorder=False,corner='BR')
    
    lat = ROOT.TLatex()
    lat.SetNDC(); lat.SetTextFont(42)
    lat.DrawLatex(0.13, 0.94, '#bf{CMS}')
    lat.DrawLatex(0.77, 0.94, '6 fb^{-1} (13 TeV)')
    lat.DrawLatex(0.20,0.83, '#bf{ECAL Barrel}')

    ## print the lumi here
    padBottom.cd()
    padBottom.SetGridy()
    padBottom.SetGridx()
    gr_lumi.SetTitle('')
    gr_lumi.SetMarkerStyle(ROOT.kFullSquare)
    gr_lumi.SetMarkerSize(3)
    gr_lumi.SetMarkerColor(ROOT.kAzure+1)
    gr_lumi.Draw('AP')
    
    gr_lumi.GetXaxis().SetTimeDisplay(1)
    gr_lumi.GetXaxis().SetTimeFormat("%H:%M")
    gr_lumi.GetXaxis().SetTitle("Time during 20/07/2016 LHC fill")
    gr_lumi.GetYaxis().SetTitle("#splitline{LHC luminosity}{  (10^{33} cm^{-2}s^{-1})}")
    gr_lumi.GetXaxis().SetTitleOffset(1)
    gr_lumi.GetXaxis().CenterTitle()
    gr_lumi.GetXaxis().SetTitleFont(42)
    gr_lumi.GetXaxis().SetTitleSize(0.1)
    gr_lumi.GetXaxis().SetLabelSize(0.07)
    gr_lumi.GetXaxis().SetTitleOffset(1.3)
    gr_lumi.GetXaxis().SetNdivisions(10)
    
    gr_lumi.GetYaxis().SetTitleOffset(1)
    gr_lumi.GetYaxis().CenterTitle()
    gr_lumi.GetYaxis().SetTitleFont(42)
    gr_lumi.GetYaxis().SetTitleSize(0.08)
    gr_lumi.GetYaxis().SetTitleOffset(0.5)
    gr_lumi.GetYaxis().SetNdivisions(4)
    gr_lumi.GetYaxis().SetLabelSize(0.05)

        
    c1.SaveAs("r9final.pdf")
    c1.SaveAs("r9final.png")
        
    
