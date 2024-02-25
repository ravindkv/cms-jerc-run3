import os
import sys
import json
sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("sample",""))
from Inputs import *

#Reduce number of condor jobs w.r.t Skim by a factor of rData and rMC
def reducedJob(nJob, sKey):
    rData   = 10
    rMC     = 2
    if "Data" in sKey:
        if nJob>rData:
            n = nJob/rData
        else:
            n = 1
    else:
        if nJob>rMC:
            n = nJob/rMC
        else:
            n = 1
    return int(n)

path = "/afs/cern.ch/work/r/rverma/public/jerc/CMSSW_13_3_0/src/cms-jerc-run3/Skim/sample"
if __name__=="__main__":
    os.system(f"cp  {path}/FilesSkim_cff.json .")
    fSkim = open("FilesSkim_cff.json", "r")
    jSkim = json.load(fSkim)
    fHist = open("FilesHist_cff.json", "w")
    dHist = {}
    allJobs = 0
    for year in Years:
        print(f"{eosHistDir}/{year}")
        os.system(f"mkdir -p {eosHistDir}/{year}")
        yJobs = 0
        for sKey in jSkim.keys():
            if year not in sKey:
                continue
            lSkim = jSkim[sKey]
            nJob  = reducedJob(len(lSkim), sKey)
            yJobs = yJobs+nJob
            lHist = []
            for i in range(nJob):
                lHist.append("%s/%s/%s__%sof%s.root"%(eosHistDir, year, sKey, i+1, nJob))
            dHist[sKey] = lHist
            print(f"{year}: {sKey}: nJob = {nJob}")
        print(f"\n{year}: nJobs  = {yJobs}\n")
    allJobs = allJobs + yJobs
    print(f"All jobs =  {allJobs}")
    json.dump(dHist, fHist, indent=4) 
    
