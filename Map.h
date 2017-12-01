#include "TFile.h"
#include "TTree.h"
#include "TColor.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TString.h"
#include "TMath.h"
#include "TVirtualFFT.h"
#include "TApplication.h"
#include "TCanvas.h"
#include <vector>
#include <string>
#include <sys/stat.h>
#include <sstream>
#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>

///////////////////////////////////////////////////
//                                               //
//                Channel Map                    //
//                                               //
///////////////////////////////////////////////////
#define TOP_Eta_ch00 0
#define TOP_Eta_ch01 1
#define TOP_Eta_ch02 2
#define TOP_Eta_ch03 3
#define TOP_Eta_ch04 4
#define TOP_Eta_ch05 5
#define TOP_Eta_ch06 6
#define TOP_Eta_ch07 7
#define TOP_Eta_ch08 8
#define TOP_Eta_ch09 9
#define TOP_Eta_ch10 10
#define TOP_Eta_ch11 11
#define TOP_Eta_ch12 12
#define TOP_Eta_ch13 13
#define TOP_Eta_ch14 14
#define TOP_Eta_ch15 15
#define TOP_Phi_ch00 16
#define TOP_Phi_ch01 17
#define TOP_Phi_ch02 18
#define TOP_Phi_ch03 19
#define TOP_Phi_ch04 20
#define TOP_Phi_ch05 21
#define TOP_Phi_ch06 22
#define TOP_Phi_ch07 23
#define TOP_Phi_ch08 24
#define TOP_Phi_ch09 25
#define TOP_Phi_ch10 26
#define TOP_Phi_ch11 27
#define TOP_Phi_ch12 28
#define TOP_Phi_ch13 29
#define TOP_Phi_ch14 30
#define TOP_Phi_ch15 31
#define MID_Eta_ch00 32
#define MID_Eta_ch01 33
#define MID_Eta_ch02 34
#define MID_Eta_ch03 35
#define MID_Eta_ch04 36
#define MID_Eta_ch05 37
#define MID_Eta_ch06 38
#define MID_Eta_ch07 39
#define MID_Eta_ch08 40
#define MID_Eta_ch09 41
#define MID_Eta_ch10 42
#define MID_Eta_ch11 43
#define MID_Eta_ch12 44
#define MID_Eta_ch13 45
#define MID_Eta_ch14 46
#define MID_Eta_ch15 47
#define MID_Phi_ch00 48
#define MID_Phi_ch01 49
#define MID_Phi_ch02 50
#define MID_Phi_ch03 51
#define MID_Phi_ch04 52
#define MID_Phi_ch05 53
#define MID_Phi_ch06 54
#define MID_Phi_ch07 55
#define MID_Phi_ch08 56
#define MID_Phi_ch09 57
#define MID_Phi_ch10 58
#define MID_Phi_ch11 59
#define MID_Phi_ch12 60
#define MID_Phi_ch13 61
#define MID_Phi_ch14 62
#define MID_Phi_ch15 63
#define BOT_Eta_ch00 64
#define BOT_Eta_ch01 65
#define BOT_Eta_ch02 66
#define BOT_Eta_ch03 67
#define BOT_Eta_ch04 68
#define BOT_Eta_ch05 69
#define BOT_Eta_ch06 70
#define BOT_Eta_ch07 71
#define BOT_Eta_ch08 72
#define BOT_Eta_ch09 73
#define BOT_Eta_ch10 74
#define BOT_Eta_ch11 75
#define BOT_Eta_ch12 76
#define BOT_Eta_ch13 77
#define BOT_Eta_ch14 78
#define BOT_Eta_ch15 79
#define BOT_Phi_ch00 80
#define BOT_Phi_ch01 81
#define BOT_Phi_ch02 82
#define BOT_Phi_ch03 83
#define BOT_Phi_ch04 84
#define BOT_Phi_ch05 85
#define BOT_Phi_ch06 86
#define BOT_Phi_ch07 87
#define BOT_Phi_ch08 88
#define BOT_Phi_ch09 89
#define BOT_Phi_ch10 90
#define BOT_Phi_ch11 91
#define BOT_Phi_ch12 92
#define BOT_Phi_ch13 93
#define BOT_Phi_ch14 94
#define BOT_Phi_ch15 95

