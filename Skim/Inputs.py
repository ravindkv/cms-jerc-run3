#-----------------------------------------------------------------
eosSkimDir="/eos/user/r/rverma/www/public/cms-jerc-run3/Skim"
#-----------------------------------------------------------------
Years = []
#Years.append('2022')
Years.append('2023')

Channels = []
Channels.append("GamJet")
Channels.append("DiEleJet")
Channels.append("DiMuJet")
Channels.append("DiJet")

vomsProxy = "/afs/cern.ch/user/r/rverma/x509up_u93032"
#vomsProxy = "/tmp/x509up_u93032" # Does NOT work on condor node
evtPerJobMC   = 1e6 # n million events per job
evtPerJobData = 5e6 

