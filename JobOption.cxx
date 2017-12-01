//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 05.12.2013, AUTHOR: OLIVER KORTNER
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: IMPLEMENTATION OF CONSTRUCTORS AND METHODS DEFINED IN THE CLASS ::
//::                              JobOption                          ::
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//::::::::::::::::::
//:: HEADER FILES ::
//::::::::::::::::::

// standard C //
#include <cstdlib>

// my stuff //
#include "../include/JobOption.h"

//::::::::::::::::::::::::
//:: NAMESPACE SETTINGS ::
//::::::::::::::::::::::::

using namespace std;

//*****************************************************************************

//:::::::::::::::::::::::::
//:: DEFAULT CONSTRUCTOR ::
//:::::::::::::::::::::::::

JobOption::JobOption(void) {

    m_name.clear();
    m_value.clear();

}

//*****************************************************************************

//:::::::::::::::::
//:: CONSTRUCTOR ::
//:::::::::::::::::

JobOption::JobOption(const std::string & name, const std::string & value) {

    m_name = name;
    m_value = value;

}

//*****************************************************************************

//::::::::::::::::::::
//:: METHOD getName ::
//::::::::::::::::::::

const std::string & JobOption::getName(void) const {

    return m_name;

}

//*****************************************************************************

//:::::::::::::::::::::
//:: METHOD getValue ::
//:::::::::::::::::::::

const std::string & JobOption::getValue(void) const {

    return m_value;

}

//*****************************************************************************

//::::::::::::::::::::::::::::
//:: METHOD getIntegerValue ::
//::::::::::::::::::::::::::::

int JobOption::getIntegerValue(void) const {

    return atoi(m_value.c_str());

}

//*****************************************************************************

//:::::::::::::::::::::::::::
//:: METHOD getDoubleValue ::
//:::::::::::::::::::::::::::

double JobOption::getDoubleValue(void) const {

    return atof(m_value.c_str());

}
