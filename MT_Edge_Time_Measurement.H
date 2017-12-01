//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 08.06.2015, AUTHOR: OLIVER KORTNER
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef MT_Edge_Time_MeasurementH
#define MT_Edge_Time_MeasurementH

//::::::::::::::::::::::::::::::::::::
//:: CLASS MT_Edge_Time_Measurement ::
//::::::::::::::::::::::::::::::::::::

/// \class MT_Edge_Time_Measurement
///
/// This is an auxiliary class to store TDC hit data in the class
/// MT_Edge_Time_Measurement.
///
/// \date 08.06.2015
///
/// \author Oliver.Kortner@CERN.CH

class MT_Edge_Time_Measurement {

public:
// Constructor //
    MT_Edge_Time_Measurement(void);
    ///< Default constructor.
    ///< No meaningful content is generated. This constructor exists to
    ///< facilitate the use of STL containers.

    MT_Edge_Time_Measurement(const unsigned int & channel,
                             const unsigned int & TDC_count,
                             bool is_leading_edge);
    ///< Constructor.
    ///< \param channel Generalized channel number.
    ///< \param TDC_count   Edge time in TDC counts.
    ///< \param is_leading_edge Flag indicating if the measurement regards a
    ///<                        leading edge (true) or a trailing edge (false).

// Methods //
    const unsigned int channel(void) const;
    ///< Get the generalized channel number.

    const unsigned int & time(void) const;
    ///< Get the time of the edge in TDC counts.

    bool isLeadingEdge(void) const;
    ///< Returns true in case of a leading edge.

    bool isTrailingEdge(void) const;
    ////< Returns true in case of a trailing edge.

    bool operator < (const MT_Edge_Time_Measurement & edge) const;
    ///< < operator to allow the sorting of edge time measurements.

private:
    unsigned int m_channel;
    unsigned int m_time;
    bool m_is_leading_edge;

};

#endif
