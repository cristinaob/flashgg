import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import os
from flashgg.MetaData.MetaConditionsReader import *

# ============================================================
# CMSSW Default Configuration
process = cms.Process("FLASHggBPbH")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('file:/eos/cms/store/group/phys_b2g/rdalialu/Bprime_M1000/v2/BprimeBToHB_Hgg_M-1000_TuneCP5_13TeV-madgraph-pythia8/Bprime_M1000-v2-v0-RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/231006_101357/0000/BprimeBToHB_Hgg_M1000_microAOD_1.root')
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("BprimeBtoBH_Hgg_M1000_1_test.root"))

from flashgg.MetaData.JobConfig import customize

customize.options.register('ignoreNegR9',
                           True,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'ignoreNegR9')

customize.setDefault("maxEvents",-1)
customize.parse()
customize.metaConditions = MetaConditionsReader(customize.metaConditions)

from Configuration.AlCa.GlobalTag import GlobalTag
if customize.processId == "Data": process.GlobalTag.globaltag = str(customize.metaConditions['globalTags']['data'])
else: process.GlobalTag.globaltag = str(customize.metaConditions['globalTags']['MC'])

process.load("flashgg.Taggers.flashggBPbHSequence_cfi")
from flashgg.Taggers.flashggBPbHSequence_cfi import *
process.flashggBPbHSequence = flashggPrepareBPbHSequence( process, customize.metaConditions )

from flashgg.Systematics.SystematicsCustomize import *
process.load("flashgg.Systematics.flashggDiPhotonSystematics_cfi")
import flashgg.Systematics.flashggDiPhotonSystematics_cfi as diPhotons_syst
diPhotons_syst.setupDiPhotonSystematics(process,customize)
process.flashggPreselectedDiPhotons.src = cms.InputTag('flashggDiPhotonSystematics')

useEGMTools(process)

from flashgg.Taggers.tagsDumpers_cfi import createTagDumper
process.load("flashgg.Taggers.BPbHDumper_cff")
process.BPbHDumper.className = "CutBasedBPbHTagDumper"
process.BPbHDumper.dumpTrees = True
process.BPbHDumper.dumpHistos = False
process.BPbHDumper.dumpWorkspace = False
process.BPbHDumper.src = "flashggBPbH"
process.BPbHDumper.processId = customize.processId
process.BPbHDumper.nameTemplate = cms.untracked.string("$PROCESS_$SQRTS_$CLASSNAME_$SUBCAT_$LABEL")

import flashgg.Taggers.dumperConfigTools as cfgTools
bpbh_variables = [
    "dipho_mass           := diPhoton.mass",
    "dipho_sumpt          := diPhoton.sumPt",
    "dipho_cosphi         := abs(cos(diPhoton.leadingPhoton.phi - diPhoton.subLeadingPhoton.phi))",
    "dipho_leadPt         := diPhoton.leadingPhoton.pt",
    "dipho_leadEta        := diPhoton.leadingPhoton.eta",
    "dipho_leadPhi        := diPhoton.leadingPhoton.phi",
    "dipho_leadE          := diPhoton.leadingPhoton.energy",
    "dipho_leadIDMVA      := diPhoton.leadingView.phoIdMvaWrtChosenVtx",
    "dipho_subleadPt      := diPhoton.subLeadingPhoton.pt",
    "dipho_subleadEta     := diPhoton.subLeadingPhoton.eta",
    "dipho_subleadPhi     := diPhoton.subLeadingPhoton.phi",
    "dipho_subleadE       := diPhoton.subLeadingPhoton.energy",
    "dipho_subleadIDMVA   := diPhoton.subLeadingView.phoIdMvaWrtChosenVtx"
]

cats = [
     ("BPbHTag","1",0)
]

cfgTools.addCategories(process.BPbHDumper,
                       cats,
                       variables = bpbh_variables,
                       histograms = []
)

process.p = cms.Path( process.flashggDifferentialPhoIdInputsCorrection
                     * process.flashggDiPhotonSystematics
                     * process.flashggUnpackedJets
                     * process.flashggBPbHSequence
                     * process.BPbHDumper
                     )
