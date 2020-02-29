#ifndef AT_MAJAK_H
#define AT_MAJAK_H

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;


#define MAJAK_NONE   0
#define MAJAK_CIRCLE 1
#define MAJAK_LINE   2

//--------------------------------------------------------------------------------------------------------------------------
// majak to jest wektorowo (w dwóch wymiarach) zapisany objekt który został zidentyfikowany wcześniej na obrazie rastrowym
// kolor - color
// kształt - to jest wskaźnik mówiący o kształcie zidentyfikowanego objetu (1-koło, 2-linia prosta, ..)
//
// położenie i rozmiar przeliczane sa na współrzędne kątowe, a dokładniej na tangensy
// Xtg, Ztg,
// sXtg, sZtg - rozmiary kątowe
//--------------------------------------------------------------------------------------------------------------------------
class at_majak {
public:

    double Xtg,Ztg;     //położenie katowe
    double sXtg,sZtg;   //rozmiary kątowe objektu
    int ksztalt;
    Scalar kolor;

    at_majak(){Xtg=0;Ztg=0;sXtg=0;sZtg=0;ksztalt=0;kolor=Scalar(0,0,0);}
    at_majak( double x,  double z,  double r );                          //kreator okręgu
    at_majak( double x0, double z0, double x1, double z1 );              //kreator oodcinka
    at_majak( double x,  double z,  double r, Scalar c );                //kreator okręgu
    at_majak( double x0, double z0, double x1, double z1, Scalar c );    //kreator oodcinka

};

#endif // AT_MAJAK_H
