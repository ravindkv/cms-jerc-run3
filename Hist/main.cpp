#include "HistGamJet.h"
#include "HistDiJet.h"
#include "HistDiEleJet.h"
#include "SkimTree.h"
#include "ObjectScale.h"
#include "GlobalFlag.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

int main(int argc, char* argv[]){
    string fileDefault = "input/json/FilesSkim_2024_DiEleJet.json";//default file
    ifstream fileDefault_(fileDefault.c_str());
    nlohmann::json js; 
    try{
        js = nlohmann::json::parse(fileDefault_);
    } catch (const exception& e) {
        cout<<"\nEXCEPTION: Check the input json fileDefault: "<<fileDefault<<endl;
        cout<<e.what()<<endl;
        abort();
    }
    //--------------------------------
    // Parse command-line options
    //--------------------------------
    int opt;
    string outName; //output Name
    outName = js.begin().key()+"_Hist_1of100.root"; //defualt value
    while ((opt = getopt(argc, argv, "o:h")) != -1) {
        switch (opt) {
            case 'o':
                outName = optarg;
                break;
            case 'h':
                cout<<"Default input json: "<<fileDefault<<endl;
                cout << "Usage: ./makeHist -o sKey_Hist_1of100.root\n" << endl;
                cout<<"Choose sKey from the following:"<<endl;
                for (auto& element : js.items()) {
                    cout << element.key() << endl;
                }
                return 0;
            default:
                cerr << "Invalid option" << endl;
                return 1;
        }
    }
    cout<<"\n./makeHist -o " <<outName<<endl;
    TString oName = outName;

    cout<<"\n--------------------------------------"<<endl;
    cout<<" Set GlobalFlag.cpp"<<endl;
    cout<<"--------------------------------------"<<endl;
    GlobalFlag *globF =  new GlobalFlag(oName);
    globF->printFlag();

    cout<<"\n--------------------------------------"<<endl;
    cout<<" Set and load ObjectScale.cpp"<<endl;
    cout<<"--------------------------------------"<<endl;
    ObjectScale *objS = new ObjectScale(oName);
    //Jet veto 
    objS->setJetVetoKey(oName); 
    objS->setJetVetoName(oName); 
    objS->setJetVetoJsonPath(oName); 
    objS->loadJetVetoRef(); 

    //Jet L2L3 
    objS->setJetL2L3Names(oName); 
    objS->setJetL2L3JsonPath(oName); 
    objS->loadJetL2L3Refs(); 
    cout<<endl;

    if(globF->isGamJet){
        //Photon SS 
        objS->setPhoSsName(oName); 
        objS->setPhoSsJsonPath(oName); 
        objS->loadPhoSsRef(); 
        cout<<endl;
        
        //Electron SS 
        objS->setEleSsName(oName); 
        objS->setEleSsJsonPath(oName); 
        objS->loadEleSsRef(); 
        cout<<endl;
    }

    //Lumi
    if (globF->isData){
        objS->setLumiJsonPath(oName); 
        objS->loadLumiJson(); 
        cout<<endl;
    }

    //PU Text
    objS->setPuTextPath(oName); 
    objS->setPuMinbXsec(69200);
    objS->loadPuText(); 
    cout<<endl;

    //PU Hist
    objS->setPuHistPath(oName); 
    objS->setPuHistEras(oName); 
    objS->setPuHistTrigs(oName); 
    objS->loadPuHist(); 

    cout<<"\n--------------------------------------"<<endl;
    cout<<" Set and load SkimTree.cpp"<<endl;
    cout<<"--------------------------------------"<<endl;
    SkimTree *skimT = new SkimTree(oName);
    skimT->setInput(outName);
    skimT->loadInput();
    cout<<endl;
    skimT->setInputJsonPath("input/json/");
    skimT->loadInputJson();
    cout<<endl;
    skimT->loadJobFileNames();
    cout<<endl;
    skimT->loadTree();
    cout<<endl;

    string outDir = "output";
    mkdir(outDir.c_str(), S_IRWXU);
    TFile *fout = new TFile(outDir+"/"+oName, "RECREATE");

    cout<<"\n--------------------------------------"<<endl;
    cout<<" Loop over events and fill Histos"<<endl;
    cout<<"--------------------------------------"<<endl;
    if(globF->isGamJet){
      cout<<"==> Running GamJet"<<endl;
      HistGamJet *gamJet = new HistGamJet(oName);
      gamJet->Run(oName,  skimT, objS, fout);  
    }

    if(globF->isDiEleJet){
      cout<<"==> Running DiEleJet"<<endl;
      HistDiEleJet *diEleJet = new HistDiEleJet(oName);
      diEleJet->Run(skimT, objS, fout);  
    }
    
    if(globF->isDiMuJet){
      cout<<"==> Running DiMuJet"<<endl;
      //HistDiMuJet *diMuJet = new HistDiMuJet(oName);
      //diMuJet->Run(oName, skimT, objS, fout);  
    }

    if(globF->isDiJet){
      cout<<"==> Running DiJet"<<endl;
      //HistDiJet *diJet = new HistDiJet(oName);
      //diJet->Run(oName, skimT, objS, fout);  
    }

  return 0;
}
