#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"
#include "TCanvas.h"
#include <vector>
#include <set>
#include <iostream>

void AnalyzeHitsFourSim() {
    // Open the file and get the TTree
    //TFile *file = TFile::Open("/net/cms26/cms26r0/schmitz/milliQanSim/README/Flat_MilliQan.root");
    //TFile *file = TFile::Open("/net/cms26/cms26r0/schmitz/milliQanSim/README/Flat_MilliQan.root");
    //TFile *file = TFile::Open("/net/cms17/cms17r0/schmitz/milliQanMerged/MilliQan_Run1176.root");
    //TFile *file = TFile::Open("/net/cms26/cms26r0/schmitz/mqData/cosmicBarSample.root");
    
    TFile *file = TFile::Open("/net/cms26/cms26r0/zheng/barsim/cosmicSim/flat/gama/merge1k.root");
    TTree *t = (TTree*)file->Get("t");

    // Variables to hold branch data
    std::vector<int> *row = 0;
    std::vector<int> *column = 0;
    std::vector<int> *layer = 0;
    std::vector<int> *type = 0;
    std::vector<float> *nPE = 0;
    std::vector<float> *time = 0;
    std::vector<int> *ipulse = 0;

    // Set branch addresses
    t->SetBranchAddress("row", &row);
    t->SetBranchAddress("column", &column);
    t->SetBranchAddress("layer", &layer);
    t->SetBranchAddress("type", &type);
    t->SetBranchAddress("nPE", &nPE);
    t->SetBranchAddress("pmt_time", &time);

    Long64_t nentries = t->GetEntries();

    std::vector<int> uniqueBarCounts;
    std::vector<float> nPEBars;
    std::vector<float> Dts;
    double nPEMax[4];
    float minTime[4];
    int bigLayer;
    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);

        bool panelHit1 = false;
        bool barHit1 = false;
        bool panelHit2 = false;
        bool barHit2 = false;
        bool barHit3 = false;
        bool barHit4 = false;
        double Dt = 0.0;
        bigLayer=-2;
        std::set<int> uniqueBars;

        nPEMax[0]=0;
        nPEMax[1]=0;
        nPEMax[2]=0;
        nPEMax[3]=0;
        minTime[0]=1000;
        minTime[1]=1000;
        minTime[2]=1000;
        minTime[3]=1000;
        // Loop through all hits in the event
        for (size_t j = 0; j < row->size(); j++) {
            // Check for hits in panels with nPE > 10000
            //if ((*type)[j]==2 && ((*layer)[j] == 0 ) && (*nPE)[j] > 2) {
            if ((*type)[j]==2 && (*row)[j] == 4 && ((*layer)[j] == 0 ) && (*nPE)[j] > 2) {
                panelHit1 = true;
            }
            // Check for hits in the bottom row with nPE > 10
            //if ((*type)[j]==2 && ((*layer)[j] == 2 ) && (*nPE)[j] > 2) {
            if ((*type)[j]==2 && (*row)[j] == 4 && ((*layer)[j] == 2 ) && (*nPE)[j] > 2) {
                panelHit2 = true;
            }
            if ((*type)[j]==0 && (*layer)[j] == 0 && (*nPE)[j] > 2) {
                barHit1 = true;
//                if((*nPE)[j]>nPEMax[0]) nPEMax[0]=(*nPE)[j];
                //cout << "big hit! NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
            }
            if ((*type)[j]==0 && (*layer)[j] == 1 && (*nPE)[j] > 2) {
                barHit2 = true;
//                if((*nPE)[j]>nPEMax[0]) nPEMax[0]=(*nPE)[j];
                //cout << "big hit! NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
            }
            if ((*type)[j]==0 && (*layer)[j] == 2 && (*nPE)[j] > 2) {
                barHit3 = true;
//                if((*nPE)[j]>nPEMax[0]) nPEMax[0]=(*nPE)[j];
                //cout << "big hit! NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
            }
            if ((*type)[j]==0 && (*layer)[j] == 3 && (*nPE)[j] > 2) {
                barHit4 = true;
//                if((*nPE)[j]>nPEMax[0]) nPEMax[0]=(*nPE)[j];
                //cout << "big hit! NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
            }
        }

        // If both conditions are satisfied, count unique hits with nPE > 0.5
        if ((barHit1 && barHit2 && barHit3 && barHit4) && (panelHit1 || panelHit2)) {
            for (size_t j = 0; j < row->size(); j++) {
                if ((*type)[j] == 0 && (*nPE)[j] > 1) {
                    uniqueBars.insert((*row)[j]+4*(*column)[j]+16*(*layer)[j]);
                    if((*nPE)[j]>nPEMax[(*layer)[j]]) {nPEMax[(*layer)[j]] = (*nPE)[j];}
                   //     std::cout << "NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
                    //cout << "raw time" << (*time)[j] << endl;
                    //cout << "raw layer" << (*layer)[j] << endl;
                    //cout << "minTime[(*layer)[j]]" << minTime[(*layer)[j]] << endl;
                    if (((*time)[j] < minTime[(*layer)[j]] )) {
                        minTime[(*layer)[j]] = (*time)[j];
		        //cout << "(*time)[j]"  << (*time)[j] << endl;
                    }
               }
            }
            if(nPEMax[0]>2 && nPEMax[1]>2 && nPEMax[2]>2 && nPEMax[3]>2){
            nPEBars.push_back(nPEMax[0]);
            nPEBars.push_back(nPEMax[1]);
            nPEBars.push_back(nPEMax[2]);
            nPEBars.push_back(nPEMax[3]);
            Dt = (minTime[3]-3*3.96) - minTime[0];
            //cout << Dt << endl;
            Dts.push_back(Dt);
            uniqueBarCounts.push_back(uniqueBars.size());
            //std::cout << "Number of unique hits: " << uniqueBarCounts.size() << std::endl;
            } 
            //nPEBars.clear();
            uniqueBars.clear();
        }
    }

    // Plot the number of unique bars hit
    TH1I *hUniqueBarsSim = new TH1I("hUniqueBarsSim", "Number of Unique Bars Hit", 70, 0, 70);
    for (size_t k = 0; k < uniqueBarCounts.size(); k++) {
        hUniqueBarsSim->Fill(uniqueBarCounts[k]);
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    hUniqueBarsSim->GetXaxis()->SetTitle("Number of Unique Bars Hit");
    hUniqueBarsSim->GetYaxis()->SetTitle("Events");
    hUniqueBarsSim->Draw();

    TH1F *hNPEBars = new TH1F("hNPEBars", "Max NPE In each layer", 100, 0, 5000);
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


    TFile *fileo = new TFile("histogramsim.root", "RECREATE");
    hUniqueBarsSim->Write();
    hNPEBars->Write();
    Dtmax->Write();
    fileo->Close();

}

