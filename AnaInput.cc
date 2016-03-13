#include "AnaInput.h"

AnaInput* AnaInput::m_Instance = NULL ;

//AnaInput::AnaInput( string datacardInput ) {
AnaInput::AnaInput() {

  datacardfile = "DataCard.txt" ;

}

AnaInput::~AnaInput(){

   cout<<" close input "<<endl ;

}

AnaInput* AnaInput::Instance() {

    if (! m_Instance ) {
       m_Instance = new AnaInput( ) ;
    } 
       
    return m_Instance ;

}

void AnaInput::SetDatacard( string datacardInput ) {
 
     datacardfile = datacardInput ;
}


// Methods to read DataCard.txt
void AnaInput::GetParameters(string paraName, int* thePara, string cfgFile ){

     if ( cfgFile == "" ) cfgFile = datacardfile ;

     fstream paraFile( cfgFile.c_str() );
     if ( !paraFile.is_open() )  cout<<" file opened error => check file path and the folder "<<endl;
     string  line;
     string  getName;
     string  getValue;
     size_t  pos ;
     size_t  vpos ;

     bool gotIt = false ;
     while ( getline(paraFile, line) ) {
           if ( line[0] == '#' ) continue ;

           pos = line.find( paraName );
           vpos = pos + paraName.size() + 2;
           if ( pos < line.npos ) {
              string str_end = line.substr(vpos-1, 1) ;
              if ( str_end == ' ' || str_end == '=') {
                 getName  = line.substr( pos, paraName.size() );
                 getValue = line.substr( vpos );
                 *thePara = atoi( getValue.c_str() );
                 //cout<< paraName <<" = "<< *thePara << endl;
                 gotIt = true;
              }
           }
           if ( gotIt ) break ;
     }
     paraFile.close();
}

void AnaInput::GetParameters(string paraName, double* thePara, string cfgFile ){

     if ( cfgFile == "" ) cfgFile = datacardfile ;

     fstream paraFile( cfgFile.c_str() );
     if ( !paraFile.is_open() )  cout<<" file opened error => check file path and the folder "<<endl;
     string  line;
     string  getName;
     string  getValue;
     size_t  pos ;
     size_t  vpos ;

     bool gotIt = false ;
     while ( getline(paraFile, line) ) {
           if ( line[0] == '#' ) continue ;

           pos = line.find( paraName );
           vpos = pos + paraName.size() + 2;
           // exclude the case when this paraName is the sub-string of other paraName 
           if ( pos > 0 && pos < 999 && line[pos-1] != ' ' ) continue ;

           if ( pos < line.npos ) {
              string str_end = line.substr(vpos-1, 1) ;
              if ( str_end == ' ' || str_end == '=') {
                 getName  = line.substr( pos, paraName.size() );
                 getValue = line.substr( vpos );
                 *thePara = atof( getValue.c_str() );
                 //cout<< paraName <<" = "<< *thePara << endl;
                 gotIt = true ;
              }
           }
           if ( gotIt ) break ;
     }
     paraFile.close();
}

void AnaInput::GetParameters(string paraName, string* thePara, string cfgFile ){

     if ( cfgFile == "" ) cfgFile = datacardfile ;

     fstream paraFile( cfgFile.c_str() );
     if ( !paraFile.is_open() )  cout<<" file opened error => check file path and the folder "<<endl;
     string  line;
     string  getName;
     size_t  pos ;
     size_t  vpos ;

     bool gotIt = false ;
     while ( getline(paraFile, line) ) {
           if ( line[0] == '#' ) continue ;

           pos = line.find( paraName );
           vpos = pos + paraName.size() + 2;
           // exclude the case when this paraName is the sub-string of other paraName 
           if ( pos > 0 && pos < 999 && line[pos-1] != ' ' ) continue ;

           if ( pos < line.npos ) {
              string str_end = line.substr(vpos-1, 1) ;
              if ( str_end == ' ' || str_end == '=') {
                 //cout<<" pos = "<< pos <<endl;
                 getName  = line.substr( pos, paraName.size() );
                 //*thePara = line.substr( vpos );
                 //cout<< paraName <<" = "<< *thePara << endl;
                 string strTmp = line.substr( vpos );
                 for (string::iterator it = strTmp.begin(); it< strTmp.end(); it++) {
                     if ( (*it) != ',' && (*it) != ' ' && (*it) != '(' && (*it) != ')' && (*it) != '=') thePara->push_back( *it );
                 }
                 gotIt = true ;
              }
           }
           if ( gotIt ) break;
     }
     paraFile.close();
}

