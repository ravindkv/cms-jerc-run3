#include "GlobalFlag.h"

GlobalFlag::GlobalFlag(const string& outName) : 
    isDebug(false), 
    is2022(false), 
    is2023(false), 
    is2024(false), 
    is2025(false), 
    isData(false), 
    isMC(false), 
    isZeeJet(false), 
    isZmmJet(false), 
    isGamJet(false), 
    isMultiJet(false), 
    isWqqm(false), 
    isWqqe(false), 
    isQCD(false), 
    isMG(false){

    if (outName.find("2022")!= string::npos) is2022   = true;
    if (outName.find("2023")!= string::npos) is2023 = true;
    if (outName.find("2024")!= string::npos) is2024 = true;
    if (outName.find("2025")!= string::npos) is2025 = true;

    if (outName.find("Data")!= string::npos) isData = true;
    if (outName.find("MC")!= string::npos)   isMC = true;

    if(outName.find("ZeeJet")!= string::npos)    isZeeJet   = true;
    if(outName.find("ZmmJet")!= string::npos)    isZmmJet    = true;
    if(outName.find("GamJet")!= string::npos)    isGamJet    = true;
    if(outName.find("MultiJet")!= string::npos)  isMultiJet  = true;
    if(outName.find("Wqqm")!= string::npos)       isWqqm       = true;
    if(outName.find("Wqqe")!= string::npos)       isWqqe       = true;

    if(outName.find("QCD")!= string::npos)    isQCD    = true;
    if(outName.find("MC")!= string::npos)     isMG     = true;
}

void GlobalFlag::printFlag(){
    if(isDebug   )cout<<"isDebug   = true"<<'\n';
    if(is2022   )cout<<"is2022   = true"<<'\n';
    if(is2023   )cout<<"is2023   = true"<<'\n';
    if(is2024   )cout<<"is2024   = true"<<'\n';
    if(is2025   )cout<<"is2025   = true"<<'\n';
    if(isData    )cout<<"isData    = true"<<'\n';
    if(isMC      )cout<<"isMC      = true"<<'\n';
    if(isZeeJet  )cout<<"isZeeJet  = true"<<'\n';
    if(isZmmJet  )cout<<"isZmmJet  = true"<<'\n';
    if(isGamJet  )cout<<"isGamJet  = true"<<'\n';
    if(isMultiJet)cout<<"isMultiJet= true"<<'\n';
    if(isWqqm     )cout<<"isWqqm     = true"<<'\n';
    if(isWqqe     )cout<<"isWqqe     = true"<<'\n';

    if(isQCD     )cout<<"isQCD     = true"<<'\n';
    if(isMG      )cout<<"isMG      = true"<<'\n';
}


