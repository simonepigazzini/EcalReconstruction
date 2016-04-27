import FWCore.ParameterSet.Config as cms

EL_ID_BDT=("((pt<10 && ((isEB && sigmaIetaIeta<0.01) || (isEE && sigmaIetaIeta<0.03)) ) || "+
            "(pt <= 20 && pt > 10) && (       abs(superCluster.eta) <  0.8    && userFloat('bdttrig') > 0.00  ||"+
                        " 0.8 <= abs(superCluster.eta) <  1.479  && userFloat('bdttrig') > 0.10 || "+
                        "        abs(superCluster.eta) >= 1.479  && userFloat('bdttrig') > 0.62) || "+
            "pt > 20 && (        abs(superCluster.eta) <  0.8    && userFloat('bdttrig') > 0.94  ||"+
                        " 0.8 <= abs(superCluster.eta) <  1.479  && userFloat('bdttrig') > 0.85 || "+
                        "        abs(superCluster.eta) >=  1.479 && userFloat('bdttrig') > 0.92))")
EL_CONV="gsfTrack.trackerExpectedHitsInner.numberOfHits == 0"

EL_PT_MIN=7;  EL_PT_MIN_LOOSE=7
EL_PRESELECTION = ("abs(eta) < 2.5")                                          ## is below

ELID_LOOSE_NO_PT_CUT = " && ".join([EL_PRESELECTION])
ELID_LOOSE = "pt > %f && %s" % (EL_PT_MIN_LOOSE, ELID_LOOSE_NO_PT_CUT)

ELID_GOOD = " && ".join(["pt > %f" % EL_PT_MIN, EL_ID_BDT])

SEL_ANY_Z = "daughter(0).pdgId = - daughter(1).pdgId"
SEL_MASS_Z = "40 < mass < 120"
SEL_BEST_Z = " && ".join([SEL_ANY_Z, SEL_MASS_Z])

