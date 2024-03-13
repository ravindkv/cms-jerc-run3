import sys
#--------------------------
#MC
#--------------------------
# Sample key naming convention = Channel__Year__Name
# keys should not contain minus (-)
def getMC_GamJet():
    samps = {}

    #2022 PreEE
    samps['GamJet__2022PreEE__G4Jets_HT_100to200']  = '/G-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'

    #2022 PostEE
    samps['GamJet__2022PostEE__G4Jets_HT_100to200'] = '/G-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'

    #2023 PreBPix
    samps['GamJet__2023PreBPix__G4Jets_HT_40to70']     = '/GJ-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v1/NANOAODSIM'
    samps['GamJet__2023PreBPix__G4Jets_HT_70to100']    = '/GJ-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
    samps['GamJet__2023PreBPix__G4Jets_HT_100to200']   = '/GJ-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
    samps['GamJet__2023PreBPix__G4Jets_HT_200to400']   = '/GJ-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v1/NANOAODSIM'
    samps['GamJet__2023PreBPix__G4Jets_HT_400to600']   = '/GJ-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v1/NANOAODSIM'
    samps['GamJet__2023PreBPix__G4Jets_HT_600toInf']   = '/GJ-4Jets_HT-600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
    samps['GamJet__2023PreBPix__GJets_HT_15to6000']    = '/GJet_PT-15to6000_TuneCP5-Flat_13p6TeV_pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
    samps['GamJet__2023PreBPix__GJets_HT_15to6000_JME']= '/GJet_PT-15to6000_TuneCP5-Flat_13p6TeV_pythia8/Run3Summer23NanoAODv12-JMENano12p5_132X_mcRun3_2023_realistic_v5-v1/NANOAODSIM'

    #2023 PostBPix
    samps['GamJet__2023PostBPix__G4Jets_HT_40to70']     = '/GJ-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps['GamJet__2023PostBPix__G4Jets_HT_70to100']    = '/GJ-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps['GamJet__2023PostBPix__G4Jets_HT_100to200']   = '/GJ-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    samps['GamJet__2023PostBPix__G4Jets_HT_200to400']   = '/GJ-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps['GamJet__2023PostBPix__G4Jets_HT_400to600']   = '/GJ-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    samps['GamJet__2023PostBPix__G4Jets_HT_600toInf']   = '/GJ-4Jets_HT-600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps['GamJet__2023PostBPix__GJets_HT_15to6000']    = '/GJet_PT-15to6000_TuneCP5-Flat_13p6TeV_pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v2/NANOAODSIM'
    samps['GamJet__2023PostBPix__GJets_HT_15to6000_JME']= '/GJet_PT-15to6000_TuneCP5-Flat_13p6TeV_pythia8/Run3Summer23BPixNanoAODv12-JMENano12p5_132X_mcRun3_2023_realistic_postBPix_v4-v1/NANOAODSIM'
    return samps

def getMC_DiMuJet():
    samps = {}
    return samps

def getMC_DiEleJet():
    samps = {}
    return samps

def getMC_DiJet():
    samps = {}
    return samps

#--------------------------
# Data
#--------------------------
def getData_GamJet():
    samps = {}

    #2022
    samps['GamJet__2022C__Data_EGamma']       = '/EGamma/Run2022C-22Sep2023-v1/NANOAOD' 
    samps['GamJet__2022D__Data_EGamma']       = '/EGamma/Run2022D-22Sep2023-v1/NANOAOD'
    samps['GamJet__2022E__Data_EGamma']       = '/EGamma/Run2022E-22Sep2023-v1/NANOAOD'
    samps['GamJet__2022F__Data_EGamma']       = '/EGamma/Run2022F-22Sep2023-v1/NANOAOD'
    samps['GamJet__2022G__Data_EGamma']       = '/EGamma/Run2022G-PromptNanoAODv10_v1-v1/NANOAOD'

    #2023: EGamma0
    samps['GamJet__2023B1__Data_EGamma0']    = '/EGamma0/Run2023B-PromptNanoAODv11p9_v1-v1/NANOAOD'
    samps['GamJet__2023C1__Data_EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['GamJet__2023C2__Data_EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['GamJet__2023C3__Data_EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['GamJet__2023C4__Data_EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps['GamJet__2023D1__Data_EGamma0']    = '/EGamma0/Run2023D-22Sep2023_v1-v1/NANOAOD'
   
    #2023: EGamma1
    samps['GamJet__2023B1__Data_EGamma1']    = '/EGamma1/Run2023B-PromptNanoAODv11p9_v1-v1/NANOAOD'
    samps['GamJet__2023C1__Data_EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['GamJet__2023C2__Data_EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['GamJet__2023C3__Data_EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['GamJet__2023C4__Data_EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps['GamJet__2023D1__Data_EGamma1']    = '/EGamma1/Run2023D-22Sep2023_v1-v1/NANOAOD'
    
    return samps

def getData_DiMuJet():
    samps = {}
    return samps

def getData_DiEleJet():
    samps = {}
    return samps

def getData_DiJet():
    samps = {}
    return samps

def sampleDict():
    allSamples = {}
    allSamples.update(getMC_GamJet())
    allSamples.update(getData_GamJet())
    return allSamples

if __name__=="__main__":
    samp = sampleDict()
    print(samp)

