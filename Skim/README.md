## Produce skim file from NanoAOD

Check the details of Inputs.py file: change the skim dir, year, channel, proxy

### Step-1: get input root files of samples from DAS
* cd input
* voms-proxy-init --voms cms --valid 24:00
* python3 getRootFiles.py 

Have a look at the files produced in the **json** directory 

### Step-2: produce ONE skim file from nanofiles 
* cd ..
* make
* ./runMain -h

Run any one command printed on the screen. Check the output directory

* ls output

### Step-3: submit condor jobs to produce MANY skims 

* cd condor
* python createJobFiles.py
* cd tmpSub
* source submitAll.sh

Monitor the condor jobs on linux terminal
* condor_q 
* condor_q -better-analyze 75671743.5

Once condor_q is DONE. Check the finished jobs
* cd ..
* python3 checkFinishedJobs.py 

This will open each file and perform many checks. It  will  create jobs files for the failed 
jobs which can be resubmitted
* cd tmpSub
* condor_submit resubJobs.jdl

The condor output is stored in the path specified in the Skim/Inputs.py file

