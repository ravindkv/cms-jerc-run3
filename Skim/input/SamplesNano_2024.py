#--------------------------
#MC
#--------------------------
# Sample key naming convention = MC_Year_Channel_SampleName
# SampleName should not contain minus (-)
def getMC_GamJet():
    samps = {}
    samps['MC_2024_GamJet_G4JetsHT100to200V14'] = '/GJ-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    samps['MC_2024_GamJet_G4JetsHT200to400V14'] = '/GJ-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    samps['MC_2024_GamJet_G4JetsHT400to600V14'] = '/GJ-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    samps['MC_2024_GamJet_G4JetsHT40to70V14']   = '/GJ-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    samps['MC_2024_GamJet_G4JetsHT600toInfV14'] = '/GJ-4Jets_HT-600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    samps['MC_2024_GamJet_G4JetsHT70to100V14']  = '/GJ-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'

    samps['MC_2024_GamJet_QCD4JetsHT40to70V14'] 	= '/QCD-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    samps['MC_2024_GamJet_QCD4JetsHT70to100V14'] 	= '/QCD-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    samps['MC_2024_GamJet_QCD4JetsHT100to200V14'] 	= '/QCD-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    samps['MC_2024_GamJet_QCD4JetsHT200to400V14'] 	= '/QCD-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    samps['MC_2024_GamJet_QCD4JetsHT400to600V14'] 	= '/QCD-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    samps['MC_2024_GamJet_QCD4JetsHT600to800V14'] 	= '/QCD-4Jets_HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    samps['MC_2024_GamJet_QCD4JetsHT800to1000V14'] 	= '/QCD-4Jets_HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM' 
    samps['MC_2024_GamJet_QCD4JetsHT1000to1200V14'] = '/QCD-4Jets_HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    samps['MC_2024_GamJet_QCD4JetsHT1200to1500V14'] = '/QCD-4Jets_HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    samps['MC_2024_GamJet_QCD4JetsHT1500to2000V14'] = '/QCD-4Jets_HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    samps['MC_2024_GamJet_QCD4JetsHT2000toInfV14'] 	= '/QCD-4Jets_HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
    return samps

def getMC_DiMuJet():
    samps = {}
    samps['MC_2024_DiMuJet_DYto2L4JetsV14']  = '/DYto2L-4Jets_MLL-50_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10_-v1/NANOAODSIM' 
    return samps

def getMC_DiEleJet():
    samps = {}
    samps['MC_2024_DiEleJet_DYto2L4JetsV14']  = '/DYto2L-4Jets_MLL-50_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM' 
    return samps

def getMC_DiJet():
	samps = {}
	samps['MC_2024_DiJet_QCD4JetsHT40to70V14'] 	= '/QCD-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT70to100V14'] 	= '/QCD-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT100to200V14'] 	= '/QCD-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT200to400V14'] 	= '/QCD-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT400to600V14'] 	= '/QCD-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT600to800V14'] 	= '/QCD-4Jets_HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT800to1000V14'] 	= '/QCD-4Jets_HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM' 
	samps['MC_2024_DiJet_QCD4JetsHT1000to1200V14'] = '/QCD-4Jets_HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT1200to1500V14'] = '/QCD-4Jets_HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT1500to2000V14'] = '/QCD-4Jets_HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT2000toInfV14'] 	= '/QCD-4Jets_HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENanoV14_133X_mcRun3_2024_realistic_v10-v1/NANOAODSIM'
	return samps

#--------------------------
# Data
#--------------------------
def getData_GamJet():
    samps = {}
    #EGamma0
    samps['Data_2024A_GamJet_EGamma0']      = '/EGamma0/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024B_GamJet_EGamma0']      = '/EGamma0/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024C_GamJet_EGamma0']      = '/EGamma0/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024D_GamJet_EGamma0']      = '/EGamma0/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024E_GamJet_EGamma0v1']    = '/EGamma0/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024E_GamJet_EGamma0v2']    = '/EGamma0/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024F_GamJet_EGamma0']      = '/EGamma0/Run2024F-PromptReco-v1/NANOAOD'
    samps['Data_2024F_GamJet_EGamma0CCDI']  = '/EGamma0/Run2024F-ECAL_CC_HCAL_DI-v1/NANOAOD'
    samps['Data_2024G_GamJet_EGamma0']      = '/EGamma0/Run2024G-PromptReco-v1/NANOAOD'
    samps['Data_2024H_GamJet_EGamma0']      = '/EGamma0/Run2024H-PromptReco-v1/NANOAOD'
    samps['Data_2024I_GamJet_EGamma0v1']    = '/EGamma0/Run2024I-PromptReco-v1/NANOAOD'
    samps['Data_2024I_GamJet_EGamma0v2']    = '/EGamma0/Run2024I-PromptReco-v2/NANOAOD'
    #EGamma1
    samps['Data_2024A_GamJet_EGamma1']      = '/EGamma1/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024B_GamJet_EGamma1']      = '/EGamma1/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024C_GamJet_EGamma1']      = '/EGamma1/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024D_GamJet_EGamma1']      = '/EGamma1/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024E_GamJet_EGamma1v1']    = '/EGamma1/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024E_GamJet_EGamma1v2']    = '/EGamma1/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024F_GamJet_EGamma1']      = '/EGamma1/Run2024F-PromptReco-v1/NANOAOD'
    samps['Data_2024F_GamJet_EGamma1CCDI']  = '/EGamma1/Run2024F-ECAL_CC_HCAL_DI-v2/NANOAOD'
    samps['Data_2024G_GamJet_EGamma1']      = '/EGamma1/Run2024G-PromptReco-v1/NANOAOD'
    samps['Data_2024H_GamJet_EGamma1']      = '/EGamma1/Run2024H-PromptReco-v1/NANOAOD'
    samps['Data_2024I_GamJet_EGamma1v1']    = '/EGamma1/Run2024I-PromptReco-v1/NANOAOD'
    samps['Data_2024I_GamJet_EGamma1v2']    = '/EGamma1/Run2024I-PromptReco-v2/NANOAOD'
    return samps

