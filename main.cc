#include <iostream> 
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <TString.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TMinuit.h>

//#include <pthread.h>
//#include <unistd.h>
//#include "TThread.h"

#include "AnaInput.h"
#include "TestRun.h"
//#include "mtest.h"

using namespace std; 

int main( int argc, const char* argv[] ) { 

  string datacardfile = ( argc > 1 ) ? argv[1] : "DataCard.txt";
  //AnaInput  *Input = new AnaInput( datacardfile );
  AnaInput  *Input = AnaInput::Instance() ;
  Input->SetDatacard( datacardfile ) ;
  
  // method to read root files
  string mesFileName ;
  Input->GetParameters( "TheMES", &mesFileName );

  int module = -1 ;
  Input->GetParameters( "Module", & module ) ;

  if ( module == 0 ) {
     TestRun   *tRun  = new TestRun( datacardfile ) ;
     //tRun->ReadMES( mesFileName );
     tRun->TestPlot( mesFileName );
     delete tRun ;
  }
  if ( module == 1 ) {
     TestRun   *tRun  = new TestRun( datacardfile ) ;
     tRun->RootMethod();
     delete tRun ;
  }
  if ( module == 2 ) {
     TestRun   *tRun  = new TestRun( datacardfile ) ;
     tRun->RawData();
     delete tRun ;
  }
  if ( module == 3 ) {
     TestRun   *tRun  = new TestRun( datacardfile ) ;
     tRun->BackgroundTune();
     delete tRun ;
  }

  delete Input ;
  cout<<" Finished !!!"<<endl ;

  return 0;

}

