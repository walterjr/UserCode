from base_cff import *

process.maxEvents.input = $events

SetLowTheta()

SetLevel4()

process.ctppsProtonReconstructionValidator.outputFile = "test_low_th_level4.root"
