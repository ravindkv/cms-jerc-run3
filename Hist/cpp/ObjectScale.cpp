#include"ObjectScale.h"
#include<iostream>
   
//-------------------------------------
// Jet Veto Maps: applied to bota data and MC
//-------------------------------------
void ObjectScale::setJetVetoKey(TString oName){
     jetVetoKey = "jetvetomap";
     cout<<setw(25)<<"setJetVetoKey() = "<<jetVetoKey<<endl;
} 
   
void ObjectScale::setJetVetoName(TString oName){
     if(is22C || is22D){
         jetVetoName = "Winter22Run3_RunCD_V1";
     }
     if(is22E){
         jetVetoName = "Winter22Run3_RunE_V1";
     }
     if(is23A || is23B || is23C || is23Pre){
         jetVetoName = "Summer23Prompt23_RunC_V1";
     }
     if(is23D || is23E || is23F || is23G || is23Post){
         jetVetoName = "Summer23BPixPrompt23_RunD_V1";
     }
     if(is24){//FIXME
         jetVetoName = "Summer23BPixPrompt23_RunD_V1";
     }
     cout<<setw(25)<<"setJetVetoName() = "<<jetVetoName<<endl;
} 
   
void ObjectScale::setJetVetoJsonPath(TString oName){
     if(is22 || is22D){
         jetVetoJsonPath = "POG/JME/2022_Prompt/jetvetomaps.json.gz";
     }
     if(is23A || is23B || is23C || is23Pre){
         jetVetoJsonPath = "POG/JME/2023_Summer23/jetvetomaps.json.gz";
     }
     if(is23D || is23E || is23F || is23G || is23Post){
         jetVetoJsonPath = "POG/JME/2023_Summer23BPix/jetvetomaps.json.gz";
     }
     if(is24){//FIXME
         jetVetoJsonPath = "POG/JME/2023_Summer23BPix/jetvetomaps.json.gz";
     }
     cout<<setw(25)<<"setJetVetoJsonPath() = "<<jetVetoJsonPath<<endl;
} 

