import re
import os
import sys
import json
import itertools
import multiprocessing
from ROOT import TFile
sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("condor",""))
from Inputs import *
from createSkimJobFiles import createJobs

#-------------------------------------------------
# Function to check a single ROOT file
#-------------------------------------------------
def check_file(args):
    sKey, skim = args
    f = None
    try:
        # Open the file in a way that ensures it's closed later
        f = TFile.Open(skim, "READ")
        
        # Quick exit if file is inaccessible or corrupted
        if not f or f.IsZombie() or f.GetSize() < 3000:
            print(f"Corrupted or empty file: {skim}")
            return (sKey, skim, True)
        
        # Check for the specific histogram and its validity
        h = f.Get("Cutflow/h1EventInCutflow")
        if not h:
            print(f"'Cutflow/h1EventInCutflow' not found in: {skim}")
            return (sKey, skim, True)

        # Check for the specific TTree and its validity
        tree = f.Get("Events")
        if not tree:
            print(f"'Events' TTree not found in: {skim}")
            return (sKey, skim, True)

        # All checks passed, file is considered good
        return (sKey, skim, False)
        
    except Exception as e:
        print(f"Exception while opening {skim}: {e}")
        return (sKey, skim, True)
    
    finally:
        if f:  # Ensure file is closed even if an error occurred
            f.Close()

#-------------------------------------------------
# Open each finished file and see if they are OK
#-------------------------------------------------
def check_jobs(jsonFile):
    unfinished = {}
    print("Checking for corrupted files using multiprocessing...")

    # Prepare a list of (sKey, skim) pairs
    file_list = []
    for sKey, skims in jsonFile.items():
        for skim in skims:
            file_list.append((sKey, skim))

    # Use multiprocessing to check files in parallel
    with multiprocessing.Pool() as pool:
        results = pool.map(check_file, file_list)

    # Process the results
    for sKey, skim, is_corrupted in results:
        if is_corrupted:
            if sKey not in unfinished:
                unfinished[sKey] = []
            unfinished[sKey].append(skim)

    return unfinished

#-------------------------------------------------
# Main execution block
#-------------------------------------------------
if __name__ == "__main__":
    logDir = "resubLog"
    dResubs = {}
    fResub = "tmpSub/resubFilesSkim.json"

    # Ensure the 'tmpSub' directory exists
    os.makedirs("tmpSub", exist_ok=True)

    with open('tmpSub/resubJobs.jdl', 'w') as jdlFile_:
        if os.path.exists(fResub):
            # If resubmission file exists, check those files
            with open(fResub, "r") as fResub_:
                jsonFile = json.load(fResub_)
            dResub = check_jobs(jsonFile)
            dResubs.update(dResub)
            # Write updated list of files to resubmit
            with open(fResub, "w") as fResub__:
                json.dump(dResubs, fResub__, indent=4)
            # Create jobs for resubmission
            with open(fResub, "r") as fResub___:
                createJobs(fResub___, jdlFile_, logDir)
        else:
            # First-time check: iterate over all years and channels
            for year, ch in itertools.product(list(Years.keys()), Channels):
                for dataOrMc, samples in Years[year].items():
                    for sample in samples:
                        chYearOther = f"{ch}_{year}_{dataOrMc}_{sample}" 
                        print(f"\nProcessing {chYearOther}")
                        json_file_path = f"skim_files/FilesSkim_{chYearOther}.json"
                        if not os.path.exists(json_file_path):
                            print(f"ERROR: JSON file {json_file_path} does not exist.")
                            sys.exit(1)
                        with open(json_file_path, "r") as fSkim:
                            jsonFile = json.load(fSkim)
                        dResub = check_jobs(jsonFile)
                        dResubs.update(dResub)
            # Write the list of files to resubmit
            with open(fResub, "w") as fResub_:
                json.dump(dResubs, fResub_, indent=4)
            # Create jobs for resubmission
            with open(fResub, "r") as fResub__:
                createJobs(fResub__, jdlFile_, logDir)

    # Calculate the total number of files to resubmit
    totResub = sum(len(files) for files in dResubs.values())
    print(f"\n=========> Total files to be resubmitted: {totResub} <==========\n")
    print(f"{fResub}\n")
    
