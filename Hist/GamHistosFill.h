#ifndef GamHistosFill_h
#define GamHistosFill_h
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
#include "src/SkimTree.h"
#include "src/EventPick.h"
#include "src/ObjectPick.h"
#include "src/ObjectScale.h"

#include "correction.h"
struct BasicHistos {
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

class gamjetHistos {
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

class gamjetHistos2 {
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

class GamHistosFill {
 public :
    GamHistosFill(int ac, char** av);

 private :

    SkimTree* tree;   
    EventPick* evtPick;   
    ObjectPick* objectPick;   
    ObjectScale* objectScale;
	
    bool            isQCD;
    bool            isMG;
    string          dataset;
    string          version;
    string          _filename; // file name for debugging purposes
    static const bool debugFiles = true;
    string systematicType;

    int eventNum = -1;

    bool isSystematicRun;

    void InitVariables();
    void FillEvent(std::string year);
    void InitBranches();
   // Code originally from jetphys/HistosFill.C
   void PrintInfo(string info, bool printcout);
   bool LoadJSON(string json);
   void LoadPU();
   std::map<int, std::map<int, int> > _json;
   map<string, map<int, TH1D*> > _pu;
   map<string, map<int, double> >  _lumi;
};

// Code originally from jetphys/HistosFill.C
void GamHistosFill::PrintInfo(string info, bool printcout)
{
  //*ferr << info << endl << flush;
  if (printcout) cout << info << endl << flush;
}

// Code originally from jetphys/HistosFill.C
bool GamHistosFill::LoadJSON(string json)
{
bool _gh_debug = false;
bool _gh_debug100 = false;


  PrintInfo(string("Processing LoadJSON() with ") + json + " ...",true);
  ifstream file(json, ios::in);
  if (!file.is_open()) { assert(false); return false; }
  char c;
  string s, s2, s3;
  char s1[256];
  int rn(0), ls1(0), ls2(0), nrun(0), nls(0);
  file.get(c);
  if (c!='{') return false;
  while (file >> s and sscanf(s.c_str(),"\"%d\":",&rn)==1) {
    if (_gh_debug) PrintInfo(Form("\"%d\": ",rn),true);

    while (file.get(c) and c==' ') {};
    if (_gh_debug) { PrintInfo(Form("%c",c),true); assert(c=='['); }
    ++nrun;

    bool endrun = false;
    while (!endrun and file >> s >> s2 and (sscanf((s+s2).c_str(),"[%d,%d]%s",&ls1,&ls2,s1)==3 or (file >> s3 and sscanf((s+s2+s3).c_str(),"[%d,%d]%s",&ls1,&ls2,s1)==3))) {

      s2 = s1;
      if (s2=="]") { file >> s3; s2 += s3; }

      if (_gh_debug) PrintInfo(Form("[%d,%d,'%s']",ls1,ls2,s1),true);

      for (int ls = ls1; ls != ls2+1; ++ls) {
        _json[rn][ls] = 1;
        ++nls;
      }

      endrun = (s2=="]," || s2=="]}");
      if (_gh_debug and !endrun and s2!=",") { PrintInfo(string("s1: ")+s2,true); assert(s2==","); }
    } // while ls
    if (_gh_debug) PrintInfo("",true);

    if (s2=="]}") continue;
    else if (_gh_debug and s2!="],") PrintInfo(string("s2: ")+s2,true); assert(s2=="],");
  } // while run
  if (s2!="]}") { PrintInfo(string("s3: ")+s2,true); return false; }

  PrintInfo(string("Called LoadJSON() with ") + json + ":",true);
  PrintInfo(Form("Loaded %d good runs and %d good lumi sections",nrun,nls),true);
  return true;
} // LoadJSON

void GamHistosFill::LoadPU() {

  cout << endl << "GamHistosFill::LoadPU" << endl << flush;
  TDirectory *curdir = gDirectory;

  string eras[] =
    {"2016P8",/*"2016APVP8",*/"2016P8APV","2017P8", "2018P8",
     "2016QCD",/*"2016APVQCD",*/"2016QCDAPV","2017QCD", "2018QCD",
     "2016APV","2016FGH","2017","2018",
     "2022P8", "2022EEP8","2022QCD", "2022EEQCD"};
  //"2016BCD","2016EF","2016FGH",
  //"2017B","2017C","2017D","2017E","2017F",
  //"2018A","2018B","2018C","2018D"};
  const int neras = sizeof(eras)/sizeof(eras[0]);
  map<string, vector<string> > trigs;
  trigs["2016P8"].push_back("mc");
  trigs["2016APVP8"] = trigs["2017P8"] = trigs["2018P8"] = 
    trigs["2016QCD"] =  trigs["2016APVQCD"] = trigs["2017QCD"] =
    trigs["2018QCD"] = trigs["2018P8"] =
    trigs["2022P8"] = trigs["2022EEP8"] =
    trigs["2022QCD"] = trigs["2022EEQCD"] =
    trigs["2016P8"];

  trigs["2016APV"].push_back("HLT_Photon22_R9Id90_HE10_IsoM");
  trigs["2016APV"].push_back("HLT_Photon30_R9Id90_HE10_IsoM");
  trigs["2016APV"].push_back("HLT_Photon36_R9Id90_HE10_IsoM");
  trigs["2016APV"].push_back("HLT_Photon50_R9Id90_HE10_IsoM");
  trigs["2016APV"].push_back("HLT_Photon75_R9Id90_HE10_IsoM");
  trigs["2016APV"].push_back("HLT_Photon90_R9Id90_HE10_IsoM");
  trigs["2016APV"].push_back("HLT_Photon120_R9Id90_HE10_IsoM");
  trigs["2016APV"].push_back("HLT_Photon165_R9Id90_HE10_IsoM");
  //trigs["2016APV"].push_back("HLT_Photon165_HE10");
  trigs["2016APV"].push_back("HLT_Photon175");
  trigs["2016FGH"] = trigs["2016APV"];

  trigs["2017"].push_back("HLT_Photon20_HoverELoose");
  trigs["2017"].push_back("HLT_Photon30_HoverELoose");
  trigs["2017"].push_back("HLT_Photon50_R9Id90_HE10_IsoM");
  trigs["2017"].push_back("HLT_Photon75_R9Id90_HE10_IsoM");
  trigs["2017"].push_back("HLT_Photon90_R9Id90_HE10_IsoM");
  trigs["2017"].push_back("HLT_Photon120_R9Id90_HE10_IsoM");
  trigs["2017"].push_back("HLT_Photon165_R9Id90_HE10_IsoM");
  trigs["2017"].push_back("HLT_Photon200");

  trigs["2018"].push_back("HLT_Photon20_HoverELoose");
  trigs["2018"].push_back("HLT_Photon30_HoverELoose");
  trigs["2018"].push_back("HLT_Photon50_R9Id90_HE10_IsoM");
  trigs["2018"].push_back("HLT_Photon75_R9Id90_HE10_IsoM");
  trigs["2018"].push_back("HLT_Photon90_R9Id90_HE10_IsoM");
  trigs["2018"].push_back("HLT_Photon100EB_TightID_TightIso");
  trigs["2018"].push_back("HLT_Photon110EB_TightID_TightIso");
  trigs["2018"].push_back("HLT_Photon200");

  trigs["2022"].push_back("HLT_Photon20_HoverELoose");
  trigs["2022"].push_back("HLT_Photon30_HoverELoose");
  trigs["2022"].push_back("HLT_Photon30EB_TightID_TightIso");
  trigs["2022"].push_back("HLT_Photon50_R9Id90_HE10_IsoM");
  trigs["2022"].push_back("HLT_Photon75_R9Id90_HE10_IsoM");
  trigs["2022"].push_back("HLT_Photon90_R9Id90_HE10_IsoM");
  trigs["2022"].push_back("HLT_Photon100EBHE10");
  trigs["2022"].push_back("HLT_Photon110EB_TightID_TightIso");
  trigs["2022"].push_back("HLT_Photon200");

  // files/pileup.root updated with tchain.C on Hefaistos
  TFile *fmc = new TFile("files/pileup.root","READ");
  assert(fmc && !fmc->IsZombie());

  for (int i = 0; i != neras; ++i) {
    string se = eras[i]; const char *ce = se.c_str();
    for (unsigned int j = 0; j != trigs[se].size(); ++j) {
      string st = trigs[se][j]; const char *ct = st.c_str();

      // Read trigger threshold from trigger name
      int itrg(0);
      if (st=="mc") itrg = 1;
      else sscanf(ct,"HLT_Photon%d*",&itrg);
      
      TFile *fdt(0);
      TH1D *h(0);
      if (st=="mc") {
	h = (TH1D*)fmc->Get(Form("pileup_%s",ce));
	if (!h) cout << "Failed to find pileup_"<<ce<<endl<<flush;
	assert(h);
      }
      else {
	// data files from Laura (on CERNbox)
	fdt = new TFile(Form("pileup/%s/pu_%s.root",ce,ct),"READ");
	assert(fdt && !fdt->IsZombie());
	h = (TH1D*)fdt->Get("pileup");
	assert(h);
      }
      assert(h);

      curdir->cd();
      h = (TH1D*)h->Clone(Form("pileup_%s_%s",ce,ct));
      double lumi = h->Integral();
      h->Scale(1./lumi);
      _pu[se][itrg] = h;
      _lumi[se][itrg] = lumi;

      cout << Form("%s/%s (%d): %1.3g %s\n",ce,ct,itrg,
		   lumi,st=="mc" ? "events" : "fb-1");

      if (fdt) fdt->Close();
    } // for j in trigs
  } // for i in eras
  fmc->Close();
  cout << endl << flush;

  return;
} // LoadPU

void GamHistosFill::InitBranches(){
}

void GamHistosFill::InitVariables()
{
}
#endif

