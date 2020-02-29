/*
by AT
*/

#ifndef atDeviceH
#define atDeviceH


#include <vector.h>
#include <String.h>
#include "PCCards.h"
#include "Stoper.h"
#include "Konfiguracja.h"

#include "atTrace.h"
#include "atError.h"

#include "Raport.h"
#include "at_minmax.h"


class atDevice;
typedef atDevice atDevice;

typedef void (__closure * EveFun) ();


//------------------------------------------------------------
// urządzenie składa sie z:
// - wejść
// - ze składowych (składowa to też urządzenie)
// - oraz z wyjścia
// wyjście urządzenia połączone jest z wyjsciem jednej ze składowych (pierwszej dodanej)
// wyjścia składowych moga być podłączone do wejść składowych o wyższych numerach
// wejścia urządzenia podłączone są do wejść składowych
//
//
// przetwarzanie odbywa sie w kolejnosci od składowej o najwyzszym mumerzedo najnizszego numeru
// zerowa (pierwsza dodana) jest najblizej wyjscia
//
// czyli:
// wejście danej składowej może byc podłączone tylko jednego wyjscia skladowej blizszej wejścia (wyzszy numer)
// wyjście danej składowej moze byc podłączone do kilku wejść składowych bliższych wyjscia
class atDevice {
private:
protected:
 void IniWe(int n){ nWe=n; aWe.resize(nWe,NULL); aConnDev.resize(nWe,-1); aConnWe.resize(nWe,-1);aIdWe.resize(nWe,"");  };
 string sNazwa;           //nazwa urządzenia
 string Id;
 string sType;            //typ urządzenia

 atDevice *ParentDev; //podzespół nadrzędny. Nasz Device należy do tego ParentDev i jest umieszczony w jego tablicy aParts
 int ParentPartNo;     //numer w tablicy podzespołów do której przynależy nasz objekt

 unsigned nWe;                 //ilosc wejść
 vector <atDevice *> aParts;   //składowe
 vector <atDevice *> aWe;      // .. wskaźnik tablicy wejść tego urządzenia
 vector <int> aConnDev;        // .. wskaźnik tablicy połaczeń wejść tego urządzenia - urzadzenie (dlugość aWe)
 vector <int> aConnWe;         // .. wskaźnik tablicy połaczeń wejść tego urządzenia - wejście (dlugość aWe)
 vector <string> aIdWe;        // .. wskaźnik tablicy połaczeń wejść tego urządzenia - wejście (dlugość aWe)
 void SetWeId(int i, string s){aIdWe.at(i)=s;};

 double Wy;                    //wyjście
 bool enabled;

 bool VirtualWeChecked;               //=true jeśli już wykonano spardzenie funkcją findVirtualWe
 vector <atDevice *> aVirtualWe;      // .. wskaźnik tablicy wejść wirtualnych
 void findVirtualWe(atDevice *d);     //wirtualne wejścia są na potrzeby TDevicePanel


 Raport Rap;                 //raport
 atError *Error;           //wskaźnik błędu
 void SetError(int e, string op, int ech, int buf, string he);
 void SetError(int e, int ech, string he)    { SetError(e,"",ech,NULL,he); };
 void SetError(int e,string he)              { SetError(e,"",NULL,NULL,he); };
 void SetError(atError *e)  {
	if(e!=NULL){
	  SetError(e->GetCode(),
	  e->GetOpis(),
	  e->GetChn(),e->GetBuf(),e->GetHdwOpis());
	}

 }; //moze konstruktor z parametrem atError?

 void SetParams(int n);

 //events
 bool EveStatus;    //wzkaźnik blokujący wielokrotne uruchomienie Eve
 bool EveOnUp_One;  //true oznacza wyczyszczenie Eve po jego wykonaniu
 bool EveOnDn_One;  //jw
 EveFun EveOnUp;    //przejście wyjścia(!) przez 0 w górę
 EveFun EveOnDn;    // w dół
 void GoEve();      //uruchamiacz zdarzeń


public:

