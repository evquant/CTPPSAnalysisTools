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
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"

#include "../interface/AlignmentsFactory.h"
#include "../interface/LHCConditionsFactory.h"
#include "../interface/XiReconstructor.h"

#include "TH1D.h"
#include "TH2D.h"

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

    edm::EDGetTokenT<std::vector<CTPPSLocalTrackLite> > ctppsTracksToken_;
    std::vector<unsigned int> rps_considered_;

    ctpps::AlignmentsFactory aligns_;
    ctpps::LHCConditionsFactory lhc_conds_;
    ctpps::XiReconstructor xi_reco_;

    std::map<unsigned short,TH2D*> h2_hitmaps_;
    std::map<unsigned short,TH1D*> h_xi_;
};

TestAnalyzer::TestAnalyzer( const edm::ParameterSet& iConfig ) :
  ctppsTracksToken_( consumes<std::vector<CTPPSLocalTrackLite> >( iConfig.getParameter<edm::InputTag>( "ctppsTracksTag" ) ) ),
  rps_considered_( iConfig.getParameter<std::vector<unsigned int> >( "potsConsidered" ) ) // TOTEM decimal notation!
{
  // retrieve all LUTs
  if ( iConfig.existsAs<edm::FileInPath>( "ctppsAlignmentsFile" ) )
    aligns_.feedAlignments( iConfig.getParameter<edm::FileInPath>( "ctppsAlignmentsFile" ).fullPath().c_str() );
  if ( iConfig.existsAs<edm::FileInPath>( "lhcConditionsFile" ) )
    lhc_conds_.feedConditions( iConfig.getParameter<edm::FileInPath>( "lhcConditionsFile" ).fullPath().c_str() );
  if ( iConfig.existsAs<edm::FileInPath>( "ctppsDispersionsFile" ) )
    xi_reco_.feedDispersions( iConfig.getParameter<edm::FileInPath>( "ctppsDispersionsFile" ).fullPath().c_str() );

  usesResource( "TFileService" );
  edm::Service<TFileService> fs;

  for ( const auto& rp : rps_considered_ ) {
    TFileDirectory pot_dir = fs->mkdir( Form( "pot_%d", rp ) );
    h2_hitmaps_[rp] = pot_dir.make<TH2D>( Form( "hitmap_%d", rp ), Form( "Pot %d;x (cm);y (cm)", rp ), 100, 0., 5., 100, -2.5, 2.5 );
    h_xi_[rp] = pot_dir.make<TH1D>( Form( "xi_%d", rp ), Form( "Pot %d;#xi_{track};Events", rp ), 100, 0., 0.2 );
  }
}

// ------------ method called for each event  ------------
void
TestAnalyzer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  const edm::EventID ev_id = iEvent.id();

  // LHC information retrieval from LUT
  const ctpps::conditions_t lhc_cond = lhc_conds_.get( ev_id );
  const double xangle = lhc_cond.crossing_angle;

  // CTPPS tracks retrieval
  // /!\ all tracks positions expressed in mm!

  edm::Handle<std::vector<CTPPSLocalTrackLite> > rpTracks;
  iEvent.getByToken( ctppsTracksToken_, rpTracks );
  for ( const auto& trk : *rpTracks ) {
    const CTPPSDetId detid( trk.getRPId() );

    // transform the raw, 32-bit unsigned integer detId into the TOTEM "decimal" notation
    const unsigned short raw_id = 100*detid.arm()+10*detid.station()+detid.rp();

    //if ( detid.station() == 1 ) continue; // skip the diamonds/UFSD for the moment
    if ( std::find( rps_considered_.begin(), rps_considered_.end(), raw_id ) == rps_considered_.end() )
      continue; // only keep the pots we are interested in

    const ctpps::alignment_t align = aligns_.get( ev_id, raw_id );
    const double trk_x_corr = ( trk.getX()/10.+align.x_align );
    h2_hitmaps_[raw_id]->Fill( trk_x_corr, trk.getY()/10. ); // expressed in cm

    double xi = 0., xi_err = 0.;
    try {
      xi_reco_.reconstruct( xangle, raw_id, trk_x_corr, xi, xi_err );
      h_xi_[raw_id]->Fill( xi );
    } catch ( std::runtime_error ) {
      // avoids to break the processing if a xing angle value was not found
      // (additional ones will be added as soon as they are available)
      continue;
    }
  }
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

