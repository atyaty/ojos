/*
by AT
*/
#include "at_Konfiguracja.h"



//--------------------------------------------------------
//Metody do operacji na pliku
//--------------------------------------------------------
bool at_Konfiguracja::Ini(string Pp,string Ff,string Ee){
   ResetError();
   at_File::Ini(Pp,Ff,Ee);
   PobierzPlik();
   return !Filetext.empty();
}



//---------------------------------------------------------------------
string at_Konfiguracja::getTag(string s,string tag){
    return getPole(s,"<"+tag+">","</"+tag+">");
}

//---------------------------------------------------------------------
string at_Konfiguracja::Odczytaj(string Text,string NazwaPola)
{
  string SubText;
  //int pos = NazwaPola.  //find(".");
  size_t pos=NazwaPola.find(".");
  if(pos!=string::npos){
      SubText=getTag(Text,NazwaPola.substr(0,pos));
      return Odczytaj(SubText,NazwaPola.substr(pos+1));
  }
  return getTag(Text,NazwaPola);
}
//---------------------------------------------------------------------
double at_Konfiguracja::OdczytajFloat(string Text,string NazwaPola)
{
  ResetError();
  string sub_text=Odczytaj(Text,NazwaPola);
  double ret_war=0;
  try{ ret_war=stod(sub_text); }
  catch(const std::invalid_argument& ia){  Error=at_Konfiguracja_ERROR_double_format; }
  return ret_war;
}
//---------------------------------------------------------------------
int at_Konfiguracja::OdczytajInt(string Text,string NazwaPola){
    ResetError();
    string sub_text=Odczytaj(Text,NazwaPola);
    int ret_war=0;
    try{ ret_war =stoi(sub_text);}
    catch(const invalid_argument& ia){ Error=at_Konfiguracja_ERROR_int_format; }
    return ret_war;
}
//---------------------------------------------------------------------
int at_Konfiguracja::OdczytajHexInt(string Text,string NazwaPola){
    ResetError();
    string sub_text=Odczytaj(Text,NazwaPola);
    int ret_war=0;
    try{ ret_war =stoul(sub_text,nullptr,16);}
    catch(const invalid_argument& ia){ Error=at_Konfiguracja_ERROR_hexint_format; }
    return ret_war;
}
//---------------------------------------------------------------------
double at_Konfiguracja::OdczytajFloat(string Text,string NazwaPola,int n)
{
  ResetError();
  string sub_text=OdczytajSubField(Text,NazwaPola,n);
  double ret_war=0;
  try{ ret_war=stod(sub_text); }
  catch(const std::invalid_argument& ia){  Error=at_Konfiguracja_ERROR_double_format; }
  return ret_war;
}
//---------------------------------------------------------------------
int at_Konfiguracja::OdczytajInt(string Text,string NazwaPola,int n)
{
    ResetError();
    string sub_text=OdczytajSubField(Text,NazwaPola,n);
    int ret_war=0;
    try{ ret_war =stoi(sub_text);}
    catch(const invalid_argument& ia){ Error=at_Konfiguracja_ERROR_int_format; }
    return ret_war;
}
//---------------------------------------------------------------------




//---------------------------------------------------------------------
//XML
//---------------------------------------------------------------------
string at_Konfiguracja::setPole(string Tekst,string beg, string end, string Value ){
    //string sub_text = "";
    size_t pos0= Tekst.find(beg);
    size_t pos1= Tekst.find(end);
    if(pos0!=string::npos && pos1!=string::npos && pos1>pos0){
        return Tekst.substr(0,pos0+beg.length()) + Value + Tekst.substr(pos1);
    }
    else{
        return Tekst+"\n"+beg+Value+end;
    }
}
//---------------------------------------------------------------------
string at_Konfiguracja::setTag(string Tekst,string NazwaPola,string Value){
    return setPole(Tekst,"<"+NazwaPola+">","</"+NazwaPola+">",Value);
}
//---------------------------------------------------------------------
string at_Konfiguracja::Zapisz(string Text,string NazwaPola,string Value){
  size_t pos = NazwaPola.find(".");
  if( pos!=string::npos ){
      string N1 = NazwaPola.substr(0,pos);
      string SubText=getTag(Text,N1);
      string N2 = NazwaPola.substr(pos+1);
      return setTag(Text,N1,Zapisz(SubText,N2,Value));
  }
  return setTag(Text,NazwaPola,Value);
}
//---------------------------------------------------------------------
string at_Konfiguracja::Zapisz(string Text,string NazwaPola,double Value){
  return Zapisz(Text,NazwaPola,to_string(Value));
}
//---------------------------------------------------------------------
string at_Konfiguracja::Zapisz(string Text,string NazwaPola,int Value){
  return Zapisz(Text,NazwaPola,to_string(Value));
}




//---------------------------------------------------------------------
//funkcje do zliczania pól
//---------------------------------------------------------------------
int at_Konfiguracja::PoliczFields(string Tekst){
  string NazwaPola=getPole(Tekst,"<",">");
  if(NazwaPola=="") return 0;
  return 1 + PoliczFields( getAfter(Tekst,"</"+NazwaPola+">") );
}
//---------------------------------------------------------------------
//odczytaj pole o podanym numerze
string at_Konfiguracja::OdczytajField(string Tekst,int n){
  string NazwaPola=getPole(Tekst,"<",">");
  if(NazwaPola=="") return "";
  if( n>0) return OdczytajField(getAfter(Tekst,"</"+NazwaPola+">"),n-1);
  return getTag(Tekst,NazwaPola);
}
//---------------------------------------------------------------------
//odczytaj nazwę pola o podanym numerze
string at_Konfiguracja::OdczytajFieldNaz(string Tekst,int n){
  string NazwaPola=getPole(Tekst,"<",">");
  if(NazwaPola=="") return "";
  if( getAfter(Tekst,"<"+NazwaPola+">").find("</"+NazwaPola+">")==string::npos ) return "";
  if( n>0) return OdczytajFieldNaz(getAfter(Tekst,"</"+NazwaPola+">"),n-1);
  else return NazwaPola;
}




