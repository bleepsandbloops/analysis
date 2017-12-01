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
				 const string & job_options_file
				 ) {

    readCablingFile(config_file_name);
    readJobOptionsFile(job_options_file);
    p_reader = new RPCH8DataReader(input_file_name);
    doROOTStuff(output_file_name);

    m_hit = std::vector< std::vector<RPCHit *> >(3);
   
    c1 = new TCanvas("c1","Hits per strip",400,600);
    //c1->SetCanvasSize(1500,1500);
    //c1->SetWindowSize(500,500);
    //c2 = new TCanvas("c2","Pulse length");//Pulse length
    //c3 = new TCanvas("c3","Hit Time Comparison");//Hit Time Comparison
  
    // h1 = new TH1I("h1","channels",16,0,16);
    // h2 = new TH1F("h2","time over threshold",180,1,181);
    // h3 = new TH2I("h3","channels per event",16,0,16,6,0,6);
    // h4 = new TH2F("h4","Time over threshold for different layers",180,1,181,6,0,6);
    // h5 = new TH1I("h5","hits on neighboring strips",16,0,16);
    // //h6 = new TH2I("h6","hits on neighboring layers",16,0,16,6,0,6);
    // h7 = new TH2I("h7","Hit time > 1000",16,0,16,6,0,6);
    // h8 = new TH2I("h8","Hit time < 1000",16,0,16,6,0,6);
    // h9 = new TH1I("h9","Strip popularity eta",16,0,16); 
    // h10 = new TH1I("h10","Strip popularity phi",16,0,16);

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

        analyseHits();

    }

////////////////////////////
// CLOSE ROOT OUTPUT FILE //
////////////////////////////

    //c1->SetCanvasSize(1500,1500);    
    c1->Divide(2,3,0.01,0.01,0);
      
    for(int a = 0; a < 3; a++){
      int etapad = 2*a;
      int phipad = 2*a+1;

      c1->cd(etapad);
      h_hits_per_strip_eta[a]->Draw();

      c1->cd(phipad);
      h_hits_per_strip_phi[a]->Draw();
      //c1->Update();
    }
    c1->Modified();
  
    
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

//  //////////////////////////////
// // OPEN THE JOB OPTIONS FILE //
// ///////////////////////////////

    ifstream infile(job_options_file_name.c_str());
    if (infile.fail()) {
        cerr << "Class RPCDataAnalysis, "
             << "method readJobOptionsFile: ERROR!" << endl
             << "Could not open file " << job_options_file_name << "!" << endl;
        exit(1);
    }

// ////////////////////////////////
// // PARSE THE JOB OPTIONS FILE //
// ////////////////////////////////

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

  //auxiliaries 
  ToString tostring;
  string name;
  string title;
  // 
    p_outfile = new TFile(output_file_name.c_str(), "RECREATE");


//::::::::::::::::::::::::::::::::::::::::::::::
//:: Declare some vectors of three histograms ::
//:::::::::::::::::::::::::::::::::::::::::::::: 


//cuts will be imposed using the vectors:
    double m_t_min_eta[3];
    double m_t_max_eta[3];
    double m_t_min_phi[3];
    double m_t_max_phi[3];

    
  //TH1F: Time over threshold AKA pulse length (cut) (h2, h4,h7,h8)
    //h_tot_eta[k]= vector<TH1F *>(3);
    //h_tot_phi[k]= vector<TH1F *>(3);

  //TH1I: popularity of strips (cut) (h1,h3,9,10)
    h_hits_per_strip_eta = std::vector<TH1F *>(3);
    h_hits_per_strip_phi = std::vector<TH1F *>(3);

    
    //TH1F: efficiency of strips (cut) (based on h1/h3) 
    //h_strip_eff_eta[k]= vector<TH1F *>(3);
    //h_strip_eff_phi[k]= vector<TH1F *>(3);

    //TH1F: cluster size (cut) (BASED ON h5)
    //h_cluster_size[k]= vector<TH1F *>(3);
    //h_cluster_size[k]= vector<TH1F *>(3);



//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//:: Now declare the histograms contained in the vectors ::
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::
  
  for(unsigned int k=0; k<3; k++){

    
    // name = "h_tot_eta_"+tostring(k);
    // h_hits_per_strip_eta[k] = new TH1F(name.c_str(),"Time over threshold",180,1,181);
    // h_hits_per_strip_eta[k]->SetXTitle("Strip");
    // h_hits_per_strip_eta[k]->SetYTitle("Counts");

    name = "h_hits_per_strip_eta_"+tostring(k);
    title = "Counts per Strip, eta singlet "+tostring(k+1);
    h_hits_per_strip_eta[k] = new TH1F(name.c_str(),title.c_str(),16,0,16);
    h_hits_per_strip_eta[k]->SetXTitle("Strip");
    h_hits_per_strip_eta[k]->SetYTitle("Counts");

    name = "h_hits_per_strip_phi_"+tostring(k);
    title = "Counts per Strip, phi singlet "+tostring(k+1);
    h_hits_per_strip_phi[k] = new TH1F(name.c_str(),title.c_str(),16,0,16);
    h_hits_per_strip_phi[k]->SetXTitle("Strip");
    h_hits_per_strip_phi[k]->SetYTitle("Counts");
    

  }

  return;
}


