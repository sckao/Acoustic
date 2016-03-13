#ifndef MathTools_H
#define MathTools_H

#include "TObject.h"
#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>
#include <TMath.h>
#include <TF1.h>
//#include <TLorentzVector.h>

using namespace std;

class MathTools {

public:

  MathTools( ) { } ;
  ~MathTools() ;

  static pair<double,double> EffError( double N_all, double N_pass ) ;
  static pair<double,double> ErrAxB( double A, double B, double u_A = -1, double d_A = -1, double u_B = -1, double d_B = -1 ) ;
  static pair<double,double> ErrAovB( double A, double B, double u_A = -1, double d_A = -1, double u_B = -1, double d_B = -1 ) ;
  static pair<double,double> ErrApnB( double A, double B, double u_A = -1, double d_A = -1, double u_B = -1, double d_B = -1 ) ;
  static pair<double,double> StatErr( double m ) ;

  static Double_t BinomialErr( Double_t* x, Double_t* par ) ;
  static Double_t fExp(Double_t *v, Double_t *par) ;
  static Double_t fitGS(Double_t *v, Double_t *par) ;
  static Double_t fitPoly(Double_t *v, Double_t *par) ;
  static Double_t fitExp(Double_t *v, Double_t *par) ;
  static Double_t fitExp1(Double_t *v, Double_t *par) ;

private:



};

//#if !defined(__CINT__)
//    ClassImp(MathTools);
#endif
