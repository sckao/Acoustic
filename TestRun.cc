#include "TestRun.h"
#include "gnuplot-iostream/gnuplot-iostream.h"

TestRun::TestRun( string datacardfile ) {

  Input = AnaInput::Instance() ;

  Input->GetParameters("PlotType",      &plotType ) ; 
  Input->GetParameters("Path",          &hfolder ) ; 

  Input->GetParameters("MES_DIR",       &mfolder ) ; 
  Input->GetParameters("TheMES",        &mesFileName );
  Input->GetParameters("HFileName",     &hFileName );

  Input->GetParameters("MeasIndex",     &mesIdx ) ; 
  Input->GetParameters("DataScale",     &dataSc ) ; 
  Input->GetParameters("SignalScale",   &sigSc ) ; 
  Input->GetParameters("DataRange",     &dataR ) ; 
  Input->GetParameters("N_Para",        &nPara ) ; 
  Input->GetParameters("FixedPara",     &fixP ) ; 
  Input->GetParameters("SignalRange",   &sigR ) ; 
  Input->GetParameters("BkgFitRange",   &bkgR ) ; 
  Input->GetParameters("FuncRange",     &funcR ) ; 
  Input->GetParameters("Para",          &para ) ; 

}

TestRun::~TestRun(){

  //delete Input ;
  cout<<" done ! "<<endl ;
}

 
// Example using gnu plot
void TestRun::TestPlot( string fileName ) {


    vector<mes> data ;
    int err = Input->ReadMES( fileName, data ) ;
    printf(" data size = %d , error code = %d\n", (int)data.size(), err ) ;

    std::vector<std::pair<double, double> > xy_pts_A;
    for ( size_t i =0 ; i< data.size() ;  i++ ) {
        if ( data[i].idx != 0 ) break ;
        xy_pts_A.push_back( std::make_pair( data[i].time,  data[i].p2 ) ) ;
    }

    printf(" data size = %d \n", (int)xy_pts_A.size() ) ;
    // Use GNUPlot to plot !
    Gnuplot gp;
    gp << "set xrange [145:600]\nset yrange [0.0002:0.0012]\n" ; 
    gp << "plot '-' with lines title 'Acustic' \n";
    gp.send1d(xy_pts_A);

}


