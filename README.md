# Analysis tools for CTPPS

This repository provides a collection of useful tools for the analysis of 2017+ data collected using the CTPPS detectors.

## Usage

### As a package in a `CMSSW` environment

This tool may be used as a standard `CMSSW` package (e.g. as a set of useful object-oriented helpers used in an analysis workflow's `EDAnalyzer`).

In your favourite `${CMSSW_BASE}/src` environment, just clone this repository through:

```.sh
git clone https://github.com/forthommel/CTPPSAnalysisTools.git
```

This collection of tools is supposingly compatible with all `CMSSW` releases (including pre-`7_X_Y` releases). In case this statement is outdated, feel free to open an issue/PR!

### In a standalone way

Additionally, this repository ships a set of tools to run in a simpler workflow, e.g. through a conventional ROOT macro.

The recipe is equivalent to the previous case: in your development area, clone the repository:

```.sh
git clone https://github.com/forthommel/CTPPSAnalysisTools.git
```

Then, you may use the helpers inside your ROOT macro as in [this alignment retrieval](https://github.com/forthommel/CTPPSAnalysisTools/blob/master/Alignment/test/test_alignment.C) and [this reconstruction](https://github.com/forthommel/CTPPSAnalysisTools/blob/master/Reconstruction/test/test_reco.C) examples.
