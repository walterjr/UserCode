from base_cff import *

process.maxEvents.input = $events

SetLargeTheta()

SetLevel3()

process.ctppsProtonReconstructionValidator.outputFile = "test_large_th_level3.root"
