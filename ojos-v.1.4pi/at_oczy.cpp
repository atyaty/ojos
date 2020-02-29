#include "at_oczy.h"

at_oczy::at_oczy(at_kamera *c0,at_kamera *c1, double b)
{
    cam0=c0;
    cam1=c1;
    baza=b;

    MaxdsX=10;
    MaxdsY=10;
    MaxdsZ=10;

    MaxdX=10;
    MaxdY=10;
    MaxdZ=10;

    MaxdsZpix=10;
    MaxdZtg=0.2;

}
