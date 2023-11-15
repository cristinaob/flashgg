#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "flashgg/DataFormats/interface/Met.h"

namespace flashgg {

    class BPbHTag: public DiPhotonTagBase {

        public:
            BPbHTag();
            BPbHTag(edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>);
            BPbHTag(edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult);

            ~BPbHTag();

            BPbHTag *clone() const override { return (new BPbHTag(*this)); }

            DiPhotonTagBase::tag_t tagEnum() const override { return DiPhotonTagBase::kBPbH; }
      
           // const std::vector<edm::Ptr<Muon> > muons() const { return Muons_; }
	   // const std::vector<edm::Ptr<Jet> > jets() const { return Jets_; }


	   // void setMuons( std::vector<edm::Ptr<Muon> > Muons ) { Muons_ = Muons; }
	   // void setJets( std::vector<edm::Ptr<Jet> > Jets ) { Jets_ = Jets; }   



 };

} // closing 'namespace flashgg'

