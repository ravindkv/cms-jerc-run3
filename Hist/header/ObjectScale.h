#ifndef OBJECTSCALE_H
#define OBJECTSCALE_H
        
#include<vector>
#include<iostream>
#include<algorithm>
#include<TH1F.h>
#include<TMath.h>
#include<TLorentzVector.h>
#include "SkimTree.h"
#include "correction.h"

#include "TH1D.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TChain.h"
#include "TFile.h"

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <utility> // std::pair

class ObjectScale{
    public: 
	ObjectScale(){
        debug  = false;
        isData = false;
    };
    ~ObjectScale();
    void applyJEC(SkimTree* tree, correction::CompoundCorrection::Ref jesRefSF, correction::Correction::Ref jesRefUnc, string systVar);

    // Jet energy scale: (L2L3)
    vector<string> getL2L3Names(TString oName);
    // Jet veto
    string getJvName(TString oName);
    string getJvKey(TString oName);
    
    void PrintInfo(string info, bool printcout);
    std::map<int, std::map<int, int>> LoadJSON(string json);
    map<string, map<int, double> >  LoadLumi(std::vector<std::string> &eras, map<string, vector<string> > &trigs);
    map<string, map<int, TH1D*> > LoadPU(std::vector<std::string> &eras, map<string, vector<string> > &trigs);
    
    void setIsData(bool value);

    struct lumiInfo {
      double lum;
      double muavg;
      double murms;
      lumiInfo(double lum_=0, double muavg_=0, double murms_=0)
        : lum(lum_), muavg(muavg_), murms(murms_) {};
    };
    map<int, map<int, lumiInfo> > parsePileUpJSON(string filename, string json, double minbXsec);
    double getTruePU(map<int, map<int, lumiInfo> >_mus, int run, int ls, double *rms);

    private:
    bool debug ;
    bool isData;
};


#endif
