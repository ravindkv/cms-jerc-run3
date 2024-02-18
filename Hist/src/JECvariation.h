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

class JECvariation{
public: 
	JECvariation();
    ~JECvariation();
    void applyJEC(SkimTree* tree, correction::CompoundCorrection::Ref jesRefSF, correction::Correction::Ref jesRefUnc, string systVar);
};


#endif