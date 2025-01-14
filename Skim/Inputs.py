# Skim/Inputs.py

# Directory where skimmed files will be stored
#eosSkimDir = "/eos/user/r/rverma/www/public/cms-jerc-run3/Skim"
eosSkimDir = "/eos/cms/store/group/phys_jetmet/rverma/cms-jerc-run3/Skim"
#eosSkimDir = "root://hip-cms-se.csc.fi/store/user/rverma/cms-jerc-run2/Jan2025/Skim" 

# Years and Channels to process
Years = [
    #'2022',
    #'2023',
    '2024'
]

Channels = [
    #'ZeeJet',
    #'ZmmJet',
    #'GamJet',
    'Wqqm',
    'Wqqe',
    #'WqqDiLep',
    #'MultiJet',
]

# VOMS Proxy path (adjust as needed)
vomsProxy = "x509up_u93032"

# Events per job
eventsPerJobMC = 1e6  # Number of events per job for MC
eventsPerJobData = 5e6  # Number of events per job for Data

