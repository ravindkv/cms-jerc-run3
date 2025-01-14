#include "SkimTree.h"

void SkimTree::setInput(string oName){
    iName  = oName;
    cout<<"+ setInput() = "<<iName<<endl;
    std::vector<std::string> v_iName      = splitString(iName, ".root");
    loadedFibKey = v_iName.at(0); 
    std::cout << "loadedFibKey: " << loadedFibKey << std::endl;
} 
void SkimTree::loadInput(){
    cout<<"==> loadInput()"<<endl;
    loadedNthJob = 1; 
    loadedTotJob = 1;
} 
void SkimTree::setInputJsonPath(string inDir){
    string year = "2022";
    TString oN = iName;
    if(oN.Contains("2023")) year = "2023";
    if(oN.Contains("2024")) year = "2024";
    string channel = "GamJet";
    if(oN.Contains("DiJet")) channel = "DiJet";
    if(oN.Contains("DiMuJet")) channel = "DiMuJet";
    if(oN.Contains("DiEleJet")) channel = "DiEleJet";
    inputSkimJson = inDir+"/FilesSkim_"+year+"_"+channel+".json";
    inputFibJson = inDir+"/FilesFib_"+year+"_"+channel+".json";
    cout<<"+ setInputJsonPath() = "<<inputSkimJson<<endl;
    cout<<"+ setInputJsonPath() = "<<inputFibJson<<endl;
}

void SkimTree:: loadInputJson(){
    cout<<"==> loadInputJson()"<<endl;
    ifstream fileNameSkim_(inputSkimJson.c_str());
    nlohmann::json jsSkim;
    ifstream fileNameFib_(inputFibJson.c_str());
    nlohmann::json jsFib;
    try{
        jsSkim = nlohmann::json::parse(fileNameSkim_);
        jsFib = nlohmann::json::parse(fileNameFib_);
    } catch (const exception& e) {
        cout<<"\nEXCEPTION: Check the input  inputSkimJson: "<<inputSkimJson<<endl;
        cout<<"\nEXCEPTION: Check the input inputFibJson: "<<inputFibJson<<endl;
        cout<<e.what()<<endl;
        abort();
    }
    try{
        std::vector<std::string> fibVals;
        jsFib.at(loadedFibKey).get_to(fibVals);
        loadedSkimKey = fibVals.at(0);
        beginRun = fibVals.at(1);
        endRun   = fibVals.at(2);
        jsSkim.at(loadedSkimKey).get_to(loadedAllFileNames);
    }catch (const exception & e){
        cout<<"\nEXCEPTION: Check the loadedSkimKey: "<<loadedSkimKey<<endl;
        cout<<e.what()<<endl;
        cout<<"Choose loadedSkimKey from the following:"<<endl;
        for (auto& element : jsSkim.items()) {
            cout << element.key() << endl;
        }
        abort();
    }//
}

void SkimTree::loadJobFileNames(){
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
}

void SkimTree::loadTree(){
    cout << "==> loadTree()" << endl;
    TString oN = iName;
    std::cout << "Start SkimTree" << std::endl;
    fChain->SetCacheSize(100*1024*1024);
    int nFiles = loadedJobFileNames.size();
    string dir = "";

    for(int fileI = 0; fileI < nFiles; fileI++){
        string fName = loadedJobFileNames[fileI];
        string fullPath = dir + fName;

        // Open the file
        TFile *file = TFile::Open(fullPath.c_str(), "READ");
        if (!file || file->IsZombie()) {
            std::cerr << "Error: Could not open file " << fullPath << std::endl;
            if (file) file->Close();
            continue;
        }

        // Check if the 'Events' TTree exists
        TTree *tree = static_cast<TTree*>(file->Get("Events"));
        if (!tree) {
            std::cerr << "Warning: 'Events' TTree not found in file " << fullPath << std::endl;
            file->Close();
            continue;
        }

        // Check for the presence of 'HLT_ZeroBias' branch
        if (!tree->GetListOfBranches()->FindObject("HLT_ZeroBias")) {
            std::cerr << "Warning: 'HLT_ZeroBias' branch not found in file " << fullPath << std::endl;
            file->Close();
            continue;
        }

        // Add the file to fChain
        fChain->Add(fullPath.c_str());
        std::cout << fullPath << " added to fChain with entries: " << fChain->GetEntries() << std::endl;

        // Close the file
        file->Close();
    }

    std::cout << "Begin" << std::endl;

    // Example branch setting (uncomment or modify as needed)
    // fChain->SetBranchStatus("*", 0);
    // fChain->SetBranchStatus("run", 1);
    // fChain->SetBranchStatus("event", 1);
    // fChain->SetBranchStatus("luminosityBlock", 1);

    fChain->SetBranchAddress("run", &run, &b_run);
    fChain->SetBranchAddress("event", &event, &b_event);
    fChain->SetBranchAddress("luminosityBlock", &lumi, &b_lumi);
}


SkimTree::SkimTree(){
    fCurrent  = -1;
}
SkimTree::~SkimTree(){
    delete fChain;
}

Long64_t SkimTree::GetEntries(){
    return fChain->GetEntries();
}

Int_t SkimTree::GetEntry(Long64_t entry){
    return fChain->GetEntry(entry);
}

Long64_t SkimTree::loadEntry(Long64_t entry)                                  
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
