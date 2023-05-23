import FWCore.ParameterSet.Config as cms

flashggBPbH = cms.EDProducer("FlashggBPbHTagProducer",
                             DiPhotonTag = cms.InputTag('flashggPreselectedDiPhotons'),
                             MVAResultTag = cms.InputTag('flashggDiPhotonMVA'),
                             leadPhoOverMassThreshold = cms.double(0.33),
                             subleadPhoOverMassThreshold = cms.double(0.25),
                             PhoMVAThreshold = cms.double(-0.7)
                             ) 
