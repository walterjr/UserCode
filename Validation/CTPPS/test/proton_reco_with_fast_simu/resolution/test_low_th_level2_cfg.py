from base_cff import *

process.maxEvents.input = $events

SetLowTheta()

SetLevel2()

process.ctppsProtonReconstructionValidator.outputFile = "test_low_th_level2.root"
