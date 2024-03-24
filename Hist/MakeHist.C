#include "HistGamJet.h"
#include "HistDiJet.h"
#include "HistDiEleJet.h"
#include "SkimTree.h"
#include "ObjectScale.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>

int main(int argc, char* argv[]){
    std::string fileDefault = "input/json/FilesSkim_2023_DiEleJet.json";//default file
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
    outName = js.begin().key()+"_Hist_1of100.root"; //defualt value
    while ((opt = getopt(argc, argv, "o:h")) != -1) {
        switch (opt) {
            case 'o':
                outName = optarg;
                break;
            case 'h':
                cout<<"Default input json: "<<fileDefault<<endl;
                std::cout << "Usage: ./runMakeHist -o sKey_Hist_1of100.root\n" << std::endl;
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
    cout<<"Inputs: ./runMakeHist -o " <<outName<<endl;
    cout<<"--------------------------------------------"<<endl;
    // outName = sKey_Hist_nofN.root
    std::string sKey;
    std::string n;
    std::string N;
    SkimTree *tree; 
    try{
        std::vector<std::string> v_outName      = tree->splitString(outName, "_Hist_");
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
        cout<<"outName format should be: DataOrMC_Year_Channel_Sample_Hist_nofN.rooot"<<endl;
        cout<<"Run ./runMakeHist -h for more details"<<endl;
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
    fileName = "input/json/FilesSkim_"+year+"_"+channel+".json";
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
        std::abort();
    }
	if (nthJob>0 && totJob>0){
	    cout <<"jobs: " <<nthJob << " of " << totJob << endl;
		cout << "Processing " << (1.*nFiles)/totJob << " files per job on average" << endl;
	    cout << "new output file name: "<< oName << endl;
	}
    else{
        cout<<"\n Error: Make sure nthJob > 0 and totJob > 0\n ";
        std::abort();
    }


  std::vector<std::vector<std::string>> smallVectors = tree->splitVector(fileNames, totJob);
  tree = new SkimTree(oName, smallVectors[nthJob-1]); 

  ObjectScale *objS;

  std::string outDir = "output";
  mkdir(outDir.c_str(), S_IRWXU);
  cout << "new output file name: "<< outDir+"/"+oName << endl;
  TFile *fout = new TFile(outDir+"/"+oName, "RECREATE");
  
  //--------------------------------------
  // Run to Fill GamJet Histos
  //--------------------------------------
  if(oName.Contains("GamJet")){
    HistGamJet *gamJet;
    gamJet->Run(oName, tree, objS, fout);  
  }
  //--------------------------------------
  // Run to Fill DiEleJet Histos
  //--------------------------------------
  //if(oName.Contains("DiEleJet")){
    HistDiEleJet *diEleJet;
    diEleJet->Run(oName, tree, objS, fout);  
  //}

  //--------------------------------------
  // Run to Fill DiMuJet Histos
  //--------------------------------------
  if(oName.Contains("DiMuJet")){
    //HistDiMuJet *diMuJet;
    //diMuJet->Run(oName, tree, objS, fout);  
  }
  //--------------------------------------
  // Run to Fill DiJet Histos
  //--------------------------------------
  if(oName.Contains("DiJet")){
    HistDiJet *diJet;
    diJet->Run(oName, tree, objS, fout);  
  }

  return 0;
}
