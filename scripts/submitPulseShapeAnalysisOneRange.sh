#! /bin/bash
# bash submitPulseShapeAnalysisOneRange.sh 257394 257751

firstRun=$1
lastRun=$2

root -b <<EOF

.L PulseShapeAnalysis.cxx+
saveAllTemplatesOneRange($firstRun,$lastRun)
.q

EOF
