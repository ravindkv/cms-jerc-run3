"""
Script to merge multiple FilesFib_GamJet JSON dictionaries into a single combined JSON.

Reads all matching JSON files in the working directory, updates a master dictionary
with each file’s contents (later files override earlier keys), and writes out
FilesFib_2024_GamJet.json with the merged result.
"""
import json
import glob
import os

def merge_json_files(output_filename: str, patterns: list):
    merged_data = {}

    for pattern in patterns:
        for filepath in sorted(glob.glob(pattern)):
            print(f"Loading {filepath}...")
            with open(filepath, 'r', encoding='utf-8') as f:
                data = json.load(f)
                # Update the master dict; later entries override earlier ones if keys collide
                merged_data.update(data)

    # Ensure output directory exists
    os.makedirs(os.path.dirname(output_filename) or '.', exist_ok=True)

    print(f"Writing merged data to {output_filename}...")
    with open(output_filename, 'w', encoding='utf-8') as f:
        json.dump(merged_data, f, indent=4)

if __name__ == '__main__':
    # Patterns matching your JSON files
    file_patterns = [
        'FilesFib_MultiJet_2024_Data_2024?.json',
        'FilesFib_MultiJet_2024_DataReprocessing_2024?.json'
    ]
    # Output filename
    output_file = 'FilesFib_2024_MultiJet.json'

    merge_json_files(output_file, file_patterns)
    print('Merge complete.')

