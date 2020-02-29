#include "at_kamera.h"

at_kamera::at_kamera(int x, int z, double tx, double tz)
{
    //wczytać to z jakiejś konfiguracji
    Xtg=tx;
    Ztg=tz;
    X=x;
    Z=z;
    //połówki żeby łatwiej tego uzywać
    _Xtg=Xtg/2;
    _Ztg=Ztg/2;
    _X=X/2;
    _Z=Z/2;
    opened=false;
}
