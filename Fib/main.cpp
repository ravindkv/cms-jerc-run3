#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <iomanip>
#include <chrono> // For timing

#include "SkimTree.h"
#include <nlohmann/json.hpp>

#include <sys/stat.h>

// Struct to hold event information for sorting
struct EventEntry {
    int run;
    ULong64_t event;
    Long64_t entryNumber;
};

// Comparator to sort EventEntry by run and then by event number
bool compareByRunAndEvent(const EventEntry& a, const EventEntry& b) {
    if (a.run != b.run)
        return a.run < b.run;
    return a.event < b.event;
}

void sortTree(TFile * inFile, TFile * outFile){
    // Open the temporary ROOT file
    if (!inFile || inFile->IsZombie()) {
        std::cerr << "Error opening "<<inFile->GetName() << std::endl;
        return;
    }
    inFile->cd();
    // Get the tempTree
    TTree* inTree = (TTree*)inFile->Get("Events"); // Ensure the tree name matches
    inTree->SetCacheSize(100 * 1024 * 1024); // 100 MB
    inTree->SetCacheLearnEntries(1000);     // Number of entries to learn cache patterns

    if (!inTree) {
        std::cerr << "Error: TTree 'Events' not found in "<<inFile->GetName() << std::endl;
        inFile->Close();
        return;
    }

    if (!outFile || outFile->IsZombie()) {
        std::cerr << "Error creating output file "<<outFile->GetName() << std::endl;
        return;
    }
    outFile->cd();
    
    // Set branch addresses for sorting
    UInt_t run = 0;
    ULong64_t event = 0;
    TBranch *b_run_;
    TBranch *b_event_;
    inTree->SetBranchStatus("*", 0);
    inTree->SetBranchStatus("run", 1);
    inTree->SetBranchStatus("event", 1);
    inTree->SetBranchAddress("run", &run, &b_run_);
    inTree->SetBranchAddress("event", &event, &b_event_);

    // Collect event entries into a vector
    std::vector<EventEntry> sortedEventEntries;
    sortedEventEntries.reserve(inTree->GetEntries());

    for(Long64_t i = 0; i < inTree->GetEntries(); i++) {
        //inTree->GetEntry(i);
        b_run_->GetEntry(i);
        b_event_->GetEntry(i);
        EventEntry evt;
        evt.run = run;
        evt.event = event;
        evt.entryNumber = i;
        sortedEventEntries.push_back(evt);
    }

    int endEntry= sortedEventEntries.size();
    std::cout << "Sorting " <<  endEntry << " events by event number..." << std::endl;

    // Sort the vector by event number
    std::sort(sortedEventEntries.begin(), sortedEventEntries.end(), compareByRunAndEvent);

    std::cout << "Sorting completed. Filling the sorted tree..." << std::endl;


    // Clone the tree structure without copying entries
    inTree->SetBranchStatus("*", 1);
    TTree* outTree = inTree->CloneTree(0);
    outTree->SetCacheSize(100 * 1024 * 1024);  // 100 MB
    outTree->SetCacheLearnEntries(1000);  

    // Loop over the sorted events and fill the outTree
    double totalTime = 0.0;
    auto startClock = std::chrono::high_resolution_clock::now();
    int i = 0;
    for(const auto& evt : sortedEventEntries){
        i++;
        if(endEntry > 100 && i % (endEntry / 100) == 0){
            totalTime += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startClock).count();
            int sec = static_cast<int>(totalTime) % 60;
            int min = static_cast<int>(totalTime) / 60;
            std::cout << std::setw(10) << 100*i/endEntry << " %" 
                      << std::setw(10) << min << "m " << sec << "s" << std::endl;
            startClock = std::chrono::high_resolution_clock::now();
        }
        Long64_t entryNumber = evt.entryNumber;
        inTree->GetEntry(entryNumber);
        outTree->Fill();
    }
    inFile->Close();

    std::cout << "nEvents_Fib = " << outTree->GetEntries() << std::endl;

    // Write the sorted tree
    outFile->cd();
    outTree->Write();
}



