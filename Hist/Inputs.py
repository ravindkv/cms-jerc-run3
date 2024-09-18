#-----------------------------------------------------------------
eosHistDir="/eos/user/r/rverma/www/public/cms-jerc-run3/Hist"
#-----------------------------------------------------------------
Years = []
#Years.append('2022')
#Years.append('2023')
Years.append('2024')

Channels = {}
Channels["GamJet"]    = "GamJet"
Channels["ZeeJet"]    = "DiEleJet"
Channels["ZmmJet"]    = "DiMuJet"
Channels["MCTruth"]   = "DiJet"
Channels["Flavour"]   = "DiJet"
Channels["VetoMap"]   = "DiJet"
Channels["DiJet"]     = "DiJet"
Channels["MultiJet"]  = "DiJet"
Channels["IncJet"]    = "DiJet"

vomsProxy = "/afs/cern.ch/user/r/rverma/x509up_u93032"
#vomsProxy = "/tmp/x509up_u93032" # Does NOT work on condor node
reduceJobsMCBy     = 2
reduceJobsDataBy   = 5

