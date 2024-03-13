import os
import sys
import subprocess
import numpy as np
import itertools
import json
sys.dont_write_bytecode = True
from SamplesNano import sampleDict 
sys.path.insert(0, os.getcwd().replace("sample",""))
from Inputs import *

def getFiles(dataset):
    dasquery = ["dasgoclient",  "-query=file dataset=%s" % dataset]
    files = subprocess.check_output(dasquery)
    files = files.splitlines()
    files_ = []
    for file in files:
        files_.append(file.decode('utf-8'))
    return files_

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
    f1 = open("FilesNano_cff.json", "w")
    f2 = open("JobsSkim_cff.json", "w")
    f3 = open("FilesSkim_cff.json", "w")
    allJobs = 0
    toNano = {}
    toSkim = {}
    toJobs = {}
    for ch, year in itertools.product(Channels, Years):
        print('---------------------------------------')
        print(ch, year)
        print("nFiles\t  nJobs\t nEvents\t Samples")
        print('---------------------------------------')
        jobs = 0
        for sKey, sName in sampleDict().items():
            if not ch in sKey: continue
            if not year in sKey: continue
            fNano = getFiles(sName)
            if not fNano:
                print(f'PROBLEM: {sName}\n')
                continue
            toNano[sKey] = fNano
            nFiles = len(fNano)
            evt     = getEvents(sName)
            evtStr  = formatNum(evt) 
            if "Data" in sKey:
                nJob = int(np.ceil(evt/evtPerJobData))
            else:
                nJob = int(np.ceil(evt/evtPerJobMC))
            if nFiles<nJob: 
                nJob = nFiles
            toJobs[sKey] = [nJob, evtStr, evt, nFiles]
            jobs += nJob
            fSkim = []
            for i in range(nJob):
                fSkim.append("%s/%s/%s/%s__Skim_%sof%s.root"%(eosSkimDir, ch, year, sKey, i+1, nJob))
            toSkim[sKey] = fSkim
            print("%i\t %i\t %s\t %s"%(nFiles, nJob, evtStr, sKey))
        print("AllJobs_%s = %i"%(year, jobs))
        allJobs += jobs
    json.dump(toNano, f1, indent=4)
    json.dump(toJobs, f2, indent=4)
    json.dump(toSkim, f3, indent=4)
    print('---------------------------------------')
    print("AllJobs_AllYears = %s \n"%str(allJobs))
    print('---------------------------------------')
 
