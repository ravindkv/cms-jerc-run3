import sys
#--------------------------
#MC
#--------------------------
# Sample key naming convention = MC__Year__Channel__Name
# keys should not contain minus (-)
def getMC_GamJet():
    samps = {}
    #PreBPix
    samps['MC__2023PreBPix__GamJet__G4Jets_HT_40to70']     = '/GJ-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v1/NANOAODSIM'
    samps['MC__2023PreBPix__GamJet__G4Jets_HT_70to100']    = '/GJ-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
    samps['MC__2023PreBPix__GamJet__G4Jets_HT_100to200']   = '/GJ-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
    samps['MC__2023PreBPix__GamJet__G4Jets_HT_200to400']   = '/GJ-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v1/NANOAODSIM'
    samps['MC__2023PreBPix__GamJet__G4Jets_HT_400to600']   = '/GJ-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v1/NANOAODSIM'
    samps['MC__2023PreBPix__GamJet__G4Jets_HT_600toInf']   = '/GJ-4Jets_HT-600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
    samps['MC__2023PreBPix__GamJet__GJets_HT_15to6000']    = '/GJet_PT-15to6000_TuneCP5-Flat_13p6TeV_pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
    samps['MC__2023PreBPix__GamJet__GJets_HT_15to6000_JME']= '/GJet_PT-15to6000_TuneCP5-Flat_13p6TeV_pythia8/Run3Summer23NanoAODv12-JMENano12p5_132X_mcRun3_2023_realistic_v5-v1/NANOAODSIM'

    #PostBPix
    samps['MC__2023PostBPix__GamJet__G4Jets_HT_40to70']     = '/GJ-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps['MC__2023PostBPix__GamJet__G4Jets_HT_70to100']    = '/GJ-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps['MC__2023PostBPix__GamJet__G4Jets_HT_100to200']   = '/GJ-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    samps['MC__2023PostBPix__GamJet__G4Jets_HT_200to400']   = '/GJ-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps['MC__2023PostBPix__GamJet__G4Jets_HT_400to600']   = '/GJ-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    samps['MC__2023PostBPix__GamJet__G4Jets_HT_600toInf']   = '/GJ-4Jets_HT-600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps['MC__2023PostBPix__GamJet__GJets_HT_15to6000']    = '/GJet_PT-15to6000_TuneCP5-Flat_13p6TeV_pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v2/NANOAODSIM'
    samps['MC__2023PostBPix__GamJet__GJets_HT_15to6000_JME']= '/GJet_PT-15to6000_TuneCP5-Flat_13p6TeV_pythia8/Run3Summer23BPixNanoAODv12-JMENano12p5_132X_mcRun3_2023_realistic_postBPix_v4-v1/NANOAODSIM'
    return samps

def getMC_DiMuJet():
    samps = {}
    return samps

def getMC_DiEleJet():
    samps = {}
    return samps

def getMC_DiJet():
	samps = {}
    #PreBPix
	samps['MC__2023PreBPix__DiJet__QCD4Jets_HT_40to70'] 	= '/QCD-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
	samps['MC__2023PreBPix__DiJet__QCD4Jets_HT_70to100'] 	= '/QCD-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
	samps['MC__2023PreBPix__DiJet__QCD4Jets_HT_100to200'] 	= '/QCD-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
	samps['MC__2023PreBPix__DiJet__QCD4Jets_HT_200to400'] 	= '/QCD-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
	samps['MC__2023PreBPix__DiJet__QCD4Jets_HT_400to600'] 	= '/QCD-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
	samps['MC__2023PreBPix__DiJet__QCD4Jets_HT_600to800'] 	= '/QCD-4Jets_HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
	samps['MC__2023PreBPix__DiJet__QCD4Jets_HT_800to1000'] 	= '/QCD-4Jets_HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM' 
	samps['MC__2023PreBPix__DiJet__QCD4Jets_HT_1000to1200'] = '/QCD-4Jets_HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
	samps['MC__2023PreBPix__DiJet__QCD4Jets_HT_1200to1500'] = '/QCD-4Jets_HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
	samps['MC__2023PreBPix__DiJet__QCD4Jets_HT_1500to2000'] = '/QCD-4Jets_HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
	samps['MC__2023PreBPix__DiJet__QCD4Jets_HT_2000toInf'] 	= '/QCD-4Jets_HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
    
	#PostBPix
	samps['MC__2023PostBPix__DiJet__QCD4Jets_HT_40to70'] 	= '/QCD-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
	samps['MC__2023PostBPix__DiJet__QCD4Jets_HT_70to100'] 	= '/QCD-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
	samps['MC__2023PostBPix__DiJet__QCD4Jets_HT_100to200'] 	= '/QCD-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
	samps['MC__2023PostBPix__DiJet__QCD4Jets_HT_200to400'] 	= '/QCD-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
	samps['MC__2023PostBPix__DiJet__QCD4Jets_HT_400to600'] 	= '/QCD-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
	samps['MC__2023PostBPix__DiJet__QCD4Jets_HT_600to800'] 	= '/QCD-4Jets_HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
	samps['MC__2023PostBPix__DiJet__QCD4Jets_HT_800to1000'] = '/QCD-4Jets_HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM' 
	samps['MC__2023PostBPix__DiJet__QCD4Jets_HT_1000to1200']= '/QCD-4Jets_HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
	samps['MC__2023PostBPix__DiJet__QCD4Jets_HT_1200to1500']= '/QCD-4Jets_HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
	samps['MC__2023PostBPix__DiJet__QCD4Jets_HT_1500to2000']= '/QCD-4Jets_HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
	samps['MC__2023PostBPix__DiJet__QCD4Jets_HT_2000toInf'] = '/QCD-4Jets_HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
	return samps

