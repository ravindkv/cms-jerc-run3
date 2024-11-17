#ifndef NANOTREE_H
#define NANOTREE_H

#include<iostream>
#include <fstream>
#include<TFile.h>
#include<TTree.h>
#include<TChain.h>
#include<TMath.h>
#include <map>
#include <nlohmann/json.hpp>
#include <filesystem>

#include<vector>

using namespace std;
const Int_t maxP = 600;

class SkimTree{
 public:
    SkimTree();
    ~SkimTree();
    Long64_t GetEntries();
    Int_t GetEntry(Long64_t entry);
    Int_t           fCurrent; //!current Tree number in a TChain
    virtual Long64_t loadEntry(Long64_t entry);
    
    UInt_t    run;
    ULong64_t event;
    UInt_t    lumi;

    TBranch *b_run;
    TBranch *b_event;
    TBranch *b_lumi;

    //Inputs
    string iName;
    void setInput(string oName);
    void loadInput();
    string loadedSkimKey = "MC_Year_Channel_Name";
    string loadedFibKey = "MC_Year_Channel_Name";
    string beginRun = "1";
    string endRun = "1";
    int loadedNthJob = 1;
    int loadedTotJob =100;

    //Inputs json
    string inputSkimJson = "./FilesSkim_2022_GamJet.json";
    string inputFibJson = "./FilesFib_2022_GamJet.json";
    void setInputJsonPath(string inDir); 
    void loadInputJson();
    vector<string> loadedAllFileNames;

    //Skim files and tree
    vector<string> loadedJobFileNames;
    void loadJobFileNames();
    TChain *fChain = new TChain("Events");   
    void loadTree();
    std::vector<std::vector<std::string>> splitVector(const std::vector<std::string>& strings, int n);
    std::vector<std::string> splitString(const std::string& s, const std::string& delimiter);

};

#endif
