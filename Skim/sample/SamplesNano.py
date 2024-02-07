import sys
#--------------------------
#MC
#--------------------------
#keys should not contain minus (-)
#first  letter of the key should not be number
def getGJet(year):
    samps22 = {}
    samps22['G4Jets_HT_100to200_preEE']  = '/G-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps22['G4Jets_HT_100to200_postEE'] = '/G-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    
    samps23 = {}
    #preBpix
    samps23['G4Jets_HT_40to70_preBPix']     = '/GJ-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v1/NANOAODSIM'
    samps23['G4Jets_HT_70to100_preBPix']    = '/GJ-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
    samps23['G4Jets_HT_100to200_preBPix']   = '/GJ-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
    samps23['G4Jets_HT_200to400_preBPix']   = '/GJ-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v1/NANOAODSIM'
    samps23['G4Jets_HT_400to600_preBPix']   = '/GJ-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v1/NANOAODSIM'
    samps23['G4Jets_HT_600toInf_preBPix']   = '/GJ-4Jets_HT-600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
    samps23['GJets_HT_15to6000_preBPix']    = '/GJet_PT-15to6000_TuneCP5-Flat_13p6TeV_pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
    samps23['GJets_HT_15to6000_JME_preBPix']= '/GJet_PT-15to6000_TuneCP5-Flat_13p6TeV_pythia8/Run3Summer23NanoAODv12-JMENano12p5_132X_mcRun3_2023_realistic_v5-v1/NANOAODSIM'
    #postBpix
    samps23['G4Jets_HT_40to70_postBPix']     = '/GJ-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps23['G4Jets_HT_70to100_postBPix']    = '/GJ-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps23['G4Jets_HT_100to200_postBPix']   = '/GJ-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    samps23['G4Jets_HT_200to400_postBPix']   = '/GJ-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps23['G4Jets_HT_400to600_postBPix']   = '/GJ-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    samps23['G4Jets_HT_600toInf_postBPix']   = '/GJ-4Jets_HT-600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps23['GJets_HT_15to6000_postBPix']    = '/GJet_PT-15to6000_TuneCP5-Flat_13p6TeV_pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v2/NANOAODSIM'
    samps23['GJets_HT_15to6000_JME_postBPix']= '/GJet_PT-15to6000_TuneCP5-Flat_13p6TeV_pythia8/Run3Summer23BPixNanoAODv12-JMENano12p5_132X_mcRun3_2023_realistic_postBPix_v4-v1/NANOAODSIM'
    
    samps = {}
    samps['2022']        = samps22
    samps['2023']        = samps23
    return samps[year]

#--------------------------
# Data
#--------------------------
def getData(year):
    samps22 = {}
    samps22['Data_2022C']       = '/EGamma/Run2022C-22Sep2023-v1/NANOAOD' 
    samps22['Data_2022D']       = '/EGamma/Run2022D-22Sep2023-v1/NANOAOD'
    samps22['Data_2022E']       = '/EGamma/Run2022E-22Sep2023-v1/NANOAOD'
    samps22['Data_2022F']       = '/EGamma/Run2022F-22Sep2023-v1/NANOAOD'
    samps22['Data_2022G']       = '/EGamma/Run2022G-PromptNanoAODv10_v1-v1/NANOAOD'

    samps23 = {}
    #EGamma0
    samps23['Data_2023B0v1']    = '/EGamma0/Run2023B-PromptNanoAODv11p9_v1-v1/NANOAOD'
    samps23['Data_2023C0v1']    = '/EGamma0/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps23['Data_2023C0v2']    = '/EGamma0/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps23['Data_2023C0v3']    = '/EGamma0/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps23['Data_2023C0v4']    = '/EGamma0/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps23['Data_2022D0v1']    = '/EGamma0/Run2023D-22Sep2023_v1-v1/NANOAOD'
    #EGamma1
    samps23['Data_2023B1v1']    = '/EGamma1/Run2023B-PromptNanoAODv11p9_v1-v1/NANOAOD'
    samps23['Data_2023C1v1']    = '/EGamma1/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps23['Data_2023C1v2']    = '/EGamma1/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps23['Data_2023C1v3']    = '/EGamma1/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps23['Data_2023C1v4']    = '/EGamma1/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps23['Data_2022D1v1']    = '/EGamma1/Run2023D-22Sep2023_v1-v1/NANOAOD'
    
    samps = {}
    samps['2022']        = samps22
    samps['2023']        = samps23
    return samps[year]

def getMC(year):
    samps = {}
    samps.update(getGJet(year))
    return samps

def sampleDict(year):
    allSamples = {}
    #allSamples.update(getGJet(year))
    allSamples.update(getData(year))
    return allSamples

if __name__=="__main__":
    samp = sampleDict('2022')
    #samp = sampleDict('2016Post')
    #samp = sampleDict('2017')
    #samp = sampleDict('2018')
    print(samp)

