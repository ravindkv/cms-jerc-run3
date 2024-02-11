import os
import sys
import json
import itertools
sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("condor",""))
from Inputs import vomsProxy

def createJobs(jsonFile, jdlFile, logDir="log"):
    common_command = \
    'Universe   = vanilla\n\
    should_transfer_files = YES\n\
    when_to_transfer_output = ON_EXIT\n\
    Transfer_Input_Files = Skim.tar.gz, runMakeSkims.sh\n\
    x509userproxy        = %s\n\
    +MaxRuntime = 60*60*24\n\
    max_retries = 2\n\
    use_x509userproxy = true\n\
    Output = %s/log_$(cluster)_$(process).stdout\n\
    Log = %s/log_$(cluster)_$(process).log\n\
    Error  = %s/log_$(cluster)_$(process).stderr\n\n'%(vomsProxy, logDir, logDir, logDir)

    #---------------------------------------------
    #Create jdl (job discription language) files
    #---------------------------------------------
    data = json.load(jsonFile)
    jdlFile.write('Executable =  runMakeSkims.sh \n')
    jdlFile.write(common_command)
    for sKey, skims in data.items():
        jdlFile.write("\n")
        for skim in skims:
            outDir  = skim.split(sKey)[0]
            restStr = skim.split(sKey)[1]
            oName   = "%s%s"%(sKey, restStr)
            args =  'Arguments  = %s %s\n' %(oName, outDir)
            args += "Queue 1\n"
            jdlFile.write(args)
    jdlFile.close() 

if __name__=="__main__":
    tarFile = "tmpSub/Skim.tar.gz"
    os.system("tar --exclude condor --exclude *.root -zcvf %s ../../Skim"%tarFile)
    os.system("cp runMakeSkims.sh tmpSub/")
    if os.path.exists("tmpSub"):
        #os.system("rm -r tmpSub")
        print("Deleted dir: tmpSub")
    os.system("mkdir -p tmpSub/log")
    print("Created dir: tmpSub")
    jsonFile = open("../sample/FilesSkim_cff.json", "r")
    jdlFile  = open('tmpSub/submitJobs_cff.jdl','w')
    logDir   = "log"
    createJobs(jsonFile, jdlFile, logDir)
