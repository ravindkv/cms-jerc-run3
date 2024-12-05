#include "NanoTree.h"

void NanoTree::setInput(string oName){
    iName  = oName;
    cout<<"+ setInput() = "<<iName<<endl;
} 
void NanoTree::loadInput(){
    cout<<"==> loadInput()"<<endl;
    try{
        std::vector<std::string> v_iName      = splitString(iName, "_Skim_");
        loadedSampKey = v_iName.at(0); 
        std::cout << "loadedSampKey: " << loadedSampKey << std::endl;
        std::string nofN_root   = v_iName.at(1); 
        std::vector<std::string> v_nofN_root    = splitString(nofN_root, ".root"); 
        std::string nofN        = v_nofN_root.at(0); 
        std::cout << "nofN: " << nofN << std::endl;
        std::vector<std::string> v_nofN         = splitString(nofN, "of"); 
        loadedNthJob = std::stoi(v_nofN.at(0));
        loadedTotJob = std::stoi(v_nofN.at(1));
    }catch(const std::exception &e){
        cout<<"\nEXCEPTION: Check the iName_: "<<iName<<endl;
        cout<<"iName format should be: DataOrMC_Year_Channel_Sample_Skim_nofN.rooot"<<endl;
        cout<<"Run ./runMain -h for more details"<<endl;
        cout<<e.what()<<endl;
        std::abort();
    }
} 
void NanoTree::setInputJsonPath(string inDir){
    string year = "2022";
    TString oN = iName;
    if(oN.Contains("2023")) year = "2023";
    if(oN.Contains("2024")) year = "2024";
    string channel = splitString(loadedSampKey, "_").at(2);
    inputJsonPath = inDir+"/FilesNano_"+year+"_"+channel+".json";
    cout<<"+ setInputJsonPath() = "<<inputJsonPath<<endl;
}

void NanoTree:: loadInputJson(){
    cout<<"==> loadInputJson()"<<endl;
    ifstream fileName_(inputJsonPath.c_str());
    nlohmann::json js;
    try{
        js = nlohmann::json::parse(fileName_);
    } catch (const exception& e) {
        cout<<"\nEXCEPTION: Check the input json inputJsonPath: "<<inputJsonPath<<endl;
        cout<<e.what()<<endl;
        abort();
    }
    try{
        js.at(loadedSampKey).get_to(loadedAllFileNames);
    }catch (const exception & e){
        cout<<"\nEXCEPTION: Check the loadedSampKey: "<<loadedSampKey<<endl;
        cout<<e.what()<<endl;
        cout<<"Choose loadedSampKey from the following:"<<endl;
        for (auto& element : js.items()) {
            cout << element.key() << endl;
        }
        abort();
    }//
}

void NanoTree::loadJobFileNames(){
    cout<<"==> loadJobFileNames()"<<endl;
    int nFiles  = loadedAllFileNames.size();
    cout<<"Total files = "<<nFiles<<endl;
    if (loadedTotJob > nFiles){
        cout<<"Since loadedTotJob > nFiles, setting it to the nFiles, loadedTotJob = "<<nFiles<<endl;
        loadedTotJob = nFiles;
    }
    if (loadedNthJob > loadedTotJob){
        cout<<"Error: Make sure loadedNthJob < loadedTotJob"<<endl;
        std::abort();
    }
	if (loadedNthJob>0 && loadedTotJob>0){
	    cout <<"jobs: " <<loadedNthJob << " of " << loadedTotJob << endl;
		cout << (1.*nFiles)/loadedTotJob << " files per job on average" << endl;
	}
    else{
        cout<<"\n Error: Make sure loadedNthJob > 0 and loadedTotJob > 0\n ";
        std::abort();
    }
    std::vector<std::vector<std::string>> smallVectors = splitVector(loadedAllFileNames, loadedTotJob);
    loadedJobFileNames = smallVectors[loadedNthJob-1];
    //loadedJobFileNames = {"/store/data/Run2024H/EGamma0/NANOAOD/PromptReco-v1/000/385/836/00000/ff057d36-5d80-40e5-a785-c4a729416c76.root"};
    //loadedJobFileNames = {"/store/data/Run2024I/ZeroBias/NANOAOD/PromptReco-v2/000/386/034dbe81-5740-4de8-a57f-766537a8aaed.root"};
    //loadedJobFileNames = {"/store/data/Run2024I/JetMET0/NANOAOD/PromptReco-v1/000/386/604/00000/e48a0a0c-d9bf-414d-8d77-ce05f83466ee.root"};
    //loadedJobFileNames = {"/store/mc/Run3Winter24NanoAOD/GJ-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/NANOAODSIM/JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/2520000/9f11bda3-0a00-419e-826a-abcc4cc65235.root"};
}


