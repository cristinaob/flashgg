import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggJets_cfi import flashggUnpackedJets
from flashgg.Taggers.flashggDiPhotonMVA_cfi import flashggDiPhotonMVA
from flashgg.Taggers.flashggBPbH_cff import *
from flashgg.Taggers.flashggPreselectedDiPhotons_cfi import flashggPreselectedDiPhotons
from flashgg.Taggers.flashggDifferentialPhoIdInputsCorrection_cfi import flashggDifferentialPhoIdInputsCorrection, setup_flashggDifferentialPhoIdInputsCorrection
from flashgg.Taggers.flashggPrefireDiPhotons_cff import flashggPrefireDiPhotons


def flashggPrepareBPbHSequence(process, options):
    setup_flashggDifferentialPhoIdInputsCorrection(process, options)
    flashggPreselectedDiPhotons.src = "flashggPrefireDiPhotons"

    if "flashggDiPhotonMVA" in options:
        flashggDiPhotonMVA.diphotonMVAweightfile = cms.FileInPath(str(options["flashggDiPhotonMVA"]["weightFile"]))
        flashggDiPhotonMVA.Version = cms.string(str(options["flashggDiPhotonMVA"]["version"]))

    flashggTagSequence = cms.Sequence(#flashggDifferentialPhoIdInputsCorrection
                                      #* flashggUnpackedJets
                                      #flashggPrefireDiPhotons
                                      flashggPreselectedDiPhotons
                                      * flashggDiPhotonMVA
                                      #* flashggUnpackedJets
                                      * flashggBPbH
                                      #* flashggTagSorter
                                  )

    return flashggTagSequence
