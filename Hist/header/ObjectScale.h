#ifndef OBJECTSCALE_H
#define OBJECTSCALE_H
        
#include <set>
#include<iostream>
#include <fstream>

#include "SkimTree.h"
#include "correction.h"

#include "TH2D.h"
#include "TRandom.h"
#include "TCanvas.h"

class ObjectScale{
    public: 
        ObjectScale(){
            is22   = false;
            is23   = false;
            is24   = false;
            isData = false;
            debug  = false;
            jetVetoKey      ="jetvetomap";
            jetVetoName     = "Winter22Run3_RunCD_V1";
            jetVetoJsonPath = "./jetvetomaps.json";
            jetL2L3Names    = {};
            jetL2L3JsonPath = "./jet_jerc.json";
            phoSsName       = "2022Re-recoE+PromptFG_ScaleJSON";
            phoSsJsonPath   = "./photonSS.json";
            eleSsName       = "2022Re-recoE+PromptFG_ScaleJSON";
            eleSsJsonPath   = "./electronSS.json";
            lumiJsonPath    = "./Cert_Collisions2022_355100_362760_Golden.json";
            puTextPath      = "./pileup_ASCII_UL16-UL18.txt";
            minbXsec        = 69200;
            puHistPath      = "pileup.root";
            puHistEras      = {};
            puHistTrigs     = {};
        };
        ~ObjectScale();
        struct lumiInfo {
          double lum;
          double muavg;
          double murms;
          lumiInfo(double lum_=0, double muavg_=0, double murms_=0)
            : lum(lum_), muavg(muavg_), murms(murms_) {};
        };
        // Year, Data
        bool is22;
        bool is23;
        bool is24;
        bool isData;
        void setIs22(bool value);
        void setIs23(bool value);
        void setIs24(bool value);
        void setIsData(bool value);
        
        // Jet veto
        string jetVetoKey;
        string jetVetoName;
        string jetVetoJsonPath;
        void setJetVetoKey(TString oName);
        void setJetVetoName(TString oName);
        void setJetVetoJsonPath(TString oName);
        void loadJetVetoRef();
        correction::Correction::Ref loadedJetVetoRef;

        // Jet L2L3
        vector<string> jetL2L3Names;
        string jetL2L3JsonPath;
        void setJetL2L3Names(TString oName);
        void setJetL2L3JsonPath(TString oName);
        void loadJetL2L3Refs();
        vector<correction::Correction::Ref> loadedJetL2L3Refs;

        // Photon Scale and Smearing (Ss)
        string phoSsName;
        string phoSsJsonPath;
        void setPhoSsName(TString oName);
        void setPhoSsJsonPath(TString oName);
        void loadPhoSsRef();
        correction::Correction::Ref loadedPhoSsRef;
        
        // Electron Scale and Smearing (Ss)
        string eleSsName;
        string eleSsJsonPath;
        void setEleSsName(TString oName);
        void setEleSsJsonPath(TString oName);
        void loadEleSsRef();
        correction::Correction::Ref loadedEleSsRef;

        // Lumi
        void PrintInfo(string info, bool printcout);
        string lumiJsonPath;
        void setLumiJsonPath(TString oName);
        void loadLumiJson();
        std::map<int, std::map<int, int>> loadedLumiJson;

        // Pileup Text
        string puTextPath;
        void setPuTextPath(TString oName);
        void setPuMinbXsec(double xsec);
        void loadPuText();
        map<int, map<int, lumiInfo> > loadedPuText;
        double getTruePU(int run, int ls, double *rms);

        // Pileup Hist
        string puHistPath;
        std::vector<std::string> puHistEras;
        map<string, vector<string> > puHistTrigs;
        void setPuHistPath(TString oName);
        void setPuHistEras(TString oName);
        void setPuHistTrigs(TString oName);
        void loadPuHist();
        map<string, map<int, TH1D*> >  loadedPuHist;
        map<string, map<int, double> > loadedPuLumi;
    
        void applyJEC(SkimTree* tree, correction::CompoundCorrection::Ref jesRefSF, correction::Correction::Ref jesRefUnc, string systVar);

    private:
        bool debug;
        double minbXsec;
};


#endif