def getData_DiEleJet():
    samps = {}
    #EGamma0
    samps['Data_2024A_DiEleJet_EGamma0']      = '/EGamma0/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024B_DiEleJet_EGamma0']      = '/EGamma0/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024C_DiEleJet_EGamma0']      = '/EGamma0/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024D_DiEleJet_EGamma0']      = '/EGamma0/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024E_DiEleJet_EGamma0v1']    = '/EGamma0/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024E_DiEleJet_EGamma0v2']    = '/EGamma0/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024F_DiEleJet_EGamma0']      = '/EGamma0/Run2024F-PromptReco-v1/NANOAOD'
    samps['Data_2024G_DiEleJet_EGamma0']      = '/EGamma0/Run2024G-PromptReco-v1/NANOAOD'
    samps['Data_2024H_DiEleJet_EGamma0']      = '/EGamma0/Run2024H-PromptReco-v1/NANOAOD'
    samps['Data_2024I_DiEleJet_EGamma0v1']    = '/EGamma0/Run2024I-PromptReco-v1/NANOAOD'
    samps['Data_2024I_DiEleJet_EGamma0v2']    = '/EGamma0/Run2024I-PromptReco-v2/NANOAOD'
    #EGamma1
    samps['Data_2024A_DiEleJet_EGamma1']      = '/EGamma1/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024B_DiEleJet_EGamma1']      = '/EGamma1/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024C_DiEleJet_EGamma1']      = '/EGamma1/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024D_DiEleJet_EGamma1']      = '/EGamma1/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024E_DiEleJet_EGamma1v1']    = '/EGamma1/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024E_DiEleJet_EGamma1v2']    = '/EGamma1/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024F_DiEleJet_EGamma1']      = '/EGamma1/Run2024F-PromptReco-v1/NANOAOD'
    samps['Data_2024G_DiEleJet_EGamma1']      = '/EGamma1/Run2024G-PromptReco-v1/NANOAOD'
    samps['Data_2024H_DiEleJet_EGamma1']      = '/EGamma1/Run2024H-PromptReco-v1/NANOAOD'
    samps['Data_2024I_DiEleJet_EGamma1v1']    = '/EGamma1/Run2024I-PromptReco-v1/NANOAOD'
    samps['Data_2024I_DiEleJet_EGamma1v2']    = '/EGamma1/Run2024I-PromptReco-v2/NANOAOD'
    return samps

def getData_DiMuJet():
    samps = {}
    #Muon0
    samps['Data_2024A_DiMuJet_Muon0']      = '/Muon0/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024B_DiMuJet_Muon0']      = '/Muon0/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024C_DiMuJet_Muon0']      = '/Muon0/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024D_DiMuJet_Muon0']      = '/Muon0/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024E_DiMuJet_Muon0v1']    = '/Muon0/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024E_DiMuJet_Muon0v2']    = '/Muon0/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024F_DiMuJet_Muon0']      = '/Muon0/Run2024F-PromptReco-v1/NANOAOD'
    samps['Data_2024G_DiMuJet_Muon0']      = '/Muon0/Run2024G-PromptReco-v1/NANOAOD'
    samps['Data_2024H_DiMuJet_Muon0']      = '/Muon0/Run2024H-PromptReco-v1/NANOAOD'
    samps['Data_2024I_DiMuJet_Muon0v1']    = '/Muon0/Run2024I-PromptReco-v1/NANOAOD'
    samps['Data_2024I_DiMuJet_Muon0v2']    = '/Muon0/Run2024I-PromptReco-v2/NANOAOD'
    #Muon1
    samps['Data_2024A_DiMuJet_Muon1']      = '/Muon1/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024B_DiMuJet_Muon1']      = '/Muon1/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024C_DiMuJet_Muon1']      = '/Muon1/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024D_DiMuJet_Muon1']      = '/Muon1/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024E_DiMuJet_Muon1v1']    = '/Muon1/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024E_DiMuJet_Muon1v2']    = '/Muon1/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024F_DiMuJet_Muon1']      = '/Muon1/Run2024F-PromptReco-v1/NANOAOD'
    samps['Data_2024G_DiMuJet_Muon1']      = '/Muon1/Run2024G-PromptReco-v1/NANOAOD'
    samps['Data_2024H_DiMuJet_Muon1']      = '/Muon1/Run2024H-PromptReco-v1/NANOAOD'
    samps['Data_2024I_DiMuJet_Muon1v1']    = '/Muon1/Run2024I-PromptReco-v1/NANOAOD'
    samps['Data_2024I_DiMuJet_Muon1v2']    = '/Muon1/Run2024I-PromptReco-v2/NANOAOD'
    return samps

