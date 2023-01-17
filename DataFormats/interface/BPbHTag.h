#ifndef FLASHgg_BPbHTag_h
#define FLASHgg_BPbHTag_h

#include "flashgg/DataFormats/interface/DiPhotonTagBase.h"
#include "flashgg/DataFormats/interface/Electron.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/Muon.h"

namespace flashgg {

    class BPbHTag : public DiPhotonTagBase {

        public:
            BPbHTag();
            BPbHTag(edm::Ptr<DiPhotonCandidate>, edm::Ptr<DiPhotonMVAResult>);
            BPbHTag(edm::Ptr<DiPhotonCandidate>, DiPhotonMVAResult);

            ~BPbHTag();

            BPbHTag *clone() const override { return (new BPbHTag(*this)); }

    };

} // closing 'namespace flashgg'

#endif
