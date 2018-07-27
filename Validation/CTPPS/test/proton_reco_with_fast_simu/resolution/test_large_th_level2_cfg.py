from base_cff import *

process.maxEvents.input = $events

SetLargeTheta()

SetLevel2()

process.ctppsProtonReconstructionValidator.outputFile = "test_large_th_level2.root"
