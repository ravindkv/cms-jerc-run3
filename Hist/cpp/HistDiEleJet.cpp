#include "HistDiEleJet.h"
   
int HistDiEleJet::Run(SkimTree *tree, ObjectScale *objS, TFile *fout){
   
  TDirectory *curdir = gDirectory;
  assert(fout && !fout->IsZombie());
  string dir = (isMC ? "MC" : "DATA");

  //pT binning
  double binsPt[] = {15, 20, 25, 30, 35, 40, 50, 60, 75, 90, 110, 130, 175, 230,
  		 300, 400, 500, 600, 700, 850, 1000, 1200, 1450, 1750,
   	  2100, 2500, 3000};
  const int nPt = sizeof(binsPt)/sizeof(binsPt[0])-1;
  //Eta binning
  double binsEta[] = {-5.191, -3.839, -3.489, -3.139, -2.964, -2.853, -2.650,
   	  -2.500, -2.322, -2.172, -1.930, -1.653, -1.479, -1.305,
   	  -1.044, -0.783, -0.522, -0.261, 0.000, 0.261, 0.522, 0.783,
   	  1.044, 1.305, 1.479, 1.653, 1.930, 2.172, 2.322, 2.500,
   	  2.650, 2.853, 2.964, 3.139, 3.489, 3.839, 5.191};
  const int nEta = sizeof(binsEta)/sizeof(binsEta[0])-1;

  //------------------------------------
  // Cutflow histograms 
  //------------------------------------
  TH1D *hCutflow = new TH1D("hCutflow",";hCutflow",15,0.5,15.5);
  TH1D *hCutflowWeight = new TH1D("hCutflowWeight",";hCutflowWeight",15,0.5,15.5);
  vector<string> cuts;
  cuts.push_back("passHLT");
  cuts.push_back("passGoodLumi");
  cuts.push_back("passMetFilter");
  cuts.push_back("passAtleast1Probe");
  cuts.push_back("passAtleast1Jet");
  cuts.push_back("passJetVetoMap");
  cuts.push_back("passDPhiProbeJet1");
  cuts.push_back("passProbeBarrel");
  cuts.push_back("passJet1EtaJet2Pt");
  cuts.push_back("passResponse");
  for (int i = 0; i != cuts.size(); ++i) {
    hCutflow->GetXaxis()->SetBinLabel(i+1, cuts[i].c_str());
    hCutflowWeight->GetXaxis()->SetBinLabel(i+1, cuts[i].c_str());
  }
  
  //Probe = Z->ee, Z->mumu, Gamma, Jet
  //------------------------------------
  // Variables after at least 1 Probe 
  //------------------------------------
  fout->mkdir("passAtleast1Probe");
  fout->cd("passAtleast1Probe");
  TH1D *hProbePt = new TH1D("hProbePt","",197,15,1000);
  TH1D *hProbePtBarrel = new TH1D("hProbePtBarrel","",197,15,1000);
  TH2D *h2ProbeEtaProbePhi = new TH2D("h2ProbeEtaProbePhi","",30,-1.305,+1.305,
   	   	     72,-TMath::Pi(),TMath::Pi());
  // Match barrel edge to 1.305 with 3.132, even though EC edge should be 3.139
  TH2D *h2ProbeEtaProbePhiRebin = new TH2D("h2ProbeEtaProbePhiRebin","",360,-3.132,+3.132,
   	   		360,-TMath::Pi(),TMath::Pi());
  TH2D *h2ProbeEtaProbePhiProbePt110 = new TH2D("h2ProbeEtaProbePhiProbePt110","",30,-1.305,+1.305,
   	   	     72,-TMath::Pi(),TMath::Pi());
  TH2D *h2ProbeEtaProbePhiProbePt110Rebin = new TH2D("h2ProbeEtaProbePhiProbePt110Rebin","",360,-3.132,+3.132,
   	   		360,-TMath::Pi(),TMath::Pi());
  TH2D *h2ProbePtCountProbe = new TH2D("h2ProbePtCountProbe","",nPt,binsPt,5,0,5);
  // 2D plots for mu vs Probe pT
  TH1D *hNpuProbePt210 = new TH1D("hNpuProbePt210","",100,0,100);
  TH2D *h2ProbePtNpu = new TH2D("h2ProbePtNpu","",nPt,binsPt,100,0,100);

  TH1D *hGenProbePt = new TH1D("hGenProbePt","",nPt,binsPt);
  TProfile *pProbePtOgenProbePtInGenProbePt = new TProfile("pProbePtOgenProbePtInGenProbePt","",nPt,binsPt);// O = Over
  TProfile *pGenProbePtOProbePtInProbePt  = new TProfile("pGenProbePtOProbePtInProbePt","",nPt,binsPt);
   
  //------------------------------------
  // Variables after leading Probe in barrel (eta < 1.33) 
  //------------------------------------
  fout->mkdir("passProbeBarrel");
  fout->cd("passProbeBarrel");
  // Follow up on problematic cuts
  TH1D *hDphiJetProbe = new TH1D("hDphiJetProbe",";#Delta#phi;N_{events}",
   	   	 126,-TMath::TwoPi(),TMath::TwoPi());
  TH1D *hDrJetProbe = new TH1D("hDrJetProbe",";#DeltaR;N_{events}",100,0,10);
  TH2D *h2ProbePtDbResp = new TH2D("h2ProbePtDbResp","",nPt,binsPt,200,0,4);
  TH2D *h2ProbePtMpfResp = new TH2D("h2ProbePtMpfResp","",nPt,binsPt,300,-2,4);
  // Plots for jet properties
  TProfile *pGenJ1ptOprobePtInProbePt = new TProfile("pGenJ1ptOprobePtInProbePt","",nPt,binsPt);
  TProfile *pJ1ptOprobePtInProbePt = new TProfile("pJ1ptOprobePtInProbePt","",nPt,binsPt);
  TProfile *pJ1ptOgenJ1ptInGenJ1pt = new TProfile("pJ1ptOgenJ1ptInGenJ1pt","",nPt,binsPt);
   
  fout->mkdir("passProbeBarrel/flavourTagged");
  fout->cd("passProbeBarrel/flavourTagged");
  map<string, double> mvar;
  map<string, map<string, map<string, TH1*> > > mp;
  string avar[] = {"hProbePt",
                  "pMpfRespInProbePt",
                  "pDbRespInProbePt",
                  "pMpfResp1InProbePt",
                  "pMpfRespNInProbePt",
                  "pMpfRespUInProbePt",
                  "pRhoInProbePt",
                  "pJet1PtOProbePtInProbePt",
                  "pGenJet1PtOProbePtInProbePt",
                  "pJet1PtOGenJet1PtInGenJet1Pt"};
  string atag[] = {"i","b","c","q","g","n"};
  string aflv[] = {"i","b","c","q","s","ud","g","n"};
  const int nvar = sizeof(avar)/sizeof(avar[0]);
  const int ntag = sizeof(atag)/sizeof(atag[0]);
  const int nflv = sizeof(aflv)/sizeof(aflv[0]);
  for (int ivar = 0; ivar != nvar; ++ivar) {
    for (int itag = 0; itag != ntag; ++itag) {
      for (int iflv = 0; iflv != nflv; ++iflv) {
        string& var = avar[ivar]; const char *cv = var.c_str();
        string& tag = atag[itag]; const char *ct = tag.c_str();
        string& flv = aflv[iflv]; const char *cf = flv.c_str();
        if (var=="hGenJet1Pt")
          mp[var][tag][flv] = new TH1D(Form("%s_%s%s",cv,ct,cf),"",nPt,binsPt);
        else
          mp[var][tag][flv] = new TProfile(Form("%s_%s%s",cv,ct,cf),"",nPt,binsPt);
      } // for iflv
    } // for itag
  } // for ivar

  //------------------------------------
  // Variables after Jet1Eta, Jet2Pt cut 
  //------------------------------------
  fout->mkdir("passJet1EtaJet2Pt");
  fout->cd("passJet1EtaJet2Pt");
  TProfile *pJetEtaInProbePt = new TProfile("pJetEtaInProbePt","",nPt,binsPt);
  TH2D *h2JetEtaProbePt = new TH2D("h2JetEtaProbePt","",nPt,binsPt,nEta,binsEta);
  TProfile *pGenJ2ptOprobePtInProbePt = new TProfile("pGenJ2ptOprobePtInProbePt","",nPt,binsPt);
  TProfile *pJ2ptOprobePtInProbePt = new TProfile("pJ2ptOprobePtInProbePt","",nPt,binsPt);
  TProfile *pJ2ptOgenJ2ptInGenJ2pt = new TProfile("pJ2ptOgenJ2ptInGenJ2pt","",nPt,binsPt);

  // 1D composition and response
  // p = Profile
  // Db = Direct Balance, MPF = MET Projection Fraction
  // Resp = Response
  // Chhef = Charged Hadron Energy Fraction
  // Nehef = Neutral Hadron Energy Fraction
  // Neemef = Neutral ElectronMagnetic Energy Fraction
  // Chemef = Charged ElectronMagnetic Energy Fraction
  // Muef = Muon Energy Fraction
  // Chpv0ef = Charged PV=0 Energy Fraction
  TProfile *pDbRespInProbePt  = new TProfile("pDbRespInProbePt","",nPt,binsPt);
  TProfile *pMpfRespInProbePt = new TProfile("pMpfRespInProbePt","",nPt,binsPt);
  TProfile *pJetChhefInProbePt = new TProfile("pJetChhefInProbePt","",nPt,binsPt);
  TProfile *pJetNehefInProbePt = new TProfile("pJetNhefInProbePt","",nPt,binsPt);
  TProfile *pJetNeemefInProbePt  = new TProfile("pJetNeemefInProbePt","",nPt,binsPt);
  TProfile *pJetChemefInProbePt  = new TProfile("pJetChemefInProbePt","",nPt,binsPt);
  TProfile *pJetMuefInProbePt  = new TProfile("pJetMuefInProbePt","",nPt,binsPt);
  TProfile *pJetChpv0efInProbePt  = new TProfile("pJetChpv0efInProbePt","",nPt,binsPt);
  
  // for (int i = 0; i != 72; ++i) cout << Form("%1.3f, ",-TMath::Pi()+i*TMath::TwoPi()/72.); cout << endl;
  const int nPhi = 72;
  const double binsPhi[nPhi+1] = 
    {-3.142, -3.054, -2.967, -2.880, -2.793, -2.705, -2.618, -2.531, -2.443,
     -2.356, -2.269, -2.182, -2.094, -2.007, -1.920, -1.833, -1.745, -1.658,
     -1.571, -1.484, -1.396, -1.309, -1.222, -1.134, -1.047, -0.960, -0.873,
     -0.785, -0.698, -0.611, -0.524, -0.436, -0.349, -0.262, -0.175, -0.087,
     0.000, 0.087, 0.175, 0.262, 0.349, 0.436, 0.524, 0.611, 0.698, 0.785,
     0.873, 0.960, 1.047, 1.134, 1.222, 1.309, 1.396, 1.484, 1.571, 1.658,
     1.745, 1.833, 1.920, 2.007, 2.094, 2.182, 2.269, 2.356, 2.443, 2.531,
     2.618, 2.705, 2.793, 2.880, 2.967, 3.054,3.142};
  // 2D composition and response
  TProfile2D *p2DbRespInJetEtaJetPhiProbePt230  = new TProfile2D("p2DbRespInJetEtaJetPhiProbePt230","",nEta,binsEta,nPhi,binsPhi);
  TProfile2D *p2MpfRespInJetEtaJetPhiProbePt230 = new TProfile2D("p2MpfRespInJetEtaJetPhiProbePt230","",nEta,binsEta,nPhi,binsPhi);
  TProfile2D *p2JetChhefInJetEtaJetPhiProbePt230 = new TProfile2D("p2JetChhefInJetEtaJetPhiProbePt230","",nEta,binsEta,nPhi,binsPhi);
  TProfile2D *p2JetNehefInJetEtaJetPhiProbePt230 = new TProfile2D("p2JetNehefInJetEtaJetPhiProbePt230","",nEta,binsEta,nPhi,binsPhi);
  TProfile2D *p2JetNeemefInJetEtaJetPhiProbePt230 = new TProfile2D("p2JetNeemefInJetEtaJetPhiProbePt230","",nEta,binsEta,nPhi,binsPhi);
  TProfile2D *p2JetChemefInJetEtaJetPhiProbePt230 = new TProfile2D("p2JetChemefInJetEtaJetPhiProbePt230","",nEta,binsEta,nPhi,binsPhi);
  TProfile2D *p2JetMuefInJetEtaJetPhiProbePt230 = new TProfile2D("p2JetMuefInJetEtaJetPhiProbePt230","",nEta,binsEta,nPhi,binsPhi);
  TProfile2D *p2JetChfpv0efInJetEtaJetPhiProbePt230 = new TProfile2D("p2JetChfpv0efInJetEtaJetPhiProbePt230","",nEta,binsEta,nPhi,binsPhi);
  // Plots of npvgood, npvall vs mu
  TProfile *pNpuInNpuProbePt230 = new TProfile("pNpuInNpuProbePt230","",100,0,100);
  TProfile *pRhoInNpuProbePt230 = new TProfile("pRhoInNpuProbePt230","",100,0,100);
  TProfile *pNpvgoodInNpuProbePt230 = new TProfile("pNpvgoodInNpuProbePt230","",100,0,100);
  TProfile *pNpvInNpuProbePt230 = new TProfile("pNpvInNpuProbePt230","",100,0,100);
  // ...and vs pT
  TProfile *pNpuInProbePt = new TProfile("pNpuInProbePt","",nPt,binsPt);
  TProfile *pRhoInProbePt = new TProfile("pRhoInProbePt","",nPt,binsPt);
  TProfile *pNpvgoodInProbePt = new TProfile("pNpvgoodInProbePt","",nPt,binsPt);
  TProfile *pNpvInProbePt = new TProfile("pNpvInProbePt","",nPt,binsPt);

  TH2D *h2ProbePtDbRespPassMpf = new TH2D("h2ProbePtDbRespPassMpf","",nPt,binsPt,200,0,4);
  TH2D *h2ProbePtMpfRespPassDb = new TH2D("h2ProbePtMpfRespPassDb","",nPt,binsPt,300,-2,4);
  TH2D *h2ProbePtDbRespPassBoth = new TH2D("h2ProbePtDbRespPassBoth","",nPt,binsPt,200,0,4);
  TH2D *h2ProbePtMpfRespPassBoth = new TH2D("h2ProbePtMpfRespPassBoth","",nPt,binsPt,300,-2,4);

  //------------------------------------
  // Variables after response cuts 
  //------------------------------------
  fout->mkdir("passResponse");
  fout->cd("passResponse");
   
  Int_t runN = 20000;
  Double_t runMin = 370000.5;
  Double_t runMax = 390000.5;

  // Time stability of xsec
  TH1D *hRunProbePt30 = new TH1D("hRunProbePt30",";Run;N_{events};",runN, runMin, runMax);
  TH1D *hRunProbePt110 = new TH1D("hRunProbePt110",";Run;N_{events};",runN, runMin, runMax);
  TH1D *hRunProbePt230 = new TH1D("hRunProbePt230",";Run;N_{events};",runN, runMin, runMax);
  
  // Time stability of JEC
  TProfile *pDbRespInRunProbePt30 = new TProfile("pDbRespInRunProbePt30",";Run;BAL;",runN, runMin, runMax);
  TProfile *pDbRespInRunProbePt110 = new TProfile("pDbRespInRunProbePt110",";Run;BAL;",runN, runMin, runMax);
  TProfile *pDbRespInRunProbePt230 = new TProfile("pDbRespInRunProbePt230",";Run;BAL;",runN, runMin, runMax);
  TProfile *pMpfRespInRunProbePt30 = new TProfile("pMpfRespInRunProbePt30",";Run;MPF;",runN, runMin, runMax);
  TProfile *pMpfRespInRunProbePt110 = new TProfile("pMpfRespInRunProbePt110",";Run;MPF;",runN, runMin, runMax);
  TProfile *pMpfRespInRunProbePt230 = new TProfile("pMpfRespInRunProbePt230",";Run;MPF;",runN, runMin, runMax);
   
  // Time stability of PF composition
  TProfile *pJetChhefInRunProbePt30 = new TProfile("pJetChhefInRunProbePt30",";Run;CHF;",runN, runMin, runMax);
  TProfile *pJetChhefInRunProbePt110 = new TProfile("pJetChhefInRunProbePt110",";Run;CHF;",runN, runMin, runMax);
  TProfile *pJetChhefInRunProbePt230 = new TProfile("pJetChhefInRunProbePt230",";Run;CHF;",runN, runMin, runMax);

  TProfile *pJetNehefInRunProbePt30 = new TProfile("pJetNehefInRunProbePt30",";Run;NHF;",runN, runMin, runMax);
  TProfile *pJetNehefInRunProbePt110 = new TProfile("pJetNehefInRunProbePt110",";Run;NHF;",runN, runMin, runMax);
  TProfile *pJetNehefInRunProbePt230 = new TProfile("pJetNehefInRunProbePt230",";Run;NHF;",runN, runMin, runMax);

  TProfile *pJetNeemefInRunProbePt30 = new TProfile("pJetNeemefInRunProbePt30",";Run;NHF;",runN, runMin, runMax);
  TProfile *pJetNeemefInRunProbePt110 = new TProfile("pJetNeemefInRunProbePt110",";Run;NHF;",runN, runMin, runMax);
  TProfile *pJetNeemefInRunProbePt230 = new TProfile("pJetNeemefInRunProbePt230",";Run;NHF;",runN, runMin, runMax);
   

    
  curdir->cd();
  
  //------------------------------------
  // Event for loop 
  //------------------------------------
  TLorentzVector p4Probe, p4GenProbe, p4Jet1, p4Jet2, p4Jetn;
  TLorentzVector p4Met, p4Met1, p4Metn, p4Metu, p4Metnu, p4RawMet, p4CorrMet, p4RawProbe;
  TLorentzVector p4Jeti, p4CorrJets, p4RawJet, p4RawJets, p4RcJets, p4RcOffsets;
  TLorentzVector p4GenJeti, p4GenJet1, p4GenJet2;
  TLorentzVector p4Probex; // for MPFX
   
  Long64_t nentries = tree->GetEntries(); 
  cout << "\nStarting loop over " << nentries << " entries" << endl;
 
  std::cout<<"---------------------------"<<std::endl;
  std::cout<<setw(10)<<"Progress"<<setw(10)<<"Time"<<std::endl;
  std::cout<<"---------------------------"<<std::endl;
  double totTime = 0.0;
	auto startClock = std::chrono::high_resolution_clock::now();
 
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    //if (jentry>100) break;
		if(nentries > 100  && jentry%(nentries/100) == 0){// print after every 1% of events
      totTime+= std::chrono::duration<double>(std::chrono::high_resolution_clock::now()-startClock).count();
      int sec = (int)(totTime)%60;
      int min = (int)(totTime)/60;
	    std::cout<<setw(10)<<100*jentry/nentries<<" %"<<setw(10)<<min<<"m "<<sec<<"s"<<std::endl;
			startClock = std::chrono::high_resolution_clock::now();
		}
   
    int count_passedCut = 0;
    Long64_t ientry = tree->loadEntry(jentry);
    if (ientry < 0) break; 
    tree->fChain->GetTree()->GetEntry(ientry);

    // Weight
    double weight = (isMC ? tree->genWeight : 1);
    //------------------------------------
    // trigger and golden lumi, MET filter selection 
    //------------------------------------
    bool pass_HLT = tree->HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
    if(!pass_HLT) continue; // passHLT
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);

    bool passGoodLumi = true; 
    if(isData){
      passGoodLumi = objS->checkGoodLumi(tree->run, tree->luminosityBlock);
    }
    if(!passGoodLumi) continue; // passGoodLumi
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);

    //bool pass_Filter = eventPick->passFilter(tree);
    //if(!pass_Filter) continue; // passMetFilter
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);

    // Pileup
    double TruePUrms(0);
    //AskMikko
    //if (isData) tree->Pileup_nTrueInt = objS->getTruePU(tree->run,tree->luminosityBlock,&TruePUrms);

    //------------------------------------------
    // Select Electrons
    //------------------------------------------
    int nProbe(0);
    p4GenProbe.SetPtEtaPhiM(0,0,0,0);
    p4Probe.SetPtEtaPhiM(0,0,0,0);
    p4RawProbe.SetPtEtaPhiM(0,0,0,0);
    
    vector<int> indexLeps;
    for(int i = 0; i < tree->nEle_; ++i){
      double eta = tree->eleEta_[i];
      double SCeta = eta + tree->eleDeltaEtaSC_[i];
      double absEta = TMath::Abs(eta);
      double absSCEta = TMath::Abs(SCeta);
      bool passEtaEBEEGap = (absSCEta < 1.4442) || (absSCEta > 1.566);
      if(passEtaEBEEGap && absEta <= 2.4 && tree->elePt_[i] >= 40.0 && tree->eleID_[i]){
        indexLeps.push_back(i);
      }
    }
    if(indexLeps.size()<2) continue;
    
    //------------------------------------------
    // Select Probe
    //------------------------------------------
    vector<TLorentzVector> p4Probes;
    for (int j=0; j<indexLeps.size(); j++){
    	for (int k=0; k<indexLeps.size(); k++){
    		if(j==k) 
    			continue;
    		if((tree->eleCharge_[j])*(tree->eleCharge_[k]) == 1)
    			continue;
    		TLorentzVector p4Lep1;
    		TLorentzVector p4Lep2;
    		p4Lep1.SetPtEtaPhiM(tree->elePt_[j], tree->eleEta_[j], tree->elePhi_[j], tree->eleMass_[j]);
    		p4Lep2.SetPtEtaPhiM(tree->elePt_[k], tree->eleEta_[k], tree->elePhi_[k], tree->eleMass_[k]);
    		TLorentzVector ZBoson = p4Lep1 + p4Lep2;
    		if ( abs(ZBoson.M() - 91.1876) > 10 ) 
    			continue;
    		p4Probes.push_back(ZBoson);
    	}
    }
    nProbe = p4Probes.size();
    if(nProbe<1) continue; // passAtleast1Probe
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);

    p4RawProbe = p4Probes.at(0);
    double ptProbe = p4Probes.at(0).Pt();
    if(!ptProbe>0.0) continue;

    //Fill some histograms
    h2ProbePtCountProbe->Fill(ptProbe, nProbe, weight);
    hProbePt ->Fill(ptProbe, weight);
    h2ProbeEtaProbePhi->Fill(p4RawProbe.Eta(), p4RawProbe.Phi(), weight);
    h2ProbeEtaProbePhiRebin->Fill(p4RawProbe.Eta(), p4RawProbe.Phi(), weight);
    h2ProbePtNpu->Fill(ptProbe, tree->Pileup_nTrueInt, weight);
    if (ptProbe>=110){
      h2ProbeEtaProbePhiProbePt110->Fill(p4RawProbe.Eta(), p4RawProbe.Phi(), weight);
      h2ProbeEtaProbePhiProbePt110Rebin->Fill(p4RawProbe.Eta(), p4RawProbe.Phi(), weight);
    }
    if (ptProbe>=210){
      hNpuProbePt210->Fill(tree->Pileup_nTrueInt, weight);
    }
    
    //------------------------------------------
    // Select Gen Electrons
    //------------------------------------------
    if(isMC){
      vector<int> indexGenLeps;
      for(int i = 0; i < tree->nGenDressedLepton; ++i){
      	if(fabs(tree->GenDressedLepton_pdgId[i])==11) 
          indexGenLeps.push_back(i);
      }
      if(indexGenLeps.size() < 2) continue;
      TLorentzVector p4GenLep1;
      TLorentzVector p4GenLep2;
      int i0 = indexGenLeps.at(0);
      int i1 = indexGenLeps.at(1);
      p4GenLep1.SetPtEtaPhiM(tree->GenDressedLepton_pt[i0], tree->GenDressedLepton_eta[i0], 
                tree->GenDressedLepton_phi[i0], tree->GenDressedLepton_mass[i0]);
      p4GenLep2.SetPtEtaPhiM(tree->GenDressedLepton_pt[i1], tree->GenDressedLepton_eta[i1],
                tree->GenDressedLepton_phi[i1], tree->GenDressedLepton_mass[i1]);
      p4GenProbe = p4GenLep1 + p4GenLep2;
      //Fill some histograms
      if (p4GenProbe.Pt()>0 && fabs(p4GenProbe.Eta()) < 1.3) {
        hGenProbePt->Fill(p4GenProbe.Pt(), weight);
      }
      if (fabs(p4Probe.Eta()) < 1.3) {
        if (p4GenProbe.Pt()>0) {
          hProbePtBarrel->Fill(ptProbe, weight);
          pProbePtOgenProbePtInGenProbePt->Fill(p4GenProbe.Pt(), ptProbe / p4GenProbe.Pt(), weight);
          pGenProbePtOProbePtInProbePt->Fill(ptProbe, p4GenProbe.Pt() / ptProbe, weight);
        }
      }
    }//isMC
      
    //------------------------------------------------
    // Jet loop: Apply JEC
    //------------------------------------------------
    // Select leading jets. Just exclude Probe, don't apply JetID yet
    Float_t     Jet_resFactor[tree->nJetMax]; // Custom addition
    Float_t     Jet_deltaJES[tree->nJetMax]; // Custom addition
    Float_t     Jet_CF[tree->nJetMax]; // Custom addition
    int iJet(-1), iJet2(-1), nJets(0);
    double djes(1), jes(1), res(1);
    p4Jet1.SetPtEtaPhiM(0,0,0,0);
    p4Jet2.SetPtEtaPhiM(0,0,0,0);
    p4Jetn.SetPtEtaPhiM(0,0,0,0);
    // Also calculate corrected type-I chsMET and HDM inputs
    p4CorrJets.SetPtEtaPhiM(0,0,0,0);
    p4RawJets.SetPtEtaPhiM(0,0,0,0);
    p4RcJets.SetPtEtaPhiM(0,0,0,0);
    p4RcOffsets.SetPtEtaPhiM(0,0,0,0);

    for (int i = 0; i != tree->nJet; ++i) {
      double rawJetPt = tree->Jet_pt[i] * (1.0 - tree->Jet_rawFactor[i]);
      double rawJetMass = tree->Jet_mass[i] * (1.0 - tree->Jet_rawFactor[i]);
      double corrs = 1.0;
      if(isDebug)cout<<"---: Jet correction :---"<<endl;
      for(auto l2l3Ref:objS->loadedJetL2L3Refs){
        try{ 
          //cout<<"rawJetPt = "<<rawJetPt<<", corr = "<<tree->Jet_eta[i]<<endl;
          //auto corr = l2l3Ref->evaluate({tree->Jet_eta[i],rawJetPt}); 
          double corr = 1.0;
          corrs  = corr*corrs;
          rawJetPt = corrs*rawJetPt;
          rawJetMass = corrs*rawJetMass;
          if(isDebug) cout<<"rawJetPt = "<<rawJetPt<<", corr = "<<corr<<", corrs = "<<corrs<<endl;
        } catch (const std::exception& e) {
          cout<<"\nEXCEPTION: in l2l3Ref: "<<e.what()<<endl;
          std::abort();
        }
      }
      //double res = (v.size()>1 ? v[v.size()-1]/v[v.size()-2] : 1.);
      //Jet_RES[i] = 1./res;
      res  = 1.0;
      Jet_deltaJES[i] = (1./corrs) / (1.0 - tree->Jet_rawFactor[i]);
      tree->Jet_pt[i] = rawJetPt ;
      tree->Jet_mass[i] = rawJetMass; 
      tree->Jet_rawFactor[i] = (1.0 - 1.0/corrs);
      Jet_resFactor[i] = (1.0 - 1.0/res);
      // Smear jets
      if (smearJets) {
        //assert(false);
      }
     
      // Check that jet is not Probe and pTcorr>15 GeV
      bool pass_JetId = tree->Jet_jetId[i] & (1 << 2); // tightLepVeto
      if (tree->Jet_pt[i]>15 && pass_JetId) {
        p4Jeti.SetPtEtaPhiM(tree->Jet_pt[i], tree->Jet_eta[i], tree->Jet_phi[i], tree->Jet_mass[i]);
        if (p4Probe.DeltaR(p4Jeti)<0.2) continue; // should not happen, but does?
        ++nJets;
      
        if (iJet==-1) { // Leading jet for balance
          iJet = i;
          p4Jet1 = p4Jeti;
          djes = Jet_deltaJES[i];
          jes = (1.-tree->Jet_rawFactor[i]);
          res = (1.-Jet_resFactor[i]);
        }
        else { // Subleading jets 
          p4Jetn += p4Jeti;
          if (iJet2==-1) { // First subleading jet for alpha
          iJet2 = i;
          p4Jet2 = p4Jeti;
          }
        }
        // Calculate L1RC correction
        p4RawJet = (1-tree->Jet_rawFactor[i]) * p4Jeti;
        double corrl1rc(1.); // tree->isRun3
        p4RcJets = corrl1rc * p4RawJet;
        
        // Corrected type-I chsMET calculation
        p4CorrJets += p4Jeti;
        p4RawJets += p4RawJet;
        p4RcJets += p4RcJets;
        p4RcOffsets += (p4RawJet - p4RcJets);
      } // non-Probe jet
    } // for i in nJet
    
    if(nJets < 1) continue; // passAtleast1Jet
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);

    //------------------------------------------------
    //Event to be vetoed if leading jet falls in veto region
    //------------------------------------------------
    try{ 
      auto jvNumber= objS->loadedJetVetoRef->evaluate({objS->jetVetoKey, p4Jet1.Eta(), p4Jet1.Phi()});
    if(jvNumber>0) continue; // passJetVetoMap
    } catch (const std::exception& e) {
      cout<<"\nEXCEPTION: in objS->loadedJetVetoRef: "<<e.what()<<endl;
      std::abort();
    }
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);
     
    //------------------------------------------------
    // GenJet loop
    //------------------------------------------------
    // Select p4GenJet1 matching leading and subleading reco jet
    int iGenJet(-1), iGenJet2(-1);
    p4GenJet1.SetPtEtaPhiM(0,0,0,0);
    p4GenJet2.SetPtEtaPhiM(0,0,0,0);
    if (isMC) {
      for (Int_t i = 0; i != tree->nGenJet; ++i) {
   	      p4GenJeti.SetPtEtaPhiM(tree->GenJet_pt[i],tree->GenJet_eta[i],
                  tree->GenJet_phi[i], tree->GenJet_mass[i]);
        if (iJet!=-1 && p4GenJeti.DeltaR(p4Jet1)<0.4 && iGenJet==-1) {
          iGenJet = i;
          p4GenJet1 = p4GenJeti;
        }
        else if (iJet2!=-1 && p4GenJeti.DeltaR(p4Jet2)<0.4 && iGenJet2==-1) {
          iGenJet2 = i;
          p4GenJet2 = p4GenJeti;
        }
      } // for i in tree->nGenJet
    } // isMC
    bool pass_gen = (iGenJet!=-1);
   
    //------------------------------------------------
    // Set MET vectors
    //------------------------------------------------
    p4RawMet.SetPtEtaPhiM(tree->RawPuppiMET_pt, 0, tree->RawPuppiMET_phi, 0);
    p4RawMet += p4RawProbe - p4Probe; // replace PF Probe with Reco Probe
    p4Met1 = -p4Jet1 - p4Probe;
    p4Metn = -p4Jetn;
    //p4CorrMet = p4RawMet +p4RawJets -p4CorrJets -p4RcOffsets;
    p4CorrMet = p4RawMet +p4RcJets -p4CorrJets; // same as above
    // Unclustered MET from rawMET by taking out all the hard stuff
    // p4Metu = p4RawMet +Probe +p4RawJets -p4RcOffsets;
    // p4Metu = p4RawMet +Probe +p4RcJets;
    // Or equally well, from corrMET (modulo rounding errors)
    p4Metu = p4CorrMet + p4Probe +p4CorrJets;
    p4Metnu = p4Metn + 1.1*p4Metu;
    p4Met = p4CorrMet;
    
    // Make MET transverse
    p4CorrMet.SetPz(0);
    p4Met.SetPz(0);
    p4Metn.SetPz(0);
    p4Met1.SetPz(0);
    p4Metu.SetPz(0);
    
    //------------------------------------------------
    // Calculate basic variables
    //------------------------------------------------
    double ptjet = p4Jet1.Pt();
    double abseta = fabs(p4Jet1.Eta());
    double ptJet2 = p4Jet2.Pt();
    double ptJet2Min = 30;
    double bal(0), mpf(0), mpf1(0), mpfn(0), mpfu(0), mpfnu(0);
    double mpfx(0), mpf1x(0), mpfnPt(0), mpfux(0), mpfnux(0);
    bal = ptjet / ptProbe;
    mpf = 1 + p4Met.Vect().Dot(p4Probe.Vect()) / (ptProbe*ptProbe);
    mpf1 = 1 + p4Met1.Vect().Dot(p4Probe.Vect()) / (ptProbe*ptProbe);
    mpfn = p4Metn.Vect().Dot(p4Probe.Vect()) / (ptProbe*ptProbe);
    mpfu = p4Metu.Vect().Dot(p4Probe.Vect()) / (ptProbe*ptProbe);
    mpfnu = p4Metnu.Vect().Dot(p4Probe.Vect()) / (ptProbe*ptProbe);
    //
    p4Probex.SetPtEtaPhiM(p4Probe.Pt(),p4Probe.Eta(),p4Probe.Phi()+0.5*TMath::Pi(),0.);
    mpfx = 1 + p4Met.Vect().Dot(p4Probex.Vect()) / (ptProbe*ptProbe);
    mpf1x = 1 + p4Met1.Vect().Dot(p4Probex.Vect()) / (ptProbe*ptProbe);
    mpfnPt = p4Metn.Vect().Dot(p4Probex.Vect()) / (ptProbe*ptProbe);
    mpfux = p4Metu.Vect().Dot(p4Probex.Vect()) / (ptProbe*ptProbe);
    mpfnux = p4Metnu.Vect().Dot(p4Probex.Vect()) / (ptProbe*ptProbe);
     
    // Sanity checks for HDM inputs
    if (!(fabs(mpf1+mpfn+mpfu-mpf)<1e-4)) {
      cout << "\nHDM input error: mpf=" << mpf << " mpf1=" << mpf1
        << " mpfn=" << mpfn << " mpfu=" << mpfu << endl;
      cout << "Difference = " << mpf1+mpfn+mpfu-mpf << endl << flush;
      //assert(false);
      cout << "Skip entry " << jentry
        << " ("<<tree->run<<","<<tree->luminosityBlock<<","<<tree->event<<")"
        << " in file " << fout->GetName() << endl << flush;
      continue;
    }
   
    bool pass_dPhiProbJet1 = (fabs(p4Probe.DeltaPhi(p4Jet1)) > 2.7); // pi-0.44 as in KIT Probe+j
    if(!pass_dPhiProbJet1) continue; // passDPhiProbeJet1
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);

    if(fabs(p4Probe.Eta()) > 1.3) continue; // passProbeBarrel
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);
   	hDphiJetProbe->Fill(p4Probe.DeltaPhi(p4Jet1), weight);
   	hDrJetProbe->Fill(p4Probe.DeltaR(p4Jet1), weight);

    if (pass_gen || isData) {
      pJ1ptOprobePtInProbePt->Fill(ptProbe, p4Jet1.Pt() / ptProbe, weight);
    }
    if (pass_gen) {
      pGenJ1ptOprobePtInProbePt->Fill(ptProbe, p4GenJet1.Pt() / ptProbe, weight);
      pJ1ptOgenJ1ptInGenJ1pt->Fill(p4GenJet1.Pt(), p4Jet1.Pt() / p4GenJet1.Pt(), weight);
    }
    int flv = (isMC ? tree->GenJet_partonFlavour[iGenJet] : 99);
    mvar["hProbePt"] = 1;
    mvar["pMpfRespInProbePt"] = mpf;
    mvar["pDbRespInProbePt"] = bal;
    mvar["pMpfResp1InProbePt"] = mpf1;
    mvar["pMpfRespNInProbePt"] = mpfn;
    mvar["pMpfRespUInProbePt"] = mpfu;
    mvar["pRhoInProbePt"] = tree->fixedGridRhoFastjetAll;
    mvar["pJet1PtOProbePtInProbePt"] = (p4Jet1.Pt() / ptProbe);
    mvar["pGenJet1PtOProbePtInProbePt"] = (p4GenJet1.Pt() / ptProbe);
    mvar["pJet1PtOGenJet1PtInGenJet1Pt"] = (p4GenJet1.Pt()!=0 ? p4Jet1.Pt() / p4GenJet1.Pt() : 0);
    
    tree->Jet_btagDeepB[iJet] = tree->Jet_btagDeepFlavB[iJet];
    tree->Jet_btagDeepC[iJet] = 0.5*(tree->Jet_btagDeepFlavCvB[iJet] +
    		     tree->Jet_btagDeepFlavCvL[iJet]);
    tree->Jet_qgl[iJet] = tree->Jet_btagDeepFlavQG[iJet];
    bool isb = (tree->Jet_btagDeepB[iJet] > objS->bThresh);
    bool isc = (tree->Jet_btagDeepC[iJet] > objS->cThresh && !isb);
    bool isq = (tree->Jet_qgl[iJet]>=0.5 && tree->Jet_qgl[iJet] && !isb && !isc);
    bool isg = (tree->Jet_qgl[iJet]>=0 && tree->Jet_qgl[iJet]<0.5 && !isb && !isc);
    bool isn = (!isb && !isc && !isq && !isg);
    
    for (int ivar = 0; ivar != nvar; ++ivar) {
      for (int itag = 0; itag != ntag; ++itag) {
        for (int iflv = 0; iflv != nflv; ++iflv) {
          string& svr = avar[ivar]; const char *cv = svr.c_str();
          string& stg = atag[itag]; const char *ct = stg.c_str();
          string& sfl = aflv[iflv]; const char *cf = sfl.c_str();
          
          if (((sfl=="i") ||
             (sfl=="b" && abs(flv)==5) ||
             (sfl=="c" && abs(flv)==4) ||
             (sfl=="q" && abs(flv)<=3 && flv!=0) ||
             (sfl=="s" && abs(flv)==3) ||
             (sfl=="ud" && abs(flv)<=2 && flv!=0) ||
             (sfl=="g" && flv==21) ||
             (sfl=="n" && flv==0)) &&
            ((stg=="i") ||
             (stg=="b" && isb) ||
             (stg=="c" && isc) ||
             (stg=="q" && isq) ||
             (stg=="g" && isg) ||
             (stg=="n" && isn))) {
             double x = ptProbe;
             if (svr=="pJet1PtOGenJet1PtInGenJet1Pt") x = p4GenJet1.Pt();
             if ((svr=="pJet1PtOProbePtInProbePt" || svr=="pGenJet1PtOProbePtInProbePt") && iGenJet==-1) x = 0;
             double var = mvar[svr];
             TH1* h = mp[svr][stg][sfl];
             if (!h) {
               cout << "Missing "<<svr<<stg<<sfl<<endl<<flush;
               assert(h);
             }
          
             if (svr=="hProbePt")
               ((TH1D*)h)->Fill(x, weight);
             else
               ((TProfile*)h)->Fill(x, var, weight);
          }
        } // for iflv
      } // for itag
    } // for ivar

    bool pass_Jet1Eta = (abseta < 1.3);
    bool pass_Jet2Pt = (ptJet2 < ptProbe || ptJet2 < ptJet2Min);    
    if(!(pass_Jet1Eta &&  pass_Jet2Pt)) continue; // passJet1EtaJet2Pt
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);
    h2ProbePtDbResp->Fill(ptProbe, bal, weight);
    h2ProbePtMpfResp->Fill(ptProbe, mpf, weight);
    if (p4Jet2.Pt()>0) {
      if (iGenJet2!=-1 || !isMC) {
        pJ2ptOprobePtInProbePt->Fill(ptProbe, p4Jet2.Pt() / ptProbe, weight);
      }
      if (iGenJet2!=-1) {
        pGenJ2ptOprobePtInProbePt->Fill(ptProbe, p4GenJet2.Pt() / ptProbe, weight);
        pJ2ptOgenJ2ptInGenJ2pt->Fill(p4GenJet2.Pt(), p4Jet2.Pt() / p4GenJet2.Pt(), weight);
      }
    }
    // Control plots for Probe-jet 
    // PF composition plots
    h2JetEtaProbePt->Fill(ptProbe, tree->Jet_eta[iJet], weight);
    pJetEtaInProbePt->Fill(ptProbe, fabs(tree->Jet_eta[iJet]), weight);
    
    // 1D composition and response
    pDbRespInProbePt->Fill(ptProbe, bal, weight);
    pMpfRespInProbePt->Fill(ptProbe, mpf, weight);
    pJetChhefInProbePt->Fill(ptProbe, tree->Jet_chHEF[iJet], weight);
    pJetNehefInProbePt->Fill(ptProbe, tree->Jet_neHEF[iJet], weight);
    pJetNeemefInProbePt->Fill(ptProbe, tree->Jet_neEmEF[iJet], weight);
    pJetChemefInProbePt->Fill(ptProbe, tree->Jet_chEmEF[iJet], weight);
    pJetMuefInProbePt->Fill(ptProbe, tree->Jet_muEF[iJet], weight);
    //pJetChpv0efInProbePt->Fill(ptProbe, tree->Jet_chFPV0EF[iJet], weight);
    
    pNpuInProbePt->Fill(ptProbe, tree->Pileup_nTrueInt, weight);
    pRhoInProbePt->Fill(ptProbe, tree->fixedGridRhoFastjetAll, weight);
    pNpvgoodInProbePt->Fill(ptProbe, tree->PV_npvsGood, weight);
    pNpvInProbePt->Fill(ptProbe, tree->PV_npvs, weight);
    // 2D composition and response
    if (ptProbe>230) {
      double eta = tree->Jet_eta[iJet];
      double phi = tree->Jet_phi[iJet];
      p2DbRespInJetEtaJetPhiProbePt230->Fill(eta, phi, bal, weight);
      p2MpfRespInJetEtaJetPhiProbePt230->Fill(eta, phi, mpf, weight);
      p2JetChhefInJetEtaJetPhiProbePt230->Fill(eta, phi, tree->Jet_chHEF[iJet], weight);
      p2JetNehefInJetEtaJetPhiProbePt230->Fill(eta, phi, tree->Jet_neHEF[iJet], weight);
      p2JetNeemefInJetEtaJetPhiProbePt230->Fill(eta, phi, tree->Jet_neEmEF[iJet], weight);
      p2JetChemefInJetEtaJetPhiProbePt230->Fill(eta, phi, tree->Jet_chEmEF[iJet], weight);
      p2JetMuefInJetEtaJetPhiProbePt230->Fill(eta, phi, tree->Jet_muEF[iJet], weight);
      //p2JetChfpv0efInJetEtaJetPhiProbePt230->Fill(eta, phi, tree->Jet_chFPV0EF[iJet], weight);
      pNpuInNpuProbePt230->Fill(tree->Pileup_nTrueInt, tree->Pileup_nTrueInt, weight);
      pRhoInNpuProbePt230->Fill(tree->Pileup_nTrueInt, tree->fixedGridRhoFastjetAll, weight);
      pNpvgoodInNpuProbePt230->Fill(tree->Pileup_nTrueInt, tree->PV_npvsGood, weight);
      pNpvInNpuProbePt230->Fill(tree->Pileup_nTrueInt, tree->PV_npvs, weight);
    } // high pT range
    

    bool pass_DbResp = (fabs(1-bal)<0.7);
    bool pass_MpfResp = (fabs(1-mpf)<0.7);
    if (pass_MpfResp) h2ProbePtDbRespPassMpf->Fill(ptProbe, bal, weight);
    if (pass_DbResp) h2ProbePtMpfRespPassDb->Fill(ptProbe, mpf, weight);
    if (pass_MpfResp && pass_DbResp) h2ProbePtDbRespPassBoth->Fill(ptProbe, bal, weight);
    if (pass_MpfResp && pass_DbResp) h2ProbePtMpfRespPassBoth->Fill(ptProbe, mpf, weight);

    if(!(pass_DbResp && pass_MpfResp)) continue; // passResponse
    count_passedCut++;
    hCutflow->Fill(count_passedCut);
    hCutflowWeight->Fill(count_passedCut, weight);

    if (ptProbe>30) {
      hRunProbePt30->Fill(tree->run, weight); 
      pDbRespInRunProbePt30->Fill(tree->run, bal, weight); 
      pMpfRespInRunProbePt30->Fill(tree->run, mpf, weight);
      pJetChhefInRunProbePt30->Fill(tree->run, tree->Jet_chHEF[iJet], weight);
      pJetNehefInRunProbePt30->Fill(tree->run, tree->Jet_neHEF[iJet], weight);
      pJetNeemefInRunProbePt30->Fill(tree->run, tree->Jet_neEmEF[iJet], weight);
    }
    if (ptProbe>110) {
      hRunProbePt110->Fill(tree->run, weight);
      pDbRespInRunProbePt110->Fill(tree->run, bal, weight); 
      pMpfRespInRunProbePt110->Fill(tree->run, mpf, weight);
      pJetChhefInRunProbePt110->Fill(tree->run, tree->Jet_chHEF[iJet], weight);
      pJetNehefInRunProbePt110->Fill(tree->run, tree->Jet_neHEF[iJet], weight);
      pJetNeemefInRunProbePt110->Fill(tree->run, tree->Jet_neEmEF[iJet], weight);
    }
    if (ptProbe>230) {
      hRunProbePt230->Fill(tree->run, weight);
      pDbRespInRunProbePt230->Fill(tree->run, bal, weight); 
      pMpfRespInRunProbePt230->Fill(tree->run, mpf, weight);
      pJetChhefInRunProbePt230->Fill(tree->run, tree->Jet_chHEF[iJet], weight);
      pJetNehefInRunProbePt230->Fill(tree->run, tree->Jet_neHEF[iJet], weight);
      pJetNeemefInRunProbePt230->Fill(tree->run, tree->Jet_neEmEF[iJet], weight);
    }
  }//event for loop
  cout << "\n ===========> Processed " << nentries << " events <=============\n";
  cout<<setw(20)<<"CUT"<< setw(10)<<"ENTRIES" << setw(10)<< "WEIGHT"<< setw(10)<<"CHANGE"<<endl;
  for (int i=1; i<hCutflow->GetNbinsX(); i++){
    int previous = 0;
    if(i==1) previous = nentries; 
    else previous = hCutflow->GetBinContent(i-1);
    int current   = hCutflow->GetBinContent(i);
    int currentWithWeight   = hCutflowWeight->GetBinContent(i);
    double change = ((previous - current) / static_cast<double>(previous)) * 100;
    double weight_ = currentWithWeight/static_cast<double>(current);
    cout<<setw(20)<<hCutflow->GetXaxis()->GetBinLabel(i)<<
    setw(10)<<current << setw(10)<< weight_<< setw(5)<<" - "<<change<<" %" <<endl;
  }

  // Add extra plot for jet response vs Probe pT
  if (isMC) {
    fout->cd("passProbeBarrel");
    TH1D *hrgenvgen = pJ1ptOgenJ1ptInGenJ1pt->ProjectionX("hrgenvgen");
    TH1D *hrgenvProbe = pJ1ptOprobePtInProbePt->ProjectionX("hrgenvProbe");
    hrgenvProbe->Divide(pGenJ1ptOprobePtInProbePt);
    fout->cd("passJet1EtaJet2Pt");
    TH1D *hrgen2vgen = pJ2ptOgenJ2ptInGenJ2pt->ProjectionX("hrgen2vgen");
    TH1D *hrgen2vProbe = pJ2ptOprobePtInProbePt->ProjectionX("hrgen2vProbe");
    hrgen2vProbe->Divide(pGenJ2ptOprobePtInProbePt);
    curdir->cd();
  }
  fout->Write();
  cout<<"Output file: "<<fout->GetName()<<endl;
  fout->Close();
  return 0;
}
   
