#include"ObjectScale.h"
#include<iostream>
   
void ObjectScale::applyJEC(SkimTree* tree, correction::CompoundCorrection::Ref jesRefSF, correction::Correction::Ref jesRefUnc, string systVar){
       /*
   	TLorentzVector tMET;
   	tMET.SetPtEtaPhiM(tree->MET_pt_,0.0,tree->MET_phi_,0.0);
   
   	for(int jetInd = 0; jetInd < tree->nJet_ ; ++jetInd){
   		if(tree->jetPt_[jetInd] < 10) continue;
   		if(fabs(tree->jetEta_[jetInd]) > 5.2) continue;
           //correction = SF
           double rawPt = tree->jetPt_[jetInd] * (1.-tree->jetRawFactor_[jetInd]);
           auto corr = jesRefSF->evaluate({tree->jetArea_[jetInd], tree->jetEta_[jetInd], rawPt, tree->rho_});
   
   		TLorentzVector tjet;
   		tjet.SetPtEtaPhiM(tree->jetPt_[jetInd], tree->jetEta_[jetInd], tree->jetPhi_[jetInd], 0.0);
   		tMET+=tjet;
           //Uncertanity
           auto unc=jesRefUnc->evaluate({tree->jetEta_[jetInd], tree->jetPt_[jetInd]});
   		if(systVar=="down") corr-=unc;
   		if(systVar=="up")   corr+=unc;
           double jes = (1.-tree->jetRawFactor_[jetInd]) * corr;
   		
   		tree->jetPt_[jetInd] = tree->jetPt_[jetInd] * jes; 
           tree->jetmuEF_[jetInd] = jes; //ad-hoc way of storing jes in other var for cross-check
   		tjet.SetPtEtaPhiM(tree->jetPt_[jetInd], tree->jetEta_[jetInd], tree->jetPhi_[jetInd], 0.0);		       
   		tMET-=tjet;
   	}
   	tree->MET_pt_ = tMET.Pt();
   	tree->MET_phi_ = tMET.Phi();
       */
   
}
   
vector<string> ObjectScale::getL2L3Names(TString oName){
     vector<string> jNs;//jecNamess
     if (isData){
       if(oName.Contains("2022C")){
           jNs.push_back("Summer22_22Sep2023_V2_MC_L2Relative_AK4PFPuppi");
           jNs.push_back("Summer22_22Sep2023_RunCD_V2_DATA_L2L3Residual_AK4PFPuppi");
       }
       if(oName.Contains("2022D")){
           jNs.push_back("Summer22Run3_V1_MC_L2Relative_AK4PUPPI");
           jNs.push_back("Run22CD-22Sep2023_DATA_L2L3Residual_AK4PFPuppi");
       }
       if(oName.Contains("2022E")){
           jNs.push_back("Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI");
           jNs.push_back("Run22E-22Sep2023_DATA_L2L3Residual_AK4PFPuppi");
       }
       if(oName.Contains("2022F")){
           jNs.push_back("Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI");
           jNs.push_back("Run22F-Prompt_DATA_L2L3Residual_AK4PFPuppi");
       }
       if(oName.Contains("2022G")){
           jNs.push_back("Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI");
           jNs.push_back("Run22G-Prompt_DATA_L2L3Residual_AK4PFPuppi");
       }
     }
     else{
       jNs.push_back("Summer22_22Sep2023_V2_MC_L2Relative_AK4PFPuppi");
     }
     cout<<"\nINFO: ObjectScale::getL2L3Names()"<<endl;
     for(string jN: jNs){ cout<<jN<<endl;}
     return jNs;
} 
   
string ObjectScale::getJvName(TString oName){
       string jvN;//Jet Veto Name
       if (isData){
           if(oName.Contains("2022C") || oName.Contains("2022D")){
               //jvN = "Summer22_23Sep2023_RunCD_V1";
               //jvN = "Summer22EE_23Sep2023_RunEFG_V1";
               jvN = "Winter22Run3_RunCD_V1";
           }
       }
       else{
           jvN = "Winter22Run3_RunCD_V1";
       }
     cout<<"\nINFO: ObjectScale::getJvName()"<<endl;
     cout<<jvN<<endl;
     return jvN;
} 
   
string ObjectScale::getJvKey(TString oName){
       string jvK;//Jet Veto Key
       if (isData){
           if(oName.Contains("2022") || oName.Contains("2023")){
               jvK = "jetvetomap";
           }
       }
       else{
           jvK = "jetvetomap";
       }
     cout<<"\nINFO: ObjectScale::getJvKey()"<<endl;
     cout<<jvK<<endl;
     return jvK;
} 
   
   
   // Code originally from jetphys/HistosFill.C
