#include "at_kamera.h"

//----------------------------------------------------------------------
at_kamera::at_kamera(int c, int x, int z, double tx, double tz){

    //---------------------------------------------------------
    deviceID = c;
    apiID    = cv::CAP_ANY;       // 0 = autodetect default API
    error    =0;
    //---------------------------------
    //wczytać to z jakiejś konfiguracji
    Xtg = tx;
    Ztg = tz;
    X   = x;
    Z   = z;
    //--------------------------------
    //połówki żeby łatwiej tego uzywać
    _Xtg  = Xtg/2;
    _Ztg  = Ztg/2;
    _X    = X/2;
    _Z    = Z/2;
    //--------------------------------
    opened=false;
}
//----------------------------------------------------------------------
at_kamera::at_kamera(at_Konfiguracja * c,string s){

    //---------------------------------------------------------
    cnf      = c;
    apiID    = cv::CAP_ANY;       // 0 = autodetect default API
    error    = 0;
    opened   = false;
    //--------------------------------
    //kamera
    deviceID        = cnf->getInt(s+".deviceID");
    Xtg             = cnf->getFloat(s+".Xtg");  _Xtg  = Xtg/2;
    Ztg             = cnf->getFloat(s+".Ztg");  _Ztg  = Ztg/2;
    X               = cnf->getInt(s+".X");      _X    = X/2;
    Z               = cnf->getInt(s+".Z");      _Z    = Z/2;
    //--------------------------------
    //parametry procedury GaussianBlur (do detekcji kółek)
    BlurXs          = cnf->getInt(s+".BlurXs");
    BlurZs          = cnf->getInt(s+".BlurZs");
    BlurXdev        = cnf->getInt(s+".BlurXdev");
    BlurZdev        = cnf->getInt(s+".BlurZdev");
    //--------------------------------
    //parametry procedury HoughCircle
    CircleInversRat = cnf->getInt(s+".CircleInversRat");
    CircleminDist   = cnf->getInt(s+".CircleminDist");
    CircleParam1    = cnf->getInt(s+".CircleParam1");
    CircleParam2    = cnf->getInt(s+".CircleParam2");
    CircleminR      = cnf->getInt(s+".CircleminR");
    CirclemaxR      = cnf->getInt(s+".CirclemaxR");
    //--------------------------------
    //parametry procedury Canny
    Cannythres1      = cnf->getInt(s+".Cannythres1");
    Cannythres2      = cnf->getInt(s+".Cannythres2");
    Cannyaperture    = cnf->getInt(s+".Cannyaperture");
    //---------------------------------------
    //parametry do procedury HoughLineP
    Linerho          = cnf->getInt(s+".Linerho");
    Linetheta        = cnf->getInt(s+".Linetheta");
    Linethres3       = cnf->getInt(s+".Linethres3");
    LineminLen       = cnf->getInt(s+".LineminLen");
    LineminGap       = cnf->getInt(s+".LineminGap");
}
//------------------------------------------------------------------------
void at_kamera::Open() {
    cap.open(deviceID + apiID);
    if (!cap.isOpened()) {  error=ATERROR_CAMERA_OPEN;  }
    else                 {  opened=true; }
}

//------------------------------------------------------------------------
Mat *at_kamera::Read() {
    cap.read(frame);
    if (frame.empty()) { error=ATERROR_CAMERA_READ;  }
    return &frame;
}
//-------------------------------------------------------------------------
// należy rozważyć przekazywanie informacji zwrotnej o majakach (potwierdzeń z innych systemów),
// które pozwalałyby znajdować je łatwiej, ew uczyc kamerę znajdowania majaków
//
void at_kamera::DetectMajaki(){
    majaki.clear();
    //----------------------------------------------kółka----------------------------
    Mat gray;
    cvtColor( frame, gray, COLOR_BGR2GRAY );                              // detekcje kółek robimy na szarym rastrze
    GaussianBlur( gray, gray, Size(BlurXs, BlurZs), BlurXdev, BlurZdev ); // Reduce the noise so we avoid false circle detection
    vector<Vec3f> circles;                                                // kółka wektorowo - tablice do przechowywania obiektów zidentyfikowanych w rastrze - prosty format narzucony przez openCV
    HoughCircles( gray, circles, HOUGH_GRADIENT, CircleInversRat, gray.rows/CircleminDist, CircleParam1, CircleParam2, CircleminR, CirclemaxR );    // Apply the Hough Transform to find the circles (progi oryginalne:  200,100)
    for( size_t i = 0; i < circles.size(); i++ ) { majaki.push_back( at_majak(circles[i][0],circles[i][1],circles[i][2])); }
    //--------------------------------------------linie------------------------------------
    /*
    Mat dst;
    Canny( *img, dst, cam0->Cannythres1, cam0->Cannythres2, cam0->Cannyaperture );
    vector<Vec4i> lines;
    HoughLinesP( dst, lines, cam0->Linerho, CV_PI/cam0->Linetheta, cam0->Linethres3, cam0->LineminLen, cam0->LineminGap );
    for( size_t i = 0; i < lines.size(); i++ ) { m0->push_back(at_majak(cam0,lines[i][0],lines[i][1],lines[i][2],lines[i][3]));        }
    */
}
