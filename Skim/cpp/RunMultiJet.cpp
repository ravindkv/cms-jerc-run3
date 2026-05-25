#include "RunMultiJet.h"
#include "HistCutflow.h"
#include "Helper.h"
   
// Constructor implementation
RunMultiJet::RunMultiJet(GlobalFlag& globalFlags)
    :globalFlags_(globalFlags) {
}


auto RunMultiJet::Run(std::shared_ptr<NanoTree>& nanoT, TFile *fout) -> int{
    fout->cd();

	//----------------------------------
	// Set trigger list
	//----------------------------------
    std::vector<std::string> patterns;
    patterns = { "HLT_PFJet*", "HLT_DiPFJet*"};
    //trigList_  = Helper::GetMatchingBranchNames(nanoT->fChain->GetTree(), patterns);

    trigList_ = {
            "HLT_MC",
            "HLT_ZeroBias",
            "HLT_ZeroBias_FirstCollisionInTrain",
            "HLT_ZeroBias_LastCollisionInTrain",
            "HLT_PFJet40", 
            "HLT_PFJet60", 
            "HLT_PFJet80", 
            "HLT_PFJet110",
            "HLT_PFJet140",
            "HLT_PFJet200",
            "HLT_PFJet260",
            "HLT_PFJet320",
            "HLT_PFJet400",
            "HLT_PFJet450",
            "HLT_PFJet500",
            "HLT_PFJetFwd40", 
            "HLT_PFJetFwd60", 
            "HLT_PFJetFwd80", 
            "HLT_PFJetFwd140",
            "HLT_PFJetFwd200",
            "HLT_PFJetFwd260",
            "HLT_PFJetFwd320",
            "HLT_PFJetFwd400",
            "HLT_PFJetFwd450",
            "HLT_PFJetFwd500",
            "HLT_DiPFJetAve40",
            "HLT_DiPFJetAve60",
            "HLT_DiPFJetAve80",
            "HLT_DiPFJetAve140",
            "HLT_DiPFJetAve200",
            "HLT_DiPFJetAve260",
            "HLT_DiPFJetAve320",
            "HLT_DiPFJetAve400",
            "HLT_DiPFJetAve500",
            "HLT_DiPFJetAve60_HFJEC",
            "HLT_DiPFJetAve80_HFJEC",
            "HLT_DiPFJetAve100_HFJEC",
            "HLT_DiPFJetAve160_HFJEC",
            "HLT_DiPFJetAve220_HFJEC",
            "HLT_DiPFJetAve300_HFJEC"
    };

    if (trigList_.empty()) {
        std::cerr << "No triggers found for channel: MultiJet" << '\n';
        exit(EXIT_FAILURE);
    }
	for (const auto& trigN : trigList_) {
		nanoT->fChain->SetBranchStatus(trigN.c_str(), true);
	    nanoT->fChain->SetBranchAddress(trigN.c_str(), &trigVals_[trigN], &trigTBranches_[trigN]);
	} 

    // *** GUARD #1: make sure fChain actually has a TTree ***
    if (nanoT->fChain->GetNtrees() == 0) {
        std::cerr << "[RunGamJet] ERROR: fChain has no trees (GetNtrees()==0)."
                  << " Did loadTree() fail to add any files?\n";
        return EXIT_FAILURE;
    }

    // *** GUARD #2: force the chain to load its first tree ***
    Long64_t firstEntry = nanoT->fChain->LoadTree(0);
    if (firstEntry < 0) {
        std::cerr << "[RunGamJet] ERROR: LoadTree(0) returned " << firstEntry
                  << ". No valid tree to read from.\n";
        return EXIT_FAILURE;
    }

    // *** GUARD #3: GetTree() must be non‐null ***
    TTree* baseTree = nanoT->fChain->GetTree();
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
    newTree->SetDirectory(fout);
    newTree->SetCacheSize(Helper::tTreeCatchSize);
    Long64_t nentries = nanoT->getEntries();
    std::cout << "\nSample has "<<nentries << " entries" << '\n';

    //------------------------------------
    // Cutflow histograms
    //------------------------------------
    std::vector<std::string> cuts = { "NanoAOD", "Trigger"};
    auto h1EventInCutflow = std::make_unique<HistCutflow>("h1EventInCutflow", cuts, fout->mkdir("Cutflow"));
    
    
    //--------------------------------
    //Event loop
    //--------------------------------
    double totalTime = 0.0;
	auto startClock = std::chrono::high_resolution_clock::now();
    Helper::initProgress();
	for(Long64_t i= 0; i < nentries; i++){
        //if(i>100000) break;
        Helper::printProgress(i, nentries, startClock, totalTime);
        
        Long64_t entry = nanoT->loadEntry(i);
        h1EventInCutflow->fill("NanoAOD");

		for (const auto& trigN : trigList_) {
            if (!trigTBranches_[trigN]) continue;
		    trigTBranches_[trigN]->GetEntry(entry);//Read only content of HLT branches from NanoTree
		    if (trigVals_[trigN]) {
            	nanoT->fChain->GetTree()->GetEntry(entry);// Then read content of ALL branches
                h1EventInCutflow->fill("Trigger");
            	newTree->Fill();
		        break; // Event passes if any trigger is true
		    }
		}
    }

    // ---- Runs copy ----
    if (nanoT->fChainRuns->GetNtrees() == 0) {
        std::cerr << "Warning: fChainRuns has no trees. Skipping Runs processing.\n";
    } else {
        Long64_t firstRuns = nanoT->fChainRuns->LoadTree(0);
        if (firstRuns < 0) {
            std::cerr << "Warning: LoadTree(0) for Runs returned " << firstRuns
                      << ". Skipping Runs processing.\n";
        } else {
            TTree* runsTree = nanoT->fChainRuns->GetTree();
            if (!runsTree) {
                std::cerr << "Warning: fChainRuns->GetTree() is nullptr after LoadTree(). "
                             "Skipping Runs processing.\n";
            } else {
                TTree* newTreeRuns = runsTree->CloneTree(0);
                newTreeRuns->SetDirectory(fout);

                Long64_t nentriesRuns = nanoT->getEntriesRuns();
                for (Long64_t i = 0; i < nentriesRuns; ++i) {
                    Long64_t entry = nanoT->loadEntryRuns(i);
                    if (entry < 0) break;
                    nanoT->fChainRuns->GetTree()->GetEntry(entry);
                    newTreeRuns->Fill();
                }
                std::cout << "nRuns_Skim = " << newTreeRuns->GetEntries() << '\n';
            }
        }
    }

    // print cutflow, events already done above
    Helper::printCutflow(h1EventInCutflow->getHistogram());
    std::cout << "nEvents_Skim = " << newTree->GetEntries() << '\n';
    std::cout << "Output file path = " << fout->GetName() << '\n';
    fout->cd();
    fout->Write();
    return EXIT_SUCCESS;

}
