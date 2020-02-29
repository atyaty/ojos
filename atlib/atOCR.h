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
#include <atOCR_Vertical.h>
#include <atOCR_Wiersz.h>
#include <atOCR_Pik.h>
#include <atOCR_Garb.h>

using namespace std;
using namespace cv;

//------------------------
class atOCR {
    private:
    string filename;                   //nazwa plkiu do analizy
    cv::Mat imgOryg;
    cv::Mat imgGray;

    at_Konfiguracja * cnf;
    double rowPoziom;         //poziom do rozpoznawania ramek Horyzontalnych w analizie Wertykalnej
    double colPoziom;         //poziom do rozpoznawania ramek Wertykalnych w analizie Horyzontalnej wiersza
    int rowSzerokosc;         //szerokość piku interpretowanego jako ramka
    int colSzerokosc;         //
    int podzialVertykalny;    //ilość vertykali wstepna
    unsigned d_Szerokosc;               //do znajdowania garbów - serii pików
    unsigned d_Wysokosc;                //
    unsigned d_Pozycja;                 //
    unsigned d_Nachylenie;

    int minRow, maxRow;       //maxymalne wartości w falbance wiersza
    vector <atOCR_Vertical*> verticale;
    vector <atOCR_Garb*>     garby;
    //vector <atOCR_Pik*>     garby;

    vector <atOCR_Wiersz>   wiersze;
    vector <uchar> rows;



public:

    atOCR();
    ~atOCR();
    void     atInit(at_Konfiguracja * c);
    void     wczytaj(string f);
   // void     analizaVertykalna();
   // void     analizaVertykali();
    void     sumowanieVertykali();
    void     znajdowanieGarbow();
    //unsigned long  getSize() {return wiersze.size();}
    //atOCR_Wiersz*  getWiersz(unsigned long i) {return &(wiersze[i]); }
    unsigned long  getRowsSize() {return rows.size();}
    uchar          getRow(unsigned long i) {return rows[i]; }
    unsigned long  getVerticaleSize(){ return verticale.size();}
    atOCR_Vertical*  getVertical(unsigned long i) {return verticale[i]; }
    unsigned long  getGarbySize(){ return garby.size();}
    atOCR_Garb*  getGarb(unsigned long i) {return garby[i]; }
    atOCR_Pik*  getGarbPik(unsigned long i) {return garby[i]->getPik(); }

    int getMinRow()    {return minRow;}
    int getMaxRow()    {return maxRow;}
    cv::Mat* getImageGray() {return &imgGray;}
    at_Konfiguracja* getCnf(){return cnf;}

    double getRowPoziom()    {return rowPoziom;}
    double getColPoziom()    {return colPoziom; }
    int    getRowSzerokosc() {return rowSzerokosc;}
    int    getColSzerokosc() {return colSzerokosc;}
    unsigned    get_dSzerokosc()  {return d_Szerokosc;}
    unsigned    get_dWysokosc()   {return d_Wysokosc;}
    unsigned    get_dPozycja()    {return d_Pozycja;}
    unsigned    get_dNachylenie()    {return d_Nachylenie;}

    void addGarb(atOCR_Pik* p);
};


#endif // ATOCR_H
