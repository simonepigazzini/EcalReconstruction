import sys
energy = sys.argv[1]
eta = sys.argv[2]

steps = range(0,61,5)
for s in steps:
    print 'mv output_{s}pu.root data/waveform_signal_{ene}GeV_eta_{eta}_pu_{s}.root'.format(s=s,ene=energy,eta=eta)
