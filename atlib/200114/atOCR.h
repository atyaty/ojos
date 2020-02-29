#ifndef ATOCR_H
#define ATOCR_H


#include "iostream"
#include <gtkmm.h>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <at_File.h>
#include <at_Konfiguracja.h>

using namespace std;
using namespace cv;

class atOCR_Pole {
private:
    Mat *imgGray;
    int rowMin,rowMax;
    int colMin,colMax;
    at_Konfiguracja * cnf;
    string tekst;

public:
    atOCR_Pole( int minR, int maxR, int minC, int maxC, Mat *i,at_Konfiguracja * c){rowMin=minR;rowMax=maxR;colMin=minC;colMax=maxC;imgGray=i;cnf=c;}
    void analiza();
};

//------------------------
class atOCR_Wiersz {
private:
    Mat *imgGray;
    int rowMin,rowMax;
    at_Konfiguracja * cnf;
    vector <uchar> cols;
    vector <atOCR_Pole> pola;

public:
    atOCR_Wiersz( int min, int max, Mat *i,at_Konfiguracja * c){rowMin=min;rowMax=max;imgGray=i;cnf=c;}
    void analizaKolumn();
};

//------------------------
class atOCR {
    private:
    string filename;                   //nazwa plkiu do analizy
    cv::Mat imgOryg,imgGray;
    at_Konfiguracja * cnf;

    vector <uchar> rows;
    vector <atOCR_Wiersz> wiersze;

    double rowPoziom;         //poziom do rozpoznawania ramek Horyzontalnych w analizie Wertykalnej
    double colPoziom;         //poziom do rozpoznawania ramek Wertykalnych w analizie Horyzontalnej wiersza
    int rowSzerokosc;         //szerokość piku interpretowanego jako ramka

public:
    atOCR();
    void atInit(at_Konfiguracja * c);
    void wczytaj(string f);
    void analizaVertykalna();
    void analizaWiersza( int min, int max, Mat *img);
};

#endif // ATOCR_H
