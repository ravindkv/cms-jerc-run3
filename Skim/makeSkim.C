#include<iostream>
#include <unistd.h>
#include <cstdlib>
#include<string>
#include<TFile.h>
#include<TTree.h>
#include<TH1F.h>
#include<TDirectory.h>
#include<TObject.h>
#include<TCanvas.h>
#include<iomanip>
#include <fstream>

#include "src/SkimTree.h"
#include <nlohmann/json.hpp>

int main(int argc, char* argv[]){

    std::ifstream file("sample/FilesNano_cff.json");
    nlohmann::json js = nlohmann::json::parse(file);

    //--------------------------------
    // Parse command-line options
    //--------------------------------
    int opt;
    std::string year;
    int nthJob    = 1; //nofN
    int totJob    = 10;
    std::string oName;
    std::string iName;
    while ((opt = getopt(argc, argv, "y:n:N:o:i:h")) != -1) {
        switch (opt) {
            case 'y':
                year = optarg;
                break;
            case 'n':
                nthJob = std::stoi(optarg);
                break;
            case 'N':
                totJob = std::stoi(optarg);
                break;
            case 'o':
                oName = optarg; 
                break;
            case 'i':
                iName = optarg; 
                break;
            case 'h':
                std::cout << "Usage: ./makeSkim -y 2022 -n 1 -N 100 -o oName -i iName\n" << std::endl;
                cout<<"Choose iName from the following:"<<endl;
                for (auto& element : js.items()) {
                    std::cout << element.key() << std::endl;
                }
                return 0;
            default:
                std::cerr << "Invalid option" << std::endl;
                return 1;
        }
    }

    //--------------------------------
    // files to run for each job 
    //--------------------------------
    std::vector<std::string> fileNames;
    js.at(iName).get_to(fileNames);
    int nFiles  = fileNames.size(); 
    cout<<"Total files = "<<nFiles<<endl;
    if (totJob > nFiles){
        cout<<"totJob > nFiles. Setting it to the nFiles = "<<nFiles<<endl;
        totJob = nFiles;
    }
    if (nthJob > totJob){
        cout<<"n > N. Setting it to the N = "<<nFiles<<endl;
        totJob = nFiles;
    }
	if (nthJob>0 && totJob>0){
	    cout <<"jobs: " <<nthJob << " of " << totJob << endl;
		cout << "Processing " << (1.*nFiles)/totJob << " files per job on average" << endl;
	    oName = oName+ "_" + std::to_string(nthJob)+"of"+std::to_string(totJob)+ ".root";
	    cout << "new output file name: "<< oName << endl;
	}
    else{
        cout<<"\n ERROR: n > 0 and N > 0\n ";
        return 0;
    }

    //--------------------------------
    // Read input files 
    //--------------------------------
	SkimTree* tree;
    std::vector<std::vector<std::string>> smallVectors = tree->splitVector(fileNames, totJob);
	cout << "HERE" << endl;
	bool isMC = true;
	if( oName.find("Data") != std::string::npos){
	    cout << "IsData" << endl;
	    isMC = false;
	}
	tree = new SkimTree(year, smallVectors[nthJob-1], isMC);

	TFile* outFile = TFile::Open( oName.c_str() ,"RECREATE","",207 );
    outFile->cd();
	TTree* newTree = tree->chain->GetTree()->CloneTree(0);
	newTree->SetCacheSize(50*1024*1024);
	Long64_t nEntr = tree->GetEntries();

	int startEntry = 0;
	int endEntry = nEntr;
	int eventsPerJob = nEntr;
	std::cout << "Sample has "<<nEntr << " entries" << std::endl;
	cout << "Processing events "<<startEntry<< " to " << endEntry << endl;
	TH1F* hEvents_ = new TH1F("hEvents", "Cutflow", 5, -1.5, 3.5);
    hEvents_->GetXaxis()->SetBinLabel(1, "NanoAOD");
    hEvents_->GetXaxis()->SetBinLabel(2, "Trig");
    
    //--------------------------------
    //Event loop
    //--------------------------------
    std::cout<<"---------------------------"<<std::endl;
    std::cout<<setw(10)<<"Progress"<<setw(10)<<"Time"<<std::endl;
    std::cout<<"---------------------------"<<std::endl;
    double totalTime = 0.0;
	auto startClock = std::chrono::high_resolution_clock::now();
    bool passTrig = false;
	for(Long64_t entry= startEntry; entry < endEntry; entry++){
        //if(entry>4235) break; 
		if(entry%(eventsPerJob/100) == 0){// print after every 1% of events
            totalTime+= std::chrono::duration<double>(std::chrono::high_resolution_clock::now()-startClock).count();
            int sec = (int)(totalTime)%60;
            int min = (int)(totalTime)/60;
	        std::cout<<setw(10)<<100*entry/endEntry<<" %"<<setw(10)<<min<<"m "<<sec<<"s"<<std::endl;
			startClock = std::chrono::high_resolution_clock::now();			
		}
		tree->GetEntry(entry);
		hEvents_->Fill(0);
        //2022
		passTrig = tree->p20 || tree->p20HE || tree->p30HE || tree->p33 || tree->p200;
		if(passTrig){
            hEvents_->Fill(1);
			newTree->Fill();
		}
	}
    std::cout<<"nEvents_Skim = "<<newTree->GetEntries()<<endl;
    newTree->Write();
	hEvents_->Write();
	outFile->Close();
	
	return 0;
}
