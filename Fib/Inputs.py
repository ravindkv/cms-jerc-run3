#-----------------------------------------------------------------
eosFibDir="/eos/cms/store/group/phys_jetmet/rverma/cms-jerc-run3/Fib/15Nov2024"
#-----------------------------------------------------------------
Years = []
#Years.append('2022')
#Years.append('2023')
Years.append('2024')

Channels = {}
#Channels["GamJet"]    = "GamJet"
Channels["DiJet"]     = "DiJet"
#Channels["DiEleJet"]    = "DiEleJet"
#Channels["DiMuJet"]    = "DiMuJet"
#Channels["MCTruth"]   = "DiJet"
#Channels["Flavour"]   = "DiJet"
#Channels["VetoMap"]   = "DiJet"
#Channels["DiJet"]     = "DiJet"
#Channels["MultiJet"]  = "DiJet"
#Channels["IncJet"]    = "DiJet"

vomsProxy = "x509up_u93032"
#vomsProxy = "/tmp/x509up_u93032" # Does NOT work on condor node
reduceJobsMCBy     = 2
reduceJobsDataBy   = 5