def getData_DiJet():
    samps = {}
    samps['Data_2024A_DiJet_ZeroBias']    = '/ZeroBias/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024B_DiJet_ZeroBias']    = '/ZeroBias/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024C_DiJet_ZeroBias']    = '/ZeroBias/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024D_DiJet_ZeroBias']    = '/ZeroBias/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024E_DiJet_ZeroBiasv1']  = '/ZeroBias/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024E_DiJet_ZeroBiasv2']  = '/ZeroBias/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024F_DiJet_ZeroBias']    = '/ZeroBias/Run2024F-PromptReco-v1/NANOAOD'
    samps['Data_2024G_DiJet_ZeroBias']    = '/ZeroBias/Run2024G-PromptReco-v1/NANOAOD'
    samps['Data_2024H_DiJet_ZeroBias']    = '/ZeroBias/Run2024H-PromptReco-v1/NANOAOD'
    samps['Data_2024I_DiJet_ZeroBiasv1']  = '/ZeroBias/Run2024I-PromptReco-v1/NANOAOD'
    samps['Data_2024I_DiJet_ZeroBiasv2']  = '/ZeroBias/Run2024I-PromptReco-v2/NANOAOD'

    #JetMET0
    samps['Data_2024A_DiJet_JetMET0']    = '/JetMET0/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024B_DiJet_JetMET0']    = '/JetMET0/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024C_DiJet_JetMET0']    = '/JetMET0/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024D_DiJet_JetMET0']    = '/JetMET0/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024E_DiJet_JetMET0v1']  = '/JetMET0/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024E_DiJet_JetMET0v2']  = '/JetMET0/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024F_DiJet_JetMET0']    = '/JetMET0/Run2024F-PromptReco-v1/NANOAOD'
    samps['Data_2024G_DiJet_JetMET0']    = '/JetMET0/Run2024G-PromptReco-v1/NANOAOD'
    samps['Data_2024H_DiJet_JetMET0']    = '/JetMET0/Run2024H-PromptReco-v1/NANOAOD'
    samps['Data_2024I_DiJet_JetMET0v1']  = '/JetMET0/Run2024I-PromptReco-v1/NANOAOD'
    samps['Data_2024I_DiJet_JetMET0v2']  = '/JetMET0/Run2024I-PromptReco-v2/NANOAOD'
   
    #JetMET1
    samps['Data_2024A_DiJet_JetMET1']    = '/JetMET1/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024B_DiJet_JetMET1']    = '/JetMET1/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024C_DiJet_JetMET1']    = '/JetMET1/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024D_DiJet_JetMET1']    = '/JetMET1/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024E_DiJet_JetMET1v1']  = '/JetMET1/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024E_DiJet_JetMET1v2']  = '/JetMET1/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024F_DiJet_JetMET1']    = '/JetMET1/Run2024F-PromptReco-v1/NANOAOD'
    samps['Data_2024G_DiJet_JetMET1']    = '/JetMET1/Run2024G-PromptReco-v1/NANOAOD'
    samps['Data_2024H_DiJet_JetMET1']    = '/JetMET1/Run2024H-PromptReco-v1/NANOAOD'
    samps['Data_2024I_DiJet_JetMET1v1']  = '/JetMET1/Run2024I-PromptReco-v1/NANOAOD'
    samps['Data_2024I_DiJet_JetMET1v2']  = '/JetMET1/Run2024I-PromptReco-v2/NANOAOD'
    return samps

def sampleDict():
    allSamples = {}
    allSamples.update(getMC_GamJet())
    #allSamples.update(getMC_DiEleJet())
    #allSamples.update(getMC_DiMuJet())
    #allSamples.update(getMC_DiJet())

    #allSamples.update(getData_GamJet())
    #allSamples.update(getData_DiEleJet())
    #allSamples.update(getData_DiMuJet())
    #allSamples.update(getData_DiJet())
    return allSamples

if __name__=="__main__":
    samp = sampleDict()
    print(samp)

