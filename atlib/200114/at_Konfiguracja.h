/*
by AT
*/

#ifndef at_Konfiguracja_H
#define at_Konfiguracja_H

#include <string>
#include <stdexcept>
#include "at_File.h"
#include "atErrors.h"


class at_Konfiguracja :public at_File {
private:

   //XML-odczyt
   string getTag(string Text,string NazwaPola);
   string Odczytaj(string Text,string NazwaPola);
   double OdczytajFloat(string Text,string NazwaPola);
   double OdczytajFloat(string Text,string NazwaPola,int n);
   int OdczytajInt(string Text,string NazwaPola);
   int OdczytajInt(string Text,string NazwaPola,int n);
   int OdczytajHexInt(string Text,string NazwaPola);

   int PoliczFields(string Tekst);
   string OdczytajField(string Tekst,int n); //odczytaj n-te pole
   string OdczytajFieldNaz(string Tekst,int n);
   int PoliczSubFields(string Tekst,string NazwaPola){  return PoliczFields( getTag(Tekst,NazwaPola) ); }
   string OdczytajSubField(string Tekst,string NazwaPola,int n){  return OdczytajField(getTag(Tekst,NazwaPola),n); }
   string OdczytajSubFieldNaz(string Tekst,string NazwaPola,int n){  return OdczytajFieldNaz(getTag(Tekst,NazwaPola),n); }

   //XML-zapis
   string setPole(string Text,string beg,string end,string Value);
   string setTag(string Text,string NazwaPola,string Value);
   string Zapisz(string Text,string NazwaPola,string Value);
   string Zapisz(string Text,string NazwaPola,double Value);
   string Zapisz(string Text,string NazwaPola,int Value);

   int Error;


public:
   at_Konfiguracja(string Pp,string Ff,string Ee){ Ini(Pp,Ff,Ee);}
   at_Konfiguracja(string Pp,string Ff){ Ini(Pp,Ff,"");}
   at_Konfiguracja(string Ff){ Ini("",Ff,"");}

   bool Ini(string Pp,string Ff,string Ee);

   //XML-odczyt
   string getTag(string NazwaPola)       {return Odczytaj(Filetext, NazwaPola);}
   double getFloat(string NazwaPola)      {return OdczytajFloat(Filetext, NazwaPola);}
   double getFloat(string NazwaPola,int n){return OdczytajFloat(Filetext, NazwaPola,n);}
   int getInt(string NazwaPola)           {return OdczytajInt(Filetext, NazwaPola);}
   int getInt(string NazwaPola, int n)    {return OdczytajInt(Filetext, NazwaPola,n);}
   int getHexInt(string NazwaPola)        {return OdczytajHexInt(Filetext, NazwaPola);}

   int getPola(string pole)               {return PoliczSubFields(Filetext,pole);}
   int getPola()                           {return PoliczFields(Filetext);}
   string getTag(string pole,int n)   {return OdczytajSubField(Filetext,pole,n);}
   string getTag(int n)                {return OdczytajField(Filetext,n);}
   string getTagNaz(string pole,int n){return OdczytajSubFieldNaz(Filetext,pole,n);}
   string getTagNaz(int n)             {return OdczytajFieldNaz(Filetext,n);}

   //XML-zapis
   void setTag(string NazwaPola,string Value){Filetext = Zapisz(Filetext,NazwaPola,Value);}
   void setFloat(string NazwaPola,double Value) {Filetext = Zapisz(Filetext,NazwaPola,Value);}
   void setInt(string NazwaPola,int Value)    {Filetext = Zapisz(Filetext,NazwaPola,Value);}

   int getError(){return Error;}
   void ResetError(){Error=0;}

};

#endif
