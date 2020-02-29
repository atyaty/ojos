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
    int wierszMin,wierszMax;
    int colMin,colMax;
    at_Konfiguracja * cnf;
    string tekst;

public:
    atOCR_Pole( int minR, int maxR, int minC, int maxC, Mat *i,at_Konfiguracja * c){wierszMin=minR;wierszMax=maxR;colMin=minC;colMax=maxC;imgGray=i;cnf=c;}
    void analiza();
    int getWierszMin() {return wierszMin;}
    int getWierszMax() {return wierszMax;}
    int getColMin() {return colMin;}
    int getColMax() {return colMax;}
    string getTekst() {return tekst;}
    int getColSize() {return colMax-colMin;}
    int getRowSize() {return wierszMax-wierszMin;}
};

//------------------------
class atOCR_Wiersz {
private:
    Mat *imgGray;
    int wierszMin,wierszMax;//zakres wiersza
    int minCol,maxCol; //maxymalne wartosci w falbance

    vector <atOCR_Pole> pola;
    vector <uchar> cols;
    atOCR * ocr;
public:
    atOCR_Wiersz( int min, int max, atOCR* o);
    void analizaKolumn();
    void sumowanieKolumn();
    unsigned long  getSize() {return pola.size();}
    atOCR_Pole *getPole(unsigned long i) {return &(pola[i]); }
    unsigned long  getColsSize()           { return cols.size();}
    uchar          getCol(unsigned long i) { return cols[i]; }
    int getColGranica();
    int getWierszMin() {return wierszMin;}
    int getWierszMax() {return wierszMax;}
    void setMaxRow(int r){ wierszMax=r;}    //metoda służy do zmiany zakresu w istniejącym objekcie

};
//------------------------
class atOCR_Vertical {
private:
    Mat* imgGray;
    int minRow, maxRow; //maxymalne wartości w falbance wiersza
    int verticalMin,verticalMax; //zakres verticala
    vector <atOCR_Wiersz> wiersze;
    vector <uchar> rows;
    atOCR * ocr;
public:

    atOCR_Vertical(int min, int max, atOCR* o);
    void     analizaWierszy();
    void     sumowanieWierszy();
    void     porownaj(atOCR_Vertical* v);
    unsigned long  getSize() {return wiersze.size();}
    atOCR_Wiersz*  getWiersz(unsigned long i) {return &(wiersze[i]); }
    unsigned long  getRowsSize() {return rows.size();}
    uchar          getRow(unsigned long i) {return rows[i]; }

    //int getMinRow()    {return minRow;}
    //int getMaxRow()    {return maxRow;}
    int getRowGranica();
    int getVerticalMin() { return verticalMin;}
    int getVerticalMax() { return verticalMax;}


};
//------------------------
class atOCR_Garb {
private:
    int nr ;
    vector <atOCR_Vertical*> verticale; //tablica verticali w których pi
    vector <unsigned> nr_piku;             //nr piku (wiersza) w ramach odpowiedniego verticala z powiązanego vectora
    vector <int> value;                 //wartość
public:

    atOCR_Garb();
    void addVertical( atOCR_Vertical* v);


};
//------------------------
class atOCR {
    private:
    string filename;                   //nazwa plkiu do analizy
    cv::Mat imgOryg;

    at_Konfiguracja * cnf;
    double rowPoziom;         //poziom do rozpoznawania ramek Horyzontalnych w analizie Wertykalnej
    double colPoziom;         //poziom do rozpoznawania ramek Wertykalnych w analizie Horyzontalnej wiersza
    int rowSzerokosc;         //szerokość piku interpretowanego jako ramka
    int colSzerokosc;
    int podzialVertykalny; //ilość vertykali wstepna

    cv::Mat imgGray;
    int minRow, maxRow; //maxymalne wartości w falbance wiersza
    vector <atOCR_Wiersz>   wiersze;
    vector <atOCR_Vertical> verticale;
    vector <atOCR_Garb> garby;
    vector <uchar> rows;



public:

    atOCR();
    void     atInit(at_Konfiguracja * c);
    void     wczytaj(string f);
    void     analizaVertykalna();
    void     analizaVertykali();
    void     sumowanieVertykali();
    void     porownanieVertykali();
    unsigned long  getSize() {return wiersze.size();}
    atOCR_Wiersz*  getWiersz(unsigned long i) {return &(wiersze[i]); }
    unsigned long  getRowsSize() {return rows.size();}
    uchar          getRow(unsigned long i) {return rows[i]; }
    double getRowPoziom()   {return rowPoziom;}
    double getColPoziom()    {return colPoziom; }
    int    getRowSzerokosc(){return rowSzerokosc;}
    int    getColSzerokosc() {return colSzerokosc;}
    unsigned long  getVerticaleSize(){ return verticale.size();}
    atOCR_Vertical*  getVertical(unsigned long i) {return &(verticale[i]); }

    int getMinRow()    {return minRow;}
    int getMaxRow()    {return maxRow;}
    int getRowGranica(){return (double)maxRow*rowPoziom;}
    cv::Mat* getImageGray() {return &imgGray;}
    at_Konfiguracja* getCnf(){return cnf;}

    void addGarb(atOCR_Wiersz* w);
};


#endif // ATOCR_H