void ObjectScale::loadJetVetoRef(){
    cout<<"==> loadJetVetoRef()"<<endl;
    auto jvJson      = correction::CorrectionSet::from_file(jetVetoJsonPath);
    try{
        loadedJetVetoRef = jvJson->at(jetVetoName);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadJetVetoRef()"<<endl;
        cout<<"Check "<<jetVetoJsonPath<<" or "<<jetVetoName<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }
} 
//-------------------------------------
// Jet L2L3 corrections
//-------------------------------------
void ObjectScale::setJetL2L3Names(TString oName){
    if (isData){
        if(is22C){
            jetL2L3Names.push_back("Summer22_22Sep2023_V2_MC_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer22_22Sep2023_RunCD_V2_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is22D){
            jetL2L3Names.push_back("Summer22_22Sep2023_V2_MC_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer22_22Sep2023_RunCD_V2_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is22E){
            jetL2L3Names.push_back("Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI");
            jetL2L3Names.push_back("Run22E-22Sep2023_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is22F){
            jetL2L3Names.push_back("Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI");
            jetL2L3Names.push_back("Run22F-Prompt_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is22G){
            jetL2L3Names.push_back("Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI");
            jetL2L3Names.push_back("Run22G-Prompt_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is23A || is23B || is23Cv123){
            jetL2L3Names.push_back("Summer23Prompt23_RunCv123_V1_DATA_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer23Prompt23_RunCv123_V1_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is23Cv4){
            jetL2L3Names.push_back("Summer23Prompt23_RunCv4_V1_DATA_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer23Prompt23_RunCv4_V1_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is23D || is23E || is23F || is23G){
            jetL2L3Names.push_back("Summer23BPixPrompt23_RunD_V1_DATA_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer23BPixPrompt23_RunD_V1_DATA_L2L3Residual_AK4PFPuppi");
        }
        if(is24){//FIXME
            jetL2L3Names.push_back("Summer23BPixPrompt23_RunD_V1_DATA_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer23BPixPrompt23_RunD_V1_DATA_L2L3Residual_AK4PFPuppi");
        }
    }
    else{
        if(is22){ 
            jetL2L3Names.push_back("Summer22_22Sep2023_V2_MC_L2Relative_AK4PFPuppi");
        }
        if(is23Pre){ 
            jetL2L3Names.push_back("Summer23Prompt23_V1_MC_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer23Prompt23_V1_MC_L2L3Residual_AK4PFPuppi");
        }
        if(is23Post){ 
            jetL2L3Names.push_back("Summer23BPixPrompt23_V1_MC_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer23BPixPrompt23_V1_MC_L2L3Residual_AK4PFPuppi");
        }
        if(is24){//FIXME
            jetL2L3Names.push_back("Summer23BPixPrompt23_V1_MC_L2Relative_AK4PFPuppi");
            jetL2L3Names.push_back("Summer23BPixPrompt23_V1_MC_L2L3Residual_AK4PFPuppi");
        }
    }
    cout<<setw(25)<<"setJetL2L3Names() = ";
    for(string jN: jetL2L3Names){ cout<<jN<<", ";}
    cout<<endl;
} 
   
void ObjectScale::setJetL2L3JsonPath(TString oName){
    if(is22 || is22D){
        jetL2L3JsonPath = "POG/JME/2022_Summer22/jet_jerc.json.gz";
    }
    if(is23A || is23B || is23C || is23Pre){
        jetL2L3JsonPath = "POG/JME/2023_Summer23/jet_jerc.json.gz";
    }
    if(is23D || is23E || is23F || is23G || is23Post){
        jetL2L3JsonPath = "POG/JME/2023_Summer23BPix/jet_jerc.json.gz";
    }
    if(is24){//FIXME
        jetL2L3JsonPath = "POG/JME/2023_Summer23BPix/jet_jerc.json.gz";
    }
    cout<<setw(25)<<"setJetL2L3JsonPath() = "<<jetL2L3JsonPath<<endl;
} 
   
void ObjectScale::loadJetL2L3Refs(){
     cout<<"==> loadJetL2L3Refs()"<<endl;
     auto jercJson  = correction::CorrectionSet::from_file(jetL2L3JsonPath);
    try{
        for(auto name: jetL2L3Names){ 
            loadedJetL2L3Refs.push_back(jercJson->at(name));
        }
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadJetL2L3Refs"<<endl;
        cout<<"Check "<<jetL2L3JsonPath<<" or jetL2L3Names"<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }
} 
//-------------------------------------
// Photon Scale and Smearing 
//-------------------------------------
void ObjectScale::setPhoSsName(TString oName){
    if(is22B || is22C || is22D || isMC){
        //phoSsName = "2022Re-recoBCD_ScaleJSON";
        phoSsName = "2022Re-recoBCD_SmearingJSON";
    }
    if(is22E || is22F || is22G || (isMC && isPostEE)){
        //phoSsName = "2022Re-recoE+PromptFG_ScaleJSON";
        phoSsName = "2022Re-recoE+PromptFG_SmearingJSON";
    }
     cout<<setw(25)<<"setPhoSsName() = "<<phoSsName<<endl;
} 
   
void ObjectScale::setPhoSsJsonPath(TString oName){
    if(is22B || is22C || is22D || isMC){
        phoSsJsonPath = "POG/EGM/S+SJSON/2022Re-recoBCD/photonSS.json";
    }
    if(is22E || is22F || is22G || (isMC && isPostEE)){
        phoSsJsonPath = "POG/EGM/S+SJSON/2022Re-recoE+PromptFG/photonSS.json";
    }
     cout<<setw(25)<<"setPhoSsJsonPath() = "<<phoSsJsonPath<<endl;
} 

void ObjectScale::loadPhoSsRef(){
    cout<<"==> loadPhoSsRef()"<<endl;
    auto ssJson      = correction::CorrectionSet::from_file(phoSsJsonPath);
    try{
        loadedPhoSsRef = ssJson->at(phoSsName);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadPhoSsRef()"<<endl;
        cout<<"Check "<<phoSsJsonPath<<" or "<<phoSsName<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }
} 

//-------------------------------------
// Electron Scale and Smearing 
//-------------------------------------
void ObjectScale::setEleSsName(TString oName){
    if(is22B || is22C || is22D){
        eleSsName = "2022Re-recoBCD_ScaleJSON";
    }
    if(is22E || is22F || is22G){
        eleSsName = "2022Re-recoE+PromptFG_ScaleJSON";
    }
     cout<<setw(25)<<"setEleSsName() = "<<eleSsName<<endl;
} 
   
void ObjectScale::setEleSsJsonPath(TString oName){
    if(is22B || is22C || is22D || isMC){
        eleSsJsonPath = "POG/EGM/S+SJSON/2022Re-recoBCD/electronSS.json";
    }
    if(is22E || is22F || is22G || (isMC && isPostEE)){
        eleSsJsonPath = "POG/EGM/S+SJSON/2022Re-recoE+PromptFG/electronSS.json";
    }
     cout<<setw(25)<<"setEleSsJsonPath() = "<<eleSsJsonPath<<endl;
} 

void ObjectScale::loadEleSsRef(){
    cout<<"==> loadEleSsRef()"<<endl;
    auto ssJson      = correction::CorrectionSet::from_file(eleSsJsonPath);
    try{
        loadedEleSsRef = ssJson->at(eleSsName);
    }catch (const std::exception & e){
        cout<<"\nEXCEPTION: ObjectScale::loadEleSsRef()"<<endl;
        cout<<"Check "<<eleSsJsonPath<<" or "<<eleSsName<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }
} 

void ObjectScale::PrintInfo(string info, bool printcout)
   {
     //*ferr << info << endl << flush;
     if (printcout) cout << info << endl << flush;
}
//-------------------------------------
// Golden lumi Json 
//-------------------------------------
void ObjectScale::setLumiJsonPath(TString oName){
    if (is22)
      lumiJsonPath = "POG/files/Cert_Collisions2022_355100_362760_Golden.json";
    if (is23)
      lumiJsonPath = "POG/files/Cert_Collisions2023_366442_370790_Golden.json";
    if (is24)
      lumiJsonPath = "POG/files/Cert_Collisions2024_378981_381594_Golden.json";
    cout<<setw(25)<<"setLumiJsonPath() = "<<lumiJsonPath<<endl;
} 

void ObjectScale::loadLumiJson(){
    cout<<"==> loadLumiJson()"<<endl;
    std::ifstream file(lumiJsonPath);
    file >> loadedLumiJson;
}

bool ObjectScale::checkGoodLumi(unsigned int &run, unsigned int &lumi){
  try{
    auto it = loadedLumiJson.find(std::to_string(run));
    if (it != loadedLumiJson.end()) {
      for (const auto& range : it.value()) {
        if (lumi >= range[0] && lumi <= range[1]) {
            return true;
        }
      }
    }
  } catch (const std::exception& e) {
      cout<<e.what()<<" in the golden JSON"<<endl;
  }
  return false;
}

//-------------------------------------
// Pileup Text 
//-------------------------------------
void ObjectScale::setPuTextPath(TString oName){
    puTextPath = "POG/files/pileup_ASCII_UL16-UL18.txt";
    cout<<setw(25)<<"setPuTextPath() = "<<puTextPath<<endl;
} 
void ObjectScale::setPuMinbXsec(double xsec){
    minbXsec = xsec;
    cout<<setw(25)<<"setPuMinbXsec() = "<<minbXsec <<endl;
} 
void ObjectScale::loadPuText(){
    cout<<"==> loadPuText()"<<endl;
    string json = "";
    TDirectory *curdir = gDirectory;
    cout << Form("parsePileUpJSON(minbXsec=%1.3g):",minbXsec) << endl;
    cout << "Opening " << puTextPath << "...";
    ifstream f(puTextPath.c_str(),ios::in);
    if (f.is_open()) cout << "ok" << endl;
    //else { cout << "failure" << endl; return; }
    
    if (json!="") cout << "Opening " << json << "...";
    ifstream fj(json.c_str(),ios::in);
    if (fj.is_open()) cout << "ok" << endl;
    //else if (json!="") { cout << "failure" << endl; return; }
    
    TH1D *hpu = new TH1D("hpu",";true PU;",600,0,60);
    TH1D *hpux = new TH1D("hpux",";true PU (cleaned);",600,0,60);
    TH1D *hpus = new TH1D("hpus",";true PU (smeared);",600,0,60);
    TH1D *hpuo = new TH1D("hpuo",";observed PU;",60,0,60);
    
    TH1D *hruns = new TH1D("hruns",";run;",20000,190000,210000);
    TH1D *hrunsx = new TH1D("hrunsx",";run;",20000,190000,210000);
    
    TH2D *h2pu = new TH2D("h2pu",";true PU (px); true PU (hf);",
       	600,0,60, 600,0,60);
    
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
   			goodls.insert(pair<int,int>(run, i));
         }
       }
     }
     // Read information from file
     double maxmu = 60;
     int nrun(0), nls(0);
   
     if (f.is_open()) {
       int run, ls, cnt(0), cntMax(-1);
       double lum, xsavg, xsrms;
       while ( f >> run >> ls >> lum >> xsrms >> xsavg && ++cnt!=cntMax) {
         if (debug && cnt<10)
   	    cout << run<<" "<<ls<<" "<<lum<<" "<<xsavg<<" "<<xsrms << endl;
         if (json=="" || goodls.find(pair<int,int>(run,ls))!=goodls.end()) {
			double mu = xsavg * minbXsec;
			double murms = xsrms * minbXsec;
			//if (murms>mu) mu = min(mu,maxmu);
			//murms = max(0.,min(mu,murms));
			
			if (loadedPuText.find(run)==loadedPuText.end()) ++nrun;
			if (loadedPuText[run].find(ls)==loadedPuText[run].end()) ++nls;
			
			loadedPuText[run][ls] = lumiInfo(lum, mu, murms);
			
			lumiInfo const& m = loadedPuText[run][ls];
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
}//loadPuText()

double ObjectScale::getTruePU(int run, int ls, double *rms = 0) {
     if (loadedPuText.find(run)==loadedPuText.end()) return 0;
     if (loadedPuText[run].find(ls)==loadedPuText[run].end()) return 0;
     if (rms!=0) (*rms) = loadedPuText[run][ls].murms;
     return loadedPuText[run][ls].muavg;
}
   
//-------------------------------------
// Pileup Hist 
//-------------------------------------
void ObjectScale::setPuHistPath(TString oName){
    puHistPath = "POG/files/pileup.root";
    cout<<setw(25)<<"setPuHistPath() = "<<puHistPath<<endl;
} 
void ObjectScale::setPuHistEras(TString oName){
    puHistEras.push_back("2022");
    puHistEras.push_back("2023");
    cout<<setw(25)<<"setPuHistEras() = ";
    for(auto era: puHistEras){ cout<<era<<", ";}
    cout<<endl;
} 
void ObjectScale::setPuHistTrigs(TString oName){
    if(isGamJet){
        puHistTrigs["2022"].push_back("HLT_Photon20_HoverELoose");
        puHistTrigs["2022"].push_back("HLT_Photon30_HoverELoose");
        //puHistTrigs["2022"].push_back("HLT_Photon30EB_TightID_TightIso");
        puHistTrigs["2022"].push_back("HLT_Photon50_R9Id90_HE10_IsoM");
        puHistTrigs["2022"].push_back("HLT_Photon75_R9Id90_HE10_IsoM");
        puHistTrigs["2022"].push_back("HLT_Photon90_R9Id90_HE10_IsoM");
        //puHistTrigs["2022"].push_back("HLT_Photon100EBHE10");
        puHistTrigs["2022"].push_back("HLT_Photon110EB_TightID_TightIso");
        puHistTrigs["2022"].push_back("HLT_Photon200");
        //puHistTrigs["2023"] = puHistTrigs["2022"];
    }
    if(isDiEleJet){
        puHistTrigs["2022"].push_back("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ");
    }
    cout<<setw(25)<<"setPuHistTrigs() = ";
    for (const auto& pair : puHistTrigs) {
        std::cout <<pair.first<<endl;
        for(auto trig: pair.second){ cout<<trig<<", ";}
    }
    cout<<endl;
} 
void ObjectScale::loadPuHist(){
    cout<<"==> loadPuHist()"<<endl<<flush;
    TDirectory *curdir = gDirectory;
    
    // files/pileup.root updated with tchain.C on Hefaistos
    TFile *fmc = new TFile(puHistPath.c_str(), "READ");
    assert(fmc && !fmc->IsZombie());
    
    for (int i = 0; i != puHistEras.size(); ++i) {
        string se = puHistEras[i]; const char *ce = se.c_str();
        for (unsigned int j = 0; j != puHistTrigs[se].size(); ++j) {
            string st = puHistTrigs[se][j]; const char *ct = st.c_str();
            
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
            loadedPuHist[se][itrg] = h;
            loadedPuLumi[se][itrg] = lumi;
            cout << Form("%s/%s (%d): %1.3g %s\n",ce,ct,itrg,
   		    lumi,st=="mc" ? "events" : "fb-1");
            if (fdt) fdt->Close();
        } // for j in puHistTrigs
     } // for i in puHistEras
     fmc->Close();
     cout << endl << flush;
} // LoadPU
   

void ObjectScale::setThresh(TString oName){
    double frac = 0.5;
    if(is22 || is23 || is24){
      bThresh = 0.4184;
      cThresh = 0.137+frac*(0.66-0.137);
    } 
}

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
   
