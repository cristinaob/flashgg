import FWCore.ParameterSet.Config as cms

from flashgg.Taggers.tagsDumpConfig_cff import tagsDumpConfig

BPbHDumper = cms.EDAnalyzer('CutBasedBPbHTagDumper',
           **tagsDumpConfig.parameters_())
