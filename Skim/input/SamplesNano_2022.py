import sys
#--------------------------
#MC
#--------------------------
# Sample key naming convention = MC__Year__Channel__Name
# keys should not contain minus (-)
def getMC_GamJet():
    samps = {}

    #2022 PreEE
    samps['MC__2022PreEE__GamJet__G4Jets_HT_100to200']  = '/G-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'

    #2022 PostEE
    samps['MC__2022PostEE__GamJet__G4Jets_HT_100to200'] = '/G-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'

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
    samps['Data__2022C__GamJet__EGamma']       = '/EGamma/Run2022C-22Sep2023-v1/NANOAOD' 
    samps['Data__2022D__GamJet__EGamma']       = '/EGamma/Run2022D-22Sep2023-v1/NANOAOD'
    samps['Data__2022E__GamJet__EGamma']       = '/EGamma/Run2022E-22Sep2023-v1/NANOAOD'
    samps['Data__2022F__GamJet__EGamma']       = '/EGamma/Run2022F-22Sep2023-v1/NANOAOD'
    samps['Data__2022G__GamJet__EGamma']       = '/EGamma/Run2022G-PromptNanoAODv10_v1-v1/NANOAOD'

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

