#include "atOCR.h"

atOCR::atOCR()
{

}
//----------------------------------------
void atOCR::wczytaj(string f){
    filename =f;
    imgOryg = cv::imread(filename);
    cvtColor( imgOryg, imgGray, COLOR_BGR2GRAY );
   // analizaVertykalna();
    sumowanieVertykali();
  //  porownanieVerticali();
}
//-------------------------------------------
void atOCR::analizaVertykalna(){

    minRow=255;
    maxRow=0;
    rows.clear();
    for(int i=0; i<imgGray.rows ; i++) {
        int long sumRow=0;
        for(int j=0; j<imgGray.cols ; j++) {  sumRow+=imgGray.at<uchar>(i,j);    }
        int y1=256-sumRow/imgGray.cols;
        if(y1<minRow) minRow=y1;
        if(y1>maxRow) maxRow=y1;
        rows.push_back(y1);
    }

    int maxGranica=getRowGranica();
    int ostatniDuzy=0;
    for(int i=0; i<rows.size(); i++) {
       if(rows.at(i)>maxGranica){
           if(i>ostatniDuzy+rowSzerokosc){
               atOCR_Wiersz wiersz=atOCR_Wiersz(ostatniDuzy, i, this);
               wiersz.analizaKolumn();
               wiersze.push_back(wiersz);
           }
           ostatniDuzy=i;
       }
    }



}
//-------------------------------------------
void atOCR::analizaVertykali(){
    minRow=255;
    maxRow=0;
    rows.clear();
    for(int i=0; i<imgGray.rows ; i++) {
        int long sumRow=0;
        for(int j=0; j<imgGray.cols ; j++) {  sumRow+=imgGray.at<uchar>(i,j);    }
        int y1=256-sumRow/imgGray.cols;
        if(y1<minRow) minRow=y1;
        if(y1>maxRow) maxRow=y1;
        rows.push_back(y1);
    }

    int maxGranica=getRowGranica();
    int ostatniDuzy=0;
    for(int i=0; i<rows.size(); i++) {
       if(rows.at(i)>maxGranica){
           if(i>ostatniDuzy+rowSzerokosc){
               atOCR_Wiersz wiersz=atOCR_Wiersz(ostatniDuzy, i, this);
               wiersz.analizaKolumn();
               wiersze.push_back(wiersz);
           }
           ostatniDuzy=i;
       }
    }
}
//-----------------------------------------------------------------
void atOCR::sumowanieVertykali(){
   int szerokoscPodzialu=imgGray.cols/podzialVertykalny;
   verticale.clear();
   for(int i=0; i<podzialVertykalny; i++) {
       atOCR_Vertical vertical=atOCR_Vertical(i*szerokoscPodzialu,(i+1)*szerokoscPodzialu ,this);
       vertical.analizaWierszy();
       verticale.push_back(vertical);
   }
}
//-----------------------------------------------------------------
void atOCR::porownanieVertykali(){
   for(unsigned i=1; i<verticale.size(); i++) {

       verticale[1].porownaj(&verticale[0]);
   }
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
}
//-----------------------------------------------------------------
void atOCR::addGarb(atOCR_Wiersz* w){
    atOCR_Garb g=atOCR_Garb(w);
    garby.push_back( g );
}


//-----------------------------------------
atOCR_Wiersz::atOCR_Wiersz( int min, int max, atOCR* o){
    wierszMin=min;
    wierszMax=max;
    imgGray=o->getImageGray();
    ocr=o;
   // cnf=o->getCnf();
}
int atOCR_Wiersz::getColGranica() {return (double)maxCol*ocr->getColPoziom();}
//-------------------------------------------------------------------
void atOCR_Wiersz::sumowanieKolumn(){

    minCol=255;
    maxCol=0;
    cols.clear();
    for(int i=0; i<imgGray->cols ; i++) {
        int long sumCol=0;
        for(int j=wierszMin; j<wierszMax ; j++) {   sumCol+=imgGray->at<uchar>(j,i);     }
        int x1=256-sumCol/(wierszMax-wierszMin);
        if(x1<minCol) minCol=x1;
        if(x1>maxCol) maxCol=x1;
        cols.push_back(x1);
    }
}
//--------------------------------------------------------------------------------
void atOCR_Wiersz::analizaKolumn(){
    sumowanieKolumn();
    int colSzerokosc=ocr->getColSzerokosc();//cnf->getInt("colSzerokosc");
    double colPoziom=ocr->getColPoziom();   //cnf->getFloat("colPoziom");
    int maxGranica=getColGranica();
    int ostatniDuzy=0;
    for(int i=1; i<cols.size(); i++) {
       if(cols.at(i)>maxGranica){
           if(i>ostatniDuzy+colSzerokosc){
               //analizaKolumny(min,max, ostatniDuzy, i, dst);
               atOCR_Pole pole=atOCR_Pole(wierszMin,wierszMax,ostatniDuzy, i, imgGray,ocr->getCnf());
               pole.analiza();
               pola.push_back(pole);
           }
           ostatniDuzy=i;
       }


    }
}

