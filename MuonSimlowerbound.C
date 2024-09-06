/*
Aug 28 

with muon like hits search for the pulse delay from the small pulses.

*/



#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"
#include "TCanvas.h"
#include <vector>
#include <set>
#include <iostream>



void MuonSimlowerbound() {
    // Open the file and get the TTree
    //TFile *file = TFile::Open("/net/cms26/cms26r0/zheng/barsim/cosmicSim/flat/withoutgama/out_110.root"); //can't make dtNPE plot for unknown reason
    TFile *file = TFile::Open("/net/cms26/cms26r0/zheng/barsim/cosmicSim/flat/gama/NewMerge.root");
    //TFile *file = TFile::Open("Run1679.root");
    TTree *t = (TTree*)file->Get("t");

    // Variables to hold branch data
    std::vector<int> *row = 0;
    std::vector<int> *column = 0;
    std::vector<int> *layer = 0;
    std::vector<int> *type = 0;
    std::vector<int> *muonHit = 0;
    std::vector<float> *nPE = 0;
    //std::vector<int> *ipulse = 0;
    std::vector<float> *duration = 0;
    std::vector<float> *time = 0;
    //Long64_t fileNumber = 0;
    Int_t event = 0; //event number
    Int_t runNumber = 0; //file number
    // Set branch addresses
    t->SetBranchAddress("runNumber", &runNumber);
    t->SetBranchAddress("pmt_row", &row);
    t->SetBranchAddress("pmt_column", &column);
    t->SetBranchAddress("pmt_layer", &layer);
    t->SetBranchAddress("pmt_type", &type);
    t->SetBranchAddress("pmt_nPE", &nPE);
    t->SetBranchAddress("muonHit", &muonHit);
    
    t->SetBranchAddress("event", &event);
    //t->SetBranchAddress("duration", &duration);
    //t->SetBranchAddress("timeFit_module_calibrated", &timeFit_module_calibrated);
    //t->SetBranchAddress("time", &time); //scinttime
    t->SetBranchAddress("pmt_time", &time); //save the data in pmt_time to time variable.
    
    Long64_t nentries = t->GetEntries();

    std::vector<int> uniqueBarCounts;
    std::vector<float> nPEBars;
    std::vector<float> nPE4Bars;
    //std::vector<float> duration4Bars;
    std::vector<float> Dt;
    std::vector<float> SPnPE; //small pulse nPE distribution
    double nPEMax[4];
    //double durationMax[4];
    double Pulse4Max[4];
        



    int bigLayer;
    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);

        bool panelHit1 = false;
        bool barHit1 = false;
        bool panelHit2 = false;
        bool barHit2 = false;
        bool Muonlay0 = false;
        bool Muonlay3 = false;
        bigLayer=-2;
        std::set<int> uniqueBars;

        nPEMax[0]=0;
        nPEMax[1]=0;
        nPEMax[2]=0;
        nPEMax[3]=0;

        

        Pulse4Max[0]=0;
        Pulse4Max[1]=0;
        Pulse4Max[2]=0;
        Pulse4Max[3]=0;


        int numLargePulse = 0;
        int numSmallPulse = 0;



        // Loop through all hits in the event
        for (size_t j = 0; j < row->size(); j++) {
            // Check for hits in panels with nPE > 10000
            if ( (*type)[j]==2 && (*row)[j] == 4 && ((*layer)[j] == 0 ) && (*nPE)[j] > 300 ) {
                panelHit1 = true;
            }
            if ( (*type)[j]==0 && (*row)[j] == 0 && ((*layer)[j] == 0 || (*layer)[j] == 1) && (*nPE)[j] > 50 ) {
                barHit1 = true;
                bigLayer=(*layer)[j];
//                if((*nPE)[j]>nPEMax[0]) nPEMax[0]=(*nPE)[j];
                //cout << "big hit! NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
            }
            // Check for hits in the bottom row with nPE > 10
            if ( (*type)[j]==2 && (*row)[j] == 4 && ((*layer)[j] == 2 ) && (*nPE)[j] > 300 ) {
                panelHit2 = true;
            }
            if ( (*type)[j]==0 && (*row)[j] == 0 && ((*layer)[j] == 2 || (*layer)[j] == 3) && (*nPE)[j] > 50) {
                barHit2 = true;
                bigLayer=(*layer)[j];
//                if((*nPE)[j]>nPEMax[0]) nPEMax[0]=(*nPE)[j];
                //cout << "big hit! NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
            }
            

            //check muon pdg
            if ( ((*layer)[j] == 0) && (*type)[j]==0 && (*muonHit)[j] == 1 ) {Muonlay0 = true;}
            if ( ((*layer)[j] == 3) && (*type)[j]==0 && (*muonHit)[j] == 1 ) {Muonlay3 = true;}

        }

        // If both conditions are satisfied, count unique hits with nPE > 0.5
        //if(Muonlay0 && Muonlay3){
        if ((panelHit1 && barHit1) || (panelHit2 && barHit2)) {
        //if ((panelHit1 && barHit1 && Muonlay0 && Muonlay3) || (panelHit2 && barHit2 && Muonlay0 && Muonlay3)) {
            for (size_t j = 0; j < row->size(); j++) {
                if ((*type)[j] == 0 && (*nPE)[j] > 2) {
                    uniqueBars.insert((*row)[j]+4*(*column)[j]+16*(*layer)[j]);
                    if((*layer)[j]==bigLayer){
                        if((*nPE)[j]>nPEMax[(*row)[j]]) nPEMax[(*row)[j]] = (*nPE)[j];
                        std::cout << "NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
                        //collect the time for largest pulse time at each row
                        Pulse4Max[(*row)[j]] = (*time)[j];                  

 
                   }
                }
            }
            //if(nPEMax[0]<70 && nPEMax[0]>20  && nPEMax[1]<70 && nPEMax[1]>20  && nPEMax[2]<70 && nPEMax[2]>20  && nPEMax[3]<70 && nPEMax[3]>20){
            //if(nPEMax[0] > 2000  && nPEMax[1]>2000 && nPEMax[2]>2000){
            int count = 0;
            double minNpe = 5000;
            for (size_t k = 0; k < 4; k++){
                if(nPEMax[k] > 2000) {
                    count += 1;
                }
                if(nPEMax[k] < minNpe){
                minNpe = nPEMax[k];
                                
                }
                
                

            }
            //count the number row under big hit layer is above TH.
            if (count >=3) { 
            


            //if(nPEMax[0]>50 && nPEMax[0]<100  && nPEMax[1]> 50 && nPEMax[1]< 100  && nPEMax[2]> 50 && nPEMax[2]< 100  && nPEMax[3]> 50  && nPEMax[3]< 100){
            //if(nPEMax[0]>200 && nPEMax[1]> 200  && nPEMax[2]> 200  && nPEMax[3]> 200){
            //find the number of large pulse and  small pulse
            //for (size_t k = 0; k < 4; k++){
            //if (nPEMax[k] > 5 && nPEMax[k] < 40 ) { numSmallPulse += 1;}
            //if (nPEMax[k] >200  ) { numLargePulse += 1;}
            //}
            //case for finding 3 large pulse and 1 small pulse
            //if (numLargePulse == 3 && numSmallPulse == 1) {

            

            //find the first (bar) pulse in big hit layer
            double maxVal = 0.0;
            for(int i = 0; i < 4; i++) {
                if(Pulse4Max[i] > maxVal){
                    maxVal = Pulse4Max[i];
                }           
 
            }
            

            //find the last (bar)pulse
            double minVal = 3000.0;
            for(int i = 0; i < 4; i++) {
                if(Pulse4Max[i] < minVal){
                    minVal= Pulse4Max[i];
                }
             }
            
            //find the event that max Dt < 5ns for pulse in big hit layer
            //if (maxVal - minVal <= 3){
            double DtF = maxVal - minVal;
            nPEBars.push_back(nPEMax[0]);
            nPEBars.push_back(nPEMax[1]);
            nPEBars.push_back(nPEMax[2]);
            nPEBars.push_back(nPEMax[3]);
            uniqueBarCounts.push_back(uniqueBars.size());
            
            //std::cout << "Number of unique hits: " << uniqueBarCounts.size() << std::endl;
            //std::cout << "Number of unique uniqueBars.size: " << uniqueBars.size() << std::endl; 
            /*
            if ((uniqueBars.size() >= 20) && ((uniqueBars.size() <= 24))){
                   std::cout << "uniqueBars.size(): " << uniqueBars.size() << std::endl;
                   std::cout << "Event Number: " << (event) << std::endl;
                   std::cout << "File Number: " << (fileNumber) << std::endl;
            }
            */
            


            //if ((uniqueBars.size() == 5) && ((uniqueBars.size() == 4))){
            if (uniqueBars.size() >= 4 && uniqueBars.size() <= 6 ){
            //if (uniqueBars.size() == 4  ){
                Dt.push_back(DtF);
                nPE4Bars.push_back(nPEMax[0]);
                nPE4Bars.push_back(nPEMax[1]);
                nPE4Bars.push_back(nPEMax[2]);
                nPE4Bars.push_back(nPEMax[3]);
                //SPnPE.push_back(nPEMax[3]);
                SPnPE.push_back(minNpe);
                if (DtF > 10){               
                     std::cout << "uniqueBars.size(): " << uniqueBars.size() << std::endl;
                     std::cout << "4 bar Event Number: " << (event) << std::endl;
                     std::cout << "File Number: " << (runNumber) << std::endl;
                }
            }

            } 
            //nPEBars.clear();
            uniqueBars.clear();
            
        }
    }

    // Plot the number of unique bars hit
    TH1I *hUniqueBars = new TH1I("hUniqueBars", "Number of Unique Bars Hit", 70, 0, 70);
    for (size_t k = 0; k < uniqueBarCounts.size(); k++) {
        hUniqueBars->Fill(uniqueBarCounts[k]);
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    hUniqueBars->GetXaxis()->SetTitle("Number of Unique Bars Hit");
    hUniqueBars->GetYaxis()->SetTitle("Events");
    hUniqueBars->Draw();
    

    //NPE distribution for 4 bar 
    TCanvas *c3 = new TCanvas("c3", "c3", 800, 600);
    TH1F *hNPE4Bars = new TH1F("hNPE4Bars", "Max NPE In Rows", 100, 0, 5000);
    for (size_t k = 0; k < nPE4Bars.size(); k++) {
        hNPE4Bars->Fill(nPE4Bars[k]);
    }
    hNPE4Bars->GetXaxis()->SetTitle("Max Bar NPE in Row");
    hNPE4Bars->GetYaxis()->SetTitle("Events");
    hNPE4Bars->Draw();
    //debug
    //std::cout << 


    TH1F *hNPEBars = new TH1F("hNPEBars", "Max NPE In Other Rows", 100, 0, 5000);
    for (size_t k = 0; k < nPEBars.size(); k++) {
        hNPEBars->Fill(nPEBars[k]);
    }

    TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
    hNPEBars->GetXaxis()->SetTitle("Max Bar NPE in Row");
    hNPEBars->GetYaxis()->SetTitle("Number of Events");
    hNPEBars->Draw();







    TCanvas *c5 = new TCanvas("c5", "c5", 800, 600);
    TH1F *Dt4bar = new TH1F("Dt4bar", "Dt(ns)", 42, -1, 20);
    for (size_t k = 0; k < Dt.size(); k++) {
        Dt4bar->Fill(Dt[k]);
    }
    Dt4bar->GetYaxis()->SetTitle("Events");
    Dt4bar->Draw();


   //find the correlation btw Dt and size of small pulse(it might not be small).
   TCanvas *c6 = new TCanvas("c6", "c6", 800, 600);
   TH2F *SPnPEDt = new TH2F("SPnPEDt", "npe vs Dt;nPE;Dt(ns)", 40, 0, 500,40,0,40); 
   for (size_t k = 0; k < Dt.size(); k++) {
       SPnPEDt->Fill(SPnPE[k],Dt[k]);
   }
   SPnPEDt->Draw("COLZ");
}