 atDevice(int n)                      {SetParams(n);};
 atDevice()                           {SetParams(0);}; //część tych konstrukotorów jest do wywalenia, min ten, ale trzeba poprawić konstruktory klas podrzędnych wcześniej
 atDevice(string s)                   {SetParams(0);SetNazwa(s);};
 atDevice( int n,string s)            {SetParams(n);SetNazwa(s);};
 atDevice(string s,string id)         {SetParams(0);SetNazwa(s,id);};
 atDevice( int n,string s, string id) {SetParams(n);SetNazwa(s,id);};
 ~atDevice( ) {if(Error!=NULL) delete Error;};


 void SetRaport(AnsiString f, AnsiString p){Rap.Ini(f,p,"","");};
 void WriteRaport(AnsiString s){Rap.Write(s);};

 void SetType(string t){sType=t;}; //
 void SetId(string id){Id=id;}; //
 void SetNazwa(string s){sNazwa=s;}; //
 void SetNazwa(string s,string id){sNazwa=s;Id=id;}; //
 string GetNazwa(){ return sNazwa;}; //
 string GetId() {return Id;}; //zwraca id
 string GetType() {return sType;}; 

 unsigned SizeParts() {return aParts.size();};
 unsigned lastPartNo() {return aParts.size()-1;};
 int PartNo(string id);
 atDevice *Part(int i) {return aParts.at(i);};
 atDevice *Part(string id);
 atDevice *lastPart() {if(aParts.size()==0)return NULL; else return aParts.at(aParts.size()-1);};

 unsigned SizeWe() {return nWe;};
 int WeNo(string id);
 int WeNo(atDevice *d);
 bool WeEmpty(string id); //sprawdza czy istnieje wejście i czy jest wolne
 atDevice *We(int i) {return aWe.at(i);};
 atDevice *We(string id);
 bool IsConnected(atDevice *d); //sprawdza czy d jest podłączone do nas (uwzględnia rekurencyjnie jego Partsy)
 bool IsStrictConnected(atDevice *d); //sprawdza czy d jest podłączony do nas (tylko bezpośrednie łącze)
 bool IsItOrPart(atDevice *d); //czy d to my, czy może nasz rodzic albo dziadek?

 void findVirtualWe();
 unsigned SizeVirtualWe(){if(!VirtualWeChecked)findVirtualWe(); return aVirtualWe.size();}; //musi być wykonane find zanim zwrócimy size
 atDevice* VirtualWe(unsigned i){if(!VirtualWeChecked)findVirtualWe(); return aVirtualWe.at(i);};//jw

 double GetWy() {  return Wy; }; //zwraca wyjscie
 double GetWy(int i){  atDevice *p=Part(i);  return p->GetWy();};
 double GetWy(string id){  atDevice *p=Part(id);  return p->GetWy();};
 atError *GetError() {return Error;};

 bool connect(atDevice *d, unsigned i);   //podłącza do wejścia "i" urządzenie "d"
 bool connect(atDevice *d, string id);    //podłącza do wejścia "id" urządzenie "d"
 bool connect(atDevice *d, string idP,string idW);    //podłącza do wejścia "idW" składową idP urządzenia "d"
 bool connect(atDevice *d, string idP,unsigned nW);   // jw
 bool connect(atDevice *d, unsigned nP,unsigned nW);  // jw
 void disconnect(unsigned i){ if(i<nWe) aWe.at(i)=NULL; };

