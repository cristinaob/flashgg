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

#include "TLorentzVector.h"

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

            float totalMediumBJetMass = 0.0;   

        private:
            void produce(Event &, const EventSetup &) override;

            EDGetTokenT<View<DiPhotonCandidate>> diPhotonToken_;
            EDGetTokenT<View<DiPhotonMVAResult>> mvaResultToken_;

	    EDGetTokenT<View<flashgg::Muon>> muonToken_;
	    EDGetTokenT<View<Electron>> electronToken_;
	    EDGetTokenT<View<reco::Vertex> > vertexToken_;

            typedef std::vector<edm::Handle<edm::View<flashgg::Jet> > > JetCollectionVector;
            std::vector<edm::InputTag> inputTagJets_;
            std::vector<edm::EDGetTokenT<View<flashgg::Jet> > > tokenJets_;
            EDGetTokenT<View<reco::GenJet> > genJetToken_;

            // photons
            double leadPhoOverMassThreshold_;
            double subleadPhoOverMassThreshold_;
            double PhoMVAThreshold_;

            // leptons            
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

            // jets
            unsigned int inputJetsCollSize_;
            double jetEtaThreshold_;
            double jetPtThreshold_;
            double dRJetPhoLeadCut_;
            double dRJetPhoSubleadCut_;

            vector<double> bDiscriminator_;
            string bTag_;

            // others
            bool debugMode;

    }; // closing 'class BPbHTagProducer'

    // ----------

    BPbHTagProducer::BPbHTagProducer(const ParameterSet &iConfig) :
        diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate>>(iConfig.getParameter<InputTag> ("DiPhotonTag")))
       ,mvaResultToken_(consumes<View<flashgg::DiPhotonMVAResult>>(iConfig.getParameter<InputTag> ("MVAResultTag"))) 
       ,muonToken_(consumes<View<flashgg::Muon> >(iConfig.getParameter<InputTag>("MuonTag")))
       ,electronToken_(consumes<View<flashgg::Electron> >(iConfig.getParameter<InputTag>("ElectronTag")))
       ,vertexToken_( consumes<View<reco::Vertex> >( iConfig.getParameter<InputTag> ( "VertexTag" ) ) ) 
       ,inputTagJets_( iConfig.getParameter<std::vector<edm::InputTag> >( "inputTagJets" ) )
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

        jetPtThreshold_ = iConfig.getParameter<double>( "jetPtThreshold");
        jetEtaThreshold_ = iConfig.getParameter<double>( "jetEtaThreshold");
        dRJetPhoLeadCut_ = iConfig.getParameter<double>( "dRJetPhoLeadCut");
        dRJetPhoSubleadCut_ = iConfig.getParameter<double>( "dRJetPhoSubleadCut");

        bDiscriminator_ = iConfig.getParameter<vector<double > >( "bDiscriminator" );
        bTag_ = iConfig.getParameter<string>( "bTag" );

        debug_ = iConfig.getParameter<bool>( "debug" );

        debugMode = false;

        inputJetsCollSize_= iConfig.getParameter<unsigned int> ( "JetsCollSize" );

        std::vector<std::vector<edm::InputTag>>  jetTags;
        jetTags.push_back(std::vector<edm::InputTag>(0));
        for (unsigned int i = 0; i < inputJetsCollSize_ ; i++) {
          jetTags[0].push_back(inputTagJets_[i]);  // nominal jets
        }

        for (unsigned i = 0 ; i < inputTagJets_.size() ; i++) {
          auto token = consumes<View<flashgg::Jet> >(inputTagJets_[i]);
          tokenJets_.push_back(token);
        }

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

        JetCollectionVector Jets(inputJetsCollSize_);
        for( size_t j = 0; j < inputTagJets_.size(); ++j ) {
          evt.getByToken( tokenJets_[j], Jets[j] );
        }

        TTree* tree = new TTree("JetTree", "Tree with jet information");
        tree->Branch("jetMass", &totalMediumBJetMass, "jetMass/F" );

        // Declaring and initializing variables
        double idmva1 = 0.0;
        double idmva2 = 0.0;
        
        std::unique_ptr<vector<BPbHTag> > bpbhtags( new vector<BPbHTag> );

        if (debugMode) std::cout << "=======> New Event" << std::endl;

        if (debugMode) {
          std::cout << "Event with " << diPhotons->size() << " diphoton candidates" << std::endl; 
          std::cout << "leading Pho over Mass Threshold " << leadPhoOverMassThreshold_  << std::endl;
        }

        // Loop over diphoton candidates
        for (unsigned int diphoIndex=0; diphoIndex < diPhotons->size(); diphoIndex++) {

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt(diphoIndex);
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt(diphoIndex);

            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx(dipho->vtx());
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx(dipho->vtx()); 

            double diphotonMass    = dipho->mass();
            double diphotonPt      = dipho->pt();
            double diphotonSysPhi  = dipho->phi();
            double diphotonEta     = dipho->eta();

            if (debugMode) {   
              std::cout << "diphoIndex " << diphoIndex << " leading pT: " << dipho->leadingPhoton()->pt() << std::endl;
              std::cout << "leading g Pt " << dipho->leadingPhoton()->pt() << std::endl;
              std::cout << "subLeading g Pt " << dipho->subLeadingPhoton()->pt() << std::endl;
              std::cout << "DPhi of Diphoton System = " << diphotonSysPhi << std::endl;
              std::cout << "Mass of the diphoton system " << diphotonMass  << std::endl;            
            }

            // ------------------------------
            // Cut 1: diphoton selection
            // - pT cuts on leading and subleading photons
            // - cut on photon ID MVA
            if (dipho->leadingPhoton()->pt() < (dipho->mass())*leadPhoOverMassThreshold_) { continue; }
            if (debugMode) std::cout << "dipho_mass * leading Pho over Mass Threshold" << (dipho->mass())*leadPhoOverMassThreshold_ << std::endl; 
            if (dipho->subLeadingPhoton()->pt() < (dipho->mass())*subleadPhoOverMassThreshold_) { continue; }
            if (idmva1 < PhoMVAThreshold_ || idmva2 < PhoMVAThreshold_) { continue; }

            //if ( diphotonSysPhi > M_PI ){
            //     diphotonSysPhi -= 2 * M_PI;
            //} else if ( diphotonSysPhi < -M_PI ){
            //    diphotonSysPhi += 2 * M_PI;
            //} 

            TLorentzVector diphotonVector;
            diphotonVector.SetPtEtaPhiM(diphotonPt, diphotonEta, diphotonSysPhi, diphotonMass);
            
            // ------------------------------
            // Cut 2: lepton veto
            std::vector<edm::Ptr<flashgg::Muon>> Muons;
            std::vector<edm::Ptr<flashgg::Electron>> Electrons;

	    if (theMuons->size()>0)
	      Muons = selectMuons(theMuons->ptrs(), dipho, vertices->ptrs(), MuonPtCut_, MuonEtaCut_, MuonIsoCut_, MuonPhotonDrCut_, debug_);

	    if (theElectrons->size()>0)
              Electrons = selectElectrons(theElectrons->ptrs(), dipho, ElePtCut_, EleEtaCuts_, ElePhotonDrCut_, ElePhotonZMassCut_, DeltaRTrkEle_, debug_);

	    if (debugMode) std::cout <<" Number of leptons " << (Muons.size() + Electrons.size()) << std::endl;

	    if ( (Muons.size() + Electrons.size()) != 0 ) continue;	

            // ------------------------------
            // Cut 3: jets
            std::vector<TLorentzVector> mediumBJetVectors;
            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();            
         
            if (debugMode) std::cout << "jetCollectionIndex: " << jetCollectionIndex << std::endl;
          
            int nGoodJets        = 0;
            int nFwdJets         = 0;
            int nbjets_loose     = 0;
            int nbjets_medium    = 0;
            int nbjets_tight     = 0;
         
            for ( unsigned int jetIndex = 0; jetIndex < Jets[jetCollectionIndex]->size() ; jetIndex++ ) {

                edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( jetIndex );
                

                if( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }
                if(!thejet->passesJetID  ( flashgg::Tight2017 ) ) { continue; }
                if ( thejet->pt() < jetPtThreshold_ ) { continue; }

                float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),
                                              dipho->subLeadingPhoton()->superCluster()->phi() );
                if( dRPhoLeadJet < dRJetPhoLeadCut_ || dRPhoSubLeadJet < dRJetPhoSubleadCut_ ) { continue; }
                if( thejet->pt() < jetPtThreshold_ ) { continue; }
                
                nGoodJets++;
	      
                float bDiscriminatorValue;
                // jets centrales eta < 2.5
                if ( fabs(thejet->eta()) < 2.5) {
                    if (bTag_ == "pfDeepCSV") {
                        bDiscriminatorValue = thejet->bDiscriminator("pfDeepCSVJetTags:probb") + thejet->bDiscriminator("pfDeepCSVJetTags:probbb");
                    } else {
                        bDiscriminatorValue = thejet->bDiscriminator( bTag_ );
                    }

                    if ( bDiscriminatorValue > bDiscriminator_[0] ) {
                        nbjets_loose++;
                    }

                    if( bDiscriminatorValue > bDiscriminator_[1] ) {
                        nbjets_medium++;
                        TLorentzVector bJetMediumVector;
                        bJetMediumVector.SetPtEtaPhiE(thejet->pt(), thejet->eta(), thejet->phi(), thejet->energy());
                        mediumBJetVectors.push_back(bJetMediumVector);
                    }

                    if( bDiscriminatorValue > bDiscriminator_[2] ) {
                        nbjets_tight++;
                    }
                } else {//candidatos a Fwd jets eta > 2.5
                    nFwdJets++;
                }    
           
            }

            if ( nGoodJets < 2 ) { continue; }
            if ( nFwdJets  < 1 ) { continue; }
            if ( mediumBJetVectors.size() < 1) { continue; }
            // Continuar aqui con bjet mass

            //for (const TLorentzVector &bJetVector : mediumBJetVectors) {
            //    totalMediumBJetMass = bJetVector.M();
            //std::cout << "Bjet mass: " << mediumBJetVectors[0].M() << std::endl;
            //    tree->Fill();
            //}
            TLorentzVector Bprime = mediumBJetVectors[0] +  diphotonVector;
            std::cout << "Bprime mass = " << Bprime.M() << std::endl; 
            std::cout << "DPhi bjet diPho = " << diphotonVector.DeltaPhi(mediumBJetVectors[0]) << std::endl;

            if (debugMode) {
              std::cout << "Found " << nGoodJets << " good jets, out of " << Jets[jetCollectionIndex]->size() << "." << std::endl;
              std::cout << "loose bjets: " << nbjets_loose << std::endl;
              std::cout << "medium bjets: " << nbjets_medium << std::endl;
              std::cout << "tight bjets: " << nbjets_tight << std::endl;
              std::cout << "N Fwd Jets: " << nFwdJets << std::endl;  
            }

            if (debugMode) std::cout << "Passed cuts!" << std::endl;

            // dejar esto al final
            BPbHTag bpbhtags_obj(dipho,mvares);
            bpbhtags_obj.includeWeights(*dipho);
            bpbhtags_obj.setDiPhotonIndex(diphoIndex);

            bpbhtags->push_back(bpbhtags_obj);
            

        } // closing loop over diPhotons
        tree->Write();
        evt.put( std::move(bpbhtags) );

    } // closing 'BPbHTagProducer::producer


} // closing 'namespace flashgg'

// ----------

typedef flashgg::BPbHTagProducer FlashggBPbHTagProducer;
DEFINE_FWK_MODULE(FlashggBPbHTagProducer);

