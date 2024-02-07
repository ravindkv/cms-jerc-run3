import os
import sys
import itertools
sys.dont_write_bytecode = True

#IMPORT MODULES FROM OTHER DIR
sys.path.insert(0, os.getcwd().replace("condor",""))
sys.path.insert(0, os.getcwd().replace("condor","sample"))
from Inputs import *
#from JobsSkim_cff import Samples_2022, Samples_2023
from JobsSkim_cff import Samples_2022

if os.path.exists("tmpSub"):
    os.system("rm -r tmpSub")
    print("Deleted dir: tmpSub")
os.system("mkdir -p tmpSub/log")
print("Created dir: tmpSub")
condorLogDir = "log"
tarFile = "tmpSub/Skim.tar.gz"
os.system("tar -zcvf %s ../../Skim --exclude condor"%tarFile)
os.system("cp runMakeSkims.sh tmpSub/")
common_command = \
'Universe   = vanilla\n\
should_transfer_files = YES\n\
when_to_transfer_output = ON_EXIT\n\
Transfer_Input_Files = Skim.tar.gz, runMakeSkims.sh\n\
x509userproxy        = /tmp/x509up_u93032\n\
Output = %s/log_$(cluster)_$(process).stdout\n\
Error  = %s/log_$(cluster)_$(process).stderr\n\n'%(condorLogDir, condorLogDir)
#use_x509userproxy = true\n\

#----------------------------------------
#Create jdl files
#----------------------------------------
subFile = open('tmpSub/condorSubmit_cff.sh','w')
for year in Years: 
    samples = eval("Samples_%s"%year)
    jdlName = 'submitJobs_%s_cff.jdl'%(year)
    jdlFile = open('tmpSub/%s'%jdlName,'w')
    jdlFile.write('Executable =  runMakeSkims.sh \n')
    jdlFile.write(common_command)
    outDir="%s/%s"%(eosSkimDir, year)
    os.system("mkdir -p %s"%outDir) 
    print("Out eos dir: %s"%outDir)
    jdlFile.write("X=$(step)+1\n")
    
    for sampleName, nJobEvt in samples.items():
        nJob = nJobEvt[0]
        args =  'Arguments  = %s %s $INT(X) %i %s\n' %(year, sampleName, nJob, outDir)
        args += "Queue %i\n\n"%nJob
        jdlFile.write(args)
    
    subFile.write("condor_submit %s\n"%jdlName)
    jdlFile.close() 
subFile.close()