int main(int argc, char* argv[]){
    // Default input JSON file
    std::string fileDefault = "input/json/FilesFib_GamJet_2024_DataReprocessing_2024C.json";// for help ONLY
    std::ifstream fileDefault_(fileDefault.c_str());
    nlohmann::json js; 
    try{
        js = nlohmann::json::parse(fileDefault_);
    } catch (const std::exception& e) {
        std::cerr << "\nEXCEPTION: Check the input json fileDefault: " << fileDefault << std::endl;
        std::cerr << e.what() << std::endl;
        std::abort();
    }

    //--------------------------------
    // Parse command-line options
    //--------------------------------
    int opt;
    std::string outName; //output Name
    // Set default output name based on the first key in JSON
    if(js.empty()) {
        std::cerr << "JSON file is empty!" << std::endl;
        return 1;
    }
    outName = js.begin().key() + std::string(".root"); //default value
    while ((opt = getopt(argc, argv, "o:h")) != -1) {
        switch (opt) {
            case 'o':
                outName = optarg;
                break;
            case 'h':
                std::cout << "Default input json: " << fileDefault << std::endl;
                std::cout << "Usage: ./runMain -o sKey.root\n" << std::endl;
                for (auto& element : js.items()) {
                    std::cout << "./runMain -o " << element.key() << ".root" << std::endl;
                }
                return 0;
            default:
                std::cerr << "Invalid option" << std::endl;
                return 1;
        }
    }
    std::cout << "\n./runMain -o " << outName << std::endl;

    std::cout << "\n--------------------------------------" << std::endl;
    std::cout << " Set and load SkimTree" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    SkimTree *skimT = new SkimTree();
    skimT->setInput(outName);
    skimT->loadInput();
    skimT->setInputJsonPath("input/json/");
    skimT->loadInputJson();
    skimT->loadJobFileNames();
    skimT->loadTree();

    TString oName = outName;
    std::string outDir = "output";
    mkdir(outDir.c_str(), S_IRWXU);
    std::cout << "new output file name: " << outDir + "/" + oName << std::endl;

    // Clone the tree structure without copying entries
    // Write newTree to a temporary file
    //TFile* tempFile_ = TFile::Open("temp.root", "RECREATE");
    //TFile* tempFile_ = TFile::Open(outDir + "/" + oName, "RECREATE");
    TFile* outFile = TFile::Open(outDir + "/" + oName, "RECREATE");
    outFile->cd();


    // *** GUARD #1: make sure fChain actually has a TTree ***
    if (skimT->fChain->GetNtrees() == 0) {
        std::cerr << "[RunGamJet] ERROR: fChain has no trees (GetNtrees()==0)."
                  << " Did loadTree() fail to add any files?\n";
        return EXIT_FAILURE;
    }

    // *** GUARD #2: force the chain to load its first tree ***
    Long64_t firstEntry = skimT->fChain->LoadTree(0);
    if (firstEntry < 0) {
        std::cerr << "[RunGamJet] ERROR: LoadTree(0) returned " << firstEntry
                  << ". No valid tree to read from.\n";
        return EXIT_FAILURE;
    }

    // *** GUARD #3: GetTree() must be non‐null ***
    TTree* baseTree = skimT->fChain->GetTree();
    if (!baseTree) {
        std::cerr << "[RunGamJet] ERROR: fChain->GetTree() is nullptr. Cannot CloneTree.\n";
        return EXIT_FAILURE;
    }

    // Now it’s safe:
    TTree* newTree = baseTree->CloneTree(0);
    if (!newTree) {
        std::cerr << "[RunGamJet] ERROR: CloneTree(0) returned nullptr. Check your input TTree.\n";
        return EXIT_FAILURE;
    }
    newTree->SetDirectory(outFile);


    Long64_t nEntr = skimT->GetEntries();

    int startEntry = 0;
    int endEntry = nEntr;
    int eventsPerJob = nEntr;
    std::cout << "Sample has " << nEntr << " entries" << std::endl;
    std::cout << "Processing events " << startEntry << " to " << endEntry << std::endl;

    // Assuming beginRun and endRun are already sorted
    int startRun = std::stoi(skimT->beginRun);
    int endRun   = std::stoi(skimT->endRun);
    std::cout << "startRun = " << startRun << ", endRun = " << endRun << std::endl;
    std::cout << "---------------------------" << std::endl;
    std::cout << std::setw(10) << "Progress" << std::setw(10) << "Time" << std::endl;
    std::cout << "---------------------------" << std::endl;

    double totalTime = 0.0;
    auto startClock = std::chrono::high_resolution_clock::now();

    // Vector to store event information
    std::vector<EventEntry> eventEntries;
    eventEntries.reserve(nEntr); // Reserve space to improve performance

    // Histogram for cutflow (optional)
    TH1F* hEvents_ = new TH1F("hEvents", "Cutflow", 5, -1.5, 3.5);
    hEvents_->GetXaxis()->SetBinLabel(1, "Skim");

    for(Long64_t i = startEntry; i < endEntry; i++){
        // Progress display (every 1%)
        if(endEntry > 100 && i % (eventsPerJob / 100) == 0){
            totalTime += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startClock).count();
            int sec = static_cast<int>(totalTime) % 60;
            int min = static_cast<int>(totalTime) / 60;
            std::cout << std::setw(10) << 100*i/endEntry << " %" 
                      << std::setw(10) << min << "m " << sec << "s" << std::endl;
            startClock = std::chrono::high_resolution_clock::now();
        }

        Long64_t entry = skimT->loadEntry(i);
        skimT->b_run->GetEntry(entry);

        // Apply run range selection
        if(skimT->run < startRun) continue;
        //cout<<skimT->run<<'\n';
        if(skimT->run > endRun) continue;
		skimT->fChain->GetTree()->GetEntry(entry);
        
        hEvents_->Fill(1);
        newTree->Fill();

    }
    std::cout << "nEvents_Fib = " << newTree->GetEntries() << std::endl;
    //std::cout << "Output file = " << tempFile_->GetName() << std::endl;

    newTree->Write();
    hEvents_->Write();
    //tempFile_->Close();

    std::cout << "\nSorting the tree by event number..." << std::endl;
    //TFile* inFile = TFile::Open("temp.root", "READ");
    //TFile* outFile = TFile::Open(outDir + "/" + oName, "RECREATE");
    //sortTree(inFile, outFile);
    outFile->cd();
    std::cout << "Output file = " << outFile->GetName() << std::endl;
    //inFile->Close();
    outFile->Close();
    std::remove("temp.root");

    return 0;
}

