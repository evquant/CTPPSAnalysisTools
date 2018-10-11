import FWCore.ParameterSet.Config as cms

testAnalyzer = cms.EDAnalyzer('TestAnalyzer',
    ctppsTracksTag = cms.InputTag('ctppsLocalTrackLiteProducer'),
    potsConsidered = cms.vuint32(), # list of pots we are interested in
    # alignment/dispersion lookup tables
    ctppsAlignmentsFile = cms.FileInPath('CTPPSAnalysisTools/Alignment/data/2017/alignments_21aug2018.txt'),
    lhcConditionsFile = cms.FileInPath('CTPPSAnalysisTools/Reconstruction/data/2017/xangle_tillTS2.csv'),
    ctppsDispersionsFile = cms.FileInPath('CTPPSAnalysisTools/Reconstruction/data/2017/dispersions.txt'),
)
