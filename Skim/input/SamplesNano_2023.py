#--------------------------
#MC
#--------------------------
# Sample key naming convention = MC_Year_Channel_SampleName
# SampleName should not contain _ or -
def getMC_GamJet():
    samps = {}
    #PreBPix
    samps['MC_2023PreBPix_GamJet_G4JetsHT40to70']     = '/GJ-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v1/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_G4JetsHT70to100']    = '/GJ-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_G4JetsHT100to200']   = '/GJ-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_G4JetsHT200to400']   = '/GJ-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v1/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_G4JetsHT400to600']   = '/GJ-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v1/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_G4JetsHT600toInf']   = '/GJ-4Jets_HT-600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_GJetsHT15to6000']    = '/GJet_PT-15to6000_TuneCP5-Flat_13p6TeV_pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'

    samps['MC_2023PreBPix_GamJet_QCD4JetsHT40to70'] 	= '/QCD-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_QCD4JetsHT70to100'] 	= '/QCD-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_QCD4JetsHT100to200'] 	= '/QCD-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_QCD4JetsHT200to400'] 	= '/QCD-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_QCD4JetsHT400to600'] 	= '/QCD-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_QCD4JetsHT600to800'] 	= '/QCD-4Jets_HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_QCD4JetsHT800to1000'] 	= '/QCD-4Jets_HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM' 
    samps['MC_2023PreBPix_GamJet_QCD4JetsHT1000to1200'] = '/QCD-4Jets_HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_QCD4JetsHT1200to1500'] = '/QCD-4Jets_HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_QCD4JetsHT1500to2000'] = '/QCD-4Jets_HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
    samps['MC_2023PreBPix_GamJet_QCD4JetsHT2000toInf'] 	= '/QCD-4Jets_HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'

    #PostBPix
    samps['MC_2023PostBPix_GamJet_G4JetsHT40to70']    = '/GJ-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_G4JetsHT70to100']   = '/GJ-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_G4JetsHT100to200']  = '/GJ-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_G4JetsHT200to400']  = '/GJ-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_G4JetsHT400to600']  = '/GJ-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_G4JetsHT600toInf']  = '/GJ-4Jets_HT-600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v1/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_GJetsHT15to6000']   = '/GJet_PT-15to6000_TuneCP5-Flat_13p6TeV_pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v2/NANOAODSIM'

    samps['MC_2023PostBPix_GamJet_QCD4JetsHT40to70'] 	= '/QCD-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_QCD4JetsHT70to100'] 	= '/QCD-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_QCD4JetsHT100to200'] 	= '/QCD-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_QCD4JetsHT200to400'] 	= '/QCD-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_QCD4JetsHT400to600'] 	= '/QCD-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_QCD4JetsHT600to800'] 	= '/QCD-4Jets_HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_QCD4JetsHT800to1000'] 	= '/QCD-4Jets_HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM' 
    samps['MC_2023PostBPix_GamJet_QCD4JetsHT1000to1200'] = '/QCD-4Jets_HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_QCD4JetsHT1200to1500'] = '/QCD-4Jets_HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_QCD4JetsHT1500to2000'] = '/QCD-4Jets_HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    samps['MC_2023PostBPix_GamJet_QCD4JetsHT2000toInf'] 	= '/QCD-4Jets_HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
    return samps

def getMC_DiMuJet():
    samps = {}
    samps['MC_2023PreBPix_DiMuJet_DYto2L4Jets']  = '/DYto2L-4Jets_MLL-50_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v1/NANOAODSIM'
    samps['MC_2023PostBPix_DiMuJet_DYto2L4Jets'] = '/DYto2L-4Jets_MLL-50_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    return samps

def getMC_DiEleJet():
    samps = {}
    samps['MC_2023PreBPix_DiEleJet_DYto2L4Jets']  = '/DYto2L-4Jets_MLL-50_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v1/NANOAODSIM'
    samps['MC_2023PostBPix_DiEleJet_DYto2L4Jets'] = '/DYto2L-4Jets_MLL-50_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
    return samps

