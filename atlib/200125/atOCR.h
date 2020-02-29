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
class atOCR;
typedef atOCR atOCR;

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
    int getRowMin() {return rowMin;}
    int getRowMax() {return rowMax;}
    int getColMin() {return colMin;}
    int getColMax() {return colMax;}
    string getTekst() {return tekst;}
    int getColSize() {return colMax-colMin;}
    int getRowSize() {return rowMax-rowMin;}
};

//------------------------
class atOCR_Wiersz {
private:
    Mat *imgGray;
    int rowMin,rowMax;
    int minCol,maxCol;
   // at_Konfiguracja * cnf;
    vector <atOCR_Pole> pola;
    vector <uchar> cols;
    atOCR * ocr;
public:
   // atOCR_Wiersz( int min, int max, Mat *i,at_Konfiguracja * c){colMin=min;colMax=max;imgGray=i;cnf=c;}
    atOCR_Wiersz( int min, int max, atOCR* o);
    void analizaKolumn();
    unsigned long  getSize() {return pola.size();}
    atOCR_Pole *getPole(unsigned long i) {return &(pola[i]); }
    unsigned long  getColsSize()           { return cols.size();}
    uchar          getCol(unsigned long i) { return cols[i]; }
    int getColGranica();
    int getRowMin() {return rowMin;}

};

//------------------------
class atOCR {
    private:
    string filename;                   //nazwa plkiu do analizy
    cv::Mat imgOryg,imgGray;
    at_Konfiguracja * cnf;

    vector <atOCR_Wiersz> wiersze;

    double rowPoziom;         //poziom do rozpoznawania ramek Horyzontalnych w analizie Wertykalnej
    double colPoziom;         //poziom do rozpoznawania ramek Wertykalnych w analizie Horyzontalnej wiersza
    int rowSzerokosc;         //szerokość piku interpretowanego jako ramka
    int colSzerokosc;

    vector <uchar> rows;
    int minRow;
    int maxRow;


public:

    atOCR();
    void     atInit(at_Konfiguracja * c);
    void     wczytaj(string f);
    void     analizaVertykalna();
    void     analizaWiersza( int min, int max, Mat* img);
    unsigned long  getSize() {return wiersze.size();}
    atOCR_Wiersz*  getWiersz(unsigned long i) {return &(wiersze[i]); }
    unsigned long  getRowsSize() {return rows.size();}
    uchar          getRow(unsigned long i) {return rows[i]; }
    double getRowPoziom() {return rowPoziom;}
    double getColPoziom()    {return colPoziom; }
    double getColSzerokosc() {return colSzerokosc;}

    int getMinRow()    {return minRow;}
    int getMaxRow()    {return maxRow;}
    int getRowGranica(){return (double)maxRow*rowPoziom;}
    cv::Mat* getImageGray() {return &imgGray;}
    at_Konfiguracja* getCnf(){return cnf;}
};

#endif // ATOCR_H
