#! /usr/bin/env python
from array import array
from math import *
import ROOT
ROOT.gROOT.SetBatch(True)

nsamples = 12

def covMatrix(barrel):
    ebcov = [3.001e-06,  1.233e-05,  0.000e+00, -4.416e-06, -4.571e-06, -3.614e-06, -2.636e-06, -1.286e-06, -8.410e-07, -5.296e-07, -1.000e-07, -1.000e-08, 
             1.233e-05,  6.154e-05,  0.000e+00, -2.200e-05, -2.309e-05, -1.838e-05, -1.373e-05, -7.334e-06, -5.088e-06, -3.745e-06, -2.428e-07, -1.500e-08, 
             0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00, 
             -4.416e-06, -2.200e-05,  0.000e+00,  8.319e-06,  8.545e-06,  6.792e-06,  5.059e-06,  2.678e-06,  1.816e-06,  1.223e-06,  8.245e-07,  5.589e-07, 
             -4.571e-06, -2.309e-05,  0.000e+00,  8.545e-06,  9.182e-06,  7.219e-06,  5.388e-06,  2.853e-06,  1.944e-06,  1.324e-06,  9.083e-07,  6.335e-07, 
             -3.614e-06, -1.838e-05,  0.000e+00,  6.792e-06,  7.219e-06,  6.016e-06,  4.437e-06,  2.385e-06,  1.636e-06,  1.118e-06,  7.754e-07,  5.556e-07, 
             -2.636e-06, -1.373e-05,  0.000e+00,  5.059e-06,  5.388e-06,  4.437e-06,  3.602e-06,  1.917e-06,  1.322e-06,  9.079e-07,  6.529e-07,  4.752e-07, 
             -1.286e-06, -7.334e-06,  0.000e+00,  2.678e-06,  2.853e-06,  2.385e-06,  1.917e-06,  1.375e-06,  9.100e-07,  6.455e-07,  4.693e-07,  3.657e-07, 
             -8.410e-07, -5.088e-06,  0.000e+00,  1.816e-06,  1.944e-06,  1.636e-06,  1.322e-06,  9.100e-07,  9.115e-07,  6.062e-07,  4.436e-07,  3.422e-07, 
             -5.296e-07, -3.745e-06,  0.000e+00,  1.223e-06,  1.324e-06,  1.118e-06,  9.079e-07,  6.455e-07,  6.062e-07,  7.217e-07,  4.862e-07,  3.768e-07, 
             -2.230e-07, -2.428e-06,  0.000e+00,  8.245e-07,  9.083e-07,  7.754e-07,  6.529e-07,  4.693e-07,  4.436e-07,  4.862e-07,  6.509e-07,  4.418e-07, 
             -1.100e-07, -1.000e-06,  0.000e+00,  5.589e-07,  6.335e-07,  5.556e-07,  4.752e-07,  3.657e-07,  3.422e-07,  3.768e-07,  4.418e-07,  6.142e-07 ]

    eecov = [3.941e-05,  3.333e-05,  0.000e+00, -1.449e-05, -1.661e-05, -1.424e-05, -1.183e-05, -6.842e-06, -4.915e-06, -3.411e-06, -1.100e-06, -3.230e-07,
             3.333e-05,  2.862e-05,  0.000e+00, -1.244e-05, -1.431e-05, -1.233e-05, -1.032e-05, -5.883e-06, -4.154e-06, -2.902e-06, -2.128e-06, -5.000e-07, 
             0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00,  0.000e+00, 
             -1.449e-05, -1.244e-05,  0.000e+00,  5.840e-06,  6.649e-06,  5.720e-06,  4.812e-06,  2.708e-06,  1.869e-06,  1.330e-06,  9.186e-07,  6.446e-07, 
             -1.661e-05, -1.431e-05,  0.000e+00,  6.649e-06,  7.966e-06,  6.898e-06,  5.794e-06,  3.157e-06,  2.184e-06,  1.567e-06,  1.084e-06,  7.575e-07, 
             -1.424e-05, -1.233e-05,  0.000e+00,  5.720e-06,  6.898e-06,  6.341e-06,  5.347e-06,  2.859e-06,  1.991e-06,  1.431e-06,  9.839e-07,  6.886e-07, 
             -1.183e-05, -1.032e-05,  0.000e+00,  4.812e-06,  5.794e-06,  5.347e-06,  4.854e-06,  2.628e-06,  1.809e-06,  1.289e-06,  9.020e-07,  6.146e-07, 
             -6.842e-06, -5.883e-06,  0.000e+00,  2.708e-06,  3.157e-06,  2.859e-06,  2.628e-06,  1.863e-06,  1.296e-06,  8.882e-07,  6.108e-07,  4.283e-07, 
             -4.915e-06, -4.154e-06,  0.000e+00,  1.869e-06,  2.184e-06,  1.991e-06,  1.809e-06,  1.296e-06,  1.217e-06,  8.669e-07,  5.751e-07,  3.882e-07, 
             -3.411e-06, -2.902e-06,  0.000e+00,  1.330e-06,  1.567e-06,  1.431e-06,  1.289e-06,  8.882e-07,  8.669e-07,  9.522e-07,  6.717e-07,  4.293e-07, 
             -1.000e-06, -2.128e-06,  0.000e+00,  9.186e-07,  1.084e-06,  9.839e-07,  9.020e-07,  6.108e-07,  5.751e-07,  6.717e-07,  7.911e-07,  5.493e-07, 
             -7.323e-07, -1.320e-06,  0.000e+00,  6.446e-07,  7.575e-07,  6.886e-07,  6.146e-07,  4.283e-07,  3.882e-07,  4.293e-07,  5.493e-07,  7.027e-07 ]

    covMat = {}
    for k in range(nsamples*nsamples):
        i = k//nsamples
        j = k%nsamples
        covMat[(i,j)] = ebcov[k] if barrel else eecov[k]
    return covMat

    
