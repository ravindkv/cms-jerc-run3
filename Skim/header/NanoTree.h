#ifndef NANOTREE_H
#define NANOTREE_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <filesystem>

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TMath.h>
#include <nlohmann/json.hpp>

#include "GlobalFlag.h"

const Int_t MAX_PARTICLES = 600;

class NanoTree { 
public:
    explicit NanoTree(GlobalFlag& globalFlags);
    ~NanoTree();

    Long64_t getEntries() const;
    Long64_t getEntriesRuns() const;
    Int_t getEntry(Long64_t entry);
    Long64_t loadEntry(Long64_t entry);
    Long64_t loadEntryRuns(Long64_t entry);

    // Input management
    void setInput(const std::string& outputName);
    void loadInput();

    // JSON management
    void loadInputJson(const std::string& searchDir);

    // File loading
    void loadJobFileNames();
    void loadTree();

    std::vector<std::string> filterList;
    std::map<std::string, Bool_t> filterVals;
    std::map<std::string, TBranch*> filterTBranches;

    TChain* fChain = new TChain("Events");   
    TChain* fChainRuns = new TChain("Runs");   

private:
    // Member variables
    GlobalFlag& globalFlags_;
    Int_t fCurrent_{-1}; 
    Int_t fCurrentRuns_{-1}; 

    UInt_t run_{};
    ULong64_t event_{};
    UInt_t lumis_{};

    std::string outputName_;
    std::string loadedSampleKey_ = "MC_Year_Channel_Name";
    std::string inputJsonPath_ = "./FilesNano_2022_GamJet.json";
    std::size_t loadedNthJob_ = 1;
    std::size_t loadedTotalJobs_ = 100;

    std::vector<std::string> loadedAllFileNames_;
    std::vector<std::string> loadedJobFileNames_;
};

#endif // NANOTREE_H