void AnaInput::GetParameters(string paraName, vector<double>* thePara, string cfgFile ){

     if ( cfgFile == "" ) cfgFile = datacardfile ;

     fstream paraFile( cfgFile.c_str() );
     if ( !paraFile.is_open() )  cout<<" file opened error => check file path and the folder "<<endl;
     string  line;
     string  getName;
     string  getValue;
     size_t  pos(0) ;
     size_t  vpos(0) ;
     vector<double>  vvec;

     while ( getline(paraFile, line) ) {
           if ( line[0] == '#' ) continue ;

           pos = line.find( paraName );
           vpos = pos + paraName.size() + 1;
           // exclude the case when this paraName is the sub-string of other paraName 
           if ( pos > 0 && pos < 999 && line[pos-1] != ' ' ) continue ;

           if ( pos < line.npos ) {
              getName  = line.substr( pos, paraName.size() );
              string arrVal = line.substr( vpos );
              if ( arrVal[0] != '=' && arrVal[0] != ' ' ) continue;
	      int vidx = 0;
	      string vtemp ;
	      //cout<< paraName <<" = ( " ;
              for (string::iterator it = arrVal.begin(); it< arrVal.end(); it++) {
                  if ( (*it) != ',' && (*it) != ' ' && (*it) != '(' && (*it) != ')' && (*it) != '=') vtemp.push_back( *it );
                  if ( (*it) == ',' || (*it) == ')' ) { 
                     if ( vtemp.size() > 0 ) vvec.push_back( atof( vtemp.c_str() ) ) ;
		     //cout<< vtemp << *it;
		     vidx++ ;
		     vtemp.clear() ;
                  }
              }
              *thePara = vvec ;
           }
     }
     //cout<<""<<endl ;
     paraFile.close();

} 

void AnaInput::GetParameters(string paraName, vector<string>* thePara, string cfgFile ){

     if ( cfgFile == "" ) cfgFile = datacardfile ;

     fstream paraFile( cfgFile.c_str() );

     if ( !paraFile.is_open() )  cout<<" file opened error => check file path and the folder "<<endl;
     string  line;
     string  getName;
     string  getValue;
     size_t  pos ;
     size_t  vpos ;
     vector<string>  vvec;

     while ( getline(paraFile, line) ) {
           if ( line[0] == '#' ) continue ;

           pos = line.find( paraName );
           vpos = pos + paraName.size() ;
           if ( pos < line.npos ) {
              getName  = line.substr( pos, paraName.size() );
              string arrVal = line.substr( vpos );
              if ( arrVal[0] != '=' && arrVal[0] != ' ' ) continue;
	      int vidx = 0;
	      string vtemp ;
	      //cout<< paraName <<" = ( " ;
              for (string::iterator it = arrVal.begin(); it< arrVal.end(); it++) {
                  if ( (*it) != ',' && (*it) != ' ' && (*it) != '(' && (*it) != ')' && (*it) != '=') vtemp.push_back( *it );
                  if ( (*it) == ',' || (*it) == ')' ) { 
                     if ( vtemp.size() > 0 ) vvec.push_back( vtemp ) ;
		     //cout<< vtemp << *it;
		     vidx++ ;
		     vtemp.clear() ;
                  }
              }
              *thePara = vvec ;
           }
     }
     paraFile.close();

}
 
void AnaInput::GetParameters(string paraName, vector<int>* thePara, string cfgFile ){

     if ( cfgFile == "" ) cfgFile = datacardfile ;

     fstream paraFile( cfgFile.c_str() );
     if ( !paraFile.is_open() )  cout<<" file opened error => check file path and the folder "<<endl;
     string  line;
     string  getName;
     string  getValue;
     size_t  pos ;
     size_t  vpos ;
     vector<int>  vvec;

     while ( getline(paraFile, line) ) {
           if ( line[0] == '#' ) continue ;

           pos = line.find( paraName );
           vpos = pos + paraName.size() ;
           if ( pos < line.npos ) {
              getName  = line.substr( pos, paraName.size() );
              string arrVal = line.substr( vpos );
              if ( arrVal[0] != '=' && arrVal[0] != ' ' ) continue;
	      //int vidx = 0;
	      string vtemp ;
	      //cout<< paraName <<" = ( " ;
              for (string::iterator it = arrVal.begin(); it< arrVal.end(); it++) {
                  if ( (*it) != ',' && (*it) != ' ' && (*it) != '(' && (*it) != ')' && (*it) != '=') vtemp.push_back( *it );
                  if ( (*it) == ',' || (*it) == ')' ) { 
                     if ( vtemp.size() > 0 ) vvec.push_back( atoi( vtemp.c_str() ) ) ;
		     //cout<< vtemp << *it;
		     //vidx++ ;
		     vtemp.clear() ;
                  }
              }
              *thePara = vvec ;
           }
     }
     paraFile.close();

}

 // Read MES file