void NanoTree::loadTree() {
    std::cout << "==> loadTree()" << '\n';
	TString oN = iName;
    fChain->SetCacheSize(50*1024*1024);
    bool isCopy = false;  // Set to true if you want to copy files locally
    std::string dir = "root://cms-xrd-global.cern.ch/";  // Default remote directory
    // std::string dir = "./";  // Uncomment if using local directory

    int totalFiles = 0;
    int addedFiles = 0;
    int failedFiles = 0;
    for (const auto& fileName : loadedJobFileNames) {
        totalFiles++;
        std::string fullPath;

        if (isCopy) {
            // Extract the local file name from the remote path
            std::string localFile = fileName.substr(fileName.find_last_of('/') + 1);
            std::string cmd = "xrdcp ";
            cmd.append(dir).append(fileName).append(" ").append(localFile);
            std::cout << "Executing command: " << cmd << '\n';
            int ret = system(cmd.c_str());

            if (ret != 0) {
                std::cerr << "Error: Failed to copy " << fileName << " to local file " << localFile << '\n';
                failedFiles++;
                continue;  // Skip adding this file
            }

            // Check if the file was successfully copied
            if (!std::filesystem::exists(localFile)) {
                std::cerr << "Error: Local file " << localFile << " does not exist after copying.\n";
                failedFiles++;
                continue;  // Skip adding this file
            }

            fullPath = localFile;  // Use the local file path
        } else {
            // Remote file handling
            std::filesystem::path filePath = "/eos/cms/" + fileName;
            if (std::filesystem::exists(filePath)) {
                dir = "/eos/cms/";  // Use local EOS path
                fullPath = dir + fileName;
            } else {
                dir = "root://cms-xrd-global.cern.ch/";  // Fallback to remote xrootd path
                fullPath = dir + fileName;
            }
        }

        // Attempt to open the file to verify its validity
        TFile* f = TFile::Open(fullPath.c_str(), "READ");
        if (!f || f->IsZombie()) {
            std::cerr << "Error: Failed to open or corrupted file " << fullPath << '\n';
            if (f) f->Close();
            failedFiles++;
            continue;  // Skip adding this file
        }

        // Optionally, check for the presence of essential branches or histograms
        // For example, check if "Events" exists
        if (!f->GetListOfKeys()->Contains("Events")) {
            std::cerr << "Error: 'Events' not found in " << fullPath << '\n';
            f->Close();
            failedFiles++;
            continue;  // Skip adding this file
        }

        // Check the entries in the newly added TTree
        Long64_t fileEntries = f->Get<TTree>("Events")->GetEntries();
        if (fileEntries == 0) {
            std::cerr << "\nWarning: 'Events' TTree in file " << fullPath << " has 0 entries. Skipping file.\n\n";
            f->Close();
            failedFiles++;
            continue;  // Skip adding this file to the final count
        }

        // File is valid, add it to the TChain
        int added = fChain->Add(fullPath.c_str());
        if (added == 0) {
            std::cerr << "Warning: TChain::Add failed for " << fullPath << '\n';
            f->Close();
            failedFiles++;
            continue;  // Skip adding this file
        }

        std::cout << fullPath << "  Entries: " << fChain->GetEntries() << '\n';
        addedFiles++;
        f->Close();
    }

    // Final summary
    std::cout << "==> Finished loading files.\n";
    std::cout << "Total files processed: " << totalFiles << '\n';
    std::cout << "Successfully added files: " << addedFiles << '\n';
    std::cout << "Failed to add files: " << failedFiles << '\n';

    // Check if the chain has any trees
    if (fChain->GetNtrees() == 0) {
        std::cerr << "Error: No valid ROOT files were added to the TChain. Exiting.\n";
        return;
    }

    std::cout << "Begin" << std::endl;
    fChain->SetBranchStatus("*",0);
    // event
    fChain->SetBranchStatus("run",1);
    fChain->SetBranchStatus("event",1);
    fChain->SetBranchStatus("luminosityBlock",1);
    fChain->SetBranchStatus("bunchCrossing",1);
    
    //--------------------------------------- 
    //Jet for all channels 
    //--------------------------------------- 
    fChain->SetBranchStatus("Jet_btagDeep*",1);
    fChain->SetBranchStatus("Jet_btagPNetQvG",1);
    fChain->SetBranchStatus("Jet_btagUParTAK4QvG",1);
    fChain->SetBranchStatus("Jet_chEmEF",1);
    fChain->SetBranchStatus("Jet_chHEF" ,1);
    fChain->SetBranchStatus("Jet_eta"   ,1);
    fChain->SetBranchStatus("Jet_mass"  ,1);
    fChain->SetBranchStatus("Jet_muEF"  ,1);
    fChain->SetBranchStatus("Jet_neEmEF",1);
    fChain->SetBranchStatus("Jet_hfEmEF",1);
    fChain->SetBranchStatus("Jet_hfHEF",1);
    fChain->SetBranchStatus("Jet_neHEF" ,1);
    fChain->SetBranchStatus("Jet_phi"   ,1);
    fChain->SetBranchStatus("Jet_pt"    ,1);
    fChain->SetBranchStatus("Jet_rawFactor",1);
    fChain->SetBranchStatus("Jet_jetId",1);
    fChain->SetBranchStatus("Jet_area",1);
    if(oN.Contains("2023")){ 
        fChain->SetBranchStatus("Jet_qgl",1);
    }
    fChain->SetBranchStatus("nJet",1);

    //common branches
	  fChain->SetBranchStatus("PV_z");
	  fChain->SetBranchStatus("GenVtx_z");
    fChain->SetBranchStatus("PV_npvs",1);
    fChain->SetBranchStatus("PV_npvsGood",1);
    if(oN.Contains("Data_2024")){
      fChain->SetBranchStatus("PFMET_pt",1);
      fChain->SetBranchStatus("PFMET_phi",1);
      fChain->SetBranchStatus("PuppiMET_pt",1);
      fChain->SetBranchStatus("PuppiMET_phi",1);
      fChain->SetBranchStatus("RawPuppiMET_phi",1);
      fChain->SetBranchStatus("RawPuppiMET_pt",1);
      fChain->SetBranchStatus("RawPuppiMET_sumEt",1);
    }
    else{
      fChain->SetBranchStatus("MET_pt",1);
      fChain->SetBranchStatus("MET_phi",1);
      fChain->SetBranchStatus("ChsMET_pt",1);
      fChain->SetBranchStatus("ChsMET_phi",1);
      fChain->SetBranchStatus("RawPuppiMET_phi",1);
      fChain->SetBranchStatus("RawPuppiMET_pt",1);
      fChain->SetBranchStatus("RawPuppiMET_sumEt",1);
    }

    fChain->SetBranchStatus("Flag_*",1);
	fChain->SetBranchStatus("Rho_fixed*");
 
    if (oN.Contains("MC")){
        fChain->SetBranchStatus("Jet_genJetIdx",1);
		fChain->SetBranchStatus("genWeight");
		fChain->SetBranchStatus("nPSWeight");
		fChain->SetBranchStatus("PSWeight");
		fChain->SetBranchStatus("LHE_HT");
		fChain->SetBranchStatus("Pileup_nTrueInt");
        fChain->SetBranchStatus("GenJet_*",1);
        fChain->SetBranchStatus("nGenJet",1);
		/*
	    fChain->SetBranchStatus("Pileup_*",1);
        fChain->SetBranchStatus("GenPart_*",1);
        fChain->SetBranchStatus("GenJetAK8_*",1);
        fChain->SetBranchStatus("nGenPart",1);
        fChain->SetBranchStatus("nGenJetAK8",1);
        fChain->SetBranchStatus("LHE_*",1);
        // weight
        fChain->SetBranchStatus("Generator_weight",1);
        fChain->SetBranchStatus("nLHEScaleWeight",1);
        fChain->SetBranchStatus("LHEScaleWeight",1);
        fChain->SetBranchStatus("nLHEPdfWeight",1);
        fChain->SetBranchStatus("nLHEPart",1);
        fChain->SetBranchStatus("LHEPart_*",1);
        fChain->SetBranchStatus("LHEPdfWeight",1);
        fChain->SetBranchStatus("PSWeight",1);
        fChain->SetBranchStatus("nPSWeight",1);
        fChain->SetBranchStatus("genWeight",1);
		*/
    }//MC

    //--------------------------------------- 
    // Photon (for GamJet)
    //--------------------------------------- 
    if(oN.Contains("GamJet")){
   		fChain->SetBranchStatus("Photon_energyErr",1);
   		fChain->SetBranchStatus("Photon_eta",1);
   		fChain->SetBranchStatus("Photon_hoe",1);
   		fChain->SetBranchStatus("Photon_phi",1);
   		fChain->SetBranchStatus("Photon_pt",1);
   		fChain->SetBranchStatus("Photon_r9",1);
   		fChain->SetBranchStatus("Photon_cutBased",1);
   		fChain->SetBranchStatus("Photon_jetIdx",1);
   		fChain->SetBranchStatus("Photon_seedGain",1);
        fChain->SetBranchStatus("nPhoton",1);
        if(oN.Contains("2023")){ 
   		    fChain->SetBranchStatus("Photon_eCorr",1); 
   		    fChain->SetBranchStatus("Photon_mass",1);
        }
        fChain->SetBranchStatus("HLT_Photon*", 1);
    	if (oN.Contains("MC")){
            fChain->SetBranchStatus("GenIsolatedPhoton_*",1);
            fChain->SetBranchStatus("nGenIsolatedPhoton",1);
		}
        //address trigger
        if(oN.Contains("2022")){ 
            fChain->SetBranchAddress("HLT_Photon300_NoHE"                                                , & HLT_Photon300_NoHE                                                   , & b_HLT_Photon300_NoHE);
            fChain->SetBranchAddress("HLT_Photon20"                                                      , & HLT_Photon20                                                         , & b_HLT_Photon20);
            fChain->SetBranchAddress("HLT_Photon33"                                                      , & HLT_Photon33                                                         , & b_HLT_Photon33);
            fChain->SetBranchAddress("HLT_Photon50"                                                      , & HLT_Photon50                                                         , & b_HLT_Photon50);
            fChain->SetBranchAddress("HLT_Photon75"                                                      , & HLT_Photon75                                                         , & b_HLT_Photon75);
            fChain->SetBranchAddress("HLT_Photon90"                                                      , & HLT_Photon90                                                         , & b_HLT_Photon90);
            fChain->SetBranchAddress("HLT_Photon120"                                                     , & HLT_Photon120                                                        , & b_HLT_Photon120);
            fChain->SetBranchAddress("HLT_Photon150"                                                     , & HLT_Photon150                                                        , & b_HLT_Photon150);
            fChain->SetBranchAddress("HLT_Photon175"                                                     , & HLT_Photon175                                                        , & b_HLT_Photon175);
            fChain->SetBranchAddress("HLT_Photon200"                                                     , & HLT_Photon200                                                        , & b_HLT_Photon200);
            fChain->SetBranchAddress("HLT_Photon30EB_TightID_TightIso"                                   , & HLT_Photon30EB_TightID_TightIso                                      , & b_HLT_Photon30EB_TightID_TightIso);
            fChain->SetBranchAddress("HLT_Photon100EB_TightID_TightIso"                                  , & HLT_Photon100EB_TightID_TightIso                                     , & b_HLT_Photon100EB_TightID_TightIso);
            fChain->SetBranchAddress("HLT_Photon110EB_TightID_TightIso"                                  , & HLT_Photon110EB_TightID_TightIso                                     , & b_HLT_Photon110EB_TightID_TightIso);
            fChain->SetBranchAddress("HLT_Photon120EB_TightID_TightIso"                                  , & HLT_Photon120EB_TightID_TightIso                                     , & b_HLT_Photon120EB_TightID_TightIso);
            fChain->SetBranchAddress("HLT_Photon100EBHE10"                                               , & HLT_Photon100EBHE10                                                  , & b_HLT_Photon100EBHE10);
            fChain->SetBranchAddress("HLT_Photon100EEHE10"                                               , & HLT_Photon100EEHE10                                                  , & b_HLT_Photon100EEHE10);
            fChain->SetBranchAddress("HLT_Photon100EE_TightID_TightIso"                                  , & HLT_Photon100EE_TightID_TightIso                                     , & b_HLT_Photon100EE_TightID_TightIso);
            fChain->SetBranchAddress("HLT_Photon50_R9Id90_HE10_IsoM"                                     , & HLT_Photon50_R9Id90_HE10_IsoM                                        , & b_HLT_Photon50_R9Id90_HE10_IsoM);
            fChain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM"                                     , & HLT_Photon75_R9Id90_HE10_IsoM                                        , & b_HLT_Photon75_R9Id90_HE10_IsoM);
            fChain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3" , & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3    , & b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3);
            fChain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3" , & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3    , & b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3);
            fChain->SetBranchAddress("HLT_Photon90_R9Id90_HE10_IsoM"                                     , & HLT_Photon90_R9Id90_HE10_IsoM                                        , & b_HLT_Photon90_R9Id90_HE10_IsoM);
            fChain->SetBranchAddress("HLT_Photon120_R9Id90_HE10_IsoM"                                    , & HLT_Photon120_R9Id90_HE10_IsoM                                       , & b_HLT_Photon120_R9Id90_HE10_IsoM);
            fChain->SetBranchAddress("HLT_Photon165_R9Id90_HE10_IsoM"                                    , & HLT_Photon165_R9Id90_HE10_IsoM                                       , & b_HLT_Photon165_R9Id90_HE10_IsoM);
            fChain->SetBranchAddress("HLT_Photon35_TwoProngs35"                                          , & HLT_Photon35_TwoProngs35                                             , & b_HLT_Photon35_TwoProngs35);
            fChain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL"                                  , & HLT_Photon60_R9Id90_CaloIdL_IsoL                                     , & b_HLT_Photon60_R9Id90_CaloIdL_IsoL);
            fChain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL"                     , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL                        , & b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL);
            fChain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15"   , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15      , & b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15);
            fChain->SetBranchAddress("HLT_Photon20_HoverELoose"                                          , & HLT_Photon20_HoverELoose                                             , & b_HLT_Photon20_HoverELoose);
            fChain->SetBranchAddress("HLT_Photon30_HoverELoose"                                          , & HLT_Photon30_HoverELoose                                             , & b_HLT_Photon30_HoverELoose);
            fChain->SetBranchAddress("HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50"    , & HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50       , & b_HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50);
            fChain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3"            , & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3               , & b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3);
            fChain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3"            , & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3               , & b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3);
        }
        //2023
        if(oN.Contains("2023") || oN.Contains("2024")){ 
            fChain->SetBranchAddress("HLT_Photon300_NoHE"                                    , & HLT_Photon300_NoHE                                    , & b_HLT_Photon300_NoHE                                    );
            fChain->SetBranchAddress("HLT_Photon33"                                          , & HLT_Photon33                                          , & b_HLT_Photon33                                          );
            fChain->SetBranchAddress("HLT_Photon50"                                          , & HLT_Photon50                                          , & b_HLT_Photon50                                          );
            fChain->SetBranchAddress("HLT_Photon75"                                          , & HLT_Photon75                                          , & b_HLT_Photon75                                          );
            fChain->SetBranchAddress("HLT_Photon90"                                          , & HLT_Photon90                                          , & b_HLT_Photon90                                          );
            fChain->SetBranchAddress("HLT_Photon120"                                         , & HLT_Photon120                                         , & b_HLT_Photon120                                         );
            fChain->SetBranchAddress("HLT_Photon150"                                         , & HLT_Photon150                                         , & b_HLT_Photon150                                         );
            fChain->SetBranchAddress("HLT_Photon175"                                         , & HLT_Photon175                                         , & b_HLT_Photon175                                         );
            fChain->SetBranchAddress("HLT_Photon200"                                         , & HLT_Photon200                                         , & b_HLT_Photon200                                         );
            fChain->SetBranchAddress("HLT_Photon30EB_TightID_TightIso"                       , & HLT_Photon30EB_TightID_TightIso                       , & b_HLT_Photon30EB_TightID_TightIso                       );
            fChain->SetBranchAddress("HLT_Photon50EB_TightID_TightIso"                       , & HLT_Photon50EB_TightID_TightIso                       , & b_HLT_Photon50EB_TightID_TightIso                       );
            fChain->SetBranchAddress("HLT_Photon75EB_TightID_TightIso"                       , & HLT_Photon75EB_TightID_TightIso                       , & b_HLT_Photon75EB_TightID_TightIso                       );
            fChain->SetBranchAddress("HLT_Photon90EB_TightID_TightIso"                       , & HLT_Photon90EB_TightID_TightIso                       , & b_HLT_Photon90EB_TightID_TightIso                       );
            fChain->SetBranchAddress("HLT_Photon110EB_TightID_TightIso"                      , & HLT_Photon110EB_TightID_TightIso                      , & b_HLT_Photon110EB_TightID_TightIso                      );
            fChain->SetBranchAddress("HLT_Photon130EB_TightID_TightIso"                      , & HLT_Photon130EB_TightID_TightIso                      , & b_HLT_Photon130EB_TightID_TightIso                      );
            fChain->SetBranchAddress("HLT_Photon150EB_TightID_TightIso"                      , & HLT_Photon150EB_TightID_TightIso                      , & b_HLT_Photon150EB_TightID_TightIso                      );
            fChain->SetBranchAddress("HLT_Photon175EB_TightID_TightIso"                      , & HLT_Photon175EB_TightID_TightIso                      , & b_HLT_Photon175EB_TightID_TightIso                      );
            fChain->SetBranchAddress("HLT_Photon200EB_TightID_TightIso"                      , & HLT_Photon200EB_TightID_TightIso                      , & b_HLT_Photon200EB_TightID_TightIso                      );
            fChain->SetBranchAddress("HLT_Photon100EBHE10"                                   , & HLT_Photon100EBHE10                                   , & b_HLT_Photon100EBHE10                                   );
            fChain->SetBranchAddress("HLT_Photon50_R9Id90_HE10_IsoM"                         , & HLT_Photon50_R9Id90_HE10_IsoM                         , & b_HLT_Photon50_R9Id90_HE10_IsoM                         );
            fChain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM"                         , & HLT_Photon75_R9Id90_HE10_IsoM                         , & b_HLT_Photon75_R9Id90_HE10_IsoM                         );
            fChain->SetBranchAddress("HLT_Photon90_R9Id90_HE10_IsoM"                         , & HLT_Photon90_R9Id90_HE10_IsoM                         , & b_HLT_Photon90_R9Id90_HE10_IsoM                         );
            fChain->SetBranchAddress("HLT_Photon120_R9Id90_HE10_IsoM"                        , & HLT_Photon120_R9Id90_HE10_IsoM                        , & b_HLT_Photon120_R9Id90_HE10_IsoM                        );
            fChain->SetBranchAddress("HLT_Photon165_R9Id90_HE10_IsoM"                        , & HLT_Photon165_R9Id90_HE10_IsoM                        , & b_HLT_Photon165_R9Id90_HE10_IsoM                        );
            fChain->SetBranchAddress("HLT_Photon35_TwoProngs35"                              , & HLT_Photon35_TwoProngs35                              , & b_HLT_Photon35_TwoProngs35                              );
            fChain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350" , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350 , & b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350 );
            fChain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380" , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380 , & b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380 );
            fChain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400" , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400 , & b_HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400 );
            fChain->SetBranchAddress("HLT_Photon20_HoverELoose"                              , & HLT_Photon20_HoverELoose                              , & b_HLT_Photon20_HoverELoose                              );
            fChain->SetBranchAddress("HLT_Photon30_HoverELoose"                              , & HLT_Photon30_HoverELoose                              , & b_HLT_Photon30_HoverELoose                              );
            fChain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3", & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3, & b_HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3);
            fChain->SetBranchAddress("HLT_Photon32_OneProng32_M50To105"                      , & HLT_Photon32_OneProng32_M50To105                      , & b_HLT_Photon32_OneProng32_M50To105                      );
        }
    }//GamJet

    if(oN.Contains("DiEleJet")){
		fChain->SetBranchStatus("nElectron", 1);
		fChain->SetBranchStatus("Electron_charge", 1);
		fChain->SetBranchStatus("Electron_pt", 1);
		fChain->SetBranchStatus("Electron_deltaEtaSC", 1);
		fChain->SetBranchStatus("Electron_eta", 1);
		fChain->SetBranchStatus("Electron_phi", 1);
		fChain->SetBranchStatus("Electron_mass", 1);
		fChain->SetBranchStatus("Electron_cutBased", 1);
		fChain->SetBranchStatus("Electron_seedGain", 1);
        fChain->SetBranchStatus("nElectron",1);
        fChain->SetBranchStatus("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ*", 1);
    	if (oN.Contains("MC")){
            fChain->SetBranchStatus("GenDressedLepton_*",1);
            fChain->SetBranchStatus("nGenDressedLepton",1);
		}
        fChain->SetBranchAddress("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ", &HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ, & b_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ);
    }
    if(oN.Contains("DiMuJet")){
		fChain->SetBranchStatus("Muon_charge", 1);
		fChain->SetBranchStatus("Muon_pt", 1);
		fChain->SetBranchStatus("Muon_eta", 1);
		fChain->SetBranchStatus("Muon_phi", 1);
		fChain->SetBranchStatus("Muon_mass", 1);
		fChain->SetBranchStatus("Muon_mediumId", 1);
		fChain->SetBranchStatus("Muon_tightId", 1);
		fChain->SetBranchStatus("Muon_highPurity", 1);
		fChain->SetBranchStatus("Muon_pfRelIso04_all", 1);
		fChain->SetBranchStatus("Muon_tkRelIso", 1); 
		fChain->SetBranchStatus("Muon_dxy", 1); 
		fChain->SetBranchStatus("Muon_dz", 1); 
        fChain->SetBranchStatus("nMuon",1);
    	if (oN.Contains("MC")){
            fChain->SetBranchStatus("GenDressedLepton_*",1);
            fChain->SetBranchStatus("nGenDressedLepton",1);
		}
        fChain->SetBranchStatus("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass*", 1);
        fChain->SetBranchAddress("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8", & HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8, &b_HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_Mass8);
    }
    if(oN.Contains("DiJet")){
        if(oN.Contains("ZeroBias")){
            fChain->SetBranchStatus("HLT_ZeroBias*", 1);
            fChain->SetBranchAddress("HLT_ZeroBias"            , & HLT_ZeroBias            , & b_HLT_ZeroBias            );
        }
        else{
            fChain->SetBranchStatus("HLT_PFJet*", 1);
            fChain->SetBranchStatus("HLT_DiPFJetAve*", 1);
            fChain->SetBranchAddress("HLT_PFJet40"            , & HLT_PFJet40            , & b_HLT_PFJet40            );
            fChain->SetBranchAddress("HLT_PFJet60"            , & HLT_PFJet60            , & b_HLT_PFJet60            );
            fChain->SetBranchAddress("HLT_PFJet80"            , & HLT_PFJet80            , & b_HLT_PFJet80            );
            fChain->SetBranchAddress("HLT_PFJet140"           , & HLT_PFJet140           , & b_HLT_PFJet140           );
            fChain->SetBranchAddress("HLT_PFJet200"           , & HLT_PFJet200           , & b_HLT_PFJet200           );
            fChain->SetBranchAddress("HLT_PFJet260"           , & HLT_PFJet260           , & b_HLT_PFJet260           );
            fChain->SetBranchAddress("HLT_PFJet320"           , & HLT_PFJet320           , & b_HLT_PFJet320           );
            fChain->SetBranchAddress("HLT_PFJet400"           , & HLT_PFJet400           , & b_HLT_PFJet400           );
            fChain->SetBranchAddress("HLT_PFJet450"           , & HLT_PFJet450           , & b_HLT_PFJet450           );
            fChain->SetBranchAddress("HLT_PFJet500"           , & HLT_PFJet500           , & b_HLT_PFJet500           );
            fChain->SetBranchAddress("HLT_DiPFJetAve40"       , & HLT_DiPFJetAve40       , & b_HLT_DiPFJetAve40       );
            fChain->SetBranchAddress("HLT_DiPFJetAve60"       , & HLT_DiPFJetAve60       , & b_HLT_DiPFJetAve60       );
            fChain->SetBranchAddress("HLT_DiPFJetAve80"       , & HLT_DiPFJetAve80       , & b_HLT_DiPFJetAve80       );
            fChain->SetBranchAddress("HLT_DiPFJetAve140"      , & HLT_DiPFJetAve140      , & b_HLT_DiPFJetAve140      );
            fChain->SetBranchAddress("HLT_DiPFJetAve200"      , & HLT_DiPFJetAve200      , & b_HLT_DiPFJetAve200      );
            fChain->SetBranchAddress("HLT_DiPFJetAve260"      , & HLT_DiPFJetAve260      , & b_HLT_DiPFJetAve260      );
            fChain->SetBranchAddress("HLT_DiPFJetAve320"      , & HLT_DiPFJetAve320      , & b_HLT_DiPFJetAve320      );
            fChain->SetBranchAddress("HLT_DiPFJetAve400"      , & HLT_DiPFJetAve400      , & b_HLT_DiPFJetAve400      );
            fChain->SetBranchAddress("HLT_DiPFJetAve500"      , & HLT_DiPFJetAve500      , & b_HLT_DiPFJetAve500      );
            fChain->SetBranchAddress("HLT_DiPFJetAve60_HFJEC" , & HLT_DiPFJetAve60_HFJEC , & b_HLT_DiPFJetAve60_HFJEC );
            fChain->SetBranchAddress("HLT_DiPFJetAve80_HFJEC" , & HLT_DiPFJetAve80_HFJEC , & b_HLT_DiPFJetAve80_HFJEC );
            fChain->SetBranchAddress("HLT_DiPFJetAve100_HFJEC", & HLT_DiPFJetAve100_HFJEC, & b_HLT_DiPFJetAve100_HFJEC);
            fChain->SetBranchAddress("HLT_DiPFJetAve160_HFJEC", & HLT_DiPFJetAve160_HFJEC, & b_HLT_DiPFJetAve160_HFJEC);
            fChain->SetBranchAddress("HLT_DiPFJetAve220_HFJEC", & HLT_DiPFJetAve220_HFJEC, & b_HLT_DiPFJetAve220_HFJEC);
            fChain->SetBranchAddress("HLT_DiPFJetAve300_HFJEC", & HLT_DiPFJetAve300_HFJEC, & b_HLT_DiPFJetAve300_HFJEC);
        }
    }//DiJet
}//loadTree

NanoTree::NanoTree(){
    fCurrent  = -1;
}
NanoTree::~NanoTree(){
    delete fChain;
}

Long64_t NanoTree::GetEntries(){
    return fChain->GetEntries();
}

Int_t NanoTree::GetEntry(Long64_t entry){
    return fChain->GetEntry(entry);
}

Long64_t NanoTree::loadEntry(Long64_t entry)                                  
{                                                                              
// Set the environment to read one entry                                                  
   if (!fChain) return -5;                                                     
   Long64_t centry = fChain->LoadTree(entry);                                  
   if (centry < 0) return centry;                                              
   if (fChain->GetTreeNumber() != fCurrent) {                                  
      fCurrent = fChain->GetTreeNumber();                                      
   }                                                                           
   //cout<<entry<<", "<<centry<<", "<<fCurrent<<endl;
   return centry;                                                              
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
