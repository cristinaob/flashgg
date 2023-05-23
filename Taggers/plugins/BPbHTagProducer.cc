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

            double leadPhoOverMassThreshold_;
            double subleadPhoOverMassThreshold_;
            double PhoMVAThreshold_;

    }; // closing 'class BPbHTagProducer'

    // ----------

    BPbHTagProducer::BPbHTagProducer(const ParameterSet &iConfig) :
        diPhotonToken_(consumes<View<flashgg::DiPhotonCandidate>>(iConfig.getParameter<InputTag> ("DiPhotonTag"))),
        mvaResultToken_(consumes<View<flashgg::DiPhotonMVAResult>>(iConfig.getParameter<InputTag> ("MVAResultTag"))) {

        leadPhoOverMassThreshold_ = iConfig.getParameter<double>("leadPhoOverMassThreshold");
        subleadPhoOverMassThreshold_ = iConfig.getParameter<double>("subleadPhoOverMassThreshold");
        PhoMVAThreshold_ = iConfig.getParameter<double>("PhoMVAThreshold");

        produces<vector<BPbHTag>>();

    } // closing 'BPbHTagProducer::BPbHTagProducer'

    // ----------

    void BPbHTagProducer::produce(Event &evt, const EventSetup &) {

        // Getting object information
        Handle<View<flashgg::DiPhotonCandidate>> diPhotons;
        evt.getByToken(diPhotonToken_, diPhotons);

        Handle<View<flashgg::DiPhotonMVAResult> > mvaResults;
        evt.getByToken(mvaResultToken_, mvaResults);

        // Declaring and initializing variables
        double idmva1 = 0.0;
        double idmva2 = 0.0;

        std::unique_ptr<vector<BPbHTag> > bpbhtags( new vector<BPbHTag> );

        std::cout << "=======> New Event with " << diPhotons->size() << " diphoton candidates" << std::endl; 

        // Loop over diphoton candidates
        for (unsigned int diphoIndex=0; diphoIndex < diPhotons->size(); diphoIndex++) {

            edm::Ptr<flashgg::DiPhotonCandidate> dipho = diPhotons->ptrAt(diphoIndex);
            edm::Ptr<flashgg::DiPhotonMVAResult> mvares = mvaResults->ptrAt(diphoIndex);

            idmva1 = dipho->leadingPhoton()->phoIdMvaDWrtVtx(dipho->vtx());
            idmva2 = dipho->subLeadingPhoton()->phoIdMvaDWrtVtx(dipho->vtx());

            std::cout << "diphoIndex " << diphoIndex << " leading pT: " << dipho->leadingPhoton()->pt() << std::endl;

            // Diphoton selection:
            // - pT cuts on leading and subleading photons
            // - cut on photon ID MVA
            if (dipho->leadingPhoton()->pt() < (dipho->mass())*leadPhoOverMassThreshold_) { continue; }
            if (dipho->subLeadingPhoton()->pt() < (dipho->mass())*subleadPhoOverMassThreshold_) { continue; }
            if (idmva1 < PhoMVAThreshold_ || idmva2 < PhoMVAThreshold_) { continue; }

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

