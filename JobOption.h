//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 05.12.2013, AUTHOR: OLIVER KORTNER
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef JobOptionH
#define JobOptionH

//:::::::::::::::::::::
//:: CLASS JobOption ::
//:::::::::::::::::::::

/// \class JobOption
///
/// Auxiliary class for the SimpleFileParser. The JobOption is essentially
/// a pair of STL strings.
///
/// \cdate 05.12.2013
///
/// \author Oliver.Kortner@CERN.CH

//:::::::::::::::::
//:: HEADER FILE ::
//:::::::::::::::::

// standard C++ //
#include <string>

class JobOption {

public:
// Constructor //
    JobOption(void);
    ///< Default constructor.
    ///< An empty job option is created.

    JobOption(const std::string & name, const std::string & value);
    ///< Constructor.
    ///< \param name    Name of the job option entry.
    ///< \param value   Value of the job option entry.

// Methods //
    const std::string & getName(void) const;
    ///< Get the name of the job option entry.

    const std::string & getValue(void) const;
    ///< Get the value of the job option entry.

    int getIntegerValue(void) const;
    ///< Get the job option value string converted into an integer.
    ///< Supports conversion of 0x... into integers.

    double getDoubleValue(void) const;
    ///< Get the job option value string converted into a double.

private:
    std::string m_name;
    std::string m_value;

};

#endif
