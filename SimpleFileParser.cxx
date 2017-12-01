//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 05.12.2013, AUTHOR: OLIVER KORTNER
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: IMPLEMENTATION OF CONSTRUCTORS AND METHODS DEFINED IN THE CLASS ::
//::                         SimpleFileParser                        ::
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//::::::::::::::::::
//:: HEADER FILES ::
//::::::::::::::::::

// standard C++ //
#include <iostream>

// my stuff //
#include "../include/SimpleFileParser.h"

//::::::::::::::::::::::::
//:: NAMESPACE SETTINGS ::
//::::::::::::::::::::::::

using namespace std;

//*****************************************************************************

//:::::::::::::::::
//:: CONSTRUCTOR ::
//:::::::::::::::::

SimpleFileParser::SimpleFileParser(void) {

    m_job_option.clear();

}

//*****************************************************************************

//::::::::::::::::::
//:: METHOD parse ::
//::::::::::::::::::

const std::vector<const JobOption *> & SimpleFileParser::parse(
                                                    std::ifstream & infile) {

///////////////
// VARIABLES //
///////////////

    string sdummy;
    string value;

///////////
// CLEAR //
///////////

    for (unsigned int k=0; k<m_job_option.size(); k++) {
        if (m_job_option[k]!=0) {
            delete m_job_option[k];
        }
    }
    m_job_option.clear();

/////////////////////////
// READ THE INPUT FILE //
/////////////////////////

    while (!infile.eof()) {
        infile >> sdummy;
        if (infile.eof()) {
            break;
        }
        if (sdummy[0]=='#') {
            infile >> value;
            m_job_option.push_back(new JobOption(sdummy, value));
        }
    }

    return m_job_option;

}
