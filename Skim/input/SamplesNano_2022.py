import sys
#--------------------------
#MC
#--------------------------
# Sample key naming convention = MC_Year_Channel_Name
# keys should not contain minus (-)
def getMC_GamJet():
    samps = {}

    #2022 PreEE
    samps['MC_2022PreEE_GamJet_G4JetsHT40to70']  = '/G-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022PreEE_GamJet_G4JetsHT70to100']  = '/G-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022PreEE_GamJet_G4JetsHT100to200']  = '/G-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022PreEE_GamJet_G4JetsHT200to400']  = '/G-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022PreEE_GamJet_G4JetsHT400to600']  = '/G-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022PreEE_GamJet_G4JetsHT600toInf']  = '/G-4Jets_HT-600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'

    #2022 PostEE
    samps['MC_2022PostEE_GamJet_G4JetsHT40to70'] = '/G-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_G4JetsHT70to100'] = '/G-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_G4JetsHT100to200'] = '/G-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_G4JetsHT200to400'] = '/G-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_G4JetsHT400to600'] = '/G-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_G4JetsHT600toInf'] = '/G-4Jets_HT-600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'

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
    samps['Data_2022B_GamJet_EGamma']       = '/EGamma/Run2022B-22Sep2023-v2/NANOAOD' 
    samps['Data_2022C_GamJet_EGamma']       = '/EGamma/Run2022C-22Sep2023-v1/NANOAOD' 
    samps['Data_2022D_GamJet_EGamma']       = '/EGamma/Run2022D-22Sep2023-v1/NANOAOD'
    samps['Data_2022E_GamJet_EGamma']       = '/EGamma/Run2022E-22Sep2023-v1/NANOAOD'
    samps['Data_2022F_GamJet_EGamma']       = '/EGamma/Run2022F-22Sep2023-v1/NANOAOD'
    samps['Data_2022G_GamJet_EGamma']       = '/EGamma/Run2022G-22Sep2023-v2/NANOAOD'

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

