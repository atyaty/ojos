#ifndef AT_OCZY_H
#define AT_OCZY_H
#include "at_kamera.h"


//---------------------------------------------------
// oczy to układ dwóch kamer umieszczonych obok siebie
// patrzących równolegle do przudu
// oddalonych od siebie o odległość: baza
//---------------------------------------------------
class at_oczy
{
private:
    double baza;     //odległość pomiędzy kamerami
    at_kamera *cam0;
    at_kamera *cam1;

    //dla danych oczu zdefiniowane są współczynniki opisujące sposób detekcji obrazów z kamer:
    //
    // z każdej kamery dostajemy listę majaków jakie ta kamera zarejestrowała
    // naszym zadaniem jest rozpoznanie, które z majaków dotycza tego samego rzeczywistego obiektu (tu nazywanego omam)
    // czyli określenie że na przykład kółko widziane jednym okiem i drugim okiem to jest to samo kółko a nie dwa różne
    //
    double MaxdZtg;  //maksymalne odchylenie pomiędzy pionowymi tangensami majaków tego samego omama, przekroczeie powoduje że nie uznajemy omama
                     //jeśli położenie w pionie (w jednej kamerze i w drugiej) różni się ponad ten parametr to znaczy
                     //że to nie jest od tego samego omama tylko od różnych
    double MaxdsZpix; //maksymalne odchylenie pomiędzy pionowymi rozmiarami w pixelach (majaka)
                      // jeśli jedna kamery widzi coś dużego a druga widzi coś malego to znaczy że to nie jest ten sam omam tylko dwa różne.
                      // ten parametr określa maksymalną różnicę
                      // ponieważ oczy ustawione są obok siebie na tej samej wysokości, porównywac można rozmiar w pikselach
                      //dla  rozmiaru w poziomie nie musi to byc już prawda dlatego nie robimy tego

                    //mając dane z dwóch kamer (dwa majaki) możemy obliczyc omama (rozmiary, położenie, itp), którego one miałyby reprezentować
                    // ..omam ma też inne parametry, które wynikają np. z jego historii (np prędkość)
                    //
                    //tu określone są parametry, które służa do porównywania omamów.
                    //mamy na przykład listę omamów które były w zasięgu wcześniej, teraz widzimy nowy omam i chcemy sprawdzić
                    //czy to jest już coś widzianego wcześniej czy jakaś nowa rzecz
                    //jeśli parametry dwóch porównywanych omamów różnią się ponad poniższe wskaźniki to znaczy że to nie jest ten sam omam
                    //
    double MaxdsX;  //maksymalne odchylenie pomiędzy rozmiarami w jednostkach omama
    double MaxdsY;  //maksymalne odchylenie pomiędzy rozmiarami
    double MaxdsZ;  //maksymalne odchylenie pomiędzy rozmiarami

    double MaxdX; //maksymalne odchylenie pomiędzy pozycjami w jednostkach omama
    double MaxdY; //maksymalne odchylenie pomiędzy pozycjami
    double MaxdZ; //maksymalne odchylenie pomiędzy pozycjami


 public:
    at_oczy(at_kamera *c0,at_kamera *c1,double b);

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

};

#endif // AT_OCZY_H
