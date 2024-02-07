#!/bin/bash
#To be run on remote machine
#Take input arguments as an array
myArray=( "$@" )
#Array: Size=$#, an element=$1, all element = $@

printf "Start skimming at ";/bin/date
printf "Worker node hostname ";/bin/hostname
printf "Worker node OS " ; lsb_release -d

if [ -z ${_CONDOR_SCRATCH_DIR} ] ; then 
    echo "Running Interactively" ; 
else
    echo "Running In Batch"
    echo ${_CONDOR_SCRATCH_DIR}
    source /cvmfs/cms.cern.ch/cmsset_default.sh
    export SCRAM_ARCH=slc7_amd64_gcc700
    scramv1 project CMSSW CMSSW_12_6_0
    cd CMSSW_12_6_0/src
    eval `scramv1 runtime -sh`
    cd ../..
	tar --strip-components=1 -zxf Skim.tar.gz
fi

#Run for Base, Signal region
echo "All arguements: "$@
echo "Number of arguements: "$#
year=$1
sample=$2
nthJob=$3
totJobs=$4
outDir=$5
iName=${sample}_${year}
oName=${sample}__${year}_Skim
echo "./makeSkim -y ${year} -n ${nthJob} -N ${totJobs} -o ${oName} -i ${iName}" 
./makeSkim -y ${year} -n ${nthJob} -N ${totJobs} -o ${oName} -i ${iName} 

printf "Done skimming at ";/bin/date
#---------------------------------------------
#Copy the ouput root files
#---------------------------------------------
if [ -z ${_CONDOR_SCRATCH_DIR} ] ; then
    echo "Running Interactively" ;
else
    xrdcp -f ${oName}.root ${outDir}
    echo "Cleanup"
    rm -rf CMSSW*
    rm *.root 
fi
printf "Done ";/bin/date
