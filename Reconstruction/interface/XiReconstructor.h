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

      void reconstruct( double xangle, unsigned short pot, double x_aligned, double& xi, double& xi_err ) const {
        if ( disp_vs_xa_.count( xangle ) == 0 )
          throw std::runtime_error( std::string( "Failed to retrieve the dispersion values for this crossing-angle: ")+std::to_string( xangle ) );
        const auto& disp_vs_pot = disp_vs_xa_.at( xangle );
        if ( disp_vs_pot.count( pot ) == 0 )
          throw std::runtime_error( std::string( "Failed to retrieve the dispersion values for this pot: ")+std::to_string( pot ) );

        const double align_err = 150.e-4; // in cm
        const dispersion_t disp = disp_vs_pot.at( pot ); // in cm
        xi = x_aligned/( -disp.value );
        xi_err = sqrt( pow( align_err/disp.value, 2 )+pow( disp.err_value*xi, 2 ) );
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

    private:
      std::map<float,dispersions_t> disp_vs_xa_;
  };
}

#endif