//-----------------------------------------
atOCR_Vertical::atOCR_Vertical( int min, int max, atOCR* o){
    verticalMin=min;
    verticalMax=max;
    imgGray=o->getImageGray();
    ocr=o;

}
//-----------------------------------------
int atOCR_Vertical::getRowGranica() {return (double)maxRow*ocr->getRowPoziom();}
//-------------------------------------------
void atOCR_Vertical::sumowanieWierszy(){
    minRow=255;
    maxRow=0;
    rows.clear();
    for(int i=0; i<imgGray->rows ; i++) {
        int long sumRow=0;
        for(int j=verticalMin; j<verticalMax ; j++) {  sumRow+=imgGray->at<uchar>(i,j);    }
        //int y1=256-sumRow/imgGray->cols;
        int y1=256-sumRow/(verticalMax-verticalMin);
        if(y1<minRow) minRow=y1;
        if(y1>maxRow) maxRow=y1;
        rows.push_back(y1);
    }
}
/*
//-------------------------------------------
void atOCR_Vertical::analizaWierszy(){
    sumowanieWierszy();
    unsigned rowSzerokosc=ocr->getRowSzerokosc();
    unsigned rowMax=0;             //zerowe maximum
    unsigned rowMin=0;             //zerowe minimum
    unsigned maxValue=rows[0];     //wartosc w zerowym maximum
    unsigned prevValue=rows[0];              //wartosc w poprzednim rowsie
    unsigned minValue=rows[0];
    unsigned poziomPik=0;
    bool szukamyMax=false;
    for(unsigned i=1; i<rows.size(); i++) {
        if(!szukamyMax) {
          if(rows[i]>prevValue ){
              szukamyMax=true;//znaleźliśmy minimum i bedziemy szukac maximum
              minValue=prevValue;
              rowMin=i-1;
              poziomPik=((double)(maxValue-minValue)*ocr->getRowPoziom());
                //zmienić sposób okreslania poziomu, żeby "resetować" poziom ustawiony w innych warunkach  - patrz Vertical[0]
          }
          else {
              prevValue=rows[i]; //wartość ciągle spada więc szukamy dalej minimum
          }
        }
        else{
          if(rows[i]>prevValue ){
                prevValue=rows[i]; //wartość ciągle rośnie szukamy dalej maximum
          }
          else {
                if((prevValue-minValue)>poziomPik ){ //jeśli wysokośc jest większa niż założone minimum to znaczy że znaleźliśmy pik
                    if(i>rowMax+rowSzerokosc ){ //jeśli znaleziony pik jest odpowiednio daleko od poprzedniego to dodajemy nowy go do listy
                        atOCR_Wiersz wiersz=atOCR_Wiersz(rowMax, i, ocr);
                         //zmienić objekt na atOCR_Pik
                        wiersze.push_back(wiersz);
                        maxValue=prevValue;
                        rowMax=i-1;
                    }
                    else if(prevValue>maxValue && wiersze.size()>0){  //jeśli znaleziony pik jest blisko poprzedniego, ale jest większy niz poprzedni to zmieniamy parametry tego poprzedniego
                        wiersze[wiersze.size()-1].setMaxRow(prevValue);
                        maxValue=prevValue;
                        rowMax=i-1;
                    }
                }
                szukamyMax=false; //znaleźliśmy maximum i bedziemy dalej szukac minimum


          }

        }
    }
}*/
//-------------------------------------------
void atOCR_Vertical::analizaWierszy(){
    sumowanieWierszy();
    unsigned rowSzerokosc=ocr->getRowSzerokosc();
    unsigned rowMax=0;             //zerowe maximum
    unsigned rowMin=0;             //zerowe minimum
    unsigned maxValue=rows[0];     //wartosc w zerowym maximum
    unsigned minValue=rows[0];
    unsigned poziomPik=0;
    unsigned poziomVert=((double)(verticalMax-verticalMin)*ocr->getRowPoziom());
    bool szukamyMax=false;
    for(unsigned i=1; i<rows.size(); i++) {
        if(!szukamyMax) {
          if(rows[i]>rows[i-1] ){
              szukamyMax=true;//znaleźliśmy minimum i bedziemy szukac maximum
              minValue=rows[i-1];
              rowMin=i-1;
              poziomPik=((double)(maxValue-minValue)*ocr->getRowPoziom());
              poziomPik=(poziomPik+poziomVert)/2; //uśredniamy z poziomem obliczonym dla verticala
                //zmienić sposób okreslania poziomu, żeby "resetować" poziom ustawiony w innych warunkach  - patrz Vertical[0]
          }
        }
        else{
          if(rows[i]<rows[i-1] ){
                 if((rows[i-1]-minValue)>poziomPik ){ //jeśli wysokośc jest większa niż założone minimum to znaczy że znaleźliśmy pik
                    if(i>rowMax+rowSzerokosc ){ //jeśli znaleziony pik jest odpowiednio daleko od poprzedniego to dodajemy nowy go do listy
                        atOCR_Wiersz wiersz=atOCR_Wiersz(rowMax, i, ocr);
                         //zmienić objekt na atOCR_Pik
                        wiersze.push_back(wiersz);
                        maxValue=rows[i-1];
                        rowMax=i-1;
                    }
                    else if(rows[i-1]>maxValue && wiersze.size()>0){  //jeśli znaleziony pik jest blisko poprzedniego, ale jest większy niz poprzedni to zmieniamy parametry tego poprzedniego
                        wiersze[wiersze.size()-1].setMaxRow(rows[i-1]);
                        maxValue=rows[i-1];
                        rowMax=i-1;
                    }
                }
                szukamyMax=false; //znaleźliśmy maximum i bedziemy dalej szukac minimum
          }
        }
    }
}



