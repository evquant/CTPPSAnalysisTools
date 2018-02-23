#ifndef CTPPSAnalysisTools_Reconstruction_LHCConditionsFactory_h
#define CTPPSAnalysisTools_Reconstruction_LHCConditionsFactory_h

#include "DataFormats/Provenance/interface/EventRange.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace ctpps
{
  class LHCConditionsFactory
  {
    public:
      LHCConditionsFactory() {}
      ~LHCConditionsFactory() {}

      void feedConditions( const char* filename ) {
        std::ifstream file( filename );
        if ( !file.is_open() )
          throw std::runtime_error( std::string( "Failed to open ")+filename );
        std::string line;
        while ( !file.eof() ) {
          std::getline( file, line );
          if ( line[0] == '#' ) continue; // strip comments
          // parse all CSV file fields
          std::istringstream ss( line );
          std::string field;
          std::vector<std::string> fields;
          while ( std::getline( ss, field, ',' ) )
            fields.emplace_back( field );
          if ( fields.size() != num_fields )
            throw std::runtime_error( std::string( "Failed to parse the following line:\n\t" )+line );
          // parse the run <-> fill association map
          std::vector<std::string> f_run_fill;
          ss = std::istringstream( fields[run_fill] );
          while ( std::getline( ss, field, ':' ) )
            f_run_fill.emplace_back( field );
          if ( f_run_fill.size() != 2 )
            throw std::runtime_error( std::string( "Failed to parse the CMS run/LHC fill correspondance string: ")+fields[run_fill] );
          const unsigned int run = std::stoi( f_run_fill[0] ), fill = std::stoi( f_run_fill[1] );
          // parse the LS range
          ss = std::istringstream( fields[lsrange] );
          std::cout << run << "//" << fill << "\t" << fields[xangle] << std::endl;
        }
      }

      void dump() const {
      }

    private:
      /// CSV fields
      enum field_names_ { run_fill = 0, lsrange, time, status, deliv, reco, xangle, num_fields };
      struct lsrange_t
      {
        lsrange_t( unsigned int begin = 0, unsigned int end = 0 ) :
          begin( begin ), end( end ) {}
        operator<( const lsrange_t& rhs ) const {
          if ( begin < rhs.begin ) return true;
          if ( begin == rhs.begin && end < rhs.end ) return true;
          return false;
        }
        unsigned int begin, end;
      };
      struct conditions_t
      {
      };
      std::map<lsrange_t,conditions_t> cond_map_;
  };
}

#endif

