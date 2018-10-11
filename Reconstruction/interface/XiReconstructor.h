#ifndef CTPPSAnalysisTools_Reconstruction_xi_reconstruction_h
#define CTPPSAnalysisTools_Reconstruction_xi_reconstruction_h

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <map>

namespace ctpps
{
  struct dispersion_t
  {
    dispersion_t( double value = 0., double error = 0. ) :
      value( value ), err_value( error ) {}
    double value, err_value;
  };
  typedef std::map<unsigned short,dispersion_t> dispersions_t;
  class XiReconstructor
  {
    public:
      XiReconstructor() {}
      ~XiReconstructor() {}

      void interpolateCrossingAngle( double xangle, double scaling ) {
        if ( disp_vs_xa_.count( xangle ) > 0 ) {
          std::cerr << "Warning: Crossing angle " << xangle << " already present in the database.";
          return;
        }
        if ( xangle < disp_vs_xa_.begin()->first )
          throw std::runtime_error( std::string( "Crossing angle of " )+std::to_string( xangle )+std::string( " lower than minimal one!" ) );
        // we may interpolate if crossing angle higher than maximum stored one
        const double highest_xangle = disp_vs_xa_.rbegin()->first;
        std::cerr << "Added dispersion for xangle=" << xangle << " urad:\n";
        for ( const auto& pot_disp : disp_vs_xa_.rbegin()->second ) {
          disp_vs_xa_[xangle][pot_disp.first] = dispersion_t{
            pot_disp.second.value+scaling*( ( xangle-highest_xangle ) ),
            pot_disp.second.err_value+scaling*( ( xangle-highest_xangle ) ) //FIXME
          };
          std::cerr << ">> RP" << pot_disp.first << ": "
            << disp_vs_xa_[xangle][pot_disp.first].value << " +/- "
            << disp_vs_xa_[xangle][pot_disp.first].err_value << "\n";
        }
      }
      void reconstruct( double xangle, unsigned short pot, double x_aligned, double& xi, double& xi_err ) const {
        if ( disp_vs_xa_.count( xangle ) == 0 )
          throw std::runtime_error( std::string( "Failed to retrieve the dispersion values for this crossing-angle: ")+std::to_string( xangle ) );
        const auto& disp_vs_pot = disp_vs_xa_.at( xangle );
        if ( disp_vs_pot.count( pot ) == 0 )
          throw std::runtime_error( std::string( "Failed to retrieve the dispersion values for this pot: ")+std::to_string( pot ) );

        const dispersion_t disp = disp_vs_pot.at( pot ); // in cm
        xi = x_aligned/( -disp.value );
        xi_err = std::hypot( align_err_/disp.value, disp.err_value*xi );
      }

      void feedDispersions( const char* filename ) {
        std::ifstream file( filename );
        if ( !file.is_open() )
          throw std::runtime_error( std::string( "Failed to open ")+filename );
        std::string line;
        while ( !file.eof() ) {
          std::getline( file, line );
          if ( line[0] == '#' ) continue; // strip comments
          std::istringstream ss( line );
          std::string s_xangle, s_pot, s_disp, s_err_disp;
          while ( ss >> s_xangle >> s_pot >> s_disp >> s_err_disp ) {
            const float xangle = std::stod( s_xangle );
            const unsigned short pot = std::stoi( s_pot );
            const double disp = std::stod( s_disp ), err_disp = std::stod( s_err_disp );
            disp_vs_xa_[xangle][pot] = dispersion_t( disp, err_disp );
          }
        }
      }
      void dumpDispersions() const {
        for ( const auto& dxa : disp_vs_xa_ ) {
          std::cout << "crossing-angle: " << dxa.first << " urad" << std::endl;
          for ( const auto& dp : dxa.second ) {
            std::cout << "  pot " << dp.first << ": "
                      << "Dx = " << dp.second.value
                      << " +/- " << dp.second.err_value << " cm"
                      << std::endl;
          }
        }
      }
      const std::map<float,dispersions_t>& dispersions() const { return disp_vs_xa_; }

    private:
      static constexpr double align_err_ = 150.e-4; // in cm
      std::map<float,dispersions_t> disp_vs_xa_;
  };
}

#endif

