#--------------------------
#MC
#--------------------------
# Sample key naming convention = MC_Year_Channel_SampleName
# SampleName should not contain minus (-)
def getMC_GamJet():
    samps = {}
    samps['MC_2024_GamJet_G4JetsHT40to70']   = '/GJ-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v2/NANOAODSIM'
    samps['MC_2024_GamJet_G4JetsHT70to100']  = '/GJ-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v4/NANOAODSIM'
    samps['MC_2024_GamJet_G4JetsHT100to200'] = '/GJ-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v2/NANOAODSIM'
    samps['MC_2024_GamJet_G4JetsHT200to400'] = '/GJ-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v3/NANOAODSIM'
    samps['MC_2024_GamJet_G4JetsHT400to600'] = '/GJ-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v3/NANOAODSIM'
    samps['MC_2024_GamJet_G4JetsHT600toInf'] = '/GJ-4Jets_HT-600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v2/NANOAODSIM'
    return samps

def getMC_DiMuJet():
    samps = {}
    samps['MC_2024_DiMuJet_DYto2L4Jets']  = '/DYto2L-4Jets_MLL-50_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10_ext1-v2/NANOAODSIM' 
    return samps

def getMC_DiEleJet():
    samps = {}
    samps['MC_2024_DiEleJet_DYto2L4Jets']  = '/DYto2L-4Jets_MLL-50_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10_ext1-v2/NANOAODSIM' 
    return samps

def getMC_DiJet():
	samps = {}
	samps['MC_2024_DiJet_QCD4JetsHT40to70'] 	= '/QCD-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v2/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT70to100'] 	= '/QCD-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v2/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT100to200'] 	= '/QCD-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v2/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT200to400'] 	= '/QCD-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v2/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT400to600'] 	= '/QCD-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v2/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT600to800'] 	= '/QCD-4Jets_HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v2/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT800to1000'] 	= '/QCD-4Jets_HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v2/NANOAODSIM' 
	samps['MC_2024_DiJet_QCD4JetsHT1000to1200'] = '/QCD-4Jets_HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v2/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT1200to1500'] = '/QCD-4Jets_HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v2/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT1500to2000'] = '/QCD-4Jets_HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v2/NANOAODSIM'
	samps['MC_2024_DiJet_QCD4JetsHT2000toInf'] 	= '/QCD-4Jets_HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Winter24NanoAOD-JMENano_133X_mcRun3_2024_realistic_v10-v2/NANOAODSIM'
	return samps

#--------------------------
# Data
#--------------------------
def getData_GamJet():
    samps = {}
    #EGamma0
    samps['Data_2024Av1_GamJet_EGamma0']    = '/EGamma0/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024Bv1_GamJet_EGamma0']    = '/EGamma0/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024Cv1_GamJet_EGamma0']    = '/EGamma0/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024Dv1_GamJet_EGamma0']    = '/EGamma0/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev1_GamJet_EGamma0']    = '/EGamma0/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev2_GamJet_EGamma0']    = '/EGamma0/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024Fv1_GamJet_EGamma0']    = '/EGamma0/Run2024F-PromptReco-v1/NANOAOD'
    #EGamma1
    samps['Data_2024Av1_GamJet_EGamma1']    = '/EGamma1/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024Bv1_GamJet_EGamma1']    = '/EGamma1/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024Cv1_GamJet_EGamma1']    = '/EGamma1/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024Dv1_GamJet_EGamma1']    = '/EGamma1/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev1_GamJet_EGamma1']    = '/EGamma1/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev2_GamJet_EGamma1']    = '/EGamma1/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024Fv1_GamJet_EGamma1']    = '/EGamma1/Run2024F-PromptReco-v1/NANOAOD'
    return samps

