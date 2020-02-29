#ifndef AT_OCZY_H
#define AT_OCZY_H
#include "at_kamera.h"


//---------------------------------------------------
// oczy to układ dwóch kamer umieszczonych obok siebie
// patrzących równolegle do przudu
// oddalonych od siebie o odległość: baza
//---------------------------------------------------
class at_oczy {
private:
    double baza;     //odległość pomiędzy kamerami
    at_kamera *cam0;
    at_kamera *cam1;

public:
    at_oczy(at_kamera *c0,at_kamera *c1,double b);
/*
    double getBaza(){return baza;}

    double getMaxdZtg(){return MaxdZtg;}
    double getMaxdsZpix(){return MaxdsZpix;}

    double getMaxdsX(){return MaxdsX;}
    double getMaxdsY(){return MaxdsY;}
    double getMaxdsZ(){return MaxdsZ;}

    double getMaxdX(){return MaxdX;}
    double getMaxdY(){return MaxdY;}
    double getMaxdZ(){return MaxdZ;}
    at_kamera* getCam0(){return cam0;}
    at_kamera* getCam1(){return cam1;}
*/
};

#endif // AT_OCZY_H
