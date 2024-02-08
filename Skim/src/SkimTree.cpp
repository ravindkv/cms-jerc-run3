#include "SkimTree.h"

SkimTree::SkimTree(TString year, vector<string>fileNames, bool isMC){
    chain = new TChain("Events");
    std::cout << "Start SkimTree" << std::endl;
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
/*
    // event
    chain->SetBranchStatus("run",1);
    chain->SetBranchStatus("event",1);
    chain->SetBranchStatus("luminosityBlock",1);

    //objects
    chain->SetBranchStatus("PV_*",1);
    chain->SetBranchStatus("MET*",1);
    chain->SetBranchStatus("RawPuppiMET*",1);
    chain->SetBranchStatus("Electron_*",1);
    chain->SetBranchStatus("TrigObj_*",1);
    chain->SetBranchStatus("Muon_*",1);
    chain->SetBranchStatus("Jet_*",1);
    chain->SetBranchStatus("FatJet_*",1);
    chain->SetBranchStatus("Photon_*",1);
    chain->SetBranchStatus("Flag_*",1);
    chain->SetBranchStatus("Rho_fixedGridRhoFastjetAll",1);

    //size of objects
    chain->SetBranchStatus("nElectron",1);
    chain->SetBranchStatus("nTrigObj",1);
    chain->SetBranchStatus("nMuon",1);
    chain->SetBranchStatus("nJet",1);
    chain->SetBranchStatus("nFatJet",1);
    chain->SetBranchStatus("nPhoton",1);
    if (isMC){
	    chain->SetBranchStatus("Pileup_*",1);
        chain->SetBranchStatus("GenPart_*",1);
        chain->SetBranchStatus("GenJet_*",1);
        chain->SetBranchStatus("GenJetAK8_*",1);
        chain->SetBranchStatus("nGenPart",1);
        chain->SetBranchStatus("nGenJet",1);
        chain->SetBranchStatus("nGenJetAK8",1);
        chain->SetBranchStatus("GenIsolatedPhoton*",1);
        chain->SetBranchStatus("nGenIsolatedPhoton",1);
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
        if(year.Contains("2018")){ 
            chain->SetBranchStatus("L1PreFiringWeight_Dn",1);
            chain->SetBranchStatus("L1PreFiringWeight_Nom",1);
            chain->SetBranchStatus("L1PreFiringWeight_Up",1);
        }
    }
*/
    chain->SetBranchStatus("HLT_Photon*", 1);
    //2022
    if(year.Contains("2022")){ 
        chain->SetBranchAddress("HLT_Photon300_NoHE"                                                , & HLT_Photon300_NoHE);
        chain->SetBranchAddress("HLT_Photon20"                                                      , & HLT_Photon20);
        chain->SetBranchAddress("HLT_Photon33"                                                      , & HLT_Photon33);
        chain->SetBranchAddress("HLT_Photon50"                                                      , & HLT_Photon50);
        chain->SetBranchAddress("HLT_Photon75"                                                      , & HLT_Photon75);
        chain->SetBranchAddress("HLT_Photon90"                                                      , & HLT_Photon90);
        chain->SetBranchAddress("HLT_Photon120"                                                     , & HLT_Photon120);
        chain->SetBranchAddress("HLT_Photon150"                                                     , & HLT_Photon150);
        chain->SetBranchAddress("HLT_Photon175"                                                     , & HLT_Photon175);
        chain->SetBranchAddress("HLT_Photon200"                                                     , & HLT_Photon200);
        chain->SetBranchAddress("HLT_Photon30EB_TightID_TightIso"                                   , & HLT_Photon30EB_TightID_TightIso);
        chain->SetBranchAddress("HLT_Photon100EB_TightID_TightIso"                                  , & HLT_Photon100EB_TightID_TightIso);
        chain->SetBranchAddress("HLT_Photon110EB_TightID_TightIso"                                  , & HLT_Photon110EB_TightID_TightIso);
        chain->SetBranchAddress("HLT_Photon120EB_TightID_TightIso"                                  , & HLT_Photon120EB_TightID_TightIso);
        chain->SetBranchAddress("HLT_Photon100EBHE10"                                               , & HLT_Photon100EBHE10);
        chain->SetBranchAddress("HLT_Photon100EEHE10"                                               , & HLT_Photon100EEHE10);
        chain->SetBranchAddress("HLT_Photon100EE_TightID_TightIso"                                  , & HLT_Photon100EE_TightID_TightIso);
        chain->SetBranchAddress("HLT_Photon50_R9Id90_HE10_IsoM"                                     , & HLT_Photon50_R9Id90_HE10_IsoM);
        chain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM"                                     , & HLT_Photon75_R9Id90_HE10_IsoM);
        chain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3" , & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ300_PFJetsMJJ400DEta3);
        chain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3" , & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_CaloMJJ400_PFJetsMJJ600DEta3);
        chain->SetBranchAddress("HLT_Photon90_R9Id90_HE10_IsoM"                                     , & HLT_Photon90_R9Id90_HE10_IsoM);
        chain->SetBranchAddress("HLT_Photon120_R9Id90_HE10_IsoM"                                    , & HLT_Photon120_R9Id90_HE10_IsoM);
        chain->SetBranchAddress("HLT_Photon165_R9Id90_HE10_IsoM"                                    , & HLT_Photon165_R9Id90_HE10_IsoM);
        chain->SetBranchAddress("HLT_Photon35_TwoProngs35"                                          , & HLT_Photon35_TwoProngs35);
        chain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL"                                  , & HLT_Photon60_R9Id90_CaloIdL_IsoL);
        chain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL"                     , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL);
        chain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15"   , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350MinPFJet15);
        chain->SetBranchAddress("HLT_Photon20_HoverELoose"                                          , & HLT_Photon20_HoverELoose);
        chain->SetBranchAddress("HLT_Photon30_HoverELoose"                                          , & HLT_Photon30_HoverELoose);
        chain->SetBranchAddress("HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50"    , & HLT_Photon50_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3_PFMET50);
        chain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3"            , & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3);
        chain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3"            , & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ600DEta3);
    }
    //2023
    if(year.Contains("2023")){ 
        chain->SetBranchAddress("HLT_Photon300_NoHE"                                    , & HLT_Photon300_NoHE                                    );
        chain->SetBranchAddress("HLT_Photon33"                                          , & HLT_Photon33                                          );
        chain->SetBranchAddress("HLT_Photon50"                                          , & HLT_Photon50                                          );
        chain->SetBranchAddress("HLT_Photon75"                                          , & HLT_Photon75                                          );
        chain->SetBranchAddress("HLT_Photon90"                                          , & HLT_Photon90                                          );
        chain->SetBranchAddress("HLT_Photon120"                                         , & HLT_Photon120                                         );
        chain->SetBranchAddress("HLT_Photon150"                                         , & HLT_Photon150                                         );
        chain->SetBranchAddress("HLT_Photon175"                                         , & HLT_Photon175                                         );
        chain->SetBranchAddress("HLT_Photon200"                                         , & HLT_Photon200                                         );
        chain->SetBranchAddress("HLT_Photon30EB_TightID_TightIso"                       , & HLT_Photon30EB_TightID_TightIso                       );
        chain->SetBranchAddress("HLT_Photon50EB_TightID_TightIso"                       , & HLT_Photon50EB_TightID_TightIso                       );
        chain->SetBranchAddress("HLT_Photon75EB_TightID_TightIso"                       , & HLT_Photon75EB_TightID_TightIso                       );
        chain->SetBranchAddress("HLT_Photon90EB_TightID_TightIso"                       , & HLT_Photon90EB_TightID_TightIso                       );
        chain->SetBranchAddress("HLT_Photon110EB_TightID_TightIso"                      , & HLT_Photon110EB_TightID_TightIso                      );
        chain->SetBranchAddress("HLT_Photon130EB_TightID_TightIso"                      , & HLT_Photon130EB_TightID_TightIso                      );
        chain->SetBranchAddress("HLT_Photon150EB_TightID_TightIso"                      , & HLT_Photon150EB_TightID_TightIso                      );
        chain->SetBranchAddress("HLT_Photon175EB_TightID_TightIso"                      , & HLT_Photon175EB_TightID_TightIso                      );
        chain->SetBranchAddress("HLT_Photon200EB_TightID_TightIso"                      , & HLT_Photon200EB_TightID_TightIso                      );
        chain->SetBranchAddress("HLT_Photon100EBHE10"                                   , & HLT_Photon100EBHE10                                   );
        chain->SetBranchAddress("HLT_Photon50_R9Id90_HE10_IsoM"                         , & HLT_Photon50_R9Id90_HE10_IsoM                         );
        chain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM"                         , & HLT_Photon75_R9Id90_HE10_IsoM                         );
        chain->SetBranchAddress("HLT_Photon90_R9Id90_HE10_IsoM"                         , & HLT_Photon90_R9Id90_HE10_IsoM                         );
        chain->SetBranchAddress("HLT_Photon120_R9Id90_HE10_IsoM"                        , & HLT_Photon120_R9Id90_HE10_IsoM                        );
        chain->SetBranchAddress("HLT_Photon165_R9Id90_HE10_IsoM"                        , & HLT_Photon165_R9Id90_HE10_IsoM                        );
        chain->SetBranchAddress("HLT_Photon35_TwoProngs35"                              , & HLT_Photon35_TwoProngs35                              );
        chain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350" , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT350 );
        chain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380" , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT380 );
        chain->SetBranchAddress("HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400" , & HLT_Photon60_R9Id90_CaloIdL_IsoL_DisplacedIdL_PFHT400 );
        chain->SetBranchAddress("HLT_Photon20_HoverELoose"                              , & HLT_Photon20_HoverELoose                              );
        chain->SetBranchAddress("HLT_Photon30_HoverELoose"                              , & HLT_Photon30_HoverELoose                              );
        chain->SetBranchAddress("HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3", & HLT_Photon75_R9Id90_HE10_IsoM_EBOnly_PFJetsMJJ300DEta3);
        chain->SetBranchAddress("HLT_Photon32_OneProng32_M50To105"                      , & HLT_Photon32_OneProng32_M50To105                      );
    }
}

SkimTree::~SkimTree(){
    delete chain;
}

Long64_t SkimTree::GetEntries(){
    return chain->GetEntries();
}

Int_t SkimTree::GetEntry(Long64_t entry){
    return chain->GetEntry(entry);
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
std::vector<int> SkimTree::getJobs(std::string jobStr){
	std::vector<int> jobs;
    // Find the position of "of"
    size_t posOf = jobStr.find("of");
    if (posOf != std::string::npos) {
        // Extract the substring before "of" (first number)
        std::string num1 = jobStr.substr(0, posOf);
        // Extract the substring after "of" (second number)
        std::string num2 = jobStr.substr(posOf + 2);
        // Output the numbers
        std::cout << "nthJob: " << num1 << std::endl;
        std::cout << "totJob: " << num2 << std::endl;
		jobs.push_back(std::stoi(num1));
		jobs.push_back(std::stoi(num2));
    } else {
        std::cout << "Invalid format =  " << jobStr << std::endl;
    }
	return jobs;
}
