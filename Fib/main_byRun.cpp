#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <iomanip>
#include <chrono> // For timing

#include "SkimTree.h"
#include <nlohmann/json.hpp>

#include <sys/stat.h>

// Struct to hold event information
struct EventEntry {
    int run;
    ULong64_t event;
    Long64_t entryNumber;
};

// Comparator for sorting by event number within the same run
bool compareByEvent(const EventEntry& a, const EventEntry& b) {
    return a.event < b.event;
}

void sortTreeByRun(TFile* inFile, const std::string& outputDirectory, int startRun, int endRun) {
    if (!inFile || inFile->IsZombie()) {
        std::cerr << "Error opening " << (inFile ? inFile->GetName() : "input file") << std::endl;
        return;
    }
    inFile->cd();

    // Get the input tree
    TTree* inTree = (TTree*)inFile->Get("Events");
    if (!inTree) {
        std::cerr << "Error: TTree 'Events' not found in " << inFile->GetName() << std::endl;
        inFile->Close();
        return;
    }

    // Optimize cache
    inTree->SetCacheSize(500 * 1024 * 1024);  // 500 MB
    inTree->SetCacheLearnEntries(10000);      

    // Set branches for reading run and event
    UInt_t run = 0;
    ULong64_t event = 0;
    inTree->SetBranchStatus("*", 0);
    inTree->SetBranchStatus("run", 1);
    inTree->SetBranchStatus("event", 1);
    inTree->SetBranchAddress("run", &run);
    inTree->SetBranchAddress("event", &event);

    Long64_t nEntries = inTree->GetEntries();
    std::cout << "Reading " << nEntries << " entries..." << std::endl;

    // Map of run -> vector of events for that run
    std::map<int, std::vector<EventEntry>> runMap;

    // Read all events and group by run
    for (Long64_t i = 0; i < nEntries; i++) {
        inTree->GetEntry(i);
        if (run >= (UInt_t)startRun && run <= (UInt_t)endRun) {
            EventEntry evt;
            evt.run = (int)run;
            evt.event = event;
            evt.entryNumber = i;
            runMap[evt.run].push_back(evt);
        }
    }

    inFile->Close(); // We can close the input file here if desired

    // Now we have events grouped by run.
    // For each run, we sort by event and write out a separate file/tree.
    for (auto &runPair : runMap) {
        int currentRun = runPair.first;
        std::vector<EventEntry> &runEvents = runPair.second;

        std::cout << "Sorting run " << currentRun << " with " << runEvents.size() << " events..." << std::endl;
        std::sort(runEvents.begin(), runEvents.end(), compareByEvent);
        std::cout << "Sorting for run " << currentRun << " completed." << std::endl;

        // Re-open input file to read data again (or keep it open if memory is not an issue)
        TFile* inFileAgain = TFile::Open(inFile->GetName(), "READ");
        TTree* inTreeAgain = (TTree*)inFileAgain->Get("Events");
        inTreeAgain->SetBranchStatus("*",1);

        // Clone the tree structure for output (single run)
        std::string outFileName = outputDirectory + "/run_" + std::to_string(currentRun) + ".root";
        TFile* outFile = TFile::Open(outFileName.c_str(), "RECREATE");
        TTree* outTree = inTreeAgain->CloneTree(0);

        double totalTime = 0.0;
        auto startClock = std::chrono::high_resolution_clock::now();

        Long64_t totalEntriesRun = (Long64_t)runEvents.size();
        // Fill the output tree with sorted entries of this run
        for (Long64_t i = 0; i < totalEntriesRun; i++) {
            if (totalEntriesRun > 100 && i > 0 && i % (totalEntriesRun / 100) == 0) {
                totalTime += std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startClock).count();
                int sec = static_cast<int>(totalTime) % 60;
                int min = static_cast<int>(totalTime) / 60;
                std::cout << std::setw(10) << (100*i/totalEntriesRun) << " %"
                          << std::setw(10) << min << "m " << sec << "s (Run " << currentRun << ")" << std::endl;
                startClock = std::chrono::high_resolution_clock::now();
            }

            Long64_t entryNumber = runEvents[i].entryNumber;
            
            inTreeAgain->GetEntry(entryNumber);
            outTree->Fill();
        }

        std::cout << "Finished filling " << outTree->GetEntries() 
                  << " events for run " << currentRun << std::endl;

        outFile->cd();
        outTree->Write();
        outFile->Close();
        inFileAgain->Close();
    }

    std::cout << "Done. You now have one file per run in " << outputDirectory << "." << std::endl;
    std::cout << "You can now hadd them to get a single sorted file." << std::endl;
}

int main(int argc, char* argv[]){
    // Default input JSON file
    std::string fileDefault = "input/json/FilesFib_2024_DiJet.json";// for help ONLY
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
    TFile* tempFile_ = TFile::Open("temp.root", "RECREATE");
    //TFile* tempFile_ = TFile::Open(outDir + "/" + oName, "RECREATE");
    tempFile_->cd();
    TTree* newTree = skimT->fChain->GetTree()->CloneTree(0);
    newTree->SetCacheSize(50*1024*1024);
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
    std::cout << "Output file = " << tempFile_->GetName() << std::endl;

    newTree->Write();
    hEvents_->Write();
    tempFile_->Close();

    std::cout << "\nSorting the tree by event number..." << std::endl;
    TFile* inFile = TFile::Open("temp.root", "READ");
    TFile* outFile = TFile::Open(outDir + "/" + oName, "RECREATE");
    //sortTree(inFile, outFile);
    sortTreeByRun(inFile, "output", 380963, 381065);
    outFile->cd();
    std::cout << "Output file = " << outFile->GetName() << std::endl;
    inFile->Close();
    outFile->Close();
    std::remove("temp.root");

    return 0;
}

