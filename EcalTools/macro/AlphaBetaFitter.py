import ROOT as rt
import math

def alphabeta(x, par):
    # par[0] = normalization
    # par[1] = alpha
    # par[2] = beta
    # par[3] = tmax
    # par[4] = pedestal
    
    alphabeta = par[1]*par[2]
    deltat = x[0]-par[3]
    power_term = 1+deltat/par[1]/par[2] ** par[1]
    decay_term = math.exp(-deltat/par[2])
    if deltat > -alphabeta: fcn = par[0] * power_term * decay_term + par[4]
    else: fcn = par[4]
    return fcn

class AlphaBetaFitter:

    def __init__ (self,doEB,pedestal=.0):

        alpha = 1.250 if doEB else 1.283
        beta = 1.600 if doEB else 1.674

        self.fcnname = "alphabeta"

        self.fcn = rt.TF1(self.fcnname,alphabeta,0,10,5)
        self.fcn.SetParNames("norm","#alpha","#beta","tmax","pedestal","raiset");

        self.fcn.SetParLimits(0,0,10); # normalization
        self.fcn.SetParameter(1,alpha);
        self.fcn.SetParameter(2,beta);
        self.fcn.SetParameter(3,5.5);
        if(doEB): self.fcn.SetParLimits(1,0.8,2.5); # alpha
        else: self.fcn.SetParLimits(1,0.5,2.5);
        self.fcn.SetParLimits(2,0.8,2.5); # beta
        self.fcn.SetParLimits(3,4,7); # tmax
        self.fcn.FixParameter(5,pedestal);

        self.fcn.SetLineColor(rt.kRed+1);

        self.fitpars = []
        self.fiterrs = []

    def fit(self,histo):
        histo.Fit(self.fcnname,"Q WW M","same",0,10)
        for p in range(1,4):
            self.fitpars.append(self.fcn.GetParameter(p))
            self.fiterrs.append(self.fcn.GetParError(p))
        return

