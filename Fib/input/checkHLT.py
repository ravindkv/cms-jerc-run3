import json
import ROOT

def check_hlt_zero_bias(json_file):
    # Load JSON file
    with open(json_file, 'r') as f:
        data = json.load(f)
    
    # Iterate over each entry in the JSON
    for key, value in data.items():
        file_path = value[-1]  # The file path is the last element in the list
        try:
            # Open the ROOT file
            root_file = ROOT.TFile.Open(file_path)
            if not root_file or root_file.IsZombie():
                print(f"Error: Could not open ROOT file {file_path}")
                continue
            
            # Access the 'Events' TTree
            tree = root_file.Get("Events")
            if not tree:
                print(f"Error: 'Events' TTree not found in {file_path}")
                root_file.Close()
                continue
            
            # Check for the presence of the 'HLT_ZeroBias' branch
            if tree.GetListOfBranches().FindObject("HLT_ZeroBias"):
                print(f"'HLT_ZeroBias' branch found in {file_path}")
            else:
                print(f"\n'HLT_ZeroBias' branch NOT found in {file_path}\n")
            
            # Close the ROOT file
            root_file.Close()
        except Exception as e:
            print(f"An error occurred with file {file_path}: {e}")

# Specify the JSON file containing the ROOT file paths
json_file = "FilesFib_2024_DiJet.json"
check_hlt_zero_bias(json_file)

