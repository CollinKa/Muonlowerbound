/*
Aug 15 
find
data maybe in the 20-24 NBar  print file name, event number

*/



#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"
#include "TCanvas.h"
#include <vector>
#include <set>
#include <iostream>



void AnalyzeHitsStrict() {
    // Open the file and get the TTree
    TFile *file = TFile::Open("/net/cms17/cms17r0/schmitz/milliQanMerged/MilliQan_Run1176.root");
    //TFile *file = TFile::Open("Run1679.root");
    TTree *t = (TTree*)file->Get("t");

    // Variables to hold branch data
    std::vector<int> *row = 0;
    std::vector<int> *column = 0;
    std::vector<int> *layer = 0;
    std::vector<int> *type = 0;
    std::vector<float> *nPE = 0;
    std::vector<int> *ipulse = 0;
    std::vector<float> *duration = 0;
    std::vector<float> *timeFit_module_calibrated = 0;
    Long64_t fileNumber = 0;
    Int_t event = 0; //event number

    // Set branch addresses
    t->SetBranchAddress("row", &row);
    t->SetBranchAddress("column", &column);
    t->SetBranchAddress("layer", &layer);
    t->SetBranchAddress("type", &type);
    t->SetBranchAddress("nPE", &nPE);
    t->SetBranchAddress("ipulse", &ipulse);
    t->SetBranchAddress("fileNumber", &fileNumber);
    t->SetBranchAddress("event", &event);
    t->SetBranchAddress("duration", &duration);
    t->SetBranchAddress("timeFit_module_calibrated", &timeFit_module_calibrated);

    Long64_t nentries = t->GetEntries();

    std::vector<int> uniqueBarCounts;
    std::vector<float> nPEBars;
    std::vector<float> nPE4Bars;
    std::vector<float> duration4Bars;
    double nPEMax[4];
    double durationMax[4];
        



    int bigLayer;
    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);

        bool panelHit1 = false;
        bool barHit1 = false;
        bool panelHit2 = false;
        bool barHit2 = false;
        bigLayer=-2;
        std::set<int> uniqueBars;

        nPEMax[0]=0;
        nPEMax[1]=0;
        nPEMax[2]=0;
        nPEMax[3]=0;

        durationMax[0] = 0;
        durationMax[1] = 0;
        durationMax[2] = 0;
        durationMax[3] = 0;

        int numLargePulse = 0;
        int numSmallPulse = 0;



        // Loop through all hits in the event
        for (size_t j = 0; j < row->size(); j++) {
            // Check for hits in panels with nPE > 10000
            if ((*ipulse)[j]==0 && (*type)[j]==2 && (*row)[j] == 4 && ((*layer)[j] == 0 ) && (*nPE)[j] > 30000  && (*timeFit_module_calibrated)[j]>1250 && (*timeFit_module_calibrated)[j]<1350 ) {
                panelHit1 = true;
            }
            if ((*ipulse)[j]==0 && (*type)[j]==0 && (*row)[j] == 0 && ((*layer)[j] == 0 || (*layer)[j] == 1) && (*nPE)[j] > 50 && (*timeFit_module_calibrated)[j]>1250 && (*timeFit_module_calibrated)[j]<1350 ) {
                barHit1 = true;
                bigLayer=(*layer)[j];
//                if((*nPE)[j]>nPEMax[0]) nPEMax[0]=(*nPE)[j];
                //cout << "big hit! NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
            }
            // Check for hits in the bottom row with nPE > 10
            if ((*ipulse)[j]==0 && (*type)[j]==2 && (*row)[j] == 4 && ((*layer)[j] == 2 ) && (*nPE)[j] > 30000 && (*timeFit_module_calibrated)[j]>1250 && (*timeFit_module_calibrated)[j]<1350) {
                panelHit2 = true;
            }
            if ((*ipulse)[j]==0 && (*type)[j]==0 && (*row)[j] == 0 && ((*layer)[j] == 2 || (*layer)[j] == 3) && (*nPE)[j] > 50 && (*timeFit_module_calibrated)[j]>1250 && (*timeFit_module_calibrated)[j]<1350) {
                barHit2 = true;
                bigLayer=(*layer)[j];
//                if((*nPE)[j]>nPEMax[0]) nPEMax[0]=(*nPE)[j];
                //cout << "big hit! NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
            }
        }

        // If both conditions are satisfied, count unique hits with nPE > 0.5
        if ((panelHit1 && barHit1) || (panelHit2 && barHit2)) {
            for (size_t j = 0; j < row->size(); j++) {
                if ((*ipulse)[j]==0 && (*type)[j] == 0 && (*nPE)[j] > 2  && (*timeFit_module_calibrated)[j]>1250 && (*timeFit_module_calibrated)[j]<1350 ) {
                    uniqueBars.insert((*row)[j]+4*(*column)[j]+16*(*layer)[j]);
                    if((*layer)[j]==bigLayer){
                        if((*nPE)[j]>nPEMax[(*row)[j]]) nPEMax[(*row)[j]] = (*nPE)[j];
                        std::cout << "NPE " << (*nPE)[j] << "Row/Col/Layer: " << (*row)[j] << " " << (*column)[j] << " " << (*layer)[j] << std::endl;
                    
                        //find the max pusle duration at each row
                        if((*duration)[j]>durationMax[(*row)[j]] ) durationMax[(*row)[j]] = (*duration)[j];



                   }
                }
            }
            //if(nPEMax[0]<400 && nPEMax[0]>10  && nPEMax[1]<400 && nPEMax[1]>10  && nPEMax[2]<400 && nPEMax[2]>10  && nPEMax[3]<400 && nPEMax[3]>10 && durationMax[0] < 1200 && durationMax[1]<1200 && durationMax[2]<1200){
            //if(nPEMax[0]>50 && nPEMax[0]<100  && nPEMax[1]> 50 && nPEMax[1]< 100  && nPEMax[2]> 50 && nPEMax[2]< 100  && nPEMax[3]> 50  && nPEMax[3]< 100){
            
            //find the number of large pulse and  small pulse
            for (size_t k = 0; k < 4; k++){
            if (nPEMax[k] > 40 && nPEMax[k] < 180 ) { numSmallPulse += 1;}
            if (nPEMax[k] >200  ) { numLargePulse += 1;}
            }
            //case for finding 3 large pulse and 1 small pulse
            if (numLargePulse == 3 && numSmallPulse == 1) {
            nPEBars.push_back(nPEMax[0]);
            nPEBars.push_back(nPEMax[1]);
            nPEBars.push_back(nPEMax[2]);
            nPEBars.push_back(nPEMax[3]);
            uniqueBarCounts.push_back(uniqueBars.size());
            
            std::cout << "Number of unique hits: " << uniqueBarCounts.size() << std::endl;
            std::cout << "Number of unique uniqueBars.size: " << uniqueBars.size() << std::endl; 
            /*
            if ((uniqueBars.size() >= 20) && ((uniqueBars.size() <= 24))){
                   std::cout << "uniqueBars.size(): " << uniqueBars.size() << std::endl;
                   std::cout << "Event Number: " << (event) << std::endl;
                   std::cout << "File Number: " << (fileNumber) << std::endl;
            }
            */
            


            //if ((uniqueBars.size() == 5) && ((uniqueBars.size() == 4))){
            if (uniqueBars.size() == 4){
                nPE4Bars.push_back(nPEMax[0]);
                nPE4Bars.push_back(nPEMax[1]);
                nPE4Bars.push_back(nPEMax[2]);
                nPE4Bars.push_back(nPEMax[3]); 
                duration4Bars.push_back(durationMax[0]);
                duration4Bars.push_back(durationMax[1]);
                duration4Bars.push_back(durationMax[2]);
                duration4Bars.push_back(durationMax[3]);
                std::cout << "uniqueBars.size(): " << uniqueBars.size() << std::endl;
                std::cout << "Event Number: " << (event) << std::endl;
                std::cout << "File Number: " << (fileNumber) << std::endl;
                
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
    TH1F *hNPE4Bars = new TH1F("hNPE4Bars", "Max NPE In Rows", 100, 0, 1000);
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



    TCanvas *c4 = new TCanvas("c4", "c4", 800, 600);
    TH1F *hDuration4Bars = new TH1F("hDuration4Bars", "Max duration In Rows", 100, 0, 2500);
    for (size_t k = 0; k < duration4Bars.size(); k++) {
        hDuration4Bars->Fill(duration4Bars[k]);
    }
    hDuration4Bars->GetXaxis()->SetTitle("Max Bar duration in Row");
    hDuration4Bars->GetYaxis()->SetTitle("Events");
    hDuration4Bars->Draw();



    

}

