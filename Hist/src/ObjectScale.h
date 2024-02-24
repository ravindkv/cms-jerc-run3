#ifndef JECVARIATION_H
#define JECVARIATION_H
        
#include<vector>
#include<iostream>
#include<algorithm>
#include<TH1F.h>
#include<TMath.h>
#include<TLorentzVector.h>
#include "SkimTree.h"
#include "correction.h"

class ObjectScale{
    public: 
	ObjectScale();
    ~ObjectScale();
    void applyJEC(SkimTree* tree, correction::CompoundCorrection::Ref jesRefSF, correction::Correction::Ref jesRefUnc, string systVar);

    // Jet energy scale: (L2L3)
    vector<string> getL2L3Names(TString oName);
    // Jet veto
    string getJvName(TString oName);
    string getJvKey(TString oName);
    
    bool isData = false;
};


#endif
