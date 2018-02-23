#include "CTPPSAnalysisTools/Reconstruction/interface/LHCConditionsFactory.h"
#include "CTPPSAnalysisTools/Reconstruction/interface/XiReconstructor.h"

void test_reco()
{
  //----- crossing angle (+ other useful LHC information) tools

  ctpps::LHCConditionsFactory cond_fac;
  ostringstream cond_file1_path, cond_file2_path;
  cond_file1_path << getenv( "CMSSW_BASE" ) << "/src/CTPPSAnalysisTools/Reconstruction/data/2017/xangle_tillTS2.csv";
  cond_file2_path << getenv( "CMSSW_BASE" ) << "/src/CTPPSAnalysisTools/Reconstruction/data/2017/xangle_afterTS2.csv";
  cond_fac.feedConditions( cond_file1_path.str().c_str() );
  cond_fac.feedConditions( cond_file2_path.str().c_str() );
  //cond_fac.dump(); // a LOT of printout!
  // try to retrieve an event outside the supported range
  try {
    cout << "Invalid event test... ";
    const auto cond = cond_fac.get( edm::EventID( 10000, 1, 1 ) );
    cout << "failed!" << endl;
  } catch ( runtime_error ) { cout << "OK!" << endl; }
  // try to retrieve the crossing angle for a given event number
  try {
    cout << "Valid event test... ";
    const auto cond = cond_fac.get( edm::EventID( 299084, 163, 99999/*whatever*/ ) );
    if ( cond.crossing_angle != 150. )
      throw runtime_error( "incorrect xing-angle retrieved!" );
    cout << "OK!" << endl;
  } catch ( runtime_error ) { cout << "failed!" << endl; }

  //----- x-to-xi reconstruction tools

  ctpps::XiReconstructor reco;
  ostringstream disp_file_path;
  disp_file_path << getenv( "CMSSW_BASE" ) << "/src/CTPPSAnalysisTools/Reconstruction/data/2017/dispersions.txt";
  reco.feedDispersions( disp_file_path.str().c_str() );
  //reco.dumpDispersions();

  double xi = 0., xi_err = 0.;
  // try to retrieve a wrong xing angle
  try {
    cout << "Invalid xing angle test... ";
    reco.reconstruct( 200., 2, 0., xi, xi_err );
    cout << "failed!" << endl;
  } catch ( runtime_error ) { cout << "OK!" << endl; }
  // try to retrieve a wrong pot
  try {
    cout << "Invalid pot test... ";
    reco.reconstruct( 130., 4, 0., xi, xi_err );
    cout << "failed!" << endl;
  } catch ( runtime_error ) { cout << "OK!" << endl; }
}
