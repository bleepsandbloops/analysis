//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 08.06.2015, AUTHOR: OLIVER KORTNER
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: IMPLEMENTATIONS OF CONSTRUCTORS AND METHODS DEFINED IN THE CLASS ::
//::                     MT_Edge_Time_Measurement                     ::
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//::::::::::::::::::
//:: HEADER FILES ::
//::::::::::::::::::

#include "../include/MT_Edge_Time_Measurement.H"

//::::::::::::::::::::::::
//:: NAMESPACE SETTINGS ::
//::::::::::::::::::::::::

using namespace std;

//*****************************************************************************

//:::::::::::::::::::::::::
//:: DEFAULT CONSTRUCTOR ::
//:::::::::::::::::::::::::

MT_Edge_Time_Measurement::MT_Edge_Time_Measurement(void) {

    m_channel = 0;
    m_time = 0;
    m_is_leading_edge = false;

}

//*****************************************************************************

//:::::::::::::::::
//:: CONSTRUCTOR ::
//:::::::::::::::::

MT_Edge_Time_Measurement::MT_Edge_Time_Measurement(
                                            const unsigned int & channel,
                                            const unsigned int & TDC_count,
                                            bool is_leading_edge) {

    m_channel = channel;
    m_time = TDC_count;
    m_is_leading_edge = is_leading_edge;

}

//*****************************************************************************

//::::::::::::::::::::
//:: METHOD channel ::
//::::::::::::::::::::

const unsigned int MT_Edge_Time_Measurement::channel(void) const {

    return m_channel;

}

//*****************************************************************************

//:::::::::::::::::
//:: METHOD time ::
//:::::::::::::::::

const unsigned int & MT_Edge_Time_Measurement::time(void) const {

    return m_time;

}

//*****************************************************************************

//::::::::::::::::::::::::::
//:: METHOD isLeadingEdge ::
//::::::::::::::::::::::::::

bool MT_Edge_Time_Measurement::isLeadingEdge(void) const {

    return m_is_leading_edge;

}

//*****************************************************************************

//:::::::::::::::::::::::::::
//:: METHOD isTrailingEdge ::
//:::::::::::::::::::::::::::

bool MT_Edge_Time_Measurement::isTrailingEdge(void) const {

    return !m_is_leading_edge;

}

//*****************************************************************************

//::::::::::::::::
//:: OPERATOR < ::
//::::::::::::::::

bool MT_Edge_Time_Measurement::operator < (
                                const MT_Edge_Time_Measurement & edge) const {

    return (m_time<edge.time());

}
