#ifndef CTPPSAnalysisTools_Alignment_AlignmentsFactory_h
#define CTPPSAnalysisTools_Alignment_AlignmentsFactory_h

#include "DataFormats/Provenance/interface/EventRange.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <vector>

namespace ctpps
{
  struct alignment_t
  {
    double x_align, x_align_err;
    double y_align, y_align_err;
  };
  std::ostream& operator<<( std::ostream& os, const alignment_t al ) {
    return os << "{x=" << al.x_align << "+/-" << al.x_align_err << "; y=" << al.y_align << "+/-" << al.y_align_err << "}";
  }
  class AlignmentsFactory
  {
    public:
      AlignmentsFactory() {}
      ~AlignmentsFactory() {}

      void feedAlignments( const char* filename ) {
        std::ifstream file( filename );
        if ( !file.is_open() )
          throw std::runtime_error( std::string( "Failed to open ")+filename );
        std::string line;
        while ( !file.eof() ) {
          std::getline( file, line );
          if ( line[0] == '#' ) continue; // strip comments
          // parse all fields
          std::istringstream ss( line );
          std::string field;
          std::vector<std::string> fields;
          while ( std::getline( ss, field, ',' ) )
            fields.emplace_back( field );
          if ( fields.size() == 0 ) continue;
          if ( fields.size() != num_fields )
            throw std::runtime_error( std::string( "Failed to parse the following line:\n\t" )+line );
          unsigned int run_beg = std::stoi( fields[runbeg] ), run_end = std::stoi( fields[runend] );
          unsigned short pot = std::stoi( fields[rpot] );
          double alignment = std::stod( fields[align] );
          alignments_.emplace_back(
            runbased_alignments_t{ edm::EventRange( run_beg, 1, 1, run_end, edm::EventID::maxLuminosityBlockNumber(), edm::EventID::maxEventNumber() ), pot, alignment_t{ alignment, 0., 0., 0. } }
          );
        }
      }

      const alignment_t& get( const edm::EventID& ev, unsigned short pot ) const {
        for ( const auto& align : alignments_ ) {
          if ( align.pot != pot ) continue;
          if ( edm::contains( align.range, ev ) ) return align.alignments;
        }
        std::ostringstream ev_dmp; ev_dmp << ev;
        throw std::runtime_error( std::string( "Failed to retrieve alignment parameters in pot ")+std::to_string( pot )+std::string( " for event " )+ev_dmp.str() );
      }

    private:
      enum field_names_ { runbeg = 0, runend, rpot, align, num_fields };
      struct runbased_alignments_t
      {
        edm::EventRange range;
        unsigned short pot;
        alignment_t alignments;
      };
      std::vector<runbased_alignments_t> alignments_;
  };
}

#endif

