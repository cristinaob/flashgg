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

            // ------------------------------------------------
            // functions to add variables to output ntuple

            // -----------------------
            // mass of candidate BP

            float getBP_mass() const {  // function to be called by dumper
                return BP_mass_;
            }

            void setBP_mass(float BP_mass) {  // function called by plugin to fill variable called by dumper
                BP_mass_ = BP_mass;
            }

            // -----------------------
            // number of bjets loose

            int getnBjets_loose() const {
                return nBjets_loose_;
            }

            void setnBjets_loose(int nBjets_loose) {
                nBjets_loose_ = nBjets_loose;
            }

            // -----------------------
            // number of bjets medium

            int getnBjets_med() const {
                return nBjets_med_;
            }

            void setnBjets_med(int nBjets_med) {
                nBjets_med_ = nBjets_med;
            }

            // -----------------------
            // number of bjets tight

            int getnBjets_tight() const {
                return nBjets_tight_;
            }

            void setnBjets_tight(int nBjets_tight) {
                nBjets_tight_ = nBjets_tight;
            }

        private:
            
            float BP_mass_;
            int nBjets_loose_; 
            int nBjets_med_;
            int nBjets_tight_;

    };

} // closing 'namespace flashgg'

