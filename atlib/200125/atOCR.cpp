#include "atOCR.h"

atOCR::atOCR()
{

}
//----------------------------------------
void atOCR::wczytaj(string f){
    filename =f;
    imgOryg = cv::imread(filename);
    cvtColor( imgOryg, imgGray, COLOR_BGR2GRAY );
    analizaVertykalna();
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

    int maxGranica=(double)maxRow*rowPoziom;
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
void atOCR::atInit(at_Konfiguracja * c){
    //--------------------------------
    cnf=c;
    //--------------------------------
    rowPoziom           = cnf->getFloat("rowPoziom");
    rowSzerokosc        = cnf->getInt("rowSzerokosc");
    colPoziom           = cnf->getFloat("colPoziom");
    colSzerokosc        = cnf->getInt("colSzerokosc");
}


//-----------------------------------------
atOCR_Wiersz::atOCR_Wiersz( int min, int max, atOCR* o){
    rowMin=min;
    rowMax=max;
    imgGray=o->getImageGray();
    ocr=o;
    //cnf=o->getCnf();
}
int atOCR_Wiersz::getColGranica() {return (double)maxCol*ocr->getColPoziom();}
void atOCR_Wiersz::analizaKolumn(){

    minCol=255;
    maxCol=0;
    cols.clear();
    for(int i=0; i<imgGray->cols ; i++) {
        int long sumCol=0;
        for(int j=rowMin; j<rowMax ; j++) {   sumCol+=imgGray->at<uchar>(j,i);     }
        int x1=256-sumCol/(rowMax-rowMin);
        if(x1<minCol) minCol=x1;
        if(x1>maxCol) maxCol=x1;
        cols.push_back(x1);
    }

    int colSzerokosc=ocr->getColSzerokosc();//cnf->getInt("colSzerokosc");
    double colPoziom=ocr->getColPoziom();   //cnf->getFloat("colPoziom");
    int maxGranica=(double)maxCol*colPoziom;
    int ostatniDuzy=0;
    for(int i=1; i<cols.size(); i++) {
       if(cols.at(i)>maxGranica){
           if(i>ostatniDuzy+colSzerokosc){
               //analizaKolumny(min,max, ostatniDuzy, i, dst);
               atOCR_Pole pole=atOCR_Pole(rowMin,rowMax,ostatniDuzy, i, imgGray,ocr->getCnf());
               pole.analiza();
               pola.push_back(pole);
           }
           ostatniDuzy=i;
       }


    }






}

//-----------------------------------------
void atOCR_Pole::analiza(){

    int paddingCol=cnf->getInt("colPadding");
    int paddingRow=cnf->getInt("rowPadding");
    int Col_min=colMin+paddingCol;
    int Col_max=colMax-paddingCol;
    int Row_min=rowMin+paddingRow;
    int Row_max=rowMax-paddingRow;
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->Init(NULL, "eng")) { return;   }                       //BŁAD
    api->SetImage(imgGray->data, imgGray->cols, imgGray->rows, 3, imgGray->step);
    api->SetRectangle(Col_min,Row_min,Col_max-Col_min,Row_max-Row_min);
    char* ocrResult = api->GetUTF8Text();
    tekst=ocrResult;
    api->End();
    return;
}
