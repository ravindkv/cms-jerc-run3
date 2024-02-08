# cms-jerc-run3
Jet energy and resolution correction for Run3 and Phase 2

## Basic setups for EL9
* ssh rverma@lxplus9.cern.ch
* export SCRAM_ARCH=el9_amd64_gcc10
* cmsrel CMSSW_13_3_0
* cd CMSSW_13_3_0/src
* cmsenv
* git clone git@github.com:ravindkv/cms-jerc-run3.git
* voms-proxy-init --voms cms --valid 24:00
