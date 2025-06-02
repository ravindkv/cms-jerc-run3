#!/usr/bin/env python3
import os
import sys
import subprocess
import json
from pathlib import Path

# ------------------------------------------------------------------
# Import configuration from Inputs.py
# ------------------------------------------------------------------
currentDir = Path(__file__).resolve().parent
parentDir = currentDir.parent
sys.path.insert(0, str(parentDir))
from Inputs import Years, Channels

def check_duplicate_leaf_keys(json_data, seen=None, path=""):
    """
    Checks for duplicate leaf keys in a nested JSON-like dictionary.
    """
    if seen is None:
        seen = set()
    duplicate_found = False
    for key, value in json_data.items():
        if isinstance(value, dict):  # Recur for nested dictionaries
            duplicate_found |= check_duplicate_leaf_keys(value, seen, path)
        else:  # Leaf key
            if key in seen:
                print(f"Duplicate leaf key found: {key}")
                duplicate_found = True
            else:
                seen.add(key)
    return duplicate_found

# ------------------------------------------------------------------
# Functions to query DAS and format numbers
# ------------------------------------------------------------------
def getFiles(dataset):
    """
    Fetches the list of files for a given dataset using dasgoclient.
    """
    try:
        dasquery = ["dasgoclient", "-query=file dataset=%s" % dataset]
        output = subprocess.check_output(dasquery, stderr=subprocess.STDOUT)
        files = output.decode('utf-8').strip().splitlines()
        return files
    except subprocess.CalledProcessError as e:
        print(f"Error fetching files for dataset '{dataset}': {e.output.decode('utf-8')}")
        return []

def getEvents(dataset):
    """
    Fetches the number of events for a given dataset using dasgoclient.
    """
    try:
        dasquery = ["dasgoclient", "-query=summary dataset=%s" % dataset]
        output = subprocess.check_output(dasquery, stderr=subprocess.STDOUT)
        summary = json.loads(output.decode('utf-8').strip())
        nEvents = summary[0].get('nevents', 0)
        return nEvents
    except (subprocess.CalledProcessError, json.JSONDecodeError, IndexError) as e:
        print(f"Error fetching event count for dataset '{dataset}': {e}")
        return 0

def formatNum(num):
    """
    Formats a number into a human-readable string with suffixes.
    """
    suffixes = ['', 'K', 'M', 'B', 'T']
    magnitude = 0
    while abs(num) >= 1000 and magnitude < len(suffixes) - 1:
        magnitude += 1
        num /= 1000.0
    return f"{round(num, 1)}{suffixes[magnitude]}"

# ------------------------------------------------------------------
# Main Script
# ------------------------------------------------------------------
def main():
    allEvents = 0
    jsonDir = currentDir / "nano_files"
    jsonDir.mkdir(exist_ok=True)

    # Iterate over each channel (e.g. Wqqm, Wqqe, etc.)
    for channel in Channels:
        allEventsChannel = 0
        print(f"\n===========: Channel = {channel} :============\n")
        
        # Open the channel-based samples JSON file (e.g. SamplesNano_Wqqm.json)
        samplesJsonPath = currentDir / f"SamplesNano_{channel}.json"
        try:
            with open(samplesJsonPath, 'r') as f:
                samplesData = json.load(f)
        except Exception as e:
            print(f"Error opening {samplesJsonPath}: {e}")
            continue 

        if check_duplicate_leaf_keys(samplesData):
            print(f"{samplesJsonPath} has duplicate keys")
            continue

        # Process each year defined in Inputs.py
        for year, yinfo in Years.items():
            allEventsYear = 0
            print(f"========> {year}")

            # Loop over sample types defined for this year in Inputs.py.
            # For instance, yinfo might be:
            # { "MC": ["TTtoLNu2Q"], "MCSummer24": [...], "Data": ["2024C"], "DataReprocessing": [...] }
            for sampleType, desiredList in yinfo.items():
                # Process MC-type samples (expected to have sub-categories)
                if sampleType.lower().startswith("mc"):
                    for subcat in desiredList:
                        filesNanoDict = {}
                        print(f"  [{sampleType}/{subcat}]")
                        # Loop over each period (top-level key) in the samples JSON
                        for periodKey, periodContent in samplesData.items():
                            if not periodKey.startswith(year):
                                continue
                            branch = periodContent.get(sampleType, {})
                            # Only process the requested sub-category.
                            if subcat not in branch:
                                continue
                            sampleDict = branch[subcat]
                            print(f"    Processing period: {periodKey}")
                            for sampleKey, dataset in sampleDict.items():
                                filesNano = getFiles(dataset)
                                if not filesNano:
                                    print(f"        PROBLEM: No files found for dataset '{dataset}'.\n")
                                    continue
                                nFiles = len(filesNano)
                                nEvents = getEvents(dataset)
                                evtStr = formatNum(nEvents)
                                filesNanoDict[sampleKey] = [[evtStr, nEvents, nFiles], filesNano]
                                allEventsYear += nEvents
                                print(f"        {nFiles}\t {evtStr}\t {sampleKey}")
                        # Write out JSON for this MC sub-category (if not empty)
                        if filesNanoDict:
                            nanoOutName = jsonDir / f"FilesNano_{channel}_{year}_{sampleType}_{subcat}.json"
                            with open(nanoOutName, 'w') as f:
                                json.dump(filesNanoDict, f, indent=4)
                        else:
                            print(f"    No {sampleType} samples found for sub-category '{subcat}' in year {year}\n")

                # Process Data-type samples (expected to be organized by period)
                elif sampleType.lower().startswith("data"):
                    for period in desiredList:
                        filesNanoDict = {}
                        # The top-level key in the JSON is the year (e.g. "2024")
                        yearData = samplesData.get(year, {})
                        if not yearData:
                            print(f"  {sampleType} Year {year} not found in samples JSON!")
                            continue

                        if period not in yearData.get(sampleType, {}):
                            print(f"  [{sampleType}] Period {period} not found in samples JSON for year {year}")
                            continue

                        branch = yearData[sampleType][period]
                        print(f"  [{sampleType}/{period}]")
                        for sampleKey, dataset in branch.items():
                            filesNano = getFiles(dataset)
                            if not filesNano:
                                print(f"      PROBLEM: No files found for dataset '{dataset}'.\n")
                                continue
                            nFiles = len(filesNano)
                            nEvents = getEvents(dataset)
                            evtStr = formatNum(nEvents)
                            filesNanoDict[sampleKey] = [[evtStr, nEvents, nFiles], filesNano]
                            allEventsYear += nEvents
                            print(f"      {nFiles}\t {evtStr}\t {sampleKey}")
                        # Write out JSON for this data period (if not empty)
                        if filesNanoDict:
                            nanoOutName = jsonDir / f"FilesNano_{channel}_{year}_{sampleType}_{period}.json"
                            with open(nanoOutName, 'w') as f:
                                json.dump(filesNanoDict, f, indent=4)
                        else:
                            print(f"    No {sampleType} samples found for period '{period}' in year {year}\n")
                else:
                    print(f"Unrecognized sample type '{sampleType}' in configuration for year {year}. Skipping...")

            print(f"AllEvents for {year} = {formatNum(allEventsYear)}\n")
            allEventsChannel += allEventsYear

        print(f"AllEvents for {channel} = {formatNum(allEventsChannel)}\n")
        allEvents += allEventsChannel

    print('---------------------------------------')
    print(f"AllEvents = {formatNum(allEvents)}")
    print('---------------------------------------')

if __name__ == "__main__":
    main()

