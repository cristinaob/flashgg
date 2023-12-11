#!/bin/bash
export XRD_NETWORKSTACK=IPv4
export X509_USER_PROXY=/afs/cern.ch/user/r/rdalialu/x509up_u122685
WD=$PWD
echo
echo
echo
cd /eos/home-r/rdalialu/CMSSW_10_6_29
eval $(scram runtime -sh)
cd $WD
mkdir /afs/cern.ch/user/r/rdalialu/eos/CMSSW_10_6_29/src/flashgg/CondorTest
echo "ls $X509_USER_PROXY"
ls $X509_USER_PROXY
mkdir .dasmaps 
mv das_maps_dbs_prod.js .dasmaps/ 

declare -a jobIdsMap=(-2 -2)
cmsRun /afs/cern.ch/user/r/rdalialu/eos/CMSSW_10_6_29/src/flashgg/CondorTest/BPbHDumper_cfg.py maxEvents=-1 campaign=BprimeBToHB_Hgg_M metaConditions=/eos/home-r/rdalialu/CMSSW_10_6_29/src/flashgg/MetaData/data/MetaConditions/Era2016_RR-17Jul2018_v1.json useAAA=True processIdMap=/afs/cern.ch/user/r/rdalialu/eos/CMSSW_10_6_29/src/flashgg/CondorTest/config.json dataset=/BprimeBToHB_Hgg_M-1000_TuneCP5_13TeV-madgraph-pythia8 outputFile=/afs/cern.ch/user/r/rdalialu/eos/CMSSW_10_6_29/src/flashgg/CondorTest/output_BprimeBToHB_Hgg_M-1000_TuneCP5_13TeV-madgraph-pythia8.root nJobs=-1 jobId=${jobIdsMap[${1}]}  lastAttempt=1
retval=$?
if [[ $retval != 0 ]]; then
    retval=$(( ${jobIdsMap[${1}]} + 1 )) 
fi 
if [[ $retval == 0 ]]; then
    errors=""
    for file in $(find -name '*.root' -or -name '*.xml'); do
        echo "cp -pv ${file} /afs/cern.ch/user/r/rdalialu/eos/CMSSW_10_6_29/src/flashgg/CondorTest"
        cp -pv $file /afs/cern.ch/user/r/rdalialu/eos/CMSSW_10_6_29/src/flashgg/CondorTest
        if [[ $? != 0 ]]; then
            errors="$errors $file($?)"
        fi
    done
    if [[ -n "$errors" ]]; then
       echo "Errors while staging files"
       echo "$errors"
       exit -2
    fi
fi

exit $retval

