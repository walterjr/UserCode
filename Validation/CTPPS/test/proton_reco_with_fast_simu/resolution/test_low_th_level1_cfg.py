from base_cff import *

process.maxEvents.input = $events

SetLowTheta()

SetLevel1()

process.ctppsProtonReconstructionValidator.outputFile = "test_low_th_level1.root"