#define Signal_Beginning 390
#define Signal_Endding 510
#define Signal_Endding_Trailing 550
#define Signal_TOP_Beginning 390
#define Signal_TOP_Endding 460
#define Signal_MID_Beginning 390
#define Signal_MID_Endding 460
#define Signal_BOT_Beginning 390
#define Signal_BOT_Endding 460

#define Signal_TOP_Eta_Beginning 390
#define Signal_TOP_Eta_Endding 450
#define Signal_TOP_Phi_Beginning 390
#define Signal_TOP_Phi_Endding 510
#define Signal_MID_Eta_Beginning 390
#define Signal_MID_Eta_Endding 450
#define Signal_MID_Phi_Beginning 390
#define Signal_MID_Phi_Endding 510
#define Signal_BOT_Eta_Beginning 390
#define Signal_BOT_Eta_Endding 450
#define Signal_BOT_Phi_Beginning 390
#define Signal_BOT_Phi_Endding 510

void Ana(const TString& name_File, int N_F, const TString& HV_file);
int ReadTree(const TString& File_name);
void draw_eff();

TTree *RawData;

TH1F *h_TDC_TimeStamp_Leading = new TH1F("h_TDC_TimeStamp_Leading","h_TDC_TimeStamp_Leading",600,0,600);
TH1F *h_TDC_TimeStamp_Trailing = new TH1F("h_TDC_TimeStamp_Trailing","h_TDC_TimeStamp_Trailing",600,0,600);
TH1F *h_TDC_Width = new TH1F("h_TDC_Width","h_TDC_Width",100,0,100);
TH2F *h_Hits_Strip = new TH2F("h_Hits_Strip","h_Hits_Strip",96,0,96,96,0,96);
TH2F *h_TOP_Hits_Strip = new TH2F("h_TOP_Hits_Strip","h_TOP_Hits_Strip",16,0,16,16,16,32);
TH2F *h_MID_Hits_Strip = new TH2F("h_MID_Hits_Strip","h_MID_Hits_Strip",16,32,48,16,48,64);
TH2F *h_BOT_Hits_Strip = new TH2F("h_BOT_Hits_Strip","h_BOT_Hits_Strip",16,64,80,16,80,96);

TH1F *time_dis_Eta_T_T = new TH1F("time_dis_Eta_T_T","time_dis_Eta_T_T",400,-100,100);
TH1F *time_dis_Phi_T_T = new TH1F("time_dis_Phi_T_T","time_dis_Phi_T_T",400,-100,100);
TH1F *time_dis_Eta_T_M = new TH1F("time_dis_Eta_T_M","time_dis_Eta_T_M",400,-100,100);
TH1F *time_dis_Phi_T_M = new TH1F("time_dis_Phi_T_M","time_dis_Phi_T_M",400,-100,100);
TH1F *time_dis_Eta_T_B = new TH1F("time_dis_Eta_T_B","time_dis_Eta_T_B",400,-100,100);
TH1F *time_dis_Phi_T_B = new TH1F("time_dis_Phi_T_B","time_dis_Phi_T_B",400,-100,100);

TH2F *h_TOP_Signal_Strip = new TH2F("h_TOP_Signal_Strip","h_TOP_Signal_Strip",16,0,16,16,16,32);
TH2F *h_MID_Signal_Strip = new TH2F("h_MID_Signal_Strip","h_MID_Signal_Strip",16,32,48,16,48,64);
TH2F *h_BOT_Signal_Strip = new TH2F("h_BOT_Signal_Strip","h_BOT_Signal_Strip",16,64,80,16,80,96);

