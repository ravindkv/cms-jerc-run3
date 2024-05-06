#ifndef HistDiJet_h
#define HistDiJet_h
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "TH1D.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2.h>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <cmath>

#include "TRandom3.h"
#include "SkimTree.h"
#include "EventPick.h"
#include "ObjectPick.h"
#include "ObjectScale.h"
#include "GlobalFlag.h"

#include "correction.h"
struct BasicHistosDiJet {
  TH1D *hn;
  TH1D *hxsec;
  TProfile *prpt;
  TProfile *prbal;
  TProfile *prdb;
  TProfile *prmpf;
  TProfile *prmpf1;
  TProfile *prmpfn;
  TProfile *prmpfu;
  TProfile *prho;
  TProfile *pdjes;
  TProfile *pjes;
  TProfile *pres;
};

class MoreHistosDiJet {
 public:
  
  TH1D *hpt13, *hpt13a, *hpt13j;
  TProfile *pptg, *pptj;
  TProfile *pres, *pjsf, *pm0, *pm2, *pmn, *pmu;

  // Resolution
  TProfile *pm0x, *pm2x;

  // Extra FRS studies
  TProfile *pmnu, *pmnx, *pmux, *pmnux;
  
  // Composition
  TProfile *prho, *pchf, *pnef, *pnhf;

  // Alternative pT bins
  TProfile *presa, *pm0a, *pm2a, *pmna, *pmua;
  TProfile *presj, *pm0j, *pm2j, *pmnj, *pmuj;
};

class MoreHistosDiJet2 {
public:

  // Basic information about the trigger
  //string trg;
  //int trgpt;
  //double ptmin, ptmax, absetamin, absetamax;

  TH2D *h2pteta;
  TProfile2D *p2res, *p2m0, *p2m2, *p2mn, *p2mu;
  TProfile2D *p2m0x, *p2m2x;

  // Extra for FSR studies
  TProfile2D *p2mnu, *p2mnx, *p2mux, *p2mnux;
  //TH2D *h2ptetatc, *h2ptetapf;
  //TProfile2D *p2restc, *p2m0tc, *p2m2tc, *p2mntc, *p2mutc; // pT,tag (central)
  //TProfile2D *p2respf, *p2m0pf, *p2m2pf, *p2mnpf, *p2mupf; // pT,probe (forward)

  // Smearing controls
  TProfile2D *p2jsf;//, *p2jsftc, *p2jsfpf;
};

class HistDiJet: public GlobalFlag{
 public :
    HistDiJet();
    int Run(TString oName, SkimTree *tree, ObjectScale *objS, TFile *fout);

 private :

    EventPick* evtPick;   
    ObjectPick* objectPick;   
    ObjectScale* objectScale;
    bool _gh_debug = false;
    bool _gh_debug100 = false;

    bool doGamjet = true;
    bool doGamjet2 = true;
    bool smearJets = false;

    // Error counters
    int cntErrDR = 0;
	
    string          dataset;
    string          version;
    string          _filename; // file name for debugging purposes
    static const bool debugFiles = true;
    string systematicType;

    int eventNum = -1;

    bool isSystematicRun;

};

#endif

