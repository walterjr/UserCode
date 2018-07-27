from base_cff import *

process.maxEvents.input = $events

SetLargeTheta()

SetLevel4()

process.ctppsProtonReconstructionValidator.outputFile = "test_large_th_level4.root"
