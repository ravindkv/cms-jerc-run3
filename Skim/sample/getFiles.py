import os
import sys
import subprocess
import numpy as np
sys.dont_write_bytecode = True
from SamplesNano import sampleDict 
sys.path.insert(0, os.getcwd().replace("sample",""))
from Inputs import *

#Function to fetch the name of all files in one string
def getFileList(sample):
    std_output, std_error = subprocess.Popen("dasgoclient --query='file dataset=%s status=valid'"%sample,shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()
    names = std_output.decode("ascii").replace('\n',' ')
    return names

#function to convert number to million
def format_number(num):
    suffixes = ['', 'K', 'M', 'B', 'T']
    magnitude = 0
    while abs(num) >= 1000 and magnitude < len(suffixes)-1:
        magnitude += 1
        num /= 1000.0
    return f"{round(num, 1)}{suffixes[magnitude]}"

#Function to print the total events in nice format 
def getEvents(sample):
    try:
        std_output, std_error = subprocess.Popen("dasgoclient --query='summary dataset=%s' | sed  's/null/None/g'"%sample,shell=True,stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()
        num = eval(std_output)[0]['nevents']
        return num
    except subprocess.CalledProcessError as e:
        print(f"Error fetching events for {sample}: {e}")
        return 0 

if __name__=="__main__":
    #Store the ouputs in two separate files
    f1 = open("FilesNano_cff.sh", "w")
    f2 = open("JobsNano_cff.py", "w")

    allJobs = 0
    for year in Years:
    #for year in ['2017']: 
        splitJobs = {}
        print('---------------------------------------')
        print(year)
        print("nFiles\t  nJobs\t nEvents\t Samples")
        print('---------------------------------------')
        line = ""
        jobs = 0
        for sampleName, sample in sampleDict(year).items():
            line += '%s_FileList_%s="xrootd '%(sampleName,year)
            fileList = getFileList(sample)
            if not fileList:
                print(f'PROBLEM: {sample}\n')
                continue
            line += fileList 
            line +='"\n\n'
            nFiles = len(fileList.split(" "))
            evt     = getEvents(sample)
            evtStr  = format_number(evt) 
            nJob = int(np.ceil(evt/evtPerJob))
            if nFiles<nJob: 
                nJob = nFiles
            splitJobs[sampleName] = [nJob, evtStr, evt, nFiles]
            jobs += nJob
            print("%i\t %i\t %s\t %s"%(nFiles, nJob, evtStr, sampleName))
        f1.write(line)
        f2.write("Samples_%s = %s \n"%(str(year), str(splitJobs)))
        f2.write("AllJobs_%s = %s \n"%(str(year), str(jobs)))
        print('==================')
        print("AllJobs_%s = %i"%(year, jobs))
        print('==================')
        allJobs += jobs
    f2.write("AllJobs_AllYears = %s \n"%str(allJobs))
 
