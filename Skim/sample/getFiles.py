import os
import sys
import subprocess
import numpy as np
import json
sys.dont_write_bytecode = True
from SamplesNano import sampleDict 
sys.path.insert(0, os.getcwd().replace("sample",""))
from Inputs import *

def getFiles(dataset):
    dasquery = ["dasgoclient",  "-query=file dataset=%s" % dataset]
    files = subprocess.check_output(dasquery)
    files = files.splitlines()
    return files

def getEvents(dataset):
    dasquery = ["dasgoclient",  "-query=summary dataset=%s" % dataset]
    summary  = subprocess.check_output(dasquery)
    summary  = summary.splitlines()
    summary  = json.loads(summary[0].decode('utf-8'))
    nevents  = summary[0]['nevents'] 
    return nevents

#function to convert number to million
def formatNum(num):
    suffixes = ['', 'K', 'M', 'B', 'T']
    magnitude = 0
    while abs(num) >= 1000 and magnitude < len(suffixes)-1:
        magnitude += 1
        num /= 1000.0
    return f"{round(num, 1)}{suffixes[magnitude]}"

if __name__=="__main__":
    #Store the ouputs in two separate files
    f1 = open("FilesNano_cff.json", "w")
    f2 = open("JobsSkim_cff.py", "w")
    f3 = open("FilesSkim_cff.json", "w")
    allJobs = 0
    toNano = []
    toSkim = []
    for year in Years:
    #for year in ['2017']: 
        splitJobs = {}
        print('---------------------------------------')
        print(year)
        print("nFiles\t  nJobs\t nEvents\t Samples")
        print('---------------------------------------')
        jobs = 0
        for sKey, sName in sampleDict(year).items():
            fNano = getFiles(sName)
            if not fNano:
                print(f'PROBLEM: {sName}\n')
                continue
            tN = '\"%s_FileList_%s\" : %s'%(sKey,year, fNano)
            toNano.append(tN.replace('b\'', '\'').replace('\'', '\"'))#for json format
            nFiles = len(fNano)
            evt     = getEvents(sName)
            evtStr  = formatNum(evt) 
            nJob = int(np.ceil(evt/evtPerJob))
            if nFiles<nJob: 
                nJob = nFiles
            splitJobs[sKey] = [nJob, evtStr, evt, nFiles]
            jobs += nJob
            fSkim = []
            for i in range(nJob):
                fSkim.append('%s/%s/%s__%s_Skim_%sof%s.root'%(eosSkimDir, year, sKey, year, i+1, nJob))
            tS = '\"%s_FileList_%s\" : %s'%(sKey,year, fSkim)
            toSkim.append(tS.replace('\'', '\"'))
            print("%i\t %i\t %s\t %s"%(nFiles, nJob, evtStr, sKey))
        f2.write("Samples_%s = %s \n"%(str(year), str(splitJobs)))
        f2.write("AllJobs_%s = %s \n"%(str(year), str(jobs)))
        print('==================')
        print("AllJobs_%s = %i"%(year, jobs))
        print('==================')
        allJobs += jobs
    f1.write("{\n")
    f3.write("{\n")
    for t in range(len(toNano)): 
        if t==len(toNano)-1:
            f1.write('%s \n'%toNano[t])#for json format
            f3.write('%s \n'%toSkim[t])#for json format
        else:
            f1.write('%s,\n'%toNano[t])
            f3.write('%s,\n'%toSkim[t])
    f1.write("}\n")
    f3.write("}\n")
    f2.write("AllJobs_AllYears = %s \n"%str(allJobs))
 