void TestRun::RootMethod() {

    // Read data from MES file
    string mesfile = mfolder + mesFileName ;
    vector<mes> data ;
    int err = Input->ReadMES( mesfile, data ) ;
    printf(" data size = %d , error code = %d\n", (int)data.size(), err ) ;
    // trim the data in proper range 
    DataRemoval( data , -99, dataR[0] ) ; 
    DataRemoval( data , dataR[1], 9999999 ) ; 
    // keep the original data 
    vector<mes> data0 = data ; 
    // Setup plot name
    string mesName = mesFileName.substr(0, mesFileName.size()-4 ) ; 
    // Read Probe Power
    vector<double> probPow ;
    //Input->ReadMESInfo( mesfile, "ProbePower(mW)", probPow ) ;
    Input->ReadMESInfo( mesfile, "Probe Power", probPow ) ;
    
    //vector<double> calibPara ;
    //Input->ReadMESInfo( mesfile, "A_effective(cm^2)", calibPara ) ;
    //printf( " %f , %f , %d \n", calibPara[0]*100000, calibPara[ calibPara.size()-1 ]*1000000, (int)calibPara.size() ) ;

    // Remove the signal peaks
    vector<mes> bkg = data;
    for (size_t j=0; j< sigR.size() ; j=j+2 ) {
        DataRemoval( bkg , sigR[j], sigR[j+1] ) ; 
    }

    // Select data and Reduce the size
    ReduceSize( bkg, dataSc, mesIdx ) ;
    ReduceSize( data, dataSc, mesIdx ) ;

    // Get the data and background arrary
    const int sz = (int) bkg.size()  ;
    const int sz1 = (int) data.size() ;
    double x[sz] , y[sz], x1[sz1], y1[sz1] ;
    for ( size_t i =0 ; i< bkg.size() ;  i++ ) {
        x[i] = bkg[i].time ; 
        y[i] = bkg[i].p2*bkg[i].c*sigSc/probPow[mesIdx] ; 
    }
    for ( size_t i =0 ; i< data.size() ;  i++ ) {
        x1[i] = data[i].time ; 
        y1[i] = data[i].p2*data[i].c*sigSc/probPow[mesIdx] ; 
    }


    // Open Root file to store result
    string hfile = hfolder + hFileName + ".root" ;
    TFile* theFile = new TFile( hfile.c_str() , "RECREATE" );
    theFile->cd() ;

    gStyle->SetOptFit(1011);
    TCanvas* c0 = new TCanvas("c_0","", 800, 700);
    c0->SetFillColor(10);
    c0->SetFillColor(10);
    gPad->SetGridx();
    gPad->SetGridy();
    //c0->SetLogy() ;

    // raw data plot
    TGraph* g0 = new TGraph( sz1, x1 , y1 ) ;
    g0->SetName("RawData") ;
    g0->SetTitle("Raw Data") ;
    g0->SetMarkerStyle(20) ;
    g0->SetMarkerSize(0.5) ;
    g0->SetMarkerColor(4) ;
    g0->Draw("AP") ;    
    g0->Write() ;
    c0->Update() ;


    // No signal graph
    TGraph* g1 = new TGraph( sz, x , y ) ;
    g1->SetName("NoSignal") ;
    g1->SetTitle("No Signal") ;
    g1->SetMarkerStyle(21) ;
    g1->SetMarkerSize(0.5) ;
    g1->Draw("SAMEP") ;    
    g1->Write() ;
    c0->Update() ;

    /*
    TF1* bg1 = new TF1("bg1",  &MathTools::fitExp , 120 , 300, nPara ) ;
    bg1->SetParameter(0, para[0] ) ;
    bg1->SetParameter(1, para[1] ) ;
    bg1->SetParLimits(1, 0.5*para[1], 1.2*para[1] ) ;
    g1->Fit("bg1", "MRW", "sames", 120, 300 ) ;
    bg1->Draw("sames") ;
    
    TF1* bg2 = new TF1("bg2",  &MathTools::fitExp , 300 , 600, nPara ) ;
    bg2->SetParLimits(0, 0., 20*para[2] ) ;
    bg2->SetParameter(0, para[2] ) ;
    bg2->SetParameter(1, para[3] ) ;
    g1->Fit("bg2", "MR", "sames", 340, 600 ) ;
    bg2->Draw("sames") ;
    */

    // Fit the backgroud
    //TF1* bg0 = new TF1("bg0",  &MathTools::fitExp , 5 , 1000, nPara ) ;
    TF1* bg0 = new TF1("bg0",  &MathTools::fitExp1 , funcR[0] , funcR[1], nPara ) ;

    for ( int j=0; j < nPara; j++ ) {  
        bg0->SetParameter(j, para[j] ) ;
    }
    for (size_t k=0; k< fixP.size(); k++ ) {
        bg0->FixParameter( fixP[k], para[ fixP[k] ] ) ;
    }

    //g1->Fit("bg0", "MR", "sames", 5, 580 ) ;
    g1->Fit("bg0", "MR", "sames", bkgR[0], bkgR[1] ) ;
    bg0->Draw("sames") ;

    c0->Update() ;

    string plotname = hfolder + "bkgFit" + mesName +  ".png" ;
    c0->Print( plotname.c_str() ) ;

    // Subtract background 
    double r, b ;
    double t[sz1], s[sz1] ; 
    for ( size_t i=0; i < data.size() ; i++ ) {
        t[i] = data[i].time ;
        if ( t[i] < 0 ) continue ;
        r = data[i].p2*bkg[i].c*sigSc/probPow[mesIdx]   ; 
        b = bg0->Eval( t[i] ) ;
        //printf(" %.4f - %.4f = %.4f  \n", r, b, r-b ) ;
        s[i] = r - b ; 
    }

    c0->cd();
    TGraph* g2 = new TGraph( sz1, t , s ) ;
    g2->SetName("NoBkg") ;
    g2->SetTitle("No Background") ;
    g2->SetLineColor(4) ;
    g2->Draw("AC") ;    
    g2->Write() ;

    c0->Update() ;
    plotname = hfolder + "Signal_" + mesName + ".png" ;
    c0->Print( plotname.c_str() ) ;

    // Get noise distribution
    c0->Clear() ; 
    TH1D* h_ns  = new TH1D("h_ns", "noise distribution", 40, 0., 0.01 )   ;
    for ( int i=0; i < sz ; i++ ) {
        if ( x[i] < 0 ) continue ;
        b = bg0->Eval( x[i] ) ;
        //printf(" %.4f - %.4f = %.4f  \n", r, b, r-b ) ;
        h_ns->Fill( fabs(y[i] - b) ) ;
    }

    h_ns->Fit("gaus", "R", "sames", 0, 0.005) ;
    h_ns->Write() ;
    h_ns->Draw() ;
    
    c0->Update() ;
    plotname = hfolder + "Noise_" + mesName + ".png" ;
    c0->Print( plotname.c_str() ) ;


    theFile->Close() ;

}

