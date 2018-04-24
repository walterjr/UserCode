import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
process = cms.Process('CTPPSFastSimulation', eras.ctpps_2016)

# minimal logger settings
process.MessageLogger = cms.Service("MessageLogger",
    statistics = cms.untracked.vstring(),
    destinations = cms.untracked.vstring('cerr'),
    cerr = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING')
    )
)

# number of events
process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# particle-data table
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# particle generator
process.generator = cms.EDProducer("RandomtXiGunProducer",
  Verbosity = cms.untracked.int32(0),

  FireBackward = cms.bool(True),
  FireForward = cms.bool(True),

  PGunParameters = cms.PSet(
    PartID = cms.vint32(2212),
    ECMS = cms.double(13E3),

    Mint = cms.double(0),
    Maxt = cms.double(1),
    MinXi = cms.double(0.0),
    MaxXi = cms.double(0.1),

    MinPhi = cms.double(-3.14159265359),
    MaxPhi = cms.double(+3.14159265359)
  )
)

# random seeds
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    sourceSeed = cms.PSet(initialSeed =cms.untracked.uint32(98765)),
    generator = cms.PSet(initialSeed = cms.untracked.uint32(98766)),
    beamDivergenceVtxGenerator = cms.PSet(initialSeed =cms.untracked.uint32(3849))
)

# geometry
from Geometry.VeryForwardGeometry.geometryRP_cfi import totemGeomXMLFiles, ctppsDiamondGeomXMLFiles

process.XMLIdealGeometryESSource_CTPPS = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = totemGeomXMLFiles+ctppsDiamondGeomXMLFiles+["Validation/CTPPS/test/fast_simu_with_phys_generator/qgsjet/global/RP_Dist_Beam_Cent.xml"],
    rootNodeName = cms.string('cms:CMSE'),
)

process.TotemRPGeometryESModule = cms.ESProducer("TotemRPGeometryESModule")

# beam-smearing settings
process.load("IOMC.EventVertexGenerators.beamDivergenceVtxGenerator_cfi")
process.beamDivergenceVtxGenerator.src = cms.InputTag("generator", "unsmeared")

process.beamDivergenceVtxGenerator.simulateBeamDivergence = True
process.beamDivergenceVtxGenerator.simulateVertex = True

# values in rad
process.beamDivergenceVtxGenerator.beamDivergenceX = 20E-6
process.beamDivergenceVtxGenerator.beamDivergenceY = 20E-6

# values in cm
process.beamDivergenceVtxGenerator.vertexMeanX = 0.
process.beamDivergenceVtxGenerator.vertexMeanY = 0.
process.beamDivergenceVtxGenerator.vertexMeanZ = 0.

process.beamDivergenceVtxGenerator.vertexSigmaX = 10E-4
process.beamDivergenceVtxGenerator.vertexSigmaY = 10E-4
process.beamDivergenceVtxGenerator.vertexSigmaZ = 5

# fast simulation
process.load('SimCTPPS.OpticsParameterisation.ctppsFastProtonSimulation_cfi')
process.ctppsFastProtonSimulation.checkApertures = True
process.ctppsFastProtonSimulation.produceHitsRelativeToBeam = False
process.ctppsFastProtonSimulation.roundToPitch = False
process.ctppsFastProtonSimulation.produceScoringPlaneHits = True
process.ctppsFastProtonSimulation.produceRecHits = False

process.ctppsFastProtonSimulationNoBeamSm = process.ctppsFastProtonSimulation.clone(
  hepMCTag = cms.InputTag("generator", "unsmeared")
)

process.ctppsFastProtonSimulationWithBeamSm = process.ctppsFastProtonSimulation.clone(
  hepMCTag = cms.InputTag('beamDivergenceVtxGenerator')
)

# distribution plotters
process.ctppsFastSimulationValidator = cms.EDAnalyzer("CTPPSFastSimulationValidator",
  simuTracksTag = cms.InputTag("ctppsFastProtonSimulationNoBeamSm"),
  recoTracksTag = cms.InputTag("ctppsFastProtonSimulationWithBeamSm"),
  outputFile = cms.string("output_simu_diff_beam_sm.root")
)

# processing path
process.p = cms.Path(
    process.generator

    * process.beamDivergenceVtxGenerator

    * process.ctppsFastProtonSimulationNoBeamSm
    * process.ctppsFastProtonSimulationWithBeamSm

    * process.ctppsFastSimulationValidator
)
