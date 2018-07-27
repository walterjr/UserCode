from base_cff import *

process.maxEvents.input = $events

SetLargeTheta()

SetLevel1()

process.ctppsProtonReconstructionValidator.outputFile = "test_large_th_level1.root"
