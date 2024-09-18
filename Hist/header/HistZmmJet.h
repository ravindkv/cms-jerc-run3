#ifndef HistZmmJet_h
#define HistZmmJet_h
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

class HistZmmJet: public GlobalFlag{
 public :
    HistZmmJet(TString oName): GlobalFlag(oName){}
    int Run(SkimTree *tree, EventPick *eventP, ObjectPick *objP, ObjectScale *objS, TFile *fout);
    ~HistZmmJet();

 private :
    bool smearJets = false;

};

#endif
