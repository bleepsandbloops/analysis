//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 20.11.2017, AUTHOR: OLIVER KORTNER, CATRIONA BRUCE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef RPCDataAnalysisH
#define RPCDataAnalysisH

//:::::::::::::::::::::::::::
//:: CLASS RPCDataAnalysis ::
//:::::::::::::::::::::::::::

/// \class RPCDataAnalysis
///
/// Class for the analysis of H8 RPC test-beam data with the first large scale
/// RPC BIS-7 prototype.
///
/// \date 20.11.2017
///
/// \author Oliver.Kortner@CERN.CH

//::::::::::::::::::
//:: HEADER FILES ::
//::::::::::::::::::

// standard C++ //
#include <map>
#include <vector>
#include <string>

// ROOT //
#include "TCanvas.h"
#include "TH1I.h"
#include "TH1F.h"
#include "TH2I.h"
#include "TH2F.h"
#include "TTree.h"
#include "TFile.h"
#include "TCut.h"

// RPC analysis stuff //
#include "RPCH8DataReader.h"
#include "RPCHit.h"

class RPCDataAnalysis {

public:
// Constructor //
    RPCDataAnalysis(const std::string & input_file_name,
                    const std::string & output_file_name,
                    const std::string & config_file_name,
		    const std::string & job_options_file
		    );
    ///< Constructor.
    ///< \param input_file_name Input file name.
    ///< \param output_file_name    Output file name.
    ///< \param config_file_name    Name of the cabling file.

// Methods //
    void runAnalysis(Long64_t first_event=0, Long64_t last_event=-1);
    ///< Run the analysis programme.
    ///< \param first_event First event (starting from 0).
    ///< \param last_event  Last event (if last_event<first_event, all events
    ///<                    will be analysed).
 


  
    
private:
    RPCH8DataReader * p_reader; // data reader
    TFile *p_outfile;
    std::vector< std::vector<RPCHit *> > m_hit; // m_hit[singlet ID][hit number]
    //std::vector<int> *v;
    std::map<unsigned int, unsigned int> m_RPC_layer; // RPC layer [TDC channel]
    std::map<unsigned int, unsigned int> m_strip_type; // strip type [TDC channel]
    std::map<unsigned int, unsigned int> m_strip_number; // strip number [TDC channel]

    void readCablingFile(const std::string & config_file_name);
    void readJobOptionsFile(const std::string & job_options_file_name);
    void doROOTStuff(const std::string & output_file_name);
                                    // booking of histograms, trees, ecc.
    void analyseHits(void); // analysis of RPC hits


    // job options //
    double m_t_min_eta[3];
    double m_t_max_eta[3];
    double m_t_min_phi[3];
    double m_t_max_phi[3];

// ROOT stuff //
    std::vector<TH1F *> h_hits_per_strip_eta;
    std::vector<TH1F *> h_hits_per_strip_phi;
    std::vector<TH1F *> h_tot_eta;
    std::vector<TH1F *> h_tot_phi;
    std::vector<TH1F *> h_strip_eff_eta;
    std::vector<TH1F *> h_strip_eff_phi;
    std::vector<TH1F *> h_cluster_size_eta;
    std::vector<TH1F *> h_cluster_size_phi;

//Canvas objects//

    TCanvas *c1;
    //TCanvas *c2;
    //TCanvas *c3;
};

#endif
