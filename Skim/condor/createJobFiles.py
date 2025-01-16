import os
import sys
import json
import itertools
import subprocess
from datetime import datetime

sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("condor", ""))
from Inputs import *

def check_git_status():
    """Check that there are no uncommitted changes and that the local branch is pushed."""
    # Check for uncommitted changes
    try:
        status = subprocess.check_output(["git", "status", "--porcelain"]).decode("utf-8").strip()
    except subprocess.CalledProcessError as e:
        print("Error running git status")
        sys.exit(1)
    if status:
        print("ERROR: There are uncommitted changes in your repository. Please commit your changes before submitting jobs.")
        sys.exit(1)
    
    # Check if local branch is ahead of remote
    try:
        # Update remote information
        subprocess.check_call(["git", "fetch"])
        # Get current branch name
        branch = subprocess.check_output(["git", "rev-parse", "--abbrev-ref", "HEAD"]).decode("utf-8").strip()
        # Count commits ahead of remote
        ahead_count = subprocess.check_output(
            ["git", "rev-list", "HEAD...origin/{}".format(branch), "--count"]
        ).decode("utf-8").strip()
    except subprocess.CalledProcessError as e:
        print("Error checking if your branch is ahead of remote.")
        sys.exit(1)
    if int(ahead_count) > 0:
        print(f"ERROR: Your local branch '{branch}' is ahead of origin by {ahead_count} commits. Please push before submitting jobs.")
        sys.exit(1)
    print("Git status check passed: Repository is clean and synchronized with remote.")

def get_git_info():
    """Retrieve the short commit hash and current date."""
    try:
        git_hash = subprocess.check_output(["git", "rev-parse", "--short", "HEAD"]).decode("utf-8").strip()
    except subprocess.CalledProcessError:
        git_hash = "unknown"
    # Format the current date as YYYYMMDD
    current_date = datetime.now().strftime("%Y%m%d")
    return current_date, git_hash

def createJobs(jsonFile, jdlFile, logDir="log"):
    os.system(f"mkdir -p tmpSub/{logDir}")
    os.system("cp /tmp/%s tmpSub" % vomsProxy)
    common_command = \
    'Universe   = vanilla\n\
should_transfer_files = YES\n\
when_to_transfer_output = ON_EXIT\n\
Transfer_Input_Files = Skim.tar.gz, runMain.sh\n\
x509userproxy        = %s\n\
+MaxRuntime = 60*60*24\n\
max_retries = 2\n\
Output = %s/log_$(cluster)_$(process).stdout\n\
Log = %s/log_$(cluster)_$(process).log\n\
Error  = %s/log_$(cluster)_$(process).stderr\n\n' % (vomsProxy, logDir, logDir, logDir)

    #---------------------------------------------
    #Create jdl (job description language) files
    #---------------------------------------------
    data = json.load(jsonFile)
    jdlFile.write('Executable =  runMain.sh \n')
    jdlFile.write(common_command)
    for sKey, skims in data.items():
        jdlFile.write("\n")
        for skim in skims:
            outDir  = skim.split(sKey)[0]
            restStr = skim.split(sKey)[1]
            oName   = "%s%s" % (sKey, restStr)
            args = 'Arguments  = %s %s\n' % (oName, outDir)
            args += "Queue 1\n"
            jdlFile.write(args)
    jdlFile.close() 

if __name__ == "__main__":
    # --- Step 1: Check Git status ---
    check_git_status()

    # --- Step 2: Retrieve Git info ---
    current_date, git_hash = get_git_info()
    version_suffix = f"{current_date}_commit-{git_hash}"
    print(f"Using version: {version_suffix}")

    # --- Prepare temporary submission directory ---
    if os.path.exists("tmpSub"):
        os.system("rm -r tmpSub")
        print("Deleted dir: tmpSub")
    os.system("mkdir -p tmpSub")
    tarFile = "tmpSub/Skim.tar.gz"
    os.system("tar --exclude condor --exclude *.root --exclude output -zcvf %s ../../Skim" % tarFile)
    os.system("cp runMain.sh tmpSub/")
    print("Created dir: tmpSub")
    submitAll = open("tmpSub/submitAll.sh", "w") 

    # --- Process years and channels ---
    for year, ch in itertools.product(Years, Channels):
        # Append the git version suffix to the output directory path
        outPath = f"{eosSkimDir}/{ch}/{year}_{version_suffix}"
        print(f"Output dir: {outPath}")

        jsonFilePath = f"../input/json/FilesSkim_{ch}_{year}.json"
        if not os.path.exists(jsonFilePath):
            print(f"ERROR: JSON file {jsonFilePath} does not exist.")
            sys.exit(1)

        jsonFile = open(jsonFilePath, "r")
        jdlName  = f'submitJobs_{ch}_{year}.jdl'
        jdlFile  = open(f'tmpSub/{jdlName}', 'w')
        logDir   = "log"
        createJobs(jsonFile, jdlFile, logDir)
        submitAll.write(f'condor_submit {jdlName}\n')

    submitAll.close()
    print("Job submission scripts created in tmpSub/")

