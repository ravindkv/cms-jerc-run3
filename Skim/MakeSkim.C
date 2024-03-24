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

#include "NanoTree.h"
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <string>
#include <sstream>

#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[]){
    std::string fileDefault = "input/json/FilesNano_2023_GamJet.json";//default file
    std::ifstream fileDefault_(fileDefault.c_str());
    nlohmann::json js; 
    try{
        js = nlohmann::json::parse(fileDefault_);
    } catch (const std::exception& e) {
        cout<<"\nEXCEPTION: Check the input json fileDefault: "<<fileDefault<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }

    //--------------------------------
    // Parse command-line options
    //--------------------------------
    int opt;
    std::string outName; //output Name
    outName = js.begin().key()+"_Skim_1of100.root"; //defualt value
    while ((opt = getopt(argc, argv, "o:h")) != -1) {
        switch (opt) {
            case 'o':
                outName = optarg;
                break;
            case 'h':
                cout<<"Default input json: "<<fileDefault<<endl;
                std::cout << "Usage: ./runMakeSkim -o sKey_Skim_1of100.root\n" << std::endl;
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
    cout<<"Inputs: ./runMakeSkim -o " <<outName<<endl;
    cout<<"--------------------------------------------"<<endl;
    // outName = sKey_Skim_nofN.root
    std::string sKey;
    std::string n;
    std::string N;
	NanoTree* tree;
    try{
        std::vector<std::string> v_outName      = tree->splitString(outName, "_Skim_");
        sKey = v_outName.at(0); 
        std::cout << "sKey: " << sKey << std::endl;
        std::string nofN_root   = v_outName.at(1); 
        std::vector<std::string> v_nofN_root    = tree->splitString(nofN_root, ".root"); 
        std::string nofN        = v_nofN_root.at(0); 
        std::cout << "nofN: " << nofN << std::endl;
        std::vector<std::string> v_nofN         = tree->splitString(nofN, "of"); 
        n = v_nofN.at(0);
        N = v_nofN.at(1);
    }catch(const std::exception &e){
        cout<<"\nEXCEPTION: Check the outName: "<<outName<<endl;
        cout<<"outName format should be: DataOrMC_Year_Channel_Sample_Skim_nofN.root"<<endl;
        cout<<"Run ./runMakeSkim -h for more details"<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }

    int nthJob = std::stoi(n);
    int totJob = std::stoi(N);
    TString oName = outName;

    //--------------------------------
    // files to run for each job
    //--------------------------------
    std::string fileName;
    std::string year = "2022";
    if(oName.Contains("2023")) year = "2023";
    std::string channel = tree->splitString(sKey, "_").at(2);
    fileName = "input/json/FilesNano_"+year+"_"+channel+".json";
    cout<<"json: "<<fileName<<endl;
    std::ifstream fileName_(fileName.c_str());
    try{
        js = nlohmann::json::parse(fileName_);
    } catch (const std::exception& e) {
        cout<<"\nEXCEPTION: Check the input json fileName: "<<fileName<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }
    
    std::vector<std::string> fileNames;
    try{
        js.at(sKey).get_to(fileNames);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: Check the sKey: "<<sKey<<endl;
        cout<<e.what()<<endl;
        cout<<"Choose sKey from the following:"<<endl;
        for (auto& element : js.items()) {
            std::cout << element.key() << std::endl;
        }
        std::abort();
    }
    
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
	}
    else{
        cout<<"\n Error: Make sure nthJob > 0 and totJob > 0\n ";
        return 0;
    }

    //--------------------------------
    // Read input files
    //--------------------------------
    std::vector<std::vector<std::string>> smallVectors = tree->splitVector(fileNames, totJob);
	tree = new NanoTree(oName, smallVectors[nthJob-1]);

	std::string outDir = "output";
    mkdir(outDir.c_str(), S_IRWXU);
    cout << "new output file name: "<< outDir+"/"+oName << endl;
	TFile* outFile = TFile::Open( outDir+"/"+oName ,"RECREATE");
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
        //if(entry>100000) break;
		if(endEntry > 100  && entry%(eventsPerJob/100) == 0){// print after every 1% of events
            totalTime+= std::chrono::duration<double>(std::chrono::high_resolution_clock::now()-startClock).count();
            int sec = (int)(totalTime)%60;
            int min = (int)(totalTime)/60;
	        std::cout<<setw(10)<<100*entry/endEntry<<" %"<<setw(10)<<min<<"m "<<sec<<"s"<<std::endl;
			startClock = std::chrono::high_resolution_clock::now();
		}
		hEvents_->Fill(0);
        //GamJet
        if(oName.Contains("2022") && oName.Contains("GamJet")){ 
            tree->b_HLT_Photon300_NoHE                                                ->GetEntry(entry);
            tree->b_HLT_Photon20                                                      ->GetEntry(entry);
            tree->b_HLT_Photon33                                                      ->GetEntry(entry);
            tree->b_HLT_Photon50                                                      ->GetEntry(entry);
            tree->b_HLT_Photon75                                                      ->GetEntry(entry);
            tree->b_HLT_Photon90                                                      ->GetEntry(entry);
            tree->b_HLT_Photon120                                                     ->GetEntry(entry);
            tree->b_HLT_Photon150                                                     ->GetEntry(entry);
            tree->b_HLT_Photon175                                                     ->GetEntry(entry);
            tree->b_HLT_Photon200                                                     ->GetEntry(entry);
            tree->b_HLT_Photon30EB_TightID_TightIso                                   ->GetEntry(entry);
            tree->b_HLT_Photon100EB_TightID_TightIso                                  ->GetEntry(entry);
            tree->b_HLT_Photon110EB_TightID_TightIso                                  ->GetEntry(entry);
            tree->b_HLT_Photon120EB_TightID_TightIso                                  ->GetEntry(entry);
            tree->b_HLT_Photon100EBHE10                                               ->GetEntry(entry);
            tree->b_HLT_Photon100EEHE10                                               ->GetEntry(entry);
            tree->b_HLT_Photon100EE_TightID_TightIso                                  ->GetEntry(entry);
            tree->b_HLT_Photon50_R9Id90_HE10_IsoM                                     ->GetEntry(entry);
            tree->b_HLT_Photon75_R9Id90_HE10_IsoM                                     ->GetEntry(entry);
            tree->b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3 ->GetEntry(entry);
            tree->b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3 ->GetEntry(entry);
            tree->b_HLT_Photon90_R9Id90_HE10_IsoM                                     ->GetEntry(entry);
            tree->b_HLT_Photon120_R9Id90_HE10_IsoM                                    ->GetEntry(entry);
            tree->b_HLT_Photon165_R9Id90_HE10_IsoM                                    ->GetEntry(entry);
            tree->b_HLT_Photon35_TwoProngs35                                          ->GetEntry(entry);
            tree->b_HLT_Photon60_R9Id90_CaloIdL_IsoL                                  ->GetEntry(entry);
            tree->b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL                     ->GetEntry(entry);
            tree->b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15   ->GetEntry(entry);
            tree->b_HLT_Photon20_HoverELoose                                          ->GetEntry(entry);
            tree->b_HLT_Photon30_HoverELoose                                          ->GetEntry(entry);
            tree->b_HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50    ->GetEntry(entry);
            tree->b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3            ->GetEntry(entry);
            tree->b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3            ->GetEntry(entry);

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
        if(oName.Contains("2023") && oName.Contains("GamJet")){ 
            tree->b_HLT_Photon300_NoHE                                     ->GetEntry(entry);
            tree->b_HLT_Photon33                                           ->GetEntry(entry);
            tree->b_HLT_Photon50                                           ->GetEntry(entry);
            tree->b_HLT_Photon75                                           ->GetEntry(entry);
            tree->b_HLT_Photon90                                           ->GetEntry(entry);
            tree->b_HLT_Photon120                                          ->GetEntry(entry);
            tree->b_HLT_Photon150                                          ->GetEntry(entry);
            tree->b_HLT_Photon175                                          ->GetEntry(entry);
            tree->b_HLT_Photon200                                          ->GetEntry(entry);
            tree->b_HLT_Photon30EB_TightID_TightIso                        ->GetEntry(entry);
            tree->b_HLT_Photon50EB_TightID_TightIso                        ->GetEntry(entry);
            tree->b_HLT_Photon75EB_TightID_TightIso                        ->GetEntry(entry);
            tree->b_HLT_Photon90EB_TightID_TightIso                        ->GetEntry(entry);
            tree->b_HLT_Photon110EB_TightID_TightIso                       ->GetEntry(entry);
            tree->b_HLT_Photon130EB_TightID_TightIso                       ->GetEntry(entry);
            tree->b_HLT_Photon150EB_TightID_TightIso                       ->GetEntry(entry);
            tree->b_HLT_Photon175EB_TightID_TightIso                       ->GetEntry(entry);
            tree->b_HLT_Photon200EB_TightID_TightIso                       ->GetEntry(entry);
            tree->b_HLT_Photon100EBHE10                                    ->GetEntry(entry);
            tree->b_HLT_Photon50_R9Id90_HE10_IsoM                          ->GetEntry(entry);
            tree->b_HLT_Photon75_R9Id90_HE10_IsoM                          ->GetEntry(entry);
            tree->b_HLT_Photon90_R9Id90_HE10_IsoM                          ->GetEntry(entry);
            tree->b_HLT_Photon120_R9Id90_HE10_IsoM                         ->GetEntry(entry);
            tree->b_HLT_Photon165_R9Id90_HE10_IsoM                         ->GetEntry(entry);
            tree->b_HLT_Photon35_TwoProngs35                               ->GetEntry(entry);
            tree->b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350  ->GetEntry(entry);
            tree->b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380  ->GetEntry(entry);
            tree->b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400  ->GetEntry(entry);
            tree->b_HLT_Photon20_HoverELoose                               ->GetEntry(entry);
            tree->b_HLT_Photon30_HoverELoose                               ->GetEntry(entry);
            tree->b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3 ->GetEntry(entry);
            tree->b_HLT_Photon32_OneProng32_M50To105                       ->GetEntry(entry);

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

        //DiEleJet
        if(oName.Contains("DiEleJet")){
		    tree->b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ->GetEntry(entry);
		    passTrig = tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
        }

        //DiMuJet
        if(oName.Contains("DiMuJet")){
		    tree->b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8->GetEntry(entry);
		    passTrig = tree->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8;
        }

        //DiJet
        if(oName.Contains("DiJet")){
            tree->b_HLT_PFJet40             ->GetEntry(entry);
            tree->b_HLT_PFJet60             ->GetEntry(entry);
            tree->b_HLT_PFJet80             ->GetEntry(entry);
            tree->b_HLT_PFJet140            ->GetEntry(entry);
            tree->b_HLT_PFJet200            ->GetEntry(entry);
            tree->b_HLT_PFJet260            ->GetEntry(entry);
            tree->b_HLT_PFJet320            ->GetEntry(entry);
            tree->b_HLT_PFJet400            ->GetEntry(entry);
            tree->b_HLT_PFJet450            ->GetEntry(entry);
            tree->b_HLT_PFJet500            ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve40        ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve60        ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve80        ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve140       ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve200       ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve260       ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve320       ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve400       ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve500       ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve60_HFJEC  ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve80_HFJEC  ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve100_HFJEC ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve160_HFJEC ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve220_HFJEC ->GetEntry(entry);
            tree->b_HLT_DiPFJetAve300_HFJEC ->GetEntry(entry);

		    passTrig =
            tree->HLT_PFJet40             ||
            tree->HLT_PFJet60             ||
            tree->HLT_PFJet80             ||
            tree->HLT_PFJet140            ||
            tree->HLT_PFJet200            ||
            tree->HLT_PFJet260            ||
            tree->HLT_PFJet320            ||
            tree->HLT_PFJet400            ||
            tree->HLT_PFJet450            ||
            tree->HLT_PFJet500            ||
            tree->HLT_DiPFJetAve40        ||
            tree->HLT_DiPFJetAve60        ||
            tree->HLT_DiPFJetAve80        ||
            tree->HLT_DiPFJetAve140       ||
            tree->HLT_DiPFJetAve200       ||
            tree->HLT_DiPFJetAve260       ||
            tree->HLT_DiPFJetAve320       ||
            tree->HLT_DiPFJetAve400       ||
            tree->HLT_DiPFJetAve500       ||
            tree->HLT_DiPFJetAve60_HFJEC  ||
            tree->HLT_DiPFJetAve80_HFJEC  ||
            tree->HLT_DiPFJetAve100_HFJEC ||
            tree->HLT_DiPFJetAve160_HFJEC ||
            tree->HLT_DiPFJetAve220_HFJEC ||
            tree->HLT_DiPFJetAve300_HFJEC ;
        }
		if(passTrig){
		    tree->GetEntry(entry);
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
