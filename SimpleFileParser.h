//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 05.12.2013, AUTHOR: OLIVER KORTNER
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef SimpleFileParserH
#define SimpleFileParserH

//::::::::::::::::::::::::::::
//:: CLASS SimpleFileParser ::
//::::::::::::::::::::::::::::

/// \class SimpleFileParser
///
/// This class provides a simple way of parsing a job options file/
///
/// \date 05.12.2013
///
/// \author Oliver.Kortner@CERN.CH

//:::::::::::::::::
//:: HEADER FILE ::
//:::::::::::::::::

// standard C++ //
#include <fstream>
#include <string>
#include <vector>

// my stuff //
#include "JobOption.h"

class SimpleFileParser {

public:
// Constructor //
    SimpleFileParser(void);
    ///< Default constructor.

// Methods //
    const std::vector<const JobOption *> & parse(std::ifstream & infile);
    ///< Parse the input file.
    ///< The return value is a vector of all the job options found in the file.
    ///< A job option looks as follows in the job option file.
    ///< #<job option name> <job option value>

private:
    std::vector<const JobOption *> m_job_option;

};

#endif
