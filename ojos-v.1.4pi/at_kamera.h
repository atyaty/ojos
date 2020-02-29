#ifndef AT_KAMERA_H
#define AT_KAMERA_H


// objekt zawiera definicję kamery używanej do rejestracji obrazów
// tangensy kąta widzenia określa się w ten sposób że ustawia się kamerę przed ścianą w określonej odległości: L
// mierzy się szerokość i wysokośc ściany która została przez kamerę uchwycona na obrazie: szerokość, wysokość
// Ztg= wysokość/L
// Xtg= szerokość/L
// rozdzielczość kamery w pionie i w poziomie to ilość pikseli która jest rejestrowana w pionie i w poziomie.
//
//
class at_kamera {
private:
    double Ztg; //tangens kąta widzenia kamery w pionie
    double Xtg; //tangens kąta widzenia kamery w poziomie
    int Z; //rozdzielczość pionowa - piksele
    int X; //rozdzielczość pozioma - piksele
    //nie wiem czy powyższe zmienne się przydadzą,
    //..bo po prawdzie, to używamy schemetu liczenia od środka ekranu:
    double _Ztg; //liczone od środka ekranu Ztg/2
    double _Xtg; //..
    int _Z; //liczone od środka ekranu Z/2
    int _X; //..

    bool opened;

public:
    at_kamera(int x, int z, double tx, double tz);

    int getX(){return X;}
    int getZ(){return Z;}
    int get_X(){return _X;}
    int get_Z(){return _Z;}

    double getXtg(){return _Xtg;}
    double getZtg(){return _Ztg;}

    void setOpen(){opened=true;}
    bool IsOpened(){ return opened;}

    //----------------------------------------------------------
    //parametry procedury GaussianBlur (do detekcji kółek)
    int BlurXs;
    int BlurZs;
    int BlurXdev;
    int BlurZdev;
    //parametry procedury HoughCircle
    int CircleInversRat;
    int CircleminDist;
    int CircleParam1;
    int CircleParam2;
    int CircleminR;
    int CirclemaxR;
    //parametry procedury Canny
    int Cannythres1;
    int Cannythres2;
    int Cannyaperture;
    //parametry do procedury HoughLineP
    int Linerho;
    int Linetheta;
    int Linethres3;
    int LineminLen;
    int LineminGap;
};



#endif // AT_KAMERA_H
