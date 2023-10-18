import FWCore.ParameterSet.Config as cms
from flashgg.MicroAOD.flashggJets_cfi import flashggBTag, flashggDeepCSV, flashggDeepJet, UnpackedJetCollectionVInputTag, maxJetCollections

bDiscriminator94X= cms.vdouble(0.1522,0.4941,0.8001)

flashggBPbH = cms.EDProducer("FlashggBPbHTagProducer"
                             , DiPhotonTag = cms.InputTag('flashggPreselectedDiPhotons')
                             , FinalJets = cms.InputTag("flashggFinalJets")
			     , MVAResultTag = cms.InputTag('flashggDiPhotonMVA')
                             , leadPhoOverMassThreshold = cms.double(0.33)
                             , subleadPhoOverMassThreshold = cms.double(0.25)
                             , PhoMVAThreshold = cms.double(-0.7)
			     , ElectronTag=cms.InputTag('flashggSelectedElectrons')
			     , MuonTag=cms.InputTag('flashggSelectedMuons')
			     , VertexTag=cms.InputTag('offlineSlimmedPrimaryVertices')                             
			     , MuonEtaCut = cms.double(2.4)
                             , MuonPtCut = cms.double(5)
                             , MuonIsoCut = cms.double(0.25)
                             , MuonPhotonDrCut = cms.double(0.4)
                             , EleEtaCuts = cms.vdouble(1.4442,1.566,2.4)
                             , ElePtCut = cms.double(10)
                             , ElePhotonDrCut = cms.double(0.4)
                             , ElePhotonZMassCut = cms.double(5)
                             , DeltaRTrkEle = cms.double(0.)
                             , debug = cms.bool(False)
                             , JetsCollSize = cms.uint32(maxJetCollections)
                             , inputTagJets= UnpackedJetCollectionVInputTag
                             , jetPtThreshold = cms.double(25.)
                             , jetEtaThreshold = cms.double(4.5)
                             , dRJetPhoLeadCut =  cms.double(0.4)
                             , dRJetPhoSubleadCut = cms.double(0.4)
                             , bTag = cms.string(flashggDeepCSV)
                             , bDiscriminator = bDiscriminator94X
) 
