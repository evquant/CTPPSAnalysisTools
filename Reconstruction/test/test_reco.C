#include "CTPPSAnalysisTools/Reconstruction/interface/LHCConditionsFactory.h"
#include "CTPPSAnalysisTools/Reconstruction/interface/XiReconstructor.h"

void test_reco()
{
  ctpps::LHCConditionsFactory cond;
  ostringstream cond_file1_path, cond_file2_path;
  cond_file1_path << getenv( "CMSSW_BASE" ) << "/src/CTPPSAnalysisTools/Reconstruction/data/2017/xangle_tillTS2.csv";
  cond_file2_path << getenv( "CMSSW_BASE" ) << "/src/CTPPSAnalysisTools/Reconstruction/data/2017/xangle_afterTS2.csv";
  cond.feedConditions( cond_file1_path.str().c_str() );
  cond.feedConditions( cond_file2_path.str().c_str() );
  cond.dump();

  ctpps::XiReconstructor reco;
  ostringstream disp_file_path;
  disp_file_path << getenv( "CMSSW_BASE" ) << "/src/CTPPSAnalysisTools/Reconstruction/data/2017/dispersions.txt";
  reco.feedDispersions( disp_file_path.str().c_str() );
  reco.dumpDispersions();

  double xi = 0., xi_err = 0.;
  // try to retrieve a wrong xing angle
  try {
    cout << "Wrong xing angle test... ";
    reco.reconstruct( 200., 2, 0., xi, xi_err );
    cout << "failed!" << endl;
  } catch ( std::runtime_error ) { cout << "OK!" << endl; }
  // try to retrieve a wrong pot
  try {
    cout << "Wrong pot test... ";
    reco.reconstruct( 130., 4, 0., xi, xi_err );
    cout << "failed!" << endl;
  } catch ( std::runtime_error ) { cout << "OK!" << endl; }
}
