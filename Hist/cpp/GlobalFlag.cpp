#include "GlobalFlag.h"

GlobalFlag::GlobalFlag(TString oName){
    isDebug = false;

    //Years
    is22 = false;
    is23 = false;
    is24 = false;
    
    //Eras 2022
    is22B = false;
    is22C = false;
    is22D = false;
    is22E = false;
    is22F = false;
    is22G = false;
    isPostEE = false;
    //Eras 2023
    is23B = false;
    is23C = false;
    is23Cv123 = false;
    is23Cv4   = false;
    is23D = false;
    is23E = false;
    is23F = false;
    is23G = false;
    is23Pre  = false;
    is23Post = false;
    //Eras 2024
    is24B = false;
    is24C = false;
    is24D = false;
    is24E = false;
    is24F = false;
    is24G = false;
    
    //Data or MC
    isData  = false;
    isMC    = false;
    
    //Channels
    isGamJet    = false;
    isZeeJet  = false;
    isZmmJet   = false;
    isMCTruth   = false;
    isFlavour= false;
    isVetoMap   = false;
    isDiJet     = false;
    isIncJet    = false;
    isMultiJet  = false;
    isWqq       = false;
    
    //Sample (e.g. QCD)
    isQCD = false;
    isMG  = false;

    if (oName.Contains("2022")) is22 = true;
    if (oName.Contains("2023")) is23 = true;
    if (oName.Contains("2024")) is24 = true;

    if (oName.Contains("2022B")) is22B = true;
    if (oName.Contains("2022C")) is22C = true;
    if (oName.Contains("2022D")) is22D = true;
    if (oName.Contains("2022E")) is22E = true;
    if (oName.Contains("2022F")) is22F = true;
    if (oName.Contains("2022G")) is22G = true;
    if (oName.Contains("PostEE")) isPostEE = true;

    if (oName.Contains("2023B")) is23B = true;
    if (oName.Contains("2023C")) is23C = true;
    if (oName.Contains("2023Cv1") || oName.Contains("2023Cv2") || oName.Contains("2023Cv3")) is23Cv123 = true;
    if (oName.Contains("2023Cv4")) is23Cv4 = true;
    if (oName.Contains("2023D")) is23D = true;
    if (oName.Contains("2023E")) is23E = true;
    if (oName.Contains("2023F")) is23F = true;
    if (oName.Contains("2023G")) is23G = true;
    if (oName.Contains("2023Pre"))  is23Pre  = true;
    if (oName.Contains("2023Post")) is23Post = true;

    if (oName.Contains("2024B")) is24B = true;
    if (oName.Contains("2024C")) is24C = true;
    if (oName.Contains("2024D")) is24D = true;
    if (oName.Contains("2024E")) is24E = true;
    if (oName.Contains("2024F")) is24F = true;
    if (oName.Contains("2024G")) is24G = true;

    if (oName.Contains("Data")) isData = true;
    if (oName.Contains("MC"))   isMC = true;

    if(oName.Contains("GamJet"))    isGamJet    = true;
    if(oName.Contains("ZeeJet"))  isZeeJet  = true;
    if(oName.Contains("ZmmJet"))   isZmmJet   = true;
    if(oName.Contains("MCTruth"))   isMCTruth   = true;
    if(oName.Contains("Flavour"))   isFlavour   = true;
    if(oName.Contains("VetoMap"))   isVetoMap   = true;
    if(oName.Contains("DiJet"))     isDiJet     = true;
    if(oName.Contains("IncJet"))    isIncJet    = true;
    if(oName.Contains("MultiJet"))  isMultiJet  = true;
    if(oName.Contains("Wqq"))       isWqq       = true;

    if(oName.Contains("QCD"))    isQCD    = true;
    if(oName.Contains("MC"))     isMG     = true;
}

void GlobalFlag::printFlag(){
    if(isDebug   )std::cout<<"isDebug   = true"<<std::endl;
    if(is22      )std::cout<<"is22      = true"<<std::endl;
    if(is23      )std::cout<<"is23      = true"<<std::endl;
    if(is24      )std::cout<<"is24      = true"<<std::endl;
    if(is22B     )std::cout<<"is22B     = true"<<std::endl;
    if(is22C     )std::cout<<"is22C     = true"<<std::endl;
    if(is22D     )std::cout<<"is22D     = true"<<std::endl;
    if(is22E     )std::cout<<"is22E     = true"<<std::endl;
    if(is22F     )std::cout<<"is22F     = true"<<std::endl;
    if(is22G     )std::cout<<"is22G     = true"<<std::endl;
    if(is23B     )std::cout<<"is23B     = true"<<std::endl;
    if(is23C     )std::cout<<"is23C     = true"<<std::endl;
    if(is23Cv123 )std::cout<<"is23Cv123 = true"<<std::endl;
    if(is23Cv4   )std::cout<<"is23Cv4   = true"<<std::endl;
    if(is23D     )std::cout<<"is23D     = true"<<std::endl;
    if(is23E     )std::cout<<"is23E     = true"<<std::endl;
    if(is23F     )std::cout<<"is23F     = true"<<std::endl;
    if(is23G     )std::cout<<"is23G     = true"<<std::endl;
    if(is23Pre   )std::cout<<"is23Pre   = true"<<std::endl;
    if(is23Post  )std::cout<<"is23Post  = true"<<std::endl;
    if(is24B     )std::cout<<"is24B     = true"<<std::endl;
    if(is24C     )std::cout<<"is24C     = true"<<std::endl;
    if(is24D     )std::cout<<"is24D     = true"<<std::endl;
    if(is24E     )std::cout<<"is24E     = true"<<std::endl;
    if(is24F     )std::cout<<"is24F     = true"<<std::endl;
    if(is24G     )std::cout<<"is24G     = true"<<std::endl;
    if(isData    )std::cout<<"isData    = true"<<std::endl;
    if(isMC      )std::cout<<"isMC      = true"<<std::endl;
    if(isGamJet  )std::cout<<"isGamJet  = true"<<std::endl;
    if(isZeeJet  )std::cout<<"isZeeJet  = true"<<std::endl;
    if(isZmmJet  )std::cout<<"isZmmJet  = true"<<std::endl;
    if(isMCTruth )std::cout<<"isMCTruth = true"<<std::endl;
    if(isFlavour)std::cout<<"isFlavour= true"<<std::endl;
    if(isIncJet  )std::cout<<"isIncJet  = true"<<std::endl;
    if(isMultiJet)std::cout<<"isMultiJet= true"<<std::endl;
    if(isWqq     )std::cout<<"isWqq     = true"<<std::endl;

    if(isQCD     )std::cout<<"isQCD     = true"<<std::endl;
    if(isMG      )std::cout<<"isMG      = true"<<std::endl;
}

GlobalFlag::~GlobalFlag(){
}