def pulseCovariance(barrel):
    covMat = covMatrix(barrel)
    pulseCov = ROOT.TH2D("pulseCov","",nsamples,0,nsamples,nsamples,0,nsamples)
    [ pulseCov.SetBinContent(i+1,j+1,covMat[(i,j)]) for i in range(nsamples) for j in range(nsamples) ]
    return pulseCov

def pulseCorrelation(barrel):
    covMat = covMatrix(barrel)
    pulseCorr = ROOT.TH2D("pulseCorr","",nsamples,0,nsamples,nsamples,0,nsamples)
    for i in range(nsamples):
        for j in range(nsamples):
            val = covMat[(i,j)]/sqrt(abs(covMat[(i,i)]*covMat[(j,j)])) if covMat[(i,i)]*covMat[(j,j)] !=0 else 0
            #print "val = ", val
            if abs(val)>1: print (i," ",j,"  ",i*nsamples+j,"  ",covMat[(i,j)],"  ",sqrt(abs(covMat[(i,i)])),"  ",sqrt(abs(covMat[(j,j)])))
            pulseCorr.SetBinContent(i+1,j+1,int(100*val))
    return pulseCorr
            

def covMatAxisLabel(bin):
    label = 'S_{{ {idx} }}'.format(idx=bin+3)
    return label
    
if __name__ == "__main__":

    lat = ROOT.TLatex()
    lat.SetNDC(); lat.SetTextFont(42)
            
    # draw pulse covariance
    ROOT.gStyle.SetOptStat(0)
    ROOT.TColor.CreateGradientColorTable(3,
                                         array ("d", [0.00, 0.54, 1.00]),
                                         ##array ("d", [1.00, 1.00, 0.00]),
                                         ##array ("d", [0.70, 1.00, 0.34]),
                                         ##array ("d", [0.00, 1.00, 0.82]),
                                         array ("d", [0.00, 1.00, 1.00]),
                                         array ("d", [0.36, 1.00, 0.63]),
                                         array ("d", [0.82, 1.00, 0.00]),
                                         255,  0.95)

    
    ROOT.gStyle.SetPaintTextFormat('.0f')
    canv = ROOT.TCanvas("canv","",1200,1200)
    canv.SetGridx()
    canv.SetGridy()
    canv.SetLeftMargin(0.18)
    canv.SetRightMargin(0.18)
    canv.SetBottomMargin(0.18)
    #canv.SetLogz(1)
    
    for iseb in range(2):
        covmat = pulseCorrelation(iseb)
        
        rmax = max(abs(covmat.GetMaximum()),abs(covmat.GetMinimum()))
        covmat.GetZaxis().SetRangeUser(-rmax,rmax)

        covmat.GetXaxis().SetTickLength(0.)
        covmat.GetYaxis().SetTickLength(0.)
        covmat.GetXaxis().LabelsOption("v")
        covmat.SetMarkerSize(1.5)
        for xbin in range(nsamples):
            covmat.GetXaxis().SetBinLabel(xbin+1,covMatAxisLabel(xbin))
            covmat.GetYaxis().SetBinLabel(xbin+1,covMatAxisLabel(xbin))

        
        covmat.GetXaxis().SetTitleOffset(1.5)
        covmat.GetYaxis().SetTitleOffset(1.5)
        covmat.GetZaxis().SetTitleOffset(1.2)
        covmat.GetXaxis().SetTitleSize(0.05)
        covmat.GetYaxis().SetTitleSize(0.05)
        covmat.GetZaxis().SetTitleSize(0.05)
        
        covmat.GetXaxis().SetTitle('Time sample')
        covmat.GetYaxis().SetTitle('Time sample')
        covmat.GetZaxis().SetTitle('{subdet}  #bf{{#rho}}_{{pulse}} (%)'.format(subdet = 'Barrel' if iseb else 'Endcap') )
        covmat.GetZaxis().CenterTitle()
        covmat.Draw("colz text45")
        lat.DrawLatex(0.16, 0.92, '#bf{CMS}')
        lat.DrawLatex(0.60, 0.92, '1 fb^{-1} (13 TeV)')
        canv.SaveAs('pulsecorr_ieb{iseb}.pdf'.format(iseb=iseb))

        
