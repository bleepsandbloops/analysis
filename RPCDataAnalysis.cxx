//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 20.11.2017, AUTHOR: OLIVER KORTNER, CATRIONA BRUCE
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: IMPLEMENTATION OF CONSTRUCTORS AND METHODS DEFINED IN THE CLASS ::
//::                          RPCDataAnalysis                        ::
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//:::::::::::::::::
//:: HEADER FILE ::
//:::::::::::::::::

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include "../include/SimpleFileParser.h"
#include "../include/MT_Edge_Time_Measurement.H"
#include "../include/RPCDataAnalysis.h"
#include "../include/ToString.h"

//:::::::::::::::
//:: NAMESPACE ::
//:::::::::::::::

using namespace std;

//*****************************************************************************

//:::::::::::::::::
//:: CONSTRUCTOR ::
//:::::::::::::::::

RPCDataAnalysis::RPCDataAnalysis(const string & input_file_name,
                                 const string & output_file_name,
                                 const string & config_file_name,
                                 const std::string & job_options_file) {

    readCablingFile(config_file_name);
    readJobOptionsFile(job_options_file);
    p_reader = new RPCH8DataReader(input_file_name);
    doROOTStuff(output_file_name);

    m_hit = std::vector< std::vector<RPCHit *> >(3);

}

//*****************************************************************************

//::::::::::::::::::::::::
//:: METHOD runAnalysis ::
//::::::::::::::::::::::::

void RPCDataAnalysis::runAnalysis(Long64_t first_event,
                                  Long64_t last_event) {

///////////////
// VARIABLES //
///////////////

    Long64_t nentries(p_reader->fChain->GetEntriesFast());
    Long64_t last_evnt(last_event);

    map<unsigned int, vector<MT_Edge_Time_Measurement> > edges;

    unsigned int singlet_ID;
    unsigned int strip_ID;
    unsigned int strip_type;
    float hit_leading_egde_time;
    float hit_trailing_egde_time; 

////////////
// CHECKS //
////////////

    if (first_event>=nentries) {
        cerr << "Class RPCDataAnalysis, method runAnalysis: ERROR!" << endl
             << "Value of first entry greater than number of entries."
             << endl;
        return;
    }
    if (last_evnt<first_event || last_evnt>=nentries) {
        last_evnt = nentries;
    }

////////////////
// EVENT LOOP //
////////////////

    for (Long64_t jentry=first_event; jentry<=last_evnt;jentry++) {

// clear //
        for (unsigned int k=0; k<m_hit.size(); k++) {
            for (unsigned int l=0; l<m_hit[k].size(); l++) {
                delete m_hit[k][l];
            }
            m_hit[k].clear();
        }
        edges.clear();

// get the event //
        p_reader->fChain->GetEntry(jentry);

// group leading and trailing edges //
        for (int h=0; h<p_reader->number_of_hits; h++) {
            MT_Edge_Time_Measurement edge((*(p_reader->TDC_channel))[h],
                                          (*(p_reader->TDC_TimeStamp))[h],
                                          (*(p_reader->TDC_StyleStamp))[h]!=0);
            edges[edge.channel()].push_back(edge);
        }

    // sort leading and trailing edges //
        for (map<unsigned int, vector<MT_Edge_Time_Measurement> >::iterator it=
                edges.begin(); it!=edges.end(); it++) {
            sort(it->second.begin(), it->second.end());
        }
    // group edges to TDC hits //
        for (map<unsigned int, vector<MT_Edge_Time_Measurement> >::iterator it=
                edges.begin(); it!=edges.end(); it++) {
            for (unsigned int edge=0; edge<it->second.size(); edge++) {
                if (it->second[edge].isLeadingEdge()) {
                    hit_leading_egde_time = it->second[edge].time();
                }
                if (edge+1<it->second.size() &&
                        it->second[edge+1].isTrailingEdge()) {
                    hit_trailing_egde_time = it->second[edge+1].time();
                } else {
                    hit_trailing_egde_time = hit_leading_egde_time;
                }

                singlet_ID = m_RPC_layer[it->second[edge].channel()];
                strip_ID = m_strip_number[it->second[edge].channel()];
                strip_type = m_strip_type[it->second[edge].channel()];
                m_hit[singlet_ID].push_back(new RPCHit(singlet_ID,
                                           strip_ID,
                                           strip_type,
                                           hit_leading_egde_time,
                                           hit_trailing_egde_time));

                static ofstream outfile("debug.txt");
                outfile << it->second[edge].channel() << "\t"
                        << m_strip_type[it->second[edge].channel()] << endl;
            }
        }

// analyse the hits //
        analyseHits();

    }

////////////////////////////
// CLOSE ROOT OUTPUT FILE //
////////////////////////////

    p_outfile->Write();
    p_outfile->Close();

    return;

}

//*****************************************************************************

//::::::::::::::::::::::::::::
//:: METHOD readCablingFile ::
//::::::::::::::::::::::::::::

