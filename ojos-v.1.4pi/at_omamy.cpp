#include "at_omamy.h"

//-------------------------------------------------------------------
// to jest metoda najbardziej zewnętrzna
// dostaje aktualny (zwektoryzowany) obraz z oczu, czyli dwie tablice majaków, z dwóch kamer i na tej podstawie ma zaktualizować tablicę trójwymiarowych omamów
// (dostaje także parametr oczy.. można by sobie wyobrazić że parametr ten jest raz na zawsze wpisany do tego obiektu na etapie jego konstrukcji,
//  ale to obecne podejście pozwoli,mam nadzieję, pracować z wieloma parami "oczu", a może i innych "zmysłów"..)
//
// W najbardziej prymitywnym podejściu metoda wykonuje podwójną pętlę po majakach,
// sprawdzając majaki "każdy z każdym" próbuje wykalkulować czy dana para odpowiada jakiemuś realnemu obiektowi przestrzennemu -TryCreate
//
//---------
// Rozwój:
// jeśli tablice majaków będą krótkie, oraz tablica omamów będzie krótka
// czyli ogólnie słaba widoczność
// można wdrożyć procedurę porównywania majaków z omamami na liście ..praca na niepełnych danych
//
// porównywanie z omamami może też być przydatne w przypadku majaków bliskich, takich które tracone sa z pola widzenia którejś kamery
// ale to może nie koniecznie w tej funkcji - raczej w jakiejś od obsługi "krytycznych" omamów
//
// rozważyć funkcję konsolidowania omamów, która budowałaby z poszczególnycj elementów obrazu, np. krawędzi
// bardziej złożone struktury
// funkcja taka powinna analizować listę omamów pod kątem ich historycznego zachowania,np. jednakowe prędkości - liniowe.. kątowe
//
void at_omamy::Add( QVector<at_majak>* m0, QVector<at_majak>* m1, at_oczy* oczy ){
    for( int i = 0; i < m0->size(); i++ )    {
        at_majak mm0=m0->at(i);                      //majak z jednego oka do porównania
        for( int j = 0; j < m1->size(); j++ )    {
            at_majak mm1=m1->at(j);                  //majak z drugiego oka do porównania
            if(TryCreate(&mm0, &mm1, oczy)) break;   //próba wykreowania omama, jeśli się udało to koniec pętli
         }
    }
    Uporzadkuj();
}

//-------------------------------------------------------------------
// ta metoda mając dwa konkretne majaki próbuje skonstruować z nich trójwymiarowy omam
// jeśli utworzony na peróbę omam jest niespójny, czyli dane majaki nie pochodzą od tego samego obiektu przestrzennego kończymy procedurę
//
// "spójny" omam porównujemy z istniejącą tablicą omamów (robi to Dodaj)
// chodzi o sprawdzenie czy już wcześniej nie widzieliśmy tego samego, jeśli tak, to obiekt nie jest dodawany do tablicy po raz kolejny
// tylko tamten stary jest aktualizowany
// Dodaj zwraca albo pozycję tego nowego (jeśli został dodany) albo starego (jeśli dodanie nie nastąpiło)
//
// Uwaga! przy tworzeniu omama powinna być robiona statystyka odnośnie majaków, czyli tak na prawdę działania kamer
// należy sprawdzać która z kamer daje więcej majaków odrzuconych i odpowiednio potem regulować parametry jednej z nich
// ewentualnie mozna sterować oboma kamerami jednoczesnie żeby zwiększac szybkość (mniej majaków) lub dokładność (więcej majaków)
bool  at_omamy::TryCreate(at_majak *mm0, at_majak *mm1, at_oczy* oczy){
    if(mm0->Ksztalt()!=mm1->Ksztalt()) return false; //jeśli to jest inny kształt to nawet nie próbujemy tworzyc omama
    at_omam* o1=new at_omam(mm0,mm1,oczy);           //tworzymy nowy omam (na próbę), jesli bedzie z nim wszystko OK to go dodamy do listy, jesli ne to usuniemy
    if( !o1->Ok() ){ delete o1; return false; }      //jeżeli omam jest niespójny to usuwamy
    if( !Dodaj(o1) ) delete o1;                      //wywalamy go jeśli nie było dodania a tylko update (jakiegoś juz istniejącego)
    return true;
}
//-------------------------------------------------------------------
// próba dodania omama do tablicy,
// tablica jest uporządkowana według wagi (istotności) omama. najważniejsze omamy sa na początku tablicy
// porównujemy nasz nowy omam w pętli z kolejnymi na liście obiektami (metoda Ten_sam)
// jeśli znaleziony zostanie na liście odpowiednik naszego, to metoda Popraw uaktualni tylko poprzedni wpis
//zwrócona zostanie pozycja poprawionego omama lub pozycja nowego czyli koniec listy
bool at_omamy::Dodaj(at_omam* o1){
    for(int i=0; i<omamy.size();i++){
        if( omamy.at(i)->Ten_sam( o1 ) ) { omamy.at(i)->Popraw( o1 ); return false; } // jeżeli omam już jest na liście to go tylko updatujemy, uwaga ważne który jest stary a który nowy, kowy zawsze powinien być jako parametr zaróœno w Popraw jak i w Ten_sam!
    }
    o1->setId(getId()); //nuerujemy go własnym Id,oraz aktualną pozycję na liście
    omamy.push_back(o1);
    return true;
}

//-------------------------------------------------------------------------------------
//uwaga, to nie metoda, to zwykła funkcja!  ..która nie ma dostępu do zmiennych klasy at_omamy :(
bool omamy_erase(at_omam* o){   return  (o->getTlast()+3000) < QDateTime::currentMSecsSinceEpoch(); }
//-------------------------------------------------------------------------------------
int at_omamy::Uporzadkuj(){
    //bool omamy_erase=[](at_omam* o){   return  (o->getTlast()+5000) < QDateTime::currentMSecsSinceEpoch(); };
    omamy.erase( std::remove_if(omamy.begin(), omamy.end(), omamy_erase), omamy.end());
    return 0;
}
