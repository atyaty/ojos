#ifndef AT_FILE_H
#define AT_FILE_H

#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

using namespace std;

class at_File
{
protected:
    string Filename;    //nazwa pliku
    string Fileext;     //rozszerzenie
    string Filepath;    //ścieżka
    string FileAct;     //nazwa pliku właściwa

    string Filetext;    //zawartosc pliku

    string getAfter(string Text,string tag); //funkcja czytająca z pliku
    string getBefore(string Text,string tag); //funkcja czytająca z pliku
    string getPole(string Text,string beg,string end); //funkcja czytająca z pliku


public:
    at_File(){}
    at_File(string P, string F, string E){Ini(P,F,E);}
    void Ini(string P, string F, string E);
    void Ini();
    void Config(string P, string F, string E){Filename=F;Filepath=P;Fileext=E;}

    string PobierzPlik();
    string PobierzPlik( string FN);

    void ZapiszPlik();
    void ZapiszPlik(string Ff);
    void ZapiszPlikBak(string Ext);
    void ZapiszPlikBak(){ZapiszPlikBak(".bak");}

    string FindName(string P,string Ff,string Ext);

    string FileNameFull(){return Filename+Fileext;}
    string FileNameBase(){return Filename;}
    string FileNameAct(){return FileAct;}
    string FileNameActFull(){return FileAct+Fileext;}
    string FileExt(){return Fileext;}
    string FilePath(){return Filepath;}
    string FileText(){return Filetext;}

    void Write(string s){AppendBuffer(s);ZapiszPlik();}
    void Append();
    void Append(string s){ AppendBuffer(s); Append();}
    void AppendBuffer(string s){Filetext+=s;}
    void AppendBufferLn(string s){Filetext+=s+"\n";}
    bool isEmpty(){ return Filetext.empty();}
    void Clear(){Filetext="";}

    string getAfter(string tag) {return getAfter(Filetext,tag);}
    string getBefore(string tag) {return getBefore(Filetext,tag);}
    string getPole(string b,string e)  {return getPole(Filetext, b, e );}

};

#endif // AT_FILE_H
