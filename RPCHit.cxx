//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 20.11.2017, AUTHOR: OLIVER KORTNER
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: IMPLEMENTATION OF CONSTRUCTORS AND METHODS DEFINED IN THE CLASS RPCHit ::
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//:::::::::::::::::
//:: HEADER FILE ::
//:::::::::::::::::

#include <iostream>
#include "../include/RPCHit.h"

//*****************************************************************************

//:::::::::::::::::
//:: CONSTRUCTOR ::
//:::::::::::::::::

RPCHit::RPCHit(const unsigned int & singlet_ID,
               const unsigned int & strip_ID,
               const unsigned int & strip_type,
               const float & hit_leading_egde_time,
               const float & hit_trailing_egde_time) {

    m_singlet_ID = singlet_ID;
    m_strip_ID = strip_ID;
    m_strip_type = strip_type;
    m_hit_leading_egde_time = hit_leading_egde_time;

    if (hit_trailing_egde_time<m_hit_leading_egde_time) {
        m_hit_trailing_egde_time = m_hit_leading_egde_time;
        std::cerr << "Class RPCHit, constructor. WARNING!" << std::endl
                  << "Trailing edge before leading edge!" << std::endl;
    }
    m_hit_trailing_egde_time = hit_trailing_egde_time;

}

//*****************************************************************************

//::::::::::::::::::::::::
//:: METHOD getSingleID ::
//::::::::::::::::::::::::

unsigned int RPCHit::getSingleID(void) const {

    return m_singlet_ID;

}

//*****************************************************************************

//:::::::::::::::::::::::
//:: METHOD getStripID ::
//:::::::::::::::::::::::

unsigned int RPCHit::getStripID(void) const {

    return m_strip_ID;

}

//*****************************************************************************

//:::::::::::::::::::::::
//:: METHOD isEtaStrip ::
//:::::::::::::::::::::::

bool RPCHit::isEtaStrip(void) const {

    return (m_strip_type==0);

}

//*****************************************************************************

//:::::::::::::::::::::::
//:: METHOD isPhiStrip ::
//:::::::::::::::::::::::

bool RPCHit::isPhiStrip(void) const {

    return (m_strip_type==1);

}

//*****************************************************************************

//:::::::::::::::::::::::
//:: METHOD getHitTime ::
//:::::::::::::::::::::::

float RPCHit::getHitTime(void) const {

    return m_hit_leading_egde_time;

}

//*****************************************************************************

//:::::::::::::::::::::::::::::::::
//:: METHOD getTimeOverThreshold ::
//:::::::::::::::::::::::::::::::::

float RPCHit::getTimeOverThreshold(void) const {

    return m_hit_trailing_egde_time-m_hit_leading_egde_time;

}
