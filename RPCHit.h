//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 20.11.2017, AUTHOR: OLIVER KORTNER
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef RPCHitH
#define RPCHitH

//::::::::::::::::::
//:: CLASS RPCHit ::
//::::::::::::::::::

/// \class RPCHit
///
/// Simple RPC hit class to store singlet number, strip number, hit time,
/// time over threshold.
///
/// \date 20.11.2017
///
/// \author Oliver.Kortner@CERN.CH

class RPCHit {

public:
// Constructor //
    RPCHit(const unsigned int & singlet_ID,
           const unsigned int & strip_ID,
           const unsigned int & strip_type,
           const float & hit_leading_egde_time,
           const float & hit_trailing_egde_time);
    ///< Constructor
    ///< \param singlet_ID  singlet ID running from 0 to 2.
    ///< \param strip_ID    Strip number running from 0 to the maximum number
    ///<                    of strips.
    ///< \param strip_type  = 0 for eta strips, = 1 for phi strips.
    ///< \param hit_leading_egde_time   Time of the leading edge of a hit
    ///<                                (presumably in ns).
    ///< \param hit_trailing_egde_time  Time of the trailing edge of a hit
    ///<                                (presumably in ns).

// Method //
    unsigned int getSingleID(void) const;
    ///< Get the singlet ID.

    unsigned int getStripID(void) const;
    ///< Get the strip ID.

    bool isEtaStrip(void) const;
    ///< Returns true if we have an eta strip.

    bool isPhiStrip(void) const;
    ///< Returns true if we have a phi strip.

    float getHitTime(void) const;
    ///< Get the hit time in ns.

    float getTimeOverThreshold(void) const;
    ///< Get the time over threshold in ns.

private:
    unsigned int m_singlet_ID;
    unsigned int m_strip_ID;
    unsigned int m_strip_type;
    float m_hit_leading_egde_time;
    float m_hit_trailing_egde_time;

};

#endif
