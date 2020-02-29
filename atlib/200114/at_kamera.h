#ifndef AT_KAMERA_H
#define AT_KAMERA_H

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <at_majak.h>
#include <at_Konfiguracja.h>
//#include <gtkmm.h>

using namespace cv;
using namespace std;

#define ATERROR_CAMERA_OPEN 0x3000
#define ATERROR_CAMERA_READ 0x3001

//---------------------------------------------------------------------------------------------------------------
// objekt zawiera definicje kamery uzywanej do rejestracji obrazow
// tangensy kata widzenia okresla sie w ten sposob ze ustawia sie kamere przed sciana w okreslonej odleglosci: L
// mierzy się szerokość i wysokośc ściany która została przez kamerę uchwycona na obrazie: szerokość, wysokość
// Ztg= wysokość/L
// Xtg= szerokość/L
// rozdzielczość kamery w pionie i w poziomie to ilość pikseli która jest rejestrowana w pionie i w poziomie.
//
//----------------------------------------------------------------------------------------------------------------
class at_kamera {
private:

    Mat frame;
    VideoCapture cap;          //--- INITIALIZE VIDEOCAPTURE
    int deviceID;              // 0 = open default camera
    int apiID;                 // 0 = autodetect default API
    vector<at_majak> majaki;
    //----------------------------------------------------------------
    double Ztg,Xtg;   //tangens kąta widzenia kamery w pionie,poziomie
    int Z,X;          //rozdzielczość pionowa,pozioma - piksele
    double _Ztg,_Xtg; //liczone od środka ekranu Ztg/2
    int _Z,_X;        //liczone od środka ekranu Z/2
    //----------------------------------------------------------
    //parametry procedury GaussianBlur (do detekcji kółek)
    int BlurXs;
    int BlurZs;
    int BlurXdev;
    int BlurZdev;

    //parametry procedury HoughCircle
    int CircleInversRat;
    int CircleminDist;
    int CircleParam1;
    int CircleParam2;
    int CircleminR;
    int CirclemaxR;

    //parametry procedury Canny
    int Cannythres1;
    int Cannythres2;
    int Cannyaperture;

    //parametry do procedury HoughLineP
    int Linerho;
    int Linetheta;
    int Linethres3;
    int LineminLen;
    int LineminGap;
    //-----------------------------------------------------------------
    bool opened;
    int error;
    at_Konfiguracja * cnf;


public:
    at_kamera(int c, int x, int z, double tx, double tz);
    at_kamera(at_Konfiguracja * c,string s);

    int get_X()     { return _X;}
    int get_Z()     { return _Z;}
    double getXtg() { return _Xtg;}
    double getZtg() { return _Ztg;}

    bool IsOpened() { return opened;}
    Mat * getFrame(){ return &frame;}

    void Open();
    Mat * Read();
    void DetectMajaki();
    size_t getN()               { return majaki.size();}
    at_majak getMajak(size_t n) { return majaki.at(n);}

};



#endif // AT_KAMERA_H
