#ifndef AT_MAJAK_H
#define AT_MAJAK_H
#include <QColor>
#include "at_kamera.h"

#define MAJAK_CIRCLE 0
#define MAJAK_LINE   1


// majak to jest wektorowo (w dwóch wymiarach) zapisany objekt który został zidentyfikowany wcześniej na obrazie rastrowym
// majak opisywany jest przez:
// współżędne połozenia: X - poziom, Z - pion
// rozmiar: Xsize, Zsize
// kolor: color
// kształt - to jest wskaźnik mówiący o kształcie zidentyfikowanego objetu (1-koło, 2-linia prosta, ..)
//
// położenie i rozmiar przeliczane sa na współrzędne kątowe, a dokładniej na tangensy
// Xtg, Ztg,
//sXtg, sZtg - rozmiary kątowe
// do tego przeliczenia wykorzystuje się dane z objektu cam (kamera która zarejestrowała obraz rastrowy)
class at_majak {

private:
    at_kamera *cam;

    int X; //położenie w poziomie,
    int Z;
    int _X; //położenie w poziomie, zapisane jest już w układzie współrzędnych w środku obrazka
    int _Z;
    QColor c;
    int ksztalt;
    int Xsize;
    int Zsize;
    double Xtg; //położenie katowe
    double Ztg;
    double sXtg; //rozmiary kątowe objektu
    double sZtg;



public:
    at_majak(){}
    at_majak(at_kamera *cc, int x, int z, int r); //kreator okręgu
    at_majak(at_kamera *cc, int x, int z, int xs, int zs); //kreator oodcinka

    double getX(){return X;}
    double getZ(){return Z;}
    double get_X(){return _X;}
    double get_Z(){return _Z;}
    double getsX(){return Xsize;}
    double getsZ(){return Zsize;}

    double getXtg(){return Xtg;}
    double getZtg(){return Ztg;}
    double getsXtg(){return sXtg;}
    double getsZtg(){return sZtg;}

    bool IsCircle(){return ksztalt==MAJAK_CIRCLE;}
    bool IsLine(){return ksztalt==MAJAK_LINE;}
    int Ksztalt(){return ksztalt;}
};

#endif // AT_MAJAK_H