//-----------------------------------------
void atOCR_Pole::analiza(){

    int paddingCol=cnf->getInt("colPadding");
    int paddingRow=cnf->getInt("rowPadding");
    int Col_min=colMin+paddingCol;
    int Col_max=colMax-paddingCol;
    int Row_min=wierszMin+paddingRow;
    int Row_max=wierszMax-paddingRow;
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->Init(NULL, "eng")) { return;   }                       //BŁAD
    api->SetImage(imgGray->data, imgGray->cols, imgGray->rows, 3, imgGray->step);
    api->SetRectangle(Col_min,Row_min,Col_max-Col_min,Row_max-Row_min);
    char* ocrResult = api->GetUTF8Text();
    tekst=ocrResult;
    api->End();
    return;
}

//-------------------------------------------
void atOCR_Vertical::porownaj( atOCR_Vertical* v ){
    unsigned n=0;
    atOCR_Wiersz* w0=nullptr;
    atOCR_Wiersz* w1=v->getWiersz(n);
    for(unsigned i=0;i<wiersze.size();i++){
        while( n<v->getSize() && wiersze[i].getWierszMax()>w1->getWierszMax() ){
            n++;
            w0=w1;
            w1=v->getWiersz(n);
        }
        if( w0!=nullptr && n>=v->getSize() ){
            int d0 = abs( wiersze[i].getWierszMax() - w0->getWierszMax() );
            int d1 = abs( wiersze[i].getWierszMax() - w1->getWierszMax() );
            if(d1>d0){ ocr->addGarb();}
            else{}
        }
        if( n<v->getSize() ){
            int d2 = abs( wiersze[i].getWierszMax() - v->getWiersz(n)->getWierszMax() );
            if( d2<d1 ){ d1=d2; w1=v->getWiersz(n); }
        }
    }
}
