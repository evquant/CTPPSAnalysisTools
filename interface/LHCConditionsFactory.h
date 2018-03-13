#ifndef CTPPSAnalysisTools_Reconstruction_LHCConditionsFactory_h
#define CTPPSAnalysisTools_Reconstruction_LHCConditionsFactory_h

#include "DataFormats/Provenance/interface/EventRange.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

namespace ctpps
{
  struct conditions_t
  {
    edm::EventRange range;
    unsigned int fill;
    std::string time, status;
    struct luminosity_t { double delivered, recorded; };
    luminosity_t luminosity;
    double crossing_angle;
  };
  std::ostream& operator<<( std::ostream& os, const conditions_t& cond ) {
    return os << "range " << cond.range << " (fill " << cond.fill << "):" << std::endl
              << " : time: " << cond.time << " | LHC status: " << cond.status << std::endl
              << " : luminosity: delivered = " << cond.luminosity.delivered << " ub-1, recorded = " << cond.luminosity.recorded << " ub-1" << std::endl
              << " : crossing angle: " << cond.crossing_angle << " urad";
  }

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
          if ( fields.size() == 0 ) continue;
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
          std::vector<std::string> f_lsrange;
          while ( std::getline( ss, field, ':' ) )
            f_lsrange.emplace_back( field );
          if ( f_lsrange.size() != 2 )
            throw std::runtime_error( std::string( "Failed to parse the LS range: " )+fields[lsrange] );
          unsigned int ls_beg = std::stoi( f_lsrange[0] ), ls_end = std::stoi( f_lsrange[1] );
          if ( ls_end == 0 ) ls_end = ls_beg;
          edm::EventRange range( run, ls_beg, 1, run, ls_end, 0 );
          conditions_.emplace_back( conditions_t{ range, fill, fields[time], fields[status], { std::stod( fields[deliv] ), std::stod( fields[reco] ) }, std::stod( fields[xangle] ) } );
        }
      }

      void dump() const {
        for ( const auto& cond : conditions_ )
          std::cout << cond << std::endl;
      }

      conditions_t get( const edm::EventID& ev ) const {
        for ( const auto& cond: conditions_ )
          if ( edm::contains( cond.range, ev ) ) return cond;
        std::ostringstream ev_dmp; ev_dmp << ev;
        throw std::runtime_error( std::string( "Failed to retrieve LHC conditions for event " )+ev_dmp.str() );
      }

    private:
      /// CSV fields
      enum field_names_ { run_fill = 0, lsrange, time, status, deliv, reco, xangle, num_fields };
      std::vector<conditions_t> conditions_;
  };
}

#endif

