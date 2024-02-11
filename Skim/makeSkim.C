#include<iostream>
#include<unistd.h>
#include<cstdlib>
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
#include <boost/algorithm/string.hpp>

    #include <iostream>
#include <string>
#include <sstream>

int main(int argc, char* argv[]){

    std::ifstream file("sample/FilesNano_cff.json");
    nlohmann::json js = nlohmann::json::parse(file);

    //--------------------------------
    // Parse command-line options
    //--------------------------------
    int opt;
    std::string oName = js.begin().key()+"__1of100.root";
    while ((opt = getopt(argc, argv, "o:h")) != -1) {
        switch (opt) {
            case 'o':
                oName = optarg;
                break;
            case 'h':
                std::cout << "Usage: ./makeSkim -o sKey__1of1000.root\n" << std::endl;
                cout<<"Choose sKey from the following:"<<endl;
                for (auto& element : js.items()) {
                    std::cout << element.key() << std::endl;
                }
                return 0;
            default:
                std::cerr << "Invalid option" << std::endl;
                return 1;
        }
    }
    cout<<"--------------------------------------------"<<endl;
    cout<<"Inputs: ./makeSkim -o " <<oName<<endl;
    cout<<"--------------------------------------------"<<endl;
    // Extracting he sample key 
    std::string sKey = oName.substr(0, oName.find("__"));
    
    // Extracting year
    TString year = oName.substr(oName.find("__") + 2, 4);
    sKey = sKey+"__"+year;
    std::cout << "sKey: " << sKey << std::endl;
    
	// Finding the position of the second "__"
    std::size_t pos_second_double_underscore = oName.find("__", oName.find("__") + 1);
    if (pos_second_double_underscore == std::string::npos) {
        std::cerr << "Second '__' not found." << std::endl;
        return 1;
    }
    
    // Extracting the part after the second "__"
    std::string fraction_str = oName.substr(pos_second_double_underscore + 2);
    std::cout << "Fraction: " << fraction_str << std::endl;

	// Finding the position of "of"
    std::size_t pos_of = fraction_str.find("of");
    
    // Extracting the numerator before "of"
    std::string numerator_str = fraction_str.substr(0, pos_of);
    int nthJob = std::stoi(numerator_str);
    std::cout << "nthJob: " << nthJob << std::endl;
    
	// Extracting the denominator after "of" and before "."
    std::string denominator_str = fraction_str.substr(pos_of + 2, fraction_str.find(".") - pos_of - 2);
    int totJob = std::stoi(denominator_str);
    std::cout << "totJob: " << totJob << std::endl;

    //--------------------------------
    // files to run for each job
    //--------------------------------
    std::vector<std::string> fileNames;
    js.at(sKey).get_to(fileNames);
    int nFiles  = fileNames.size();
    cout<<"Total files = "<<nFiles<<endl;
    if (totJob > nFiles){
        cout<<"Since totJob > nFiles, setting it to the nFiles, totJob = "<<nFiles<<endl;
        totJob = nFiles;
    }
    if (nthJob > totJob){
        cout<<"Error: Make sure nthJob < totJob"<<endl;
        return 0;
    }
	if (nthJob>0 && totJob>0){
	    cout <<"jobs: " <<nthJob << " of " << totJob << endl;
		cout << "Processing " << (1.*nFiles)/totJob << " files per job on average" << endl;
	    cout << "new output file name: "<< oName << endl;
	}
    else{
        cout<<"\n Error: Make sure nthJob > 0 and totJob > 0\n ";
        return 0;
    }

    //--------------------------------
    // Read input files
    //--------------------------------
	SkimTree* tree;
    std::vector<std::vector<std::string>> smallVectors = tree->splitVector(fileNames, totJob);
	bool isMC = true;
	if( sKey.find("Data") != std::string::npos){
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
        if(year.Contains("2022")){
		passTrig =
            tree->HLT_Photon300_NoHE                                                ||
            tree->HLT_Photon20                                                      ||
            tree->HLT_Photon33                                                      ||
            tree->HLT_Photon50                                                      ||
            tree->HLT_Photon75                                                      ||
            tree->HLT_Photon90                                                      ||
            tree->HLT_Photon120                                                     ||
            tree->HLT_Photon150                                                     ||
            tree->HLT_Photon175                                                     ||
            tree->HLT_Photon200                                                     ||
            tree->HLT_Photon30EB_TightID_TightIso                                   ||
            tree->HLT_Photon100EB_TightID_TightIso                                  ||
            tree->HLT_Photon110EB_TightID_TightIso                                  ||
            tree->HLT_Photon120EB_TightID_TightIso                                  ||
            tree->HLT_Photon100EBHE10                                               ||
            tree->HLT_Photon100EEHE10                                               ||
            tree->HLT_Photon100EE_TightID_TightIso                                  ||
            tree->HLT_Photon50_R9Id90_HE10_IsoM                                     ||
            tree->HLT_Photon75_R9Id90_HE10_IsoM                                     ||
            tree->HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3 ||
            tree->HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3 ||
            tree->HLT_Photon90_R9Id90_HE10_IsoM                                     ||
            tree->HLT_Photon120_R9Id90_HE10_IsoM                                    ||
            tree->HLT_Photon165_R9Id90_HE10_IsoM                                    ||
            tree->HLT_Photon35_TwoProngs35                                          ||
            tree->HLT_Photon60_R9Id90_CaloIdL_IsoL                                  ||
            tree->HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL                     ||
            tree->HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15   ||
            tree->HLT_Photon20_HoverELoose                                          ||
            tree->HLT_Photon30_HoverELoose                                          ||
            tree->HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50    ||
            tree->HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3            ||
            tree->HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3           ;
        }
        if(year.Contains("2023")){
		passTrig =
            tree->HLT_Photon300_NoHE                                     ||
            tree->HLT_Photon33                                           ||
            tree->HLT_Photon50                                           ||
            tree->HLT_Photon75                                           ||
            tree->HLT_Photon90                                           ||
            tree->HLT_Photon120                                          ||
            tree->HLT_Photon150                                          ||
            tree->HLT_Photon175                                          ||
            tree->HLT_Photon200                                          ||
            tree->HLT_Photon30EB_TightID_TightIso                        ||
            tree->HLT_Photon50EB_TightID_TightIso                        ||
            tree->HLT_Photon75EB_TightID_TightIso                        ||
            tree->HLT_Photon90EB_TightID_TightIso                        ||
            tree->HLT_Photon110EB_TightID_TightIso                       ||
            tree->HLT_Photon130EB_TightID_TightIso                       ||
            tree->HLT_Photon150EB_TightID_TightIso                       ||
            tree->HLT_Photon175EB_TightID_TightIso                       ||
            tree->HLT_Photon200EB_TightID_TightIso                       ||
            tree->HLT_Photon100EBHE10                                    ||
            tree->HLT_Photon50_R9Id90_HE10_IsoM                          ||
            tree->HLT_Photon75_R9Id90_HE10_IsoM                          ||
            tree->HLT_Photon90_R9Id90_HE10_IsoM                          ||
            tree->HLT_Photon120_R9Id90_HE10_IsoM                         ||
            tree->HLT_Photon165_R9Id90_HE10_IsoM                         ||
            tree->HLT_Photon35_TwoProngs35                               ||
            tree->HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350  ||
            tree->HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380  ||
            tree->HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400  ||
            tree->HLT_Photon20_HoverELoose                               ||
            tree->HLT_Photon30_HoverELoose                               ||
            tree->HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3 ||
            tree->HLT_Photon32_OneProng32_M50To105                       ;
        }
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
