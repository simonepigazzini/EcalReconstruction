#! /usr/bin/env python 
import ROOT as rt

"""
Style options mostly from CMS's tdrStyle.C
"""
def customROOTstyle() :
    rt.gROOT.SetBatch(True)
    rt.gStyle.SetOptTitle(True)
    rt.gStyle.SetOptStat(False)
    rt.gStyle.SetPadTopMargin(0.06);
    rt.gStyle.SetPadBottomMargin(0.13);
    rt.gStyle.SetPadLeftMargin(0.12);
    rt.gStyle.SetPadRightMargin(.15)
    rt.gStyle.SetLabelColor(1, "XYZ");
    rt.gStyle.SetLabelFont(42, "XYZ");
    rt.gStyle.SetLabelOffset(0.007, "XYZ");
    rt.gStyle.SetLabelSize(0.05, "XYZ");
    rt.gStyle.SetTitleSize(0.05, "XYZ");
    rt.gStyle.SetTitleOffset(1.0, "X");
    rt.gStyle.SetTitleOffset(1.1, "Y");
    rt.gStyle.SetTitleOffset(1.0, "Z");
    rt.gStyle.SetAxisColor(1, "XYZ");
    rt.gStyle.SetStripDecimals(True);
    rt.gStyle.SetTickLength(0.03, "XYZ");
    rt.gStyle.SetNdivisions(510, "XYZ");
    rt.gStyle.SetPadTickX(0);
    rt.gStyle.SetPadTickY(0);
    rt.gStyle.SetMarkerStyle(20);
    rt.gStyle.SetHistLineColor(1);
    rt.gStyle.SetHistLineStyle(1);
    rt.gStyle.SetHistLineWidth(3);
    rt.gStyle.SetFrameBorderMode(0);
    rt.gStyle.SetFrameBorderSize(1);
    rt.gStyle.SetFrameFillColor(0);
    rt.gStyle.SetFrameFillStyle(0);
    rt.gStyle.SetFrameLineColor(1);
    rt.gStyle.SetFrameLineStyle(1);
    rt.gStyle.SetFrameLineWidth(1);
    rt.gStyle.SetPalette(55);
    rt.gStyle.SetNumberContours(100);

import numpy as np
def customPalette(zeropoint = 0.5):
    Number = 3;
    Red    = np.array([0  ,  100,  110],dtype=float)/255.
    Green  = np.array([0  ,  255,  0], dtype=float)/255.
    Blue   = np.array([99 ,  100,  2], dtype=float)/255.
    Length = np.array([0.0,  zeropoint, 1.0], dtype=float)
    nb=100;
    rt.TColor.CreateGradientColorTable(Number,Length,Red,Green,Blue,nb)