#--------------------------
# Data
#--------------------------
def getData_GamJet():
    samps = {}
    #EGamma0
    samps['Data__2023Bv1__GamJet__EGamma0']    = '/EGamma0/Run2023B-PromptNanoAODv11p9_v1-v1/NANOAOD'
    samps['Data__2023Cv1__GamJet__EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['Data__2023Cv2__GamJet__EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['Data__2023Cv3__GamJet__EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['Data__2023Cv4__GamJet__EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps['Data__2023Dv1__GamJet__EGamma0']    = '/EGamma0/Run2023D-22Sep2023_v1-v1/NANOAOD'
   
    #EGamma1
    samps['Data__2023Bv1__GamJet__EGamma1']    = '/EGamma1/Run2023B-PromptNanoAODv11p9_v1-v1/NANOAOD'
    samps['Data__2023Cv1__GamJet__EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['Data__2023Cv2__GamJet__EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['Data__2023Cv3__GamJet__EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['Data__2023Cv4__GamJet__EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps['Data__2023Dv1__GamJet__EGamma1']    = '/EGamma1/Run2023D-22Sep2023_v1-v1/NANOAOD'
    return samps

def getData_DiMuJet():
    samps = {}
    return samps

def getData_DiEleJet():
    samps = {}
    return samps

def getData_DiJet():
    samps = {}
    #JetMET0
    samps['Data__2023Bv1__DiJet__JetMET0']    = '/JetMET0/Run2023B-PromptNanoAODv11p9_v1-v1/NANOAOD'
    samps['Data__2023Cv1__DiJet__JetMET0']    = '/JetMET0/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['Data__2023Cv2__DiJet__JetMET0']    = '/JetMET0/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['Data__2023Cv3__DiJet__JetMET0']    = '/JetMET0/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['Data__2023Cv4__DiJet__JetMET0']    = '/JetMET0/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps['Data__2023Dv1__DiJet__JetMET0']    = '/JetMET0/Run2023D-22Sep2023_v1-v1/NANOAOD'
   
    #JetMET1
    samps['Data__2023Bv1__DiJet__JetMET1']    = '/JetMET1/Run2023B-PromptNanoAODv11p9_v1-v1/NANOAOD'
    samps['Data__2023Cv1__DiJet__JetMET1']    = '/JetMET1/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['Data__2023Cv2__DiJet__JetMET1']    = '/JetMET1/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['Data__2023Cv3__DiJet__JetMET1']    = '/JetMET1/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['Data__2023Cv4__DiJet__JetMET1']    = '/JetMET1/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps['Data__2023Dv1__DiJet__JetMET1']    = '/JetMET1/Run2023D-22Sep2023_v1-v1/NANOAOD'
    return samps

def sampleDict():
    allSamples = {}
    allSamples.update(getMC_GamJet())
    allSamples.update(getMC_DiJet())

    allSamples.update(getData_GamJet())
    allSamples.update(getData_DiJet())
    return allSamples

if __name__=="__main__":
    samp = sampleDict()
    print(samp)