TH1F *h_hit_Eta_T = new TH1F("h_hit_Eta_T","h_hit_Eta_T",64,-32,32);
TH1F *h_hit_Eta_M = new TH1F("h_hit_Eta_M","h_hit_Eta_M",64,0,64);
TH1F *h_hit_Eta_B = new TH1F("h_hit_Eta_B","h_hit_Eta_B",64,32,96);
TH1F *h_hit_Phi_T = new TH1F("h_hit_Phi_T","h_hit_Phi_T",64,-32,32);
TH1F *h_hit_Phi_M = new TH1F("h_hit_Phi_M","h_hit_Phi_M",64,0,64);
TH1F *h_hit_Phi_B = new TH1F("h_hit_Phi_B","h_hit_Phi_B",64,32,96);
TH1F *h_hit_distance_Eta_T_M = new TH1F("h_hit_distance_Eta_T_M","h_hit_distance_Eta_T_M",64,-32,32);
TH1F *h_hit_distance_Phi_T_M = new TH1F("h_hit_distance_Phi_T_M","h_hit_distance_Phi_T_M",64,-32,32);
TH1F *h_hit_distance_Eta_T_B = new TH1F("h_hit_distance_Eta_T_B","h_hit_distance_Eta_T_B",64,-32,32);
TH1F *h_hit_distance_Phi_T_B = new TH1F("h_hit_distance_Phi_T_B","h_hit_distance_Phi_T_B",64,-32,32);
TH1F *h_hit_distance_Eta_M_B = new TH1F("h_hit_distance_Eta_M_B","h_hit_distance_Eta_M_B",64,-32,32);
TH1F *h_hit_distance_Phi_M_B = new TH1F("h_hit_distance_Phi_M_B","h_hit_distance_Phi_M_M",64,-32,32);

int Tot_c;
int TOP_c_EtaOrPhi;
int TOP_c_EtaAndPhi;
int TOP_Eta_c;
int TOP_Phi_c;
int MID_c_EtaOrPhi;
int MID_c_EtaAndPhi;
int MID_Eta_c;
int MID_Phi_c;
int BOT_c_EtaOrPhi;
int BOT_c_EtaAndPhi;
int BOT_Eta_c;
int BOT_Phi_c;

int Ind_Tot_TM;
int Ind_BOT;
int Ind_Tot_MB;
int Ind_TOP;
int Ind_Tot_TB;
int Ind_MID;

int           EventNumber;
int           N_hits_evt;
std::vector<int>   *Channel = new std::vector<int>;
std::vector<float> *TimeStamp = new std::vector<float>;
std::vector<int>   *TimeStyle = new std::vector<int>;

std::vector<int>    E_channel;
std::vector<int>    E_chamber;
std::vector<bool>   E_isEta;
std::vector<double> E_Leading;
std::vector<double> E_Trailing;
std::vector<double> E_Width;
std::vector<double> E_Leading_Eta_TOP;
std::vector<double> E_Leading_Eta_MID;
std::vector<double> E_Leading_Eta_BOT;
std::vector<double> E_Leading_Phi_TOP;
std::vector<double> E_Leading_Phi_MID;
std::vector<double> E_Leading_Phi_BOT;
std::vector<double> E_Trailing_Eta_TOP;
std::vector<double> E_Trailing_Eta_MID;
std::vector<double> E_Trailing_Eta_BOT;
std::vector<double> E_Trailing_Phi_TOP;
std::vector<double> E_Trailing_Phi_MID;
std::vector<double> E_Trailing_Phi_BOT;
std::vector<double> E_Width_Eta_TOP;
std::vector<double> E_Width_Eta_MID;
std::vector<double> E_Width_Eta_BOT;
std::vector<double> E_Width_Phi_TOP;
std::vector<double> E_Width_Phi_MID;
std::vector<double> E_Width_Phi_BOT;

double HV[99];
double Efficiency_TOP_OR[99];
double Efficiency_TOP_AND[99];
double Efficiency_TOP_ETA[99];
double Efficiency_TOP_PHI[99];
double Efficiency_MID_OR[99];
double Efficiency_MID_AND[99];
double Efficiency_MID_ETA[99];
double Efficiency_MID_PHI[99];
double Efficiency_BOT_OR[99];
double Efficiency_BOT_AND[99];
double Efficiency_BOT_ETA[99];
double Efficiency_BOT_PHI[99];

std::ifstream inputFile;
char file[1024];

int N_file;
