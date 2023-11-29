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

# ============================================================
# Define number of events to process
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring('file:/eos/cms/store/group/phys_b2g/rdalialu/Bprime_M1000/v2/BprimeBToHB_Hgg_M-1000_TuneCP5_13TeV-madgraph-pythia8/Bprime_M1000-v2-v0-RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/231006_101357/0000/BprimeBToHB_Hgg_M1000_microAOD_1.root')
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("/afs/cern.ch/user/r/rdalialu/BprimeBtoBH_Hgg_M1000_1_test.root"))

from flashgg.MetaData.JobConfig import customize

customize.options.register('ignoreNegR9',
                           True,
                           VarParsing.VarParsing.multiplicity.singleton,
                           VarParsing.VarParsing.varType.bool,
                           'ignoreNegR9')

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
    "dipho_mass            := diPhoton.mass"
    ,"dipho_sumpt          := diPhoton.sumPt"
    ,"dipho_cosphi         := abs(cos(diPhoton.leadingPhoton.phi - diPhoton.subLeadingPhoton.phi))"
    ,"dipho_leadPt         := diPhoton.leadingPhoton.pt"
    ,"dipho_leadEta        := diPhoton.leadingPhoton.eta"
    ,"dipho_leadPhi        := diPhoton.leadingPhoton.phi"
    ,"dipho_leadE          := diPhoton.leadingPhoton.energy"
    ,"dipho_leadIDMVA      := diPhoton.leadingView.phoIdMvaWrtChosenVtx"
    ,"dipho_subleadPt      := diPhoton.subLeadingPhoton.pt"
    ,"dipho_subleadEta     := diPhoton.subLeadingPhoton.eta"
    ,"dipho_subleadPhi     := diPhoton.subLeadingPhoton.phi"
    ,"dipho_subleadE       := diPhoton.subLeadingPhoton.energy"
    ,"dipho_subleadIDMVA   := diPhoton.subLeadingView.phoIdMvaWrtChosenVtx"

    ,"dipho_mass_2         := getDiphoton_mass()"
    ,"diphoton_pt          := getDiphoton_pt()"
    ,"diphoton_eta         := getDiphoton_eta()"
    ,"diphoton_phi         := getDiphoton_phi()"
    ,"diphoton_mt          := getDiphoton_mt()"
    ,"diphoton_y           := getDiphoton_y()"

    ,"BPrime_mass          := getBP_mass()"
    ,"BPrime_pt            := getBP_pt()"
    ,"BPrime_eta           := getBP_eta()"
    ,"BPrime_phi           := getBP_phi()"
    ,"BPrime_mt            := getBP_mt()"
    ,"BPrime_y             := getBP_y()"

    ,"bjet_med_mass        := getbjetmed_mass()"
    ,"bjet_med_pt          := getbjetmed_pt()"
    ,"bjet_med_eta         := getbjetmed_eta()"
    ,"bjet_med_phi         := getbjetmed_phi()"
    ,"bjet_med_mt          := getbjetmed_mt()"
    ,"bjet_med_y           := getbjetmed_y()"

    ,"Fwdjet_mass          := getFwdjet_mass()"
    ,"Fwdjet_pt            := getFwdjet_pt()"
    ,"Fwdjet_eta           := getFwdjet_eta()"
    ,"Fwdjet_phi           := getFwdjet_phi()"
    ,"Fwdjet_mt            := getFwdjet_mt()"
    ,"Fwdjet_y             := getFwdjet_y()"

    ,"nbjets_loose         := getnBjets_loose()"
    ,"nbjets_med           := getnBjets_med()"
    ,"nbjets_tight         := getnBjets_tight()"
    ,"nFwdJets             := getnFwdJets()"

    ,"dR_bmedjet_diphoton  := getdRbmedjet_dipho()"
    ,"dEta_bmedjet_diphoton:= getdEtabmedjet_dipho()"
    ,"dPhi_bmedjet_diphoton:= getdPhibmedjet_dipho()"
    
    ,"dR_bmedjet_Fwdjet    := getdRbmedjet_Fwdjet()"
    ,"dEta_bmedjet_Fwdjet  := getdEtabmedjet_Fwdjet()"
    ,"dPhi_bmedjet_Fwdjet  := getdPhibmedjet_Fwdjet()"

    ,"dR_Fwdjet_dipho      := getdRFwdjet_dipho()"
    ,"dEta_Fwdjet_dipho    := getdEtaFwdjet_dipho()"
    ,"dPhi_Fwdjet_dipho    := getdPhiFwdjet_dipho()"

    ,"Ht                   := getHt()"
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
