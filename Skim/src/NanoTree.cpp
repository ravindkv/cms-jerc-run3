#include "NanoTree.h"

NanoTree::NanoTree(TString oName, vector<string>fileNames){
    chain = new TChain("Events");
    std::cout << "Start NanoTree" << std::endl;
    chain->SetCacheSize(100*1024*1024);
    bool isCopy = false;
    int nFiles = fileNames.size();
    //string dir = "root://cms-xrd-global.cern.ch/";
    string dir = "root://cmsxrootd.fnal.gov/";
    for(int fileI=0; fileI<nFiles; fileI++){
        string fName = fileNames[fileI];
        if(isCopy){
            string singleFile = fName.substr(fName.find_last_of("/")+1,fName.size());
            string xrdcp_command = "xrdcp " + dir + fName + " " + singleFile ;
            cout << xrdcp_command.c_str() << endl;
            system(xrdcp_command.c_str());
            chain->Add( singleFile.c_str());
            cout << singleFile << "  " << chain->GetEntries() << endl;
        }
        else{
            chain->Add( (dir + fName).c_str());
            cout << dir+fName << "  " << chain->GetEntries() << endl;
        }
    }
    std::cout << "Begin" << std::endl;
    chain->SetBranchStatus("*",0);
    // event
    chain->SetBranchStatus("run",1);
    chain->SetBranchStatus("event",1);
    chain->SetBranchStatus("luminosityBlock",1);
    
    //--------------------------------------- 
    //Jet for all channels 
    //--------------------------------------- 
    chain->SetBranchStatus("Jet_btagDeep*",1);
    chain->SetBranchStatus("Jet_chEmEF",1);
    chain->SetBranchStatus("Jet_chHEF" ,1);
    chain->SetBranchStatus("Jet_eta"   ,1);
    chain->SetBranchStatus("Jet_mass"  ,1);
    chain->SetBranchStatus("Jet_muEF"  ,1);
    chain->SetBranchStatus("Jet_neEmEF",1);
    chain->SetBranchStatus("Jet_neHEF" ,1);
    chain->SetBranchStatus("Jet_phi"   ,1);
    chain->SetBranchStatus("Jet_pt"    ,1);
    chain->SetBranchStatus("Jet_qgl",1);
    chain->SetBranchStatus("Jet_rawFactor",1);
    chain->SetBranchStatus("Jet_jetId",1);
    chain->SetBranchStatus("Jet_area",1);
    chain->SetBranchStatus("nJet",1);

    //common branches
    chain->SetBranchStatus("PV_npvs",1);
    chain->SetBranchStatus("PV_npvsGood",1);

    chain->SetBranchStatus("MET_pt",1);
    chain->SetBranchStatus("MET_phi",1);
    chain->SetBranchStatus("ChsMET_pt",1);
    chain->SetBranchStatus("ChsMET_phi",1);
    chain->SetBranchStatus("RawPuppiMET_pt",1);
    chain->SetBranchStatus("RawPuppiMET_phi",1);

    chain->SetBranchStatus("Flag_*",1);
	chain->SetBranchStatus("Rho_fixedGridRhoFastjetAll");
	/*
    chain->SetBranchStatus("TrigObj_*",1);
    chain->SetBranchStatus("nTrigObj",1);
    chain->SetBranchStatus("FatJet_*",1);
    chain->SetBranchStatus("nFatJet",1);
	*/
 
    if (oName.Contains("MC")){
        chain->SetBranchStatus("Jet_genJetIdx",1);
		chain->SetBranchStatus("genWeight");
		chain->SetBranchStatus("nPSWeight");
		chain->SetBranchStatus("PSWeight");
		chain->SetBranchStatus("LHE_HT");
		chain->SetBranchStatus("Pileup_nTrueInt");
        chain->SetBranchStatus("GenJet_*",1);
        chain->SetBranchStatus("nGenJet",1);
		/*
	    chain->SetBranchStatus("Pileup_*",1);
        chain->SetBranchStatus("GenPart_*",1);
        chain->SetBranchStatus("GenJetAK8_*",1);
        chain->SetBranchStatus("nGenPart",1);
        chain->SetBranchStatus("nGenJetAK8",1);
        chain->SetBranchStatus("LHE_*",1);
        // weight
        chain->SetBranchStatus("Generator_weight",1);
        chain->SetBranchStatus("nLHEScaleWeight",1);
        chain->SetBranchStatus("LHEScaleWeight",1);
        chain->SetBranchStatus("nLHEPdfWeight",1);
        chain->SetBranchStatus("nLHEPart",1);
        chain->SetBranchStatus("LHEPart_*",1);
        chain->SetBranchStatus("LHEPdfWeight",1);
        chain->SetBranchStatus("PSWeight",1);
        chain->SetBranchStatus("nPSWeight",1);
        chain->SetBranchStatus("genWeight",1);
		*/
    }//MC

    //--------------------------------------- 
    // Photon (for GamJet)
    //--------------------------------------- 
    if(oName.Contains("GamJet")){
   		chain->SetBranchStatus("Photon_eCorr",1); 
   		chain->SetBranchStatus("Photon_energyErr",1);
   		chain->SetBranchStatus("Photon_eta",1);
   		chain->SetBranchStatus("Photon_hoe",1);
   		chain->SetBranchStatus("Photon_mass",1);
   		chain->SetBranchStatus("Photon_phi",1);
   		chain->SetBranchStatus("Photon_pt",1);
   		chain->SetBranchStatus("Photon_r9",1);
   		chain->SetBranchStatus("Photon_cutBased",1);
   		chain->SetBranchStatus("Photon_jetIdx",1);
   		chain->SetBranchStatus("Photon_seedGain",1);
        chain->SetBranchStatus("nPhoton",1);
        chain->SetBranchStatus("HLT_Photon*", 1);
    	if (oName.Contains("MC")){
            chain->SetBranchStatus("GenIsolatedPhoton_*",1);
            chain->SetBranchStatus("nGenIsolatedPhoton",1);
		}
        //address trigger
        if(oName.Contains("2022")){ 
            chain->SetBranchAddress("HLT_Photon300_NoHE"                                                , & HLT_Photon300_NoHE                                                   , & b_HLT_Photon300_NoHE);
            chain->SetBranchAddress("HLT_Photon20"                                                      , & HLT_Photon20                                                         , & b_HLT_Photon20);
            chain->SetBranchAddress("HLT_Photon33"                                                      , & HLT_Photon33                                                         , & b_HLT_Photon33);
            chain->SetBranchAddress("HLT_Photon50"                                                      , & HLT_Photon50                                                         , & b_HLT_Photon50);
            chain->SetBranchAddress("HLT_Photon75"                                                      , & HLT_Photon75                                                         , & b_HLT_Photon75);
            chain->SetBranchAddress("HLT_Photon90"                                                      , & HLT_Photon90                                                         , & b_HLT_Photon90);
            chain->SetBranchAddress("HLT_Photon120"                                                     , & HLT_Photon120                                                        , & b_HLT_Photon120);
            chain->SetBranchAddress("HLT_Photon150"                                                     , & HLT_Photon150                                                        , & b_HLT_Photon150);
            chain->SetBranchAddress("HLT_Photon175"                                                     , & HLT_Photon175                                                        , & b_HLT_Photon175);
            chain->SetBranchAddress("HLT_Photon200"                                                     , & HLT_Photon200                                                        , & b_HLT_Photon200);
            chain->SetBranchAddress("HLT_Photon30EB_TightID_TightIso"                                   , & HLT_Photon30EB_TightID_TightIso                                      , & b_HLT_Photon30EB_TightID_TightIso);
            chain->SetBranchAddress("HLT_Photon100EB_TightID_TightIso"                                  , & HLT_Photon100EB_TightID_TightIso                                     , & b_HLT_Photon100EB_TightID_TightIso);
            chain->SetBranchAddress("HLT_Photon110EB_TightID_TightIso"                                  , & HLT_Photon110EB_TightID_TightIso                                     , & b_HLT_Photon110EB_TightID_TightIso);
            chain->SetBranchAddress("HLT_Photon120EB_TightID_TightIso"                                  , & HLT_Photon120EB_TightID_TightIso                                     , & b_HLT_Photon120EB_TightID_TightIso);
            chain->SetBranchAddress("HLT_Photon100EBHE10"                                               , & HLT_Photon100EBHE10                                                  , & b_HLT_Photon100EBHE10);
            chain->SetBranchAddress("HLT_Photon100EEHE10"                                               , & HLT_Photon100EEHE10                                                  , & b_HLT_Photon100EEHE10);
            chain->SetBranchAddress("HLT_Photon100EE_TightID_TightIso"                                  , & HLT_Photon100EE_TightID_TightIso                                     , & b_HLT_Photon100EE_TightID_TightIso);
            chain->SetBranchAddress("HLT_Photon50_R9Id90_HE10_IsoM"                                     , & HLT_Photon50_R9Id90_HE10_IsoM                                        , & b_HLT_Photon50_R9Id90_HE10_IsoM);
            chain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM"                                     , & HLT_Photon75_R9Id90_HE10_IsoM                                        , & b_HLT_Photon75_R9Id90_HE10_IsoM);
            chain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3" , & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3    , & b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3);
            chain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3" , & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3    , & b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3);
            chain->SetBranchAddress("HLT_Photon90_R9Id90_HE10_IsoM"                                     , & HLT_Photon90_R9Id90_HE10_IsoM                                        , & b_HLT_Photon90_R9Id90_HE10_IsoM);
            chain->SetBranchAddress("HLT_Photon120_R9Id90_HE10_IsoM"                                    , & HLT_Photon120_R9Id90_HE10_IsoM                                       , & b_HLT_Photon120_R9Id90_HE10_IsoM);
            chain->SetBranchAddress("HLT_Photon165_R9Id90_HE10_IsoM"                                    , & HLT_Photon165_R9Id90_HE10_IsoM                                       , & b_HLT_Photon165_R9Id90_HE10_IsoM);
            chain->SetBranchAddress("HLT_Photon35_TwoProngs35"                                          , & HLT_Photon35_TwoProngs35                                             , & b_HLT_Photon35_TwoProngs35);
            chain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL"                                  , & HLT_Photon60_R9Id90_CaloIdL_IsoL                                     , & b_HLT_Photon60_R9Id90_CaloIdL_IsoL);
            chain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL"                     , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL                        , & b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL);
            chain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15"   , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15      , & b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15);
            chain->SetBranchAddress("HLT_Photon20_HoverELoose"                                          , & HLT_Photon20_HoverELoose                                             , & b_HLT_Photon20_HoverELoose);
            chain->SetBranchAddress("HLT_Photon30_HoverELoose"                                          , & HLT_Photon30_HoverELoose                                             , & b_HLT_Photon30_HoverELoose);
            chain->SetBranchAddress("HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50"    , & HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50       , & b_HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50);
            chain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3"            , & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3               , & b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3);
            chain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3"            , & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3               , & b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3);
        }
        //2023
        if(oName.Contains("2023")){ 
            chain->SetBranchAddress("HLT_Photon300_NoHE"                                    , & HLT_Photon300_NoHE                                    , & b_HLT_Photon300_NoHE                                    );
            chain->SetBranchAddress("HLT_Photon33"                                          , & HLT_Photon33                                          , & b_HLT_Photon33                                          );
            chain->SetBranchAddress("HLT_Photon50"                                          , & HLT_Photon50                                          , & b_HLT_Photon50                                          );
            chain->SetBranchAddress("HLT_Photon75"                                          , & HLT_Photon75                                          , & b_HLT_Photon75                                          );
            chain->SetBranchAddress("HLT_Photon90"                                          , & HLT_Photon90                                          , & b_HLT_Photon90                                          );
            chain->SetBranchAddress("HLT_Photon120"                                         , & HLT_Photon120                                         , & b_HLT_Photon120                                         );
            chain->SetBranchAddress("HLT_Photon150"                                         , & HLT_Photon150                                         , & b_HLT_Photon150                                         );
            chain->SetBranchAddress("HLT_Photon175"                                         , & HLT_Photon175                                         , & b_HLT_Photon175                                         );
            chain->SetBranchAddress("HLT_Photon200"                                         , & HLT_Photon200                                         , & b_HLT_Photon200                                         );
            chain->SetBranchAddress("HLT_Photon30EB_TightID_TightIso"                       , & HLT_Photon30EB_TightID_TightIso                       , & b_HLT_Photon30EB_TightID_TightIso                       );
            chain->SetBranchAddress("HLT_Photon50EB_TightID_TightIso"                       , & HLT_Photon50EB_TightID_TightIso                       , & b_HLT_Photon50EB_TightID_TightIso                       );
            chain->SetBranchAddress("HLT_Photon75EB_TightID_TightIso"                       , & HLT_Photon75EB_TightID_TightIso                       , & b_HLT_Photon75EB_TightID_TightIso                       );
            chain->SetBranchAddress("HLT_Photon90EB_TightID_TightIso"                       , & HLT_Photon90EB_TightID_TightIso                       , & b_HLT_Photon90EB_TightID_TightIso                       );
            chain->SetBranchAddress("HLT_Photon110EB_TightID_TightIso"                      , & HLT_Photon110EB_TightID_TightIso                      , & b_HLT_Photon110EB_TightID_TightIso                      );
            chain->SetBranchAddress("HLT_Photon130EB_TightID_TightIso"                      , & HLT_Photon130EB_TightID_TightIso                      , & b_HLT_Photon130EB_TightID_TightIso                      );
            chain->SetBranchAddress("HLT_Photon150EB_TightID_TightIso"                      , & HLT_Photon150EB_TightID_TightIso                      , & b_HLT_Photon150EB_TightID_TightIso                      );
            chain->SetBranchAddress("HLT_Photon175EB_TightID_TightIso"                      , & HLT_Photon175EB_TightID_TightIso                      , & b_HLT_Photon175EB_TightID_TightIso                      );
            chain->SetBranchAddress("HLT_Photon200EB_TightID_TightIso"                      , & HLT_Photon200EB_TightID_TightIso                      , & b_HLT_Photon200EB_TightID_TightIso                      );
            chain->SetBranchAddress("HLT_Photon100EBHE10"                                   , & HLT_Photon100EBHE10                                   , & b_HLT_Photon100EBHE10                                   );
            chain->SetBranchAddress("HLT_Photon50_R9Id90_HE10_IsoM"                         , & HLT_Photon50_R9Id90_HE10_IsoM                         , & b_HLT_Photon50_R9Id90_HE10_IsoM                         );
            chain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM"                         , & HLT_Photon75_R9Id90_HE10_IsoM                         , & b_HLT_Photon75_R9Id90_HE10_IsoM                         );
            chain->SetBranchAddress("HLT_Photon90_R9Id90_HE10_IsoM"                         , & HLT_Photon90_R9Id90_HE10_IsoM                         , & b_HLT_Photon90_R9Id90_HE10_IsoM                         );
            chain->SetBranchAddress("HLT_Photon120_R9Id90_HE10_IsoM"                        , & HLT_Photon120_R9Id90_HE10_IsoM                        , & b_HLT_Photon120_R9Id90_HE10_IsoM                        );
            chain->SetBranchAddress("HLT_Photon165_R9Id90_HE10_IsoM"                        , & HLT_Photon165_R9Id90_HE10_IsoM                        , & b_HLT_Photon165_R9Id90_HE10_IsoM                        );
            chain->SetBranchAddress("HLT_Photon35_TwoProngs35"                              , & HLT_Photon35_TwoProngs35                              , & b_HLT_Photon35_TwoProngs35                              );
            chain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350" , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350 , & b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350 );
            chain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380" , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380 , & b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380 );
            chain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400" , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400 , & b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400 );
            chain->SetBranchAddress("HLT_Photon20_HoverELoose"                              , & HLT_Photon20_HoverELoose                              , & b_HLT_Photon20_HoverELoose                              );
            chain->SetBranchAddress("HLT_Photon30_HoverELoose"                              , & HLT_Photon30_HoverELoose                              , & b_HLT_Photon30_HoverELoose                              );
            chain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3", & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3, & b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3);
            chain->SetBranchAddress("HLT_Photon32_OneProng32_M50To105"                      , & HLT_Photon32_OneProng32_M50To105                      , & b_HLT_Photon32_OneProng32_M50To105                      );
        }
    }//GamJet

    if(oName.Contains("DiEleJet")){
		chain->SetBranchStatus("nElectron", 1);
		chain->SetBranchStatus("Electron_charge", 1);
		chain->SetBranchStatus("Electron_pt", 1);
		chain->SetBranchStatus("Electron_deltaEtaSC", 1);
		chain->SetBranchStatus("Electron_eta", 1);
		chain->SetBranchStatus("Electron_phi", 1);
		chain->SetBranchStatus("Electron_mass", 1);
		chain->SetBranchStatus("Electron_cutBased", 1);
        chain->SetBranchStatus("nElectron",1);
        chain->SetBranchStatus("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ*", 1);
    	if (oName.Contains("MC")){
            chain->SetBranchStatus("GenDressedLepton_*",1);
            chain->SetBranchStatus("nGenDressedLepton",1);
		}
        chain->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ, & b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);
    }
    if(oName.Contains("DiMuJet")){
		chain->SetBranchStatus("Muon_charge", 1);
		chain->SetBranchStatus("Muon_pt", 1);
		chain->SetBranchStatus("Muon_eta", 1);
		chain->SetBranchStatus("Muon_phi", 1);
		chain->SetBranchStatus("Muon_mass", 1);
		chain->SetBranchStatus("Muon_mediumId", 1);
		chain->SetBranchStatus("Muon_tightId", 1);
		chain->SetBranchStatus("Muon_highPurity", 1);
		chain->SetBranchStatus("Muon_pfRelIso04_all", 1);
		chain->SetBranchStatus("Muon_tkRelIso", 1); 
		chain->SetBranchStatus("Muon_dxy", 1); 
		chain->SetBranchStatus("Muon_dz", 1); 
        chain->SetBranchStatus("nMuon",1);
    	if (oName.Contains("MC")){
            chain->SetBranchStatus("GenDressedLepton_*",1);
            chain->SetBranchStatus("nGenDressedLepton",1);
		}
        chain->SetBranchStatus("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass*", 1);
        chain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8", & HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8);
    }
    if(oName.Contains("DiJet")){
        chain->SetBranchStatus("HLT_PFJet*", 1);
        chain->SetBranchStatus("HLT_DiPFJetAve*", 1);
        chain->SetBranchAddress("HLT_PFJet40"            , & HLT_PFJet40            , & b_HLT_PFJet40            );
        chain->SetBranchAddress("HLT_PFJet60"            , & HLT_PFJet60            , & b_HLT_PFJet60            );
        chain->SetBranchAddress("HLT_PFJet80"            , & HLT_PFJet80            , & b_HLT_PFJet80            );
        chain->SetBranchAddress("HLT_PFJet140"           , & HLT_PFJet140           , & b_HLT_PFJet140           );
        chain->SetBranchAddress("HLT_PFJet200"           , & HLT_PFJet200           , & b_HLT_PFJet200           );
        chain->SetBranchAddress("HLT_PFJet260"           , & HLT_PFJet260           , & b_HLT_PFJet260           );
        chain->SetBranchAddress("HLT_PFJet320"           , & HLT_PFJet320           , & b_HLT_PFJet320           );
        chain->SetBranchAddress("HLT_PFJet400"           , & HLT_PFJet400           , & b_HLT_PFJet400           );
        chain->SetBranchAddress("HLT_PFJet450"           , & HLT_PFJet450           , & b_HLT_PFJet450           );
        chain->SetBranchAddress("HLT_PFJet500"           , & HLT_PFJet500           , & b_HLT_PFJet500           );
        chain->SetBranchAddress("HLT_DiPFJetAve40"       , & HLT_DiPFJetAve40       , & b_HLT_DiPFJetAve40       );
        chain->SetBranchAddress("HLT_DiPFJetAve60"       , & HLT_DiPFJetAve60       , & b_HLT_DiPFJetAve60       );
        chain->SetBranchAddress("HLT_DiPFJetAve80"       , & HLT_DiPFJetAve80       , & b_HLT_DiPFJetAve80       );
        chain->SetBranchAddress("HLT_DiPFJetAve140"      , & HLT_DiPFJetAve140      , & b_HLT_DiPFJetAve140      );
        chain->SetBranchAddress("HLT_DiPFJetAve200"      , & HLT_DiPFJetAve200      , & b_HLT_DiPFJetAve200      );
        chain->SetBranchAddress("HLT_DiPFJetAve260"      , & HLT_DiPFJetAve260      , & b_HLT_DiPFJetAve260      );
        chain->SetBranchAddress("HLT_DiPFJetAve320"      , & HLT_DiPFJetAve320      , & b_HLT_DiPFJetAve320      );
        chain->SetBranchAddress("HLT_DiPFJetAve400"      , & HLT_DiPFJetAve400      , & b_HLT_DiPFJetAve400      );
        chain->SetBranchAddress("HLT_DiPFJetAve500"      , & HLT_DiPFJetAve500      , & b_HLT_DiPFJetAve500      );
        chain->SetBranchAddress("HLT_DiPFJetAve60_HFJEC" , & HLT_DiPFJetAve60_HFJEC , & b_HLT_DiPFJetAve60_HFJEC );
        chain->SetBranchAddress("HLT_DiPFJetAve80_HFJEC" , & HLT_DiPFJetAve80_HFJEC , & b_HLT_DiPFJetAve80_HFJEC );
        chain->SetBranchAddress("HLT_DiPFJetAve100_HFJEC", & HLT_DiPFJetAve100_HFJEC, & b_HLT_DiPFJetAve100_HFJEC);
        chain->SetBranchAddress("HLT_DiPFJetAve160_HFJEC", & HLT_DiPFJetAve160_HFJEC, & b_HLT_DiPFJetAve160_HFJEC);
        chain->SetBranchAddress("HLT_DiPFJetAve220_HFJEC", & HLT_DiPFJetAve220_HFJEC, & b_HLT_DiPFJetAve220_HFJEC);
        chain->SetBranchAddress("HLT_DiPFJetAve300_HFJEC", & HLT_DiPFJetAve300_HFJEC, & b_HLT_DiPFJetAve300_HFJEC);
    }

}

NanoTree::~NanoTree(){
    delete chain;
}

Long64_t NanoTree::GetEntries(){
    return chain->GetEntries();
}

Int_t NanoTree::GetEntry(Long64_t entry){
    return chain->GetEntry(entry);
}

std::vector<std::vector<std::string>> NanoTree::splitVector(const std::vector<std::string>& strings, int n) {
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

std::vector<std::string> NanoTree::splitString(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    
    while ((end = s.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(s.substr(start)); // Last token
    
    return tokens;
}
