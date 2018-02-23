import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source('PoolSource',
    fileNames = cms.untracked.vstring(
        '/store/data/Run2017D/SingleMuon/MINIAOD/17Nov2017-v1/40000/0CABDFD5-48DE-E711-8289-3417EBE649DE.root',
    )
)

process.load('CTPPSAnalysisTools.Analyzer.testAnalyzer_cfi')
process.testAnalyzer.potsConsidered = cms.vuint32(
    3, 103, # strips far detectors (210m)
    23, 123 # pixels far detectors (220m)
)

# prepare the output file
process.TFileService = cms.Service('TFileService',
    fileName = cms.string('output.root'),
    closeFileFast = cms.untracked.bool(True),
)

process.p = cms.Path(process.testAnalyzer)

