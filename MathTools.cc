#include "MathTools.h"

// return asymmetry errors <upward,downward>
pair<double, double> MathTools::EffError( double N_all, double N_pass ) {

    if ( N_all < 0.0001 ) {
       pair<double,double> noErr  = make_pair( 0 , 0 );
       return noErr ;
    }

    double eff0 = N_pass / N_all ;
    pair<double,double> theErr ;
    if ( eff0 > 1 ) {
       theErr = make_pair( 0 , 0 );
       return theErr ;
    }
    //cout<<" N_All: "<<N_all <<"  N_pass: "<< N_pass << endl;
    int nStep = 1000 ;
    double step = 1. / nStep ;
    //cout<<" step = "<< step <<endl;
    Double_t par[3] = { 1, N_all, N_pass } ;
    Double_t xL[1] = { eff0 } ;
    Double_t xR[1] = { eff0 } ;
    double IntEff = (N_all+ 1) * step * BinomialErr( xR, par ) ;
    //cout<<" Eff 0th : "<< BinomialErr( xR, par ) << endl ;  

    bool skipR = false ;
    bool skipL = false ;
    Double_t pR = 0 ;
    Double_t pL = 0 ;
    while ( IntEff < 0.683)  {
        if ( !skipR && xR[0] < 1 ) {
           xR[0] +=  step ;
           pR = BinomialErr( xR, par ) ;
           IntEff += (pR*step*(N_all+1) ) ;
           //cout<<" ("<< xR[0] <<") --> R : "<< IntEff <<"  pR = "<< pR <<endl ;
        }
        if ( !skipL && xL[0] > 0 && xL[0] > step ) {
           xL[0] -=  step ;
           pL = BinomialErr( xL, par ) ;
           IntEff += (pL*step*(N_all+1) ) ;
           //cout<<" ("<< xL[0] <<") <-- L : "<< IntEff <<"  pL = "<< pL <<endl;
        }
        //cout<<" ------ "<<endl; 
        skipR = ( pL > pR ) ? true : false ;
        skipL = ( pL < pR ) ? true : false ;
        if ( pL == pR ) {
           skipR = false ;
           skipL = false ;
        }
    }
    //cout<<"  ["<< N_pass/N_all <<"] Prob = "<< IntEff <<endl ; 
    //cout<<"                 - "<< (N_pass/N_all) - xL[0] <<endl ;
    //cout<<"                 + "<< xR[0] - (N_pass/N_all) <<endl ;
    theErr      = make_pair( xR[0] - eff0 , eff0 - xL[0] );

    return theErr ;
}


Double_t MathTools::BinomialErr( Double_t* x, Double_t* par ) {

  Double_t N_all  = par[1] ;
  Double_t N_pass = par[2] ;

  //Double_t Bxy = TMath::Beta( ( N_pass + 1 ), ( N_all - N_pass + 1 ) ) ;
  //cout<< " Beta(x,y) = "<< Bxy <<endl ;
  //Double_t Cnk = pow(x[0], N_pass ) * pow( (1-x[0]) , (N_all - N_pass) ) ;
  //Double_t prob = par[0]*Cnk / ( Bxy * (N_all + 1. ) );

  double betaPDF = TMath::BetaDist( x[0],  (N_pass + 1) , (N_all - N_pass + 1) ) ;
  Double_t prob = (par[0] / (N_all + 1.) ) * betaPDF ;
  //cout<<" x = "<< x[0] <<" betaPDF:"<<  betaPDF << " p = "<< prob <<endl ;

  if ( x[0] < 0 || x[0] > 1 ) prob = 0 ;

  return prob ;

}


// <upward, downward> errors
pair<double,double>  MathTools::StatErr( double m ){

  pair<double,double> pErr ;
  if ( m < 1. ) {
     pErr = make_pair( m, m );
  }
  else if ( m > 25. ) {
     pErr = make_pair( sqrt(m), sqrt(m) );
  }
  else {

     double step = 0.01 ;

     // -34%
     double k = m ;
     double lm = 0. ;
     double pp = 0. ;
     while (lm <= 0.34 || k < 0 ) {
          k = k - step ;
          pp = TMath::Poisson( k, m );
          lm = lm + (pp*step) ;
     }
     // +34%
     double j = m ;
     double hm = 0 ;
     double hp = 0 ;
     while ( hm <=0.34 || j < 0 ) {
           j = j + step ;
           hp = TMath::Poisson( j, m );
           hm = hm + (hp*step) ;
           //cout<<" j = "<< j <<" , p = "<< hp <<" int_P = "<< hm <<endl;
     }
     pErr      = make_pair( k-m, m-j );
  }
  return pErr ;

}