void TestRun::GetDataFromMES( vector<mes>& data ) {

    // Read data from MES file
    string mesfile = mfolder + mesFileName ;
    int err = Input->ReadMES( mesfile, data ) ;
    printf(" data size = %d , error code = %d\n", (int)data.size(), err ) ;
    // trim the data in proper range 
    DataRemoval( data , -99, dataR[0] ) ; 
    DataRemoval( data , dataR[1], 9999999 ) ; 
    // Read Probe Power
    vector<double> probPow ;
    Input->ReadMESInfo( mesfile, "Probe Power", probPow ) ;
    //Input->ReadMESInfo( mesfile, "ProbePower(mW)", probPow ) ;

    // keep the original data 
    vector<mes> data0 = data ; 
    ReduceSize( data, dataSc, mesIdx ) ;

    for ( size_t i =0 ; i< data.size() ;  i++ ) {
        data[i].p2 = data[i].p2*data[i].c*sigSc/probPow[mesIdx] ; 
    }
    
}

void TestRun::RawData() {

    vector<mes> data ;
    GetDataFromMES( data ) ;

    // Get the data and background arrary
    const int sz = (int) data.size() ;
    double x[sz] , y[sz] ;
    for ( size_t i =0 ; i< data.size() ;  i++ ) {
        x[i] = data[i].time ; 
        y[i] = data[i].p2 ; 
    }

    // Setup plot name
    string mesName = mesFileName.substr(0, mesFileName.size()-4 ) ; 

    // Open Root file to store result
    string hfile = hfolder + hFileName + ".root" ;
    TFile* theFile = new TFile( hfile.c_str() , "RECREATE" );
    theFile->cd() ;

    gStyle->SetOptFit(1011);
    TCanvas* c0 = new TCanvas("c_0","", 800, 700);
    c0->SetFillColor(10);
    c0->SetFillColor(10);
    gPad->SetGridx();
    gPad->SetGridy();
    //c0->SetLogy() ;

    // raw data plot
    TGraph* g0 = new TGraph( sz, x , y ) ;
    g0->SetName("RawData") ;
    g0->SetTitle("Raw Data") ;
    g0->SetMarkerStyle(20) ;
    g0->SetMarkerSize(0.5) ;
    g0->SetMarkerColor(4) ;
    g0->Draw("AP") ;    
    g0->Write() ;
    c0->Update() ;

    string plotname = hfolder + "RawData_" + mesName + ".png" ;
    c0->Print( plotname.c_str() ) ;
    theFile->Close() ;

}

