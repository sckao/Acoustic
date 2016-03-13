#ifndef AnaInput_H
#define AnaInput_H

#include "TObject.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <math.h>

#include <TMath.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <TString.h>
#include <TSystem.h>
#include <TLorentzVector.h>

using namespace std;

struct mes{

   double time ;
   double p1 ;
   double p2 ;
   double p3 ;
   double c  ; 
   int    idx  ;
} ;

//class AnaInput : public TObject {
class AnaInput {

public:

   ~AnaInput();     
   static AnaInput* Instance() ;
  
   void SetDatacard( string datacardInput ) ;

   void GetParameters( string paraName, int* thePara, string cfgFile = "" );
   void GetParameters( string paraName, double* thePara, string cfgFile ="" );
   void GetParameters( string paraName, string* thePara, string cfgFile ="" );
   void GetParameters( string paraName, vector<double>* thePara, string cfgFile = "" );
   void GetParameters( string paraName, vector<string>* thePara, string cfgFile = "" );
   void GetParameters( string paraName, vector<int>* thePara, string cfgFile = "" );
   
   int ReadMES( string fileName, vector<mes>& dataV ) ;
   void ReadMESInfo( string fileName, string paraName, vector<double>& thePara ) ;
   void ReadMES1( string fileName ) ;  // example method 

private:

   //AnaInput( string datacardInput = "DataCard.txt" );     
   AnaInput();     

   string datacardfile ;

   static AnaInput* m_Instance;


   //ClassDef(AnaInput, 1);

};

//#if !defined(__CINT__)
//    ClassImp(AnaInput);

#endif

