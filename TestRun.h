#ifndef TestRun_H
#define TestRun_H

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <TMath.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TRandom3.h>
#include <TLeaf.h>
#include <TString.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFrame.h>
#include <TLegend.h>
#include <TGraph.h>


#include <algorithm>

#include "AnaInput.h"
#include "MathTools.h"

class TestRun {

public:

   TestRun( string datacardfile = "DataCard.txt");     
   ~TestRun();     
   
   void TestPlot( string fileName ) ;
   void RootMethod() ;
   void GetDataFromMES( vector<mes>& data ) ; 
   void RawData() ;
   void BackgroundTune() ;

   void DataRemoval( vector<mes>& data, double LowCut, double UpCut ) ;
   void ReduceSize( vector<mes>& data, int factor, int idx = -1 ) ;
  
private:

   AnaInput*     Input;

   string mfolder  ;
   string hfolder  ;
   string plotType ;
   string mesFileName ;
   string hFileName ;

   int nPara ;
   int dataSc ;
   int mesIdx ;
   double sigSc ;
   vector<double> para ;
   vector<double> sigR ;
   vector<double> dataR ;
   vector<double> fixP ;
   vector<double> bkgR ;
   vector<double> funcR ;


   //ClassDef(TestRun, 1);
};

//#if !defined(__CINT__)
//    ClassImp(TestRun);
#endif

