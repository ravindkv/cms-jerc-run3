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

    samps['MC_2022PreEE_GamJet_QCD4JetsHT40to70'] 	= '/QCD-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022PreEE_GamJet_QCD4JetsHT70to100'] 	= '/QCD-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022PreEE_GamJet_QCD4JetsHT100to200'] 	= '/QCD-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022PreEE_GamJet_QCD4JetsHT200to400'] 	= '/QCD-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022PreEE_GamJet_QCD4JetsHT400to600'] 	= '/QCD-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022PreEE_GamJet_QCD4JetsHT600to800'] 	= '/QCD-4Jets_HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022PreEE_GamJet_QCD4JetsHT800to1000'] 	= '/QCD-4Jets_HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM' 
    samps['MC_2022PreEE_GamJet_QCD4JetsHT1000to1200'] = '/QCD-4Jets_HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022PreEE_GamJet_QCD4JetsHT1200to1500'] = '/QCD-4Jets_HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022PreEE_GamJet_QCD4JetsHT1500to2000'] = '/QCD-4Jets_HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    samps['MC_2022PreEE_GamJet_QCD4JetsHT2000toInf'] 	= '/QCD-4Jets_HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM'
    
    #For Fikri
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG100to200HT1000']   = "/GJ-4Jets_dRGJ-0p25_PTG-100to200_HT-1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG100to200HT200to400']	= "/GJ-4Jets_dRGJ-0p25_PTG-100to200_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG100to200HT400to600']   = "/GJ-4Jets_dRGJ-0p25_PTG-100to200_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG100to200HT40to200']  = "/GJ-4Jets_dRGJ-0p25_PTG-100to200_HT-40to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG100to200HT600to100']   = "/GJ-4Jets_dRGJ-0p25_PTG-100to200_HT-600to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG10to100HT1000toInf']   = "/GJ-4Jets_dRGJ-0p25_PTG-10to100_HT-1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG10to100HT100to200']  = "/GJ-4Jets_dRGJ-0p25_PTG-10to100_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG10to100HT200to400'] = "/GJ-4Jets_dRGJ-0p25_PTG-10to100_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG10to100HT400to600'] = "/GJ-4Jets_dRGJ-0p25_PTG-10to100_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG10to100HT40to100'] = "/GJ-4Jets_dRGJ-0p25_PTG-10to100_HT-40to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG10to100HT600to1000']  = "/GJ-4Jets_dRGJ-0p25_PTG-10to100_HT-600to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG200HT1000toInf']  = "/GJ-4Jets_dRGJ-0p25_PTG-200_HT-1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG200HT400to600']  = "/GJ-4Jets_dRGJ-0p25_PTG-200_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG200HT40to400']  = "/GJ-4Jets_dRGJ-0p25_PTG-200_HT-40to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"
    samps['MC_2022PreEE_GamJet_GJ4JetsPTG200HT600to1000']  = "/GJ-4Jets_dRGJ-0p25_PTG-200_HT-600to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22NanoAODv12-130X_mcRun3_2022_realistic_v5-v2/NANOAODSIM"


    #2022 PostEE
    samps['MC_2022PostEE_GamJet_G4JetsHT40to70'] = '/G-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_G4JetsHT70to100'] = '/G-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_G4JetsHT100to200'] = '/G-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_G4JetsHT200to400'] = '/G-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_G4JetsHT400to600'] = '/G-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_G4JetsHT600toInf'] = '/G-4Jets_HT-600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'

    samps['MC_2022PostEE_GamJet_QCD4JetsHT40to70'] 	= '/QCD-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v5/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_QCD4JetsHT70to100'] 	= '/QCD-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_QCD4JetsHT100to200'] 	= '/QCD-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_QCD4JetsHT200to400'] 	= '/QCD-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_QCD4JetsHT400to600'] 	= '/QCD-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_QCD4JetsHT600to800'] 	= '/QCD-4Jets_HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_QCD4JetsHT800to1000'] 	= '/QCD-4Jets_HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM' 
    samps['MC_2022PostEE_GamJet_QCD4JetsHT1000to1200'] = '/QCD-4Jets_HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_QCD4JetsHT1200to1500'] = '/QCD-4Jets_HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_QCD4JetsHT1500to2000'] = '/QCD-4Jets_HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'
    samps['MC_2022PostEE_GamJet_QCD4JetsHT2000toInf'] 	= '/QCD-4Jets_HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer22EENanoAODv12-130X_mcRun3_2022_realistic_postEE_v6-v2/NANOAODSIM'

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
    samps['Data_2022B_GamJet_EGamma']       = '/EGamma/Run2022B-22Sep2023-v2/NANOAOD' 
    samps['Data_2022C_GamJet_EGamma']       = '/EGamma/Run2022C-22Sep2023-v1/NANOAOD' 
    samps['Data_2022D_GamJet_EGamma']       = '/EGamma/Run2022D-22Sep2023-v1/NANOAOD'
    samps['Data_2022E_GamJet_EGamma']       = '/EGamma/Run2022E-22Sep2023-v1/NANOAOD'
    samps['Data_2022F_GamJet_EGamma']       = '/EGamma/Run2022F-22Sep2023-v1/NANOAOD'
    samps['Data_2022G_GamJet_EGamma']       = '/EGamma/Run2022G-22Sep2023-v2/NANOAOD'
    return samps

