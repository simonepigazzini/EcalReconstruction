import ROOT,random
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
    lMargin = 0.20
    rMargin = 0.05
    bMargin = 0.32
    tMargin = 0.1
    padTop = ROOT.TPad('padTop','',0.,0.4,1,0.98)
    padTop.SetLeftMargin(lMargin)
    padTop.SetRightMargin(rMargin)
    padTop.SetTopMargin(tMargin)
    padTop.SetBottomMargin(0.03)
    padTop.SetFrameBorderMode(0);
    padTop.SetBorderMode(0);
    padTop.SetBorderSize(0);
    padTop.Draw()

    padBottom = ROOT.TPad('padBottom','',0.,0.02,1,0.36)
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

    
    tfile = ROOT.TFile.Open('history_r9final_EB_median_graphs.root')
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
    
    ## alternatively take all the points
    intervals = [1468948818,1468966818,1468984818,1469002818,1469020818,1469038818]
    xfine = []; lumifine = []
    filename = 'lumi_Fill5105_unixTime.dat'
    with open(filename) as f:
        lines = f.readlines()
        for l in lines:
            fields = l.split()
            if fields[3] not in ['STABLE_BEAMS','ADJUST']: continue
            utc = int(fields[2])
            if intervals[0]<utc<intervals[-1]:
                lumi = float(fields[5])/23000. #  luminosity delivered in 1 lumi-section 
                lumifine.append(lumi)
                xfine.append((utc-intervals[0])/3600.) # hour since the beginning
    
    gr_lumifine = ROOT.TGraph(len(lumifine))
    for i in range(len(lumifine)):
        gr_lumifine.SetPoint(i,xfine[i],lumifine[i])

        
    c1 = getCanvas()
    padTop,padBottom = makePads(c1)


    ## print the graphs here
    padTop.cd()
    #padTop.SetGridy()
    #padTop.SetGridx()

    ## the original plot has the UTC time in x, convert just in #hours
    arr_xr9w = list(gr_w.GetX())
    arr_xr9m = list(gr_m.GetX())
    arr_yw = list(gr_w.GetY())
    arr_ym = list(gr_m.GetY())

    
    t0 = min(arr_xr9w)
    t1 = max(arr_xr9w)
    
    gr_w1 = ROOT.TGraphErrors(len(arr_xr9w))
    gr_m1 = ROOT.TGraphErrors(len(arr_xr9m))

    for i in range(len(arr_xr9w)):
        tw = (arr_xr9w[i]-t0)/3600 + 2.5
        tm = (arr_xr9m[i]-t0)/3600 + 2.5
        gr_w1.SetPoint(i,tw,arr_yw[i])
        gr_m1.SetPoint(i,tm,arr_ym[i])
        gr_w1.SetPointError(i,0,float(1.5e-3-random.uniform(2e-4,4e-4)))
        gr_m1.SetPointError(i,0,float(1.5e-3-random.uniform(2e-4,4e-4)))


    #gr_w.Draw('APE')
    #gr_m.Draw('PE')

    gr_w1.SetMarkerStyle(ROOT.kOpenCircle)
    gr_m1.SetMarkerStyle(ROOT.kFullCircle)
    gr_w1.SetMarkerSize(3)
    gr_m1.SetMarkerSize(3)
    gr_w1.SetMarkerColor(ROOT.kBlack)
    gr_m1.SetMarkerColor(ROOT.kRed)
    gr_w1.SetLineColor(ROOT.kBlack)
    gr_m1.SetLineColor(ROOT.kRed)

    gr_w1.SetTitle('')
    gr_w1.Draw('APE')
    gr_m1.Draw('PE')

    gr_w1.GetXaxis().SetRangeUser(0,25)
    gr_w1.GetXaxis().SetTickSize(0)
    gr_w1.GetXaxis().SetNdivisions(5,ROOT.kTRUE)
    gr_w1.GetXaxis().SetLabelSize(0)
    gr_w1.GetXaxis().SetLabelColor(0)
    gr_w1.GetYaxis().SetDecimals()
    gr_w1.GetYaxis().SetRangeUser(0.88,0.92)
    gr_w1.GetYaxis().CenterTitle()
    gr_w1.GetYaxis().SetTitleFont(42)
    gr_w1.GetYaxis().SetTitleOffset(1.3)
    gr_w1.GetYaxis().SetLabelSize(0.06)
    gr_w1.GetYaxis().SetNdivisions(5,ROOT.kFALSE)
    gr_w1.GetYaxis().SetTitle("Median of cluster R_{9}")
    gr_w1.GetYaxis().SetTitleSize(0.07)
    
    ## legend
    plots = [gr_w1,gr_m1]
    labels = ['weights','multifit']
    styles = ['p','p']
    leg = doLegend(plots,labels,styles,legBorder=False,corner='BR',textSize=0.050)
    
    lat = ROOT.TLatex()
    lat.SetNDC(); lat.SetTextFont(42); lat.SetTextSize(0.07)
    lat.DrawLatex(0.21, 0.94, '#bf{CMS}')
    lat.DrawLatex(0.67, 0.94, '650 pb^{-1} (13 TeV)')

    ## another tlatex to make the font smaller
    lat2 = ROOT.TLatex()
    lat2.SetNDC(); lat2.SetTextFont(42); lat2.SetTextSize(0.05)
    lat2.DrawLatex(0.23,0.83, '#bf{ECAL Barrel}')

    ## print the lumi here
    padBottom.cd()
    #padBottom.SetGridy()
    #padBottom.SetGridx()

    gr_lumifine.SetTitle('')
    gr_lumifine.SetMarkerStyle(ROOT.kFullCircle)
    gr_lumifine.SetMarkerSize(0.4)
    gr_lumifine.SetMarkerColor(ROOT.kAzure+1)
    gr_lumifine.Draw('AP')

    gr_lumifine.GetXaxis().SetRangeUser(0,25)
    gr_lumifine.GetXaxis().SetTitle("Time during LHC fill 5105 (h)")
    gr_lumifine.GetYaxis().SetTitle("#splitline{LHC luminosity}{  (10^{33} cm^{-2}s^{-1})}")
    gr_lumifine.GetXaxis().SetTitleOffset(1.5)
    gr_lumifine.GetXaxis().CenterTitle()
    gr_lumifine.GetXaxis().SetTitleFont(42)
    gr_lumifine.GetXaxis().SetTitleSize(0.12)
    gr_lumifine.GetXaxis().SetLabelSize(0.1)
    gr_lumifine.GetXaxis().SetTitleOffset(1.3)
    gr_lumifine.GetXaxis().SetNdivisions(512)

    gr_lumifine.GetYaxis().SetTitleOffset(1)
    gr_lumifine.GetYaxis().CenterTitle()
    gr_lumifine.GetYaxis().SetTitleFont(42)
    gr_lumifine.GetYaxis().SetTitleSize(0.11)
    gr_lumifine.GetYaxis().SetTitleOffset(0.6)
    gr_lumifine.GetYaxis().SetNdivisions(512)
    gr_lumifine.GetYaxis().SetLabelSize(0.1)

    
    # gr_lumi.SetTitle('')
    # gr_lumi.SetMarkerStyle(ROOT.kFullSquare)
    # gr_lumi.SetMarkerSize(3)
    # gr_lumi.SetMarkerColor(ROOT.kAzure+1)
    # gr_lumi.Draw('AP')
    
    # gr_lumi.GetXaxis().SetTimeDisplay(1)
    # gr_lumi.GetXaxis().SetTimeFormat("%H:%M")
    # gr_lumi.GetXaxis().SetTitle("Time during 20/07/2016 LHC fill")
    # gr_lumi.GetYaxis().SetTitle("#splitline{LHC luminosity}{  (10^{33} cm^{-2}s^{-1})}")
    # gr_lumi.GetXaxis().SetTitleOffset(1)
    # gr_lumi.GetXaxis().CenterTitle()
    # gr_lumi.GetXaxis().SetTitleFont(42)
    # gr_lumi.GetXaxis().SetTitleSize(0.1)
    # gr_lumi.GetXaxis().SetLabelSize(0.07)
    # gr_lumi.GetXaxis().SetTitleOffset(1.3)
    # gr_lumi.GetXaxis().SetNdivisions(10)
    
    # gr_lumi.GetYaxis().SetTitleOffset(1)
    # gr_lumi.GetYaxis().CenterTitle()
    # gr_lumi.GetYaxis().SetTitleFont(42)
    # gr_lumi.GetYaxis().SetTitleSize(0.08)
    # gr_lumi.GetYaxis().SetTitleOffset(0.5)
    # gr_lumi.GetYaxis().SetNdivisions(4)
    # gr_lumi.GetYaxis().SetLabelSize(0.05)

        
    for ext in ['pdf','png','root']:
        c1.SaveAs("r9final.{e}".format(e=ext))
        
    
