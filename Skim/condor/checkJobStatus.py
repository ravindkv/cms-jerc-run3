
import re
import os
import json
from ROOT import TFile
from createJdlFiles import createJobs

#-------------------------------------------------
# Open each finished file and see if they are OK
#-------------------------------------------------
fSkim_  = "../sample/FilesSkim_cff.json"
fResub_ = "tmpSub/resub_FilesSkim_cff.json"
fSkim = open(fSkim_, "r")
data = json.load(fSkim)
unfinished =  {}
nEvents = 0
print("Checking corrupted files ...") 
for sKey, skims in data.items():
    corruptedList = []
    nEvents = 0
    for skim in skims :
        try:
            f = TFile.Open(skim, "READ")
        except Exception:
            corruptedList.append(skim)
            #print("Unable to open: %s"%skim)
            continue
        if not f:
            print("Null pointer: %s"%skim)
            corruptedList.append(skim)
            continue
        if f.IsZombie():
            print("Zombie: %s"%skim)
            corruptedList.append(skim)
            continue
        if f.GetSize() < 3000:
            print("Empty file: %s"%skim)
            corruptedList.append(skim)
            continue
        h = f.Get("hEvents")
        if not h:
            print("hEvents does not exist: %s"%skim)
            corruptedList.append(skim)
            continue
        nEvents = nEvents + h.GetBinContent(2)
        f.Close()
    if len(corruptedList)>0:
        print("\n------ %s jobs to be resubmitted for %s ------\n"%(len(corruptedList),  sKey))
    unfinished[sKey] = corruptedList

fResub  = open(fResub_, "w")
json.dump(unfinished, fResub, indent=4)
fResub.close()

#-------------------------------------------------
# Create jdl files again for unfinished jobs 
#-------------------------------------------------
logDir = "log_resub"
jdlName = 'tmpSub/resub_submitJobs_cff.jdl'
createJobs(open(fResub_, "r"), open(jdlName, "w"), logDir)