void RPCDataAnalysis::readCablingFile(const string & config_file_name) {

/////////////////////////////////
// OPEN THE CONFIGURATION FILE //
/////////////////////////////////

    ifstream infile(config_file_name.c_str());
    if (infile.fail()) {
        cerr << "Class RPCDataAnalysis, method readCablingFile: ERROR!" << endl
             << "Could not open file " << config_file_name << "!" << endl;
        exit(1);
    }

////////////////////////////////
// PARSE THE JOB OPTIONS FILE //
////////////////////////////////

    SimpleFileParser parser;
    std::vector<const JobOption *> job_options(parser.parse(infile));

    for (unsigned int k=0; k<job_options.size(); k++) {

        if (job_options[k]->getName()=="#RPC_layer") {
            if (k+3>=job_options.size()) {
                cerr << "Class RPCDataAnalysis, "
                     << "method readCablingFile: WARNING!" << endl
                     << "Incomplete line, file corrupted!" <<endl;
                return;
            }
            if (job_options[k+1]->getName()!="#strip_type" ||
                job_options[k+2]->getName()!="#strip_number" ||
                job_options[k+3]->getName()!="#TDC_channel") {
                cerr << "Class RPCDataAnalysis, "
                     << "method readCablingFile: WARNING!" << endl
                     << "File syntax incorrect!" <<endl;
                return;
            }
            m_RPC_layer[job_options[k+3]->getIntegerValue()] =
                                        job_options[k]->getIntegerValue();
            m_strip_type[job_options[k+3]->getIntegerValue()] =
                                        (job_options[k+1]->getValue()=="phi");
            m_strip_number[job_options[k+3]->getIntegerValue()] =
                                        job_options[k+2]->getIntegerValue();
        }

    }

    return;

}

//*****************************************************************************

//:::::::::::::::::::::::::::::::
//:: METHOD readJobOptionsFile ::
//:::::::::::::::::::::::::::::::

void RPCDataAnalysis::readJobOptionsFile(
                                const std::string & job_options_file_name) {

 //////////////////////////////
// OPEN THE JOB OPTIONS FILE //
///////////////////////////////

    ifstream infile(job_options_file_name.c_str());
    if (infile.fail()) {
        cerr << "Class RPCDataAnalysis, "
             << "method readJobOptionsFile: ERROR!" << endl
             << "Could not open file " << job_options_file_name << "!" << endl;
        exit(1);
    }

////////////////////////////////
// PARSE THE JOB OPTIONS FILE //
////////////////////////////////

    SimpleFileParser parser;
    std::vector<const JobOption *> job_options(parser.parse(infile));

    for (unsigned int k=0; k<job_options.size(); k++) {

        if (job_options[k]->getName()=="#t_min_s0_eta") {
            m_t_min_eta[0] = job_options[k]->getDoubleValue();
        }
        if (job_options[k]->getName()=="#t_max_s0_eta") {
            m_t_max_eta[0] = job_options[k]->getDoubleValue();
        }

        if (job_options[k]->getName()=="#t_min_s1_eta") {
            m_t_min_eta[1] = job_options[k]->getDoubleValue();
        }
        if (job_options[k]->getName()=="#t_max_s1_eta") {
            m_t_max_eta[1] = job_options[k]->getDoubleValue();
        }

        if (job_options[k]->getName()=="#t_min_s2_eta") {
            m_t_max_eta[2] = job_options[k]->getDoubleValue();
        }
        if (job_options[k]->getName()=="#t_max_s2_eta") {
            m_t_max_eta[2] = job_options[k]->getDoubleValue();
        }

        if (job_options[k]->getName()=="#t_min_s0_phi") {
            m_t_min_phi[0] = job_options[k]->getDoubleValue();
        }
        if (job_options[k]->getName()=="#t_max_s0_phi") {
            m_t_max_phi[0] = job_options[k]->getDoubleValue();
        }

        if (job_options[k]->getName()=="#t_min_s1_phi") {
            m_t_min_phi[1] = job_options[k]->getDoubleValue();
        }
        if (job_options[k]->getName()=="#t_max_s1_phi") {
            m_t_max_phi[1] = job_options[k]->getDoubleValue();
        }

        if (job_options[k]->getName()=="#t_min_s2_phi") {
            m_t_max_phi[2] = job_options[k]->getDoubleValue();
        }
        if (job_options[k]->getName()=="#t_max_s2_phi") {
            m_t_max_phi[2] = job_options[k]->getDoubleValue();
        }

    }

    return;

}

//*****************************************************************************

//::::::::::::::::::::::::
//:: METHOD doROOTStuff ::
//::::::::::::::::::::::::

