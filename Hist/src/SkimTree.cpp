#include<iostream>
#include "SkimTree.h"

SkimTree::SkimTree(TString oName, vector<string>fileNames){
    fChain = new TChain("Events");
    std::cout << "\nINFO: SkimTree::SkimTree()" << std::endl;
    fChain->SetCacheSize(100*1024*1024);
    int nFiles = fileNames.size();
    //string dir = "root://cms-xrd-global.cern.ch/";
    //string dir = "root://cmsxrootd.fnal.gov/";
    string dir = "";
    for(int fileI=0; fileI<nFiles; fileI++){
        string fName = fileNames[fileI];
        fChain->Add( (dir + fName).c_str());
        cout << dir+fName << "  " << fChain->GetEntries() << endl;
    }
	fChain->SetBranchStatus("*",1);
	fChain->SetBranchAddress("run", &run);
	fChain->SetBranchAddress("luminosityBlock", &luminosityBlock);
	fChain->SetBranchAddress("event", &event);

	if(oName.Contains("2022")) is22 = true;
	if(oName.Contains("2023")) is23 = true;
	if(is22 || is23) isRun3 = true;
	if(!oName.Contains("Data")) isMC = true;

    //--------------------------------------- 
    //Jet for all channels 
    //--------------------------------------- 
	fChain->SetBranchAddress("nJet", &nJet);
	fChain->SetBranchAddress("Jet_area", &Jet_area);
	if (!(is22 || is23)) fChain->SetBranchAddress("Jet_btagDeepB", &Jet_btagDeepB);
	if (!(is22 || is23)) fChain->SetBranchAddress("Jet_btagDeepC", &Jet_btagDeepC);
	if (is22 || is23) {
	  fChain->SetBranchAddress("Jet_btagDeepFlavB"   , &Jet_btagDeepFlavB);
	  fChain->SetBranchAddress("Jet_btagDeepFlavCvB" , &Jet_btagDeepFlavCvB);
	  fChain->SetBranchAddress("Jet_btagDeepFlavCvL" , &Jet_btagDeepFlavCvL);
	  fChain->SetBranchAddress("Jet_btagDeepFlavQG"  , &Jet_btagDeepFlavQG);
	}
	fChain->SetBranchAddress("Jet_chEmEF"    , &Jet_chEmEF);
	fChain->SetBranchAddress("Jet_chHEF"     , &Jet_chHEF);
	fChain->SetBranchAddress("Jet_eta"       , &Jet_eta);
	fChain->SetBranchAddress("Jet_mass"      , &Jet_mass);
	fChain->SetBranchAddress("Jet_muEF"      , &Jet_muEF);
	fChain->SetBranchAddress("Jet_neEmEF"    , &Jet_neEmEF);
	fChain->SetBranchAddress("Jet_neHEF"     , &Jet_neHEF);
	fChain->SetBranchAddress("Jet_phi"       , &Jet_phi);
	fChain->SetBranchAddress("Jet_pt"        , &Jet_pt);
	if (!(is22 || is23)) fChain->SetBranchAddress("Jet_qgl", &Jet_qgl);
	fChain->SetBranchAddress("Jet_rawFactor", &Jet_rawFactor);
	fChain->SetBranchAddress("Jet_jetId", &Jet_jetId);
	if (isMC)
    fChain->SetBranchAddress("Jet_genJetIdx", &Jet_genJetIdx);

    //--------------------------------------- 
    // Photon (for GamJet)
    //--------------------------------------- 
	if(oName.Contains("GamJet")){
		fChain->SetBranchAddress("nPhoton", &nPhoton);
		fChain->SetBranchAddress("Photon_eCorr", &Photon_eCorr);
		fChain->SetBranchAddress("Photon_energyErr", &Photon_energyErr);
		fChain->SetBranchAddress("Photon_eta", &Photon_eta);
		fChain->SetBranchAddress("Photon_hoe", &Photon_hoe);
		fChain->SetBranchAddress("Photon_mass", &Photon_mass);
		fChain->SetBranchAddress("Photon_phi", &Photon_phi);
		fChain->SetBranchAddress("Photon_pt", &Photon_pt);
		fChain->SetBranchAddress("Photon_r9", &Photon_r9);
		fChain->SetBranchAddress("Photon_cutBased", &Photon_cutBased);
		fChain->SetBranchAddress("Photon_jetIdx", &Photon_jetIdx);
		fChain->SetBranchAddress("Photon_seedGain", &Photon_seedGain);
		if (is22 || is23) {
	        fChain->SetBranchAddress("HLT_Photon300_NoHE", &HLT_Photon300_NoHE);
	        fChain->SetBranchAddress("HLT_Photon33", &HLT_Photon33);
	        fChain->SetBranchAddress("HLT_Photon50", &HLT_Photon50);
	        fChain->SetBranchAddress("HLT_Photon75", &HLT_Photon75);
	        fChain->SetBranchAddress("HLT_Photon90", &HLT_Photon90);
	        fChain->SetBranchAddress("HLT_Photon120", &HLT_Photon120);
	        fChain->SetBranchAddress("HLT_Photon150", &HLT_Photon150);
	        fChain->SetBranchAddress("HLT_Photon175", &HLT_Photon175);
	        fChain->SetBranchAddress("HLT_Photon200", &HLT_Photon200);
	        
	        fChain->SetBranchAddress("HLT_Photon50_R9Id90_HE10_IsoM", &HLT_Photon50_R9Id90_HE10_IsoM);
	        fChain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM", &HLT_Photon75_R9Id90_HE10_IsoM);
	        fChain->SetBranchAddress("HLT_Photon90_R9Id90_HE10_IsoM", &HLT_Photon90_R9Id90_HE10_IsoM);
	        fChain->SetBranchAddress("HLT_Photon120_R9Id90_HE10_IsoM", &HLT_Photon120_R9Id90_HE10_IsoM);
	        fChain->SetBranchAddress("HLT_Photon165_R9Id90_HE10_IsoM", &HLT_Photon165_R9Id90_HE10_IsoM);
	        
	        fChain->SetBranchAddress("HLT_Photon20_HoverELoose", &HLT_Photon20_HoverELoose);
	        fChain->SetBranchAddress("HLT_Photon30_HoverELoose", &HLT_Photon30_HoverELoose);
	        fChain->SetBranchAddress("HLT_Photon30EB_TightID_TightIso", &HLT_Photon30EB_TightID_TightIso);
	        fChain->SetBranchAddress("HLT_Photon100EBHE10", &HLT_Photon100EBHE10);
	        fChain->SetBranchAddress("HLT_Photon110EB_TightID_TightIso", &HLT_Photon110EB_TightID_TightIso);
		} // is22 || is23
	}

    //--------------------------------------- 
    // Electron (for DiEleJet)
    //--------------------------------------- 
	if(oName.Contains("DiEleJet")){
		//status
		fChain->SetBranchStatus("nElectron",1);
		fChain->SetBranchStatus("Electron_*",1);
		//address
		fChain->SetBranchAddress("nElectron", &nEle_);
		fChain->SetBranchAddress("Electron_charge", &eleCharge_);	
		fChain->SetBranchAddress("Electron_pt", &elePt_);
		fChain->SetBranchAddress("Electron_deltaEtaSC", &eleDeltaEtaSC_);
		fChain->SetBranchAddress("Electron_eta", &eleEta_);
		fChain->SetBranchAddress("Electron_phi", &elePhi_);
		fChain->SetBranchAddress("Electron_mass", &eleMass_);
		fChain->SetBranchAddress("Electron_cutBased", &eleID_);
        //Gen lepton
        fChain->SetBranchAddress("nGenDressedLepton", &nGenDressedLepton);
        fChain->SetBranchAddress("GenDressedLepton_eta", &GenDressedLepton_eta);
        fChain->SetBranchAddress("GenDressedLepton_mass", &GenDressedLepton_mass);
        fChain->SetBranchAddress("GenDressedLepton_phi", &GenDressedLepton_phi);
        fChain->SetBranchAddress("GenDressedLepton_pt", &GenDressedLepton_pt);
		//address trigger
        fChain->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);
    }

    //--------------------------------------- 
    // Muon (for DiMuJet)
    //--------------------------------------- 
    if (oName.Contains("DiMuJet")){
		//status
		fChain->SetBranchStatus("nMuon",1);
		fChain->SetBranchStatus("Muon_*",1);
		//address
		fChain->SetBranchAddress("nMuon", &nMuon_);
		fChain->SetBranchAddress("Muon_charge", &muCharge_);
		fChain->SetBranchAddress("Muon_pt", &muPt_);
		fChain->SetBranchAddress("Muon_eta", &muEta_);
		fChain->SetBranchAddress("Muon_phi", &muPhi_);
		fChain->SetBranchAddress("Muon_mass", &muMass_);
		fChain->SetBranchAddress("Muon_mediumId", &muMediumId_);
		fChain->SetBranchAddress("Muon_tightId", &muTightId_);
		fChain->SetBranchAddress("Muon_highPurity", &muHighPurity_);
		fChain->SetBranchAddress("Muon_pfRelIso04_all", &muPFRelIso_);
		fChain->SetBranchAddress("Muon_tkRelIso", &muTkRelIso_);
		fChain->SetBranchAddress("Muon_dxy", &muDxy_);
		fChain->SetBranchAddress("Muon_dz", &muDz_);
        //Gen lepton
        fChain->SetBranchAddress("nGenDressedLepton", &nGenDressedLepton);
        fChain->SetBranchAddress("GenDressedLepton_eta", &GenDressedLepton_eta);
        fChain->SetBranchAddress("GenDressedLepton_mass", &GenDressedLepton_mass);
        fChain->SetBranchAddress("GenDressedLepton_phi", &GenDressedLepton_phi);
        fChain->SetBranchAddress("GenDressedLepton_pt", &GenDressedLepton_pt);
		//address trigger
        fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8", &HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8);
    }

	if (isRun2) {
	  fChain->SetBranchAddress("ChsMET_phi", &ChsMET_phi);
	  fChain->SetBranchAddress("ChsMET_pt", &ChsMET_pt);
	}
	
	if (isRun3) {
	  fChain->SetBranchAddress("RawPuppiMET_phi", &RawPuppiMET_phi);
	  fChain->SetBranchAddress("RawPuppiMET_pt", &RawPuppiMET_pt);
	}
	if (!isRun3)
	  fChain->SetBranchAddress("fixedGridRhoFastjetAll", &fixedGridRhoFastjetAll);
	if (isRun3)
	  fChain->SetBranchAddress("Rho_fixedGridRhoFastjetAll", &fixedGridRhoFastjetAll);
	fChain->SetBranchAddress("PV_npvs", &PV_npvs);
	fChain->SetBranchAddress("PV_npvsGood", &PV_npvsGood);
	
	fChain->SetBranchAddress("Flag_HBHENoiseFilter", &Flag_HBHENoiseFilter);
	fChain->SetBranchAddress("Flag_HBHENoiseIsoFilter", &Flag_HBHENoiseIsoFilter);
	fChain->SetBranchAddress("Flag_CSCTightHaloFilter", &Flag_CSCTightHaloFilter);
	fChain->SetBranchAddress("Flag_CSCTightHaloTrkMuUnvetoFilter", &Flag_CSCTightHaloTrkMuUnvetoFilter);
	fChain->SetBranchAddress("Flag_CSCTightHalo2015Filter", &Flag_CSCTightHalo2015Filter);
	fChain->SetBranchAddress("Flag_globalTightHalo2016Filter", &Flag_globalTightHalo2016Filter);
	fChain->SetBranchAddress("Flag_globalSuperTightHalo2016Filter", &Flag_globalSuperTightHalo2016Filter);
	fChain->SetBranchAddress("Flag_HcalStripHaloFilter", &Flag_HcalStripHaloFilter);
	fChain->SetBranchAddress("Flag_hcalLaserEventFilter", &Flag_hcalLaserEventFilter);
	fChain->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter);
	fChain->SetBranchAddress("Flag_EcalDeadCellBoundaryEnergyFilter", &Flag_EcalDeadCellBoundaryEnergyFilter);
	fChain->SetBranchAddress("Flag_ecalBadCalibFilter", &Flag_ecalBadCalibFilter);
	fChain->SetBranchAddress("Flag_goodVertices", &Flag_goodVertices);
	fChain->SetBranchAddress("Flag_eeBadScFilter", &Flag_eeBadScFilter);
	fChain->SetBranchAddress("Flag_ecalLaserCorrFilter", &Flag_ecalLaserCorrFilter);
	fChain->SetBranchAddress("Flag_trkPOGFilters", &Flag_trkPOGFilters);
	fChain->SetBranchAddress("Flag_chargedHadronTrackResolutionFilter", &Flag_chargedHadronTrackResolutionFilter);
	fChain->SetBranchAddress("Flag_muonBadTrackFilter", &Flag_muonBadTrackFilter);
	fChain->SetBranchAddress("Flag_BadChargedCandidateFilter", &Flag_BadChargedCandidateFilter);
	fChain->SetBranchAddress("Flag_BadPFMuonFilter", &Flag_BadPFMuonFilter);
	if (isRun3) fChain->SetBranchAddress("Flag_BadPFMuonDzFilter", &Flag_BadPFMuonDzFilter);
	if (isRun3) fChain->SetBranchAddress("Flag_hfNoisyHitsFilter", &Flag_hfNoisyHitsFilter);
	fChain->SetBranchAddress("Flag_BadChargedCandidateSummer16Filter", &Flag_BadChargedCandidateSummer16Filter);
	fChain->SetBranchAddress("Flag_BadPFMuonSummer16Filter", &Flag_BadPFMuonSummer16Filter);
	fChain->SetBranchAddress("Flag_trkPOG_manystripclus53X", &Flag_trkPOG_manystripclus53X);
	fChain->SetBranchAddress("Flag_trkPOG_toomanystripclus53X", &Flag_trkPOG_toomanystripclus53X);
	fChain->SetBranchAddress("Flag_trkPOG_logErrorTooManyClusters", &Flag_trkPOG_logErrorTooManyClusters);
	fChain->SetBranchAddress("Flag_METFilters", &Flag_METFilters);

   if (isMC) {
     fChain->SetBranchAddress("genWeight", &genWeight);
     fChain->SetBranchAddress("nPSWeight", &nPSWeight);
     fChain->SetBranchAddress("PSWeight", &PSWeight);
     fChain->SetBranchAddress("Pileup_nTrueInt", &Pileup_nTrueInt);

     fChain->SetBranchAddress("nGenJet", &nGenJet);
     fChain->SetBranchAddress("GenJet_eta", &GenJet_eta);
     fChain->SetBranchAddress("GenJet_mass", &GenJet_mass);
     fChain->SetBranchAddress("GenJet_phi", &GenJet_phi);
     fChain->SetBranchAddress("GenJet_pt", &GenJet_pt);
     fChain->SetBranchAddress("GenJet_partonFlavour", &GenJet_partonFlavour);

     if (is22 || is23)
       fChain->SetBranchAddress("LHE_HT", &LHE_HT);

     if (!oName.Contains("QCD")) {
       fChain->SetBranchAddress("nGenIsolatedPhoton", &nGenIsolatedPhoton);
       fChain->SetBranchAddress("GenIsolatedPhoton_eta", &GenIsolatedPhoton_eta);
       fChain->SetBranchAddress("GenIsolatedPhoton_mass", &GenIsolatedPhoton_mass);
       fChain->SetBranchAddress("GenIsolatedPhoton_phi", &GenIsolatedPhoton_phi);
       fChain->SetBranchAddress("GenIsolatedPhoton_pt", &GenIsolatedPhoton_pt);
     } // isQCD
   } // isMC

}

std::vector<std::vector<std::string>> SkimTree::splitVector(const std::vector<std::string>& strings, int n) {
    int size = strings.size() / n;  // Size of each small vector
    int remainder = strings.size() % n;  // Remaining elements
    std::vector<std::vector<std::string>> smallVectors;
    int index = 0;
    for (int i = 0; i < n; ++i) {
        if (i < remainder) {
            smallVectors.push_back(std::vector<std::string>(
                        strings.begin() + index, strings.begin() + index + size + 1));
            index += size + 1;
        } else {
            smallVectors.push_back(std::vector<std::string>(
                        strings.begin() + index, strings.begin() + index + size));
            index += size;
        }
    }
    return smallVectors;
}
std::vector<std::string> SkimTree::splitString(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    
    while ((end = s.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(s.substr(start)); // Last token
    
    return tokens;
}

SkimTree::~SkimTree(){
    delete fChain;
    // will be some memory leak due to created vectors
}

Long64_t SkimTree::GetEntries(){
    return fChain->GetEntries();
}

Int_t SkimTree::GetEntry(Long64_t entry){
    fChain->GetEntry(entry);
    return fChain->GetEntries();
}