pair<double,double> MathTools::ErrAxB( double A, double B, double u_A, double d_A, double u_B, double d_B ){

    pair<double,double> sA = StatErr( A ) ;
    double sAp = (u_A != -1 ) ? u_A : sA.first  ;
    double sAn = (d_A != -1 ) ? d_A : sA.second ;
    pair<double,double> sB = StatErr( B ) ;
    double sBp = (u_B != -1 ) ? u_B : sB.first  ;
    double sBn = (d_B != -1 ) ? d_B : sB.second ;

    //double f = A * B ;
    double s_fp = sqrt( B*B*sAp*sAp + A*A*sBp*sBp ) ;
    double s_fn = sqrt( B*B*sAn*sAn + A*A*sBn*sBn ) ;

    pair<double,double> sf = make_pair( s_fp, s_fn ) ;
    return sf ;
}

pair<double,double> MathTools::ErrAovB( double A, double B, double u_A, double d_A, double u_B, double d_B ){

    pair<double,double> sA = StatErr( A ) ;
    double sAp = (u_A != -1 ) ? u_A : sA.first  ;
    double sAn = (d_A != -1 ) ? d_A : sA.second ;
    pair<double,double> sB = StatErr( B ) ;
    double sBp = (u_B != -1 ) ? u_B : sB.first  ;
    double sBn = (d_B != -1 ) ? d_B : sB.second ;

    double f = A / B ;
    double s_fp = sqrt( (sAp*sAp) + (f*f*sBp*sBp) ) / B ;
    double s_fn = sqrt( (sAn*sAn) + (f*f*sBn*sBn) ) / B ;

    pair<double,double> sf = make_pair( s_fp, s_fn ) ;
    return sf ;

}

pair<double,double> MathTools::ErrApnB( double A, double B, double u_A, double d_A, double u_B, double d_B ){

    pair<double,double> sA = StatErr( A ) ;
    double sAp = (u_A != -1 ) ? u_A : sA.first  ;
    double sAn = (d_A != -1 ) ? d_A : sA.second ;
    pair<double,double> sB = StatErr( B ) ;
    double sBp = (u_B != -1 ) ? u_B : sB.first  ;
    double sBn = (d_B != -1 ) ? d_B : sB.second ;

    // f = a+b or a-b
    double s_fp =  sqrt( (sAp*sAp) + (sBp*sBp) ) ;
    double s_fn =  sqrt( (sAn*sAn) + (sBn*sBn) ) ;

    pair<double,double> sf = make_pair( s_fp, s_fn ) ;
    return sf ;
}

Double_t MathTools::fExp(Double_t *v, Double_t *par) {
      Double_t arg = v[0] /par[1];

      Double_t fitval = par[0]*TMath::Exp( -1*arg);
      return fitval;
}

Double_t MathTools::fitGS(Double_t *x, Double_t *par) {

     Double_t gs_Value = TMath::Gaus(x[0],par[1],par[2]) ;
     Double_t fitV = par[0]*gs_Value ;
     return fitV;
}


Double_t MathTools::fitPoly(Double_t *x, Double_t *par) {

         Double_t fitval =  par[0]
                          + (par[1]* x[0] )
                          + (par[2]* x[0]*x[0]  )
                          + (par[3]* x[0]*x[0]*x[0] );

         return fitval;
}

Double_t MathTools::fitExp(Double_t *x, Double_t *par) {

         Double_t x1 = -1*x[0]/par[2] ;
         Double_t x2 = -1*x[0]/par[4] ;
         Double_t x3 = -1*x[0]/par[6] ;

         Double_t fitval =  par[0]   
                          + ( par[1]*exp( x1 ) ) 
                          + ( par[3]*exp( x2 ) ) 
                          + ( par[5]*exp( x3 ) ) ;

         return fitval;
}


Double_t MathTools::fitExp1(Double_t *x, Double_t *par) {

         Double_t x1 = -1*x[0]/par[2] ;
         Double_t x2 = -1*x[0]*x[0]/par[4] ;

         Double_t fitval =  par[0]   
                          + ( par[1]*exp( x1 ) ) 
                          + ( par[3]*exp( x2 ) )  ;

         return fitval;
}
