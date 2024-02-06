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
mkdir CondorTest
echo "ls $X509_USER_PROXY"
ls $X509_USER_PROXY
mkdir .dasmaps 
mv das_maps_dbs_prod.js .dasmaps/ 

declare -a jobIdsMap=()
cmsRun /eos/home-r/rdalialu/CMSSW_10_6_29/src/flashgg/Taggers/test/CondorTest/BPbHDumper_cfg.py maxEvents=-1 campaign=Era2016_RR-17Jul2028_v2 metaConditions=/eos/home-r/rdalialu/CMSSW_10_6_29/src/flashgg/MetaData/data/MetaConditions/Era2016_RR-17Jul2018_v1.json useAAA=True processIdMap=/eos/home-r/rdalialu/CMSSW_10_6_29/src/flashgg/Taggers/test/CondorTest/config.json dataset=/WplusH_HToGG_WToAll_M125_13TeV_powheg_pythia8 outputFile=CondorTest/output_WplusH_HToGG_WToAll_M125_13TeV_powheg_pythia8.root nJobs=-1 jobId=${jobIdsMap[${1}]} 
retval=$?
if [[ $retval != 0 ]]; then
    retval=$(( ${jobIdsMap[${1}]} + 1 )) 
fi 
if [[ $retval == 0 ]]; then
    errors=""
    for file in $(find -name '*.root' -or -name '*.xml'); do
        echo "cp -pv ${file} /eos/home-r/rdalialu/CMSSW_10_6_29/src/flashgg/Taggers/test/CondorTest"
        cp -pv $file /eos/home-r/rdalialu/CMSSW_10_6_29/src/flashgg/Taggers/test/CondorTest
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

