#ifndef AT_OMAMY_H
#define AT_OMAMY_H
#include "at_omam.h"
#include <QVector>
//#include <QtAlgorithms>
#include <algorithm>

//--------------------------------------------------------------------------------------------
// chodzi nam o to by "widzieć" czyli mieć informację o tym gdzie i jakie obiekty się znajdują,
// listą obiektów wraz z ich parametrami jest zmienna: omamy
// jeden omam na tej liście to jeden wykryty objekt.
// szczegółowość opisu obiektu czyli omama może być jeszcze dopracowana, na razie rozumiemy to mniej więcej tak:
//  -zielone, okrągłe, ma współrzędne 100,12,200
//  -narożnik, o współrzędnych 120,300,0
//  - itd
//
// lista jest na bieżąco aktualizowana metodami Add
// lista ma ograniczoną pojemność, chodzi o to, żeby nie rozpraszać uwagi i kontrolować tylko najważniejsze punkty przestrzeni w której sie poruszamy
// omamy na liscie są uporzadkowane wedlug istotnosci (wagi)
//  ..istotne są te które są blisko, albo te które są bardziej wyraźne, albo bardziej stabilne.. to jest do ustalenia
//
//
class at_omamy
{
private:
    QList<at_omam*> omamy;
    int MaxSize;                //maksymalny rozmiar tablicy omamów
    int MaxBlackTime;           //maksymalny czas niewidzenia omama (po tym czasie jest kasowany z tablicy)
    int LastId;                 //do nadawania Id kolejnym omamom

    double OdlegloscKrytyczna;     //obiekty zbyt bliskie (poniżej tej odległości) uznajemy za krytyczne i im się bardziej przyglądamy
    double CzasDoKolizjiKrytyczny; //jw

public:
    at_omamy(int max){MaxBlackTime=max; MaxSize=10; LastId=0;OdlegloscKrytyczna=-1;CzasDoKolizjiKrytyczny=-1;}

    bool Dodaj(at_omam* o1);
    bool TryCreate(at_majak *m0, at_majak *m1, at_oczy* o);
    void Add( QVector<at_majak>* m0, QVector<at_majak>* m1, at_oczy* o);
    int Uporzadkuj();
    //bool omamy_erase(at_omam* o);

    int getId()             { LastId++; return LastId; }
    int getSize()           { return omamy.size();}
    at_omam* getOmam(int i) { if(i>=0 && i<omamy.size())  return omamy.at(i);   return NULL;   }
};

#endif // AT_OMAMY_H
