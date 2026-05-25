import json
import glob

# Pattern to match all relevant json files
file_patterns = [
    "json/FilesSkim_MultiJet_2024_Data_2024*.json",
    "json/FilesSkim_MultiJet_2024_DataReprocessing_2024*.json"
]

# Find all files matching the patterns
input_files = []
for pattern in file_patterns:
    input_files.extend(glob.glob(pattern))

# Output file name
output_file = "json/FilesSkim_2024_MultiJet.json"

merged_dict = {}

for fname in input_files:
    with open(fname, 'r') as f:
        this_dict = json.load(f)
        for key, value in this_dict.items():
            if key in merged_dict:
                # Extend list for existing keys
                merged_dict[key].extend(value)
            else:
                # Create new key
                merged_dict[key] = value

# Optional: Remove duplicate files for each key (just in case)
for key in merged_dict:
    merged_dict[key] = list(sorted(set(merged_dict[key])))

# Write to output
with open(output_file, 'w') as f:
    json.dump(merged_dict, f, indent=2)

print(f"Merged {len(input_files)} files into {output_file}")

