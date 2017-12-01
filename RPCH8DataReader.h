//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Nov 20 12:27:21 2017 by ROOT version 6.10/02
// from TTree RAWData/RAWData
// found on file: /remote/ceph/user/k/kortner/data/RPC/rundef1/Testdefinitivo_ATL_RPC_10000_Beam-Trigger_0.4V3.5V0.9V2.5V1.4V_4400V_run20171016011059.root
//////////////////////////////////////////////////////////

#ifndef RPCH8DataReader_h
#define RPCH8DataReader_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <string>


class RPCH8DataReader {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           EventNumber;
   Int_t           number_of_hits;
   std::vector<int>     *TDC_channel;
   std::vector<float>   *TDC_TimeStamp;
   std::vector<int>     *TDC_StyleStamp;

   // List of branches
   TBranch        *b_EventNumber;   //!
   TBranch        *b_number_of_hits;   //!
   TBranch        *b_TDC_channel;   //!
   TBranch        *b_TDC_TimeStamp;   //!
   TBranch        *b_TDC_StyleStamp;   //!

   RPCH8DataReader(const std::string & file_name);
   virtual ~RPCH8DataReader();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

// #ifdef RPCH8DataReader_cxx
// RPCH8DataReader::RPCH8DataReader(const std::string & file_name) : fChain(0) 
// {
// // if parameter tree is not specified (or zero), connect the file
// // used to generate this class and read the Tree.
//    TTree *tree(0);
//    {
//       TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(file_name.c_str());
//       if (!f || !f->IsOpen()) {
//          f = new TFile(file_name.c_str());
//       }
//       f->GetObject("RAWData",tree);
// 
//    }
//    Init(tree);
// }
// 
// RPCH8DataReader::~RPCH8DataReader()
// {
//    if (!fChain) return;
//    delete fChain->GetCurrentFile();
// }
// 
// Int_t RPCH8DataReader::GetEntry(Long64_t entry)
// {
// // Read contents of entry.
//    if (!fChain) return 0;
//    return fChain->GetEntry(entry);
// }
// Long64_t RPCH8DataReader::LoadTree(Long64_t entry)
// {
// // Set the environment to read one entry
//    if (!fChain) return -5;
//    Long64_t centry = fChain->LoadTree(entry);
//    if (centry < 0) return centry;
//    if (fChain->GetTreeNumber() != fCurrent) {
//       fCurrent = fChain->GetTreeNumber();
//       Notify();
//    }
//    return centry;
// }
// 
// void RPCH8DataReader::Init(TTree *tree)
// {
//    // The Init() function is called when the selector needs to initialize
//    // a new tree or chain. Typically here the branch addresses and branch
//    // pointers of the tree will be set.
//    // It is normally not necessary to make changes to the generated
//    // code, but the routine can be extended by the user if needed.
//    // Init() will be called many times when running on PROOF
//    // (once per file to be processed).
// 
//    // Set object pointer
//    TDC_channel = 0;
//    TDC_TimeStamp = 0;
//    TDC_StyleStamp = 0;
//    // Set branch addresses and branch pointers
//    if (!tree) return;
//    fChain = tree;
//    fCurrent = -1;
//    fChain->SetMakeClass(1);
// 
//    fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
//    fChain->SetBranchAddress("number_of_hits", &number_of_hits, &b_number_of_hits);
//    fChain->SetBranchAddress("TDC_channel", &TDC_channel, &b_TDC_channel);
//    fChain->SetBranchAddress("TDC_TimeStamp", &TDC_TimeStamp, &b_TDC_TimeStamp);
//    fChain->SetBranchAddress("TDC_StyleStamp", &TDC_StyleStamp, &b_TDC_StyleStamp);
//    Notify();
// }
// 
// Bool_t RPCH8DataReader::Notify()
// {
//    // The Notify() function is called when a new file is opened. This
//    // can be either for a new TTree in a TChain or when when a new TTree
//    // is started when using PROOF. It is normally not necessary to make changes
//    // to the generated code, but the routine can be extended by the
//    // user if needed. The return value is currently not used.
// 
//    return kTRUE;
// }
// 
// void RPCH8DataReader::Show(Long64_t entry)
// {
// // Print contents of entry.
// // If entry is not specified, print current entry
//    if (!fChain) return;
//    fChain->Show(entry);
// }
// Int_t RPCH8DataReader::Cut(Long64_t entry)
// {
// // This function may be called from Loop.
// // returns  1 if entry is accepted.
// // returns -1 otherwise.
//    return 1;
// }
// #endif // #ifdef RPCH8DataReader_cxx
