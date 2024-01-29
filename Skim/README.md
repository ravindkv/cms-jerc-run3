## To produce one skim file from NanoAOD
* cd Skim_NanoAOD
* make clean
* make
* voms-proxy-init -voms cms
* cd sample
* python getFiles.py
* cd ..
* source sample/FilesNano_cff.sh
* ./makeNtuple Semilep 2017 SignalSpin12_M800__JEC_SubTotalRelative_up 1of1 . $SignalSpin12_M800_FileList_2017

### to produce many skim files using the condor setup
* cd condor
* python createJdlFiles.py
* cd tmpSub
* source condorSubmit.sh

Monitor the conodr jobs on linux terminal
* condor_q 
* condor_q -better-analyze 75671743.5
* condor_release rverma -name lpcschedd3.fnal.gov
* condor_qedit rverma RequestMemory 3072 -name lpcschedd3.fnal.gov
