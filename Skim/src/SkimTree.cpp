#include "SkimTree.h"

SkimTree::SkimTree(string year, vector<string>fileNames, bool isMC){
    chain = new TChain("Events");
    std::cout << "Start SkimTree" << std::endl;
    chain->SetCacheSize(100*1024*1024);
    bool isCopy = false;
    int nFiles = fileNames.size();
    string dir = "root://cms-xrd-global.cern.ch/";
    //string dir = "root://cmsxrootd.fnal.gov/";
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
        chain->SetBranchStatus("L1PreFiringWeight_Dn",1);
        chain->SetBranchStatus("L1PreFiringWeight_Nom",1);
        chain->SetBranchStatus("L1PreFiringWeight_Up",1);
    }

    chain->SetBranchStatus("HLT_Photon*", 1);
    //2022
    chain->SetBranchAddress("HLT_Photon20" , &p20);
    chain->SetBranchAddress("HLT_Photon20_HoverELoose" , &p20HE);
    chain->SetBranchAddress("HLT_Photon30_HoverELoose" , &p30HE);
    chain->SetBranchAddress("HLT_Photon33" , &p33);
    chain->SetBranchAddress("HLT_Photon200" , &p200);

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
