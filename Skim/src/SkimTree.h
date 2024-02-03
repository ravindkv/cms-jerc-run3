#ifndef EVENTTREE_H
#define EVENTTREE_H

#include<iostream>
#include<TFile.h>
#include<TTree.h>
#include<TChain.h>
#include<TMath.h>
#include <map>

#include<vector>

using namespace std;
const Int_t maxP = 600;

class SkimTree{
 public:
    SkimTree(string year, vector<string> fileNames, bool isMC);
    ~SkimTree();
    Long64_t GetEntries();
    Int_t GetEntry(Long64_t entry);
    TChain* chain;
    
    UInt_t    run_;
    ULong64_t event_;
    UInt_t    lumis_;

    bool is16 = false;
    bool is17 = false;
    bool is18 = false;
    bool is22 = false;
    bool is23 = false;
    
    //2022
    Bool_t p20;
    Bool_t p20HE;
    Bool_t p30HE;
    Bool_t p33;
    Bool_t p200;

    std::vector<std::vector<std::string>> splitVector(const std::vector<std::string>& strings, int n);
};

#endif
