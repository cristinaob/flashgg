#include "flashgg/DataFormats/interface/BPbHTag.h"

#include <algorithm>

using namespace flashgg;

BPbHTag::BPbHTag() : DiPhotonTagBase::DiPhotonTagBase() {
}

BPbHTag::~BPbHTag() {
}

BPbHTag::BPbHTag(edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares) : BPbHTag::BPbHTag(diPho, *mvares) {
}

BPbHTag::BPbHTag(edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares) : DiPhotonTagBase::DiPhotonTagBase(dipho, mvares) {
}
