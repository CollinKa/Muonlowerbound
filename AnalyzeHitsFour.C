//Original author: Ryan Schimitz
//Modified by Collin Zheng




#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"
#include "TCanvas.h"
#include <vector>
#include <set>
#include <iostream>

void AnalyzeHitsFour() {
    // Open the file and get the TTree
    TFile *file = TFile::Open("/net/cms26/cms26r0/zheng/barsim/ExtraValidationR/M1176V35_merged.root");
    //TFile *file = TFile::Open("Run1679.root");
    TTree *t = (TTree*)file->Get("t");

    // Variables to hold branch data
    std::vector<int> *row = 0;
    std::vector<int> *column = 0;
    std::vector<int> *layer = 0;
    std::vector<int> *type = 0;
    std::vector<float> *nPE = 0;
    std::vector<int> *ipulse = 0;
    std::vector<float> *time = 0;
    Bool_t boardsMatched;
    std::vector<bool> *pickupFlag = nullptr;


    // Set branch addresses
    t->SetBranchAddress("row", &row);
    t->SetBranchAddress("column", &column);
    t->SetBranchAddress("layer", &layer);
    t->SetBranchAddress("type", &type);
    t->SetBranchAddress("nPE", &nPE);
    t->SetBranchAddress("ipulse", &ipulse);
    t->SetBranchAddress("timeFit_module_calibrated", &time);
    t->SetBranchAddress("boardsMatched", &boardsMatched);
    t->SetBranchAddress("pickupFlag", &pickupFlag);    


    Long64_t nentries = t->GetEntries();

    std::vector<int> uniqueBarCounts;
    std::vector<float> nPEBars;
    std::vector<float> Dts;
    double nPEMax[4];
    double minTime[4];
    int bigLayer;
    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);
        
        if (boardsMatched == 0) {continue;}
        
        bool panelHit1 = false;
        bool barHit1 = false;
        bool panelHit2 = false;
        bool barHit2 = false;
        bool barHit3 = false;
        bool barHit4 = false;
        double Dt = 0.0;
        minTime[0]=3000;
        minTime[1]=3000;
        minTime[2]=3000;  
        minTime[3]=3000;      
        bigLayer=-2;
        std::set<int> uniqueBars;
        nPEMax[0]=0;
        nPEMax[1]=0;
        nPEMax[2]=0;
        nPEMax[3]=0;
        // Loop through all hits in the event
        for (size_t j = 0; j < row->size(); j++) {
            //if ((*pickupFlag)[j] == 1) {continue;}
            //if (((*time)[j] < 1250) || ((*time)[j] > 1350)) {continue;}
            // Check for hits in panels with nPE > 10000
            //if ((*ipulse)[j]==0 && (*type)[j]==2 && ((*layer)[j] == 0 ) && (*nPE)[j] > 10000) {
            if ((*ipulse)[j]==0 && (*type)[j]==2 && (*row)[j] == 4 && ((*layer)[j] == 0 ) && (*nPE)[j] > 10000) {
                panelHit1 = true;
            }
            // Check for hits in the bottom row with nPE > 10
            //if ((*ipulse)[j]==0 && (*type)[j]==2 && ((*layer)[j] == 2 ) && (*nPE)[j] > 10000) {
            if ((*ipulse)[j]==0 && (*type)[j]==2 && (*row)[j] == 4 && ((*layer)[j] == 2 ) && (*nPE)[j] > 10000) {
                panelHit2 = true;
            }
            if ((*ipulse)[j]==0 && (*type)[j]==0 && (*layer)[j] == 0 && (*nPE)[j] > 2) {
                barHit1 = true;
//                if((*nPE)[j]>nPEMax[0]) nPEMax[0]=(*nPE)[j];
                //cout << "big hit! NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
            }
            if ((*ipulse)[j]==0 && (*type)[j]==0 && (*layer)[j] == 1 && (*nPE)[j] > 2) {
                barHit2 = true;
//                if((*nPE)[j]>nPEMax[0]) nPEMax[0]=(*nPE)[j];
                //cout << "big hit! NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
            }
            if ((*ipulse)[j]==0 && (*type)[j]==0 && (*layer)[j] == 2 && (*nPE)[j] > 2) {
                barHit3 = true;
//                if((*nPE)[j]>nPEMax[0]) nPEMax[0]=(*nPE)[j];
                //cout << "big hit! NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
            }
            if ((*ipulse)[j]==0 && (*type)[j]==0 && (*layer)[j] == 3 && (*nPE)[j] > 2) {
                barHit4 = true;
//                if((*nPE)[j]>nPEMax[0]) nPEMax[0]=(*nPE)[j];
                //cout << "big hit! NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
            }
        }
        //cout << "worki" << endl; 
        // If both conditions are satisfied, count unique hits with nPE > 0.5
        //cout << barHit1 << endl;
        //cout << barHit2 << endl;
        //cout << barHit3 << endl;
        //cout << barHit4 << endl;
        //cout << (panelHit1 || panelHit2) << endl;

        if ((barHit1 && barHit2 && barHit3 && barHit4) && (panelHit1 || panelHit2)) {
            //cout << "worki2" << endl;
            for (size_t j = 0; j < row->size(); j++) {
                //cout << "work0" << endl;
                if ((*ipulse)[j]==0 && (*type)[j] == 0 && (*nPE)[j] > 1) {
                    uniqueBars.insert((*row)[j]+4*(*column)[j]+16*(*layer)[j]);
                    //cout << "work1" << endl;
                    if((*nPE)[j]>nPEMax[(*layer)[j]]) {nPEMax[(*layer)[j]] = (*nPE)[j];}
                   //     std::cout << "NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
                    //cout << "work" << endl;
                    if ((*time)[j] < minTime[(*layer)[j]]) {minTime[(*layer)[j]] = (*time)[j];}
                    //cout << "work2" << endl;
                }
            }
            if(nPEMax[0]>2 && nPEMax[1]>2 && nPEMax[2]>2 && nPEMax[3]>2){
            nPEBars.push_back(nPEMax[0]);
            nPEBars.push_back(nPEMax[1]);
            nPEBars.push_back(nPEMax[2]);
            nPEBars.push_back(nPEMax[3]);
            Dt = (minTime[3]) - minTime[0];
            Dts.push_back(Dt);
            uniqueBarCounts.push_back(uniqueBars.size());
            //std::cout << "Number of unique hits: " << uniqueBarCounts.size() << std::endl;
            } 
            //nPEBars.clear();
            uniqueBars.clear();
        }
    }
    //cout << "work2" << endl;
    // Plot the number of unique bars hit
    TH1I *hUniqueBars = new TH1I("hUniqueBars", "Number of Unique Bars Hit", 70, 0, 70);
    for (size_t k = 0; k < uniqueBarCounts.size(); k++) {
        hUniqueBars->Fill(uniqueBarCounts[k]);
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    hUniqueBars->GetXaxis()->SetTitle("Number of Unique Bars Hit");
    hUniqueBars->GetYaxis()->SetTitle("Events");
    hUniqueBars->Draw();

    TH1F *hNPEBars = new TH1F("hNPEBars", "Max NPE in each layer", 100, 0, 5000);
    for (size_t k = 0; k < nPEBars.size(); k++) {
        hNPEBars->Fill(nPEBars[k]);
    }

    TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
    hNPEBars->GetXaxis()->SetTitle("Max Bar NPE in Row");
    hNPEBars->GetYaxis()->SetTitle("Number of Events");
    hNPEBars->Draw();


    TH1F *Dtmax = new TH1F("Dtmax", "Dt max", 30, -30, 30);
    for (size_t k = 0; k < Dts.size(); k++) {
        Dtmax->Fill(Dts[k]);
    }

    TFile *fileo = new TFile("histograms.root", "UPDATE");
    hUniqueBars->Write();
    hNPEBars->Write();
    Dtmax->Write();
    fileo->Close(); 
}

