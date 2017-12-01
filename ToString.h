// from ATLAS MuonCalibStl

#ifndef TOSTRING
#define TOSTRING

#include <sstream>
#include <string>

/**
@class ToString
provides an operator converting a class T object into a std::string.
*/

 class ToString {
  public:
   template< class T >
    std::string operator()( const T& i )
    {
      std::ostringstream os;
      os << i;
      return os.str();
    }
 };

#endif