def getMC_DiJet():
	samps = {}
    #PreBPix
	samps['MC_2023PreBPix_DiJet_QCD4JetsHT40to70'] 	    = '/QCD-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
	samps['MC_2023PreBPix_DiJet_QCD4JetsHT70to100'] 	= '/QCD-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
	samps['MC_2023PreBPix_DiJet_QCD4JetsHT100to200'] 	= '/QCD-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
	samps['MC_2023PreBPix_DiJet_QCD4JetsHT200to400'] 	= '/QCD-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
	samps['MC_2023PreBPix_DiJet_QCD4JetsHT400to600'] 	= '/QCD-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
	samps['MC_2023PreBPix_DiJet_QCD4JetsHT600to800'] 	= '/QCD-4Jets_HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
	samps['MC_2023PreBPix_DiJet_QCD4JetsHT800to1000'] 	= '/QCD-4Jets_HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM' 
	samps['MC_2023PreBPix_DiJet_QCD4JetsHT1000to1200']  = '/QCD-4Jets_HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
	samps['MC_2023PreBPix_DiJet_QCD4JetsHT1200to1500']  = '/QCD-4Jets_HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
	samps['MC_2023PreBPix_DiJet_QCD4JetsHT1500to2000']  = '/QCD-4Jets_HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v3/NANOAODSIM'
	samps['MC_2023PreBPix_DiJet_QCD4JetsHT2000toInf'] 	= '/QCD-4Jets_HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23NanoAODv12-130X_mcRun3_2023_realistic_v14-v2/NANOAODSIM'
    
	#PostBPix
	samps['MC_2023PostBPix_DiJet_QCD4JetsHT40to70'] 	= '/QCD-4Jets_HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
	samps['MC_2023PostBPix_DiJet_QCD4JetsHT70to100'] 	= '/QCD-4Jets_HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
	samps['MC_2023PostBPix_DiJet_QCD4JetsHT100to200'] 	= '/QCD-4Jets_HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
	samps['MC_2023PostBPix_DiJet_QCD4JetsHT200to400'] 	= '/QCD-4Jets_HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
	samps['MC_2023PostBPix_DiJet_QCD4JetsHT400to600'] 	= '/QCD-4Jets_HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
	samps['MC_2023PostBPix_DiJet_QCD4JetsHT600to800'] 	= '/QCD-4Jets_HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
	samps['MC_2023PostBPix_DiJet_QCD4JetsHT800to1000']  = '/QCD-4Jets_HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM' 
	samps['MC_2023PostBPix_DiJet_QCD4JetsHT1000to1200'] = '/QCD-4Jets_HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
	samps['MC_2023PostBPix_DiJet_QCD4JetsHT1200to1500'] = '/QCD-4Jets_HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
	samps['MC_2023PostBPix_DiJet_QCD4JetsHT1500to2000'] = '/QCD-4Jets_HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v3/NANOAODSIM'
	samps['MC_2023PostBPix_DiJet_QCD4JetsHT2000toInf']  = '/QCD-4Jets_HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/Run3Summer23BPixNanoAODv12-130X_mcRun3_2023_realistic_postBPix_v2-v4/NANOAODSIM'
	return samps