 bool addPart( atDevice *d);
 bool addPart(atDevice *d, unsigned nDev, unsigned nWe);
 bool addPart(atDevice *d, string idDev, string idWe);
 bool addPart(atDevice *d, string idDev, unsigned nWe);
 bool addWe( unsigned n, unsigned nD, unsigned nW);
 bool addWe( unsigned n, string idD, string idW);
 bool addWe( unsigned n, string idD, unsigned nW);
 bool addConnection( unsigned nD0, unsigned nD1, unsigned nW); //wewnętrzne połączenie urzadzenia  nD0 wejścia nW urządzenia nD1
 bool addConnection( string idD0, string idD1, string idW); //wewnętrzne połączenie urzadzenia  nD0 wejścia nW urządzenia nD1
 bool addConnection( string idD0, string idD1, unsigned nW); //wewnętrzne połączenie urzadzenia  nD0 wejścia nW urządzenia nD1
 bool addConnection( unsigned nD0, unsigned nP,  unsigned nD1, unsigned nW);
 bool addConnection( string idD0, string idP,  string idD1, string idW);
 bool addConnection( string idD0, string idP,  string idD1, unsigned nW);

 void GoParts(); //oblicza składowe
 void ExtFunParts(int i);


 double Go(){ Wy=GoFun(); GoEve(); return Wy;  };    //oblicza wyjscie i wstawia na zmiennną Wy
 void SetWy(double w){ Wy=w; EveStatus=(Wy>0);};
 bool Enabled(){return enabled;};
 void SetEnabled(bool e){enabled=e;};

 void SetEveOnUp(EveFun f)    {EveOnUp=f; EveOnUp_One=false;};
 void SetEveOnDn(EveFun f)    {EveOnDn=f; EveOnDn_One=false;};
 void SetEveOnUp_One(EveFun f){EveOnUp=f; EveOnUp_One=true;};
 void SetEveOnDn_One(EveFun f){EveOnDn=f; EveOnDn_One=true;};

 //parametry ustawiane na poziomie i na potrzeby DevicePanel
 bool Part_Visible; //czy widoczne jako Part w Panelu nadrzeędnym
 bool ButtSS_Visible; //czy widoczny klwaisz SS
 int EditWidth;     //szerokość pola Wy
 int EditWy_prec;   //il. c. dzies. pola Wy
 int EditWy_digits; //ilość cyfr pola Wy
 int EditWeWidth;   //szerokość pola We

 //-------------------------------------------
 // virtual
 //-------------------------------------------
 virtual double GoFun();     //oblicza wyjscie
							 //funkcja działa gdy urządzenie nie ma składowych!
							 //jesli urządzenie ma składowe to na wyjście wstawiana jest wartość wyjścia pierwszej składowej
							 // ?? czy to powyzej to jest oby dobre założenie ??
 virtual void ExtFun(int i); //zewnetrzna zmiana parametrów
 virtual string GetOpis() {return "";};   //dodatkowy opis zaleźny od klasy
 virtual void SetFun(int j){ };     //funkcje ustawień
 virtual void SetDouble(double j){ };
 virtual TColor GetColor(){return NULL;};


};

//----------------------------------------------------------
//przełącznik bistabilny. zmienia stan wyjscia w zależności od parametru funkcji zewnętrznej ExtFun
//..można go zastąpić objektem atDeviceConf
//..dodatkowo zyskując możliwość ustawiania wartości wyjściowej (załączanej) w konfiguracji
//
class atDeviceOnOff : public atDevice {
 public:
  atDeviceOnOff(string s):atDevice(s){SetType("devOnOff");};//{on=false;Go();SetNazwa(s);};
  atDeviceOnOff(string s, string id):atDevice(s,id){SetType("devOnOff");};//{on=false;Go();SetNazwa(s,id);};
  double GoFun();
  void ExtFun(int i);
};

