import os
import sys
import json
import itertools
sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("input",""))
from Inputs import *

#Reduce number of condor jobs w.r.t Skim by a factor of rData and rMC
def reducedJob(nJob, sKey):
    rData   = reduceJobsDataBy
    rMC     = reduceJobsMCBy
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
    skimDir = "../../Skim/input/json/"
    if not os.path.exists("json"):
        os.makedirs("json")
    allJobs = 0
    for year, ch in itertools.product(Years, Channels):
        skimFile = f"FilesSkim_{year}_{ch}.json"
        os.system(f"cp  {skimDir}/{skimFile} json/")
        
        fSkim = open(f"json/{skimFile}", "r")
        jSkim = json.load(fSkim)
        fHist = open(f"json/FilesHist_{year}_{ch}.json", "w")
        dHist = {}
        os.system(f"mkdir -p {eosHistDir}/{year}/{ch}")
        print(f"{eosHistDir}/{year}/{ch}")
        yJobs = 0
        for sKey in jSkim.keys():
            if year not in sKey:
                continue
            lSkim = jSkim[sKey]
            nJob  = reducedJob(len(lSkim), sKey)
            yJobs = yJobs+nJob
            lHist = []
            for i in range(nJob):
                lHist.append("%s/%s/%s/%s_Hist_%sof%s.root"%(eosHistDir, year, ch, sKey, i+1, nJob))
            dHist[sKey] = lHist
            print(f"{year}: {ch}: {sKey}: nJob = {nJob}")
        print(f"\n{year} : {ch}: nJobs  = {yJobs}\n")
        allJobs = allJobs + yJobs
    print(f"All jobs =  {allJobs}")
    json.dump(dHist, fHist, indent=4) 
    
