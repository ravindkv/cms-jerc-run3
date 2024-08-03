#include "EventPick.h"

EventPick::~EventPick(){
}
//https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#Run_3_recommendations
bool EventPick::passFilter(SkimTree *tree){
    //We can also pick year-specific filters
    //if(isData){}
    //if(is2022){}
    bool pass = 
    (tree->Flag_goodVertices &&
    tree->Flag_globalSuperTightHalo2016Filter &&
    tree->Flag_EcalDeadCellTriggerPrimitiveFilter &&
    tree->Flag_BadPFMuonFilter &&
    tree->Flag_BadPFMuonDzFilter &&
    tree->Flag_hfNoisyHitsFilter &&
    tree->Flag_eeBadScFilter &&
    tree->Flag_ecalBadCalibFilter);

    if(isDebug){
      cout<<"tree->Flag_goodVertices                       = "<<tree->Flag_goodVertices <<endl;
      cout<<"tree->Flag_globalSuperTightHalo2016Filter     = "<<tree->Flag_globalSuperTightHalo2016Filter <<endl;
      cout<<"tree->Flag_EcalDeadCellTriggerPrimitiveFilter = "<<tree->Flag_EcalDeadCellTriggerPrimitiveFilter <<endl;
      cout<<"tree->Flag_BadPFMuonFilter                    = "<<tree->Flag_BadPFMuonFilter <<endl;
      cout<<"tree->Flag_BadPFMuonDzFilter                  = "<<tree->Flag_BadPFMuonDzFilter <<endl;
      cout<<"tree->Flag_hfNoisyHitsFilter                  = "<<tree->Flag_hfNoisyHitsFilter <<endl;
      cout<<"tree->Flag_eeBadScFilter                      = "<<tree->Flag_eeBadScFilter <<endl;
      cout<<"tree->Flag_ecalBadCalibFilter)                = "<<tree->Flag_ecalBadCalibFilter<<endl;
    }

    return pass;
}