//----------------------------------------------------------
//bramka AND. wartośc wyjścia jest funkcją AND wejśc
class atDeviceAND : public atDevice {
 public:
//  atDeviceAND(int n,string s){atDevice::atDevice(n,s);};
//  atDeviceAND(int n,string s) {IniWe(n);Error=0;SetNazwa(s);};
  atDeviceAND(int n,string s):atDevice(n,s)  {SetType("devAND");};
  atDeviceAND(int n,string s,string id):atDevice(n,s,id){SetType("devAND");};
  double GoFun();
};
//----------------------------------------------------------
// bramka OR
class atDeviceOR : public atDevice {
 public:
  atDeviceOR(int n,string s):atDevice(n,s){SetType("devOR");};//  {IniWe(n);SetNazwa(s);};
  atDeviceOR(int n,string s,string id):atDevice(n,s,id){SetType("devOR");};//  {IniWe(n);SetNazwa(s,id);};
  double GoFun();
};
//----------------------------------------------------------
class atDeviceNOR : public atDevice {
 public:
  atDeviceNOR(int n,string s):atDevice(n,s){SetType("devNOR");};//    {IniWe(n);SetNazwa(s);};
  atDeviceNOR(int n,string s,string id):atDevice(n,s,id){SetType("devNOR");};//    {IniWe(n);SetNazwa(s,id);};
  double GoFun();
};
//----------------------------------------------------------
class atDeviceNAND : public atDevice {
 public:
  atDeviceNAND(int n,string s):atDevice(n,s){SetType("devNAND");};//    {IniWe(n);SetNazwa(s);};
  atDeviceNAND(int n,string s,string id):atDevice(n,s,id){SetType("devNAND");};//    {IniWe(n);SetNazwa(s,id);};
  double GoFun();
};
//----------------------------------------------------------
//Zawsze zwraca 1 ..zawsze
class atDeviceTrue : public atDevice {
 public:
  atDeviceTrue():atDevice("true"){ SetType("devTrue");Go();  };
  atDeviceTrue(string s):atDevice(s) { SetType("devTrue");Go(); };
  atDeviceTrue(string s,string id):atDevice(s,id) { SetType("devTrue");Go(); };
  double GoFun();
};
//----------------------------------------------------------
//Zawsze zwraca 1 ..zawsze
class atDeviceFalse : public atDevice {
 public:
  atDeviceFalse():atDevice("false"){ SetType("devFalse");Go();  };
  atDeviceFalse(string s):atDevice(s) { SetType("devFalse");Go(); };
  atDeviceFalse(string s,string id):atDevice(s,id) { SetType("devFalse");Go(); };
  double GoFun();
};
//----------------------------------------------------------
//suma wejść jest wartościa wyjścia
class atDeviceSum : public atDevice {
 protected:
   void IniPP(){ EditWeWidth=50;EditWidth=50;};
 public:
  atDeviceSum(int n,string s,string id):atDevice(n,s,id){IniPP();SetType("devSum");};
  double GoFun();
};
//----------------------------------------------------------
//średnia z wejść jest wartościa wyjścia
class atDeviceAvg : public atDeviceSum {
 public:
  atDeviceAvg(int n,string s,string id) : atDeviceSum(n,s,id) {SetType("devAvg");};
  double GoFun(){return (aWe.size()<=0)?0:(atDeviceSum::GoFun()/aWe.size());};
};

//----------------------------------------------------------
//suma wejśc ze znakiem minus
class atDeviceInv : public atDevice {
 private:
   void IniPP(){ EditWidth=50;};
 public:
  atDeviceInv(int n,string s,string id):atDevice(n,s,id)  {IniPP();SetType("devInv");};
  double GoFun();
};
//----------------------------------------------------------
// różnica We0 i pozostałych
class atDeviceRoz : public atDevice {
 private:
   void IniPP(){ EditWidth=50;};
 public:
  atDeviceRoz(int n,string s,string id):atDevice(n,s,id){IniPP();SetType("devRoz");};
  double GoFun();
};

