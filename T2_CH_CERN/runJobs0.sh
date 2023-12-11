#!/bin/bash
export XRD_NETWORKSTACK=IPv4
export X509_USER_PROXY=/tmp/x509up_u122685
WD=$PWD
echo
echo
echo
cd /eos/home-r/rdalialu/CMSSW_10_6_29
eval $(scram runtime -sh)
cd $WD
mkdir T2_CH_CERN
echo "ls $X509_USER_PROXY"
ls $X509_USER_PROXY
mkdir .dasmaps 
mv das_maps_dbs_prod.js .dasmaps/ 

declare -a jobIdsMap=(-2 -2)
cmsRun /eos/home-r/rdalialu/CMSSW_10_6_29/src/flashgg/T2_CH_CERN/BPbHDumper_cfg.py maxEvents=10 campaign=BprimeBToHB_Hgg_M metaConditions=/eos/home-r/rdalialu/CMSSW_10_6_29/src/flashgg/MetaData/data/MetaConditions/Era2016_RR-17Jul2018_v1.json useAAA=True processIdMap=/eos/home-r/rdalialu/CMSSW_10_6_29/src/flashgg/T2_CH_CERN/config.json dataset=/BprimeBToHB_Hgg_M-1000_TuneCP5_13TeV-madgraph-pythia8 outputFile=T2_CH_CERN//store/group/phys_b2g/rdalialu_BprimeBToHB_Hgg_M-1000_TuneCP5_13TeV-madgraph-pythia8.root nJobs=-1 jobId=${jobIdsMap[${1}]}  lastAttempt=1
retval=$?
if [[ $retval != 0 ]]; then
    retval=$(( ${jobIdsMap[${1}]} + 1 )) 
fi 
if [[ $retval == 0 ]]; then
    errors=""
    for file in $(find -name '*.root' -or -name '*.xml'); do
        echo "cp -pv ${file} /eos/home-r/rdalialu/CMSSW_10_6_29/src/flashgg/T2_CH_CERN"
        cp -pv $file /eos/home-r/rdalialu/CMSSW_10_6_29/src/flashgg/T2_CH_CERN
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

