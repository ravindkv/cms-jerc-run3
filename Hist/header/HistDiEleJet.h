#ifndef HistDiEleJet_h
#define HistDiEleJet_h
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

struct BasicHistosDiEleJet {
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

class HistDiEleJet: public GlobalFlag{
 public :
    HistDiEleJet(TString oName): GlobalFlag(oName){}
    int Run(SkimTree *tree, ObjectScale *objS, TFile *fout);
    ~HistDiEleJet();

 private :
    EventPick* eventPick;
    ObjectPick* objectPick;   
    //ObjectScale* objectScale;
	
    bool smearJets = false;

};

#endif