def getData_DiEleJet():
    samps = {}
    #EGamma0
    samps['Data_2024Av1_DiEleJet_EGamma0']    = '/EGamma0/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024Bv1_DiEleJet_EGamma0']    = '/EGamma0/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024Cv1_DiEleJet_EGamma0']    = '/EGamma0/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024Dv1_DiEleJet_EGamma0']    = '/EGamma0/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev1_DiEleJet_EGamma0']    = '/EGamma0/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev2_DiEleJet_EGamma0']    = '/EGamma0/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024Fv1_DiEleJet_EGamma0']    = '/EGamma0/Run2024F-PromptReco-v1/NANOAOD'
    #EGamma1
    samps['Data_2024Av1_DiEleJet_EGamma1']    = '/EGamma1/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024Bv1_DiEleJet_EGamma1']    = '/EGamma1/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024Cv1_DiEleJet_EGamma1']    = '/EGamma1/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024Dv1_DiEleJet_EGamma1']    = '/EGamma1/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev1_DiEleJet_EGamma1']    = '/EGamma1/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev2_DiEleJet_EGamma1']    = '/EGamma1/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024Fv1_DiEleJet_EGamma1']    = '/EGamma1/Run2024F-PromptReco-v1/NANOAOD'
    return samps

def getData_DiMuJet():
    samps = {}
    #Muon0
    samps['Data_2024Av1_DiMuJet_Muon0']    = '/Muon0/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024Bv1_DiMuJet_Muon0']    = '/Muon0/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024Cv1_DiMuJet_Muon0']    = '/Muon0/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024Dv1_DiMuJet_Muon0']    = '/Muon0/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev1_DiMuJet_Muon0']    = '/Muon0/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev2_DiMuJet_Muon0']    = '/Muon0/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024Fv1_DiMuJet_Muon0']    = '/Muon0/Run2024F-PromptReco-v1/NANOAOD'
   
    #Muon1
    samps['Data_2024Av1_DiMuJet_Muon1']    = '/Muon1/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024Bv1_DiMuJet_Muon1']    = '/Muon1/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024Cv1_DiMuJet_Muon1']    = '/Muon1/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024Dv1_DiMuJet_Muon1']    = '/Muon1/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev1_DiMuJet_Muon1']    = '/Muon1/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev2_DiMuJet_Muon1']    = '/Muon1/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024Fv1_DiMuJet_Muon1']    = '/Muon1/Run2024F-PromptReco-v1/NANOAOD'
    return samps

def getData_DiJet():
    samps = {}
    #JetMET0
    samps['Data_2024Av1_DiJet_JetMET0']    = '/JetMET0/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024Bv1_DiJet_JetMET0']    = '/JetMET0/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024Cv1_DiJet_JetMET0']    = '/JetMET0/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024Dv1_DiJet_JetMET0']    = '/JetMET0/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev1_DiJet_JetMET0']    = '/JetMET0/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev2_DiJet_JetMET0']    = '/JetMET0/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024Fv1_DiJet_JetMET0']    = '/JetMET0/Run2024F-PromptReco-v1/NANOAOD'
   
    #JetMET1
    samps['Data_2024Av1_DiJet_JetMET1']    = '/JetMET1/Run2024A-PromptReco-v1/NANOAOD'
    samps['Data_2024Bv1_DiJet_JetMET1']    = '/JetMET1/Run2024B-PromptReco-v1/NANOAOD'
    samps['Data_2024Cv1_DiJet_JetMET1']    = '/JetMET1/Run2024C-PromptReco-v1/NANOAOD'
    samps['Data_2024Dv1_DiJet_JetMET1']    = '/JetMET1/Run2024D-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev1_DiJet_JetMET1']    = '/JetMET1/Run2024E-PromptReco-v1/NANOAOD'
    samps['Data_2024Ev2_DiJet_JetMET1']    = '/JetMET1/Run2024E-PromptReco-v2/NANOAOD'
    samps['Data_2024Fv1_DiJet_JetMET1']    = '/JetMET1/Run2024F-PromptReco-v1/NANOAOD'
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