void RPCDataAnalysis::doROOTStuff(const string & output_file_name) {

// auxiliaries //
    ToString tostring;
    string name;

// ROOT file //
    p_outfile = new TFile(output_file_name.c_str(), "RECREATE");

// histograms //
    h_cluster_size_eta = vector<TH1F *>(3);
    h_cluster_size_phi = vector<TH1F *>(3);
    h_hit_time_eta = vector<TH1F *>(3);
    h_hit_time_phi = vector<TH1F *>(3);
    h_hit_ToT_eta = vector<TH1F *>(3);
    h_hit_ToT_phi = vector<TH1F *>(3);

    for (unsigned int k=0; k<3; k++) {
        name = "h_cluster_size_eta_"+tostring(k);
        h_cluster_size_eta[k] = new TH1F(name.c_str(), "", 10, -0.5, 9.5);
            h_cluster_size_eta[k]->SetXTitle("Cluster size");
            h_cluster_size_eta[k]->SetYTitle("Counts");

        name = "h_cluster_size_phi_"+tostring(k);
        h_cluster_size_phi[k] = new TH1F(name.c_str(), "", 10, -0.5, 9.5);
            h_cluster_size_phi[k]->SetXTitle("Cluster size");
            h_cluster_size_phi[k]->SetYTitle("Counts");

        name = "h_hit_time_eta_"+tostring(k);
        h_hit_time_eta[k] = new TH1F(name.c_str(), "", 1011, -10.5, 1000.5);
            h_hit_time_eta[k]->SetXTitle("Hit time [ns]");
            h_hit_time_eta[k]->SetYTitle("dn/dt [1/ns]");

        name = "h_hit_time_phi_"+tostring(k);
        h_hit_time_phi[k] = new TH1F(name.c_str(), "", 1011, -10.5, 1000.5);
            h_hit_time_phi[k]->SetXTitle("Hit time [ns]");
            h_hit_time_phi[k]->SetYTitle("dn/dt [1/ns]");

        name = "h_hit_ToT_eta_"+tostring(k);
        h_hit_ToT_eta[k] = new TH1F(name.c_str(), "", 1011, -10.5, 1000.5);
            h_hit_ToT_eta[k]->SetXTitle("Time over threshold [ns]");
            h_hit_ToT_eta[k]->SetYTitle("dn/dt [1/ns]");

        name = "h_hit_ToT_phi_"+tostring(k);
        h_hit_ToT_phi[k] = new TH1F(name.c_str(), "", 1011, -10.5, 1000.5);
            h_hit_ToT_phi[k]->SetXTitle("Time over threshold [ns]");
            h_hit_ToT_phi[k]->SetYTitle("dn/dt [1/ns]");
    }

    return;

}

//*****************************************************************************

//::::::::::::::::::::::::
//:: METHOD analyseHits ::
//::::::::::::::::::::::::

void RPCDataAnalysis::analyseHits(void) {

//     static ofstream outfile("debug.txt");
//     for (unsigned s=0; s<3; s++) {
//         for (unsigned int h=0; h<m_hit[s].size(); h++) {
//             outfile << s << "\t"
//                     << m_hit[s][h]->getSingleID() << "\t"
//                     << m_hit[s][h]->getStripID() << "\t"
//                     << m_hit[s][h]->isEtaStrip() << "\t"
//                     << m_hit[s][h]->getHitTime() << "\t"
//                     << m_hit[s][h]->getTimeOverThreshold() << endl;
//         }
//     }

//////////////////////
// HIT TIME AND TOT //
//////////////////////

    for (unsigned int s=0; s<3; s++) {
        for (unsigned int h=0; h<m_hit[s].size(); h++) {
            if (m_hit[s][h]->isEtaStrip()) {
                h_hit_time_eta[s]->Fill(m_hit[s][h]->getHitTime(), 1.0);
                h_hit_ToT_eta[s]->Fill(m_hit[s][h]->getTimeOverThreshold(),
                                                                        1.0);
            } else {
                h_hit_time_phi[s]->Fill(m_hit[s][h]->getHitTime(), 1.0);
                h_hit_ToT_phi[s]->Fill(m_hit[s][h]->getTimeOverThreshold(),
                                                                        1.0);
            }
        }
    }

//////////////////
// CLUSTER SIZE //
//////////////////

    for (unsigned int s=0; s<3; s++) {
        vector<unsigned int> strip[2];
        for (unsigned int h=0; h<m_hit[s].size(); h++) {
            if (m_hit[s][h]->isEtaStrip() &&
                m_hit[s][h]->getHitTime()>m_t_min_eta[s] &&
                m_hit[s][h]->getHitTime()<m_t_max_eta[s]) {
                strip[m_hit[s][h]->isEtaStrip()].push_back(
                                                m_hit[s][h]->getStripID());
            }
            if (!m_hit[s][h]->isEtaStrip() &&
                m_hit[s][h]->getHitTime()>m_t_min_phi[s] &&
                m_hit[s][h]->getHitTime()<m_t_max_phi[s]) {
                strip[m_hit[s][h]->isEtaStrip()].push_back(
                                                m_hit[s][h]->getStripID());
            }
        }
        sort(strip[0].begin(), strip[0].end());
        sort(strip[1].begin(), strip[1].end());
        unsigned int size[2] = { 1, 1 };
        for (unsigned int k=0; k<2; k++) {
        for (unsigned int h=1; h<strip[k].size(); h++) {
            if (strip[k][h]==strip[k][h-1]) {
                size[k]++;
            } else {
                if (k==1) {
                    h_cluster_size_eta[s]->Fill(size[1], 1.0);
                } else {
                    h_cluster_size_phi[s]->Fill(size[0], 1.0);
                }
                size[k] = 1;
            }
        }
        }
    }

    return;

}
