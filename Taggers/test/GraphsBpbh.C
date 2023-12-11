#include <iostream>
#include "tdrstyle.C"
#include "CMS_lumi.C"
#include <TFile.h>
#include <TDirectory.h>
#include <TTree.h>

void GraphsBpbh()
{
   setTDRStyle();
   gStyle->SetOptFit(0);
   writeExtraText = false;
   gROOT->SetStyle("Plain");
   gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   int W = 800; int H = 600; float T = 0.08*H;  float B = 0.12*H;  float L = 0.11*W;  float R = 0.04*W;
   
   TFile *f0 = TFile::Open("/afs/cern.ch/user/r/rdalialu/eos/CMSSW_10_6_29/src/flashgg/BprimeBtoBH_Hgg_M1000_1_test.root");

   TIter next(f0->GetListOfKeys());
   TKey *key;
   
   while ( (key = (TKey *)next()) ) {
      if (key->IsFolder()) {
         TDirectory* dir = (TDirectory*)f0->Get(key->GetName());
         TTree* tree = nullptr;
         if (dir){
         TDirectory* subdir = (TDirectory*)dir->Get("BPbHDumper;1");
         if (subdir){
            TDirectory* subsubdir = (TDirectory*)subdir->Get("trees;1");
            if (subsubdir){
               tree = (TTree*)subsubdir->Get("bpbh_125_13TeV_BPbHTag");
             }
          }
         }
         //TTree* tree = r(TTree*)dir->Get("BPbHDumper;1/trees;1/bpbh_125_13TeV_BPbHTag");
         if (tree) {
            Long64_t entries = tree->GetEntries();
            std::cout << "Directory: " << key->GetName() << ", Tree Entries: " << entries << std::endl;
         
            //---------- Higgs Pt
            TCanvas *c0 = new TCanvas("c0","C0",200,10,700,500);
            c0->cd();  c0->SetLeftMargin( L/W );  c0->SetRightMargin( R/W );  c0->SetTopMargin( T/H );  c0->SetBottomMargin( B/H );  
            TH1F *h0 = new TH1F("h0", "Diphoton P_{t} [GeV]",100, 0, 100); 
            tree->Draw("diphoton_pt>>h0"); h0->SetXTitle("P_{t} [GeV]");  h0->SetYTitle("Counts");
            h0->SetLineWidth(2);  h0->Rebin(10);  h0->SetLineColor(6);  h0->SetFillColorAlpha(6,0.25);  h0->Draw();
            TLegend* leg0 = new TLegend(0.5,.6,0.9,0.9);  leg0->SetFillStyle(0);  leg0->SetBorderSize(0);  leg0->SetTextSize(0.035);
            leg0->AddEntry(h0,"Dihoton P_{t} [GeV]","l");  leg0->Draw();
            c0->Print("Diphoton_Pt.png");

            //---------- bp Pt
            TCanvas *c1 = new TCanvas("c1","C1",200,10,700,500);
            c1->cd();  c1->SetLeftMargin( L/W );  c1->SetRightMargin( R/W );  c1->SetTopMargin( T/H );  c1->SetBottomMargin( B/H );
            TH1F *h1 = new TH1F("h1", "Diphoton Mass [GeV]", 100, 0, 100);
            tree->Draw("diphoton_mass2>>h1"); h1->SetXTitle("P_{t} [GeV]");  h1->SetYTitle("Counts");
            h1->SetLineWidth(2);  h1->Rebin(10);  h1->SetLineColor(3);  h1->SetFillColorAlpha(3,0.25);  h1->Draw();
            TLegend* leg1 = new TLegend(0.5,.6,0.9,0.9);  leg1->SetFillStyle(0);  leg1->SetBorderSize(0);  leg1->SetTextSize(0.035);
            leg1->AddEntry(h1,"bp P_{t} [GeV]","l");  leg1->Draw();
            c1->Print("bp_Pt.png");

            //delete h0;
            //delete h1;
         }
      }
   }

   //---------- Higgs Pt
   //TCanvas *c0 = new TCanvas("c0","C0",200,10,700,500);
   //c0->cd();  c0->SetLeftMargin( L/W );  c0->SetRightMargin( R/W );  c0->SetTopMargin( T/H );  c0->SetBottomMargin( B/H );
   //TH1F *h0 = (TH1F*)entries->Get(diphoton_pt); h0->SetXTitle("P_{t} [GeV]");  h0->SetYTitle("Counts");
   //h0->SetLineWidth(2);  h0->Rebin(10);  h0->SetLineColor(6);  h0->SetFillColorAlpha(6,0.25);  h0->Draw();
   //TLegend* leg0 = new TLegend(0.5,.6,0.9,0.9);  leg0->SetFillStyle(0);  leg0->SetBorderSize(0);  leg0->SetTextSize(0.035);
   //leg0->AddEntry(h0,"Dihoton P_{t} [GeV]","l");  leg0->Draw();
   //c0->Print("Diphoton_Pt.png");
                           
   //---------- bp Pt
   //TCanvas *c1 = new TCanvas("c1","C1",200,10,700,500);
   //c1->cd();  c1->SetLeftMargin( L/W );  c1->SetRightMargin( R/W );  c1->SetTopMargin( T/H );  c1->SetBottomMargin( B/H );
   //TH1F *h1 = (TH1F*)f0->Get("h_bp_pT");  h1->SetXTitle("P_{t} [GeV]");  h1->SetYTitle("Counts");
   //h1->SetLineWidth(2);  h1->Rebin(10);  h1->SetLineColor(3);  h1->SetFillColorAlpha(3,0.25);  h1->Draw();
   //TLegend* leg1 = new TLegend(0.5,.6,0.9,0.9);  leg1->SetFillStyle(0);  leg1->SetBorderSize(0);  leg1->SetTextSize(0.035);
   //leg1->AddEntry(h1,"bp P_{t} [GeV]","l");  leg1->Draw();
   //c1->Print("bp_Pt.png");
                                                     
   //---------- bdec Pt
   TCanvas *c2 = new TCanvas("c2","C2",200,10,700,500);
   c2->cd();  c2->SetLeftMargin( L/W );  c2->SetRightMargin( R/W );  c2->SetTopMargin( T/H );  c2->SetBottomMargin( B/H );
   TH1F *h2 = (TH1F*)f0->Get("h_bdec_pT");  h2->SetXTitle("P_{t} [GeV]");  h2->SetYTitle("Counts");
   h2->SetLineWidth(2);  h2->Rebin(10);  h2->SetLineColor(4);  h2->SetFillColorAlpha(4,0.25);  h2->Draw();
   TLegend* leg2 = new TLegend(0.5,.6,0.9,0.9);  leg2->SetFillStyle(0);  leg2->SetBorderSize(0);  leg2->SetTextSize(0.035);
   leg2->AddEntry(h2,"bdec P_{t} [GeV]","l");  leg2->Draw();
   c2->Print("bdec_Pt.png");
                                                                                
   //--------- B Pt
   TCanvas *c3 = new TCanvas("c3","C3",200,10,700,500);
   c3->cd();  c3->SetLeftMargin( L/W );  c3->SetRightMargin( R/W );  c3->SetTopMargin( T/H );  c3->SetBottomMargin( B/H );
   TH1F *h3 = (TH1F*)f0->Get("h_B_pT");  h3->SetXTitle("P_{t} [GeV]");  h3->SetYTitle("Counts");
   h3->SetLineWidth(2);  h3->Rebin(10);  h3->SetLineColor(5);  h3->SetFillColorAlpha(5,0.25);  h3->Draw();
   TLegend* leg3 = new TLegend(0.5,.6,0.9,0.9);  leg3->SetFillStyle(0);  leg3->SetBorderSize(0);  leg3->SetTextSize(0.035);
   leg3->AddEntry(h3,"B P_{t} [GeV]","l");  leg3->Draw();
   c3->Print("B_Pt.png");
   
   ////---------------------------------------------------------------------------------------------------------------------------------------------
   //***********************************************************************************************************************************************
   //---------- Higgs Eta
   TCanvas *c4 = new TCanvas("c4","C4",200,10,700,500);
   c4->cd();  c4->SetLeftMargin( L/W );  c4->SetRightMargin( R/W );  c4->SetTopMargin( T/H );  c4->SetBottomMargin( B/H );
   TH1F *h4 = (TH1F*)f0->Get("h_higgs_eta");  h4->SetXTitle("#eta");  h4->SetYTitle("Counts");
   h4->SetLineWidth(2);  h4->Rebin(1);  h4->SetLineColor(6);  h4->SetFillColorAlpha(6,0.25);  h4->Draw();
   TLegend* leg4 = new TLegend(0.7,.6,1.,0.9);  leg4->SetFillStyle(0);  leg4->SetBorderSize(0);  leg4->SetTextSize(0.035);
   leg4->AddEntry(h4,"Higgs #eta","l");  leg4->Draw();
   c4->Print("Higgs_Eta.png");
   
   //---------- bp Eta
   TCanvas *c5 = new TCanvas("c5","C5",200,10,700,500);
   c5->cd();  c5->SetLeftMargin( L/W );  c5->SetRightMargin( R/W );  c5->SetTopMargin( T/H );  c5->SetBottomMargin( B/H );
   TH1F *h5 = (TH1F*)f0->Get("h_bp_eta");  h5->SetXTitle("#eta");  h5->SetYTitle("Counts");
   h5->SetLineWidth(2);  h5->Rebin(1);  h5->SetLineColor(3);  h5->SetFillColorAlpha(3,0.25);  h5->Draw();
   TLegend* leg5 = new TLegend(0.7,.6,1.,0.9);  leg5->SetFillStyle(0);  leg5->SetBorderSize(0);  leg5->SetTextSize(0.035);
   leg5->AddEntry(h5,"bp #eta","l");  leg5->Draw();
   c5->Print("bp_Eta.png");
                                                                                                                                                                            
   //---------- bdec Eta
   TCanvas *c6 = new TCanvas("c6","C6",200,10,700,500);
   c6->cd();  c6->SetLeftMargin( L/W );  c6->SetRightMargin( R/W );  c6->SetTopMargin( T/H );  c6->SetBottomMargin( B/H );
   TH1F *h6 = (TH1F*)f0->Get("h_bdec_eta");  h6->SetXTitle("#eta");  h6->SetYTitle("Counts");
   h6->SetLineWidth(2);  h6->Rebin(1);  h6->SetLineColor(4);  h6->SetFillColorAlpha(4,0.25);  h6->Draw();
   TLegend* leg6 = new TLegend(0.7,.6,1.,0.9);  leg6->SetFillStyle(0);  leg6->SetBorderSize(0);  leg6->SetTextSize(0.035);
   leg6->AddEntry(h6,"bdec #eta","l");  leg6->Draw();
   c6->Print("bdec_Eta.png");
                                                                                                                                                                                                         
   //---------- B Eta
   TCanvas *c7 = new TCanvas("c7","C7",200,10,700,500);
   c7->cd();  c7->SetLeftMargin( L/W );  c7->SetRightMargin( R/W );  c7->SetTopMargin( T/H );  c7->SetBottomMargin( B/H );
   TH1F *h7 = (TH1F*)f0->Get("h_B_eta");  h7->SetXTitle("#eta");  h7->SetYTitle("Counts");
   h7->SetLineWidth(2);  h7->Rebin(1);  h7->SetLineColor(5);  h7->SetFillColorAlpha(5,0.25);  h7->Draw();
   TLegend* leg7 = new TLegend(0.7,.6,1.,0.9);  leg7->SetFillStyle(0);  leg7->SetBorderSize(0);  leg7->SetTextSize(0.035);
   leg7->AddEntry(h7,"B #eta","l");  leg7->Draw();
   c7->Print("B_Eta.png");
                                                                                                                                                                                         
   ////---------------------------------------------------------------------------------------------------------------------
   //***********************************************************************************************************************
   //---------- dR(bdec,h)
   TCanvas *c8 = new TCanvas("c8","C8",200,10,700,500);
   c8->cd();  c8->SetLeftMargin( L/W );  c8->SetRightMargin( R/W );  c8->SetTopMargin( T/H );  c8->SetBottomMargin( B/H );
   TH1F *h8 = (TH1F*)f0->Get("h_dR_bdec_higgs");  h8->SetXTitle("#DeltaR_{(bdec,H)}");  h8->SetYTitle("Counts");
   h8->SetLineWidth(2);  h8->Rebin(1);  h8->SetLineColor(93);  h8->SetFillColorAlpha(93,0.25);  h8->Draw();
   TLegend* leg8 = new TLegend(0.7,.6,0.9,0.9);  leg8->SetFillStyle(0);  leg8->SetBorderSize(0);  leg8->SetTextSize(0.035);
   leg8->AddEntry(h8,"#DeltaR_{(bdec,H)}","l");  leg8->Draw();
   c8->Print("dR(bdec,H).png");
   
   //---------- dR(h, bp)
   TCanvas *c9 = new TCanvas("c9","C9",200,10,700,500);
   c9->cd();  c9->SetLeftMargin( L/W );  c9->SetRightMargin( R/W );  c9->SetTopMargin( T/H );  c9->SetBottomMargin( B/H );
   TH1F *h9 = (TH1F*)f0->Get("h_dR_bp_higgs");  h9->SetXTitle("#DeltaR_{(H,bp)}");  h9->SetYTitle("Counts");
   h9->SetLineWidth(2);  h9->Rebin(1);  h9->SetLineColor(99);  h9->SetFillColorAlpha(99,0.25);  h9->Draw();
   TLegend* leg9 = new TLegend(0.7,.6,0.9,0.9);  leg9->SetFillStyle(0);  leg9->SetBorderSize(0);  leg9->SetTextSize(0.035);
   leg9->AddEntry(h9,"#DeltaR_{(H,bp)}","l");  leg9->Draw();
   c9->Print("dR(H,bp).png");
   
   //---------- dR(bdec,bp) c10
   TCanvas *c10 = new TCanvas("c10","C10",200,10,700,500);
   c10->cd();  c10->SetLeftMargin( L/W );  c10->SetRightMargin( R/W );  c10->SetTopMargin( T/H );  c10->SetBottomMargin( B/H );
   TH1F *h10 = (TH1F*)f0->Get("h_dR_bp_bdec");  h10->SetXTitle("#DeltaR_{(bp,bdec)}");  h10->SetYTitle("Counts");
   h10->SetLineWidth(2);  h10->Rebin(1);  h10->SetLineColor(70);  h10->SetFillColorAlpha(70,0.25);  h10->Draw();
   TLegend* leg10 = new TLegend(0.7,.6,0.9,0.9);  leg10->SetFillStyle(0);  leg10->SetBorderSize(0);  leg10->SetTextSize(0.035);
   leg10->AddEntry(h10,"#DeltaR_{(bp,bdec)}","l");  leg10->Draw();
   c10->Print("dR(bp,bdec).png");
   
   //---------- dR(bdec,q) c11
   TCanvas *c11 = new TCanvas("c11","C11",200,10,700,500);
   c11->cd();  c11->SetLeftMargin( L/W );  c11->SetRightMargin( R/W );  c11->SetTopMargin( T/H );  c11->SetBottomMargin( B/H );
   TH1F *h11 = (TH1F*)f0->Get("h_dR_q_bdec");  h11->SetXTitle("#DeltaR_{(bdec,q)}");  h11->SetYTitle("Counts");
   h11->SetLineWidth(2);  h11->Rebin(1);  h11->SetLineColor(65);  h11->SetFillColorAlpha(65,0.25);  h11->Draw();
   TLegend* leg11 = new TLegend(0.7,.6,0.9,0.9);  leg11->SetFillStyle(0);  leg11->SetBorderSize(0);  leg11->SetTextSize(0.035);
   leg11->AddEntry(h11,"#DeltaR_{(bdec,q)}","l");  leg11->Draw();
   c11->Print("dR(bdec,q).png");
  
   //---------- dR(q,bp) c12
   TCanvas *c12 = new TCanvas("c12","C12",200,10,700,500);
   c12->cd();  c12->SetLeftMargin( L/W );  c12->SetRightMargin( R/W );  c12->SetTopMargin( T/H );  c12->SetBottomMargin( B/H );
   TH1F *h12 = (TH1F*)f0->Get("h_dR_q_bp");  h12->SetXTitle("#DeltaR_{(bp,q)}");  h12->SetYTitle("Counts");
   h12->SetLineWidth(2);  h12->Rebin(1);  h12->SetLineColor(51);  h12->SetFillColorAlpha(51,0.25);  h12->Draw();
   TLegend* leg12 = new TLegend(0.7,.6,0.9,0.9);  leg12->SetFillStyle(0);  leg12->SetBorderSize(0);  leg12->SetTextSize(0.035);
   leg12->AddEntry(h12,"#DeltaR_{(bp,q)}","l");  leg12->Draw();
   c12->Print("dR(bp,q).png");
  
   //---------- dR(q,h) c13
   TCanvas *c13 = new TCanvas("c13","C13",200,10,700,500);
   c13->cd();  c13->SetLeftMargin( L/W );  c13->SetRightMargin( R/W );  c13->SetTopMargin( T/H );  c13->SetBottomMargin( B/H );
   TH1F *h13 = (TH1F*)f0->Get("h_dR_q_higgs");  h13->SetXTitle("#DeltaR_{(q,h)}");  h13->SetYTitle("Counts");
   h13->SetLineWidth(2);  h13->Rebin(1);  h13->SetLineColor(208);  h13->SetFillColorAlpha(208,0.25);  h13->Draw();
   TLegend* leg13 = new TLegend(0.7,.6,0.9,0.9);  leg13->SetFillStyle(0);  leg13->SetBorderSize(0);  leg13->SetTextSize(0.035);
   leg13->AddEntry(h13,"#DeltaR_{(q,h)}","l");  leg13->Draw();
   c13->Print("dR(q,h).png");
                                                                             
   ////---------------------------------------------------------------------------------------------------------------------
   //***********************************************************************************************************************
   //---------- dPhi(bdec,h) c14
   TCanvas *c14 = new TCanvas("c14","C14",200,10,700,500);
   c14->cd();  c14->SetLeftMargin( L/W );  c14->SetRightMargin( R/W );  c14->SetTopMargin( T/H );  c14->SetBottomMargin( B/H );
   TH1F *h14 = (TH1F*)f0->Get("h_dPhi_bdec_higgs");  h14->SetXTitle("#Delta#Phi_{(bdec,H)}");  h14->SetYTitle("Counts");
   h14->SetLineWidth(2);  h14->Rebin(1);  h14->SetLineColor(93);  h14->SetFillColorAlpha(93,0.25);  h14->Draw();
   TLegend* leg14 = new TLegend(0.7,.6,0.9,0.9);  leg14->SetFillStyle(0);  leg14->SetBorderSize(0);  leg14->SetTextSize(0.035);
   leg14->AddEntry(h14,"#Delta#Phi_{(bdec,H)}","l");  leg14->Draw();
   c14->Print("dPhi(bdec,H).png");

   //---------- dPhi(h, bp) c15
   TCanvas *c15 = new TCanvas("c15","C15",200,10,700,500);
   c15->cd();  c15->SetLeftMargin( L/W );  c15->SetRightMargin( R/W );  c15->SetTopMargin( T/H );  c15->SetBottomMargin( B/H );
   TH1F *h15 = (TH1F*)f0->Get("h_dPhi_bp_higgs");  h15->SetXTitle("#Delta#Phi_{(H,bp)}");  h15->SetYTitle("Counts");
   h15->SetLineWidth(2);  h15->Rebin(1);  h15->SetLineColor(99);  h15->SetFillColorAlpha(99,0.25);  h15->Draw();
   TLegend* leg15 = new TLegend(0.7,.6,0.9,0.9);  leg15->SetFillStyle(0);  leg15->SetBorderSize(0);  leg15->SetTextSize(0.035);
   leg15->AddEntry(h15,"#Delta#Phi_{(H,bp)}","l");  leg15->Draw();
   c15->Print("dPhi(H,bp).png");

   //---------- dPhi(bp,bdec) c16
   TCanvas *c16 = new TCanvas("c16","C16",200,10,700,500);
   c16->cd();  c16->SetLeftMargin( L/W );  c16->SetRightMargin( R/W );  c16->SetTopMargin( T/H );  c16->SetBottomMargin( B/H );
   TH1F *h16 = (TH1F*)f0->Get("h_dPhi_bp_bdec");  h16->SetXTitle("#Delta#Phi_{(bp,bdec)}");  h16->SetYTitle("Counts");
   h16->SetLineWidth(2);  h16->Rebin(1);  h16->SetLineColor(70);  h16->SetFillColorAlpha(70,0.25);  h16->Draw();
   TLegend* leg16 = new TLegend(0.7,.6,0.9,0.9);  leg16->SetFillStyle(0);  leg16->SetBorderSize(0);  leg16->SetTextSize(0.035);
   leg16->AddEntry(h16,"#Delta#Phi_{(bp,bdec)}","l");  leg16->Draw();
   c16->Print("dPhi(bp,bdec).png");

   //---------- dPhi(bdec,q) c17
   TCanvas *c17 = new TCanvas("c17","C17",200,10,700,500);
   c17->cd();  c17->SetLeftMargin( L/W );  c17->SetRightMargin( R/W );  c17->SetTopMargin( T/H );  c17->SetBottomMargin( B/H );
   TH1F *h17 = (TH1F*)f0->Get("h_dPhi_q_bdec");  h17->SetXTitle("#Delta#Phi_{(bdec,q)}");  h17->SetYTitle("Counts");
   h17->SetLineWidth(2);  h17->Rebin(1);  h17->SetLineColor(65);  h17->SetFillColorAlpha(65,0.25);  h17->Draw();
   TLegend* leg17 = new TLegend(0.7,.6,0.9,0.9);  leg17->SetFillStyle(0);  leg17->SetBorderSize(0);  leg17->SetTextSize(0.035);
   leg17->AddEntry(h17,"#Delta#Phi_{(bdec,q)}","l");  leg17->Draw();
   c17->Print("dPhi(bdec,q).png");

   //---------- dPhi(bp,q) c18
   TCanvas *c18 = new TCanvas("c18","C18",200,10,700,500);
   c18->cd();  c18->SetLeftMargin( L/W );  c18->SetRightMargin( R/W );  c18->SetTopMargin( T/H );  c18->SetBottomMargin( B/H );
   TH1F *h18 = (TH1F*)f0->Get("h_dPhi_q_bp");  h18->SetXTitle("#Delta#Phi_{(bp,q)}");  h18->SetYTitle("Counts");
   h18->SetLineWidth(2);  h18->Rebin(1);  h18->SetLineColor(51);  h18->SetFillColorAlpha(51,0.25);  h18->Draw();
   TLegend* leg18 = new TLegend(0.7,.6,0.9,0.9);  leg18->SetFillStyle(0);  leg18->SetBorderSize(0);  leg18->SetTextSize(0.035);
   leg18->AddEntry(h18,"#Delta#Phi_{(bp,q)}","l");  leg18->Draw();
   c18->Print("dPhi(bp,q).png");
   
   //---------- dPhi(q,h) c19
   TCanvas *c19 = new TCanvas("c19","C19",200,10,700,500);
   c19->cd();  c19->SetLeftMargin( L/W );  c19->SetRightMargin( R/W );  c19->SetTopMargin( T/H );  c19->SetBottomMargin( B/H );
   TH1F *h19 = (TH1F*)f0->Get("h_dPhi_q_higgs");  h19->SetXTitle("#Delta#Phi_{(q,h)}");  h19->SetYTitle("Counts");
   h19->SetLineWidth(2);  h19->Rebin(1);  h19->SetLineColor(208);  h19->SetFillColorAlpha(208,0.25);  h19->Draw();
   TLegend* leg19 = new TLegend(0.7,.6,0.9,0.9);  leg19->SetFillStyle(0);  leg19->SetBorderSize(0);  leg19->SetTextSize(0.035);
   leg19->AddEntry(h19,"#Delta#Phi_{(q,h)}","l");  leg19->Draw();
   c19->Print("dPhi(q,h).png");
   
   ////---------------------------------------------------------------------------------------------------------------------
   //***********************************************************************************************************************
   //---------- dEta(bdec,h) c20
   TCanvas *c20 = new TCanvas("c20","C120",200,10,700,500);
   c20->cd();  c20->SetLeftMargin( L/W );  c20->SetRightMargin( R/W );  c20->SetTopMargin( T/H );  c20->SetBottomMargin( B/H );
   TH1F *h20 = (TH1F*)f0->Get("h_dEta_bdec_higgs");  h20->SetXTitle("#Delta#eta_{(bdec,H)}");  h20->SetYTitle("Counts");
   h20->SetLineWidth(2);  h20->Rebin(1);  h20->SetLineColor(93);  h20->SetFillColorAlpha(93,0.25);  h20->Draw();
   TLegend* leg20 = new TLegend(0.7,.6,0.9,0.9);  leg20->SetFillStyle(0);  leg20->SetBorderSize(0);  leg20->SetTextSize(0.035);
   leg20->AddEntry(h20,"#Delta#eta_{(bdec,H)}","l");  leg20->Draw();
   c20->Print("dEta(bdec,H).png");
   
   //---------- dEta(h, bp) c21
   TCanvas *c21 = new TCanvas("c21","C21",200,10,700,500);
   c21->cd();  c21->SetLeftMargin( L/W );  c21->SetRightMargin( R/W );  c21->SetTopMargin( T/H );  c21->SetBottomMargin( B/H );
   TH1F *h21 = (TH1F*)f0->Get("h_dEta_bp_higgs");  h21->SetXTitle("#Delta#eta_{(H,bp)}");  h21->SetYTitle("Counts");
   h21->SetLineWidth(2);  h21->Rebin(1);  h21->SetLineColor(99);  h21->SetFillColorAlpha(99,0.25);  h21->Draw();
   TLegend* leg21 = new TLegend(0.7,.6,0.9,0.9);  leg21->SetFillStyle(0);  leg21->SetBorderSize(0);  leg21->SetTextSize(0.035);
   leg21->AddEntry(h21,"#Delta#eta_{(H,bp)}","l");  leg21->Draw();
   c21->Print("dEta(H,bp).png");
   
   //---------- dEta(bdec,bp) c22
   TCanvas *c22 = new TCanvas("c22","C22",200,10,700,500);
   c22->cd();  c22->SetLeftMargin( L/W );  c22->SetRightMargin( R/W );  c22->SetTopMargin( T/H );  c22->SetBottomMargin( B/H );
   TH1F *h22 = (TH1F*)f0->Get("h_dEta_bp_bdec");  h22->SetXTitle("#Delta#eta_{(bp,bdec)}");  h22->SetYTitle("Counts");
   h22->SetLineWidth(2);  h22->Rebin(1);  h22->SetLineColor(70);  h22->SetFillColorAlpha(70,0.25);  h22->Draw();
   TLegend* leg22 = new TLegend(0.7,.6,0.9,0.9);  leg22->SetFillStyle(0);  leg22->SetBorderSize(0);  leg22->SetTextSize(0.035);
   leg22->AddEntry(h22,"#Delta#eta_{(bp,bdec)}","l");  leg22->Draw();
   c22->Print("dEta(bp,bdec).png");
   
   //---------- dEta(bdec,q) c23
   TCanvas *c23 = new TCanvas("c23","C23",200,10,700,500);
   c23->cd();  c23->SetLeftMargin( L/W );  c23->SetRightMargin( R/W );  c23->SetTopMargin( T/H );  c23->SetBottomMargin( B/H );
   TH1F *h23 = (TH1F*)f0->Get("h_dEta_q_bdec");  h23->SetXTitle("#Delta#eta_{(bdec,q)}");  h23->SetYTitle("Counts");
   h23->SetLineWidth(2);  h23->Rebin(1);  h23->SetLineColor(65);  h23->SetFillColorAlpha(65,0.25);  h23->Draw();
   TLegend* leg23 = new TLegend(0.7,.6,0.9,0.9);  leg23->SetFillStyle(0);  leg23->SetBorderSize(0);  leg23->SetTextSize(0.035);
   leg23->AddEntry(h23,"#Delta#eta_{(bdec,q)}","l");  leg23->Draw();
   c23->Print("dEta(bdec,q).png");
   
   //---------- dEta(q,bp) c24
   TCanvas *c24 = new TCanvas("c24","C24",200,10,700,500);
   c24->cd();  c24->SetLeftMargin( L/W );  c24->SetRightMargin( R/W );  c24->SetTopMargin( T/H );  c24->SetBottomMargin( B/H );
   TH1F *h24 = (TH1F*)f0->Get("h_dEta_q_bp");  h24->SetXTitle("#Delta#eta_{(bp,q)}");  h24->SetYTitle("Counts");
   h24->SetLineWidth(2);  h24->Rebin(1);  h24->SetLineColor(51);  h24->SetFillColorAlpha(51,0.25);  h24->Draw();
   TLegend* leg24 = new TLegend(0.7,.6,0.9,0.9);  leg24->SetFillStyle(0);  leg24->SetBorderSize(0);  leg24->SetTextSize(0.035);
   leg24->AddEntry(h24,"#Delta#eta_{(bp,q)}","l");  leg24->Draw();
   c24->Print("dEta(bp,q).png");
   
   //---------- dEta(q,h) c25
   TCanvas *c25 = new TCanvas("c25","C25",200,10,700,500);
   c25->cd();  c25->SetLeftMargin( L/W );  c25->SetRightMargin( R/W );  c25->SetTopMargin( T/H );  c25->SetBottomMargin( B/H );
   TH1F *h25 = (TH1F*)f0->Get("h_dEta_q_higgs");  h25->SetXTitle("#Delta#eta_{(q,h)}");  h25->SetYTitle("Counts");
   h25->SetLineWidth(2);  h25->Rebin(1);  h25->SetLineColor(208);  h25->SetFillColorAlpha(208,0.25);  h25->Draw();
   TLegend* leg25 = new TLegend(0.7,.6,0.9,0.9);  leg25->SetFillStyle(0);  leg25->SetBorderSize(0);  leg25->SetTextSize(0.035);
   leg25->AddEntry(h25,"#Delta#eta_{(q,h)}","l");  leg25->Draw();
   c25->Print("dEta(q,h).png");
   
   ///////-----------------------------************************************q
   //---------- q Pt
   TCanvas *c26 = new TCanvas("c26","C26",200,10,700,500);
   c26->cd();  c26->SetLeftMargin( L/W );  c26->SetRightMargin( R/W );  c26->SetTopMargin( T/H );  c26->SetBottomMargin( B/H );
   TH1F *h26 = (TH1F*)f0->Get("h_q_pT");  h26->SetXTitle("P_{t} [GeV]");  h26->SetYTitle("Counts");
   h26->SetLineWidth(2);  h26->Rebin(10);  h26->SetLineColor(213);  h26->SetFillColorAlpha(213,0.25);  h26->Draw();
   TLegend* leg26 = new TLegend(0.5,.6,0.9,0.9);  leg26->SetFillStyle(0);  leg26->SetBorderSize(0);  leg26->SetTextSize(0.035);
   leg26->AddEntry(h26,"q P_{t} [GeV]","l");  leg26->Draw();
   c26->Print("q_Pt.png");
   
   //---------- q mass
   TCanvas *c27 = new TCanvas("c27","C27",200,10,700,500);
   c27->cd();  c27->SetLeftMargin( L/W );  c27->SetRightMargin( R/W );  c27->SetTopMargin( T/H );  c27->SetBottomMargin( B/H );
   TH1F *h27 = (TH1F*)f0->Get("h_q_mass");  h27->SetXTitle("Mass [GeV]");  h27->SetYTitle("Counts");
   h27->SetLineWidth(2);  h27->Rebin(1);  h27->SetLineColor(213);  h27->SetFillColorAlpha(213,0.25);  h27->Draw();
   TLegend* leg27 = new TLegend(0.5,.6,0.9,0.9);  leg27->SetFillStyle(0);  leg27->SetBorderSize(0);  leg27->SetTextSize(0.035);
   leg27->AddEntry(h27,"q Mass [GeV]","l");  leg27->Draw();
   c27->Print("q_Mass.png");
   
   //---------- q Eta
   TCanvas *c28 = new TCanvas("c28","C28",200,10,700,500);
   c28->cd();  c28->SetLeftMargin( L/W );  c28->SetRightMargin( R/W );  c28->SetTopMargin( T/H );  c28->SetBottomMargin( B/H );
   TH1F *h28 = (TH1F*)f0->Get("h_q_eta");  h28->SetXTitle("#eta");  h28->SetYTitle("Counts");
   h28->SetLineWidth(2);  h28->Rebin(1);  h28->SetLineColor(213);  h28->SetFillColorAlpha(213,0.25);  h28->Draw();
   TLegend* leg28 = new TLegend(0.7,.6,0.9,0.9);  leg28->SetFillStyle(0);  leg28->SetBorderSize(0);  leg28->SetTextSize(0.035);
   leg28->AddEntry(h28,"q #eta","l");  leg28->Draw();
   c28->Print("q_Eta.png");
   
   /////////------------Masses
   ///
   //---------- Higgs Mass
   TCanvas *c29 = new TCanvas("c29","C29",200,10,700,500);
   c29->cd();  c29->SetLeftMargin( L/W );  c29->SetRightMargin( R/W );  c29->SetTopMargin( T/H );  c29->SetBottomMargin( B/H );
   TH1F *h29 = (TH1F*)f0->Get("h_higgs_mass");  h29->SetXTitle("M [GeV]");  h29->SetYTitle("Counts");
   h29->SetLineWidth(2);  h29->Rebin(10);  h29->SetLineColor(6);  h29->SetFillColorAlpha(6,0.25);  h29->Draw();
   TLegend* leg29 = new TLegend(0.5,.6,0.9,0.9);  leg29->SetFillStyle(0);  leg29->SetBorderSize(0);  leg29->SetTextSize(0.035);
   leg29->AddEntry(h29,"Higgs Mass [GeV]","l");  leg29->Draw();
   c29->Print("Higgs_Mass.png");
   
   //---------- bp Mass
   TCanvas *c30 = new TCanvas("c30","C30",200,10,700,500);
   c30->cd();  c30->SetLeftMargin( L/W );  c30->SetRightMargin( R/W );  c30->SetTopMargin( T/H );  c30->SetBottomMargin( B/H );
   TH1F *h30 = (TH1F*)f0->Get("h_bp_mass");  h30->SetXTitle("M [GeV]");  h30->SetYTitle("Counts");
   h30->SetLineWidth(2);  h30->Rebin(10);  h30->SetLineColor(3);  h30->SetFillColorAlpha(3,0.25);  h30->Draw();
   TLegend* leg30 = new TLegend(0.5,.6,0.9,0.9);  leg30->SetFillStyle(0);  leg30->SetBorderSize(0);  leg30->SetTextSize(0.035);
   leg30->AddEntry(h30,"bp Mass [GeV]","l");  leg30->Draw();
   c30->Print("bp_Mass.png");
  
   //---------- bdec Mass
   TCanvas *c31 = new TCanvas("c31","C31",200,10,700,500);
   c31->cd();  c31->SetLeftMargin( L/W );  c31->SetRightMargin( R/W );  c31->SetTopMargin( T/H );  c31->SetBottomMargin( B/H );
   TH1F *h31 = (TH1F*)f0->Get("h_bdec_mass");  h31->SetXTitle("M [GeV]");  h31->SetYTitle("Counts");
   h31->SetLineWidth(2);  h31->Rebin(10);  h31->SetLineColor(4);  h31->SetFillColorAlpha(4,0.25);  h31->Draw();
   TLegend* leg31 = new TLegend(0.5,.6,0.9,0.9);  leg31->SetFillStyle(0);  leg31->SetBorderSize(0);  leg31->SetTextSize(0.035);
   leg31->AddEntry(h31,"bdec Mass [GeV]","l");  leg31->Draw();
   c31->Print("bdec_Mass.png");
   
   //---------- B Mass
   TCanvas *c32 = new TCanvas("c32","C32",200,10,700,500);
   c32->cd();  c32->SetLeftMargin( L/W );  c32->SetRightMargin( R/W );  c32->SetTopMargin( T/H );  c32->SetBottomMargin( B/H );
   TH1F *h32 = (TH1F*)f0->Get("h_B_mass");  h32->SetXTitle("M [GeV]");  h32->SetYTitle("Counts");
   h32->SetLineWidth(2);  h32->Rebin(10);  h32->SetLineColor(5);  h32->SetFillColorAlpha(5,0.25);  h32->Draw();
   TLegend* leg32 = new TLegend(0.5,.6,0.9,0.9);  leg32->SetFillStyle(0);  leg32->SetBorderSize(0);  leg32->SetTextSize(0.035);
   leg32->AddEntry(h32,"B Mass [GeV]","l");  leg32->Draw();
   c32->Print("B_Mass.png");
   
   //---------- H rapidity
   TCanvas *c33 = new TCanvas("c33","C33",200,10,700,500);
   c33->cd();  c33->SetLeftMargin( L/W );  c33->SetRightMargin( R/W );  c33->SetTopMargin( T/H );  c33->SetBottomMargin( B/H );
   TH1F *h33 = (TH1F*)f0->Get("h_higgs_rapidity");  h33->SetXTitle("Rapidity");  h33->SetYTitle("Counts");
   h33->SetLineWidth(2);  h33->Rebin(1);  h33->SetLineColor(6);  h33->SetFillColorAlpha(6,0.25);  h33->Draw();
   TLegend* leg33 = new TLegend(0.5,.6,0.9,0.9);  leg33->SetFillStyle(0);  leg33->SetBorderSize(0);  leg33->SetTextSize(0.035);
   leg33->AddEntry(h33,"Higgs Rapidity","l");  leg33->Draw();
   c33->Print("Higgs_rapidity.png");
  
   //----------- bp rapidity 
   TCanvas *c34 = new TCanvas("c34","C34",200,10,700,500);
   c34->cd();  c34->SetLeftMargin( L/W );  c34->SetRightMargin( R/W );  c34->SetTopMargin( T/H );  c34->SetBottomMargin( B/H );
   c34->cd();  c34->SetLeftMargin( L/W );  c34->SetRightMargin( R/W );  c34->SetTopMargin( T/H );  c34->SetBottomMargin( B/H );
   TH1F *h34 = (TH1F*)f0->Get("h_bp_rapidity");  h34->SetXTitle("Rapidity");  h34->SetYTitle("Counts");
   h34->SetLineWidth(2);  h34->Rebin(10);  h34->SetLineColor(3);  h34->SetFillColorAlpha(3,0.25);  h34->Draw();
   TLegend* leg34 = new TLegend(0.5,.6,0.9,0.9);  leg34->SetFillStyle(0);  leg34->SetBorderSize(0);  leg34->SetTextSize(0.035);
   leg34->AddEntry(h34,"bp Rapidity","l");  leg34->Draw();
   c34->Print("bp_rapidity.png");
   
   //-------------bdec rapidity
   TCanvas *c35 = new TCanvas("c35","C35",200,10,700,500);
   c35->cd();  c35->SetLeftMargin( L/W );  c35->SetRightMargin( R/W );  c35->SetTopMargin( T/H );  c35->SetBottomMargin( B/H );
   TH1F *h35 = (TH1F*)f0->Get("h_bdec_rapidity");  h35->SetXTitle("Rapidity");  h35->SetYTitle("Counts");
   h35->SetLineWidth(2);  h35->Rebin(10);  h35->SetLineColor(4);  h35->SetFillColorAlpha(4,0.25);  h35->Draw();
   TLegend* leg35 = new TLegend(0.5,.6,0.9,0.9);  leg35->SetFillStyle(0);  leg35->SetBorderSize(0);  leg35->SetTextSize(0.035);
   leg35->AddEntry(h35,"bdec Rapidity","l");  leg35->Draw();
   c35->Print("bdec_rapidity.png");
   
   //--------------B rapidity
   TCanvas *c36 = new TCanvas("c36","C36",200,10,700,500);
   c36->cd();  c36->SetLeftMargin( L/W );  c36->SetRightMargin( R/W );  c36->SetTopMargin( T/H );  c36->SetBottomMargin( B/H );
   TH1F *h36 = (TH1F*)f0->Get("h_B_rapidity");  h36->SetXTitle("Rapidity");  h36->SetYTitle("Counts");
   h36->SetLineWidth(2);  h36->Rebin(10);  h36->SetLineColor(5);  h36->SetFillColorAlpha(5,0.25);  h36->Draw();
   TLegend* leg36 = new TLegend(0.5,.6,0.9,0.9);  leg36->SetFillStyle(0);  leg36->SetBorderSize(0);  leg36->SetTextSize(0.035);
   leg36->AddEntry(h36,"B Rapidity","l");  leg36->Draw();
   c36->Print("B_rapidity.png");
   
   //------------q rapidity
   TCanvas *c37 = new TCanvas("c37","C37",200,10,700,500);
   c37->cd();  c37->SetLeftMargin( L/W );  c37->SetRightMargin( R/W );  c37->SetTopMargin( T/H );  c37->SetBottomMargin( B/H );
   TH1F *h37 = (TH1F*)f0->Get("h_q_rapidity");  h37->SetXTitle("Rapidity");  h37->SetYTitle("Counts");
   h37->SetLineWidth(2);  h37->Rebin(10);  h37->SetLineColor(213);  h37->SetFillColorAlpha(213,0.25);  h37->Draw();
   TLegend* leg37 = new TLegend(0.5,.6,0.9,0.9);  leg37->SetFillStyle(0);  leg37->SetBorderSize(0);  leg37->SetTextSize(0.035);
   leg37->AddEntry(h37,"q Rapidity","l");  leg37->Draw();
   c37->Print("q_rapidity.png");

}
