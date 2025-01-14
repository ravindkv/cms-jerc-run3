import json
import re
import sys
import os
import itertools
sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("input",""))
from Inputs import *

def getFib(fibSummary):
    # Parse fib.txt
    fib_list = []
    with open(fibSummary, 'r') as f:
        for line in f:
            # Skip empty lines or headers
            if line.strip() == '' or line.startswith('[run'):
                continue
            # Extract run ranges and nib-fib names
            match = re.match(r'\[(\d+),\s*(\d+)\]\s*\|\s*(\S+)\s*\|.*', line)
            if match:
                run_start, run_end, nib_fib_name = match.groups()
                fib_list.append({
                    'run_start': run_start,
                    'run_end': run_end,
                    'nib_fib_name': nib_fib_name
                })

    #print(fib_list)
    return fib_list


skimDir = "../../Skim/input/json/"
os.system("mkdir -p json")
fib_list = getFib(f'FibSummary.txt')


for year, ch in itertools.product(Years, Channels):
    skimJson = f'FilesSkim_{year}_{ch}.json'
    os.system(f"cp {skimDir}/{skimJson} json/")
    with open(f"json/{skimJson}", 'r') as f:
        skim_data = json.load(f)

    # Step 2: Generate the output JSON
    output_json = {}

    # Loop over datasets in Skim JSON
    for dataset_name in skim_data.keys():
        # Loop over fib entries
        for fib in fib_list:
            run_start = fib['run_start']
            run_end = fib['run_end']
            nib_fib_name = fib['nib_fib_name']
            match = False
            #if nib_fib_name.split('-')[0]== dataset_name.split('_')[1]: 
            if nib_fib_name.split('-')[0] in dataset_name: 
                match = True

            #if dataset_name.split('_')[1] in nib_fib_name: 
            if dataset_name.split('_')[1] in nib_fib_name: 
                match = True

            if not match: continue
            # Construct new dataset name with nib-fib
            new_dataset_name = f"{dataset_name}_{nib_fib_name}-{run_start}-{run_end}"

            # Construct the output file path
            output_file_path = f"{eosFibDir}/{year}/{ch}/{new_dataset_name}.root"

            # Update the output JSON
            output_json[new_dataset_name] = [
                dataset_name,
                run_start,
                run_end,
                output_file_path
            ]

    # Step 3: Write the output JSON
    out_file = f'json/FilesFib_{year}_{ch}.json'
    with open(out_file, 'w') as f:
        json.dump(output_json, f, indent=4)
    print(out_file)
