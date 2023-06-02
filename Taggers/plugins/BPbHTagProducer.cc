#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

#include "flashgg/DataFormats/interface/BPbHTag.h"
#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/DiPhotonMVAResult.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Met.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "flashgg/Taggers/interface/LeptonSelection2018.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <algorithm>
#include <string>
#include <vector>
#include <utility>

#include "TMath.h"
#include "TMVA/Reader.h"

using namespace std;
using namespace edm;

// ----------

namespace flashgg {

    // ----------

    class BPbHTagProducer : public EDProducer {

        public:
            BPbHTagProducer(const ParameterSet &);
        private:
            void produce(Event &, const EventSetup &) override;

            EDGetTokenT<View<DiPhotonCandidate>> diPhotonToken_;
            EDGetTokenT<View<DiPhotonMVAResult>> mvaResultToken_;

	    EDGetTokenT<View<flashgg::Muon>> muonToken_;
	    EDGetTokenT<View<Electron>> electronToken_;
	    EDGetTokenT<View<reco::Vertex> > vertexToken_;

            double leadPhoOverMassThreshold_;
            double subleadPhoOverMassThreshold_;
            double PhoMVAThreshold_;

            //leptons
            
            double MuonEtaCut_;
            double MuonPtCut_;
            double MuonIsoCut_;
            double MuonPhotonDrCut_;
            double ElePtCut_;
            std::vector<double> EleEtaCuts_;
            double ElePhotonDrCut_;
            double ElePhotonZMassCut_;
            double DeltaRTrkEle_;
            bool   debug_;

    }; // closing 'class BPbHTagProducer'

    // ----------

    BPbHTagProducer::BPbHTagProducer(const ParameterSet &iConfig) :
        diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate>>(iConfig.getParameter<InputTag> ("DiPhotonTag")))
       ,mvaResultToken_(consumes<View<flashgg::DiPhotonMVAResult>>(iConfig.getParameter<InputTag> ("MVAResultTag"))) 
       ,muonToken_(consumes<View<flashgg::Muon> >(iConfig.getParameter<InputTag>("MuonTag")))
       ,electronToken_(consumes<View<flashgg::Electron> >(iConfig.getParameter<InputTag>("ElectronTag")))
       ,vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ) 
{

        leadPhoOverMassThreshold_ = iConfig.getParameter<double>("leadPhoOverMassThreshold");
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>("subleadPhoOverMassThreshold");
        PhoMVAThreshold_ = iConfig.getParameter<double>("PhoMVAThreshold");

	MuonEtaCut_ = iConfig.getParameter<double>( "MuonEtaCut");
        MuonPtCut_ = iConfig.getParameter<double>( "MuonPtCut");
        MuonIsoCut_ = iConfig.getParameter<double>( "MuonIsoCut");
        MuonPhotonDrCut_ = iConfig.getParameter<double>( "MuonPhotonDrCut");
 
        EleEtaCuts_ = iConfig.getParameter<std::vector<double>>( "EleEtaCuts");
        ElePtCut_ = iConfig.getParameter<double>( "ElePtCut");
        ElePhotonDrCut_ = iConfig.getParameter<double>( "ElePhotonDrCut");
        ElePhotonZMassCut_ = iConfig.getParameter<double>( "ElePhotonZMassCut");
        DeltaRTrkEle_ = iConfig.getParameter<double>( "DeltaRTrkEle");

        debug_ = iConfig.getParameter<bool>( "debug" );

        produces<vector<BPbHTag>>();

    } // closing 'BPbHTagProducer::BPbHTagProducer'

    // ----------

    void BPbHTagProducer::produce(Event &evt, const EventSetup &) {

        // Getting object information
        Handle<View<flashgg::DiPhotonCandidate>> diPhotons;
        evt.getByToken(diPhotonToken_, diPhotons);

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken(mvaResultToken_, mvaResults);

        Handle<View<flashgg::Muon> > theMuons;
	evt.getByToken(muonToken_, theMuons );

	Handle<View<flashgg::Electron> > theElectrons;
        evt.getByToken(electronToken_, theElectrons );

	Handle<View<reco::Vertex> > vertices;
	evt.getByToken( vertexToken_, vertices );

        // Declaring and initializing variables
        double idmva1 = 0.0;
        double idmva2 = 0.0;

        std::unique_ptr<vector<BPbHTag> > bpbhtags( new vector<BPbHTag> );

        std::cout << "=======> New Event with " << diPhotons->size() << " diphoton candidates" << std::endl; 
        //std::cout << "leading Pho over Mass Threshold " << leadPhoOverMassThreshold_  << std::endl;

        // Loop over diphoton candidates
        for (unsigned int diphoIndex=0; diphoIndex < diPhotons->size(); diphoIndex++) {

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt(diphoIndex);
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt(diphoIndex);

            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx(dipho->vtx());
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx(dipho->vtx());
            
            // std::cout << "diphoIndex " << diphoIndex << " leading pT: " << dipho->leadingPhoton()->pt() << std::endl;
            // std::cout << "leading g Pt " << dipho->leadingPhoton()->pt() << std::endl;
            // std::cout << "subLeading g Pt " << dipho->subLeadingPhoton()->pt() << std::endl;

            // Cut 1: diphoton selection:
            // - pT cuts on leading and subleading photons
            // - cut on photon ID MVA
            if (dipho->leadingPhoton()->pt() < (dipho->mass())*leadPhoOverMassThreshold_) { continue; }
            // std::cout << "dipho_mass * leading Pho over Mass Threshold" << (dipho->mass())*leadPhoOverMassThreshold_ << std::endl; 
            if (dipho->subLeadingPhoton()->pt() < (dipho->mass())*subleadPhoOverMassThreshold_) { continue; }
            if (idmva1 < PhoMVAThreshold_ || idmva2 < PhoMVAThreshold_) { continue; }

            // Cut 2: lepton veto
            std::vector<edm::Ptr<flashgg::Muon>> Muons;
            std::vector<edm::Ptr<flashgg::Electron>> Electrons;

	    if (theMuons->size()>0)
	    	Muons = selectMuons(theMuons->ptrs(), dipho, vertices->ptrs(), MuonPtCut_, MuonEtaCut_, MuonIsoCut_, MuonPhotonDrCut_, debug_);

	    if (theElectrons->size()>0)
                Electrons = selectElectrons(theElectrons->ptrs(), dipho, ElePtCut_, EleEtaCuts_, ElePhotonDrCut_, ElePhotonZMassCut_, DeltaRTrkEle_, debug_);

	    std::cout <<" Number of leptons " << (Muons.size() + Electrons.size()) << std::endl;

	    if ( (Muons.size() + Electrons.size()) != 0 ) continue;

	

            std::cout << "Passed cuts!" << std::endl;

            BPbHTag bpbhtags_obj(dipho,mvares);
            bpbhtags_obj.includeWeights(*dipho);
            bpbhtags_obj.setDiPhotonIndex(diphoIndex);

            bpbhtags->push_back(bpbhtags_obj);

        } // closing loop over diPhotons

        evt.put( std::move(bpbhtags) );

    } // closing 'BPbHTagProducer::produce'
    

} // closing 'namespace flashgg'

// ----------

typedef flashgg::BPbHTagProducer FlashggBPbHTagProducer;
DEFINE_FWK_MODULE(FlashggBPbHTagProducer);

