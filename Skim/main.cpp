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
    std::string fileDefault = "input/json/FilesNano_2022_GamJet.json";//default file
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
                std::cout << "Usage: ./makeSkim -o sKey_Skim_1of100.root\n" << std::endl;
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
    cout<<"\n./makeSkim -o " <<outName<<endl;

    cout<<"\n--------------------------------------"<<endl;
    cout<<" Set and load NanoTree"<<endl;
    cout<<"--------------------------------------"<<endl;
    NanoTree *nanoT = new NanoTree();
    nanoT->setInput(outName);
    nanoT->loadInput();
    nanoT->setInputJsonPath("input/json/");
    nanoT->loadInputJson();
    nanoT->loadJobFileNames();
    nanoT->loadTree();

    TString oName = outName;
	std::string outDir = "output";
    mkdir(outDir.c_str(), S_IRWXU);
    cout << "new output file name: "<< outDir+"/"+oName << endl;
	TFile* outFile = TFile::Open( outDir+"/"+oName ,"RECREATE");
	TTree* newTree = nanoT->fChain->GetTree()->CloneTree(0);
	newTree->SetCacheSize(50*1024*1024);
	Long64_t nEntr = nanoT->GetEntries();

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
            nanoT->b_HLT_Photon300_NoHE                                                ->GetEntry(entry);
            nanoT->b_HLT_Photon20                                                      ->GetEntry(entry);
            nanoT->b_HLT_Photon33                                                      ->GetEntry(entry);
            nanoT->b_HLT_Photon50                                                      ->GetEntry(entry);
            nanoT->b_HLT_Photon75                                                      ->GetEntry(entry);
            nanoT->b_HLT_Photon90                                                      ->GetEntry(entry);
            nanoT->b_HLT_Photon120                                                     ->GetEntry(entry);
            nanoT->b_HLT_Photon150                                                     ->GetEntry(entry);
            nanoT->b_HLT_Photon175                                                     ->GetEntry(entry);
            nanoT->b_HLT_Photon200                                                     ->GetEntry(entry);
            //nanoT->b_HLT_Photon30EB_TightID_TightIso                                   ->GetEntry(entry);
            //nanoT->b_HLT_Photon100EB_TightID_TightIso                                  ->GetEntry(entry);
            nanoT->b_HLT_Photon110EB_TightID_TightIso                                  ->GetEntry(entry);
            //nanoT->b_HLT_Photon120EB_TightID_TightIso                                  ->GetEntry(entry);
            nanoT->b_HLT_Photon100EBHE10                                               ->GetEntry(entry);
            //nanoT->b_HLT_Photon100EEHE10                                               ->GetEntry(entry);
            //nanoT->b_HLT_Photon100EE_TightID_TightIso                                  ->GetEntry(entry);
            nanoT->b_HLT_Photon50_R9Id90_HE10_IsoM                                     ->GetEntry(entry);
            nanoT->b_HLT_Photon75_R9Id90_HE10_IsoM                                     ->GetEntry(entry);
            //nanoT->b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3 ->GetEntry(entry);
            //nanoT->b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3 ->GetEntry(entry);
            nanoT->b_HLT_Photon90_R9Id90_HE10_IsoM                                     ->GetEntry(entry);
            nanoT->b_HLT_Photon120_R9Id90_HE10_IsoM                                    ->GetEntry(entry);
            nanoT->b_HLT_Photon165_R9Id90_HE10_IsoM                                    ->GetEntry(entry);
            nanoT->b_HLT_Photon35_TwoProngs35                                          ->GetEntry(entry);
            //nanoT->b_HLT_Photon60_R9Id90_CaloIdL_IsoL                                  ->GetEntry(entry);
            //nanoT->b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL                     ->GetEntry(entry);
            nanoT->b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15   ->GetEntry(entry);
            nanoT->b_HLT_Photon20_HoverELoose                                          ->GetEntry(entry);
            nanoT->b_HLT_Photon30_HoverELoose                                          ->GetEntry(entry);
            //nanoT->b_HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50    ->GetEntry(entry);
            nanoT->b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3            ->GetEntry(entry);
            //nanoT->b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3            ->GetEntry(entry);

		    passTrig =
            nanoT->HLT_Photon300_NoHE                                                ||
            nanoT->HLT_Photon20                                                      ||
            nanoT->HLT_Photon33                                                      ||
            nanoT->HLT_Photon50                                                      ||
            nanoT->HLT_Photon75                                                      ||
            nanoT->HLT_Photon90                                                      ||
            nanoT->HLT_Photon120                                                     ||
            nanoT->HLT_Photon150                                                     ||
            nanoT->HLT_Photon175                                                     ||
            nanoT->HLT_Photon200                                                     ||
            nanoT->HLT_Photon30EB_TightID_TightIso                                   ||
            nanoT->HLT_Photon100EB_TightID_TightIso                                  ||
            nanoT->HLT_Photon110EB_TightID_TightIso                                  ||
            nanoT->HLT_Photon120EB_TightID_TightIso                                  ||
            nanoT->HLT_Photon100EBHE10                                               ||
            nanoT->HLT_Photon100EEHE10                                               ||
            nanoT->HLT_Photon100EE_TightID_TightIso                                  ||
            nanoT->HLT_Photon50_R9Id90_HE10_IsoM                                     ||
            nanoT->HLT_Photon75_R9Id90_HE10_IsoM                                     ||
            nanoT->HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3 ||
            nanoT->HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3 ||
            nanoT->HLT_Photon90_R9Id90_HE10_IsoM                                     ||
            nanoT->HLT_Photon120_R9Id90_HE10_IsoM                                    ||
            nanoT->HLT_Photon165_R9Id90_HE10_IsoM                                    ||
            nanoT->HLT_Photon35_TwoProngs35                                          ||
            nanoT->HLT_Photon60_R9Id90_CaloIdL_IsoL                                  ||
            nanoT->HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL                     ||
            nanoT->HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15   ||
            nanoT->HLT_Photon20_HoverELoose                                          ||
            nanoT->HLT_Photon30_HoverELoose                                          ||
            nanoT->HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50    ||
            nanoT->HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3            ||
            nanoT->HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3           ;
        }
        if(oName.Contains("2023") && oName.Contains("GamJet")){ 
            nanoT->b_HLT_Photon300_NoHE                                     ->GetEntry(entry);
            nanoT->b_HLT_Photon33                                           ->GetEntry(entry);
            nanoT->b_HLT_Photon50                                           ->GetEntry(entry);
            nanoT->b_HLT_Photon75                                           ->GetEntry(entry);
            nanoT->b_HLT_Photon90                                           ->GetEntry(entry);
            nanoT->b_HLT_Photon120                                          ->GetEntry(entry);
            nanoT->b_HLT_Photon150                                          ->GetEntry(entry);
            nanoT->b_HLT_Photon175                                          ->GetEntry(entry);
            nanoT->b_HLT_Photon200                                          ->GetEntry(entry);
            nanoT->b_HLT_Photon30EB_TightID_TightIso                        ->GetEntry(entry);
            nanoT->b_HLT_Photon50EB_TightID_TightIso                        ->GetEntry(entry);
            nanoT->b_HLT_Photon75EB_TightID_TightIso                        ->GetEntry(entry);
            nanoT->b_HLT_Photon90EB_TightID_TightIso                        ->GetEntry(entry);
            nanoT->b_HLT_Photon110EB_TightID_TightIso                       ->GetEntry(entry);
            nanoT->b_HLT_Photon130EB_TightID_TightIso                       ->GetEntry(entry);
            nanoT->b_HLT_Photon150EB_TightID_TightIso                       ->GetEntry(entry);
            nanoT->b_HLT_Photon175EB_TightID_TightIso                       ->GetEntry(entry);
            nanoT->b_HLT_Photon200EB_TightID_TightIso                       ->GetEntry(entry);
            nanoT->b_HLT_Photon100EBHE10                                    ->GetEntry(entry);
            nanoT->b_HLT_Photon50_R9Id90_HE10_IsoM                          ->GetEntry(entry);
            nanoT->b_HLT_Photon75_R9Id90_HE10_IsoM                          ->GetEntry(entry);
            nanoT->b_HLT_Photon90_R9Id90_HE10_IsoM                          ->GetEntry(entry);
            nanoT->b_HLT_Photon120_R9Id90_HE10_IsoM                         ->GetEntry(entry);
            nanoT->b_HLT_Photon165_R9Id90_HE10_IsoM                         ->GetEntry(entry);
            nanoT->b_HLT_Photon35_TwoProngs35                               ->GetEntry(entry);
            nanoT->b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350  ->GetEntry(entry);
            nanoT->b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380  ->GetEntry(entry);
            nanoT->b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400  ->GetEntry(entry);
            nanoT->b_HLT_Photon20_HoverELoose                               ->GetEntry(entry);
            nanoT->b_HLT_Photon30_HoverELoose                               ->GetEntry(entry);
            nanoT->b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3 ->GetEntry(entry);
            nanoT->b_HLT_Photon32_OneProng32_M50To105                       ->GetEntry(entry);

		    passTrig =
            nanoT->HLT_Photon300_NoHE                                     ||
            nanoT->HLT_Photon33                                           ||
            nanoT->HLT_Photon50                                           ||
            nanoT->HLT_Photon75                                           ||
            nanoT->HLT_Photon90                                           ||
            nanoT->HLT_Photon120                                          ||
            nanoT->HLT_Photon150                                          ||
            nanoT->HLT_Photon175                                          ||
            nanoT->HLT_Photon200                                          ||
            nanoT->HLT_Photon30EB_TightID_TightIso                        ||
            nanoT->HLT_Photon50EB_TightID_TightIso                        ||
            nanoT->HLT_Photon75EB_TightID_TightIso                        ||
            nanoT->HLT_Photon90EB_TightID_TightIso                        ||
            nanoT->HLT_Photon110EB_TightID_TightIso                       ||
            nanoT->HLT_Photon130EB_TightID_TightIso                       ||
            nanoT->HLT_Photon150EB_TightID_TightIso                       ||
            nanoT->HLT_Photon175EB_TightID_TightIso                       ||
            nanoT->HLT_Photon200EB_TightID_TightIso                       ||
            nanoT->HLT_Photon100EBHE10                                    ||
            nanoT->HLT_Photon50_R9Id90_HE10_IsoM                          ||
            nanoT->HLT_Photon75_R9Id90_HE10_IsoM                          ||
            nanoT->HLT_Photon90_R9Id90_HE10_IsoM                          ||
            nanoT->HLT_Photon120_R9Id90_HE10_IsoM                         ||
            nanoT->HLT_Photon165_R9Id90_HE10_IsoM                         ||
            nanoT->HLT_Photon35_TwoProngs35                               ||
            nanoT->HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350  ||
            nanoT->HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380  ||
            nanoT->HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400  ||
            nanoT->HLT_Photon20_HoverELoose                               ||
            nanoT->HLT_Photon30_HoverELoose                               ||
            nanoT->HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3 ||
            nanoT->HLT_Photon32_OneProng32_M50To105                       ;
        }

        //DiEleJet
        if(oName.Contains("DiEleJet")){
		    nanoT->b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ->GetEntry(entry);
		    passTrig = nanoT->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
        }

        //DiMuJet
        if(oName.Contains("DiMuJet")){
		    nanoT->b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8->GetEntry(entry);
		    passTrig = nanoT->HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8;
        }

        //DiJet
        if(oName.Contains("DiJet")){
            nanoT->b_HLT_PFJet40             ->GetEntry(entry);
            nanoT->b_HLT_PFJet60             ->GetEntry(entry);
            nanoT->b_HLT_PFJet80             ->GetEntry(entry);
            nanoT->b_HLT_PFJet140            ->GetEntry(entry);
            nanoT->b_HLT_PFJet200            ->GetEntry(entry);
            nanoT->b_HLT_PFJet260            ->GetEntry(entry);
            nanoT->b_HLT_PFJet320            ->GetEntry(entry);
            nanoT->b_HLT_PFJet400            ->GetEntry(entry);
            nanoT->b_HLT_PFJet450            ->GetEntry(entry);
            nanoT->b_HLT_PFJet500            ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve40        ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve60        ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve80        ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve140       ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve200       ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve260       ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve320       ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve400       ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve500       ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve60_HFJEC  ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve80_HFJEC  ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve100_HFJEC ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve160_HFJEC ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve220_HFJEC ->GetEntry(entry);
            nanoT->b_HLT_DiPFJetAve300_HFJEC ->GetEntry(entry);

		    passTrig =
            nanoT->HLT_PFJet40             ||
            nanoT->HLT_PFJet60             ||
            nanoT->HLT_PFJet80             ||
            nanoT->HLT_PFJet140            ||
            nanoT->HLT_PFJet200            ||
            nanoT->HLT_PFJet260            ||
            nanoT->HLT_PFJet320            ||
            nanoT->HLT_PFJet400            ||
            nanoT->HLT_PFJet450            ||
            nanoT->HLT_PFJet500            ||
            nanoT->HLT_DiPFJetAve40        ||
            nanoT->HLT_DiPFJetAve60        ||
            nanoT->HLT_DiPFJetAve80        ||
            nanoT->HLT_DiPFJetAve140       ||
            nanoT->HLT_DiPFJetAve200       ||
            nanoT->HLT_DiPFJetAve260       ||
            nanoT->HLT_DiPFJetAve320       ||
            nanoT->HLT_DiPFJetAve400       ||
            nanoT->HLT_DiPFJetAve500       ||
            nanoT->HLT_DiPFJetAve60_HFJEC  ||
            nanoT->HLT_DiPFJetAve80_HFJEC  ||
            nanoT->HLT_DiPFJetAve100_HFJEC ||
            nanoT->HLT_DiPFJetAve160_HFJEC ||
            nanoT->HLT_DiPFJetAve220_HFJEC ||
            nanoT->HLT_DiPFJetAve300_HFJEC ;
        }
		if(passTrig){
		    nanoT->GetEntry(entry);
            hEvents_->Fill(1);
			newTree->Fill();
		}
	}
    std::cout<<"nEvents_Skim = "<<newTree->GetEntries()<<endl;
    outFile->cd();
    newTree->Write();
	hEvents_->Write();
	outFile->Close();

	return 0;
}
