// -*- C++ -*-
//
// Package:    CTPPSAnalysisTools/Analyzer
// Class:      TestAnalyzer
// 
/**\class TestAnalyzer TestAnalyzer.cc CTPPSAnalysisTools/Analyzer/plugins/TestAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Laurent Forthomme
//         Created:  Fri, 23 Feb 2018 10:49:41 GMT
//
//

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TestAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>
{
  public:
    explicit TestAnalyzer( const edm::ParameterSet& );
    ~TestAnalyzer() {}

    static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );

  private:
    virtual void beginJob() override;
    virtual void analyze( const edm::Event&, const edm::EventSetup& ) override;
    virtual void endJob() override;
};

TestAnalyzer::TestAnalyzer( const edm::ParameterSet& iConfig )
{
  //usesResource( "TFileService" );
}

// ------------ method called for each event  ------------
void
TestAnalyzer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
}

// ------------ method called once each job just before starting event loop  ------------
void 
TestAnalyzer::beginJob()
{}

// ------------ method called once each job just after ending the event loop  ------------
void 
TestAnalyzer::endJob() 
{}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TestAnalyzer::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault( desc );
}

//define this as a plug-in
DEFINE_FWK_MODULE( TestAnalyzer );