//----------------------------------------------------------
//iloczyn wejść
class atDeviceMno : public atDevice {
 private:
   void IniPP(){ EditWidth=50;};
 public:
  atDeviceMno(int n,string s,string id):atDevice(n,s,id)  {IniPP();SetType("devMno");};
  double GoFun();
};
//----------------------------------------------------------
//potęga        mul * 10^power
//we1 - wykładnik- power
//we0 - mnożnik -mul
class atDevicePot : public atDevice {
 private:
   void IniPP(){ EditWidth=50;};
 public:
  atDevicePot(string s,string id):atDevice(3,s,id)  {IniPP();SetType("devPot");};
  double GoFun();
};
//----------------------------------------------------------
//odwrotnośc iloczynu wejsc
class atDeviceOdw : public atDevice {
 private:
   void IniPP(){ EditWidth=50;};
 public:
  atDeviceOdw(int n,string s,string id):atDevice(n,s,id)  {IniPP();SetType("devOdw");};
  double GoFun();
};
//----------------------------------------------------------
// dzielenie We0 przez pozostałe
class atDeviceDzi : public atDevice {
 private:
   void IniPP(){ EditWidth=50;EditWeWidth=50;};
 public:
  atDeviceDzi(int n,string s,string id):atDevice(n,s,id)  {IniPP();SetType("devDzi");};
  double GoFun();
};
//----------------------------------------------------------
// wartość bezwzględna
class atDeviceAbs : public atDevice {
 private:
   void IniPP(){ EditWidth=50;EditWeWidth=50;};
 public:
  atDeviceAbs(string s,string id):atDevice(1,s,id)  {IniPP();SetType("devAbs");};
  double GoFun();
};

//----------------------------------------------------------
//powtarza na wyjściu wartość z we0 o ile we1!=0
//
//..zamiast (oprócz) semafora dorobic mechanizm wielu wejśc dla jednego DO lub AO
//działało by tak samo jak semafor.. przez ExtFun wybierałoby się wejście sterujące
//(..e ..byłby konflikt znaczeniowy dla ExtFun(0)..)
//
class atDeviceSemafor : public atDevice {
 public:
  atDeviceSemafor(string s,string id):atDevice(2,s,id){SetType("devSemafor");};
  double GoFun();
};
//----------------------------------------------------------
//powtarza na wyjściu wartość z we*
//ExtFun wskazuje z którego wejścia podawac wartośc
class atDeviceSem : public atDevice {
 private:
  int we_nr;
 public:
  atDeviceSem(int n,string s,string id):atDevice(n,s,id){SetType("devSem");we_nr=0; EditWidth=50; EditWeWidth=50;};
  double GoFun();
  void SetFun(int i);
  string GetOpis();
};

//----------------------------------------------------------
// stan wyjścia odpowiada odczytanemu bitowi DI
class atDeviceDI : public atDevice {
 private:
  PCBuffer *PCBuf; //Bufor DIO
  unsigned BN;        //Numer bitu w buforze
  void SetBuf(PCBuffer *buf,unsigned bn);
 public:
  atDeviceDI(PCBuffer *buf,unsigned bn):atDevice(0,"DI"){ SetBuf(buf,bn); SetType("devDI"); };
  atDeviceDI(PCBuffer *buf,unsigned bn,string s):atDevice(0,s){SetBuf(buf,bn);SetType("devDI"); };
  atDeviceDI(PCBuffer *buf,unsigned bn,string s,string id):atDevice(0,s,id){ SetBuf(buf,bn);SetType("devDI");  };
  double GoFun();
  unsigned long BufferNumber(){return PCBuf->BufferNumber();};
  string GetOpis();
  TColor GetColor();

};
//----------------------------------------------------------
class atDeviceDIBlock : public atDevice {
 private:
  PCBuffer *PCBuf; //Bufor DIO
 public:
  atDeviceDIBlock(PCBuffer *buf);
  ~atDeviceDIBlock();
  AnsiString BufferNo(){return PCBuf->BufferNo();};
//  unsigned long BufferNumber(){return PCBuf->BufferNumber();};
//  string GetOpis();
};
//----------------------------------------------------------
// bit DO ustawiany jest zgodnie w wartościa wejścia
class atDeviceDO : public atDevice {
 private:
  PCBuffer *PCBuf; //Bufor DIO
  unsigned BN;        //Numer bitu w buforze
  void SetBuf(PCBuffer *buf,unsigned bn);
 public:
  atDeviceDO(PCBuffer *buf,unsigned bn){ IniWe(1);SetBuf(buf,bn); SetType("devDO"); };
  atDeviceDO(PCBuffer *buf,unsigned bn,string s){ IniWe(1);SetBuf(buf,bn);SetNazwa(s); SetType("devDO"); } ;
  atDeviceDO(PCBuffer *buf,unsigned bn,string s,string id){ IniWe(1);SetBuf(buf,bn);SetNazwa(s,id); SetType("devDO"); } ;
  double GoFun();
  unsigned long BufferNumber(){return PCBuf->BufferNumber();};
  string GetOpis();
  TColor GetColor();
};