void ObjectScale::PrintInfo(string info, bool printcout)
   {
     //*ferr << info << endl << flush;
     if (printcout) cout << info << endl << flush;
}
   
   // Code originally from jetphys/HistosFill.C
std::map<int, std::map<int, int>> ObjectScale::LoadJSON(string json){
       bool _gh_debug = false;
       bool _gh_debug100 = false;
       std::map<int, std::map<int, int> > _json;
   
       PrintInfo(string("Processing LoadJSON() with ") + json + " ...",true);
       ifstream file(json, ios::in);
       //if (!file.is_open()) { assert(false); return false; }
       if (!file.is_open()) { assert(false);}
       char c;
       string s, s2, s3;
       char s1[256];
       int rn(0), ls1(0), ls2(0), nrun(0), nls(0);
       file.get(c);
       //if (c!='{') return false;
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
       if (s2!="]}") { PrintInfo(string("s3: ")+s2,true);}
       //if (s2!="]}") { PrintInfo(string("s3: ")+s2,true); return false; }
       
       PrintInfo(string("Called LoadJSON() with ") + json + ":",true);
       PrintInfo(Form("Loaded %d good runs and %d good lumi sections",nrun,nls),true);
       return _json;
} // LoadJSON
   
map<string, map<int, TH1D*> > ObjectScale::LoadPU(std::vector<std::string> &eras, map<string, vector<string> > &trigs) {
       map<string, map<int, TH1D*> > _pu;
     cout << endl << "ObjectScale::LoadPU" << endl << flush;
     TDirectory *curdir = gDirectory;
   
     // files/pileup.root updated with tchain.C on Hefaistos
     TFile *fmc = new TFile("files/pileup.root","READ");
     assert(fmc && !fmc->IsZombie());
   
     for (int i = 0; i != eras.size(); ++i) {
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
         cout << Form("%s/%s (%d): %1.3g %s\n",ce,ct,itrg,
   		   lumi,st=="mc" ? "events" : "fb-1");
   
         if (fdt) fdt->Close();
       } // for j in trigs
     } // for i in eras
     fmc->Close();
     cout << endl << flush;
   
     return _pu;
} // LoadPU
   
   
map<string, map<int, double> > ObjectScale::LoadLumi(std::vector<std::string> &eras, map<string, vector<string> > &trigs) {
       map<string, map<int, double> > _lumi;
     cout << endl << "ObjectScale::LoadPU" << endl << flush;
     TDirectory *curdir = gDirectory;
   
     // files/pileup.root updated with tchain.C on Hefaistos
     TFile *fmc = new TFile("files/pileup.root","READ");
     assert(fmc && !fmc->IsZombie());
   
     for (int i = 0; i != eras.size(); ++i) {
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
         _lumi[se][itrg] = lumi;
   
         cout << Form("%s/%s (%d): %1.3g %s\n",ce,ct,itrg,
   		   lumi,st=="mc" ? "events" : "fb-1");
   
         if (fdt) fdt->Close();
       } // for j in trigs
     } // for i in eras
     fmc->Close();
     cout << endl << flush;
   
     return _lumi;
} // LoadLumi
   
