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
            float  diphoton_Mass_;
            float  diphoton_Pt_;
            float  diphoton_Eta_;
            float  diphoton_Phi_;

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

            int nbjets_loose_;
            int nbjets_med_;
            int nbjets_tight_;
            int nFwdJets_;

            // others
            bool debugMode;

            // BP candidate
            float BP_mass_;
            float BP_pt_;
            float BP_eta_;
            float BP_phi_;

            // bjet_med
            float bjet_med_mass_;
            float bjet_med_pt_;
            float bjet_med_eta_;
            float bjet_med_phi_;

            // Fwd jet
            float Fwdjet_mass_;
            float Fwdjet_pt_;
            float Fwdjet_eta_;
            float Fwdjet_phi_;

            // dR s
            float dR_bmedjet_dipho_;
            float dR_bmedjet_Fwdjet_;
            float dR_Fwdjet_dipho_;

            // dEta s
            float dEta_bmedjet_dipho_;
            float dEta_bmedjet_Fwdjet_;
            float dEta_Fwdjet_dipho_;

            // dPhi s
            float dPhi_bmedjet_dipho_;
            float dPhi_bmedjet_Fwdjet_;
            float dPhi_Fwdjet_dipho_;

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

        // getting object information
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

        // declaring and initializing variables
        double idmva1 = 0.0;
        double idmva2 = 0.0;
        
        std::unique_ptr<vector<BPbHTag> > bpbhtags( new vector<BPbHTag> );

        if (debugMode) std::cout << "=======> New Event" << std::endl;

        if (debugMode) {
            std::cout << "Event with " << diPhotons->size() << " diphoton candidates" << std::endl; 
        }

        // loop over diphoton candidates
        for (unsigned int diphoIndex=0; diphoIndex < diPhotons->size(); diphoIndex++) {

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt(diphoIndex);
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt(diphoIndex);

            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx(dipho->vtx());
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx(dipho->vtx()); 

            // ------------------------------
            // Cut 1: diphoton selection
            // - pT cuts on leading and subleading photons
            // - cut on photon ID MVA
            if (dipho->leadingPhoton()->pt() < (dipho->mass())*leadPhoOverMassThreshold_) { continue; }
            if (dipho->subLeadingPhoton()->pt() < (dipho->mass())*subleadPhoOverMassThreshold_) { continue; }
            if (idmva1 < PhoMVAThreshold_ || idmva2 < PhoMVAThreshold_) { continue; }

            // ------------------------------
            // Cut 2: lepton veto
            std::vector<edm::Ptr<flashgg::Muon>> Muons;
            std::vector<edm::Ptr<flashgg::Electron>> Electrons;

	    if (theMuons->size()>0)
	        Muons = selectMuons(theMuons->ptrs(), dipho, vertices->ptrs(), MuonPtCut_, MuonEtaCut_, MuonIsoCut_, MuonPhotonDrCut_, debug_);

	    if (theElectrons->size()>0)
                Electrons = selectElectrons(theElectrons->ptrs(), dipho, ElePtCut_, EleEtaCuts_, ElePhotonDrCut_, ElePhotonZMassCut_, DeltaRTrkEle_, debug_);

	    if ( (Muons.size() + Electrons.size()) != 0 ) continue;	

            // ------------------------------
            // loop over jets
            std::vector<TLorentzVector> looseBJetVectors;
            std::vector<TLorentzVector> mediumBJetVectors;
            std::vector<TLorentzVector> tightBJetVectors;
            std::vector<TLorentzVector> FwdJetVectors;

            unsigned int jetCollectionIndex = diPhotons->ptrAt( diphoIndex )->jetCollectionIndex();            
         
            int nGoodJets = 0;
            int nFwdJets = 0;
            int nbjets_loose = 0;
            int nbjets_medium = 0;
            int nbjets_tight = 0;
         
            for ( unsigned int jetIndex = 0; jetIndex < Jets[jetCollectionIndex]->size() ; jetIndex++ ) {

                edm::Ptr<flashgg::Jet> thejet = Jets[jetCollectionIndex]->ptrAt( jetIndex );
                
                if ( fabs( thejet->eta() ) > jetEtaThreshold_ ) { continue; }
                if (!thejet->passesJetID ( flashgg::Tight2017 ) ) { continue; }
                if ( thejet->pt() < jetPtThreshold_ ) { continue; }

                float dRPhoLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->leadingPhoton()->superCluster()->eta(), dipho->leadingPhoton()->superCluster()->phi() ) ;
                float dRPhoSubLeadJet = deltaR( thejet->eta(), thejet->phi(), dipho->subLeadingPhoton()->superCluster()->eta(),
                                                dipho->subLeadingPhoton()->superCluster()->phi() );
                if( dRPhoLeadJet < dRJetPhoLeadCut_ || dRPhoSubLeadJet < dRJetPhoSubleadCut_ ) { continue; }
                if( thejet->pt() < jetPtThreshold_ ) { continue; }
                
                nGoodJets++;
	      
                float bDiscriminatorValue;
                if ( fabs(thejet->eta()) < 2.5) {
                    // central jets
                    if (bTag_ == "pfDeepCSV") {
                        bDiscriminatorValue = thejet->bDiscriminator("pfDeepCSVJetTags:probb") + thejet->bDiscriminator("pfDeepCSVJetTags:probbb");
                    } else {
                        bDiscriminatorValue = thejet->bDiscriminator( bTag_ );
                    }

                    if ( bDiscriminatorValue > bDiscriminator_[0] ) {
                        nbjets_loose++;
                        TLorentzVector bJetLooseVector;
                        bJetLooseVector.SetPtEtaPhiE(thejet->pt(), thejet->eta(), thejet->phi(), thejet->energy());
                        looseBJetVectors.push_back(bJetLooseVector);
                    }

                    if( bDiscriminatorValue > bDiscriminator_[1] ) {
                        nbjets_medium++;
                        TLorentzVector bJetMediumVector;
                        bJetMediumVector.SetPtEtaPhiE(thejet->pt(), thejet->eta(), thejet->phi(), thejet->energy());
                        mediumBJetVectors.push_back(bJetMediumVector);
                    }

                    if( bDiscriminatorValue > bDiscriminator_[2] ) {
                        nbjets_tight++;
                        TLorentzVector bJetTightVector;
                        bJetTightVector.SetPtEtaPhiE(thejet->pt(), thejet->eta(), thejet->phi(), thejet->energy());
                        tightBJetVectors.push_back(bJetTightVector);
                    }
                } else {
                    // forward jets
                    nFwdJets++;
                    TLorentzVector FwdJetVector;
                    FwdJetVector.SetPtEtaPhiE(thejet->pt(), thejet->eta(), thejet->phi(), thejet->energy());
                    FwdJetVectors.push_back(FwdJetVector);
                }    
           
            }

            // ------------------------------
            // Cut 3: At least 2 good jets
            if ( nGoodJets < 2 ) { continue; }

            // ------------------------------
            // Cut 4: At least 1 forward jet
            if ( nFwdJets  < 1 ) { continue; }

            // ------------------------------
            // Cut 5: At least 1 medium b-tagged jet
            if ( mediumBJetVectors.size() < 1) { continue; }

            // ------------------------------
            // working after cuts...

            double diphotonMass = dipho->mass();
            double diphotonPt = dipho->pt();
            double diphotonPhi = dipho->phi();
            double diphotonEta = dipho->eta();

            TLorentzVector diphotonVector;
            diphotonVector.SetPtEtaPhiM(diphotonPt, diphotonEta, diphotonPhi, diphotonMass);

            diphoton_Mass_ = diphotonVector.M();
            diphoton_Pt_   = diphotonVector.Pt();
            diphoton_Eta_  = diphotonVector.Eta();
            diphoton_Phi_  = diphotonVector.Phi();

            TLorentzVector Bprime = mediumBJetVectors[0] +  diphotonVector;
            BP_mass_ = Bprime.M();
            BP_pt_   = Bprime.Pt();
            BP_eta_  = Bprime.Eta();
            BP_phi_  = Bprime.Phi();

            bjet_med_mass_ = mediumBJetVectors[0].M();
            bjet_med_pt_   = mediumBJetVectors[0].Pt();
            bjet_med_eta_  = mediumBJetVectors[0].Eta();
            bjet_med_phi_  = mediumBJetVectors[0].Phi();

            Fwdjet_mass_   = FwdJetVectors[0].M();
            Fwdjet_pt_     = FwdJetVectors[0].Pt();
            Fwdjet_eta_    = FwdJetVectors[0].Eta();
            Fwdjet_phi_    = FwdJetVectors[0].Phi();

            nbjets_loose_  = nbjets_loose;
            nbjets_med_    = nbjets_medium;
            nbjets_tight_  = nbjets_tight;
            nFwdJets_      = nFwdJets;

            dR_bmedjet_dipho_   = mediumBJetVectors[0].DeltaR(diphotonVector);
            dPhi_bmedjet_dipho_ = mediumBJetVectors[0].DeltaPhi(diphotonVector);
            dEta_bmedjet_dipho_ = mediumBJetVectors[0].Eta() - diphoton_Eta_;

            dR_bmedjet_Fwdjet_  = mediumBJetVectors[0].DeltaR(FwdJetVectors[0]);
            dPhi_bmedjet_Fwdjet_= mediumBJetVectors[0].DeltaPhi(FwdJetVectors[0]);
            dEta_bmedjet_Fwdjet_= mediumBJetVectors[0].Eta() - FwdJetVectors[0].Eta();

            dR_Fwdjet_dipho_    = FwdJetVectors[0].DeltaR(diphotonVector);
            dPhi_Fwdjet_dipho_  = FwdJetVectors[0].DeltaPhi(diphotonVector);
            dEta_Fwdjet_dipho_  = FwdJetVectors[0].Eta() - diphoton_Eta_;

            if (debugMode) {
                std::cout << "Bprime mass = " << Bprime.M() << std::endl; 
                std::cout << "Bprime Pt = " << Bprime.Pt() << std::endl;
                std::cout << "DPhi bjet diPho = " << diphotonVector.DeltaPhi(mediumBJetVectors[0]) << std::endl;
                std::cout << "Found " << nGoodJets << " good jets, out of " << Jets[jetCollectionIndex]->size() << "." << std::endl;
                std::cout << "loose bjets: " << nbjets_loose << std::endl;
                std::cout << "medium bjets: " << nbjets_medium << std::endl;
                std::cout << "tight bjets: " << nbjets_tight << std::endl;
                std::cout << "N Fwd Jets: " << nFwdJets << std::endl;  
            }

            if (debugMode) std::cout << "Passed cuts!" << std::endl;

            // ------------------------------
            // write objects to ntuple
            BPbHTag bpbhtags_obj(dipho,mvares);
            // photons
            bpbhtags_obj.includeWeights(*dipho);
            bpbhtags_obj.setDiPhotonIndex(diphoIndex);

            bpbhtags_obj.setDiphoton_mass(diphoton_Mass_);
            bpbhtags_obj.setDiphoton_pt(diphoton_Pt_);
            bpbhtags_obj.setDiphoton_eta(diphoton_Eta_);
            bpbhtags_obj.setDiphoton_phi(diphoton_Phi_);

            // BP candidate
            bpbhtags_obj.setBP_mass(BP_mass_);
            bpbhtags_obj.setBP_pt(BP_pt_);
            bpbhtags_obj.setBP_eta(BP_eta_);
            bpbhtags_obj.setBP_phi(BP_phi_);

            // bjet_med 
            bpbhtags_obj.setbjetmed_mass(bjet_med_mass_);
            bpbhtags_obj.setbjetmed_pt(bjet_med_pt_);
            bpbhtags_obj.setbjetmed_eta(bjet_med_eta_);
            bpbhtags_obj.setbjetmed_phi(bjet_med_phi_);

            // Fwd jet
            bpbhtags_obj.setFwdjet_mass(Fwdjet_mass_);
            bpbhtags_obj.setFwdjet_pt(Fwdjet_pt_);
            bpbhtags_obj.setFwdjet_eta(Fwdjet_eta_);
            bpbhtags_obj.setFwdjet_phi(Fwdjet_phi_);

            // jets
            bpbhtags_obj.setnBjets_loose(nbjets_loose_);
            bpbhtags_obj.setnBjets_med(nbjets_med_);
            bpbhtags_obj.setnBjets_tight(nbjets_tight_);
            bpbhtags_obj.setnFwdJets(nFwdJets_);
            
            // dR
            bpbhtags_obj.setdRbmedjet_dipho(dR_bmedjet_dipho_);
            bpbhtags_obj.setdRbmedjet_Fwdjet(dR_bmedjet_Fwdjet_);
            bpbhtags_obj.setdRFwdjet_dipho(dR_Fwdjet_dipho_);

            // dEta
            bpbhtags_obj.setdEtabmedjet_dipho(dEta_bmedjet_dipho_);
            bpbhtags_obj.setdEtabmedjet_Fwdjet(dEta_bmedjet_Fwdjet_);
            bpbhtags_obj.setdEtaFwdjet_dipho(dEta_Fwdjet_dipho_);

            // dPhi
            bpbhtags_obj.setdPhibmedjet_dipho(dPhi_bmedjet_dipho_);
            bpbhtags_obj.setdPhibmedjet_Fwdjet(dPhi_bmedjet_Fwdjet_);
            bpbhtags_obj.setdPhiFwdjet_dipho(dPhi_Fwdjet_dipho_);

            bpbhtags->push_back(bpbhtags_obj);    

        } // closing loop over diPhotons

        evt.put( std::move(bpbhtags) );

    } // closing 'BPbHTagProducer::producer

} // closing 'namespace flashgg'

// ----------

typedef flashgg::BPbHTagProducer FlashggBPbHTagProducer;
DEFINE_FWK_MODULE(FlashggBPbHTagProducer);