def getData_DiEleJet():
    samps = {}
    samps['Data_2022B_DiEleJet_EGamma']       = '/EGamma/Run2022B-22Sep2023-v2/NANOAOD' 
    samps['Data_2022C_DiEleJet_EGamma']       = '/EGamma/Run2022C-22Sep2023-v1/NANOAOD' 
    samps['Data_2022D_DiEleJet_EGamma']       = '/EGamma/Run2022D-22Sep2023-v1/NANOAOD'
    samps['Data_2022E_DiEleJet_EGamma']       = '/EGamma/Run2022E-22Sep2023-v1/NANOAOD'
    samps['Data_2022F_DiEleJet_EGamma']       = '/EGamma/Run2022F-22Sep2023-v1/NANOAOD'
    samps['Data_2022G_DiEleJet_EGamma']       = '/EGamma/Run2022G-22Sep2023-v2/NANOAOD'
    return samps

def getData_DiMuJet():
    samps = {}
    samps['Data_2022B_DiMuJet_MuonEG']     = '/MuonEG/Run2022B-22Sep2023-v1/NANOAOD' 
    samps['Data_2022C_DiMuJet_Muon']       = '/Muon/Run2022C-22Sep2023-v1/NANOAOD' 
    samps['Data_2022D_DiMuJet_Muon']       = '/Muon/Run2022D-22Sep2023-v1/NANOAOD'
    samps['Data_2022E_DiMuJet_Muon']       = '/Muon/Run2022E-22Sep2023-v1/NANOAOD'
    samps['Data_2022F_DiMuJet_Muon']       = '/Muon/Run2022F-22Sep2023-v2/NANOAOD'
    samps['Data_2022G_DiMuJet_Muon']       = '/Muon/Run2022G-22Sep2023-v1/NANOAOD'
    return samps

def getData_DiJet():
    samps = {}
    samps['Data_2022B_DiJet_ZeroBias']        = '/ZeroBias/Run2022B-22Sep2023-v1/NANOAOD'
    samps['Data_2022C_DiJet_ZeroBias']       = '/ZeroBias/Run2022C-22Sep2023-v1/NANOAOD' 
    samps['Data_2022D_DiJet_ZeroBias']       = '/ZeroBias/Run2022D-22Sep2023-v1/NANOAOD'
    samps['Data_2022E_DiJet_ZeroBias']       = '/ZeroBias/Run2022E-22Sep2023-v1/NANOAOD'
    samps['Data_2022F_DiJet_ZeroBias']       = '/ZeroBias/Run2022F-22Sep2023-v1/NANOAOD'
    samps['Data_2022G_DiJet_ZeroBias']       = '/ZeroBias/Run2022G-22Sep2023-v1/NANOAOD'

    samps['Data_2022B_DiJet_JetHT']        = '/JetHT/Run2022B-22Sep2023-v1/NANOAOD'
    samps['Data_2022C_DiJet_JetMET']       = '/JetMET/Run2022C-22Sep2023-v1/NANOAOD' 
    samps['Data_2022D_DiJet_JetMET']       = '/JetMET/Run2022D-22Sep2023-v1/NANOAOD'
    samps['Data_2022E_DiJet_JetMET']       = '/JetMET/Run2022E-22Sep2023-v1/NANOAOD'
    samps['Data_2022F_DiJet_JetMET']       = '/JetMET/Run2022F-22Sep2023-v2/NANOAOD'
    samps['Data_2022G_DiJet_JetMET']       = '/JetMET/Run2022G-22Sep2023-v2/NANOAOD'
    return samps

def sampleDict():
    allSamples = {}
    allSamples.update(getMC_GamJet())
    allSamples.update(getMC_DiEleJet())
    allSamples.update(getMC_DiMuJet())
    allSamples.update(getMC_DiJet())

    allSamples.update(getData_GamJet())
    allSamples.update(getData_DiEleJet())
    allSamples.update(getData_DiMuJet())
    allSamples.update(getData_DiJet())
    return allSamples

if __name__=="__main__":
    samp = sampleDict()
    print(samp)

