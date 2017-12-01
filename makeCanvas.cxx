void RPCDataAnalysis::readCanvasDesignFile(
				       const::std string & canvas_design_file_name){

 //////////////////////////////
// OPEN THE JOB OPTIONS FILE //
///////////////////////////////
   
  ifstream infile(canvas_design_file_name.c_str());
  if(infile.fail()){
    cerr << "Class RPCDataAnalysis,"
	 << "method readCanvasDesignFile: ERROR!"
	 << "Could not open file " << canvas_design_file_name << "!" << endl;
    exit(1);
  }

 ////////////////////////////////
// PARSE THE JOB OPTIONS FILE //
////////////////////////////////

   const Int_t nhist = 10;
   const Int_t ncanv = 3;

   char *hnames[nhist] = {"h1","h2","h3","h4","h5","h6","h7","h8","h9","h10"};
   char *cnames[ncanv] = {"Strip Popularity","Pulse Length","Hit time comparison"};
   std::map<unsigned int, string> m_canvas;
   TFile *histos = new TFile("tmp.root"); 

   TH1 *hist[nhist];
   TCanvas *c[ncanv];

   for (Int_t i=0;i<nhist;i++) {
      hist[i] = (TH1*)histos->Get(names[i]);
   }

   for( Int_t j=0;j<ncanv;j++){
     c[j] = new TCanvas(Form("c%d",i));
      hist[1]->Draw();
      hist[3]->Draw();
      hist[5]->Draw();
      hist[9]->Draw();
      hist[10]->Draw();
      c[2] = new TCanvas("c2",*cnames
   }
}