#--------------------------
# Data
#--------------------------
def getData_GamJet():
    samps = {}
    #EGamma0
    samps['Data_2023Bv1_GamJet_EGamma0']    = '/EGamma0/Run2023B-22Sep2023-v1/NANOAOD'
    samps['Data_2023Cv1_GamJet_EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Cv2_GamJet_EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['Data_2023Cv3_GamJet_EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['Data_2023Cv4_GamJet_EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps['Data_2023Dv1_GamJet_EGamma0']    = '/EGamma0/Run2023D-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Dv2_GamJet_EGamma0']    = '/EGamma0/Run2023D-22Sep2023_v2-v1/NANOAOD'
   
    #EGamma1
    samps['Data_2023Bv1_GamJet_EGamma1']    = '/EGamma1/Run2023B-22Sep2023-v1/NANOAOD'
    samps['Data_2023Cv1_GamJet_EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Cv2_GamJet_EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['Data_2023Cv3_GamJet_EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['Data_2023Cv4_GamJet_EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps['Data_2023Dv1_GamJet_EGamma1']    = '/EGamma1/Run2023D-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Dv2_GamJet_EGamma1']    = '/EGamma1/Run2023D-22Sep2023_v2-v1/NANOAOD'
    return samps

def getData_DiEleJet():
    samps = {}
    #EGamma0
    samps['Data_2023Bv1_DiEleJet_EGamma0']    = '/EGamma0/Run2023B-22Sep2023-v1/NANOAOD'
    samps['Data_2023Cv1_DiEleJet_EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Cv2_DiEleJet_EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['Data_2023Cv3_DiEleJet_EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['Data_2023Cv4_DiEleJet_EGamma0']    = '/EGamma0/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps['Data_2023Dv1_DiEleJet_EGamma0']    = '/EGamma0/Run2023D-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Dv2_DiEleJet_EGamma0']    = '/EGamma0/Run2023D-22Sep2023_v2-v1/NANOAOD'
   
    #EGamma1
    samps['Data_2023Bv1_DiEleJet_EGamma1']    = '/EGamma1/Run2023B-22Sep2023-v1/NANOAOD'
    samps['Data_2023Cv1_DiEleJet_EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Cv2_DiEleJet_EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['Data_2023Cv3_DiEleJet_EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['Data_2023Cv4_DiEleJet_EGamma1']    = '/EGamma1/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps['Data_2023Dv1_DiEleJet_EGamma1']    = '/EGamma1/Run2023D-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Dv2_DiEleJet_EGamma1']    = '/EGamma1/Run2023D-22Sep2023_v2-v1/NANOAOD'
    return samps

def getData_DiMuJet():
    samps = {}
    samps['Data_2023Bv1_DiMuJet_Muon0']    = '/Muon0/Run2023B-22Sep2023-v1/NANOAOD'
    samps['Data_2023Cv1_DiMuJet_Muon0']    = '/Muon0/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Cv2_DiMuJet_Muon0']    = '/Muon0/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['Data_2023Cv3_DiMuJet_Muon0']    = '/Muon0/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['Data_2023Cv4_DiMuJet_Muon0']    = '/Muon0/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps['Data_2023Dv1_DiMuJet_Muon0']    = '/Muon0/Run2023D-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Dv2_DiMuJet_Muon0']    = '/Muon0/Run2023D-22Sep2023_v2-v1/NANOAOD'
   
    #Muon1
    samps['Data_2023Bv1_DiMuJet_Muon1']    = '/Muon1/Run2023B-22Sep2023-v1/NANOAOD'
    samps['Data_2023Cv1_DiMuJet_Muon1']    = '/Muon1/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Cv2_DiMuJet_Muon1']    = '/Muon1/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['Data_2023Cv3_DiMuJet_Muon1']    = '/Muon1/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['Data_2023Cv4_DiMuJet_Muon1']    = '/Muon1/Run2023C-22Sep2023_v4-v2/NANOAOD'
    samps['Data_2023Dv1_DiMuJet_Muon1']    = '/Muon1/Run2023D-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Dv2_DiMuJet_Muon1']    = '/Muon1/Run2023D-22Sep2023_v2-v1/NANOAOD'
    return samps

def getData_DiJet():
    samps = {}
    #ZeroBias
    samps['Data_2023Bv1_DiJet_ZeroBias']    = '/ZeroBias/Run2023B-22Sep2023-v1/NANOAOD'
    samps['Data_2023Cv1_DiJet_ZeroBias']    = '/ZeroBias/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Cv2_DiJet_ZeroBias']    = '/ZeroBias/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['Data_2023Cv3_DiJet_ZeroBias']    = '/ZeroBias/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['Data_2023Cv4_DiJet_ZeroBias']    = '/ZeroBias/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps['Data_2023Dv1_DiJet_ZeroBias']    = '/ZeroBias/Run2023D-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Dv2_DiJet_ZeroBias']    = '/ZeroBias/Run2023D-22Sep2023_v2-v1/NANOAOD'

    #JetMET0
    samps['Data_2023Bv1_DiJet_JetMET0']    = '/JetMET0/Run2023B-22Sep2023-v1/NANOAOD'
    samps['Data_2023Cv1_DiJet_JetMET0']    = '/JetMET0/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Cv2_DiJet_JetMET0']    = '/JetMET0/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['Data_2023Cv3_DiJet_JetMET0']    = '/JetMET0/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['Data_2023Cv4_DiJet_JetMET0']    = '/JetMET0/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps['Data_2023Dv1_DiJet_JetMET0']    = '/JetMET0/Run2023D-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Dv2_DiJet_JetMET0']    = '/JetMET0/Run2023D-22Sep2023_v2-v1/NANOAOD'
   
    #JetMET1
    samps['Data_2023Bv1_DiJet_JetMET1']    = '/JetMET1/Run2023B-22Sep2023-v2/NANOAOD'
    samps['Data_2023Cv1_DiJet_JetMET1']    = '/JetMET1/Run2023C-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Cv2_DiJet_JetMET1']    = '/JetMET1/Run2023C-22Sep2023_v2-v1/NANOAOD'
    samps['Data_2023Cv3_DiJet_JetMET1']    = '/JetMET1/Run2023C-22Sep2023_v3-v1/NANOAOD'
    samps['Data_2023Cv4_DiJet_JetMET1']    = '/JetMET1/Run2023C-22Sep2023_v4-v1/NANOAOD'
    samps['Data_2023Dv1_DiJet_JetMET1']    = '/JetMET1/Run2023D-22Sep2023_v1-v1/NANOAOD'
    samps['Data_2023Dv2_DiJet_JetMET1']    = '/JetMET1/Run2023D-22Sep2023_v2-v1/NANOAOD'
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

