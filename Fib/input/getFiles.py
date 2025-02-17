import json
import re
import sys
import os
import itertools

sys.dont_write_bytecode = True
# Adjust the path so that Inputs.py (in the parent directory) can be imported.
sys.path.insert(0, os.getcwd().replace("input", ""))
from Inputs import *

def getFib(fibSummary):
    """
    Parse the FibSummary.txt file and return a list of dictionaries containing:
      - run_start
      - run_end
      - nib_fib_name
    """
    fib_list = []
    with open(fibSummary, 'r') as f:
        for line in f:
            # Skip empty lines or header lines (e.g. lines starting with '[run')
            if line.strip() == '' or line.startswith('[run'):
                continue
            # Match lines like: [run_start, run_end] | nib_fib_name | ...
            match = re.match(r'\[(\d+),\s*(\d+)\]\s*\|\s*(\S+)\s*\|.*', line)
            if match:
                run_start, run_end, nib_fib_name = match.groups()
                fib_list.append({
                    'run_start': run_start,
                    'run_end': run_end,
                    'nib_fib_name': nib_fib_name
                })
    return fib_list

# Directory containing the skim JSON files
skimDir = "../../Skim/condor/skim_files"
os.system("mkdir -p json")
fib_list = getFib('FibSummary.txt')

# Process each skim JSON file.
# The new file naming convention is: FilesSkim_{Channel}_{Year}_{Category}_{Subdataset}.json
for ch in Channels:
    for year, cat_dict in Years.items():
        for category, subdatasets in cat_dict.items():
            for sub in subdatasets:
                skimJson = f"FilesSkim_{ch}_{year}_{category}_{sub}.json"
                skimJsonPath = os.path.join(skimDir, skimJson)
                if not os.path.exists(skimJsonPath):
                    print(f"Warning: {skimJsonPath} does not exist. Skipping.")
                    continue
                # Copy the skim JSON file to the local json/ directory
                os.system(f"cp {skimJsonPath} json/")
                with open(os.path.join("json", skimJson), 'r') as f:
                    skim_data = json.load(f)

                output_json = {}
                # Loop over each dataset in the skim JSON
                for dataset_name in skim_data.keys():
                    # Loop over each fib entry from FibSummary.txt
                    for fib in fib_list:
                        run_start = fib['run_start']
                        run_end   = fib['run_end']
                        nib_fib_name = fib['nib_fib_name']
                        match = False
                        # Matching logic (adjust as needed)
                        if nib_fib_name.split('-')[0] in dataset_name:
                            match = True
                        # Ensure there is a second token before attempting to access it
                        if len(dataset_name.split('_')) > 1 and dataset_name.split('_')[1] in nib_fib_name:
                            match = True
                        if not match:
                            continue
                        # Construct a new dataset name that appends nib_fib info and run range
                        new_dataset_name = f"{dataset_name}_{nib_fib_name}-{run_start}-{run_end}"
                        # Construct the output file path. Here we include year, category and channel
                        output_file_path = f"{outFibDir}/{year}/{category}/{ch}/{new_dataset_name}.root"
                        output_json[new_dataset_name] = [
                            dataset_name,
                            run_start,
                            run_end,
                            output_file_path
                        ]
                # Write the output JSON for this skim file
                out_file = f"json/FilesFib_{ch}_{year}_{category}_{sub}.json"
                with open(out_file, 'w') as f:
                    json.dump(output_json, f, indent=4)
                print(f"Created: {out_file}")