int AnaInput::ReadMES( string fileName, vector<mes>& data ) {

  // Open data file
  //printf(" File:%s \n", fileName.c_str() ) ;
  fstream logfile( fileName.c_str() );

  // Open MES file to read
  if ( logfile == NULL ) {
      printf(" file open error \n") ;
      return 1 ;
  }
 
  // Variables to hold data 
  float time(0), p1(0), p2(0), p3(0), c1(0) ;
  int fo = -1 ;

  // data collection
  mes entry ;

  string  line;
  double tCut = -999. ;
  int id = 0 ;
  while ( getline( logfile, line) ) {
        if ( line[0] == '/' ) continue ;


        fo = sscanf( line.c_str() , "%f  %f  %f  %f  %f" , &time, &p1, &p2, &p3, &c1 );
        //printf(" read : t[%f] , [%f] , [%f] , [%f] , c[%f] \n", time, p1, p2, p3, c1 ) ;
        //printf(" (%d) %f %f %f \n", id , time, p1, p2 ) ;
 
        if ( time < tCut ) id++ ;
        tCut = time ;

        entry.time = time ;
        entry.p1   = p1 ;      
        entry.p2   = p2 ;      
        entry.p3   = p3 ;      
        entry.c    = c1 ;      
        entry.idx  = id ; 
        data.push_back( entry ) ;

        if ( fo != 5 ) {
           printf(" !!! fscan fail %d \n", fo ) ;
           break;
        }
  }
  //printf(" data size == %d , %d measurement \n", (int)data.size(), id+1 ) ;

  if ( fo !=5 ) { 
     return 2 ;  // read error 
  } else if ( data.size() < 1 )  {
     return 3 ;
  } else if ( data.size()%(id+1) != 0 ) {
     return 4 ;
  } else {
     return 0 ;
  }

}

// Read Calibration and Measurement setup 
void AnaInput::ReadMESInfo( string fileName, string paraName, vector<double>& paraV  ) {

  // Open data file
  //printf(" File:%s \n", fileName.c_str() ) ;
  fstream logfile( fileName.c_str() );

  // Open MES file to read
  if ( logfile == NULL ) {
      printf(" file open error \n") ;
  }
 
  // data collection
  string  line;
  string  getName;
  string  getValue;
  size_t  pos(0) ;
  size_t  vpos(0) ;
  double thePara ;

  int id = 0 ;
  while ( getline( logfile, line) ) {
        if ( line[0] != '/' ) continue ;

        pos = line.find( paraName );
        vpos = pos + paraName.size() + 1 ;
	// exclude the case when this paraName is the sub-string of other paraName 
	if ( pos > 0 && pos < 999 && line[pos-1] != ' ' ) continue ;

        if ( pos < line.npos ) {

           string str_end = line.substr(vpos-1, 1) ;
           if ( str_end == ' ' || str_end == ':') {
              getName  = line.substr( pos, paraName.size() );
              getValue = line.substr( vpos );
              thePara = atof( getValue.c_str() );
              paraV.push_back( thePara ) ; 
              //cout<< paraName <<" = "<< thePara <<" -> "<< id << endl;
           }
           id++ ;
        }
 
  }
  //printf(" data size == %d , %d measurement \n", (int)data.size(), id+1 ) ;


}

// Read MES file - method 1 , using fscanf
// Example method only  - obsolete
void AnaInput::ReadMES1( string fileName ) {

  // Open MES file to read
  FILE* logfile = fopen( fileName.c_str() , "r");
  if ( logfile == NULL ) printf(" file open error \n") ;

  bool eof = false ;
  char cbuf[108] ; // has to be 108, otherwise 1st entry will be ignored
  float time(0), p1(0), p2(0), p3(0), c1(0) ;
  int fo = -1 ;

  vector<double> tV ;
  vector<double> rV ;
  tV.clear() ;
  rV.clear() ;

  while ( !eof ) {

     fgets(cbuf, sizeof(cbuf), logfile) ;
     if ( cbuf[0] == '/') continue;

     fo = fscanf( logfile, "%f  %f  %f  %f  %f" , &time, &p1, &p2, &p3, &c1 );
     printf(" read : t[%f] , [%f] , [%f] , [%f] , c[%f] \n", time, p1, p2, p3, c1 ) ;

     tV.push_back( time ) ;
     rV.push_back( p1 ) ;

     if ( fo != 5 ) {
         printf(" !!! fscan fail %d \n", fo ) ;
         break;
     }

     eof = (feof( logfile) != 0) ? true : false ;
  }

  printf(" size of data = %d , t0 = %f\n", (int)tV.size(), tV[0] ) ;

}


