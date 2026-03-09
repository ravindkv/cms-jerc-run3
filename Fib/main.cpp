#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <sys/stat.h>

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TString.h>

#include <ROOT/RDataFrame.hxx>
#include <nlohmann/json.hpp>

#include "SkimTree.h"

int main(int argc, char* argv[])
{
    // ----------------------------
    // Default JSON (for -h and default outName)
    // ----------------------------
    const std::string fileDefault = "input/json/FilesFib_MultiJet_2024_Data_2024I.json"; // for help ONLY
    std::ifstream fileDefault_(fileDefault.c_str());
    if (!fileDefault_.is_open()) {
        std::cerr << "[main] ERROR: Could not open default JSON: " << fileDefault << "\n";
        return EXIT_FAILURE;
    }

    nlohmann::json js;
    try {
        js = nlohmann::json::parse(fileDefault_);
    } catch (const std::exception& e) {
        std::cerr << "\nEXCEPTION: Check the input json fileDefault: " << fileDefault << "\n";
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    // ----------------------------
    // Parse command-line options
    // ----------------------------
    int opt;
    std::string outName;

    if (js.empty()) {
        std::cerr << "[main] ERROR: JSON file is empty!\n";
        return EXIT_FAILURE;
    }

    // Default output name based on first key in JSON
    outName = js.begin().key() + std::string(".root");

    while ((opt = getopt(argc, argv, "o:h")) != -1) {
        switch (opt) {
            case 'o':
                outName = optarg;
                break;
            case 'h':
                std::cout << "Default input json: " << fileDefault << "\n";
                std::cout << "Usage: ./runMain -o sKey.root\n\n";
                for (auto& element : js.items()) {
                    std::cout << "./runMain -o " << element.key() << ".root\n";
                }
                return 0;
            default:
                std::cerr << "Invalid option\n";
                return EXIT_FAILURE;
        }
    }

    std::cout << "\n./runMain -o " << outName << "\n";

    // ----------------------------
    // Set and load SkimTree (your existing machinery)
    // ----------------------------
    std::cout << "\n--------------------------------------\n";
    std::cout << " Set and load SkimTree\n";
    std::cout << "--------------------------------------\n";

    SkimTree *skimT = new SkimTree();
    skimT->setInput(outName);
    skimT->loadInput();                 // sets beginRun/endRun etc (your current logic)
    skimT->setInputJsonPath("input/json/");
    skimT->loadInputJson();
    skimT->loadJobFileNames();
    skimT->loadTree();                  // builds fChain

    // Basic guards
    if (!skimT->fChain) {
        std::cerr << "[main] ERROR: skimT->fChain is nullptr\n";
        delete skimT;
        return EXIT_FAILURE;
    }
    if (skimT->fChain->GetNtrees() == 0) {
        std::cerr << "[main] ERROR: fChain has no trees (GetNtrees()==0). Did loadTree() fail?\n";
        delete skimT;
        return EXIT_FAILURE;
    }

    // Force load first tree so branch discovery is stable
    if (skimT->fChain->LoadTree(0) < 0 || !skimT->fChain->GetTree()) {
        std::cerr << "[main] ERROR: could not LoadTree(0) / GetTree()\n";
        delete skimT;
        return EXIT_FAILURE;
    }

    // ----------------------------
    // Output directory and final path
    // ----------------------------
    const std::string outDir = "output";
    mkdir(outDir.c_str(), S_IRWXU);

    const std::string outPath = outDir + "/" + outName;
    std::cout << "Output file: " << outPath << "\n";

    // ----------------------------
    // Run range selection
    // ----------------------------
    int startRun = 0;
    int endRun   = 0;
    try {
        startRun = std::stoi(skimT->beginRun);
        endRun   = std::stoi(skimT->endRun);
    } catch (...) {
        std::cerr << "[main] ERROR: beginRun/endRun are not valid integers: '"
                  << skimT->beginRun << "' '" << skimT->endRun << "'\n";
        delete skimT;
        return EXIT_FAILURE;
    }

    std::cout << "startRun = " << startRun << ", endRun = " << endRun << "\n";

    // ----------------------------
    // Chain I/O tuning (helps even without MT)
    // ----------------------------
    skimT->fChain->SetCacheSize(256 * 1024 * 1024);
    skimT->fChain->SetCacheLearnEntries(10000);

    // ----------------------------
    // RDataFrame pipeline: Filter by run range and snapshot
    // ----------------------------
    std::cout << "\n[RDataFrame] Filtering by run range and writing Events...\n";

    // NOTE: no EnableImplicitMT() since you said MT does not work on condor nodes.
    ROOT::RDataFrame df(*skimT->fChain);

    // Use a typed lambda. run is typically UInt_t in NanoAOD-like trees.
    auto dfSel = df.Filter(
        [startRun, endRun](UInt_t run) {
            return (int)run >= startRun && (int)run <= endRun;
        },
        {"run"},
        "run_range"
    );

    // Snapshot writes the selected entries to a NEW file (RECREATE)
    dfSel.Snapshot("Events", outPath.c_str());

    // ----------------------------
    // Write cutflow histogram (after snapshot, cheap to read output entries)
    // ----------------------------
    {
        TFile fOut(outPath.c_str(), "UPDATE");
        if (fOut.IsZombie()) {
            std::cerr << "[main] ERROR: Could not open output in UPDATE: " << outPath << "\n";
        } else {
            TTree *tOut = dynamic_cast<TTree*>(fOut.Get("Events"));
            Long64_t nSel = tOut ? tOut->GetEntries() : 0;

            TH1F *hEvents_ = new TH1F("hEvents", "Cutflow", 5, -1.5, 3.5);
            hEvents_->GetXaxis()->SetBinLabel(1, "Skim");
            // Put the actual selected count in bin 1 (same semantics as your old Fill(1) loop)
            hEvents_->SetBinContent(1, nSel);

            hEvents_->Write();
            fOut.Close();

            std::cout << "[main] nEvents_Fib (selected) = " << nSel << "\n";
        }
    }

    std::cout << "\nDone.\n";
    std::cout << "Output file = " << outPath << "\n";

    delete skimT;
    return 0;
}