//----------------------------------------------------------
class atDeviceDOBlock : public atDevice {
 private:
  PCBuffer *PCBuf; //Bufor DIO
 public:
  atDeviceDOBlock(PCBuffer *buf);
  ~atDeviceDOBlock();
  unsigned long BufferNumber(){return PCBuf->BufferNumber();};
};
//----------------------------------------------------------
//działa na funkcję zwewnętrzną, tak jak Flip, ale jednocześnie przestawia podłączone DO
//
/*
class atDeviceDOSwitch : public atDevice {
 private:
  bool on;
  PCBuffer *PCBuf; //Bufor DIO
  unsigned BN;        //Numer bitu w buforze
  void IniPP(){ButtSS_Visible=true;};

 public:
  atDeviceDOSwitch(PCBuffer *buf,unsigned bn){IniPP(); on=false; IniWe(0); PCBuf=buf; BN=bn;  };
  atDeviceDOSwitch(PCBuffer *buf,unsigned bn,string s){IniPP();	on=false; IniWe(0);  PCBuf=buf;  BN=bn;  SetNazwa(s);  };
  atDeviceDOSwitch(PCBuffer *buf,unsigned bn,string s,string id){IniPP();	on=false; IniWe(0);  PCBuf=buf;  BN=bn;  SetNazwa(s,id);  };
  double GoFun();
  void ExtFun(int i);
  void Flip(){on=!on;Go();};
  unsigned long BufferNumber(){return PCBuf->BufferNumber();};
};
*/
/* już jej nie lubimy bo nie chce mi się poprawać PCBuf->AddDevice
//----------------------------------------------------------
//działa na funkcję zwewnętrzną, tak jak Flip, ale jednocześnie przestawia podłączone DO
//
class atDeviceDOOnOff : public atDevice {
 private:
  bool on;
  PCDIOBuffer *PCBuf; //Bufor DIO
  unsigned BN;        //Numer bitu w buforze
  void IniPP(){ButtSS_Visible=true;};

 public:
  atDeviceDOOnOff(PCDIOBuffer *buf,unsigned bn){IniPP(); on=false; IniWe(0); PCBuf=buf; BN=bn;  };
  atDeviceDOOnOff(PCDIOBuffer *buf,unsigned bn,string s){IniPP();	on=false; IniWe(0);  PCBuf=buf;  BN=bn;  SetNazwa(s);  };
  atDeviceDOOnOff(PCDIOBuffer *buf,unsigned bn,string s,string id){IniPP();	on=false; IniWe(0);  PCBuf=buf;  BN=bn;  SetNazwa(s,id);  };
  double GoFun();
  void ExtFun(int i);
  unsigned long BufferNumber(){return PCBuf->BufferNumber();};
};
*/
//----------------------------------------------------------
/*class atDeviceDOSwitchBlock : public atDevice {
 private:
  PCBuffer *PCBuf; //Bufor DIO
 public:
  atDeviceDOSwitchBlock(PCBuffer *buf);
  ~atDeviceDOSwitchBlock();
  unsigned long BufferNumber(){return PCBuf->BufferNumber();};
};
*/

