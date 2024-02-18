# Produce skim file from NanoAOD

## Step-1: get root files of samples from DAS
* cd sample
* python3 getFiles.py  
* cd ..

## Step-2: produce ONE skim file from nanofiles 
* make
* ./makeSkim -h

or 
* ./makeSkim

## Step-3: submit condor jobs to produce MANY skims 

* cd condor
* python createJdlFiles.py
* cd tmpSub
* condor_submit submitJobs_cff.jdl

Monitor the condor jobs on linux terminal
* condor_q 
* condor_q -better-analyze 75671743.5

Once condor_q is DONE. Check the finished jobs
* cd ..
* python3 checkJobStatus.py

This will open each file and perform many checks. It  will  create JDL files for the failed 
jobs which can be resubmitted
* cd tmpSub
* condor_submit resub_submitJobs_cff.jdl
