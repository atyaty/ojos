#include "at_File.h"


//--------------------------------------------------------
//Metody do operacji na pliku
//--------------------------------------------------------
// inicjuje nazwę pliku
void at_File::Ini(string P,string F,string E){
    Filename = F;
    Fileext = E;
    Filepath = P;
    Filetext = "";
    FileAct=FindName(Filepath,Filename,Fileext);
}
//--------------------------------------------------------
// inicjuje nazwę pliku
void at_File::Ini(){    FileAct=FindName(Filepath,Filename,Fileext); }
//--------------------------------------------------------
// wczytuje do bufora z pliku o wskazanej nazwie
string at_File::PobierzPlik(string FN){
    //char t[FILENAME_MAX];
    //char * pat=getcwd(t,sizeof(t));

    FILE *f=fopen(FN.c_str(),"rb");
    if(f==NULL) return "";
    fseek(f,0,SEEK_END);
    long lSize=ftell(f);
    rewind(f);
    char *buffer = new char[lSize+1];
    long result=fread(buffer,1,lSize,f);
    if(result!=lSize) return "";
    buffer[lSize]=0;

    Filetext=buffer;
    delete[] buffer;
    fclose(f);
    return Filetext;

}
//--------------------------------------------------------
//wczytuje do bufora z plku o nawie podanej w parametrach inicjacji
string at_File::PobierzPlik(){
    string s=Filepath;
    if(s.length() && s.substr(s.length()-1,1)!="/")s+="/";
    s+=Filename+Fileext;
    return PobierzPlik(s);
}
//--------------------------------------------------------
//zapisuje plik pod wskazaną nazwą
void at_File::ZapiszPlik(string FN){
    FILE *f=fopen(FN.c_str(),"wb");
    if(f==NULL) return ;
    fwrite( Filetext.c_str(), sizeof(char),Filetext.length(),f);
    fclose(f);
    return ;
}
//--------------------------------------------------------
//zapisuje plik pod nazwą rozszerzoną
void at_File::ZapiszPlik(){
    string s=Filepath;
    if(s.substr(s.length()-1,1)!="/")s+="/";
    s+=Filename+Fileext;
    ZapiszPlik(s);
    Filetext = "";
}
//--------------------------------------------------------
//zapisuje plik pod nazwą rozszerzoną
void at_File::Append(){
    FILE *f=fopen(FileAct.c_str(),"a");
    if(f==NULL) return ;
    fwrite( Filetext.c_str(), sizeof(char),Filetext.length(),f);
    fclose(f);
    Filetext="";
    return ;
}
//--------------------------------------------------------
//zapisuje plik pod nazwą parametrową w trybie "bak"
void at_File::ZapiszPlikBak(string Ext){
    string sBak=Filepath;
    if(sBak.length() && sBak.substr(sBak.length()-1,1)!="/")sBak+="/";
    sBak+=Filename+Ext;
    remove(sBak.c_str());

    string sName=Filepath;
    if(sName.length() && sName.substr(sName.length()-1,1)!="/")sName+="/";
    sName+=Filename+Fileext;
    int result = rename(sName.c_str(),sBak.c_str());
    if(result!=0) return;

    ZapiszPlik( sName.c_str() );
}
//--------------------------------------------------------
//znajduje nową nazwę dla pliku
string at_File::FindName(string P,string F,string Ext){
   int i=1;
   while(true){
    string s=P;
    if(s.length() && s.substr(s.length()-1,1)!="/")s+="/";
    s+=(F+"_"+to_string(i++)+Ext);
    FILE *f=fopen(s.c_str(),"rb");
    if(f==NULL) return s;
    fclose(f);
   }
   return "";
}
//---------------------------------------------------------------------
// prymitywna funkcja czytania
string at_File::getAfter(string s,string tag){
    size_t i=s.find(tag);
    if(i==string::npos){ return "";}
    return s.substr(i+tag.length());
}
//---------------------------------------------------------------------
// prymitywna funkcja czytania
string at_File::getBefore(string s,string tag){
    size_t i=s.find(tag);
    if(i==string::npos){ return "";}
    return s.substr(0,i);
}
//---------------------------------------------------------------------
// prymitywna funkcja czytania
string at_File::getPole(string s,string beg, string end){
    size_t i=s.find(beg);
    if(i==string::npos){ return "";}
    s=s.substr(i+beg.length());
    i=s.find(end);
    if(i==string::npos){ return "";}
    return s.substr(0,i);
}