//----------------------------------------------------------
class atDeviceAD : public atDevice {
 private:
  PCBuffer *PCBuf; //Bufor AD
  void IniPP(){ EditWidth=50;};
  void SetBuf(PCBuffer *buf);
 public:
  atDeviceAD(PCBuffer *buf,string s):atDevice(s){               IniPP(); IniWe(0); SetBuf(buf); Go(); SetType("devAD"); };
  atDeviceAD(PCBuffer *buf,string s,string id):atDevice(s,id){  IniPP(); IniWe(0); SetBuf(buf); Go(); SetType("devAD"); };
  double GoFun();
  string GetOpis();
  TColor GetColor();
 // unsigned long ChannelNumber(){return PCBuf->ChannelNumber();};
};



//----------------------------------------------------------
class atDeviceTara : public atDevice {
 private:
  double tara;
 public:
  atDeviceTara(string s,string id);
  double GoFun();
  void SetFun(int i);
};
//----------------------------------------------------------
class atDeviceCalc : public atDevice {
 private:
  atTrace *Tr;
 public:
  atDeviceCalc(string tr,string s,string id);
  atDeviceCalc(string tr,string kX,string kY,string s,string id);
  ~atDeviceCalc();
  double GoFun();
  atTrace *Trace(){return Tr;};
  void SetDev();
  string GetOpis();
  
};
/*  już jej nie lubię
//----------------------------------------------------------
class atDeviceADCalc : public atDevice {
 private:
  PCAIBuffer *PCBuf; //Bufor AD
  atTrace *Tr;
  void IniPP(){ EditWidth=50;};

 public:
  atDeviceADCalc(PCAIBuffer *buf,string tr,string s,string id);
  ~atDeviceADCalc();
  double GoFun();
 // unsigned long ChannelNumber(){return PCBuf->ChannelNumber();};
  atTrace *Trace(){return Tr;};
  void SetDev(string tr);

};
  */
//----------------------------------------------------------
class atDeviceADBlock : public atDevice {
 private:
  PCAIBuffer **PCBuf; //tablica wskaźników do buforów AD
 public:
  atDeviceADBlock(PCCard *pcc);
  ~atDeviceADBlock();
};
//----------------------------------------------------------
//na wyjsciu ustawiana jest wartośc zmiennej do której wskaźnik mamy w Buf
class atDeviceInput : public atDevice {
 private:
  double *Buf; //zmienna do odczytania
  void IniPP(){ IniWe(0);EditWidth=50;EditWeWidth=50;};
 public:
  atDeviceInput(double *b){IniPP(); Buf=b; Go(); SetType("devInput"); };
  atDeviceInput(double *b,string s){IniPP(); Buf=b; Go(); SetNazwa(s); SetType("devInput");  };
  atDeviceInput(double *b,string s,string id){IniPP(); Buf=b; Go(); SetNazwa(s,id); SetType("devInput"); };
  double GoFun() {return enabled?*Buf:0.;};
};

//--------------------//
//   Math             //
//--------------------//
//----------------------------------------------------------
//  nieprzetestowana...
class atDeviceMathSrednia : public atDevice {
 private:
  double sum,sred;
  int N;
  int Nmax;
  bool en,ready;
 public:
  atDeviceMathSrednia(int nmax,string s,string id)  {IniWe(1); en=false; ready=false; Nmax=nmax; sred=0.;sum=0.;N=0;SetNazwa(s,id);};
  double GoFun();
  void ExtFun(int i);
  bool Ready(){return ready;};
  double TakeRes() {ready=false; return sred; };
};
//----------------------------------------------------------
//
class atDeviceMathMax : public atDevice {
 private:
  double max;
  bool started;
 public:
  atDeviceMathMax(string s,string id) : atDevice(1,s,id) {max=0;started=false;};//  {IniWe(1); SetNazwa(s,id);};
  double GoFun();
  void ExtFun(int i);
};
//----------------------------------------------------------
//
class atDeviceMathMin : public atDevice {
 private:
  double min;
  bool started;
 public:
  atDeviceMathMin(string s,string id) : atDevice(1,s,id){min=0;started=false;};
  double GoFun();
  void ExtFun(int i);
};

