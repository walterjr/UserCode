from base_cff import *

process.maxEvents.input = $events

SetLowTheta()

SetLevel3()

process.ctppsProtonReconstructionValidator.outputFile = "test_low_th_level3.root"