void TestRun::BackgroundTune() {

    cout<<" fk1 "<<endl ;
    vector<mes> bkg ;
    GetDataFromMES( bkg ) ;

    cout<<" fk2 "<<endl ;
    // Remove the signal peaks
    for (size_t j=0; j< sigR.size() ; j=j+2 ) {
        DataRemoval( bkg , sigR[j], sigR[j+1] ) ; 
    }

    // Get the background arrary
    const int sz = (int) bkg.size() ;
    double x[sz] , y[sz] ;
    for ( size_t i =0 ; i< bkg.size() ;  i++ ) {
        x[i] = bkg[i].time ; 
        y[i] = bkg[i].p2 ; 
    }

    cout<<" fk3 "<<endl ;
    // Open Root file to store result
    string hfile = hfolder + hFileName + ".root" ;
    TFile* theFile = new TFile( hfile.c_str() , "RECREATE" );
    theFile->cd() ;

    gStyle->SetOptFit(1011);
    TCanvas* c0 = new TCanvas("c_0","", 800, 700);
    c0->SetFillColor(10);
    c0->SetFillColor(10);
    gPad->SetGridx();
    gPad->SetGridy();
    //c0->SetLogy() ;

    cout<<" fk4 "<<endl ;
    // Background / No signal graph
    TGraph* g1 = new TGraph( sz, x , y ) ;
    g1->SetName("NoSignal") ;
    g1->SetTitle("No Signal") ;
    g1->SetMarkerStyle(21) ;
    g1->SetMarkerSize(0.5) ;
    g1->Draw("AP") ;    
    g1->Write() ;
    c0->Update() ;

    // Setup fitting function
    TF1* bg0 = new TF1("bg0",  &MathTools::fitExp1 , funcR[0] , funcR[1], nPara ) ;

    for ( int j=0; j < nPara; j++ ) {  
        bg0->SetParameter(j, para[j] ) ;
    }
    for (size_t k=0; k< fixP.size(); k++ ) {
        bg0->FixParameter( fixP[k], para[ fixP[k] ] ) ;
    }

    cout<<" fit 0 "<<endl ;
    g1->Fit("bg0", "MR", "sames", bkgR[0], bkgR[1] ) ;
    bg0->Draw("sames") ;
    c0->Update() ;

    cout<<" fit 1 "<<endl ;
    TF1* bg1 = new TF1("bg1",  &MathTools::fitExp1 , funcR[0] , 350, 3 ) ;
    bg1->FixParameter(0, para[0] ) ;
    bg1->FixParameter(1, para[1] ) ;
    bg1->FixParameter(2, para[2] ) ;
    //bg1->SetParLimits(1, 0.5*para[1], 1.2*para[1] ) ;
    g1->Fit("bg1", "MRW", "sames", funcR[0], funcR[1] ) ;
    bg1->Draw("sames") ;
    
    cout<<" fit 2 "<<endl ;
    TF1* bg2 = new TF1("bg2",  &MathTools::fitExp , 350 , funcR[1], 3 ) ;
    //bg2->SetParLimits(0, 0., 20*para[2] ) ;
    bg2->FixParameter(0, para[0] ) ;
    bg2->FixParameter(1, para[2] ) ;
    bg2->FixParameter(2, para[3] ) ;
    g1->Fit("bg2", "MR", "sames", 340, 600 ) ;
    bg2->Draw("sames") ;

    // Setup plot name
    string mesName = mesFileName.substr(0, mesFileName.size()-4 ) ; 
    string plotname = hfolder + "bkgFit" + mesName +  ".png" ;
    c0->Print( plotname.c_str() ) ;

}

// Remove Data between LowCut and UpCut of time
void TestRun::DataRemoval( vector<mes>& data, double LowCut, double UpCut ) {

     int sz = (int) data.size() ;
     double x = 0  ;
     for (int i=0; i < sz; i++) {
         x = data[i].time ;    

         if ( x > LowCut && x < UpCut ) {
            data.erase( data.begin() + i ) ;
            sz = sz -1 ;
            i-- ;
         } 
        
     }

}

// Reduce data size with the factor, n -> n/factor 
// Retrieve the data with the measurement idx, if idx = -1 (by default), keep all the measurements 
void TestRun::ReduceSize( vector<mes>& data, int factor, int idx ){

    vector<mes> tmpV ; 

    for ( size_t i=0; i< data.size() ; i++) {
        if ( idx > -1 && data[i].idx != idx ) continue ;
        if ( i%factor == 0 ) {
          tmpV.push_back( data[i] ) ;
        }
    }    
    
    printf(" Old size = %d , New Size = %d  \n", (int)data.size(), (int)tmpV.size() ) ;

    data.clear() ;
    data = tmpV ;

}

