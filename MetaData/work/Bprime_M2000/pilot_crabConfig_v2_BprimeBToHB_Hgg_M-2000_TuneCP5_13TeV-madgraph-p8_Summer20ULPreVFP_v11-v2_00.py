# CRAB3 config template for flashgg
# More options available on the twiki :
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookCRAB3Tutorial
# To actually prepare the jobs, please execute prepareCrabJobs.py

from WMCore.Configuration import Configuration
config = Configuration()
import os

config.section_("General")
config.General.requestName = "v2_BprimeBToHB_Hgg_M-2000_TuneCP5_13TeV-madgraph-p8_Summer20ULPreVFP_v11-v2_00"
config.General.transferLogs = False

config.section_("JobType")
config.JobType.pluginName = "Analysis"
config.JobType.psetName = "microAODstd.py"

## to include local file in the sendbox, this will put the file in the directory where cmsRun runs
config.JobType.inputFiles = ['QGL_AK4chs_94X.db']

## incrase jobs time wall, maximum 2750 minutes (~46 hours)
#config.JobType.maxJobRuntimeMin = 2750

#config.JobType.maxMemoryMB = 2500 # For memory leaks. NB. will block jobs on many sites
## config.JobType.scriptExe = "cmsWrapper.sh"
config.JobType.pyCfgParams = ['datasetName=/BprimeBToHB_Hgg_M-2000_TuneCP5_13TeV-madgraph-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM', 'processType=background', 'conditionsJSON=/afs/cern.ch/user/r/rdalialu/eos/CMSSW_10_6_29/src/flashgg/MetaData/data/MetaConditions/Era2016_RR-17Jul2018_v1.json']
config.JobType.sendPythonFolder = True
config.JobType.sendExternalFolder = True

config.section_("Data")
config.Data.inputDataset = "/BprimeBToHB_Hgg_M-2000_TuneCP5_13TeV-madgraph-pythia8/RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2/MINIAODSIM"
config.Data.inputDBS = 'global'
config.Data.splitting = "EventAwareLumiBased"
config.Data.unitsPerJob = 25000
config.Data.publication = True
config.Data.publishDBS = 'phys03'
config.Data.outputDatasetTag = 'Bprime_M2000-v2-v0-RunIISummer20UL16MiniAODAPVv2-106X_mcRun2_asymptotic_preVFP_v11-v2'
config.Data.outLFNDirBase = "/store/group/phys_b2g/rdalialu/Bprime_M2000/v2"

config.section_("Site")
config.Site.storageSite = "T2_CH_CERN"
config.Site.blacklist = ["T2_US_Nebraska"]
#config.Site.blacklist = ["T2_UK_London_Brunel","T1_US_FNAL","T2_US_MIT"]

config.Data.totalUnits = 25000
config.Data.publication = False
config.General.requestName = "pilot_v2_BprimeBToHB_Hgg_M-2000_TuneCP5_13TeV-madgraph-p8_Summer20ULPreVFP_v11-v2_00"
