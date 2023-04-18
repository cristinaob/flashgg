import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariables,minimalHistograms,minimalNonSignalVariables,systematicVariables
from flashgg.Systematics.SystematicDumperDefaultVariables import minimalVariablesHTXS,systematicVariablesHTXS
import sys, os
from flashgg.MetaData.MetaConditionsReader import *

n_events = -1

# SYSTEMATICS SECTION
process = cms.Process("FLASHggTag")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 1 )

# Input file
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring("file:myMicroAODOutputFile.root"))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(n_events))

from flashgg.MetaData.MetaConditionsReader import *

# set default options if needed
metaConditions = MetaConditionsReader("MetaData/data/MetaConditions/Era2018_RR-17Sep2018_v1.json")

ISDATA = False

### Global Tag
from Configuration.AlCa.GlobalTag import GlobalTag
if ISDATA:
    process.GlobalTag.globaltag = str(metaConditions['globalTags']['data'])
else:
    process.GlobalTag.globaltag = str(metaConditions['globalTags']['MC'])

from flashgg.Systematics.SystematicsCustomize import *
class customizer:
  def __init__(self, **kwargs):
    self.metaConditions = kwargs.get("metaConditions")
    self.processId = kwargs.get("processType")
    self.ignoreNegR9 = True

customize = customizer(metaConditions = metaConditions, processType = "data" if ISDATA else "mc")

# needed for 0th vertex from microAOD
process.load("flashgg.MicroAOD.flashggDiPhotons_cfi")
process.flashggDiPhotons.whichVertex = cms.uint32(0)
process.flashggDiPhotons.useZerothVertexFromMicro = cms.bool(True)

process.load("flashgg.Taggers.flashggTagSequence_cfi")
from flashgg.Taggers.flashggTagSequence_cfi import *
process.flashggTagSequence = flashggPrepareTagSequence(process, customize.metaConditions)

process.load("flashgg.Taggers.flashggTagTester_cfi")

# Remove unneeded tags
process.flashggTagSequence.remove(process.flashggTTHDiLeptonTag)
process.flashggTagSequence.remove(process.flashggVBFTag)
process.flashggTagSequence.remove(process.flashggVHMetTag)
process.flashggTagSequence.remove(process.flashggWHLeptonicTag)
process.flashggTagSequence.remove(process.flashggZHLeptonicTag)
process.flashggTagSequence.remove(process.flashggVHLeptonicLooseTag)
process.flashggTagSequence.remove(process.flashggVHHadronicTag)
process.flashggTagSequence.remove(process.flashggUntagged)
process.flashggTagSequence.remove(process.flashggVBFMVA)
process.flashggTagSequence.remove(process.flashggVBFDiPhoDiJetMVA)
process.flashggTagSequence.remove(process.flashggTTHLeptonicTag)
process.flashggTagSequence.remove(process.flashggTTHHadronicTag)
process.flashggTagSequence.remove(process.flashggVHhadMVA)
process.flashggTagSequence.remove(process.flashggGluGluHMVA)
process.flashggTagSequence.remove(process.flashggTHQLeptonicTag)

print process.flashggTagSequence

from flashgg.Taggers.flashggTagOutputCommands_cff import tagDefaultOutputCommand

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myTagOutputFile.root'),
                               outputCommands = tagDefaultOutputCommand			       
                               )

process.p = cms.Path(process.flashggTagSequence*process.flashggTagTester)

process.e = cms.EndPath(process.out)