//*****************************************************************************

//::::::::::::::::::::::::
//:: METHOD analyseHits ::
//::::::::::::::::::::::::

void RPCDataAnalysis::analyseHits(void) {
 
 
//////////////////////////
// STRIP COUNT  AND TOT //
//////////////////////////

  for (unsigned int s=0; s<3; s++){

    for (unsigned int h=0; h<m_hit[s].size();h++){

///////////////////
// Get the cuts  //
///////////////////


      Long64_t tot = m_hit[s][h]->getTimeOverThreshold();
      Long64_t let = m_hit[s][h]->getHitTime();//Leading edge time
      Long64_t tet = let + tot;//Trailing edge time

      if(tot != 0){
	if(m_hit[s][h]->isEtaStrip()){
	  if(let > m_t_min_eta[s] && tet < m_t_max_eta[s]){
	    h_hits_per_strip_eta[s]->Fill(m_hit[s][h]->getStripID(), 1.0);
	    //other histos fillable here
	  }
	
	}
	else{
	  if(let > m_t_min_phi[s] && tet < m_t_max_phi[s]){
	    h_hits_per_strip_phi[s]->Fill(m_hit[s][h]->getStripID(), 1.0);
	    //other histos fillable here
	  }
	}
     
      }
    }

  }

  // m_t_min_eta
//////////////////////
// STRIP EFFICIENCY //
//////////////////////

//   counts_eta_strips = new std::vector< std::vector< int >(16)>(3);
//   counts_phi_strips = new std::vector< std::vector< int >(16)>(3);
//     //CLEAR V AFTER EVERY ROUND.... 

//  for (unsigned int s=0; s<3; s++){

//     for (unsigned int h=0; h<m_hit[s].size();h++){
//       // if(m_hit[s][h]->getHitTime()>0 && m_hit[s][h] < 1500){
//       // if(m_hit[s][h]->getTimeOverThreshold() != 0){

//       if(m_hit[s][h]->isEtaStrip()){
// 	*counts_eta_strips[s][h] += 1;
//       }
//       else{
// 	*counts_phi_strips[s][h] += 1;
//       }

//       //  }
//       // }
//     }

//   }

// h_strip_eff_eta[s]->Fill(efficiency, 1.0);
// h_strip_eff_phi[s]->Fill(efficiency, 1.0);


//////////////////
// CLUSTER SIZE //
//////////////////

//Things that are needed for the histograms:

//sortable & comparable vector of the strips that have been hit, separated in respective eta/phi and k groups. 
//remember the hit time in order to be able to do the cuts. 


//efficiency = perfect / number of times hit.
//figure out, for each individual strip, how many pulses it had, then divide by the total number of events. EG if it was actuvated 50% of the time.

//ability to sort the strip vector into ascneding order and check if neighboring strips where hit
//increment a counter for every hit strip, abort when the next stirp was not hit. 

  
//SOME TEMPORARY VARIABLES & CONTAINERS FOR HISTOGRAM FILLING
      //int strip = m_hit[s][h]->getStripID(); 
      //float tot = m_hit[s][h]->getTimeOverThreshold();

    //	if(m_hit[s][h]->isEtaStrip()){
    //	  orientation=0;
	  //  eta[s]->push_back(strip);//h6
    //	}
	// else{
	//   orientation = 1;
	//   //  phi[s]->push_back(strip);//h6
	// };

	// unsigned int singlet = m_hit[s][h]->getSingleID();
	// v->push_back(strip);
    
//FILL HISTOGRAM 5
	// if (!v->empty()){
	//   std::sort(v->begin(), v->end());//sort in ascending order
	//   vector<int>::iterator it;
	//   for (it = v->begin(); it != v->end(); ++it){
	//     if(((it != v->end()) && (*(it+1) - *it == 1)) \
	//       || ((it != v->begin()) && (*it - *(it-1) == 1))){ 
	//       h5->Fill(*it);}
	//   }
	// }

  
  
    return;

}




//FILL HISTOGRAM 6 
  //   eta = new std::vector< vector< int > * >;//h6
  //   phi = new std::vector< vector< int > * >;//h6
  //   vector <int> row;//INSTEAD OF ROW, CAST s INTO TYPE str AND CALL IT THAT
    
  // for (unsigned int s=0; s<3; s++){
    
  //   for (unsigned int h=0; h<m_hit[s].size(); h++){
      
  //     int strip = m_hit[s][h]->getStripID(); //from 0-15
  //     if(m_hit[s][h]->isEtaStrip
  // 	 }
  //   }
  //   eta->push_back(row);//h6
  //   eta->at(row).push_back(strip);//h6
  //   phi->push_back(s)
  //   phi->at(row).push_back(strip);//h6
  // }
  
  // HISTOGRAM 6: to check for neighboring singlet hits in the same event,
  //get a similar sorted vector. but this time it should be a 3D vector of vectors.
  //Then you can compare the numbers in each singlet and see if there are any that
  //are only [-1,1] awayfrom each other.
  //HISTOGRAM 6: do a new for loop to fill the vectors



	  // get the ones with isetastrip
	  // get the strip IDs
	  //  put them in a vecotr called seta
	  // get the ones without iseeatstrip
	  //get their strip ids
  //put them in a vecotr


//       static ofstream outfile("debug.txt");
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




	 