map<int, map<int, ObjectScale::lumiInfo> > ObjectScale::parsePileUpJSON(string filename, string json, double minbXsec) {
       map<int, map<int, ObjectScale::lumiInfo> > _mus;
     TDirectory *curdir = gDirectory;
     cout << Form("parsePileUpJSON(minbXsec=%1.3g):",minbXsec) << endl;
     cout << "Opening " << filename << "...";
     ifstream f(filename.c_str(),ios::in);
     if (f.is_open()) cout << "ok" << endl;
     //else { cout << "failure" << endl; return; }
   
     if (json!="") cout << "Opening " << json << "...";
     ifstream fj(json.c_str(),ios::in);
     if (fj.is_open()) cout << "ok" << endl;
     //else if (json!="") { cout << "failure" << endl; return; }
   
     cout<<"PU:-1"<<endl;
     TH1D *hpu = new TH1D("hpu",";true PU;",600,0,60);
     TH1D *hpux = new TH1D("hpux",";true PU (cleaned);",600,0,60);
     TH1D *hpus = new TH1D("hpus",";true PU (smeared);",600,0,60);
     TH1D *hpuo = new TH1D("hpuo",";observed PU;",60,0,60);
   
     TH1D *hruns = new TH1D("hruns",";run;",20000,190000,210000);
     TH1D *hrunsx = new TH1D("hrunsx",";run;",20000,190000,210000);
   
     TH2D *h2pu = new TH2D("h2pu",";true PU (px); true PU (hf);",
   			600,0,60, 600,0,60);
   
     cout<<"PU:0"<<endl;
     set<int> hiruns;
     hiruns.insert(199703); // part
     hiruns.insert(200190); // part
     hiruns.insert(203830);
     hiruns.insert(203832);
     hiruns.insert(203833);
     hiruns.insert(203834);
     hiruns.insert(203835); // part
     hiruns.insert(208509); // part
   
     // Load list of good LS from re-reco JSON
     set<pair<int, int> > goodls;
     if (fj.is_open()) {
       int run, ls1, ls2;
       while (fj >> run >> ls1 >> ls2) {
   
         for (int i = ls1; i != ls2+1; ++i) {
   	//goodls.insert(make_pair<int,int>(run, ls1+i));
   	//goodls.insert(make_pair<int,int>(run, i));
   	goodls.insert(pair<int,int>(run, i));
         }
       }
     }
     cout<<"PU:1"<<endl;
     // Read information from file
     double maxmu = 60;
     int nrun(0), nls(0);
   
     if (f.is_open()) {
       int run, ls, cnt(0), cntMax(-1);
       double lum, xsavg, xsrms;
       while ( f >> run >> ls >> lum >> xsrms >> xsavg && ++cnt!=cntMax) {
         
         if (debug && cnt<10)
   	cout << run<<" "<<ls<<" "<<lum<<" "<<xsavg<<" "<<xsrms << endl;
         
         //if (json=="" || goodls.find(make_pair<int,int>(run,ls))!=goodls.end()) {
         if (json=="" || goodls.find(pair<int,int>(run,ls))!=goodls.end()) {
   	
   	double mu = xsavg * minbXsec;
   	double murms = xsrms * minbXsec;
   	//if (murms>mu) mu = min(mu,maxmu);
   	//murms = max(0.,min(mu,murms));
   	
   	if (_mus.find(run)==_mus.end()) ++nrun;
   	if (_mus[run].find(ls)==_mus[run].end()) ++nls;
   
   	_mus[run][ls] = lumiInfo(lum, mu, murms);
   	
   	lumiInfo const& m = _mus[run][ls];
   	if (debug && cnt<10)
   	cout << "=>"<<m.muavg<<" +/- "<<m.murms<<endl;
   	
   	if (debug) {
   	  hruns->Fill(run, lum);
   	  if (mu>maxmu) hrunsx->Fill(run, lum);
   	  
   	  hpu->Fill(mu, lum);
   	  if (hiruns.find(run)!=hiruns.end()) hpux->Fill(run, lum);
   	  double murnd = gRandom->Gaus(mu, murms); 
   	  hpus->Fill(murnd, lum);
   	  int muobs = gRandom->Poisson(murnd);
   	  hpuo->Fill(muobs, lum);
   	}
         }
       }
     } // read from file
     cout<<"PU:2"<<endl;
   
     if (debug) {
       hpu->SetLineColor(kBlue);
       hpu->Draw();
       hpux->SetLineColor(kBlue+2);
       hpux->SetLineStyle(kDotted);
       hpux->Draw("SAME");
       hpus->SetLineColor(kBlack);
       hpus->Draw("SAME");
       hpuo->Scale(0.1); // for bin size
       hpuo->SetLineColor(kRed);
       hpuo->Draw("SAME");
   
       TCanvas *c2 = new TCanvas("c2","c2",600,600); c2->cd();
       hruns->Draw();
       hrunsx->SetLineColor(kRed);
       hrunsx->Draw("SAME");
     }
   
     cout << "Read in "<<nls<<" lumisections in "<<nrun<<" runs"<<endl;
       return _mus;
} // parsePileUpJSON
   
double ObjectScale::getTruePU(map<int, map<int, ObjectScale::lumiInfo> > _mus, int run, int ls, double *rms = 0) {
   
     if (_mus.find(run)==_mus.end()) return 0;
     if (_mus[run].find(ls)==_mus[run].end()) return 0;
     if (rms!=0) (*rms) = _mus[run][ls].murms;
     return _mus[run][ls].muavg;
}
   
void ObjectScale::setIsData(bool value) {
    isData = value;
}
