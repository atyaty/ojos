#include "at_majak.h"

//-----------------------------------------------------------------
// konstruktor kółka
at_majak::at_majak( double x, double z, double r) {
    Xtg=x;
    Ztg=z;
    sXtg=r;  //rozmiar kątowy
    sZtg=r;  //rozmiar kątowy
    ksztalt=MAJAK_CIRCLE;
    kolor=Scalar(0,0,0);
}
//-----------------------------------------------------------------
at_majak::at_majak( double x, double z, double r, Scalar c) { at_majak(x,z,r); kolor=c; }
//-----------------------------------------------------------------
//konstruktor krawędzi
at_majak::at_majak( double x0, double z0, double x1, double z1) {
    Xtg=x0;
    Ztg=z0;
    sXtg=x1-x0;  //rozmiar kątowy
    sZtg=z1-z0;  //rozmiar kątowy
    ksztalt=MAJAK_LINE;
    kolor=Scalar(0,0,0);
}
//-----------------------------------------------------------------
at_majak::at_majak( double x0, double z0, double x1, double z1, Scalar c) { at_majak(x0,z0,x1,z1); kolor=c; }

