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
            // pt of candidate BP
            float getBP_pt() const {  // function to be called by dumper
                return BP_pt_;
            }

            void setBP_pt(float BP_pt) {  // function called by plugin to fill variable called by dumper
                BP_pt_ = BP_pt;
            }

            // -----------------------
            // eta of candidate BP
            float getBP_eta() const {  // function to be called by dumper
                return BP_eta_;
            }
   
            void setBP_eta(float BP_eta) {  // function called by plugin to fill variable called by dumper
                BP_eta_ = BP_eta;
            }
            
            // -----------------------
            // phi of candidate BP
            float getBP_phi() const {  // function to be called by dumper
                return BP_phi_;
            }

            void setBP_phi(float BP_phi) {  // function called by plugin to fill variable called by dumper
                BP_phi_ = BP_phi;
            }

            // -----------------------
            // mass of the diphoton
            float getDiphoton_mass() const {
                return diphoton_Mass_;
            }

            void setDiphoton_mass(float diphoton_Mass) {
                diphoton_Mass_ = diphoton_Mass;
            }

            // -----------------------
            // pt of diphoton
            float getDiphoton_pt() const {  // function to be called by dumper
                return diphoton_Pt_;
            }

            void setDiphoton_pt(float diphoton_Pt) {  // function called by plugin to fill variable called by dumper
                diphoton_Pt_ = diphoton_Pt;
            }

            // -----------------------
            // eta of diphoton
            float getDiphoton_eta() const {  // function to be called by dumper
                return diphoton_Eta_;
            }

            void setDiphoton_eta(float diphoton_Eta) {  // function called by plugin to fill variable called by dumper
                diphoton_Eta_ = diphoton_Eta;
            }

            // -----------------------
            // phi of diphoton
            float getDiphoton_phi() const {  // function to be called by dumper
                return diphoton_Phi_;
            }

            void setDiphoton_phi(float diphoton_Phi) {  // function called by plugin to fill variable called by dumper
                diphoton_Phi_ = diphoton_Phi;
            }

            // -----------------------
            // mass of the bjet med 
            float getbjetmed_mass() const {
                return bjet_med_mass_;
            }
            void setbjetmed_mass(float bjet_med_mass) {
                bjet_med_mass_ = bjet_med_mass;
            }

            // -----------------------
            // pt of the bjet med 
            float getbjetmed_pt() const {
                return bjet_med_pt_;
            }
            void setbjetmed_pt(float bjet_med_pt) {
                bjet_med_pt_ = bjet_med_pt;
            }
            
            // -----------------------
            // eta of the bjet med 
            float getbjetmed_eta() const {
                return bjet_med_eta_;
            }                       
            void setbjetmed_eta(float bjet_med_eta) {
                bjet_med_eta_ = bjet_med_eta;
            }

            // -----------------------
            // phi of the bjet med 
            float getbjetmed_phi() const {
                return bjet_med_phi_;
            }
            void setbjetmed_phi(float bjet_med_phi) {
                bjet_med_phi_ = bjet_med_phi;
            }

            // -----------------------
            // mass Fwd jet
            float getFwdjet_mass() const { 
                return Fwdjet_mass_;
            }
            void setFwdjet_mass(float Fwdjet_mass) {
                Fwdjet_mass_ = Fwdjet_mass;
            }
            // -----------------------
            // Pt Fwd jet
            float getFwdjet_pt() const {
                return Fwdjet_pt_;
            }
            void setFwdjet_pt(float Fwdjet_pt) {
                Fwdjet_pt_ = Fwdjet_pt;
            }
            // -----------------------
            // Eta Fwd jet
            float getFwdjet_eta() const {
                return Fwdjet_eta_;
            }
            void setFwdjet_eta(float Fwdjet_eta) {
                Fwdjet_eta_ = Fwdjet_eta;
            }

            // -----------------------
            // Phi Fwd jet
            float getFwdjet_phi() const {
                return Fwdjet_phi_;
            }
            void setFwdjet_phi(float Fwdjet_phi) {
                Fwdjet_phi_ = Fwdjet_phi;
            }
            
            
            
            



            // -----------------------
            // number of Fwd jets
    
            int getnFwdJets() const {
                return nFwdJets_;
            }

            void setnFwdJets(int nFwdJets) {
                 nFwdJets_ = nFwdJets;
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

            // -----------------------
            // dR between bmedjet and diphoton system
            float getdRbmedjet_dipho() const{
                return dR_bmedjet_dipho_;
            }
            void setdRbmedjet_dipho(float dR_bmedjet_dipho) {
                dR_bmedjet_dipho_ = dR_bmedjet_dipho;
            }
           
            // -----------------------
            // dEta between bmedjet and diphoton system
            float getdEtabmedjet_dipho() const{
                return dEta_bmedjet_dipho_;
            }
            void setdEtabmedjet_dipho(float dEta_bmedjet_dipho) {
                dEta_bmedjet_dipho_ = dEta_bmedjet_dipho;
            }

            // -----------------------
            // dPhi between bmedjet and diphoton system
            float getdPhibmedjet_dipho() const{
                return dPhi_bmedjet_dipho_;
            }
            void setdPhibmedjet_dipho(float dPhi_bmedjet_dipho) {
                dPhi_bmedjet_dipho_ = dPhi_bmedjet_dipho;
            }


           // -----------------------
           // dR between bmedjet and Fwd jet
           float getdRbmedjet_Fwdjet() const{
               return dR_bmedjet_Fwdjet_;
           }
           void setdRbmedjet_Fwdjet(float dR_bmedjet_Fwdjet) {
               dR_bmedjet_Fwdjet_ = dR_bmedjet_Fwdjet;
           }

           // -----------------------
           // dEta between bmedjet and Fwd jet
           float getdEtabmedjet_Fwdjet() const{
               return dEta_bmedjet_Fwdjet_;
           }
           void setdEtabmedjet_Fwdjet(float dEta_bmedjet_Fwdjet) {
               dEta_bmedjet_Fwdjet_ = dEta_bmedjet_Fwdjet;
           }

           // -----------------------
           // dPhi between bmedjet and Fwd jet
           float getdPhibmedjet_Fwdjet() const{
               return dPhi_bmedjet_Fwdjet_;
           }
           void setdPhibmedjet_Fwdjet(float dPhi_bmedjet_Fwdjet) {
               dPhi_bmedjet_Fwdjet_ = dPhi_bmedjet_Fwdjet;
           }


           // -----------------------
           // dR between Fwd jet and dipho
           float getdRFwdjet_dipho() const{
               return dR_Fwdjet_dipho_;
           }
           void setdRFwdjet_dipho(float dR_Fwdjet_dipho) {
               dR_Fwdjet_dipho_ = dR_Fwdjet_dipho;
           }
           // -----------------------
           // dEta between Fwd jet and dipho
           float getdEtaFwdjet_dipho() const{
               return dEta_Fwdjet_dipho_;
           }
           void setdEtaFwdjet_dipho(float dEta_Fwdjet_dipho) {
               dEta_Fwdjet_dipho_ = dEta_Fwdjet_dipho;
           }
           
           // -----------------------
           // dPhi between Fwd jet and dipho
           float getdPhiFwdjet_dipho() const{
               return dPhi_Fwdjet_dipho_;
           }
           void setdPhiFwdjet_dipho(float dPhi_Fwdjet_dipho) {
               dPhi_Fwdjet_dipho_ = dPhi_Fwdjet_dipho;
           }
           
           

        private:
            
            float diphoton_Mass_;
            float diphoton_Pt_;
            float diphoton_Eta_;
            float diphoton_Phi_;

            float BP_mass_;
            float BP_pt_;
            float BP_eta_;
            float BP_phi_;
            
            float bjet_med_mass_;
            float bjet_med_pt_;
            float bjet_med_eta_;
            float bjet_med_phi_;

            float Fwdjet_mass_;
            float Fwdjet_pt_;
            float Fwdjet_eta_;
            float Fwdjet_phi_;

            int   nBjets_loose_; 
            int   nBjets_med_;
            int   nBjets_tight_;
            int   nFwdJets_;

            float dR_bmedjet_dipho_;
            float dEta_bmedjet_dipho_;
            float dPhi_bmedjet_dipho_;

            float dR_bmedjet_Fwdjet_;
            float dEta_bmedjet_Fwdjet_;
            float dPhi_bmedjet_Fwdjet_;

            float dR_Fwdjet_dipho_;
            float dEta_Fwdjet_dipho_;
            float dPhi_Fwdjet_dipho_;

    };

} // closing 'namespace flashgg'

