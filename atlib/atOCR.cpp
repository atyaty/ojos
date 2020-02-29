#include "atOCR.h"

//-----------------------------------------------------------------
atOCR::atOCR(){}
//-----------------------------------------------------------------
atOCR::~atOCR(){
    for(unsigned i=0; i<verticale.size();i++){delete verticale[i];}
    for(unsigned i=0; i<garby.size();i++){delete garby[i];}
}
//-----------------------------------------------------------------
void atOCR::atInit(at_Konfiguracja * c){
    cnf=c;
    //--------------------------------
    rowPoziom           = cnf->getFloat("rowPoziom");
    rowSzerokosc        = cnf->getInt("rowSzerokosc");
    colPoziom           = cnf->getFloat("colPoziom");
    colSzerokosc        = cnf->getInt("colSzerokosc");
    podzialVertykalny   = cnf->getInt("podzialVertykalny");

    d_Pozycja     = cnf->getInt("d_Pozycja");
    d_Szerokosc   = cnf->getInt("d_Szerokosc");
    d_Wysokosc    = cnf->getInt("d_Wysokosc");
    d_Nachylenie    = cnf->getInt("d_Nachylenie");


}

//----------------------------------------
void atOCR::wczytaj(string f){
    filename =f;
    imgOryg = cv::imread(filename);
    cvtColor( imgOryg, imgGray, COLOR_BGR2GRAY );
    sumowanieVertykali();
    znajdowanieGarbow();
}

//-----------------------------------------------------------------
void atOCR::sumowanieVertykali(){
   int szerokoscPodzialu=imgGray.cols/podzialVertykalny;
   verticale.clear();
   for(int i=0; i<podzialVertykalny; i++) {
       atOCR_Vertical* v= new atOCR_Vertical(i*szerokoscPodzialu,(i+1)*szerokoscPodzialu ,this);
       if(verticale.size()!=0) verticale[verticale.size()-1]->setNext(v);
       verticale.push_back( v  );
   }
}

//-----------------------------------------------------------------
void atOCR::znajdowanieGarbow(){
   for(unsigned i=0; i<verticale.size(); i++) {
       verticale[i]->znajdzGarby();
   }
   for(unsigned i=0; i<garby.size(); i++){
       garby[i]->countLen();
   }
}

//-----------------------------------------------------------------
void atOCR::addGarb(atOCR_Pik* p){
    atOCR_Garb* g = new atOCR_Garb(p);
    garby.push_back( g );
    //garby.push_back( p );
}