//----------------------------------------------------------
// Detektor przełamania trendu
// sprawdzamy czy sygnal wejsciowy zachowuje sie "trendy" :) znaczy czy jest monotoniczny
// t0-poprzedni odczyt zapamiętany, t1-następny odczyt zapamietany
// pomiar rozpoczyna się gdy stat ustawiony zostanie na 1 zewnętrzną funkcją
// od tego momeentu sprawdzamy czy przyrost sygnalu nastąpił w tym samym kierunku co poprzednio (trend_old, trend_new)
// zliczamy na n ilość przyrostów w tym samym kierunku (zerowe przyrosty sa pomijane)
// jeśli zmiana trendu następuje wcześniej wcześniej niz po 3 zliczeniach to uznajemy że
// poprzedni trend byl fałszywy i zaczynamy liczyc od nowa w odwrotnym kierunku
// jesli zmiana trendu nastąpiła później niz po 3 zliczeniach zmieniamy status na 0
// ..co oznacza złamanie trendu.
class atDeviceBreakDetector : public atDevice {
 private:
  double t0,t1;//kolejne pomiary zapamiętane
  int stat;    //0-zatrztmane, 1-przygotowanie, 2-pomiar
  int n;       //ilosć pomiarów zgodnych z trendem
 public:
  atDeviceBreakDetector(string s,string id):atDevice(1,s,id){t0=0.;t1=0.;stat=0;n=0;SetType("devBreakDetector");};
  double GoFun();
  void ExtFun(int i);
};

//----------------------------------------------------------
// DA
class atDeviceDA : public atDevice {
 private:
  PCBuffer *PCBuf;   //Bufor DA
  void SetBuf(PCBuffer *buf);
 public:
  atDeviceDA(PCBuffer *buf){                    IniWe(1);SetBuf(buf);SetType("devDA");  };
  atDeviceDA(PCBuffer *buf,string s){           IniWe(1);SetBuf(buf);SetNazwa(s);SetType("devDA");  } ;
  atDeviceDA(PCBuffer *buf,string s,string id){ IniWe(1);SetBuf(buf);SetNazwa(s,id); SetType("devDA"); } ;
  double GoFun();
  unsigned long BufferNumber(){return PCBuf->BufferNumber();};
  string GetOpis();
  TColor GetColor();
  
};

//----------------------------------------------------------
class atDeviceDABlock : public atDevice {
 private:
  PCBuffer **PCBuf; //tablica wskaźników do buforów DA
 public:
  atDeviceDABlock(PCCard *pcc);
  ~atDeviceDABlock();
};
//----------------------------------------------------------
//urządzenie zadaną ilość kroków i przechodzi w stan spoczynku (licznik)
//może być inicjowany przez SetFin - wtedy na parametrze podajemy ilośc kroków do odliczenia
//może być inicjowany wejściem, wtedy ilość kroków podajemy w konstruktorze, a inicjacja jest zoczem dodatnim wejścia
class atDeviceKroki : public atDevice {
 private:
  int Kroki;
  int K;   //ilość kroków jaka ma być ustawiona, podczas inicjacji wejściem
  double we_last; //poprzedni stan wejścia, w konstruktorze inicjujemy jako 1 żeby nie uruchamiało sie na starcie
 public:
  atDeviceKroki(string s,string id):atDevice(s,id){Kroki=0;SetType("devKroki");};
  atDeviceKroki(int n,string s,string id):atDevice(s,id){IniWe(1); we_last=1; Kroki=0; K=n;SetType("devKroki");};
  double GoFun();
  void SetFun(int i);

};



#endif
