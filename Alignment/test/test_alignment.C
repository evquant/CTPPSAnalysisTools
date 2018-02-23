#include "CTPPSAnalysisTools/Alignment/interface/AlignmentsFactory.h"

void test_alignment()
{
  //----- x alignment tools

  ctpps::AlignmentsFactory align_fac;
  ostringstream align_file_path;
  align_file_path << getenv( "CMSSW_BASE" ) << "/src/CTPPSAnalysisTools/Alignment/data/2017/alignments_30jan2017.txt";
  align_fac.feedAlignments( align_file_path.str().c_str() );
  //reco.dumpDispersions();

  // try to retrieve a wrong run period angle
  try {
    cout << "Invalid run period test... ";
    const ctpps::alignment_t align = align_fac.get( edm::EventID( 10000, 1, 999999 ), 103 );
    cout << "failed!" << endl;
  } catch ( runtime_error ) { cout << "OK!" << endl; }
  // try to retrieve a wrong pot
  try {
    cout << "Invalid pot test... ";
    const ctpps::alignment_t align = align_fac.get( edm::EventID( 300000, 1, 99999 ), 1 );
    cout << "failed!" << endl;
  } catch ( runtime_error ) { cout << "OK!" << endl; }
}
