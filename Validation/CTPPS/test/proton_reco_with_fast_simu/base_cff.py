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
    input = cms.untracked.int32(10000)
)

outputDir = "1E4"

# particle-data table
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# particle generator
process.generator = cms.EDProducer("RandomXiThetaGunProducer",
  particleId = cms.uint32(2212),

  energy = cms.double(6500),  # nominal beam energy, GeV
  xi_min = cms.double(0.),
  xi_max = cms.double(0.20),
  theta_x_mean = cms.double(0),
  theta_x_sigma = cms.double(100E-6), # in rad
  theta_y_mean = cms.double(0),
  theta_y_sigma = cms.double(100E-6),

  nParticlesSector45 = cms.uint32(1),
  nParticlesSector56 = cms.uint32(1),
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
    geomXMLFiles = totemGeomXMLFiles+ctppsDiamondGeomXMLFiles+["Validation/CTPPS/test/rp_positions/data/2016_preTS2_without_margin_end/RP_Dist_Beam_Cent.xml"],
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
process.ctppsFastProtonSimulation.hepMCTag = cms.InputTag('beamDivergenceVtxGenerator')
process.ctppsFastProtonSimulation.checkApertures = False
process.ctppsFastProtonSimulation.roundToPitch = True
process.ctppsFastProtonSimulation.pitch = 66E-3 * 12 / 19 # effective value to reproduce real RP resolution
process.ctppsFastProtonSimulation.produceHitsRelativeToBeam = True
process.ctppsFastProtonSimulation.produceScoringPlaneHits = False
process.ctppsFastProtonSimulation.produceRecHits = True

# strips reco: pattern recognition
process.load('RecoCTPPS.TotemRPLocal.totemRPUVPatternFinder_cfi')
process.totemRPUVPatternFinder.tagRecHit = cms.InputTag('ctppsFastProtonSimulation')

# strips reco: track fitting
process.load('RecoCTPPS.TotemRPLocal.totemRPLocalTrackFitter_cfi')

# common reco: lite track production
process.load('RecoCTPPS.TotemRPLocal.ctppsLocalTrackLiteProducer_cfi')

# proton reconstruction
process.load("RecoCTPPS.ProtonReconstruction.ctppsProtonReconstruction_cfi")
process.ctppsProtonReconstruction.tagLocalTrackLite = cms.InputTag('ctppsLocalTrackLiteProducer')

# reconstruction validation
process.load("Validation.CTPPS.ctppsProtonReconstructionValidator_cfi")
process.ctppsProtonReconstructionValidator.tagHepMCBeforeSmearing = cms.InputTag("generator", "unsmeared")
process.ctppsProtonReconstructionValidator.tagHepMCAfterSmearing = cms.InputTag("beamDivergenceVtxGenerator")
process.ctppsProtonReconstructionValidator.tagRecoProtons = cms.InputTag("ctppsProtonReconstruction")

# processing path
process.p = cms.Path(
    process.generator
    * process.beamDivergenceVtxGenerator
    * process.ctppsFastProtonSimulation

    * process.totemRPUVPatternFinder
    * process.totemRPLocalTrackFitter
    * process.ctppsLocalTrackLiteProducer
    * process.ctppsProtonReconstruction

    * process.ctppsProtonReconstructionValidator
)

# customisation functions

def SetLevel1():
  process.beamDivergenceVtxGenerator.vertexSigmaX = 0E-4
  process.beamDivergenceVtxGenerator.vertexSigmaZ = 0

  process.beamDivergenceVtxGenerator.beamDivergenceX = 0E-6
  process.beamDivergenceVtxGenerator.beamDivergenceY = 0E-6

  process.ctppsFastProtonSimulation.roundToPitch = False


def SetLevel2():
  process.beamDivergenceVtxGenerator.beamDivergenceX = 0E-6
  process.beamDivergenceVtxGenerator.beamDivergenceY = 0E-6

  process.ctppsFastProtonSimulation.roundToPitch = False


def SetLevel3():
  process.ctppsFastProtonSimulation.roundToPitch = False


def SetLevel4():
  pass

def SetLowTheta():
  process.generator.theta_x_sigma = 0E-6
  process.generator.theta_y_sigma = 0E-6


def SetLargeTheta():
  pass
