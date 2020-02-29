#include "at_majak.h"

//-----------------------------------------------------------------
// konstruktorkółka
// parametry podawane są w normalnych współrzędnych ekranu, liczonych o górnego lewego narożnika
// zapamiętane są na zmiennych X,Z.. ale tylko dla operacji pomocniczych - np. wyświetlanie na ekranie
// ze wszystkimi innymi operacjami przechodzimy do układu współrzędnych w środku ekranu
at_majak::at_majak(at_kamera *cc, int x, int z, int r)
{
    cam=cc;
    X=x; //w pikselach
    Z=z; //w pikselach

    _X=x-cam->get_X(); //w pikselach w układzie środka ekranu
    _Z=z-cam->get_Z(); //w pikselach..
    Xsize=r; //w pikselach
    Zsize=r; //..
    ksztalt=MAJAK_CIRCLE;

    Xtg=((double)_X/cam->get_X()) * cam->getXtg();
    Ztg=((double)_Z/cam->get_Z()) * cam->getZtg();

    sXtg=((double)Xsize/cam->getX()) *cam->getXtg();  //rozmiar kątowy - poprawić to wyliczenie
    sZtg=((double)Zsize/cam->getZ()) *cam->getZtg();  //rozmiar kątowy
}

//-----------------------------------------------------------------
//konstruktor krawędzi
at_majak::at_majak(at_kamera *cc, int x0, int z0, int x1, int z1)
{
    cam=cc;
    X=x0; //w pikselach
    Z=z0; //w pikselach

    _X=x0-cam->get_X(); //w pikselach...
    _Z=z0-cam->get_Z(); //w pikselach..
    Xsize=(x1-x0); //to jeszcze przemyśleć bo mamy tu możliwe ujemne wymiary.. czy to oby gdzieś nie wylezie. (abs nie można, bo też sie pogubi)
    Zsize=(z1-z0); //..
    ksztalt=MAJAK_LINE;

    Xtg=((double)_X/cam->get_X()) * cam->getXtg();
    Ztg=((double)_Z/cam->get_Z()) * cam->getZtg();
    sXtg=((double)Xsize/cam->getX()) *cam->getXtg();  //rozmiar kątowy - poprawić to wyliczenie, to zależy także od Xtg
    sZtg=((double)Zsize/cam->getZ()) *cam->getZtg();  //rozmiar kątowy
}
