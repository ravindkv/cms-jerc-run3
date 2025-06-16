import os
import sys
import json
import itertools
sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("condor",""))
from Inputs import *

def createJobs(jsonFile, jdlFile, logDir="log"):
    os.system(f"mkdir -p tmpSub/{logDir}")
    os.system("cp /tmp/%s tmpSub"%vomsProxy)
    common_command = \
    'Universe   = vanilla\n\
    should_transfer_files = YES\n\
    when_to_transfer_output = ON_EXIT\n\
    Transfer_Input_Files = Fib.tar.gz, runMain.sh\n\
    x509userproxy        = %s\n\
    +MaxRuntime = 60*60*24\n\
    max_retries = 2\n\
    Output = %s/log_$(cluster)_$(process).stdout\n\
    Log = %s/log_$(cluster)_$(process).log\n\
    Error  = %s/log_$(cluster)_$(process).stderr\n\n'%(vomsProxy, logDir, logDir, logDir)

    #---------------------------------------------
    #Create jdl (job discription language) files
    #---------------------------------------------
    data = json.load(jsonFile)
    jdlFile.write('Executable =  runMain.sh \n')
    jdlFile.write(common_command)
    localRun = open("tmpSub/runLocal.sh", "w")
    for sKey, skims in data.items():
        jdlFile.write("\n")
        skim = skims[-1]
        outDir  = skim.split(sKey)[0]
        restStr = skim.split(sKey)[1]
        oName   = "%s%s"%(sKey, restStr)
        args =  'Arguments  = %s %s\n' %(oName, outDir)
        args += "Queue 1\n"
        jdlFile.write(args)
        localRun.write(f"./runMain -o {oName} && xrdcp -f output/{oName} {outDir}\n\n")
    jdlFile.close() 
    localRun.close()

if __name__=="__main__":
    if os.path.exists("tmpSub"):
        os.system("rm -r tmpSub")
        print("Deleted dir: tmpSub")
    os.system("mkdir -p tmpSub")
    tarFile = "tmpSub/Fib.tar.gz"
    os.system("tar --exclude condor --exclude *.root --exclude output -zcvf %s ../../Fib"%tarFile)
    os.system("cp runMain.sh tmpSub/")
    print("Created dir: tmpSub")
    submitAll = open("tmpSub/submitAll.sh", "w") 
    for year, ch in itertools.product(Years, Channels):
        outPath = f"{outFibDir}/{year}/{ch}"
        os.system(f"mkdir -p {outPath}")
        print(f"Created dir: {outPath}")
        jsonFile = open(f"../input/json/FilesFib_{year}_{ch}.json", "r")
        jdlName  = f'submitJobs_{year}_{ch}.jdl'
        jdlFile  = open(f'tmpSub/{jdlName}','w')
        logDir   = "log"
        createJobs(jsonFile, jdlFile, logDir)
        submitAll.write(f'condor_submit {jdlName}\n')
