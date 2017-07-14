import ROOT as rt
import math
from enum import Enum

def alphabeta(x, par):
    # par[0] = normalization
    # par[1] = alpha
    # par[2] = beta
    # par[3] = tmax
    # par[4] = pedestal
    
    alphabeta = par[1]*par[2]
    deltat = x[0]-par[3]
    power_term = rt.TMath.Power(1+deltat/par[1]/par[2], par[1])
    decay_term = math.exp(-deltat/par[2])
    if deltat > -alphabeta: fcn = par[0] * power_term * decay_term + par[4]
    else: fcn = par[4]
    return fcn

class AlphaBetaParameter(Enum):
    alpha = 0
    beta = 1
    T0 = 2

class AlphaBetaFitter:

    def __init__ (self,fcn,doEB,pedestal=0.):

        self.doEB = doEB
        self.alpha = 1.250 if doEB else 1.283
        self.beta = 1.600 if doEB else 1.674

        #self.fcn = rt.TF1(self.fcnname,alphabeta,0,10,5)
        self.fcn = fcn

        self.fcn.SetParNames("norm","#alpha","#beta","tmax","pedestal");

        #self.fcn.SetParLimits(0,0,10); # normalization
        self.fcn.FixParameter(0,1);
        self.fcn.SetParameter(1,self.alpha);
        self.fcn.SetParameter(2,self.beta);
        self.fcn.SetParameter(3,5.5);
        if(doEB): self.fcn.SetParLimits(1,0.8,2.5); # alpha
        else: self.fcn.SetParLimits(1,0.5,2.5);
        self.fcn.SetParLimits(2,0.8,2.5); # beta
        self.fcn.SetParLimits(3,4,7); # tmax
        self.fcn.FixParameter(4,pedestal);

        self.fcn.SetLineColor(rt.kRed+1);


    def fit(self,histo,doEB,canvasName=''):

        if len(canvasName)>0: 
            canv = rt.TCanvas("fitc","",600,600)
            histo.Draw("hist E2")

        histo.Fit("alphabeta","QM","same",0,10)
        fitpars = []
        fiterrs = []
        for p in range(1,4):
            fitpars.append(self.fcn.GetParameter(p))
            fiterrs.append(self.fcn.GetParError(p))
        ret = {}
        ret['pars'] = fitpars
        ret['errs'] = fiterrs

        if len(canvasName)>0: 
            self.fcn.Draw("same")
            canv.SaveAs(canvasName)

        return ret

    def getFcn(self): 
        return self.fcn
