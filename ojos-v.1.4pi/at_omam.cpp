#include "at_omam.h"

//-----------------------------------------------------------------------------
at_omam::at_omam(at_majak *m0, at_majak *m1, at_oczy* o){
    id=-1;
    n=0;
    vX=0.;
    vY=0.;
    vZ=0.;
    t0=t1=QDateTime::currentMSecsSinceEpoch();

    ksztalt=m0->Ksztalt();

    check=Check(m0,m1,o);
    if(check>0) oczy=o;   //zapamiętujem jakie oczy widziały tego omama żeby mieć dostęp do współczynników kontroli, które mogą być różne dla różnych oczu
}

//-----------------------------------------------------------------------------
// sprawdzenie czy dwa majaki repreaentują ten sam przestrzenny objekt czyli omam
// wykonujemy serię sprawdzeń które mogą wykluczyć lub potwierdzić że to ten sam omam
// jeśli nie zostanie odrzucony,wyliczana są jego podstawowe parametry
//
// ta funkcja jest częścią konstruktora,
// dane mamy więc jedynie z dostarczonych majaków (no chyba że wymyśli się jakąś statystykę)
// tworzymy tak na prawdę próbnego omama, który nie koniecznie musi przetrwać próbę
// ..pomysły uwzględnienia dodatkowych danych z innych sesji/z historii.. należy realizować na innych etapach
//
int at_omam::Check(at_majak *m0, at_majak *m1, at_oczy* o){

    if(m0->Ksztalt()!=m1->Ksztalt()){ return -4; }

    //---------------------------------------------
    // jesli tgZ1 != tgZ0 (kąt patrzenia pionowego na obiekt jest różny) to nie może być to ten sam obiekt = dyskwalifikacja
    // oczy czyli kamery ustawione sa obok siebie i patrzą równolegle do przodu, nie mogą więc widzieć tego samego obiektu na różnych wysokościach
    if(fabs(m0->getZtg()-m0->getZtg()) > o->getMaxdZtg()) {return -1;}

    //---------------------------------------------
    // jesli rozmiar Z obiektu widzany z obu kamer jest różny to nie może być to ten sam obiekt = dyskwalifikacja
    // z powyższego wynika że także rozmiar pionowy powinien być taki sam w obu kamerach
    // ..to samo porównanie można zrobić ewentualnie na tangensach (byłoby bardziej spójnie)
    if(fabs(m0->getsZ()-m0->getsZ()) > o->getMaxdsZpix() ) {return -2;}

    //---------------------------------------------
    // kolory?
    // - trzeba dorobić analizę kolorów dla majaków.
    //   Na etapie tworzenia majaków należy uruchomić procedurę rozpoznawania (obliczania) kolorów
    //   ..ewentualanie taką procedurę uruchamiac gdzieś tu gdy zachodzi rzeczywista potrzeba??
    //
    //
    //

    //--------------------------------------------------
    // odpowiednia różnica w X też może wykluczyć majaki
    // chodzi o to, że obiekt nie może być widziany przez prawe oko pod kątem większym niż przez lewe
    if(m0->getXtg()>m1->getXtg()){ return -3;  } //tylko uzgodnić czy kierunek jest ok

    //--------------------------------------------------
    // obliczenia -teoria :)
    // (Y- trzeci wymiar)
    // tg1 = X1/Y  (normalny wzór na tangens)
    // tg0 = X0/Y
    // X0  = X1+baza
    // czyli:
    // tg0 = (X1+baza)/Y
    // czyli:
    // (tg0-tg1)*Y=baza
    // czyli:
    // Y=baza/(tg0-tg1)
    // czyli:
    Y = o->getBaza()/(m0->getXtg()-m1->getXtg());  //w metrach ..jeśli baza jest w metrach
    X = m1->getXtg()*Y+o->getBaza()/2;             //w metrach
    double Z0=m0->getZtg()*Y;
    double Z1=m1->getZtg()*Y;
    Z=(Z0+Z1)/2;
    dZ=fabs(Z1-Z0); //niespójność położenia Z

    double sZ0=m0->getsZtg()*Y;
    double sZ1=m1->getsZtg()*Y;
    sZ=(sZ0+sZ1)/2;
    dsZ=fabs(sZ1-sZ0); //niesoójność rozmiaru pionowego

    double sX0=m0->getsXtg()*Y;
    double sX1=m1->getsXtg()*Y;
    sX=(sX0+sX1)/2;
    dsX=fabs(sX1-sX0); //niespójność rozmiaru X
    //to może być podstawa do obliczenia sY oraz dsY

    sY=0; dsY=0;//??
    //na razie podstawiamy zero,
    //na tym etapie - analizy majaków z jednej sesji nie ma możliwości poprawnego ustalenia rozmiarów Y
    //można będzie do tego wrócic na późniejszych etapach analizy

    //kolory !!!
    //uwzględniać oświetlenie które może być wyliczone z wielu omamów i trzymane razem z listą omamów
    // double sC= średnia z koloru
    // double dsC= odchyłka koloru
    // itd..
    //ale ta analiza poinna byc juz na kolejnych teapach

    return 1;

}

//-------------------------------------------------------------------------------------
// funkcja nie jest symetryczna (jako parametr powinien być podawany nowy obiekt, który poddajemy analizie na spójność)
// chodzi o to że omam kumuluje dane historyczne i należy przyjąć założenie który z porównywanych obiektów tę historię posiada,
// a który jest niepewny czyli na przykład nowy)
bool at_omam::Ten_sam(at_omam* o){
    if(fabs(X  - o->X)>oczy->getMaxdX())   {return false;}
    if(fabs(Y  - o->Y)>oczy->getMaxdY())   {return false;}
    if(fabs(Z  - o->Z)>oczy->getMaxdZ())   {return false;}
    if(fabs(sX - o->sX)>oczy->getMaxdsX()) {return false;}
    if(fabs(sY - o->sY)>oczy->getMaxdsY()) {return false;}
    if(fabs(sZ - o->sZ)>oczy->getMaxdsZ()) {return false;}
    //uwzględnić tu dane o prędkości..

    return true;
}

//-------------------------------------------------------------------------------------
// procedura popraw jest uruchomiana jesli uznamy że obiekt "o" to nowa wersja nas samych
//
//jest pewien zgryz jeśli zechcemy faktycznie uzywac wiele oczu
//wtedy nie wiadomo który parametr zapisać pod "oczy,które widziały" !!
//jeśli różne oczy nie są jednoznacznie lepsze od innych (różne parametry kontrolowane są lepiej przez różne oczy)
//to słuszne wydaje się byc zapisanie odchyłek dla poszczególnych parametrów
//
void at_omam::Popraw(at_omam* o){

    double dt=(o->getTlast()-getTlast())/1000.;

    t1=o->t1;
    vX=(o->X-X)/(dt);
    vY=(o->Y-Y)/(dt);
    vZ=(o->Z-Z)/(dt);
    X=o->X;
    Y=o->Y;
    Z=o->Z;
    sX=o->sX;
    sY=o->sY;
    sZ=o->sZ;
    n++;

}
