#ifndef AT_OMAM_H
#define AT_OMAM_H

#include "at_majak.h"
#include "at_oczy.h"
#include "math.h"
#include <QDateTime>

//omam to jest wekotorowa postac jakiegoś rozpoznanego objektu rzeczywistego
// omam powstaje na podstawie zarejestrowanych przez oczy (czyli dwie kamery) majaków (czyli znalezionych w rastrowym obrazie
// charakterystycznych objektów wektorowych)
// każda kamera daje rastrowy obraz
// w poszczególnych rastrowych obrazach znajdujemy charakterystyczne objekty (kółka, linie, itp) które nazywamy majakami
// dopasowujemy do siebie majaki odczytane w kazdej z kamer
// jeśli dwa majaki pasują do siebie - tworzymy na ich podstawie omam
// dla omama możemy wyliczyć (na ogół) komplet informacji o jego położeiu i rozmiarze
//
// rozpoznane omamy zapisujemy na liście omamów, (patrz klasa omamy)
// lista jest na bieżąco aktualizowana
// zanim dodamy omam do listy sprawdzamy czy ten który dodajemy nie wystepuje juz na liście. jesli występuje to tylko aktualizujemy stary
// podczas aktualizacji obliczamy parametry wynikające z historii (np. prędkośc, czas do kolizji)
//
class at_omam {

private:

    at_oczy* oczy;   //oczy które zarejestrowały omamama,
                    //  ...w całym sysyemie może być wiele oczu, które mogą widzieć tego omama.. może tu powinna być lista oczu?

    //
    double X,Y,Z;    //odległości w układze współrzędnych kamery (zero układu jest pomiędzy kamerami)
    double vX,vY,vZ; //prędkość
    double sX,sY,sZ; //rozmiar rzeczywisty (promień albo wymiary poprzeczne odczytane z majaków)
    int ksztalt;

                    //poniższe informacje mówią o tym jak bardzo możemy być pewni danego omama
                    //jeśli niespójności pomiędzy majakami na podstawie których on powstał są małe, mamy większą pewność że ten omam jest dobry
    double dZ;       //niespójność połozenia Z dla majaków
    double dsX,dsY,dsZ;  //niespójność rozmiarów dla majaków

    double odleglosc;
    double czas_do_kolizji;

    //vector <Point> traj; //trajektoria

    int id;               //id omama
    qint64 t0,t1;         //pierwsze/ostatnie objawienie się omamu
    int n;                //iloś objawień

    int check;      //aktualny wzkaźnik spójności omamu
    int check_last; //ostatni wzkaźnik spójności omamu
    int check_sr;   //średni

public:
    at_omam(at_majak *m0, at_majak *m1, at_oczy* o);

    int Check(at_majak *m0, at_majak *m1, at_oczy* o);
    bool Ok() {return check>0;}   //wskaźnik prwidłowo utworzonego (z majaków) obiektu
    int getCheck(){return check;}

    bool Ten_sam(at_omam* o);
    void Popraw(at_omam* o1);
    void setId(int i){id=i;}

    int getId(){ return id; }
    qint64 getTlast(){return  t1;}
    at_oczy* getOczy(){return oczy;}
    int Ksztalt(){return ksztalt;}
    bool IsCircle(){return ksztalt==MAJAK_CIRCLE;}
    bool IsLine(){return ksztalt==MAJAK_LINE;}
    double getX(){return X;}
    double getY(){return Y;}
    double getZ(){return Z;}
    double getsX(){return sX;}
    double getsY(){return sY;}
    double getsZ(){return sZ;}
    double getvX(){return vX;}
    double getvY(){return vY;}
    double getvZ(){return vZ;}
    int getN(){return n;}
    double getOdleglosc(){return odleglosc;}
    double getWaga(){return n/odleglosc;} //to trzeba eksperymantalnie ustalić, które objekty są dla nas ważne i taką wagę zaimplementowac

    //parametry obliczone wtórnie
    double getX0tg(){ return (X+oczy->getBaza()/2)/Y;}
    double getX1tg(){ return (X-oczy->getBaza()/2)/Y;}
    double getZtg() { return Z/Y; }
    double getsZtg(){ return sZ/Y; }
    double getsXtg(){ return sX/Y; }

    bool Porownaj(at_omam* o){return ((t1==o->t1) ? (n > o->n) : (t1 > o->t1)); }

};

#endif // AT_OMAM_H
