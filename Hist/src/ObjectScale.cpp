#include"ObjectScale.h"
#include<iostream>


ObjectScale::ObjectScale(){
}

ObjectScale::~ObjectScale(){
}

void ObjectScale::applyJEC(SkimTree* tree, correction::CompoundCorrection::Ref jesRefSF, correction::Correction::Ref jesRefUnc, string systVar){
    /*
	TLorentzVector tMET;
	tMET.SetPtEtaPhiM(tree->MET_pt_,0.0,tree->MET_phi_,0.0);

	for(int jetInd = 0; jetInd < tree->nJet_ ; ++jetInd){
		if(tree->jetPt_[jetInd] < 10) continue;
		if(fabs(tree->jetEta_[jetInd]) > 5.2) continue;
        //correction = SF
        double rawPt = tree->jetPt_[jetInd] * (1.-tree->jetRawFactor_[jetInd]);
        auto corr = jesRefSF->evaluate({tree->jetArea_[jetInd], tree->jetEta_[jetInd], rawPt, tree->rho_});

		TLorentzVector tjet;
		tjet.SetPtEtaPhiM(tree->jetPt_[jetInd], tree->jetEta_[jetInd], tree->jetPhi_[jetInd], 0.0);
		tMET+=tjet;
        //Uncertanity
        auto unc=jesRefUnc->evaluate({tree->jetEta_[jetInd], tree->jetPt_[jetInd]});
		if(systVar=="down") corr-=unc;
		if(systVar=="up")   corr+=unc;
        double jes = (1.-tree->jetRawFactor_[jetInd]) * corr;
		
		tree->jetPt_[jetInd] = tree->jetPt_[jetInd] * jes; 
        tree->jetmuEF_[jetInd] = jes; //ad-hoc way of storing jes in other var for cross-check
		tjet.SetPtEtaPhiM(tree->jetPt_[jetInd], tree->jetEta_[jetInd], tree->jetPhi_[jetInd], 0.0);		       
		tMET-=tjet;
	}
	tree->MET_pt_ = tMET.Pt();
	tree->MET_phi_ = tMET.Phi();
    */

}

vector<string> ObjectScale::getL2L3Names(TString oName){
  vector<string> jNs;//jecNamess
  if (isData){
    if(oName.Contains("2022C")){
        jNs.push_back("Summer22_22Sep2023_V2_MC_L2Relative_AK4PFPuppi");
        jNs.push_back("Summer22_22Sep2023_RunCD_V2_DATA_L2L3Residual_AK4PFPuppi");
    }
    if(oName.Contains("2022D")){
        jNs.push_back("Summer22Run3_V1_MC_L2Relative_AK4PUPPI");
        jNs.push_back("Run22CD-22Sep2023_DATA_L2L3Residual_AK4PFPuppi");
    }
    if(oName.Contains("2022E")){
        jNs.push_back("Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI");
        jNs.push_back("Run22E-22Sep2023_DATA_L2L3Residual_AK4PFPuppi");
    }
    if(oName.Contains("2022F")){
        jNs.push_back("Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI");
        jNs.push_back("Run22F-Prompt_DATA_L2L3Residual_AK4PFPuppi");
    }
    if(oName.Contains("2022G")){
        jNs.push_back("Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI");
        jNs.push_back("Run22G-Prompt_DATA_L2L3Residual_AK4PFPuppi");
    }
  }
  else{
      jNs.push_back("Summer22Run3_V1_MC_L2Relative_AK4PUPPI");
  }
  cout<<"\nINFO: ObjectScale::getL2L3Names()"<<endl;
  for(string jN: jNs){ cout<<jN<<endl;}
  return jNs;
} 

string ObjectScale::getJvName(TString oName){
  string jvN;//Jet Veto Name
  if (isData){
    if(oName.Contains("2022C") || oName.Contains("2022D")){
        //jvN = "Summer22_23Sep2023_RunCD_V1";
        //jvN = "Summer22EE_23Sep2023_RunEFG_V1";
        jvN = "Winter22Run3_RunCD_V1";
    }
  }
  cout<<"\nINFO: ObjectScale::getJvName()"<<endl;
  cout<<jvN<<endl;
  return jvN;
} 

string ObjectScale::getJvKey(TString oName){
  string jvK;//Jet Veto Key
  if (isData){
    if(oName.Contains("2022") || oName.Contains("2023")){
        jvK = "jetvetomap";
    }
  }
  cout<<"\nINFO: ObjectScale::getJvKey()"<<endl;
  cout<<jvK<<endl;
  return jvK;
} 



