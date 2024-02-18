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

class NanoTree{
 public:
    NanoTree(TString year, vector<string> fileNames, bool isMC);
    ~NanoTree();
    Long64_t GetEntries();
    Int_t GetEntry(Long64_t entry);
    TChain* chain;
    
    UInt_t    run_;
    ULong64_t event_;
    UInt_t    lumis_;
    //-----------------------------
    // Branch values
    //-----------------------------
    //2022
    Bool_t HLT_Photon300_NoHE = false;
    Bool_t HLT_Photon20 = false;
    Bool_t HLT_Photon33 = false;
    Bool_t HLT_Photon50 = false;
    Bool_t HLT_Photon75 = false;
    Bool_t HLT_Photon90 = false;
    Bool_t HLT_Photon120 = false;
    Bool_t HLT_Photon150 = false;
    Bool_t HLT_Photon175 = false;
    Bool_t HLT_Photon200 = false;
    Bool_t HLT_Photon30EB_TightID_TightIso = false;
    Bool_t HLT_Photon100EB_TightID_TightIso = false;
    Bool_t HLT_Photon110EB_TightID_TightIso = false;
    Bool_t HLT_Photon120EB_TightID_TightIso = false;
    Bool_t HLT_Photon100EBHE10 = false;
    Bool_t HLT_Photon100EEHE10 = false;
    Bool_t HLT_Photon100EE_TightID_TightIso = false;
    Bool_t HLT_Photon50_R9Id90_HE10_IsoM = false;
    Bool_t HLT_Photon75_R9Id90_HE10_IsoM = false;
    Bool_t HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3 = false;
    Bool_t HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3 = false;
    Bool_t HLT_Photon90_R9Id90_HE10_IsoM = false;
    Bool_t HLT_Photon120_R9Id90_HE10_IsoM = false;
    Bool_t HLT_Photon165_R9Id90_HE10_IsoM = false;
    Bool_t HLT_Photon35_TwoProngs35 = false;
    Bool_t HLT_Photon60_R9Id90_CaloIdL_IsoL = false;
    Bool_t HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL = false;
    Bool_t HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15 = false;
    Bool_t HLT_Photon20_HoverELoose = false;
    Bool_t HLT_Photon30_HoverELoose = false;
    Bool_t HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50 = false;
    Bool_t HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3 = false;
    Bool_t HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3 = false;

    //additional for 2023
    Bool_t HLT_Photon50EB_TightID_TightIso = false;
    Bool_t HLT_Photon75EB_TightID_TightIso = false;
    Bool_t HLT_Photon90EB_TightID_TightIso = false;
    Bool_t HLT_Photon130EB_TightID_TightIso = false;
    Bool_t HLT_Photon150EB_TightID_TightIso = false;
    Bool_t HLT_Photon175EB_TightID_TightIso = false;
    Bool_t HLT_Photon200EB_TightID_TightIso = false;
    Bool_t HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350 = false;
    Bool_t HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380 = false;
    Bool_t HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400 = false;
    Bool_t HLT_Photon32_OneProng32_M50To105 = false;

    std::vector<std::vector<std::string>> splitVector(const std::vector<std::string>& strings, int n);
    std::vector<int> getJobs(std::string jobStr="1of100");
};

#endif
